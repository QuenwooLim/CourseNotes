# ch08 CUDA 编程复习提纲

## 一、CUDA 基础（ch08-1）

### CUDA 是什么
- Compute Unified Device Architecture
- NVIDIA GPU 上的通用并行计算平台和编程模型
- 扩展了 C/C++/Fortran/Python，通过 `nvcc` 编译 `.cu` 文件

### 编译流程
```
.cu 文件 → nvcc 编译器 → 主机代码(.c/.cpp→gcc) + 设备代码(PTX → JIT编译)
```

### CUDA 程序基本结构
1. 分配 GPU 内存 (`cudaMalloc`)
2. 数据 Host → Device (`cudaMemcpy`)
3. 启动 Kernel 在 GPU 上并行执行
4. 数据 Device → Host (`cudaMemcpy`)
5. 释放 GPU 内存 (`cudaFree`)

### 函数类型限定符

| 限定符 | 在哪执行 | 可从哪调用 |
|--------|---------|-----------|
| `__global__` | Device | Host（或 Device，CC≥3.5） |
| `__device__` | Device | Device only |
| `__host__` | Host | Host only（可省略） |

### Kernel 限制
- 必须 `void` 返回
- 只能访问设备内存
- 不支持可变参数、静态变量、函数指针

---

## 二、线程层次结构（ch08-2）⭐ 重点

### 三层结构
```
Grid（网格）
  └── Block（线程块）
        └── Thread（线程）
              └── Warp（硬件执行单位 = 32 线程）
```

### 关键内置变量（必背）

| 变量 | 含义 |
|------|------|
| `threadIdx.x/y/z` | 线程在块内的索引 |
| `blockIdx.x/y/z` | 块在网格中的索引 |
| `blockDim.x/y/z` | 每块的线程数 |
| `gridDim.x/y/z` | 网格的块数 |

### 全局索引计算（笔试必考）

**1D 网格 + 1D 块：**
```c
int i = blockIdx.x * blockDim.x + threadIdx.x;
```

**2D 网格 + 2D 块 → 2D 矩阵的 1D 偏移：**
```c
int ix = threadIdx.x + blockIdx.x * blockDim.x;
int iy = threadIdx.y + blockIdx.y * blockDim.y;
int idx = iy * nx + ix;     // 行主序
```

### Kernel 启动语法
```c
kernel_name<<<gridDim, blockDim>>>(args);
// gridDim, blockDim 都是 dim3 类型（x,y,z，未用默认=1）
```

### 异步执行
- Kernel 启动是异步的，控制立即返回 CPU
- 用 `cudaDeviceSynchronize()` 强制等待所有 Kernel 完成

---

## 二-B、Kernel 编写常见考点 ⭐ 笔试高频

### 1. 边界检查（Boundary Check）—— 必考！

**线程总数可能超过数据量**，不检查就会越界访问。

```c
int idx = blockIdx.x * blockDim.x + threadIdx.x;   // 全局线程索引
if (idx < n) {                                      // ← 必须检查！
    c[idx] = a[idx] + b[idx];
}
```

**为什么必须写？** 启动的 Block 数是 `ceil(n / blockSize)` 向上取整的 → 最后一批线程可能超出 n → 必须跳过。

**不写边界检查的后果：** 数组越界访问 → 结果错误、段错误或 Kernel 崩溃。

### 2. Block 数量计算公式

```c
int threads = 256;
int blocks = (n + threads - 1) / threads;   // 向上取整
// 等价于 ceil((float)n / threads)
```

**示例：** n=1000, threads=256 → blocks = (1000+255)/256 = 4 → 总线程 = 4×256 = 1024 → **超出 24 个线程**，靠边界检查跳过。

### 3. 2D 索引必须掌握的三种映射

**矩阵行优先存储（标准 C 方式）：**
```c
int row = blockIdx.y * blockDim.y + threadIdx.y;   // 行号
int col = blockIdx.x * blockDim.x + threadIdx.x;   // 列号

if (row < N && col < N) {
    mat[row * N + col] = ...;    // 访问 (row, col)，1D 偏移 = row × N + col
}
```

**矩阵转置：**
```c
matT[col * N + row] = mat[row * N + col];
//     ↑ 行列互换
```

**2D Grid/Block 尺寸计算：**
```c
dim3 blockSize(16, 16);   // 每个 block 256 个线程
dim3 gridSize((N + 16 - 1) / 16, (N + 16 - 1) / 16);
//               ↑ gridSize.x          ↑ gridSize.y
```

### 4. 例题速算演示（N=1000, blockSize=16×16）

```
gridSize.x = (1000 + 15) / 16 = 63
gridSize.y = (1000 + 15) / 16 = 63
总 Block 数 = 63 × 63 = 3969
每个 Block 线程 = 16 × 16 = 256
总线程数 = 3969 × 256 = 1,016,064

但矩阵只有 1000×1000 = 1,000,000 个元素
→ 最后一行和最后一列的线程部分超出 → 依赖边界检查跳过
```

**最后一个 Block 有多少线程不干活？**

```
最后一个 Block 的 row 范围: 16×62=992 到 16×63-1=1007
有效行数: 1000 - 992 = 8（row 0~7 对应 row=992~999，row=8~15 超出 N）
有效列数: 同样 8
有效线程 = 8 × 8 = 64
不干活线程 = 256 - 64 = 192
```

### 5. 2D → 1D 偏移速记

```
矩阵 (row, col)，行优先存储：
  偏移地址 = row × N + col       ← 跨过 row 个完整行，再走 col 列

转置访问：
  mat[row × N + col]   →   原矩阵 (row, col)
  mat[col × N + row]   →   转置后 (col, row)，行列互换
```

### 6. `__device__` 变量的读写

```c
__device__ float d_total;   // 全局变量，所有线程可读可写
__constant__ float c_alpha; // 全局变量，只读，读相同的值最快

// Kernel 内部对 __device__ 变量的操作需要原子操作
atomicAdd(&d_total, value);  // 多线程安全累加
```

| 声明 | 位置 | 读写性 | 初始化方式 |
|------|------|--------|-----------|
| `__device__` 变量 | 全局内存 | 所有线程可读写 | `cudaMemcpyToSymbol` |
| `__constant__` 变量 | 常量内存+缓存 | 线程只读 | `cudaMemcpyToSymbol` |
| `__shared__` 变量 | 片上共享内存 | Block 内共享 | Kernel 内赋值 |

---

## 三、内存层次结构（ch08-2 + ch08-4）⭐ 重点

### 内存类型（从快到慢）

| 内存 | 位置 | 作用域 | 生命周期 | 修饰符 |
|------|------|--------|---------|--------|
| **寄存器** | 片上 | 单线程 | Kernel | 自动变量 |
| **共享内存** | 片上 | 同 Block 线程 | Block | `__shared__` |
| **常量内存** | 片外+缓存 | 全局（只读） | 程序 | `__constant__` |
| **纹理内存** | 片外+只读缓存 | 全局（只读） | 程序 | — |
| **本地内存** | 片外（寄存器溢出） | 单线程 | Kernel | 编译器决定 |
| **全局内存** | 片外 | 全局 | 程序 | `cudaMalloc` / `__device__` |

### 寄存器
- GPU 最快，私有
- 每个 SM 上的活跃 Warp 共享，用太多 → 寄存器溢出 → 变本地内存 → 变慢
- `__launch_bounds__` 可以限制每块最大线程数以控制寄存器使用

### 共享内存
- `__shared__` 修饰
- Block 内共享，块间不共享
- 用 `__syncthreads()` 同步（同一 Block 内所有线程到达后继续）
- **片上 64KB 可配置划分**：L1 缓存 / 共享内存
  - `cudaFuncCachePreferShared`：48KB 共享 + 16KB L1
  - `cudaFuncCachePreferL1`：16KB 共享 + 48KB L1

### 常量内存
- `__constant__` 全局声明，64KB 限制
- 主机用 `cudaMemcpyToSymbol` 初始化
- Warp 内所有线程读同一地址 → 最佳性能

### 全局内存
- 最大、最慢
- GPU 只缓存读取，不缓存写入
- 访问按 **32/64/128 字节的事务**，合并访问（Coalesced）至关重要

### 内存 API

| 标准 C | CUDA C |
|--------|--------|
| `malloc` | `cudaMalloc` |
| `memcpy` | `cudaMemcpy` |
| `memset` | `cudaMemset` |
| `free` | `cudaFree` |

```c
cudaMemcpy(dst, src, bytes, cudaMemcpyHostToDevice);   // CPU→GPU
cudaMemcpy(dst, src, bytes, cudaMemcpyDeviceToHost);   // GPU→CPU
```

**常见错误**：设备指针不能在主机代码中解引用！

---

## 四、执行模型（ch08-3）⭐ 重点

### Warp
- **GPU 硬件执行的基本单位 = 32 个线程**
- Warp 内所有线程执行相同指令（SIMT）
- 一个 Block 的线程被分成 warp：`ceil(threadsPerBlock / 32)`
- 2D/3D 线程块内部展开为 1D，然后按 threadIdx 连续 32 个为一组

### SIMT vs SIMD
| | SIMD | SIMT |
|---|---|---|
| 寄存器 | 共享同一向量寄存器 | **每个线程私有** |
| 分支 | 只能用掩码 | 每个线程可独立分支 |
| 寻址 | 必须连续 | 可分开寻址 |

### Warp 发散 ⭐ 核心优化概念

**同一 warp 内不同的执行路径 = 串行执行各分支 → 性能灾难**

```c
// ❌ 高度发散
if (tid % 2 == 0) { ... }   // 偶数线程一条路，奇数另一条

// ✅ 避免发散 — 分支粒度为 warp 大小的倍数
if ((tid / 32) % 2 == 0) { ... }  // 偶数 warp 走一条，奇数走另一条
```

**CUDA 编译器可能用谓词指令优化短分支，可用 nvprof 查看分支效率。**

### 占用率 Occupancy
```
占用率 = 每 SM 的活跃 Warp 数 / 每 SM 最大 Warp 数
```
- 高占用率 ≠ 高性能（可能受限于内存带宽）
- 每 Block 至少 128~256 线程，Block 数 ≫ SM 数

### 延迟隐藏
- 算术延迟 10-20 周期，全局内存 400-800 周期
- **靠 Warp 调度器切换其他 Warp 的计算来填充等待时间**
- Little 定律：需要活跃 Warp ≈ 平均延迟 × 每周期发射 Warp 数

### 同步
- `__syncthreads()`：Block 内同步
- `cudaDeviceSynchronize()`：Host 和 Device 同步
- **没有 Block 间同步**（只能在 Kernel 边界同步）

### 资源划分
```
SM 上的活跃 Warp 数受限于：
  - 最大 Warp 数（硬件限制，如 Kepler=64, Fermi=48）
  - 寄存器数量（每线程用多了 Warp 就少）
  - 共享内存大小（每 Block 用多了 Block 就少）
```

---

## 五、并行归约优化（ch08-3）⭐ 高频考点

### 演化过程

| 优化技术 | 相对于原始加速 |
|---------|-------------|
| 原始邻居对（高度发散） | 1.0× |
| 改进邻居对（减少发散） | 1.26× |
| 交错对（Interleaved） | 1.69× |
| 循环展开 ×2 | 3.42× |
| 展开 Warp + volatile | 8.65× |
| 完全展开 | 9.16× |
| 模板函数完全展开 | 最大 |

### 关键代码模式

**交错归约：**
```c
for (int stride = blockDim.x / 2; stride > 0; stride >>= 1) {
    if (tid < stride)
        idata[tid] += idata[tid + stride];
}
```

**Warp 级展开（volatile 防编译器优化）：**
```c
if (tid < 32) {
    volatile int* vmem = idata;
    vmem[tid] += vmem[tid + 32];  // 最后 6 步手动展开
    vmem[tid] += vmem[tid + 16];
    vmem[tid] += vmem[tid + 8];
    vmem[tid] += vmem[tid + 4];
    vmem[tid] += vmem[tid + 2];
    vmem[tid] += vmem[tid + 1];
}
```

### 循环展开
- 减少分支指令和循环维护开销
- 模板函数展开：`template<unsigned int iBlockSize>` 将块大小变成编译期常量
- 编译器自动删除恒为 false 的 `if (iBlockSize >= 1024)` 分支

---

## 六、动态并行（ch08-3）

- 允许 GPU Kernel 内部启动新的 Kernel（子网格）
- 父网格在所有子网格完成后才完成
- 需要 CC ≥ 3.5，最大嵌套深度 24
- 编译：`nvcc -rdc=true ... -lcudadevrt`
- 用途：递归算法（如递归归约）、层次化并行

---

## 七、GPU 架构演化（ch08-3）

| 架构 | 年代 | 关键特性 |
|------|------|---------|
| Tesla | 2006 | 首个统一着色器 |
| **Fermi** | 2010 | 首个完整计算架构，16 SM，512 CUDA Core，可配置 64KB 片上内存，支持并发 Kernel 执行 |
| **Kepler** | 2012 | 动态并行、Hyper-Q（32 硬件工作队列）、每 SM 192 CUDA Core |
| Maxwell | 2014 | 能效优化 |
| Pascal | 2016 | NVLink、HBM2 |
| Volta | 2017 | Tensor Core |
| Ampere/Hopper/Blackwell | 2020+ | |

---

## 八、设备管理（ch08-2）

```c
int deviceCount;
cudaGetDeviceCount(&deviceCount);                       // 查询 GPU 数量
cudaGetDeviceProperties(&props, deviceID);              // 查询属性（SM 数、内存等）
cudaSetDevice(deviceID);                                // 选择 GPU
CUDA_VISIBLE_DEVICES=2,3 ./app                          // 环境变量选择 GPU
```

---

## 九、错误处理和验证（ch08-2）

```c
#define CHECK(call) {                                                   \
    const cudaError_t err = call;                                       \
    if (err != cudaSuccess) {                                           \
        printf("Error: %s:%d, code=%d, reason=%s\n",                    \
               __FILE__, __LINE__, err, cudaGetErrorString(err));        \
        exit(1);                                                        \
    }                                                                   \
}

CHECK(cudaMemcpy(d_C, h_C, nBytes, cudaMemcpyHostToDevice));
```

---

## 十、性能分析（ch08-2 + ch08-3）

### 工具
- `nvprof`：命令行性能分析，查看 Kernel 时间、内存传输时间
- `nvvp`：可视化性能分析器
- `nvidia-smi`：查询 GPU 状态

### 性能限制分析
```
Tesla K10 示例：
  算术峰值 = 核心频率 × GPU 数量 × (SM 数 × CUDA Core/SM) × 2 ops/cycle
  内存带宽 = GPU 数量 × 位宽 × 内存频率 × 2 DDR / 8 bits/byte
  指令:字节比 = 峰值 FLOPS / 峰值带宽

  如果每访问 1 字节发出超过此比值 → 算术限制
  否则 → 内存带宽限制（大多数 HPC 应用受内存限制）
```

---

## 十一、CUDA 编程最佳实践（总结）

1. **线程块大小** = 32 的倍数，至少 128~256 个线程
2. **块的数量** ≫ SM 数量（暴露足够并行性）
3. **避免 Warp 发散**：分支粒度对齐到 warp 大小
4. **合并全局内存访问**：连续线程访问连续地址
5. **使用共享内存**减少全局内存访问
6. **循环展开**减少开销
7. **高占用率是起点，不是终点**——找影响性能的真正瓶颈

---

## 速记对比表

### Host vs Device 代码
| | Host (CPU) | Device (GPU) |
|---|---|---|
| 函数 | 普通/+ `__host__` | `__global__`, `__device__` |
| 内存 | `malloc/free` | `cudaMalloc/cudaFree` |
| 指针解引用 | ✅ | ❌ 不能在 Host 解引用 |
| 数据传输 | — | `cudaMemcpy` |

### Block vs Warp vs Thread
| | Thread | Warp | Block |
|---|---|---|---|
| 数量 | 最多 1024/Block | 32 Thread | 多个 Block/Grid |
| 同步 | `__syncthreads()` | SIMT 自动 | 无 Block 间同步 |
| 通信 | 共享内存 | Warp shuffle | 共享内存 |
