# cuBLAS库用户指南总结

## 一、库概述

### 1.1 基本介绍
cuBLAS库是基于NVIDIA CUDA运行时的BLAS（基础线性代数子程序）实现，允许用户访问NVIDIA GPU的计算资源。

### 1.2 三种API
- **cuBLAS API**：标准API（从CUDA 6.0开始）
- **cuBLASXt API**：支持主机和多个设备间的数据分发（从CUDA 6.0开始）
- **cuBLASLt API**：轻量级库，专注于通用矩阵乘法（GEMM）操作（从CUDA 10.1开始）

### 1.3 数据布局
- 使用**列优先存储**（column-major）
- **1-based索引**（为兼容Fortran）
- C/C++代码建议使用宏定义转换索引：
  - 1-based: `#define IDX2F(i,j,ld) ((((j)-1)*(ld))+((i)-1))`
  - 0-based: `#define IDX2C(i,j,ld) (((j)*(ld))+(i))`

### 1.4 新旧API对比
**新API（cublas_v2.h）特性：**
- 显式传递cuBLAS上下文句柄
- 标量参数可通过主机或设备引用传递
- 支持异步执行
- 返回错误状态`cublasStatus_t`
- 已废弃`cublasAlloc()`和`cublasFree()`

**注意：** 旧API已废弃，将在未来版本中移除。

---

## 二、核心使用方法

### 2.1 初始化与销毁
```c
cublasCreate(&handle);  // 初始化
// ... 使用库函数 ...
cublasDestroy(handle);  // 释放资源
```

### 2.2 基本工作流程
1. 初始化cuBLAS上下文句柄
2. 在GPU内存中分配矩阵和向量
3. 填充数据
4. 调用cuBLAS函数
5. 从GPU内存上传结果到主机
6. 销毁句柄

### 2.3 流（Stream）并行性
```c
cudaStreamCreate(&stream);
cublasSetStream(handle, stream);
// cuBLAS函数调用
cudaStreamSynchronize(stream);
```

### 2.4 批处理小内核
- 为每个小任务创建独立流
- 使用`cublasSetStream()`设置不同流
- 最多支持32个并发内核

---

## 三、关键配置选项

### 3.1 指针模式
- `CUBLAS_POINTER_MODE_HOST`：标量在主机上（同步）
- `CUBLAS_POINTER_MODE_DEVICE`：标量在设备上（异步）

### 3.2 工作空间
```c
cublasSetWorkspace(handle, workspace, workspaceSize);
```
- 推荐大小：至少4MiB
- 对齐要求：至少256字节

### 3.3 数学模式
- `CUBLAS_DEFAULT_MATH`：默认高性能模式
- `CUBLAS_PEDANTIC_MATH`：精确数值模式
- `CUBLAS_TF32_TENSOR_OP_MATH`：TF32加速
- `CUBLAS_MATH_DISALLOW_REDUCED_PRECISION_REDUCTION`：禁止降精度约简

### 3.4 原子操作模式
```c
cublasSetAtomicsMode(handle, CUBLAS_ATOMICS_ALLOWED);
```
- 某些函数（如`cublas<t>symv`）有更快的原子操作实现
- 但结果可能不严格一致

### 3.5 Tensor Core使用
- Volta架构（sm_70+）自动启用
- 最佳性能条件：
  - `m % 8 == 0`, `k % 8 == 0`
  - 内存对齐：16字节

---

## 四、数据类型

### 4.1 cuBLAS数据类型
- `cublasHandle_t`：库上下文句柄
- `cublasStatus_t`：函数返回状态
- `cublasOperation_t`：矩阵操作类型（N/T/C）
- `cublasFillMode_t`：填充模式（LOWER/UPPER）
- `cublasDiagType_t`：对角线类型（UNIT/NON_UNIT）
- `cublasSideMode_t`：矩阵位置（LEFT/RIGHT）
- `cublasPointerMode_t`：指针模式
- `cublasComputeType_t`：计算精度类型

### 4.2 CUDA数据类型
- `CUDA_R_16F`：半精度浮点
- `CUDA_R_32F`：单精度浮点
- `CUDA_R_64F`：双精度浮点
- `CUDA_R_8I`：8位整数
- `CUDA_R_32I`：32位整数
- `CUDA_R_16BF`：bfloat16
- `CUDA_R_8F_E4M3/E5M2`：8位浮点

---

## 五、函数分类

### 5.1 Level-1 函数（向量操作）
- `cublasI<t>amax/amin`：最大/最小元素索引
- `cublas<t>asum`：绝对值和
- `cublas<t>axpy`：向量缩放加法
- `cublas<t>copy`：向量复制
- `cublas<t>dot`：点积
- `cublas<t>nrm2`：欧几里得范数
- `cublas<t>rot/rotg/rotm/rotmg`：Givens旋转
- `cublas<t>scal`：向量缩放
- `cublas<t>swap`：向量交换

### 5.2 Level-2 函数（矩阵-向量操作）
- `cublas<t>gbmv`：带状矩阵-向量乘法
- `cublas<t>gemv`：通用矩阵-向量乘法
- `cublas<t>ger`：秩-1更新
- `cublas<t>sbmv`：对称带状矩阵-向量乘法
- `cublas<t>spmv`：对称打包矩阵-向量乘法
- `cublas<t>spr/spr2`：对称打包秩-1/2更新
- `cublas<t>symv`：对称矩阵-向量乘法
- `cublas<t>syr/syr2`：对称秩-1/2更新
- `cublas<t>tbmv/tbsv`：三角带状矩阵-向量乘法/求解

### 5.3 Level-3 函数（矩阵-矩阵操作）
- `cublas<t>gemm`：通用矩阵-矩阵乘法
- `cublas<t>gemmEx`：扩展精度GEMM
- `cublas<t>trsm`：三角矩阵求解
- `cublas<t>symm`：对称矩阵乘法

---

## 六、重要注意事项

### 6.1 线程安全
- 库是线程安全的
- 不建议多个线程共享同一句柄
- 句柄配置更改会影响所有使用该句柄的线程

### 6.2 结果可重现性
- 单流执行保证位级可重现
- 多流执行可能因内部优化而不保证
- 解决方案：
  - 为每个流提供独立工作空间
  - 每个流使用独立句柄
  - 设置环境变量`CUBLAS_WORKSPACE_CONFIG`

### 6.3 CUDA Graphs支持
- 大多数cuBLAS函数支持CUDA Graphs捕获
- 例外：输出结果到主机缓冲区的函数会强制同步
- 每次捕获会分配设备内存，建议使用`cublasSetWorkspace()`

### 6.4 静态库支持
```bash
# 动态链接
nvcc myApp.c -lcublas -o myApp

# 静态链接
nvcc myApp.c -lcublas_static -lculibos -o myApp
```

---

## 七、性能优化建议

1. **使用Tensor Core**：确保矩阵维度和内存对齐满足要求
2. **批量小内核**：使用多个流并行执行
3. **工作空间**：提供足够大的工作空间（≥4MiB）
4. **流并行**：利用多个CUDA流重叠计算
5. **指针模式**：使用`CUBLAS_POINTER_MODE_DEVICE`实现完全异步
6. **数学模式**：根据精度需求选择合适的计算模式

---

## 八、错误处理

所有cuBLAS函数返回`cublasStatus_t`，可能的值包括：
- `CUBLAS_STATUS_SUCCESS`：成功
- `CUBLAS_STATUS_NOT_INITIALIZED`：未初始化
- `CUBLAS_STATUS_ALLOC_FAILED`：资源分配失败
- `CUBLAS_STATUS_INVALID_VALUE`：无效参数
- `CUBLAS_STATUS_ARCH_MISMATCH`：架构不匹配
- `CUBLAS_STATUS_EXECUTION_FAILED`：执行失败
- `CUBLAS_STATUS_INTERNAL_ERROR`：内部错误
- `CUBLAS_STATUS_NOT_SUPPORTED`：不支持
- `CUBLAS_STATUS_LICENSE_ERROR`：许可证错误