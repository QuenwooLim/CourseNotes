/*
 * cuDNN 卷积性能测试
 * 中山大学 计算机学院 并行程序设计与算法实验
 */
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cudnn.h>

#define CHECK_CUDA(call) do { \
    cudaError_t e = call; \
    if (e != cudaSuccess) { fprintf(stderr,"CUDA 错误 %s:%d - %s\n",__FILE__,__LINE__,cudaGetErrorString(e)); exit(1); } \
} while(0)
#define CHECK_CUDNN(call) do { \
    cudnnStatus_t s = call; \
    if (s != CUDNN_STATUS_SUCCESS) { fprintf(stderr,"cuDNN 错误 %s:%d - %s\n",__FILE__,__LINE__,cudnnGetErrorString(s)); exit(1); } \
} while(0)

// cuDNN 卷积计时：包含算法选择 + workspace 分配 + 执行
float bench_cudnn(cudnnHandle_t h,
                  cudnnTensorDescriptor_t in_desc, float *d_in,
                  cudnnFilterDescriptor_t flt_desc, float *d_flt,
                  cudnnConvolutionDescriptor_t conv_desc,
                  cudnnTensorDescriptor_t out_desc, float *d_out,
                  int warmup, int runs) {
    // 让 cuDNN 自己挑最快的算法
    int req_cnt = 8, ret_cnt = 0;
    cudnnConvolutionFwdAlgoPerf_t perf[8];
    CHECK_CUDNN(cudnnFindConvolutionForwardAlgorithm(
        h, in_desc, flt_desc, conv_desc, out_desc, req_cnt, &ret_cnt, perf));

    cudnnConvolutionFwdAlgo_t algo = perf[0].algo;
    size_t ws_size = 0;
    CHECK_CUDNN(cudnnGetConvolutionForwardWorkspaceSize(
        h, in_desc, flt_desc, conv_desc, out_desc, algo, &ws_size));

    void *d_ws = NULL;
    if (ws_size > 0) CHECK_CUDA(cudaMalloc(&d_ws, ws_size));

    float alpha = 1.0f, beta = 0.0f;
    cudaEvent_t start, stop;
    CHECK_CUDA(cudaEventCreate(&start));
    CHECK_CUDA(cudaEventCreate(&stop));

    // 预热
    for (int w = 0; w < warmup; w++)
        CHECK_CUDNN(cudnnConvolutionForward(h, &alpha, in_desc, d_in,
            flt_desc, d_flt, conv_desc, algo, d_ws, ws_size, &beta, out_desc, d_out));
    CHECK_CUDA(cudaDeviceSynchronize());

    // 正式计时（只计 cudnnConvolutionForward 的时间）
    CHECK_CUDA(cudaEventRecord(start, 0));
    for (int r = 0; r < runs; r++)
        CHECK_CUDNN(cudnnConvolutionForward(h, &alpha, in_desc, d_in,
            flt_desc, d_flt, conv_desc, algo, d_ws, ws_size, &beta, out_desc, d_out));
    CHECK_CUDA(cudaEventRecord(stop, 0));
    CHECK_CUDA(cudaEventSynchronize(stop));

    float ms;
    CHECK_CUDA(cudaEventElapsedTime(&ms, start, stop));
    CHECK_CUDA(cudaEventDestroy(start));
    CHECK_CUDA(cudaEventDestroy(stop));
    if (d_ws) CHECK_CUDA(cudaFree(d_ws));
    return ms / (float)runs;
}

int main() {
    cudaDeviceProp prop; int dev;
    CHECK_CUDA(cudaGetDevice(&dev));
    CHECK_CUDA(cudaGetDeviceProperties(&prop, dev));
    printf("GPU: %s\n", prop.name);

    cudnnHandle_t handle;
    CHECK_CUDNN(cudnnCreate(&handle));

    const int C = 3, K = 3, P = 1, K_out = 1;
    int warmup = 10, runs = 50;

    // 按最大尺寸一次性分配
    int max_N = 1024;
    float *h_in = (float*)malloc(max_N*max_N*C*sizeof(float));
    float *h_flt = (float*)malloc(K_out*C*K*K*sizeof(float));
    srand(42);
    for (int i = 0; i < max_N*max_N*C; i++) h_in[i] = (float)(rand()%100)/10.0f;
    for (int i = 0; i < K_out*C*K*K; i++) h_flt[i] = (float)(rand()%50)/100.0f-0.25f;

    float *d_in, *d_flt, *d_out;
    CHECK_CUDA(cudaMalloc(&d_in, max_N*max_N*C*sizeof(float)));
    CHECK_CUDA(cudaMalloc(&d_flt, K_out*C*K*K*sizeof(float)));
    CHECK_CUDA(cudaMalloc(&d_out, max_N*max_N*sizeof(float)));
    CHECK_CUDA(cudaMemcpy(d_in, h_in, max_N*max_N*C*sizeof(float), cudaMemcpyHostToDevice));
    CHECK_CUDA(cudaMemcpy(d_flt, h_flt, K_out*C*K*K*sizeof(float), cudaMemcpyHostToDevice));

    int sizes[] = {256, 512, 1024};
    int strides[] = {1, 2, 3};

    printf("=== cuDNN 卷积测试结果 ===\n");
    printf("%-14s %-4s %-6s %-10s %-20s\n", "输入", "S", "Nout", "耗时(ms)", "cuDNN 选的算法");
    printf("----------------------------------------------------------------\n");

    for (int is = 0; is < 3; is++) {
        int N = sizes[is];
        for (int ss = 0; ss < 3; ss++) {
            int S = strides[ss];
            int N_out = (N + 2*P - K) / S + 1;
            if (N_out <= 0) continue;

            // 创建描述符
            cudnnTensorDescriptor_t in_desc;
            cudnnCreateTensorDescriptor(&in_desc);
            cudnnSetTensor4dDescriptor(in_desc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT,
                                        1, C, N, N);

            cudnnFilterDescriptor_t flt_desc;
            cudnnCreateFilterDescriptor(&flt_desc);
            cudnnSetFilter4dDescriptor(flt_desc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW,
                                        K_out, C, K, K);

            cudnnConvolutionDescriptor_t conv_desc;
            cudnnCreateConvolutionDescriptor(&conv_desc);
            cudnnSetConvolution2dDescriptor(conv_desc, P, P, S, S, 1, 1,
                                             CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);

            cudnnTensorDescriptor_t out_desc;
            cudnnCreateTensorDescriptor(&out_desc);
            cudnnSetTensor4dDescriptor(out_desc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT,
                                        1, K_out, N_out, N_out);

            // 找出 cuDNN 选的算法叫什么名字
            int req_cnt = 8, ret_cnt = 0;
            cudnnConvolutionFwdAlgoPerf_t perf[8];
            cudnnFindConvolutionForwardAlgorithm(handle, in_desc, flt_desc, conv_desc,
                                                  out_desc, req_cnt, &ret_cnt, perf);

            float t = bench_cudnn(handle, in_desc, d_in, flt_desc, d_flt,
                                   conv_desc, out_desc, d_out, warmup, runs);

            const char* algo_name = "未知";
            switch(perf[0].algo) {
                case CUDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_GEMM: algo_name="IMPLICIT_GEMM"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_PRECOMP_GEMM: algo_name="PRECOMP_GEMM"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_GEMM: algo_name="GEMM"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_DIRECT: algo_name="DIRECT"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_FFT: algo_name="FFT"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_WINOGRAD: algo_name="WINOGRAD"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_WINOGRAD_NONFUSED: algo_name="WINOGRAD_NF"; break;
                case CUDNN_CONVOLUTION_FWD_ALGO_FFT_TILING: algo_name="FFT_TILING"; break;
                default: algo_name = "其他"; break;
            }

            char buf[32]; sprintf(buf, "%dx%d", N, N);
            printf("%-14s %-4d %-6d %-10.6f %-20s\n",
                   buf, S, N_out, t, algo_name);

            cudnnDestroyTensorDescriptor(in_desc);
            cudnnDestroyTensorDescriptor(out_desc);
            cudnnDestroyFilterDescriptor(flt_desc);
            cudnnDestroyConvolutionDescriptor(conv_desc);
        }
    }

    CHECK_CUDA(cudaFree(d_in)); CHECK_CUDA(cudaFree(d_flt)); CHECK_CUDA(cudaFree(d_out));
    free(h_in); free(h_flt);
    cudnnDestroy(handle);
    printf("\n全部完成。\n"); return 0;
}
