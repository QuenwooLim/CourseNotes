/*
 * 共享内存分块 2D 卷积
 * 中山大学 计算机学院 并行程序设计与算法实验
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

#define CHECK_CUDA(call) do { \
    cudaError_t e = call; \
    if (e != cudaSuccess) { fprintf(stderr,"CUDA 错误: %s\n",cudaGetErrorString(e)); exit(1); } \
} while(0)

// 分块参数：16x16 线程块 + 上下左右各 1 个 halo（3x3 卷积核半径=1）
#define BLK 16
#define HALO 1
#define TILE_W (BLK + 2*HALO)        // 18
#define SHMEM_SIZE (TILE_W * TILE_W * 3)  // 18*18*3 = 972 个 float

// -----------------------------------------------------------
// 共享内存分块卷积 kernel
// 先把输入的一个 tile（包含 halo）搬到共享内存，
// 然后所有线程从共享内存读数据做卷积
// -----------------------------------------------------------
__global__ void conv2d_shared(float *in, float *ker, float *out,
                               int N, int C, int K, int P, int S, int N_out) {
    __shared__ float tile[SHMEM_SIZE];
    int tx = threadIdx.x, ty = threadIdx.y;
    int ow = blockIdx.x * BLK + tx;
    int oh = blockIdx.y * BLK + ty;

    // 协作加载：每个线程搬若干元素到共享内存
    int total = TILE_W * TILE_W * C;
    int n_threads = BLK * BLK;
    for (int i = tx + ty * BLK; i < total; i += n_threads) {
        int c = i / (TILE_W * TILE_W);
        int r = (i % (TILE_W * TILE_W)) / TILE_W;
        int col = (i % (TILE_W * TILE_W)) % TILE_W;
        int ih = (blockIdx.y * BLK) + r - HALO;
        int iw = (blockIdx.x * BLK) + col - HALO;
        tile[i] = ((unsigned)ih < (unsigned)N && (unsigned)iw < (unsigned)N)
                  ? in[c * N * N + ih * N + iw] : 0.0f;
    }
    __syncthreads();

    // 从共享内存读数据计算本线程的输出像素
    if (oh < N_out && ow < N_out) {
        float sum = 0.0f;
        for (int c = 0; c < C; c++) {
            int c_off = c * TILE_W * TILE_W;
            for (int kh = 0; kh < K; kh++) {
                for (int kw = 0; kw < K; kw++) {
                    // 把 oh,ow 映射回 tile 里的坐标
                    int r_idx, c_idx;
                    if (S == 1) {
                        r_idx = ty + kh;           // S=1 时很简单
                        c_idx = tx + kw;
                    } else {
                        r_idx = (blockIdx.y * BLK + ty) * S + kh - P
                                - (blockIdx.y * BLK) + HALO;
                        c_idx = (blockIdx.x * BLK + tx) * S + kw - P
                                - (blockIdx.x * BLK) + HALO;
                    }
                    if ((unsigned)r_idx < (unsigned)TILE_W &&
                        (unsigned)c_idx < (unsigned)TILE_W) {
                        sum += tile[c_off + r_idx * TILE_W + c_idx]
                             * ker[c * K * K + kh * K + kw];
                    }
                    // 不在 tile 内的就是 padding 0，不用加
                }
            }
        }
        out[oh * N_out + ow] = sum;
    }
}

// CPU 参考（用来验证）
void conv2d_cpu(float *in, float *ker, float *out, int N, int C, int K, int P, int S) {
    int N_out = (N+2*P-K)/S+1;
    for (int oh=0; oh<N_out; oh++)
        for (int ow=0; ow<N_out; ow++) {
            float sum = 0;
            for (int c=0; c<C; c++)
                for (int kh=0; kh<K; kh++)
                    for (int kw=0; kw<K; kw++) {
                        int ih=oh*S+kh-P, iw=ow*S+kw-P;
                        if ((unsigned)ih<(unsigned)N && (unsigned)iw<(unsigned)N)
                            sum += in[c*N*N+ih*N+iw] * ker[c*K*K+kh*K+kw];
                    }
            out[oh*N_out+ow] = sum;
        }
}

bool verify(float *g, float *c, int n, float eps) {
    for (int i=0; i<n; i++) {
        float d = fabsf(g[i]-c[i]);
        if (d>eps && d/(fabsf(c[i])+1e-8f)>eps) return false;
    }
    return true;
}

float bench_ms(void (*fn)(), int wup, int runs) {
    cudaEvent_t s,e;
    CHECK_CUDA(cudaEventCreate(&s)); CHECK_CUDA(cudaEventCreate(&e));
    for (int w=0; w<wup; w++) fn();
    CHECK_CUDA(cudaDeviceSynchronize());
    CHECK_CUDA(cudaEventRecord(s,0));
    for (int r=0; r<runs; r++) fn();
    CHECK_CUDA(cudaEventRecord(e,0)); CHECK_CUDA(cudaEventSynchronize(e));
    float ms;
    CHECK_CUDA(cudaEventElapsedTime(&ms,s,e));
    CHECK_CUDA(cudaEventDestroy(s)); CHECK_CUDA(cudaEventDestroy(e));
    return ms/(float)runs;
}

static float *g_in, *g_ker, *g_out;
static int g_N, g_C=3, g_K=3, g_P=1, g_S, g_Nout;
static dim3 g_grid, g_block;

void launch_shared() { conv2d_shared<<<g_grid, g_block>>>(g_in, g_ker, g_out, g_N, g_C, g_K, g_P, g_S, g_Nout); }

int main() {
    cudaDeviceProp prop; int dev;
    CHECK_CUDA(cudaGetDevice(&dev)); CHECK_CUDA(cudaGetDeviceProperties(&prop,dev));
    printf("GPU: %s\n\n", prop.name);

    int max_N=1024;
    float *h_in=(float*)malloc(max_N*max_N*3*sizeof(float));
    float *h_ker=(float*)malloc(27*sizeof(float));
    float *h_cpu=(float*)malloc(max_N*max_N*sizeof(float));
    float *h_gpu=(float*)malloc(max_N*max_N*sizeof(float));
    srand(42);
    for (int i=0; i<max_N*max_N*3; i++) h_in[i]=(float)(rand()%100)/10.0f;
    for (int i=0; i<27; i++) h_ker[i]=(float)(rand()%50)/100.0f-0.25f;

    CHECK_CUDA(cudaMalloc(&g_in, max_N*max_N*3*sizeof(float)));
    CHECK_CUDA(cudaMalloc(&g_ker, 27*sizeof(float)));
    CHECK_CUDA(cudaMalloc(&g_out, max_N*max_N*sizeof(float)));
    CHECK_CUDA(cudaMemcpy(g_in, h_in, max_N*max_N*3*sizeof(float), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(g_ker, h_ker, 27*sizeof(float), cudaMemcpyHostToDevice));

    int warmup=10, runs=50;
    g_N=512; g_S=1;
    g_Nout=(g_N+2*g_P-g_K)/g_S+1;

    // 验证
    conv2d_cpu(h_in, h_ker, h_cpu, g_N, g_C, g_K, g_P, g_S);
    g_block=dim3(BLK,BLK); g_grid=dim3((g_Nout+BLK-1)/BLK,(g_Nout+BLK-1)/BLK);
    launch_shared();
    CHECK_CUDA(cudaMemcpy(h_gpu, g_out, g_Nout*g_Nout*sizeof(float), cudaMemcpyDeviceToHost));
    printf("共享内存分块验证: %s\n", verify(h_gpu, h_cpu, g_Nout*g_Nout, 1e-4f) ? "通过" : "失败");

    // 计时
    float t = bench_ms(launch_shared, warmup, runs);
    printf("耗时 (512x512, stride=1): %.6f ms\n", t);

    CHECK_CUDA(cudaFree(g_in)); CHECK_CUDA(cudaFree(g_ker)); CHECK_CUDA(cudaFree(g_out));
    free(h_in); free(h_ker); free(h_cpu); free(h_gpu);
    printf("完成。\n"); return 0;
}
