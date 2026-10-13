#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <math.h>
#include <string.h>

// CUDA / cuRAND 错误检查宏
#define CUDA_CHECK(call) do {                                              \
    cudaError_t err = call;                                                \
    if (err != cudaSuccess) {                                              \
        fprintf(stderr, "CUDA Error at %s:%d - %s\n", __FILE__, __LINE__, \
                cudaGetErrorString(err));                                   \
        exit(EXIT_FAILURE);                                                \
    }                                                                      \
} while(0)

#define CURAND_CHECK(call) do {                                            \
    curandStatus_t err = call;                                             \
    if (err != CURAND_STATUS_SUCCESS) {                                    \
        fprintf(stderr, "cuRAND Error at %s:%d (code %d)\n",              \
                __FILE__, __LINE__, err);                                   \
        exit(EXIT_FAILURE);                                                \
    }                                                                      \
} while(0)

// CPU 上跑一遍矩阵乘法，用来和 GPU 结果做对比
void cpu_matmul(const float *A, const float *B, float *C, int M, int N, int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            for (int k = 0; k < K; k++) {
                sum += A[i * K + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// 对比 GPU 和 CPU 的结果，逐元素检查相对误差
bool verify_result(const float *gpu_C, const float *cpu_C, int M, int N, float eps) {
    for (int i = 0; i < M * N; i++) {
        float diff = fabsf(gpu_C[i] - cpu_C[i]);
        float rel = diff / fmaxf(fabsf(cpu_C[i]), 1e-8f);
        if (rel > eps && diff > eps) {
            printf("  Mismatch at [%d]: GPU=%.6f, CPU=%.6f, diff=%.6f\n",
                   i, gpu_C[i], cpu_C[i], diff);
            return false;
        }
    }
    return true;
}

// 用 event 计时，返回毫秒
float time_kernel_ms(cudaEvent_t start, cudaEvent_t stop) {
    float ms;
    CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
    return ms;
}

// ============================================================
// 第一类：最朴素的全局内存实现（naive）
// ============================================================

// 按行分块：grid 的一维对应行组，threadIdx.y 定位组内行，threadIdx.x 遍历列
__global__ void matmul_naive_row(const float *__restrict__ A,
                                  const float *__restrict__ B,
                                  float *__restrict__ C,
                                  int M, int N, int K) {
    int row = blockIdx.x * blockDim.y + threadIdx.y;

    if (row < M) {
        for (int col = threadIdx.x; col < N; col += blockDim.x) {
            float sum = 0.0f;
            for (int k = 0; k < K; k++) {
                sum += A[row * K + k] * B[k * N + col];
            }
            C[row * N + col] = sum;
        }
    }
}

// 按列分块：和上面对称，只是把列当作外层
__global__ void matmul_naive_col(const float *__restrict__ A,
                                  const float *__restrict__ B,
                                  float *__restrict__ C,
                                  int M, int N, int K) {
    int col = blockIdx.x * blockDim.y + threadIdx.y;

    if (col < N) {
        for (int row = threadIdx.x; row < M; row += blockDim.x) {
            float sum = 0.0f;
            for (int k = 0; k < K; k++) {
                sum += A[row * K + k] * B[k * N + col];
            }
            C[row * N + col] = sum;
        }
    }
}

// 常规 2D 分块，一个线程算一个元素
__global__ void matmul_naive_block(const float *__restrict__ A,
                                    const float *__restrict__ B,
                                    float *__restrict__ C,
                                    int M, int N, int K) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < M && col < N) {
        float sum = 0.0f;
        for (int k = 0; k < K; k++) {
            sum += A[row * K + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

// ============================================================
// 第二类：共享内存分块（shared memory tiling）
// BLOCK_SIZE 作为模板参数，编译期确定共享内存大小
// ============================================================

// 共享内存 + 按行划分
template <int BLOCK_SIZE>
__global__ void matmul_shared_row(const float *__restrict__ A,
                                   const float *__restrict__ B,
                                   float *__restrict__ C,
                                   int M, int N, int K) {
    __shared__ float As[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][BLOCK_SIZE];

    int row = blockIdx.x * BLOCK_SIZE + threadIdx.y;

    float sum = 0.0f;

    for (int col_tile = 0; col_tile < N; col_tile += BLOCK_SIZE) {
        int col = col_tile + threadIdx.x;
        sum = 0.0f;

        for (int tk = 0; tk < K; tk += BLOCK_SIZE) {
            // 搬 A 的一块到共享内存
            if (row < M && (tk + threadIdx.x) < K)
                As[threadIdx.y][threadIdx.x] = A[row * K + tk + threadIdx.x];
            else
                As[threadIdx.y][threadIdx.x] = 0.0f;

            // 搬 B 的一块到共享内存
            int b_row = tk + threadIdx.y;
            if (b_row < K && col < N)
                Bs[threadIdx.y][threadIdx.x] = B[b_row * N + col];
            else
                Bs[threadIdx.y][threadIdx.x] = 0.0f;

            __syncthreads();

            for (int k = 0; k < BLOCK_SIZE; k++) {
                sum += As[threadIdx.y][k] * Bs[k][threadIdx.x];
            }
            __syncthreads();
        }

        if (row < M && col < N) {
            C[row * N + col] = sum;
        }
    }
}

// 共享内存 + 按列划分
template <int BLOCK_SIZE>
__global__ void matmul_shared_col(const float *__restrict__ A,
                                   const float *__restrict__ B,
                                   float *__restrict__ C,
                                   int M, int N, int K) {
    __shared__ float As[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][BLOCK_SIZE];

    int col = blockIdx.x * BLOCK_SIZE + threadIdx.y;

    float sum = 0.0f;

    for (int row_tile = 0; row_tile < M; row_tile += BLOCK_SIZE) {
        int row = row_tile + threadIdx.x;
        sum = 0.0f;

        for (int tk = 0; tk < K; tk += BLOCK_SIZE) {
            // 搬 A 的一块
            int a_row = row;
            int a_col = tk + threadIdx.y;
            if (a_row < M && a_col < K)
                As[threadIdx.x][threadIdx.y] = A[a_row * K + a_col];
            else
                As[threadIdx.x][threadIdx.y] = 0.0f;

            // 搬 B 的一块
            int b_row = tk + threadIdx.x;
            if (b_row < K && col < N)
                Bs[threadIdx.x][threadIdx.y] = B[b_row * N + col];
            else
                Bs[threadIdx.x][threadIdx.y] = 0.0f;

            __syncthreads();

            for (int k = 0; k < BLOCK_SIZE; k++) {
                sum += As[threadIdx.x][k] * Bs[k][threadIdx.y];
            }
            __syncthreads();
        }

        if (row < M && col < N) {
            C[row * N + col] = sum;
        }
    }
}

// 共享内存 + 常规 2D 分块
template <int BLOCK_SIZE>
__global__ void matmul_shared_block(const float *__restrict__ A,
                                     const float *__restrict__ B,
                                     float *__restrict__ C,
                                     int M, int N, int K) {
    __shared__ float As[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][BLOCK_SIZE];

    int row = blockIdx.y * BLOCK_SIZE + threadIdx.y;
    int col = blockIdx.x * BLOCK_SIZE + threadIdx.x;

    float sum = 0.0f;

    for (int tk = 0; tk < K; tk += BLOCK_SIZE) {
        // 搬 A 的一块
        if (row < M && (tk + threadIdx.x) < K)
            As[threadIdx.y][threadIdx.x] = A[row * K + tk + threadIdx.x];
        else
            As[threadIdx.y][threadIdx.x] = 0.0f;

        // 搬 B 的一块
        if ((tk + threadIdx.y) < K && col < N)
            Bs[threadIdx.y][threadIdx.x] = B[(tk + threadIdx.y) * N + col];
        else
            Bs[threadIdx.y][threadIdx.x] = 0.0f;

        __syncthreads();

        for (int k = 0; k < BLOCK_SIZE; k++) {
            sum += As[threadIdx.y][k] * Bs[k][threadIdx.x];
        }
        __syncthreads();
    }

    if (row < M && col < N) {
        C[row * N + col] = sum;
    }
}

// ============================================================
// 第三类：寄存器分块（register tiling）
// 每个线程用寄存器算 TILE_DIM×TILE_DIM 个结果
// TILE_DIM = 32 / BLOCK_SIZE，保证 SMEM_DIM 恒为 32
// ============================================================

// 编译期算 TILE_DIM 和 SMEM_DIM
template <int BLOCK_SIZE>
struct TileConfig {
    static const int TILE_DIM = 32 / BLOCK_SIZE;
    static const int SMEM_DIM = BLOCK_SIZE * TILE_DIM; // = 32
};

// 寄存器分块 + 按行划分
template <int BLOCK_SIZE>
__global__ void matmul_reg_row(const float *__restrict__ A,
                                const float *__restrict__ B,
                                float *__restrict__ C,
                                int M, int N, int K) {
    const int TILE_DIM = TileConfig<BLOCK_SIZE>::TILE_DIM;
    const int SMEM_DIM = TileConfig<BLOCK_SIZE>::SMEM_DIM;

    __shared__ float As[SMEM_DIM][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][BLOCK_SIZE];

    int base_row = blockIdx.x * SMEM_DIM + threadIdx.y * TILE_DIM;
    int tid_y = threadIdx.y;
    int tid_x = threadIdx.x;

    // 每个线程维护 TILE_DIM 个部分和（沿列方向滑动）
    float c[TILE_DIM] = {0.0f};
    float a_reg[TILE_DIM];
    float b_reg;

    for (int col_tile = 0; col_tile < N; col_tile += BLOCK_SIZE) {
        int col = col_tile + tid_x;
        for (int i = 0; i < TILE_DIM; i++) c[i] = 0.0f;

        for (int tk = 0; tk < K; tk += BLOCK_SIZE) {
            // 搬 A：每个线程负责 TILE_DIM 行
            for (int i = 0; i < TILE_DIM; i++) {
                int r = base_row + i;
                int c_a = tk + tid_x;
                As[tid_y * TILE_DIM + i][tid_x] =
                    (r < M && c_a < K) ? A[r * K + c_a] : 0.0f;
            }

            // 搬 B
            int b_row = tk + tid_y;
            if (b_row < K && col < N)
                Bs[tid_y][tid_x] = B[b_row * N + col];
            else
                Bs[tid_y][tid_x] = 0.0f;

            __syncthreads();

            // 寄存器上做乘加
            for (int k = 0; k < BLOCK_SIZE; k++) {
                for (int i = 0; i < TILE_DIM; i++)
                    a_reg[i] = As[tid_y * TILE_DIM + i][k];
                b_reg = Bs[k][tid_x];

                for (int i = 0; i < TILE_DIM; i++)
                    c[i] += a_reg[i] * b_reg;
            }
            __syncthreads();
        }

        for (int i = 0; i < TILE_DIM; i++) {
            int r = base_row + i;
            if (r < M && col < N)
                C[r * N + col] = c[i];
        }
    }
}

// 寄存器分块 + 按列划分
template <int BLOCK_SIZE>
__global__ void matmul_reg_col(const float *__restrict__ A,
                                const float *__restrict__ B,
                                float *__restrict__ C,
                                int M, int N, int K) {
    const int TILE_DIM = TileConfig<BLOCK_SIZE>::TILE_DIM;
    const int SMEM_DIM = TileConfig<BLOCK_SIZE>::SMEM_DIM;

    __shared__ float As[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][SMEM_DIM];

    int base_col = blockIdx.x * SMEM_DIM + threadIdx.y * TILE_DIM;
    int tid_y = threadIdx.y;
    int tid_x = threadIdx.x;

    float c[TILE_DIM] = {0.0f};
    float a_reg;
    float b_reg[TILE_DIM];

    for (int row_tile = 0; row_tile < M; row_tile += BLOCK_SIZE) {
        int row = row_tile + tid_x;
        for (int i = 0; i < TILE_DIM; i++) c[i] = 0.0f;

        for (int tk = 0; tk < K; tk += BLOCK_SIZE) {
            // 搬 A
            int a_row = row;
            int a_col = tk + tid_y;
            if (a_row < M && a_col < K)
                As[tid_x][tid_y] = A[a_row * K + a_col];
            else
                As[tid_x][tid_y] = 0.0f;

            // 搬 B：每个线程负责 TILE_DIM 列
            for (int i = 0; i < TILE_DIM; i++) {
                int b_row = tk + tid_x;
                int c_b = base_col + i;
                Bs[tid_x][tid_y * TILE_DIM + i] =
                    (b_row < K && c_b < N) ? B[b_row * N + c_b] : 0.0f;
            }

            __syncthreads();

            for (int k = 0; k < BLOCK_SIZE; k++) {
                a_reg = As[tid_x][k];
                for (int i = 0; i < TILE_DIM; i++)
                    b_reg[i] = Bs[k][tid_y * TILE_DIM + i];

                for (int i = 0; i < TILE_DIM; i++)
                    c[i] += a_reg * b_reg[i];
            }
            __syncthreads();
        }

        for (int i = 0; i < TILE_DIM; i++) {
            int c_col = base_col + i;
            if (row < M && c_col < N)
                C[row * N + c_col] = c[i];
        }
    }
}

// 寄存器分块 + 常规 2D 分块
template <int BLOCK_SIZE>
__global__ void matmul_reg_block(const float *__restrict__ A,
                                  const float *__restrict__ B,
                                  float *__restrict__ C,
                                  int M, int N, int K) {
    const int TILE_DIM = TileConfig<BLOCK_SIZE>::TILE_DIM;
    const int SMEM_DIM = BLOCK_SIZE * TILE_DIM;

    __shared__ float As[SMEM_DIM][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][SMEM_DIM];

    int base_row = blockIdx.y * SMEM_DIM + threadIdx.y * TILE_DIM;
    int base_col = blockIdx.x * SMEM_DIM + threadIdx.x * TILE_DIM;

    float c[TILE_DIM][TILE_DIM] = {{0.0f}};
    float a_reg[TILE_DIM];
    float b_reg[TILE_DIM];

    for (int tk = 0; tk < K; tk += BLOCK_SIZE) {
        // 协作搬 A：每个线程搬 TILE_DIM 个元素
        for (int i = 0; i < TILE_DIM; i++) {
            int r = base_row + i;
            int c_a = tk + threadIdx.x;
            As[threadIdx.y * TILE_DIM + i][threadIdx.x] =
                (r < M && c_a < K) ? A[r * K + c_a] : 0.0f;
        }

        // 协作搬 B
        for (int i = 0; i < TILE_DIM; i++) {
            int b_row = tk + threadIdx.y;
            int c_b = base_col + i;
            Bs[threadIdx.y][threadIdx.x * TILE_DIM + i] =
                (b_row < K && c_b < N) ? B[b_row * N + c_b] : 0.0f;
        }

        __syncthreads();

        // 寄存器上算 TILE_DIM×TILE_DIM 的外积累加
        for (int k = 0; k < BLOCK_SIZE; k++) {
            for (int i = 0; i < TILE_DIM; i++)
                a_reg[i] = As[threadIdx.y * TILE_DIM + i][k];
            for (int j = 0; j < TILE_DIM; j++)
                b_reg[j] = Bs[k][threadIdx.x * TILE_DIM + j];

            for (int i = 0; i < TILE_DIM; i++)
                for (int j = 0; j < TILE_DIM; j++)
                    c[i][j] += a_reg[i] * b_reg[j];
        }
        __syncthreads();
    }

    // 写回全局内存
    for (int i = 0; i < TILE_DIM; i++) {
        for (int j = 0; j < TILE_DIM; j++) {
            int r = base_row + i;
            int c_col = base_col + j;
            if (r < M && c_col < N)
                C[r * N + c_col] = c[i][j];
        }
    }
}

// ============================================================
// 各 kernel 的 launch 包装函数，处理模板实例化
// ============================================================
template <int BLOCK_SIZE>
void launch_shared_row(float *d_A, float *d_B, float *d_C, int M, int N, int K) {
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((M + BLOCK_SIZE - 1) / BLOCK_SIZE);
    matmul_shared_row<BLOCK_SIZE><<<grid, block>>>(d_A, d_B, d_C, M, N, K);
}

template <int BLOCK_SIZE>
void launch_shared_col(float *d_A, float *d_B, float *d_C, int M, int N, int K) {
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((N + BLOCK_SIZE - 1) / BLOCK_SIZE);
    matmul_shared_col<BLOCK_SIZE><<<grid, block>>>(d_A, d_B, d_C, M, N, K);
}

template <int BLOCK_SIZE>
void launch_shared_block(float *d_A, float *d_B, float *d_C, int M, int N, int K) {
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((N + BLOCK_SIZE - 1) / BLOCK_SIZE,
              (M + BLOCK_SIZE - 1) / BLOCK_SIZE);
    matmul_shared_block<BLOCK_SIZE><<<grid, block>>>(d_A, d_B, d_C, M, N, K);
}

template <int BLOCK_SIZE>
void launch_reg_row(float *d_A, float *d_B, float *d_C, int M, int N, int K) {
    const int SMEM_DIM = BLOCK_SIZE * TileConfig<BLOCK_SIZE>::TILE_DIM;
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((M + SMEM_DIM - 1) / SMEM_DIM);
    matmul_reg_row<BLOCK_SIZE><<<grid, block>>>(d_A, d_B, d_C, M, N, K);
}

template <int BLOCK_SIZE>
void launch_reg_col(float *d_A, float *d_B, float *d_C, int M, int N, int K) {
    const int SMEM_DIM = BLOCK_SIZE * TileConfig<BLOCK_SIZE>::TILE_DIM;
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((N + SMEM_DIM - 1) / SMEM_DIM);
    matmul_reg_col<BLOCK_SIZE><<<grid, block>>>(d_A, d_B, d_C, M, N, K);
}

template <int BLOCK_SIZE>
void launch_reg_block(float *d_A, float *d_B, float *d_C, int M, int N, int K) {
    const int SMEM_DIM = BLOCK_SIZE * TileConfig<BLOCK_SIZE>::TILE_DIM;
    dim3 block(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((N + SMEM_DIM - 1) / SMEM_DIM,
              (M + SMEM_DIM - 1) / SMEM_DIM);
    matmul_reg_block<BLOCK_SIZE><<<grid, block>>>(d_A, d_B, d_C, M, N, K);
}

// 根据 kernel 名字和 block_size 分发到具体实现，返回耗时(ms)
float run_kernel(const char *kernel_name, int block_size,
                  float *d_A, float *d_B, float *d_C,
                  int M, int N, int K,
                  cudaEvent_t start, cudaEvent_t stop) {
    dim3 block(block_size, block_size);
    dim3 grid;

    CUDA_CHECK(cudaEventRecord(start));

    // naive 实现
    if (strcmp(kernel_name, "naive_row") == 0) {
        grid = dim3((M + block_size - 1) / block_size);
        matmul_naive_row<<<grid, block>>>(d_A, d_B, d_C, M, N, K);
    }
    else if (strcmp(kernel_name, "naive_col") == 0) {
        grid = dim3((N + block_size - 1) / block_size);
        matmul_naive_col<<<grid, block>>>(d_A, d_B, d_C, M, N, K);
    }
    else if (strcmp(kernel_name, "naive_block") == 0) {
        grid = dim3((N + block_size - 1) / block_size,
                    (M + block_size - 1) / block_size);
        matmul_naive_block<<<grid, block>>>(d_A, d_B, d_C, M, N, K);
    }
    // 共享内存实现
    else if (strcmp(kernel_name, "shared_row") == 0) {
        if (block_size == 8)       launch_shared_row<8>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 16) launch_shared_row<16>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 32) launch_shared_row<32>(d_A, d_B, d_C, M, N, K);
    }
    else if (strcmp(kernel_name, "shared_col") == 0) {
        if (block_size == 8)       launch_shared_col<8>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 16) launch_shared_col<16>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 32) launch_shared_col<32>(d_A, d_B, d_C, M, N, K);
    }
    else if (strcmp(kernel_name, "shared_block") == 0) {
        if (block_size == 8)       launch_shared_block<8>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 16) launch_shared_block<16>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 32) launch_shared_block<32>(d_A, d_B, d_C, M, N, K);
    }
    // 寄存器分块实现
    else if (strcmp(kernel_name, "reg_row") == 0) {
        if (block_size == 8)       launch_reg_row<8>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 16) launch_reg_row<16>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 32) launch_reg_row<32>(d_A, d_B, d_C, M, N, K);
    }
    else if (strcmp(kernel_name, "reg_col") == 0) {
        if (block_size == 8)       launch_reg_col<8>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 16) launch_reg_col<16>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 32) launch_reg_col<32>(d_A, d_B, d_C, M, N, K);
    }
    else if (strcmp(kernel_name, "reg_block") == 0) {
        if (block_size == 8)       launch_reg_block<8>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 16) launch_reg_block<16>(d_A, d_B, d_C, M, N, K);
        else if (block_size == 32) launch_reg_block<32>(d_A, d_B, d_C, M, N, K);
    }
    else {
        fprintf(stderr, "Unknown kernel: %s\n", kernel_name);
        exit(1);
    }

    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));
    return time_kernel_ms(start, stop);
}

// 测一组 (kernel, block_size, N)，先 warmup 再跑 5 次取平均，最后验结果
float test_kernel(const char *kernel_name, const char *partition_name,
                  int block_size, int N,
                  float *h_A, float *h_B, float *h_C_ref, float *h_C_gpu,
                  float *d_A, float *d_B, float *d_C,
                  cudaEvent_t start, cudaEvent_t stop) {
    // 预热
    run_kernel(kernel_name, block_size, d_A, d_B, d_C, N, N, N, start, stop);

    // 跑 5 次取平均
    float total_time = 0.0f;
    int runs = 5;
    for (int r = 0; r < runs; r++) {
        CUDA_CHECK(cudaMemset(d_C, 0, N * N * sizeof(float)));
        total_time += run_kernel(kernel_name, block_size, d_A, d_B, d_C,
                                  N, N, N, start, stop);
    }
    float avg_time = total_time / runs;

    // 拷回 host 验结果
    CUDA_CHECK(cudaMemcpy(h_C_gpu, d_C, N * N * sizeof(float),
                           cudaMemcpyDeviceToHost));

    bool ok = verify_result(h_C_gpu, h_C_ref, N, N, 1e-3f);
    // 寄存器版本因为 FMA 重排，误差稍大，放宽一点再试
    if (!ok) {
        ok = verify_result(h_C_gpu, h_C_ref, N, N, 5e-3f);
    }

    printf("  [%s/%s] block=%2d×%-2d N=%-4d  time=%8.3f ms  %s\n",
           partition_name, kernel_name, block_size, block_size, N,
           avg_time, ok ? "PASS" : "FAIL");

    if (!ok) {
        fprintf(stderr, "  WARNING: Verification failed for %s/%s at N=%d, block=%d\n",
                partition_name, kernel_name, N, block_size);
    }

    return avg_time;
}

int main() {
    CUDA_CHECK(cudaSetDevice(0));

    cudaDeviceProp prop;
    CUDA_CHECK(cudaGetDeviceProperties(&prop, 0));
    printf("GPU: %s\n", prop.name);
    printf("Compute Capability: %d.%d\n", prop.major, prop.minor);
    printf("Max Threads per Block: %d\n", prop.maxThreadsPerBlock);
    printf("Shared Memory per Block: %zu KB\n", prop.sharedMemPerBlock / 1024);
    printf("SMs: %d\n\n", prop.multiProcessorCount);

    // 测试的矩阵大小和 block 大小
    int sizes[] = {512, 1024, 2048};
    int block_sizes[] = {8, 16, 32};
    int num_sizes = 3;
    int num_blocks = 3;

    const char *partition_names[] = {"row", "col", "block"};
    const char *impl_names[] = {"naive", "shared", "reg"};

    // results[划分][实现][大小][block]
    float results[3][3][3][3];

    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    int max_N = 2048;
    size_t max_bytes = max_N * max_N * sizeof(float);

    float *h_A = (float *)malloc(max_bytes);
    float *h_B = (float *)malloc(max_bytes);
    float *h_C_ref = (float *)malloc(max_bytes);
    float *h_C_gpu = (float *)malloc(max_bytes);

    float *d_A, *d_B, *d_C;
    CUDA_CHECK(cudaMalloc(&d_A, max_bytes));
    CUDA_CHECK(cudaMalloc(&d_B, max_bytes));
    CUDA_CHECK(cudaMalloc(&d_C, max_bytes));

    // cuRAND 生成随机矩阵
    curandGenerator_t gen;
    CURAND_CHECK(curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT));
    CURAND_CHECK(curandSetPseudoRandomGeneratorSeed(gen, 1234ULL));

    CURAND_CHECK(curandGenerateUniform(gen, d_A, max_N * max_N));
    CURAND_CHECK(curandGenerateUniform(gen, d_B, max_N * max_N));

    CUDA_CHECK(cudaMemcpy(h_A, d_A, max_bytes, cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(h_B, d_B, max_bytes, cudaMemcpyDeviceToHost));

    printf("=== CUDA 矩阵乘法性能对比 ===\n");
    printf("矩阵大小: 512, 1024, 2048\n");
    printf("Block 大小: 8×8, 16×16, 32×32\n");
    printf("实现: naive / 共享内存 / 寄存器分块\n");
    printf("划分方式: 按行 / 按列 / 2D分块\n\n");

    for (int si = 0; si < num_sizes; si++) {
        int N = sizes[si];
        printf("--- 矩阵大小: %d × %d ---\n", N, N);

        cpu_matmul(h_A, h_B, h_C_ref, N, N, N);

        for (int p = 0; p < 3; p++) {
            for (int impl = 0; impl < 3; impl++) {
                char kernel_name[64];
                snprintf(kernel_name, sizeof(kernel_name), "%s_%s",
                         impl_names[impl], partition_names[p]);

                for (int bi = 0; bi < num_blocks; bi++) {
                    int bs = block_sizes[bi];

                    float t = test_kernel(kernel_name, partition_names[p],
                                          bs, N,
                                          h_A, h_B, h_C_ref, h_C_gpu,
                                          d_A, d_B, d_C,
                                          start, stop);
                    results[p][impl][si][bi] = t;
                }
            }
        }
        printf("\n");
    }

    // 汇总表格
    printf("\n\n========== 性能汇总 ==========\n\n");

    for (int p = 0; p < 3; p++) {
        printf("=== 划分方式: %s ===\n", partition_names[p]);
        printf("%-8s %-10s %-12s %-12s %-12s\n",
               "N", "Block", "Naive(ms)", "Shared(ms)", "Register(ms)");
        printf("--------------------------------------------------------\n");
        for (int si = 0; si < num_sizes; si++) {
            int N = sizes[si];
            for (int bi = 0; bi < num_blocks; bi++) {
                int bs = block_sizes[bi];
                printf("%-8d %2d×%-7d %-12.3f %-12.3f %-12.3f\n",
                       N, bs, bs,
                       results[p][0][si][bi],
                       results[p][1][si][bi],
                       results[p][2][si][bi]);
            }
            if (si < num_sizes - 1) printf("\n");
        }
        printf("\n");
    }

    // LaTeX 表格数据，方便直接粘贴
    printf("\n\n========== LaTeX 表格数据 ==========\n");
    const char *part_labels[] = {"Row-wise", "Column-wise", "Block-wise"};

    for (int p = 0; p < 3; p++) {
        printf("\n%% %s\n", part_labels[p]);
        for (int si = 0; si < num_sizes; si++) {
            int N = sizes[si];
            for (int bi = 0; bi < num_blocks; bi++) {
                int bs = block_sizes[bi];
                printf("%% N=%d, Block=%dx%d: naive=%.3f, shared=%.3f, reg=%.3f\n",
                       N, bs, bs,
                       results[p][0][si][bi],
                       results[p][1][si][bi],
                       results[p][2][si][bi]);
            }
        }
    }

    // 清理
    free(h_A); free(h_B); free(h_C_ref); free(h_C_gpu);
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CURAND_CHECK(curandDestroyGenerator(gen));

    printf("\n测试结束。\n");
    return 0;
}
