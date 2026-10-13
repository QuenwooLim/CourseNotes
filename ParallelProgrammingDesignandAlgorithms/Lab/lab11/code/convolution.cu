/*
 * 一维卷积 CUDA 实现 —— 四种优化版本
 * 中山大学 计算机学院 并行程序设计与算法实验
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

// 一些常量
#define MAX_KERNEL_SIZE     31   // 最大卷积核长度
#define BLOCK_SIZE          256  // 每个 block 的线程数
#define TILE_SIZE           4    // 寄存器分块时每线程处理的输出数

// 检查 CUDA 调用是否出错
#define CHECK_CUDA(call) do {                                         \
    cudaError_t err = call;                                           \
    if (err != cudaSuccess) {                                         \
        fprintf(stderr, "CUDA 错误 %s:%d - %s\n",                     \
                __FILE__, __LINE__, cudaGetErrorString(err));         \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
} while(0)

// 常量内存，存卷积核（版本二用）
__constant__ float d_const_kernel[MAX_KERNEL_SIZE];

// -----------------------------------------------------------
// CPU 上的串行卷积，用来验证 GPU 结果对不对
// -----------------------------------------------------------
void convolution_cpu(float *input, float *kernel, float *output,
                     int input_size, int kernel_size) {
    int radius = kernel_size / 2;
    for (int i = 0; i < input_size; i++) {
        float sum = 0.0f;
        for (int k = 0; k < kernel_size; k++) {
            int input_idx = i + k - radius;
            if (input_idx >= 0 && input_idx < input_size) {
                sum += input[input_idx] * kernel[k];
            }
        }
        output[i] = sum;
    }
}

// -----------------------------------------------------------
// 版本一：最朴素的全局内存版本
// 每个线程算一个输出，直接从全局内存读输入和卷积核
// -----------------------------------------------------------
__global__ void convolution_basic(float *input, float *kernel,
                                   float *output, int input_size,
                                   int kernel_size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < input_size) {
        float sum = 0.0f;
        int radius = kernel_size / 2;
        for (int k = 0; k < kernel_size; k++) {
            int input_idx = idx + k - radius;
            if (input_idx >= 0 && input_idx < input_size) {
                sum += input[input_idx] * kernel[k];
            }
        }
        output[idx] = sum;
    }
}

// -----------------------------------------------------------
// 版本二：用常量内存存卷积核
// 卷积核很小（最多 31 个 float），扔进 __constant__ 里，
// 同一 warp 内的线程读到同一个地址时硬件会广播，比反复从
// 全局内存读快很多
// -----------------------------------------------------------
__global__ void convolution_constant(float *input, float *output,
                                      int input_size, int kernel_size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < input_size) {
        float sum = 0.0f;
        int radius = kernel_size / 2;
        for (int k = 0; k < kernel_size; k++) {
            int input_idx = idx + k - radius;
            if (input_idx >= 0 && input_idx < input_size) {
                sum += input[input_idx] * d_const_kernel[k];  // 从常量内存读
            }
        }
        output[idx] = sum;
    }
}

// -----------------------------------------------------------
// 版本三：共享内存版本
// 每个 block 把需要的那段输入数据搬到共享内存里，
// 包括左右两边各 radius 个"halo"元素（用于边界计算）。
// 共享内存大小 = BLOCK_SIZE + kernel_size - 1
// -----------------------------------------------------------
__global__ void convolution_shared(float *input, float *kernel,
                                    float *output, int input_size,
                                    int kernel_size) {
    extern __shared__ float shared_input[];

    int tid = threadIdx.x;
    int idx = blockIdx.x * blockDim.x + tid;
    int radius = kernel_size / 2;
    int block_start = blockIdx.x * blockDim.x;

    // 先把主数据加载进来（每个线程搬一个元素，左移 radius）
    int global_load_idx = block_start + tid - radius;
    if (global_load_idx >= 0 && global_load_idx < input_size) {
        shared_input[tid] = input[global_load_idx];
    } else {
        shared_input[tid] = 0.0f;  // 超出边界的填 0
    }

    // 再加载右边多余的 halo 部分（前面 kernel_size-1 个线程干这个活）
    if (tid < kernel_size - 1) {
        int extra_idx = block_start + blockDim.x + tid - radius;
        if (extra_idx >= 0 && extra_idx < input_size) {
            shared_input[blockDim.x + tid] = input[extra_idx];
        } else {
            shared_input[blockDim.x + tid] = 0.0f;
        }
    }

    __syncthreads();  // 等所有线程都加载完

    // 从共享内存读数据做卷积
    if (idx < input_size) {
        float sum = 0.0f;
        for (int k = 0; k < kernel_size; k++) {
            sum += shared_input[tid + k] * kernel[k];
        }
        output[idx] = sum;
    }
}

// -----------------------------------------------------------
// 版本四：寄存器分块版本
// 每个线程一次算 TILE_SIZE 个输出，卷积核塞进寄存器，
// 输入数据同样用共享内存缓存
// -----------------------------------------------------------
__global__ void convolution_register(float *input, float *kernel,
                                      float *output, int input_size,
                                      int kernel_size) {
    extern __shared__ float shared_input[];

    int tid = threadIdx.x;
    int block_output_start = blockIdx.x * (blockDim.x * TILE_SIZE);
    int kernel_radius = kernel_size / 2;
    int global_start_idx = block_output_start - kernel_radius;

    // 共享内存需要的大小：BLOCK_SIZE * TILE_SIZE + kernel_size - 1
    int shared_size = blockDim.x * TILE_SIZE + kernel_size - 1;

    // 协作加载：每个线程搬若干元素到共享内存
    for (int t = 0; t < TILE_SIZE; t++) {
        int shared_idx = tid + t * blockDim.x;
        int global_idx = global_start_idx + shared_idx;
        if (shared_idx < shared_size) {
            if (global_idx >= 0 && global_idx < input_size) {
                shared_input[shared_idx] = input[global_idx];
            } else {
                shared_input[shared_idx] = 0.0f;
            }
        }
    }

    // 尾部剩余的几个元素
    int base_loaded = blockDim.x * TILE_SIZE;
    if (tid + base_loaded < shared_size) {
        int shared_idx = tid + base_loaded;
        int global_idx = global_start_idx + shared_idx;
        if (global_idx >= 0 && global_idx < input_size) {
            shared_input[shared_idx] = input[global_idx];
        } else {
            shared_input[shared_idx] = 0.0f;
        }
    }

    __syncthreads();

    // 卷积核塞进寄存器，后续反复用
    float k_reg[MAX_KERNEL_SIZE];
    for (int k = 0; k < kernel_size; k++) {
        k_reg[k] = kernel[k];
    }

    // 每个线程算 TILE_SIZE 个输出
    for (int t = 0; t < TILE_SIZE; t++) {
        int idx = block_output_start + tid + t * blockDim.x;
        if (idx < input_size) {
            float sum = 0.0f;
            int shared_base = tid + t * blockDim.x;
            for (int k = 0; k < kernel_size; k++) {
                sum += shared_input[shared_base + k] * k_reg[k];
            }
            output[idx] = sum;
        }
    }
}

// -----------------------------------------------------------
// 工具函数
// -----------------------------------------------------------

// 随机初始化数组
void init_array(float *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = (float)(rand() % 100) / 10.0f;
    }
}

// 随机初始化卷积核（有正有负）
void init_kernel(float *kernel, int size) {
    for (int i = 0; i < size; i++) {
        kernel[i] = (float)(rand() % 50) / 10.0f - 2.5f;
    }
}

// 对比 GPU 结果和 CPU 结果
bool verify_result(float *gpu_out, float *cpu_out, int size, float eps) {
    for (int i = 0; i < size; i++) {
        float diff = fabsf(gpu_out[i] - cpu_out[i]);
        float rel = diff / (fabsf(cpu_out[i]) + 1e-8f);
        if (rel > eps && diff > eps) {
            printf("  第 %d 个元素不一致: GPU=%f, CPU=%f\n", i, gpu_out[i], cpu_out[i]);
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------
// 跑一次 benchmark，返回平均时间（ms）
// -----------------------------------------------------------
typedef struct {
    const char *name;
    bool is_constant;
    bool is_shared;
    bool is_register;
    int shared_mem_bytes;
    int block_size;
} KernelConfig;

float benchmark_kernel(KernelConfig *cfg, float *d_input, float *d_kernel,
                       float *d_output, int input_size, int kernel_size,
                       int num_warmup, int num_runs) {
    int grid_size = (input_size + cfg->block_size - 1) / cfg->block_size;
    cudaEvent_t start, stop;
    CHECK_CUDA(cudaEventCreate(&start));
    CHECK_CUDA(cudaEventCreate(&stop));

    // 预热，让 GPU 进入稳定状态
    for (int i = 0; i < num_warmup; i++) {
        if (cfg->is_constant) {
            convolution_constant<<<grid_size, cfg->block_size>>>(
                d_input, d_output, input_size, kernel_size);
        } else if (cfg->is_register) {
            convolution_register<<<grid_size, cfg->block_size, cfg->shared_mem_bytes>>>(
                d_input, d_kernel, d_output, input_size, kernel_size);
        } else if (cfg->is_shared) {
            convolution_shared<<<grid_size, cfg->block_size, cfg->shared_mem_bytes>>>(
                d_input, d_kernel, d_output, input_size, kernel_size);
        } else {
            convolution_basic<<<grid_size, cfg->block_size>>>(
                d_input, d_kernel, d_output, input_size, kernel_size);
        }
        CHECK_CUDA(cudaDeviceSynchronize());
    }

    // 正式计时
    float total_time = 0.0f;
    for (int i = 0; i < num_runs; i++) {
        CHECK_CUDA(cudaEventRecord(start, 0));
        if (cfg->is_constant) {
            convolution_constant<<<grid_size, cfg->block_size>>>(
                d_input, d_output, input_size, kernel_size);
        } else if (cfg->is_register) {
            convolution_register<<<grid_size, cfg->block_size, cfg->shared_mem_bytes>>>(
                d_input, d_kernel, d_output, input_size, kernel_size);
        } else if (cfg->is_shared) {
            convolution_shared<<<grid_size, cfg->block_size, cfg->shared_mem_bytes>>>(
                d_input, d_kernel, d_output, input_size, kernel_size);
        } else {
            convolution_basic<<<grid_size, cfg->block_size>>>(
                d_input, d_kernel, d_output, input_size, kernel_size);
        }
        CHECK_CUDA(cudaEventRecord(stop, 0));
        CHECK_CUDA(cudaEventSynchronize(stop));

        float elapsed;
        CHECK_CUDA(cudaEventElapsedTime(&elapsed, start, stop));
        total_time += elapsed;
    }

    CHECK_CUDA(cudaEventDestroy(start));
    CHECK_CUDA(cudaEventDestroy(stop));
    return total_time / (float)num_runs;
}

// -----------------------------------------------------------
// 占用率分析：不同 block size 对性能的影响
// -----------------------------------------------------------
void analyze_occupancy(float *d_input, float *d_kernel, float *d_output,
                        int input_size, int kernel_size) {
    int block_sizes[] = {32, 64, 128, 256, 512, 1024};
    int num_bs = sizeof(block_sizes) / sizeof(block_sizes[0]);

    printf("\n=== 占用率分析 (input=%d, kernel=%d) ===\n", input_size, kernel_size);

    for (int v = 0; v < 3; v++) {
        const char *ver_name = (v == 0) ? "基础版本" :
                               (v == 1) ? "常量内存" : "共享内存";
        printf("\n  %s:\n", ver_name);
        printf("  %-10s %-10s %-10s\n", "BlockSize", "GridSize", "Time(ms)");

        for (int b = 0; b < num_bs; b++) {
            int bs = block_sizes[b];
            int gs = (input_size + bs - 1) / bs;
            int shared_bytes = (bs + kernel_size - 1) * sizeof(float);

            cudaEvent_t start, stop;
            CHECK_CUDA(cudaEventCreate(&start));
            CHECK_CUDA(cudaEventCreate(&stop));

            for (int w = 0; w < 5; w++) {
                if (v == 0)
                    convolution_basic<<<gs, bs>>>(d_input, d_kernel, d_output,
                                                   input_size, kernel_size);
                else if (v == 1)
                    convolution_constant<<<gs, bs>>>(d_input, d_output,
                                                      input_size, kernel_size);
                else
                    convolution_shared<<<gs, bs, shared_bytes>>>(
                        d_input, d_kernel, d_output, input_size, kernel_size);
                CHECK_CUDA(cudaDeviceSynchronize());
            }

            CHECK_CUDA(cudaEventRecord(start, 0));
            for (int r = 0; r < 50; r++) {
                if (v == 0)
                    convolution_basic<<<gs, bs>>>(d_input, d_kernel, d_output,
                                                   input_size, kernel_size);
                else if (v == 1)
                    convolution_constant<<<gs, bs>>>(d_input, d_output,
                                                      input_size, kernel_size);
                else
                    convolution_shared<<<gs, bs, shared_bytes>>>(
                        d_input, d_kernel, d_output, input_size, kernel_size);
            }
            CHECK_CUDA(cudaEventRecord(stop, 0));
            CHECK_CUDA(cudaEventSynchronize(stop));

            float elapsed;
            CHECK_CUDA(cudaEventElapsedTime(&elapsed, start, stop));
            CHECK_CUDA(cudaEventDestroy(start));
            CHECK_CUDA(cudaEventDestroy(stop));
            printf("  %-10d %-10d %-10.6f\n", bs, gs, elapsed / 50.0f);
        }
    }
}

// -----------------------------------------------------------
// 主函数
// -----------------------------------------------------------
int main(int argc, char **argv) {
    // 打印 GPU 信息
    cudaDeviceProp prop;
    int device;
    CHECK_CUDA(cudaGetDevice(&device));
    CHECK_CUDA(cudaGetDeviceProperties(&prop, device));
    printf("GPU: %s\n", prop.name);
    printf("计算能力: %d.%d\n", prop.major, prop.minor);
    printf("最大线程数/块: %d\n", prop.maxThreadsPerBlock);
    printf("最大共享内存/块: %zu KB\n", prop.sharedMemPerBlock / 1024);
    printf("SM 数量: %d\n", prop.multiProcessorCount);
    printf("时钟频率: %.2f GHz\n", prop.clockRate / 1e6);
    printf("全局内存: %.2f GB\n\n", prop.totalGlobalMem / 1e9);

    // 测试用的参数
    int input_sizes[] = {1024, 4096, 16384, 65536, 262144};
    int kernel_sizes[] = {3, 5, 7, 9, 11};
    int num_inputs = sizeof(input_sizes) / sizeof(input_sizes[0]);
    int num_kernels = sizeof(kernel_sizes) / sizeof(kernel_sizes[0]);

    int num_warmup = 5;
    int num_runs = 20;
    srand(42);

    // ============================================================
    // 第一步：验证正确性
    // ============================================================
    printf("=== 正确性验证 ===\n");
    int test_size = 1024;
    int test_kernel = 7;

    float *h_input = (float*)malloc(test_size * sizeof(float));
    float *h_kernel = (float*)malloc(test_kernel * sizeof(float));
    float *h_cpu_out = (float*)malloc(test_size * sizeof(float));
    float *h_gpu_out = (float*)malloc(test_size * sizeof(float));

    init_array(h_input, test_size);
    init_kernel(h_kernel, test_kernel);

    // CPU 参考结果
    convolution_cpu(h_input, h_kernel, h_cpu_out, test_size, test_kernel);

    // 分配 GPU 内存
    float *d_input, *d_kernel, *d_output;
    CHECK_CUDA(cudaMalloc(&d_input, test_size * sizeof(float)));
    CHECK_CUDA(cudaMalloc(&d_kernel, test_kernel * sizeof(float)));
    CHECK_CUDA(cudaMalloc(&d_output, test_size * sizeof(float)));

    CHECK_CUDA(cudaMemcpy(d_input, h_input, test_size * sizeof(float),
                          cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_kernel, h_kernel, test_kernel * sizeof(float),
                          cudaMemcpyHostToDevice));

    int grid = (test_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // 版本一
    convolution_basic<<<grid, BLOCK_SIZE>>>(d_input, d_kernel, d_output,
                                             test_size, test_kernel);
    CHECK_CUDA(cudaMemcpy(h_gpu_out, d_output, test_size * sizeof(float),
                          cudaMemcpyDeviceToHost));
    bool ok1 = verify_result(h_gpu_out, h_cpu_out, test_size, 1e-5f);
    printf("  版本一（基础）:    %s\n", ok1 ? "通过" : "失败");

    // 版本二
    CHECK_CUDA(cudaMemcpyToSymbol(d_const_kernel, h_kernel,
                                   test_kernel * sizeof(float)));
    convolution_constant<<<grid, BLOCK_SIZE>>>(d_input, d_output,
                                                test_size, test_kernel);
    CHECK_CUDA(cudaMemcpy(h_gpu_out, d_output, test_size * sizeof(float),
                          cudaMemcpyDeviceToHost));
    bool ok2 = verify_result(h_gpu_out, h_cpu_out, test_size, 1e-5f);
    printf("  版本二（常量内存）: %s\n", ok2 ? "通过" : "失败");

    // 版本三
    int shared_bytes3 = (BLOCK_SIZE + test_kernel - 1) * sizeof(float);
    convolution_shared<<<grid, BLOCK_SIZE, shared_bytes3>>>(
        d_input, d_kernel, d_output, test_size, test_kernel);
    CHECK_CUDA(cudaMemcpy(h_gpu_out, d_output, test_size * sizeof(float),
                          cudaMemcpyDeviceToHost));
    bool ok3 = verify_result(h_gpu_out, h_cpu_out, test_size, 1e-5f);
    printf("  版本三（共享内存）: %s\n", ok3 ? "通过" : "失败");

    // 版本四
    int grid4 = (test_size + BLOCK_SIZE * TILE_SIZE - 1) / (BLOCK_SIZE * TILE_SIZE);
    int shared_bytes4 = (BLOCK_SIZE * TILE_SIZE + test_kernel - 1) * sizeof(float);
    convolution_register<<<grid4, BLOCK_SIZE, shared_bytes4>>>(
        d_input, d_kernel, d_output, test_size, test_kernel);
    CHECK_CUDA(cudaMemcpy(h_gpu_out, d_output, test_size * sizeof(float),
                          cudaMemcpyDeviceToHost));
    bool ok4 = verify_result(h_gpu_out, h_cpu_out, test_size, 1e-5f);
    printf("  版本四（寄存器分块）: %s\n", ok4 ? "通过" : "失败");

    CHECK_CUDA(cudaFree(d_input));
    CHECK_CUDA(cudaFree(d_kernel));
    CHECK_CUDA(cudaFree(d_output));
    free(h_input); free(h_kernel); free(h_cpu_out); free(h_gpu_out);

    if (!ok1 || !ok2 || !ok3 || !ok4) {
        printf("有版本没通过验证，先检查代码吧。\n");
        return 1;
    }
    printf("四个版本全部通过!\n\n");

    // ============================================================
    // 第二步：性能测试
    // ============================================================
    printf("=== 性能测试 ===\n");
    printf("%-10s %-8s %-12s %-12s %-12s %-12s %-8s %-8s %-8s\n",
           "规模", "核大小", "基础(ms)", "常量(ms)", "共享(ms)",
           "寄存器(ms)", "C/B", "S/B", "R/B");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int is = 0; is < num_inputs; is++) {
        int N = input_sizes[is];

        float *h_in = (float*)malloc(N * sizeof(float));
        float *h_out_cpu = (float*)malloc(N * sizeof(float));
        float *h_out_gpu = (float*)malloc(N * sizeof(float));
        init_array(h_in, N);

        for (int ks = 0; ks < num_kernels; ks++) {
            int K = kernel_sizes[ks];

            float *h_ker = (float*)malloc(K * sizeof(float));
            init_kernel(h_ker, K);

            // CPU 参考
            convolution_cpu(h_in, h_ker, h_out_cpu, N, K);

            float *d_in, *d_ker, *d_out;
            CHECK_CUDA(cudaMalloc(&d_in, N * sizeof(float)));
            CHECK_CUDA(cudaMalloc(&d_ker, K * sizeof(float)));
            CHECK_CUDA(cudaMalloc(&d_out, N * sizeof(float)));
            CHECK_CUDA(cudaMemcpy(d_in, h_in, N * sizeof(float), cudaMemcpyHostToDevice));
            CHECK_CUDA(cudaMemcpy(d_ker, h_ker, K * sizeof(float), cudaMemcpyHostToDevice));

            int gs_basic = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
            int shm = (BLOCK_SIZE + K - 1) * sizeof(float);
            int gs_tile = (N + BLOCK_SIZE * TILE_SIZE - 1) / (BLOCK_SIZE * TILE_SIZE);
            int shm_tile = (BLOCK_SIZE * TILE_SIZE + K - 1) * sizeof(float);

            KernelConfig configs[4] = {
                {"基础",   false, false, false, 0,         BLOCK_SIZE},
                {"常量",   true,  false, false, 0,         BLOCK_SIZE},
                {"共享",   false, true,  false, shm,       BLOCK_SIZE},
                {"寄存器", false, false, true,  shm_tile,  BLOCK_SIZE},
            };

            float times[4];

            times[0] = benchmark_kernel(&configs[0], d_in, d_ker, d_out,
                                        N, K, num_warmup, num_runs);

            CHECK_CUDA(cudaMemcpyToSymbol(d_const_kernel, h_ker, K * sizeof(float)));
            times[1] = benchmark_kernel(&configs[1], d_in, d_ker, d_out,
                                        N, K, num_warmup, num_runs);

            times[2] = benchmark_kernel(&configs[2], d_in, d_ker, d_out,
                                        N, K, num_warmup, num_runs);

            times[3] = benchmark_kernel(&configs[3], d_in, d_ker, d_out,
                                        N, K, num_warmup, num_runs);

            float sp_c = times[0] / times[1];  // 常量 / 基础
            float sp_s = times[0] / times[2];  // 共享 / 基础
            float sp_r = times[0] / times[3];  // 寄存器 / 基础

            printf("%-10d %-8d %-12.6f %-12.6f %-12.6f %-12.6f %-8.2f %-8.2f %-8.2f\n",
                   N, K, times[0], times[1], times[2], times[3],
                   sp_c, sp_s, sp_r);

            // 跑都跑了，顺便验证一下共享和寄存器版本在当前参数下的正确性
            convolution_shared<<<gs_basic, BLOCK_SIZE, shm>>>(
                d_in, d_ker, d_out, N, K);
            CHECK_CUDA(cudaMemcpy(h_out_gpu, d_out, N * sizeof(float), cudaMemcpyDeviceToHost));
            if (!verify_result(h_out_gpu, h_out_cpu, N, 1e-4f))
                printf("  警告: 共享版本 N=%d K=%d 验证失败\n", N, K);

            convolution_register<<<gs_tile, BLOCK_SIZE, shm_tile>>>(
                d_in, d_ker, d_out, N, K);
            CHECK_CUDA(cudaMemcpy(h_out_gpu, d_out, N * sizeof(float), cudaMemcpyDeviceToHost));
            if (!verify_result(h_out_gpu, h_out_cpu, N, 1e-4f))
                printf("  警告: 寄存器分块版本 N=%d K=%d 验证失败\n", N, K);

            CHECK_CUDA(cudaFree(d_in)); CHECK_CUDA(cudaFree(d_ker)); CHECK_CUDA(cudaFree(d_out));
            free(h_ker);
        }
        free(h_in); free(h_out_cpu); free(h_out_gpu);
    }

    printf("\n");

    // ============================================================
    // 第三步：占用率分析
    // ============================================================
    int occ_N = 65536, occ_K = 7;
    float *h_occ_in = (float*)malloc(occ_N * sizeof(float));
    float *h_occ_ker = (float*)malloc(occ_K * sizeof(float));
    init_array(h_occ_in, occ_N);
    init_kernel(h_occ_ker, occ_K);

    float *d_occ_in, *d_occ_ker, *d_occ_out;
    CHECK_CUDA(cudaMalloc(&d_occ_in, occ_N * sizeof(float)));
    CHECK_CUDA(cudaMalloc(&d_occ_ker, occ_K * sizeof(float)));
    CHECK_CUDA(cudaMalloc(&d_occ_out, occ_N * sizeof(float)));
    CHECK_CUDA(cudaMemcpy(d_occ_in, h_occ_in, occ_N * sizeof(float), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_occ_ker, h_occ_ker, occ_K * sizeof(float), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpyToSymbol(d_const_kernel, h_occ_ker, occ_K * sizeof(float)));

    analyze_occupancy(d_occ_in, d_occ_ker, d_occ_out, occ_N, occ_K);

    CHECK_CUDA(cudaFree(d_occ_in)); CHECK_CUDA(cudaFree(d_occ_ker)); CHECK_CUDA(cudaFree(d_occ_out));
    free(h_occ_in); free(h_occ_ker);

    printf("\n全部跑完了。\n");
    return 0;
}
