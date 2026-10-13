/*
 * 二维直接卷积 —— 线程块大小 & 任务映射对比
 * 中山大学 计算机学院 并行程序设计与算法实验
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

#define CHECK_CUDA(call) do {                                         \
    cudaError_t err = call;                                           \
    if (err != cudaSuccess) {                                         \
        fprintf(stderr, "CUDA 错误 %s:%d - %s\n",                     \
                __FILE__, __LINE__, cudaGetErrorString(err));         \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
} while(0)

// 全局变量，供 launch 封装函数使用
static float *g_input, *g_kernel, *g_output;
static int g_N, g_C=3, g_K=3, g_P=1, g_S, g_Nout;
static dim3 g_grid, g_block;

// 前向声明
__global__ void conv2d_1pixel(float*,float*,float*,int,int,int,int,int,int);
__global__ void conv2d_1row(float*,float*,float*,int,int,int,int,int,int);
__global__ void conv2d_tile(float*,float*,float*,int,int,int,int,int,int);

void launch_1pixel() {
    conv2d_1pixel<<<g_grid, g_block>>>(g_input, g_kernel, g_output,
                                        g_N, g_C, g_K, g_P, g_S, g_Nout);
}
void launch_1row() {
    conv2d_1row<<<g_grid, g_block>>>(g_input, g_kernel, g_output,
                                      g_N, g_C, g_K, g_P, g_S, g_Nout);
}
void launch_tile() {
    conv2d_tile<<<g_grid, g_block>>>(g_input, g_kernel, g_output,
                                      g_N, g_C, g_K, g_P, g_S, g_Nout);
}

// -----------------------------------------------------------
// CPU 参考实现
// -----------------------------------------------------------
void conv2d_cpu(float *input, float *kernel, float *output,
                int N, int C, int K_size, int P, int S) {
    int N_out = (N + 2 * P - K_size) / S + 1;
    for (int oh = 0; oh < N_out; oh++)
        for (int ow = 0; ow < N_out; ow++) {
            float sum = 0.0f;
            for (int c = 0; c < C; c++)
                for (int kh = 0; kh < K_size; kh++)
                    for (int kw = 0; kw < K_size; kw++) {
                        int ih = oh * S + kh - P, iw = ow * S + kw - P;
                        if ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                            sum += input[c * N * N + ih * N + iw] *
                                   kernel[c * K_size * K_size + kh * K_size + kw];
                    }
            output[oh * N_out + ow] = sum;
        }
}

// -----------------------------------------------------------
// 策略一：每线程一个输出像素，2D 线程块 (BX, BY)
// -----------------------------------------------------------
__global__ void conv2d_1pixel(float *input, float *kernel, float *output,
                               int N, int C, int K_size, int P, int S,
                               int N_out) {
    int ow = blockIdx.x * blockDim.x + threadIdx.x;
    int oh = blockIdx.y * blockDim.y + threadIdx.y;
    if (oh >= N_out || ow >= N_out) return;

    float sum = 0.0f;
    for (int c = 0; c < C; c++)
        for (int kh = 0; kh < K_size; kh++)
            for (int kw = 0; kw < K_size; kw++) {
                int ih = oh * S + kh - P, iw = ow * S + kw - P;
                if ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                    sum += input[c * N * N + ih * N + iw] *
                           kernel[c * K_size * K_size + kh * K_size + kw];
            }
    output[oh * N_out + ow] = sum;
}

// -----------------------------------------------------------
// 策略二：grid.y 映射到输出行，block = (BX, 1)
// -----------------------------------------------------------
__global__ void conv2d_1row(float *input, float *kernel, float *output,
                             int N, int C, int K_size, int P, int S,
                             int N_out) {
    int ow = blockIdx.x * blockDim.x + threadIdx.x;
    int oh = blockIdx.y;
    if (oh >= N_out || ow >= N_out) return;

    float sum = 0.0f;
    for (int c = 0; c < C; c++)
        for (int kh = 0; kh < K_size; kh++)
            for (int kw = 0; kw < K_size; kw++) {
                int ih = oh * S + kh - P, iw = ow * S + kw - P;
                if ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                    sum += input[c * N * N + ih * N + iw] *
                           kernel[c * K_size * K_size + kh * K_size + kw];
            }
    output[oh * N_out + ow] = sum;
}

// -----------------------------------------------------------
// 策略三：寄存器分块，每线程算 2x2=4 个输出像素
// -----------------------------------------------------------
__global__ void conv2d_tile(float *input, float *kernel, float *output,
                             int N, int C, int K_size, int P, int S,
                             int N_out) {
    int ow0 = (blockIdx.x * blockDim.x + threadIdx.x) * 2;
    int oh0 = (blockIdx.y * blockDim.y + threadIdx.y) * 2;

    float k_reg[27];
    int k_total = C * K_size * K_size;
    for (int i = 0; i < k_total; i++) k_reg[i] = kernel[i];

    float s00 = 0, s01 = 0, s10 = 0, s11 = 0;

    for (int c = 0; c < C; c++) {
        for (int kh = 0; kh < K_size; kh++) {
            for (int kw = 0; kw < K_size; kw++) {
                float kv = k_reg[c * K_size * K_size + kh * K_size + kw];
                int ih0 = oh0 * S + kh - P;
                int ih1 = (oh0 + 1) * S + kh - P;
                int iw0 = ow0 * S + kw - P;
                int iw1 = (ow0 + 1) * S + kw - P;

                if (ih0 >= 0 && ih0 < N) {
                    if (iw0 >= 0 && iw0 < N && oh0 < N_out && ow0 < N_out)
                        s00 += input[c * N * N + ih0 * N + iw0] * kv;
                    if (iw1 >= 0 && iw1 < N && oh0 < N_out && ow0 + 1 < N_out)
                        s01 += input[c * N * N + ih0 * N + iw1] * kv;
                }
                if (ih1 >= 0 && ih1 < N && oh0 + 1 < N_out) {
                    if (iw0 >= 0 && iw0 < N && ow0 < N_out)
                        s10 += input[c * N * N + ih1 * N + iw0] * kv;
                    if (iw1 >= 0 && iw1 < N && ow0 + 1 < N_out)
                        s11 += input[c * N * N + ih1 * N + iw1] * kv;
                }
            }
        }
    }

    if (oh0 < N_out && ow0 < N_out)       output[oh0 * N_out + ow0] = s00;
    if (oh0 < N_out && ow0 + 1 < N_out)   output[oh0 * N_out + ow0 + 1] = s01;
    if (oh0 + 1 < N_out && ow0 < N_out)   output[(oh0 + 1) * N_out + ow0] = s10;
    if (oh0 + 1 < N_out && ow0 + 1 < N_out) output[(oh0 + 1) * N_out + ow0 + 1] = s11;
}

// 验证
bool verify(float *gpu, float *cpu, int size, float eps) {
    for (int i = 0; i < size; i++) {
        float diff = fabsf(gpu[i] - cpu[i]);
        float rel = diff / (fabsf(cpu[i]) + 1e-8f);
        if (rel > eps && diff > eps) return false;
    }
    return true;
}

// 计时：预热后跑 runs 次取平均
float bench_ms(void (*fn)(), int warmup, int runs) {
    cudaEvent_t start, stop;
    CHECK_CUDA(cudaEventCreate(&start)); CHECK_CUDA(cudaEventCreate(&stop));
    for (int w = 0; w < warmup; w++) fn();
    CHECK_CUDA(cudaDeviceSynchronize());
    CHECK_CUDA(cudaEventRecord(start, 0));
    for (int r = 0; r < runs; r++) fn();
    CHECK_CUDA(cudaEventRecord(stop, 0));
    CHECK_CUDA(cudaEventSynchronize(stop));
    float ms;
    CHECK_CUDA(cudaEventElapsedTime(&ms, start, stop));
    CHECK_CUDA(cudaEventDestroy(start)); CHECK_CUDA(cudaEventDestroy(stop));
    return ms / (float)runs;
}

int main() {
    cudaDeviceProp prop; int dev;
    CHECK_CUDA(cudaGetDevice(&dev));
    CHECK_CUDA(cudaGetDeviceProperties(&prop, dev));
    printf("GPU: %s (计算能力 %d.%d, %d 个 SM)\n\n",
           prop.name, prop.major, prop.minor, prop.multiProcessorCount);

    const int C = 3, K_size = 3, P = 1;
    int warmup = 5, runs = 50;

    int max_N = 1024, max_out = max_N;
    size_t inB = max_N * max_N * C * sizeof(float);
    size_t kerB = C * K_size * K_size * sizeof(float);
    size_t outB = max_out * max_out * sizeof(float);

    float *h_in = (float*)malloc(inB);
    float *h_ker = (float*)malloc(kerB);
    float *h_cpu = (float*)malloc(outB);
    float *h_gpu = (float*)malloc(outB);

    srand(42);
    for (int i = 0; i < max_N * max_N * C; i++) h_in[i] = (float)(rand() % 100) / 10.0f;
    for (int i = 0; i < C * K_size * K_size; i++) h_ker[i] = (float)(rand() % 50) / 100.0f - 0.25f;

    CHECK_CUDA(cudaMalloc(&g_input, inB));
    CHECK_CUDA(cudaMalloc(&g_kernel, kerB));
    CHECK_CUDA(cudaMalloc(&g_output, outB));
    CHECK_CUDA(cudaMemcpy(g_input, h_in, inB, cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(g_kernel, h_ker, kerB, cudaMemcpyHostToDevice));

    // ============================================================
    // 实验一：线程块大小的影响
    // ============================================================
    printf("=============================================================\n");
    printf("实验一：线程块大小对直接卷积的影响\n");
    printf("固定：输入 512x512x3, stride=1, 每线程 1 像素\n");
    printf("=============================================================\n");
    printf("%-16s %-10s %-12s\n", "输入", "线程块", "耗时(ms)");
    printf("-------------------------------------------------------\n");

    g_N = 512; g_S = 1;
    g_Nout = (g_N + 2 * P - K_size) / g_S + 1;

    for (int bi = 0; bi < 3; bi++) {
        int BX = (bi == 0) ? 8 : (bi == 1) ? 16 : 32;
        g_block = dim3(BX, BX);
        g_grid = dim3((g_Nout + BX - 1) / BX, (g_Nout + BX - 1) / BX);
        float t = bench_ms(launch_1pixel, warmup, runs);
        printf("%-16s %-10s %-12.6f\n", "512x512",
               (BX == 8 ? "8x8" : BX == 16 ? "16x16" : "32x32"), t);
    }

    // 验证
    conv2d_cpu(h_in, h_ker, h_cpu, g_N, C, K_size, P, g_S);
    g_block = dim3(16, 16);
    g_grid = dim3((g_Nout + 16 - 1) / 16, (g_Nout + 16 - 1) / 16);
    launch_1pixel();
    CHECK_CUDA(cudaMemcpy(h_gpu, g_output, g_Nout*g_Nout*sizeof(float), cudaMemcpyDeviceToHost));
    printf("  验证: %s\n\n", verify(h_gpu, h_cpu, g_Nout*g_Nout, 1e-4f) ? "通过" : "失败");

    // ============================================================
    // 实验二：任务映射方式的影响
    // ============================================================
    printf("=============================================================\n");
    printf("实验二：任务映射方式的影响\n");
    printf("固定：输入 512x512x3, stride=1, 约 256 线程/块\n");
    printf("=============================================================\n");
    printf("%-6s %-46s %-12s\n", "编号", "映射方式", "耗时(ms)");
    printf("----------------------------------------------------------------\n");

    g_block = dim3(16, 16);
    g_grid = dim3((g_Nout + 16 - 1) / 16, (g_Nout + 16 - 1) / 16);
    printf("%-6s %-46s %-12.6f\n", "方式一",
           "每线程 1 像素, 2D 块 16x16", bench_ms(launch_1pixel, warmup, runs));

    g_block = dim3(256, 1);
    g_grid = dim3((g_Nout + 256 - 1) / 256, g_Nout);
    printf("%-6s %-46s %-12.6f\n", "方式二",
           "每线程 1 像素, grid.y=输出行", bench_ms(launch_1row, warmup, runs));

    int N_tile = (g_Nout + 1) / 2;
    g_block = dim3(16, 16);
    g_grid = dim3((N_tile + 16 - 1) / 16, (N_tile + 16 - 1) / 16);
    float t3 = bench_ms(launch_tile, warmup, runs);
    printf("%-6s %-46s %-12.6f\n", "方式三",
           "每线程 2x2=4 像素(寄存器分块)", t3);

    // 验证分块版本
    launch_tile();
    CHECK_CUDA(cudaMemcpy(h_gpu, g_output, g_Nout*g_Nout*sizeof(float), cudaMemcpyDeviceToHost));
    printf("  方式三验证: %s\n\n", verify(h_gpu, h_cpu, g_Nout*g_Nout, 1e-4f) ? "通过" : "失败");

    // ============================================================
    // 实验三：不同尺寸和 stride 的完整数据
    // ============================================================
    printf("=============================================================\n");
    printf("实验三：不同输入尺寸和 stride 下的直接卷积\n");
    printf("=============================================================\n");
    printf("%-14s %-8s %-10s %-14s\n", "输入", "Stride", "N_out", "耗时(ms)");
    printf("--------------------------------------------------------\n");

    int sizes[] = {64, 128, 256, 512, 1024};
    int strides[] = {1, 2, 3};

    for (int is = 0; is < 5; is++) {
        g_N = sizes[is];
        for (int ss = 0; ss < 3; ss++) {
            g_S = strides[ss];
            g_Nout = (g_N + 2 * P - K_size) / g_S + 1;
            if (g_Nout <= 0) continue;

            g_block = dim3(16, 16);
            g_grid = dim3((g_Nout + 16 - 1) / 16, (g_Nout + 16 - 1) / 16);
            float t = bench_ms(launch_1pixel, warmup, runs);

            char buf[16]; sprintf(buf, "%dx%d", g_N, g_N);
            printf("%-14s %-8d %-10d %-14.6f\n", buf, g_S, g_Nout, t);
        }
    }

    CHECK_CUDA(cudaFree(g_input)); CHECK_CUDA(cudaFree(g_kernel)); CHECK_CUDA(cudaFree(g_output));
    free(h_in); free(h_ker); free(h_cpu); free(h_gpu);
    printf("\n全部完成。\n"); return 0;
}
