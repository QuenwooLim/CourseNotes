# CUDA卷积计算作业要求

**中山大学计算机学院**  
**并行程序设计与算法（实验）**  
**第11次实验作业**

---

## 一、实验目的

1. 掌握CUDA卷积运算的并行实现方法
2. 理解不同优化策略对卷积性能的影响
3. 学习如何利用GPU内存层次结构优化计算密集型任务
4. 掌握性能分析和调优的基本方法

---

## 二、实验内容

### 2.1 卷积运算基础

**卷积运算定义**：  
卷积是一种在图像处理、信号处理和机器学习中广泛应用的数组操作。其本质是用一个小窗口（卷积核/过滤器）在输入数据上滑动，对每个局部区域做加权求和。

**一维卷积示意图描述**：
```
输入数组: [a0, a1, a2, a3, a4, a5, a6, a7, a8, a9]
卷积核:   [k0, k1, k2] (长度为3，奇数)

计算过程:
输出[0] = a0*k0 + a1*k1 + a2*k2
输出[1] = a1*k0 + a2*k1 + a3*k2
输出[2] = a2*k0 + a3*k1 + a4*k2
...
```

**二维卷积示意图描述**：
```
输入图像 (5×5):
┌─────┐
│a b c│
│d e f│
│g h i│
└─────┘

卷积核 (3×3):
┌─────┐
│k0 k1 k2│
│k3 k4 k5│
│k6 k7 k8│
└─────┘

输出计算:
输出中心 = a*k0 + b*k1 + c*k2 +
          d*k3 + e*k4 + f*k5 +
          g*k6 + h*k7 + i*k8
```

### 2.2 实现要求

#### 2.2.1 基础版本（必做）

**实现方法**：基本卷积核函数  
**原理**：每个GPU线程计算输出数组的一个元素，通过全局内存访问输入数据和卷积核进行计算

**核函数伪代码**：
```cuda
__global__ void convolution_basic(float *input, float *kernel, 
                                   float *output, int input_size, 
                                   int kernel_size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < output_size) {
        float sum = 0.0f;
        int kernel_radius = kernel_size / 2;
        for (int k = 0; k < kernel_size; k++) {
            int input_idx = idx + k - kernel_radius;
            if (input_idx >= 0 && input_idx < input_size) {
                sum += input[input_idx] * kernel[k];
            }
        }
        output[idx] = sum;
    }
}
```

**性能问题**：
- 全局内存访问延迟高（几百个时钟周期）
- 卷积核被重复读取，带宽浪费严重
- 计算访存比低

#### 2.2.2 优化版本一：常量存储器优化（必做）

**优化原理**：将卷积核存储到常量内存中，利用硬件缓存机制减少全局内存访问

**实现要点**：
1. 使用`__constant__`修饰符声明常量内存
2. 使用`cudaMemcpyToSymbol()`将卷积核复制到常量内存
3. 核函数中直接访问常量内存中的卷积核

**代码框架**：
```cuda
__constant__ float d_kernel[CONSTANT_KERNEL_SIZE];

// 主机端
cudaMemcpyToSymbol(d_kernel, h_kernel, kernel_size * sizeof(float));

// 设备端核函数
__global__ void convolution_constant(float *input, float *output, 
                                      int input_size, int kernel_size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < output_size) {
        float sum = 0.0f;
        int kernel_radius = kernel_size / 2;
        for (int k = 0; k < kernel_size; k++) {
            int input_idx = idx + k - kernel_radius;
            if (input_idx >= 0 && input_idx < input_size) {
                sum += input[input_idx] * d_kernel[k];
            }
        }
        output[idx] = sum;
    }
}
```

**性能提升**：
- 常量内存自动缓存到L2缓存
- 减少全局内存访问次数
- 适合小尺寸卷积核（通常<64KB）

#### 2.2.3 优化版本二：共享存储器优化（选做）

**优化原理**：利用共享内存缓存输入数据块，实现数据重用

**实现要点**：
1. 每个线程块加载一个输入数据块到共享内存
2. 线程块内所有线程共享这个数据块
3. 减少对全局内存的重复访问

**代码框架**：
```cuda
__global__ void convolution_shared(float *input, float *kernel, 
                                     float *output, int input_size, 
                                     int kernel_size) {
    __shared__ float shared_input[SHARED_SIZE];
    
    int tid = threadIdx.x;
    int idx = blockIdx.x * blockDim.x + tid;
    
    // 加载数据到共享内存（需要处理边界）
    int kernel_radius = kernel_size / 2;
    int load_idx = idx - kernel_radius;
    if (load_idx >= 0 && load_idx < input_size) {
        shared_input[tid] = input[load_idx];
    } else {
        shared_input[tid] = 0.0f;
    }
    __syncthreads();
    
    // 使用共享内存进行计算
    if (idx < output_size) {
        float sum = 0.0f;
        for (int k = 0; k < kernel_size; k++) {
            sum += shared_input[tid + k] * kernel[k];
        }
        output[idx] = sum;
    }
}
```

**性能提升**：
- 共享内存访问延迟低（几十个时钟周期）
- 数据重用率高
- 需要处理边界条件和同步

#### 2.2.4 优化版本三：寄存器分块优化（选做）

**优化原理**：在共享内存基础上，进一步利用寄存器缓存数据，提高计算密度

**实现要点**：
1. 每个线程处理多个输出元素（tile）
2. 使用寄存器缓存中间结果
3. 减少对共享内存的访问次数

**性能提升**：
- 寄存器访问延迟极低（几个时钟周期）
- 计算访存比显著提高
- 需要仔细管理寄存器使用

---

## 三、实验要求

### 3.1 实现要求

1. **基础版本**：实现基本的一维卷积核函数
2. **常量存储器版本**：使用常量内存优化卷积核访问
3. **共享存储器版本**（选做）：使用共享内存优化输入数据访问
4. **寄存器分块版本**（选做）：进一步优化计算密度

### 3.2 测试要求

1. **正确性验证**：
   - 与CPU串行实现结果对比
   - 使用相对误差判断正确性（误差<1e-5）

2. **性能测试**：
   - 测试不同输入规模（1024, 4096, 16384, 65536, 262144）
   - 测试不同卷积核大小（3, 5, 7, 9, 11）
   - 记录每个版本的运行时间
   - 计算加速比

3. **占用率分析**：
   - 分析不同线程块大小对性能的影响
   - 找到最优的线程块配置

### 3.3 报告要求

实验报告应包含以下内容：

#### 3.3.1 实验环境
- GPU型号
- CUDA版本
- 操作系统

#### 3.3.2 实现细节
- 每个版本的核函数代码
- 关键优化策略说明
- 内存分配和数据传输代码

#### 3.3.3 性能分析
- 运行时间对比表格
- 加速比曲线图
- 不同优化策略的性能提升分析

**性能对比表格示例**：

| 输入规模 | 核大小 | 基础版本(ms) | 常量内存版本(ms) | 共享内存版本(ms) | 加速比(常量/基础) | 加速比(共享/基础) |
|---------|-------|-------------|-----------------|-----------------|------------------|------------------|
| 1024    | 3     | 0.12        | 0.08            | 0.06            | 1.50             | 2.00             |
| 4096    | 3     | 0.45        | 0.30            | 0.22            | 1.50             | 2.05             |
| 16384   | 3     | 1.80        | 1.20            | 0.88            | 1.50             | 2.05             |
| 65536   | 3     | 7.20        | 4.80            | 3.52            | 1.50             | 2.05             |
| 262144  | 3     | 28.80       | 19.20           | 14.08           | 1.50             | 2.05             |

#### 3.3.4 优化效果分析
- 分析常量内存优化的效果和适用场景
- 分析共享内存优化的效果和局限性
- 讨论不同输入规模下各优化策略的表现

#### 3.3.5 问题与思考
1. 为什么常量内存适合存储卷积核？
2. 共享内存优化在什么情况下效果最好？
3. 如何选择最优的线程块大小？
4. 对于二维卷积，上述优化策略是否适用？需要做哪些调整？

---

## 四、评分标准

| 项目 | 分值 | 说明 |
|-----|------|------|
| 基础版本实现 | 30分 | 正确实现基本卷积核函数 |
| 常量存储器优化 | 30分 | 正确使用常量内存优化 |
| 共享存储器优化 | 20分 | （选做）正确实现共享内存优化 |
| 寄存器分块优化 | 10分 | （选做）正确实现寄存器优化 |
| 性能测试与分析 | 10分 | 完整的性能测试和分析报告 |

---

## 五、参考资料

1. CUDA C++ Programming Guide
2. NVIDIA cuDNN Library Documentation
3. 《GPU高性能编程CUDA实战》

---

## 六、提交要求

1. **源代码**：包含所有实现版本的完整CUDA代码
2. **Makefile**：编译脚本
3. **实验报告**：PDF格式，包含所有要求的内容
4. **性能数据**：原始测试数据和分析图表

**提交截止时间**：请查看课程网站通知

---

## 七、附录：卷积运算详细说明

### 7.1 一维卷积数学定义

给定输入数组 $N$（长度为 $size$）和卷积核 $M$（长度为 $size\_kernel$），输出数组 $P$ 的计算公式为：

$$P[i] = \sum_{k=0}^{size\_kernel-1} N[i + k - radius] \times M[k]$$

其中 $radius = \frac{size\_kernel - 1}{2}$，边界位置超出范围的输入值视为0。

### 7.2 边界处理

在卷积计算中，当卷积核滑动到输入数组边界时，部分卷积核元素会超出有效范围。常见的边界处理方式：

1. **零填充（Zero Padding）**：超出范围的位置视为0（本实验采用）
2. **镜像填充（Mirror Padding）**：使用边界对称位置的值
3. **循环填充（Circular Padding）**：使用循环索引

### 7.3 性能优化关键点

1. **内存访问模式**：
   - 合并访问（Coalesced Access）
   - 避免内存银行冲突（Bank Conflict）

2. **计算优化**：
   - 减少分支判断
   - 使用向量类型（float4, float2）
   - 循环展开

3. **资源利用**：
   - 合理配置线程块大小
   - 充分利用共享内存
   - 控制寄存器使用量

---
