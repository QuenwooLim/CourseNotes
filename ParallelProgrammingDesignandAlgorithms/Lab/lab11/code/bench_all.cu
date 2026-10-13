/*
 * 二维卷积完整对比测试 —— 直接卷积 vs im2col+GEMV
 * 中山大学 计算机学院 并行程序设计与算法实验
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cuda_runtime.h>

#define CHECK_CUDA(call) do { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
        fprintf(stderr, "CUDA 错误 %s:%d - %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

// 全局变量，方便 launch 函数用
static float *g_in, *g_ker, *g_out, *g_col;
static int g_N, g_C, g_K, g_P, g_S, g_Nout, g_M, g_Kdim;
static dim3 g_grid, g_block;

// 前向声明
__global__ void conv2d_direct(float*,float*,float*,int,int,int,int,int,int);
__global__ void im2col(float*,float*,int,int,int,int,int,int);
__global__ void gemv_simple(float*,float*,float*,int,int);

// 封装一下，方便传给计时函数
void launch_direct() {
    conv2d_direct<<<g_grid, g_block>>>(g_in, g_ker, g_out, g_N, g_C, g_K, g_P, g_S, g_Nout);
}
void launch_im2col() {
    im2col<<<g_grid, g_block>>>(g_in, g_col, g_N, g_C, g_K, g_P, g_S, g_Nout);
}
void launch_gemv() {
    gemv_simple<<<g_grid, g_block>>>(g_col, g_ker, g_out, g_M, g_Kdim);
}

// -----------------------------------------------------------
// CPU 参考实现
// -----------------------------------------------------------
void conv2d_cpu(float *in, float *ker, float *out, int N, int C, int K, int P, int S) {
    int N_out = (N + 2*P - K) / S + 1;
    for (int oh = 0; oh < N_out; oh++)
        for (int ow = 0; ow < N_out; ow++) {
            float sum = 0.0f;
            for (int c = 0; c < C; c++)
                for (int kh = 0; kh < K; kh++)
                    for (int kw = 0; kw < K; kw++) {
                        int ih = oh*S + kh - P, iw = ow*S + kw - P;
                        if ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                            sum += in[c*N*N + ih*N + iw] * ker[c*K*K + kh*K + kw];
                    }
            out[oh*N_out + ow] = sum;
        }
}

// -----------------------------------------------------------
// 直接卷积 kernel：每个线程算一个输出像素
// 输入布局 NCHW，out_pixel(oh,ow) = sum_c sum_kh,kw in[c][ih][iw] * ker[c][kh][kw]
// -----------------------------------------------------------
__global__ void conv2d_direct(float *in, float *ker, float *out,
                               int N, int C, int K, int P, int S, int N_out) {
    int ow = blockIdx.x*blockDim.x + threadIdx.x;
    int oh = blockIdx.y*blockDim.y + threadIdx.y;
    if (oh >= N_out || ow >= N_out) return;
    float sum = 0.0f;
    for (int c = 0; c < C; c++)
        for (int kh = 0; kh < K; kh++)
            for (int kw = 0; kw < K; kw++) {
                int ih = oh*S + kh - P, iw = ow*S + kw - P;
                if ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                    sum += in[c*N*N + ih*N + iw] * ker[c*K*K + kh*K + kw];
            }
    out[oh*N_out + ow] = sum;
}

// -----------------------------------------------------------
// im2col kernel：把每个滑动窗口展开成 col_matrix 的一行
// col 按行主序存储：[M_patches][K_dim]，每个 patch 的 K_dim 个元素连续
// -----------------------------------------------------------
__global__ void im2col(float *in, float *col,
                        int N, int C, int K, int P, int S, int N_out) {
    int idx = blockIdx.x*blockDim.x + threadIdx.x;
    int M = N_out * N_out;
    if (idx >= M) return;
    int oh = idx / N_out, ow = idx % N_out;
    int K_dim = C*K*K;
    int base = idx * K_dim;
    for (int c = 0; c < C; c++)
        for (int kh = 0; kh < K; kh++)
            for (int kw = 0; kw < K; kw++) {
                int ih = oh*S + kh - P, iw = ow*S + kw - P;
                float val = 0.0f;
                if ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                    val = in[c*N*N + ih*N + iw];
                col[base + c*K*K + kh*K + kw] = val;
            }
}

// -----------------------------------------------------------
// 矩阵-向量乘法：out = col * ker
// col: M 行 K_dim 列，ker: K_dim 维向量
// 每个线程算一行（一个输出像素）
// -----------------------------------------------------------
__global__ void gemv_simple(float *col, float *ker, float *out, int M, int K_dim) {
    int idx = blockIdx.x*blockDim.x + threadIdx.x;
    if (idx >= M) return;
    float sum = 0.0f;
    float *row = col + idx * K_dim;
    for (int i = 0; i < K_dim; i++) sum += row[i] * ker[i];
    out[idx] = sum;
}

// 验证 GPU 结果是否和 CPU 一致
bool verify(float *g, float *c, int n, float eps) {
    for (int i = 0; i < n; i++) {
        float d = fabsf(g[i]-c[i]), r = d/(fabsf(c[i])+1e-8f);
        if (r > eps && d > eps) return false;
    }
    return true;
}

// 计时：预热 wup 次后跑 runs 次取平均，返回毫秒
float bench_ms(void (*fn)(), int wup, int runs) {
    cudaEvent_t s, e;
    CHECK_CUDA(cudaEventCreate(&s)); CHECK_CUDA(cudaEventCreate(&e));
    for (int w = 0; w < wup; w++) fn();
    CHECK_CUDA(cudaDeviceSynchronize());
    CHECK_CUDA(cudaEventRecord(s, 0));
    for (int r = 0; r < runs; r++) fn();
    CHECK_CUDA(cudaEventRecord(e, 0));
    CHECK_CUDA(cudaEventSynchronize(e));
    float ms;
    CHECK_CUDA(cudaEventElapsedTime(&ms, s, e));
    CHECK_CUDA(cudaEventDestroy(s)); CHECK_CUDA(cudaEventDestroy(e));
    return ms/(float)runs;
}

int main() {
    cudaDeviceProp prop; int dev;
    CHECK_CUDA(cudaGetDevice(&dev));
    CHECK_CUDA(cudaGetDeviceProperties(&prop, dev));
    printf("GPU: %s (%.1f GHz, %d 个 SM)\n\n",
           prop.name, prop.clockRate/1e6, prop.multiProcessorCount);

    const int C = 3, K_size = 3, P = 1;
    const int K_dim = C*K_size*K_size;  // 3*3*3 = 27
    int wup = 10, runs = 50;

    int max_N = 1024, max_out = max_N, max_M = max_out*max_out;
    size_t inB = max_N*max_N*C*sizeof(float);
    size_t kerB = K_dim*sizeof(float);
    size_t outB = max_M*sizeof(float);
    size_t colB = max_M*K_dim*sizeof(float);

    float *h_in = (float*)malloc(inB);
    float *h_ker = (float*)malloc(kerB);
    float *h_cpu = (float*)malloc(outB);
    float *h_gpu = (float*)malloc(outB);

    srand(42);
    for (int i = 0; i < max_N*max_N*C; i++) h_in[i] = (float)(rand()%100)/10.0f;
    for (int i = 0; i < K_dim; i++) h_ker[i] = (float)(rand()%50)/100.0f - 0.25f;

    CHECK_CUDA(cudaMalloc(&g_in, inB));   CHECK_CUDA(cudaMalloc(&g_ker, kerB));
    CHECK_CUDA(cudaMalloc(&g_out, outB)); CHECK_CUDA(cudaMalloc(&g_col, colB));
    CHECK_CUDA(cudaMemcpy(g_in, h_in, inB, cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(g_ker, h_ker, kerB, cudaMemcpyHostToDevice));

    // ============================================================
    // 实验一：线程块大小的影响（512x512, stride=1）
    // ============================================================
    printf("=== 实验一：线程块大小的影响（512x512, stride=1） ===\n");
    g_N = 512; g_S = 1; g_C = C; g_K = K_size; g_P = P;
    g_Nout = (g_N+2*P-K_size)/g_S + 1;

    for (int bi=0; bi<3; bi++) {
        int B = (bi==0)?8:(bi==1)?16:32;
        g_block = dim3(B, B);
        g_grid = dim3((g_Nout+B-1)/B, (g_Nout+B-1)/B);
        float t = bench_ms(launch_direct, wup, runs);
        printf("  线程块 %2dx%-2d (%d 线程): %.6f ms\n", B, B, B*B, t);
    }

    // 验证
    conv2d_cpu(h_in, h_ker, h_cpu, g_N, C, K_size, P, g_S);
    g_block = dim3(16,16); g_grid = dim3((g_Nout+16-1)/16,(g_Nout+16-1)/16);
    launch_direct();
    CHECK_CUDA(cudaMemcpy(h_gpu, g_out, g_Nout*g_Nout*sizeof(float), cudaMemcpyDeviceToHost));
    printf("  验证: %s\n\n", verify(h_gpu,h_cpu,g_Nout*g_Nout,1e-4f)?"通过":"失败");

    // ============================================================
    // 实验二：任务映射方式的影响（512x512, stride=1）
    // ============================================================
    printf("=== 实验二：任务映射方式的影响（512x512, stride=1） ===\n");

    // 方式一：2D 线程块 16x16
    g_block = dim3(16,16); g_grid = dim3((g_Nout+16-1)/16,(g_Nout+16-1)/16);
    printf("  方式一（2D 块 16x16, 每线程 1 像素）: %.6f ms\n",
           bench_ms(launch_direct, wup, runs));

    // 方式二：grid.y 映射到输出行, block.x=256 线程
    g_block = dim3(256,1); g_grid = dim3((g_Nout+256-1)/256, g_Nout);
    printf("  方式二（grid.y=输出行, 256 线程/行）: %.6f ms\n",
           bench_ms(launch_direct, wup, runs));

    // 方式三：等效 2x2 分块 grid
    { int NT=(g_Nout+1)/2; g_block=dim3(16,16); g_grid=dim3((NT+16-1)/16,(NT+16-1)/16);
      printf("  方式三（等效 2x2 分块 grid）: %.6f ms\n\n",
             bench_ms(launch_direct, wup, runs)); }

    // ============================================================
    // 实验三：直接卷积 vs im2col+GEMV 全面对比
    // ============================================================
    printf("=== 实验三：直接卷积 vs im2col+GEMV ===\n");
    printf("%-14s %-4s %-6s %-10s %-10s %-10s %-10s\n",
           "输入", "S", "Nout", "直接卷积", "im2col", "GEMV", "im2col总计");
    printf("---------------------------------------------------------------\n");

    int sizes[] = {64, 128, 256, 512, 1024};
    int strides[] = {1, 2, 3};
    char buf[32];

    for (int is=0; is<5; is++) {
        g_N = sizes[is];
        for (int ss=0; ss<3; ss++) {
            g_S = strides[ss];
            g_Nout = (g_N+2*P-K_size)/g_S + 1;
            if (g_Nout <= 0) continue;
            g_M = g_Nout * g_Nout;
            g_Kdim = K_dim;

            // 直接卷积
            g_block = dim3(16,16); g_grid = dim3((g_Nout+16-1)/16,(g_Nout+16-1)/16);
            float td = bench_ms(launch_direct, wup, runs);

            // im2col 展开
            int blk=256, grd=(g_M+blk-1)/blk;
            g_block = dim3(blk,1); g_grid = dim3(grd,1);
            float ti = bench_ms(launch_im2col, wup, runs);

            // GEMV 乘法
            float tg = bench_ms(launch_gemv, wup, runs);

            // 验证 im2col+GEMV 结果
            conv2d_cpu(h_in, h_ker, h_cpu, g_N, C, K_size, P, g_S);
            launch_im2col(); launch_gemv();
            CHECK_CUDA(cudaMemcpy(h_gpu, g_out, g_M*sizeof(float), cudaMemcpyDeviceToHost));
            bool ok = verify(h_gpu, h_cpu, g_M, 1e-4f);

            sprintf(buf, "%dx%d", g_N, g_N);
            printf("%-14s %-4d %-6d %-10.6f %-10.6f %-10.6f %-10.6f %s\n",
                   buf, g_S, g_Nout, td, ti, tg, ti+tg, ok?"OK":"FAIL");
        }
    }

    CHECK_CUDA(cudaFree(g_in)); CHECK_CUDA(cudaFree(g_ker));
    CHECK_CUDA(cudaFree(g_out)); CHECK_CUDA(cudaFree(g_col));
    free(h_in); free(h_ker); free(h_cpu); free(h_gpu);
    printf("\n全部完成。\n"); return 0;
}
