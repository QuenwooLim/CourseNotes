# Makefile —— CUDA 卷积实验
# 中山大学 计算机学院 并行程序设计与算法实验

NVCC := /usr/local/cuda-12.4/bin/nvcc
NVCC_FLAGS := -arch=sm_70 -O3 -std=c++11

# cuDNN 路径（从 pip 包中找的）
CUDNN_PATH := /opt/conda/lib/python3.11/site-packages/nvidia/cudnn
CUDNN_FLAGS := -I$(CUDNN_PATH)/include -L$(CUDNN_PATH)/lib
CUDNN_LIBS := -lcudnn

.PHONY: all clean

all: convolution conv2d bench_all cudnn_bench

# 一维卷积（四种优化版本）
convolution: convolution.cu
	$(NVCC) $(NVCC_FLAGS) -o convolution convolution.cu

# 二维直接卷积（线程块 & 任务映射对比）
conv2d: conv2d.cu
	$(NVCC) $(NVCC_FLAGS) -o conv2d conv2d.cu

# 完整对比测试（直接卷积 vs im2col+GEMM）
bench_all: bench_all.cu
	$(NVCC) $(NVCC_FLAGS) -o bench_all bench_all.cu

# cuDNN 卷积测试
cudnn_bench: cudnn_bench.cu
	$(NVCC) $(NVCC_FLAGS) $(CUDNN_FLAGS) -o cudnn_bench cudnn_bench.cu $(CUDNN_LIBS)

clean:
	rm -f convolution conv2d bench_all cudnn_bench conv2d_shared
