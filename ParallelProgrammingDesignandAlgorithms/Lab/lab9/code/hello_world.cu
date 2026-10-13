#include <stdio.h>
#include <cuda_runtime.h>

// CUDA Kernel: each thread prints its thread and block info
__global__ void hello_kernel() {
    // blockIdx.x gives the block index
    // threadIdx.y gives the row index within the block
    // threadIdx.x gives the column index within the block
    int block_id = blockIdx.x;
    int row = threadIdx.y;
    int col = threadIdx.x;

    printf("Hello World from Thread (%d, %d) in Block %d!\n", row, col, block_id);
}

int main(int argc, char *argv[]) {
    int n = 4;  // number of blocks (default)
    int m = 2;  // rows per block (default)
    int k = 2;  // cols per block (default)

    // Parse command line arguments if provided
    if (argc >= 4) {
        n = atoi(argv[1]);
        m = atoi(argv[2]);
        k = atoi(argv[3]);
    }

    // Ensure parameters are within valid range [1, 32]
    if (n < 1 || n > 32) n = 4;
    if (m < 1 || m > 32) m = 2;
    if (k < 1 || k > 32) k = 2;

    printf("Configuration: %d blocks, each block %d x %d threads\n", n, m, k);
    printf("Total threads: %d\n\n", n * m * k);

    // Host output
    printf("Hello World from the host!\n\n");

    // Launch kernel: n blocks, each with dim3(k, m) threads
    dim3 blockDim(k, m);
    dim3 gridDim(n);
    hello_kernel<<<gridDim, blockDim>>>();

    // Synchronize to ensure all printf from device are flushed
    cudaDeviceSynchronize();

    // Check for errors
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA Error: %s\n", cudaGetErrorString(err));
        return 1;
    }

    return 0;
}
