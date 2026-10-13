# 并行程序设计 — 公式速查手册

> 基于 ch01~ch08 课件笔记，按章节分类，覆盖考试可能出现的所有公式。

---

# ch02 并行硬件和软件

## 2.1 加速比 Speedup

$$
S = \frac{T_{\text{serial}}}{T_{\text{parallel}}}
$$

- `S`：加速比
- `T_serial`：串行程序运行时间
- `T_parallel`：并行程序运行时间
- 理想情况 `S = p`（p 个核心），实际 `S < p`

---

## 2.2 效率 Efficiency

$$
E = \frac{S}{p} = \frac{T_{\text{serial}}}{p \cdot T_{\text{parallel}}}
$$

- `p`：处理器/核心数量
- 理想 `E = 1`，实际 `E < 1`

---

## 2.3 实际并行时间（考虑开销）

$$
T_{\text{parallel}} = \frac{T_{\text{serial}}}{p} + T_{\text{overhead}}
$$

- `T_overhead`：通信、同步、负载不均、额外计算等开销
- 这是为什么 `T_parallel` 永远 > `T_serial / p`

---

## 2.4 阿姆达尔定律 Amdahl's Law ⭐

$$
S \leq \frac{1}{f + \dfrac{1-f}{p}}
$$

- `f`：程序中**不可并行化**的串行比例（0 < f < 1）
- `1-f`：可并行化的比例
- `p`：处理器数量

**推论：**

- 当 `p → ∞` 时，`S ≤ 1/f`
- 如果 10% 串行（f=0.1），最多加速 10 倍，无论多少核

**计算示例：**

```
已知：f = 0.1（90%可并行）, p = 10
S_max = 1 / (0.1 + 0.9/10) = 1 / (0.1 + 0.09) = 1 / 0.19 ≈ 5.26

已知：f = 0.1, p → ∞
S_max = 1 / 0.1 = 10（上限）
```

---

## 2.5 强可扩展性 Strong Scalability

**问题规模固定，增加处理器 → 时间减少。**

$$
\text{若 } N \text{ 固定，} p \text{ 翻倍，则 } T_{\text{parallel}} \approx \frac{T_{\text{original}}}{2}
$$

---

## 2.6 弱可扩展性 Weak Scalability

**问题规模随处理器同比增加 → 时间不变。**

$$
\text{若 } p \text{ 翻倍，同时 } N \text{ 翻倍，则 } T_{\text{parallel}} \approx T_{\text{original}}
$$

---

## 2.7 等分宽度 Bisection Width

全连接网络（p 个节点）的等分宽度：

$$
\text{Bisection Width} = \frac{p^2}{4}
$$

- 衡量将网络切成两半时，跨越切口的链路带宽之和
- 全连接网络：每半有 `p/2` 个节点，两半之间共 `(p/2) × (p/2)` 条链路

---

## 2.8 路由延迟

**存储转发 Store-and-Forward：**

$$
T = h \cdot (T_{\text{xmit}} + T_{\text{switch}})
$$

**直通式 Cut-Through：**

$$
T = h \cdot T_{\text{switch}} + T_{\text{xmit}}
$$

- `h`：跳数（经过的交换机数）
- `T_xmit`：传输延迟（发送整个数据包的时间）
- `T_switch`：交换延迟（处理包头部的时间）
- **直通式比存储转发少付 `(h-1)×T_xmit` 的代价**

---

# ch03 MPI（timing 相关）

## 3.1 MPI 性能公式

```c
// 计时模板
double start  = MPI_Wtime();
// ... 并行代码 ...
double finish = MPI_Wtime();
double elapsed = finish - start;   // 墙上时钟时间（秒）

// 取所有进程中的最大耗时（因为最慢的决定整体时间）
double local_elapsed = finish - start, max_elapsed;
MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE,
           MPI_MAX, 0, MPI_COMM_WORLD);
```

$$
T_{\text{parallel}} = \max_{i=0}^{p-1} (T_i)
$$

即并行时间 = 所有进程中耗时最长的那个。

---

## 3.2 块分区数据局部范围

```c
local_n     = n / comm_sz;                       // 每个进程的元素数
local_a     = a + my_rank * local_n * h;         // 梯形积分左端点
local_first = my_rank * local_n;                 // 数组起始索引
local_last  = local_first + local_n;             // 数组结束索引（不含）
```

---

# ch04 Pthreads（无独立公式，性能概念同 ch02）

加速比和效率公式同 ch02。ch04 重点在概念（竞态、临界区、伪共享）。

---

# ch05 OpenMP

## 5.1 梯形法则积分公式

$$
\int_a^b f(x) \, dx \approx h \left[ \frac{f(x_0)}{2} + f(x_1) + f(x_2) + \cdots + f(x_{n-1}) + \frac{f(x_n)}{2} \right]
$$

其中：

$$
h = \frac{b-a}{n}, \quad x_i = a + i \cdot h
$$

```c
h = (b - a) / n;
approx = (f(a) + f(b)) / 2.0;
for (i = 1; i <= n-1; i++)
    approx += f(a + i * h);
approx = h * approx;
```

---

## 5.2 π 的级数公式

$$
\pi = 4 \left( 1 - \frac{1}{3} + \frac{1}{5} - \frac{1}{7} + \cdots \right)
    = 4 \sum_{k=0}^{\infty} \frac{(-1)^k}{2k+1}
$$

```c
double factor = 1.0, sum = 0.0;
for (k = 0; k < n; k++) {
    sum += factor / (2 * k + 1);
    factor = -factor;
}
pi = 4.0 * sum;
```

---

## 5.3 调度与负载均衡

**循环工作量公式（笔记 ch05 示例）：**

$$
f(i) = \sum_{j = i(i+1)/2}^{i(i+1)/2 + i} \sin(j)
$$

- `f(i)` 的执行时间与 `i` 成正比（`i` 越大计算量越大）
- 默认块分区：一半线程拿到小 `i`（早做完），一半拿到大 `i`（做到最后）→ 加速比仅 1.33
- 循环分区 `schedule(static,1)`：大小均分 → 加速比 1.99

---

# ch06 并行程序开发

## 6.1 万有引力定律（N-body 问题）

粒子 q 受到粒子 k 的力：

$$
\mathbf{F}_{qk} = \frac{G \cdot m_q \cdot m_k}{| \mathbf{s}_q - \mathbf{s}_k |^3} \cdot (\mathbf{s}_q - \mathbf{s}_k)
$$

或按坐标分量写（2D）：

$$
F_{qk}^X = \frac{G \cdot m_q \cdot m_k}{r^3} \cdot (x_q - x_k), \quad
F_{qk}^Y = \frac{G \cdot m_q \cdot m_k}{r^3} \cdot (y_q - y_k)
$$

其中：

$$
r = \sqrt{(x_q - x_k)^2 + (y_q - y_k)^2}
$$

---

## 6.2 牛顿第二定律

粒子 q 的加速度：

$$
\mathbf{a}_q = \frac{\mathbf{F}_q}{m_q}
$$

---

## 6.3 欧拉法更新位置和速度

$$
\mathbf{s}(t + \Delta t) = \mathbf{s}(t) + \mathbf{v}(t) \cdot \Delta t
$$

$$
\mathbf{v}(t + \Delta t) = \mathbf{v}(t) + \mathbf{a}(t) \cdot \Delta t
$$

---

## 6.4 牛顿第三定律（约简算法）

粒子 q 对 k 的力和 k 对 q 的力大小相等、方向相反：

$$
\mathbf{F}_{kq} = -\mathbf{F}_{qk}
$$

**一次计算更新两个粒子的力：**

```c
forces[q] += force_qk;    // q 受力
forces[k] -= force_qk;    // k 受反作用力
```

**循环次数减半：** 只计算 `k > q` 的部分。

---

# ch07 向量化编程

## 7.1 向量执行时间

$$
T_{\text{vector}} \approx \frac{\text{向量长度}}{\text{每条指令吞吐}}
$$

- RV64V 功能单元每个时钟周期消耗一个元素
- 执行时间大约是向量长度（除法时考虑功能单元并行度）

---

## 7.2 内存银行数量计算 ⭐

$$
\text{银行数} = \frac{\text{处理器数} \times \text{每处理器每周期访问数} \times \text{SRAM周期}}{\text{CPU周期}}
$$

**计算示例：**

```
已知：
  32 个处理器
  每处理器每周期：4 次加载 + 2 次存储 = 6 次
  CPU 周期 = 2.167 ns
  SRAM 周期（银行忙时间）= 15 ns

银行数 ≈ 32 × (4+2) × 15 / 2.167 ≈ 1344 个
```

---

## 7.3 步长 Stride

矩阵 D 按列访问时的步长：

$$
\text{stride}_D = N（一行有 N 个元素）
$$

矩阵 B 按行访问时的步长：

$$
\text{stride}_B = 1（连续存储）
$$

- **步长** = 向量寄存器中相邻两个元素在内存中的地址间距

---

## 7.4 银行冲突公式

$$
\text{实际使用的银行数} = \frac{\text{银行总数}}{\gcd(\text{stride}, \text{银行总数})}
$$

- `gcd` 越大 → 实际使用的银行越少 → 冲突越严重
- stride 为银行数整数倍时 → gcd = 银行数 → 只用到 1 个银行 → 灾难

---

## 7.5 GPU 峰值性能计算

**峰值单精度 FLOPS：**

$$
\text{FLOPS} = f_{\text{core}} \times N_{\text{GPU}} \times (N_{\text{SM}} \times N_{\text{core/SM}}) \times 2
$$

**Tesla K10 示例：**

```
FLOPS = 745 MHz × 2 GPU × (8 SM × 192 cores/SM) × 2 ops/cycle
      = 4.58 TFLOPS
```

---

**峰值内存带宽：**

$$
\text{Bandwidth} = \frac{N_{\text{GPU}} \times \text{位宽} \times f_{\text{mem}} \times 2\text{ (DDR)}}{8\text{ (bits/byte)}}
$$

**Tesla K10 示例：**

```
Bandwidth = 2 GPU × 256-bit × 2500 MHz × 2 / 8
          = 320 GB/s
```

---

## 7.6 指令:字节比

$$
\text{Ratio} = \frac{\text{峰值 FLOPS}}{\text{峰值 Bandwidth}}
$$

```
Tesla K10:  4.58 TFLOPS / 320 GB/s ≈ 13.6 指令/字节

如果每访问 1 字节发出 > 13.6 条指令 → 算术限制
否则 → 内存带宽限制（大多数 HPC 应用）
```

---

## 7.7 截分带 Strip Mining

当 `n > MVL`（最大向量长度）时：

$$
\text{vl} = \min(\text{mvl}, n)
$$

一轮处理 `vl` 个元素，`n = n - vl`，重复直到 `n = 0`。

---

# ch08 CUDA

## 8.1 线程索引公式（必背）⭐

**1D 全局线程索引：**

$$
\text{idx} = \text{blockIdx.x} \times \text{blockDim.x} + \text{threadIdx.x}
$$

**2D 行、列：**

$$
\text{row} = \text{blockIdx.y} \times \text{blockDim.y} + \text{threadIdx.y}
$$

$$
\text{col} = \text{blockIdx.x} \times \text{blockDim.x} + \text{threadIdx.x}
$$

**2D → 1D 行优先偏移：**

$$
\text{offset} = \text{row} \times N + \text{col}
$$

---

## 8.2 Block 数量向上取整 ⭐

$$
\text{blocks} = \left\lceil \frac{N}{\text{threadsPerBlock}} \right\rceil
            = \frac{N + \text{threadsPerBlock} - 1}{\text{threadsPerBlock}}
$$

**示例：**

```
N = 1000, threadsPerBlock = 256
blocks = (1000 + 255) / 256 = 4
总线程 = 4 × 256 = 1024
超出线程 = 1024 - 1000 = 24（靠 if(idx < n) 跳过）
```

---

## 8.3 2D Grid 尺寸计算

$$
\text{gridSize.x} = \left\lceil \frac{N}{\text{blockSize.x}} \right\rceil
                = \frac{N + \text{blockSize.x} - 1}{\text{blockSize.x}}
$$

$$
\text{gridSize.y} = \left\lceil \frac{N}{\text{blockSize.y}} \right\rceil
                = \frac{N + \text{blockSize.y} - 1}{\text{blockSize.y}}
$$

**示例：** N=1000, blockSize=(16,16)

```
gridSize.x = (1000 + 15) / 16 = 63
gridSize.y = (1000 + 15) / 16 = 63
总 Block  = 63 × 63 = 3969
```

---

## 8.4 线程/Block/Warp 数量关系

**每个 Block 的 Warp 数：**

$$
\text{WarpsPerBlock} = \left\lceil \frac{\text{ThreadsPerBlock}}{32} \right\rceil
$$

**每个 Block 的线程数（2D block）：**

$$
\text{ThreadsPerBlock} = \text{blockDim.x} \times \text{blockDim.y}
$$

**总线程数：**

$$
\text{TotalThreads} = \text{gridDim.x} \times \text{gridDim.y}
                   \times \text{blockDim.x} \times \text{blockDim.y}
$$

**不干活线程（最后一个 block 中）：**

$$
\text{IdleThreads} = \text{ThreadsPerBlock} - (\text{有效线程数})
$$

**有效行数** = `N - blockSize × (gridSize - 1)`，**有效列数**同理。

---

## 8.5 占用率 Occupancy

$$
\text{Occupancy} = \frac{\text{ActiveWarps}}{\text{MaxWarps}}
$$

- 每个 SM 的 MaxWarps：Fermi = 48, Kepler = 64
- 每个 SM 的 MaxThreads：Fermi = 1536, Kepler = 2048

---

## 8.6 分支效率 Branch Efficiency

$$
\text{Branch Efficiency} = 100 \times \frac{\text{Branches} - \text{DivergentBranches}}{\text{Branches}}
$$

---

## 8.7 全局内存加载效率

$$
\text{gld\_efficiency} = \frac{\text{Requested Global Load Throughput}}
                              {\text{Required Global Load Throughput}}
$$

- 衡量应用程序的加载操作对设备内存带宽的利用程度
- 越接近 100% 越好

---

## 8.8 Little 定律（延迟隐藏）

$$
\text{所需活跃 Warp} \approx \text{平均指令延迟} \times \text{每周期发射的 Warp 数}
$$

**示例：**

```
平均延迟 = 5 周期，每周期需要发射 6 个 warp
→ 至少需要 5 × 6 = 30 个活跃 warp 才能隐藏延迟
```

---

## 8.9 线程块最大线程数限制

```c
__global__ void __launch_bounds__(maxThreadsPerBlock, minBlocksPerSM)
kernel(...) { ... }
```

- 限制每 Block 最大线程数，以控制寄存器使用量
- 防止寄存器溢出到本地内存

---

# 附：公式索引（按题型分类）

## 题型 A：性能计算

| 公式 | 用途 |
|------|------|
| `S = T_serial / T_parallel` | 计算加速比 |
| `E = S / p` | 计算效率 |
| `S ≤ 1 / (f + (1-f)/p)` | 阿姆达尔定律上限 |
| `T_parallel = T_serial / p + T_overhead` | 实际并行时间 |

## 题型 B：CUDA 索引

| 公式 | 用途 |
|------|------|
| `idx = blockIdx.x * blockDim.x + threadIdx.x` | 1D 全局索引 |
| `row = blockIdx.y * blockDim.y + threadIdx.y` | 2D 行号 |
| `col = blockIdx.x * blockDim.x + threadIdx.x` | 2D 列号 |
| `offset = row * N + col` | 行优先 1D 偏移 |
| `blocks = (N + threads - 1) / threads` | Block 数向上取整 |
| `gridSize = (N + blockSize - 1) / blockSize` | Grid 尺寸 |
| `WarpsPerBlock = ceil(ThreadsPerBlock / 32)` | Warp 数 |

## 题型 C：内存系统

| 公式 | 用途 |
|------|------|
| `银行数 = 处理器×访问/周期×SRAM周期/CPU周期` | 内存银行数计算 |
| `实际使用银行 = 总银行 / gcd(stride, 总银行)` | 银行冲突程度 |
| `峰值FLOPS = f×GPU数×(SM数×核数/SM)×2` | GPU 算力 |
| `带宽 = GPU数×位宽×f_mem×2/8` | GPU 带宽 |
| `指令:字节比 = FLOPS / Bandwidth` | 性能瓶颈判断 |

## 题型 D：向量/物理

| 公式 | 用途 |
|------|------|
| `F_qk = G·m_q·m_k/r³ · Δs` | N-body 引力 |
| `s(t+Δt) = s(t) + v(t)·Δt` | 欧拉法位置更新 |
| `v(t+Δt) = v(t) + a(t)·Δt` | 欧拉法速度更新 |
| `vl = min(mvl, n)` | Strip Mining |
| `h·[f(a)/2 + Σf(x_i) + f(b)/2]` | 梯形积分 |
| `4·Σ(-1)^k/(2k+1)` | π 级数 |
