#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cuda_runtime.h>

#define CUDA_CHECK(err) \
    do { \
        cudaError_t err_ = (err); \
        if (err_ != cudaSuccess) { \
            fprintf(stderr, "CUDA Error at %s:%d - %s\n", \
                    __FILE__, __LINE__, cudaGetErrorString(err_)); \
            exit(1); \
        } \
    } while(0)

// ============================================================
// Version 1: Global Memory Only Matrix Transpose
// ============================================================
// Simple transpose: each thread reads one element and writes
// to the transposed position in global memory.
// Coalesced reads, but non-coalesced writes → low bandwidth.
__global__ void transpose_global(float *out, const float *in, int N) {
    int bx = blockIdx.x * blockDim.x;
    int by = blockIdx.y * blockDim.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by + ty;
    int col = bx + tx;

    if (row < N && col < N) {
        // in[row][col] → out[col][row]
        out[col * N + row] = in[row * N + col];
    }
}

// ============================================================
// Version 2: Shared Memory Matrix Transpose (with bank conflicts)
// ============================================================
// Each block loads a tile into shared memory, then writes
// the transposed tile out.
// Shared memory read uses tile[tx][ty] (column-major read)
// → bank conflicts when TILE_DIM is a multiple of 32.
#define TILE_DIM 32

__global__ void transpose_shared(float *out, const float *in, int N) {
    __shared__ float tile[TILE_DIM][TILE_DIM];

    int bx = blockIdx.x * blockDim.x;
    int by = blockIdx.y * blockDim.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row_in  = by + ty;
    int col_in  = bx + tx;

    // Coalesced read from global memory into shared memory
    // tile[ty][tx] = in[row_in][col_in] — row-major write → no bank conflict
    if (row_in < N && col_in < N) {
        tile[ty][tx] = in[row_in * N + col_in];
    }
    __syncthreads();

    // Write transposed — read tile[tx][ty] (column-major)
    // tile[tx][ty] was set by thread(ty,tx) to in[by+ty][bx+tx]
    // So tile[tx][ty] == in[by+tx][bx+ty]
    // Output position: (bx+ty, by+tx)
    int row_out = bx + ty;
    int col_out = by + tx;

    if (row_out < N && col_out < N) {
        // Bank conflict: threads in a warp read tile[tx][ty]
        // consecutive tx with same ty → stride by TILE_DIM
        out[row_out * N + col_out] = tile[tx][ty];
    }
}

// ============================================================
// Version 3: Optimized Shared Memory (no bank conflicts)
// ============================================================
// Same algorithm but with one extra column in shared memory
// TILE_DIM+1 is odd → breaks bank conflict pattern on V100 (32 banks)
__global__ void transpose_shared_opt(float *out, const float *in, int N) {
    __shared__ float tile[TILE_DIM][TILE_DIM + 1];

    int bx = blockIdx.x * blockDim.x;
    int by = blockIdx.y * blockDim.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row_in  = by + ty;
    int col_in  = bx + tx;

    // Coalesced read from global memory into shared memory
    if (row_in < N && col_in < N) {
        tile[ty][tx] = in[row_in * N + col_in];
    }
    __syncthreads();

    // Write transposed — read tile[tx][ty] with padded stride
    // (TILE_DIM+1) breaks bank alignment → no bank conflicts
    int row_out = bx + ty;
    int col_out = by + tx;

    if (row_out < N && col_out < N) {
        out[row_out * N + col_out] = tile[tx][ty];
    }
}

// ============================================================
// CPU transpose (ground truth)
// ============================================================
void transpose_cpu(float *out, const float *in, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            out[j * N + i] = in[i * N + j];
}

// ============================================================
// Verify GPU result against CPU result
// ============================================================
int verify(const float *gpu, const float *cpu, int N, float *max_err) {
    *max_err = 0.0f;
    for (int i = 0; i < N * N; i++) {
        float err = fabsf(gpu[i] - cpu[i]);
        if (err > *max_err) *max_err = err;
    }
    return (*max_err < 1e-4f);
}

// ============================================================
// Benchmark a kernel
// ============================================================
float bench(void (*kernel)(float*, const float*, int),
            dim3 grid, dim3 block, float *d_out, const float *d_in, int N,
            const char *name) {
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    // Warm-up
    kernel<<<grid, block>>>(d_out, d_in, N);
    CUDA_CHECK(cudaDeviceSynchronize());

    // Timed run
    CUDA_CHECK(cudaEventRecord(start));
    kernel<<<grid, block>>>(d_out, d_in, N);
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));

    float ms = 0;
    CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));

    printf("  %-28s: %8.4f ms", name, ms);
    return ms;
}

// ============================================================
// ============================================================
// Print matrix (for small demo matrices)
// ============================================================
void print_matrix(const char *label, const float *mat, int N) {
    printf("%s:\n", label);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.1f ", mat[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// ============================================================
int main(int argc, char *argv[]) {
    int N = 1024;
    int BD = 16;   // block dimension
    int demo = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--demo") == 0) {
            demo = 1;
        } else if (demo == 0 && i == 1) {
            N = atoi(argv[i]);
        } else if (demo == 0 && i == 2) {
            BD = atoi(argv[i]);
        }
    }

    if (demo) {
        N = 6;      // small matrix for demonstration
        BD = 2;     // small block to show tiling
    }

    printf("========================================\n");
    printf("CUDA Matrix Transpose Benchmark\n");
    printf("========================================\n");
    printf("Matrix: %d x %d  |  %.2f MB per matrix\n",
           N, N, (float)(N * N * (int)sizeof(float)) / (1 << 20));
    printf("Block:  %d x %d\n", BD, BD);

    dim3 block(BD, BD);
    dim3 grid((N + BD - 1) / BD, (N + BD - 1) / BD);
    printf("Grid:   %d x %d\n\n", grid.x, grid.y);

    size_t sz = (size_t)N * N * sizeof(float);

    // Host memory
    float *h_in  = (float *)malloc(sz);
    float *h_gpu = (float *)malloc(sz);
    float *h_cpu = (float *)malloc(sz);
    if (!h_in || !h_gpu || !h_cpu) {
        fprintf(stderr, "Host allocation failed!\n");
        return 1;
    }

    // Random data
    srand(42);
    for (int i = 0; i < N * N; i++)
        h_in[i] = (float)(rand() % 100) / 10.0f;

    // CPU ground truth
    transpose_cpu(h_cpu, h_in, N);

    // ---- Demo: print matrices ----
    if (demo) {
        printf("--- Matrix Output (Demo: N=%d, Block=%dx%d) ---\n\n", N, BD, BD);
        print_matrix("Original Matrix A", h_in, N);
        print_matrix("Transposed Matrix A^T (CPU)", h_cpu, N);
    }

    // Device memory
    float *d_in, *d_out;
    CUDA_CHECK(cudaMalloc(&d_in,  sz));
    CUDA_CHECK(cudaMalloc(&d_out, sz));
    CUDA_CHECK(cudaMemcpy(d_in, h_in, sz, cudaMemcpyHostToDevice));

    float max_err; int pass;

    // ---- 1) Global Memory ----
    float t1 = bench(transpose_global, grid, block, d_out, d_in, N,
                     "Global Memory");
    CUDA_CHECK(cudaMemcpy(h_gpu, d_out, sz, cudaMemcpyDeviceToHost));
    pass = verify(h_gpu, h_cpu, N, &max_err);
    printf("  err=%.2e  %s\n", max_err, pass ? "PASS" : "FAIL");
    CUDA_CHECK(cudaMemset(d_out, 0, sz));

    // ---- 2) Shared Memory (bank conflicts) ----
    float t2 = bench(transpose_shared, grid, block, d_out, d_in, N,
                     "Shared Memory");
    CUDA_CHECK(cudaMemcpy(h_gpu, d_out, sz, cudaMemcpyDeviceToHost));
    pass = verify(h_gpu, h_cpu, N, &max_err);
    printf("  err=%.2e  %s\n", max_err, pass ? "PASS" : "FAIL");
    CUDA_CHECK(cudaMemset(d_out, 0, sz));

    // ---- 3) Optimized Shared Memory (no bank conflicts) ----
    float t3 = bench(transpose_shared_opt, grid, block, d_out, d_in, N,
                     "Optimized Shared Memory");
    CUDA_CHECK(cudaMemcpy(h_gpu, d_out, sz, cudaMemcpyDeviceToHost));
    pass = verify(h_gpu, h_cpu, N, &max_err);
    printf("  err=%.2e  %s\n", max_err, pass ? "PASS" : "FAIL");

    // ---- Demo: print GPU output ----
    if (demo) {
        print_matrix("Transposed Matrix A^T (GPU, Optimized Shared)", h_gpu, N);
    }

    // ---- Summary ----
    printf("\n========================================\n");
    printf("Summary  (N=%d  Block=%dx%d)\n", N, BD, BD);
    printf("========================================\n");
    printf("Global Memory:    %8.4f ms\n", t1);
    printf("Shared Memory:    %8.4f ms  (%.2fx)\n", t2, t1/t2);
    printf("Opt Shared:       %8.4f ms  (%.2fx)\n", t3, t1/t3);

    // CSV for data collection
    printf("\nCSV:%d,%d,%.4f,%.4f,%.4f\n", N, BD, t1, t2, t3);

    // Cleanup
    CUDA_CHECK(cudaFree(d_in));
    CUDA_CHECK(cudaFree(d_out));
    free(h_in); free(h_gpu); free(h_cpu);
    return 0;
}
