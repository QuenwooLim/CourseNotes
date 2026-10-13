# 并行程序设计 — 笔试手写代码指南

> 基于 ch01~ch08 课件笔记，覆盖 MPI / Pthreads / OpenMP / CUDA 四种编程模型的手写代码模板。

---

# 一、手写代码通用原则

1. **先写骨架，再填细节** — 函数签名、初始化、收尾不能漏
2. **关键 API 参数记准确** — 参数顺序和含义是扣分重点
3. **加上必要注释** — 向阅卷老师展示你理解每一步在做什么
4. **注意边界条件** — CUDA 的 `if (idx < n)`，MPI 的 `rank==0`
5. **避免死锁** — MPI 奇偶交替收发，Pthreads 锁的获取顺序一致

---

# 二、MPI 手写代码（ch03）

## 2.1 最小程序骨架（必背）

```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int my_rank, comm_sz;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* ===== 在这里写并行逻辑 ===== */

    MPI_Finalize();
    return 0;
}
```

**易错提醒：** `MPI_Init` 必须是第一个 MPI 调用，`MPI_Finalize` 必须是最后一个。

---

## 2.2 梯形积分（数据划分 + Reduce）

```c
#include <stdio.h>
#include <mpi.h>

double f(double x) { return x * x; }   // 被积函数

double Trap(double left, double right, int trap_count, double base_len) {
    double estimate, x;
    int i;
    estimate = (f(left) + f(right)) / 2.0;
    for (i = 1; i <= trap_count - 1; i++) {
        x = left + i * base_len;
        estimate += f(x);
    }
    return estimate * base_len;
}

int main(int argc, char* argv[]) {
    int my_rank, comm_sz, n = 1000;
    double a = 0.0, b = 1.0, h, local_a, local_b;
    double local_int, total_int;
    int local_n;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    h = (b - a) / n;
    local_n = n / comm_sz;                           // 每人负责 n/p 个梯形
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    local_int = Trap(local_a, local_b, local_n, h);

    MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);           // 求和到进程 0

    if (my_rank == 0)
        printf("Integral ≈ %.15e\n", total_int);

    MPI_Finalize();
    return 0;
}
```

---

## 2.3 向量加法（Scatter + Gather）

```c
// 进程 0 拥有完整的向量 x[ ] 和 y[ ]，分发给所有人做加法
int main(int argc, char* argv[]) {
    int my_rank, comm_sz, n = 12;
    double *global_x = NULL, *global_y = NULL, *global_z = NULL;
    double local_x[4], local_y[4], local_z[4];   // 假设 n/p = 4
    int local_n = n / comm_sz;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {
        global_x = malloc(n * sizeof(double));
        global_y = malloc(n * sizeof(double));
        global_z = malloc(n * sizeof(double));
        // 初始化 global_x, global_y...
    }

    MPI_Scatter(global_x, local_n, MPI_DOUBLE,
                local_x,  local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(global_y, local_n, MPI_DOUBLE,
                local_y,  local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
        local_z[i] = local_x[i] + local_y[i];

    MPI_Gather(local_z, local_n, MPI_DOUBLE,
               global_z, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        // global_z 持有完整结果
        free(global_x); free(global_y); free(global_z);
    }

    MPI_Finalize();
    return 0;
}
```

---

## 2.4 死锁避免 — 奇偶交替收发（必考）

```c
// 不安全写法（可能死锁）：
// MPI_Send(..., dest, ...);
// MPI_Recv(..., source, ...);     ← 所有人同时 Send，没人 Recv → 死锁

// ✅ 安全写法：奇偶交替
int partner = ...;   // 计算通信伙伴

if (my_rank % 2 == 0) {                  // 偶数先收后发
    MPI_Recv(&recv_data, ..., partner, ..., MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&send_data, ..., partner, ..., MPI_COMM_WORLD);
} else {                                 // 奇数先发后收
    MPI_Send(&send_data, ..., partner, ..., MPI_COMM_WORLD);
    MPI_Recv(&recv_data, ..., partner, ..., MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

// ✅ 或者直接用 MPI_Sendrecv
MPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag,
             recvbuf, recvcount, recvtype, source, recvtag,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```

---

## 2.5 广播输入数据 — 只有 rank=0 读 stdin

```c
double a, b;
int n;

if (my_rank == 0) {
    printf("Enter a, b, n: ");
    scanf("%lf %lf %d", &a, &b, &n);
}

// 广播给所有进程
MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Bcast(&n, 1, MPI_INT,    0, MPI_COMM_WORLD);

// 现在所有进程都有 a, b, n 了
```

---

# 三、Pthreads 手写代码（ch04）

## 3.1 最小程序骨架（必背）

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;                     // 全局：线程数
// 其他共享变量声明在这里

void* Thread_work(void* rank) {
    long my_rank = (long) rank;
    // 每个线程的局部工作
    return NULL;
}

int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       Thread_work, (void*) thread);

    printf("Hello from the main thread\n");

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
    return 0;
}
```

**易错提醒：** 传给线程的参数用 `(void*)` 强制转换，取值时用 `(long) rank`。

---

## 3.2 互斥锁保护全局求和（π 计算）

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int    thread_count;
long long n = 100000000;   // 项数
double sum = 0.0;
pthread_mutex_t mutex;

void* Thread_sum(void* rank) {
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = n / thread_count;
    long long my_first = my_n * my_rank;
    long long my_last  = my_first + my_n;
    double my_sum = 0.0;               // ★ 局部私有和

    if (my_first % 2 == 0) factor =  1.0;
    else                  factor = -1.0;

    for (i = my_first; i < my_last; i++, factor = -factor)
        my_sum += factor / (2.0 * i + 1.0);

    pthread_mutex_lock(&mutex);        // ★ 只在合并时进临界区
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       Thread_sum, (void*) thread);
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    printf("π ≈ %.15e\n", 4.0 * sum);

    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    return 0;
}
```

---

## 3.3 信号量控制顺序（链式消息传递）

```c
#include <semaphore.h>

sem_t* semaphores;  // 每个线程一个信号量

void* Send_msg(void* rank) {
    long my_rank = (long) rank;
    long dest = (my_rank + 1) % thread_count;
    char* my_msg = malloc(100);

    sprintf(my_msg, "Hello from %ld", my_rank);
    messages[dest] = my_msg;

    sem_post(&semaphores[dest]);         // 通知下一个线程
    sem_wait(&semaphores[my_rank]);      // 等待自己的通知

    printf("Thread %ld > %s\n", my_rank, messages[my_rank]);
    free(my_msg);
    return NULL;
}

// main 中：信号量全部初始化为 0
//      sem_init(&semaphores[i], 0, 0);
//      创建线程后 sem_post(&semaphores[0]) 启动链条
```

---

## 3.4 条件变量实现屏障

```c
int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void Barrier(int thread_count) {
    pthread_mutex_lock(&mutex);
    counter++;

    if (counter == thread_count) {            // 我是最后一个
        counter = 0;
        pthread_cond_broadcast(&cond_var);    // 叫醒所有人
    } else {
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
        // wait 内部：放锁 → 睡觉 → 被唤醒 → 重新拿锁 → 返回
    }

    pthread_mutex_unlock(&mutex);             // 统一在这里放锁
}
```

---

# 四、OpenMP 手写代码（ch05）

## 4.1 最小程序骨架（必背）

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);

    #pragma omp parallel num_threads(thread_count)
    {
        int my_rank = omp_get_thread_num();
        int count   = omp_get_num_threads();
        printf("Hello from thread %d of %d\n", my_rank, count);
    }
    return 0;
}
```

---

## 4.2 Parallel For + Reduction（梯形积分）

```c
double Trap(double a, double b, int n, int thread_count) {
    double h = (b - a) / n;
    double approx = (f(a) + f(b)) / 2.0;

    #pragma omp parallel for num_threads(thread_count) \
            reduction(+: approx)
    for (int i = 1; i <= n - 1; i++)
        approx += f(a + i * h);

    return h * approx;
}
```

**关键点：** `reduction(+: approx)` 让每个线程拥有 `approx` 的私有副本，最后自动合并，无需手动 critical。

---

## 4.3 Parallel For + Schedule（负载均衡）

```c
// 循环迭代计算量随 i 增加 → 用 schedule(static, 1) 循环分区
#pragma omp parallel for num_threads(thread_count) \
        reduction(+: sum) schedule(static, 1)
for (int i = 0; i <= n; i++)
    sum += f(i);
```

---

## 4.4 带数据依赖的 π 计算（用 private 消除依赖）

```c
double Pi(int n, int thread_count) {
    double sum = 0.0;

    #pragma omp parallel for num_threads(thread_count) \
            default(none) reduction(+: sum) \
            private(k, factor) shared(n)
    for (int k = 0; k < n; k++) {
        if (k % 2 == 0)
            factor = 1.0;
        else
            factor = -1.0;
        sum += factor / (2.0 * k + 1.0);
    }

    return 4.0 * sum;
}
```

---

## 4.5 并行奇偶排序（parallel 放外层复用线程）

```c
#pragma omp parallel num_threads(thread_count) \
        shared(a, n) private(i, tmp)
{
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {            // 偶数阶段
            #pragma omp for
            for (i = 1; i < n; i += 2)
                if (a[i] < a[i-1]) {
                    tmp = a[i]; a[i] = a[i-1]; a[i-1] = tmp;
                }
        } else {                          // 奇数阶段
            #pragma omp for
            for (i = 1; i < n - 1; i += 2)
                if (a[i] < a[i+1]) {
                    tmp = a[i]; a[i] = a[i+1]; a[i+1] = tmp;
                }
        }
    }  // parallel 块末尾隐式 barrier
}
```

**关键点：** `parallel` 只创建一次线程，内层 `for` 复用，外层 phase 循环结束时 `for` 有隐式 barrier。

---

## 4.6 Critical vs Atomic

```c
// critical：保护多行代码块
#pragma omp critical
{
    global_sum += my_sum;
    count++;
}

// atomic：仅保护单条 x <op>= expr（更快，硬件支持）
#pragma omp atomic
global_sum += my_sum;

// 注意：critical 和 atomic 互不排斥，同一变量不能混用！
```

---

# 五、CUDA 手写代码（ch08）

## 5.1 最小程序框架（必背）

```c
#include <stdio.h>
#include <cuda_runtime.h>

__global__ void kernel(float* d_data, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        d_data[idx] = d_data[idx] * 2.0f;
    }
}

int main() {
    int n = 1024;
    size_t size = n * sizeof(float);
    float *h_data, *d_data;

    h_data = (float*) malloc(size);
    // 初始化 h_data ...

    cudaMalloc((void**)&d_data, size);
    cudaMemcpy(d_data, h_data, size, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks  = (n + threads - 1) / threads;
    kernel<<<blocks, threads>>>(d_data, n);

    cudaMemcpy(h_data, d_data, size, cudaMemcpyDeviceToHost);

    cudaFree(d_data);
    free(h_data);
    return 0;
}
```

---

## 5.2 向量加法（1D block + 1D grid）⭐

```c
__global__ void vecAdd(float* A, float* B, float* C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;   // 全局索引
    if (i < n)                                        // ★ 边界检查
        C[i] = A[i] + B[i];
}
```

---

## 5.3 矩阵加法（2D block + 2D grid）⭐

```c
__global__ void matAdd(float* A, float* B, float* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N)                    // ★ 二维边界检查
        C[row * N + col] = A[row * N + col] + B[row * N + col];
}

// 主机端启动
// dim3 blockSize(16, 16);
// dim3 gridSize((N+15)/16, (N+15)/16);
// matAdd<<<gridSize, blockSize>>>(d_A, d_B, d_C, N);
```

---

## 5.4 矩阵转置 ⭐

```c
__global__ void Transpose(float* mat, float* matT, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N)
        matT[col * N + row] = mat[row * N + col];   // ★ 行列互换
}
```

---

## 5.5 矩阵缩放（2D block + 2D grid）⭐

```c
__global__ void MatScale2(float* mat, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N)
        mat[row * N + col] = 2.0f * mat[row * N + col];
}
```

---

## 5.6 使用共享内存（共享内存矩阵乘法示例）

```c
__global__ void sharedMemExample(float* g_in, float* g_out, int n) {
    __shared__ float s_tile[256];                   // ★ 共享内存
    int tid = threadIdx.x;
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        s_tile[tid] = g_in[idx];                   // 协作加载到共享内存
        __syncthreads();                            // ★ 确保全加载完
        // 基于共享内存做计算...
        g_out[idx] = s_tile[tid] * 2.0f;
    }
}
```

---

## 5.7 使用常量内存

```c
__constant__ float c_alpha;    // 全局声明，64KB 限制

int main() {
    float alpha = 0.5f;
    cudaMemcpyToSymbol(c_alpha, &alpha, sizeof(float));   // 主机初始化
    // ... 启动 kernel ...
}
```

---

## 5.8 原子操作（多线程安全累加）

```c
__device__ float d_total;       // 全局变量

__global__ void atomicDemo(float* data, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n)
        atomicAdd(&d_total, data[idx]);    // ★ 原子加法
}
```

---

## 5.9 索引公式速记卡（必背）

```
1D 全局线程索引:
  idx = blockIdx.x * blockDim.x + threadIdx.x

2D (row, col) 转 1D 行优先偏移:
  row = blockIdx.y * blockDim.y + threadIdx.y
  col = blockIdx.x * blockDim.x + threadIdx.x
  offset = row * N + col

Block 数向上取整:
  blocks = (N + threadsPerBlock - 1) / threadsPerBlock

边界检查:
  if (idx < n)           // 1D
  if (row < N && col < N)  // 2D

总线程数 = blocks × threadsPerBlock
超出线程不干活 → 靠 if 跳过
```

---

# 六、性能计算（ch02 + ch03）

## 6.1 加速比与效率

```
S = T_serial / T_parallel         加速比
E = S / p                         效率
T_parallel = T_serial / p + T_overhead    实际并行时间
```

## 6.2 阿姆达尔定律（必考）

```
S ≤ 1 / (f + (1-f)/p)
  f = 不可并行的串行比例

例：90% 可并行，f=0.1，p=10 核
  S = 1 / (0.1 + 0.9/10) = 1 / 0.19 ≈ 5.26
  即使 1000 核：S ≤ 1/0.1 = 10
```

## 6.3 并行耗时计算

```c
// MPI 程序计时模板
double start = MPI_Wtime();
// ... 并行计算 ...
double finish = MPI_Wtime();
double elapsed = finish - start;

// 用 MPI_Reduce 取所有进程中最大的耗时
double local_elapsed = finish - start, max_elapsed;
MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE,
           MPI_MAX, 0, MPI_COMM_WORLD);
```

---

# 七、四种模型快速选择指南

| 题目特征 | 选什么模型 |
|---------|-----------|
| "编写一个多进程程序，进程间需要交换数据" | **MPI** |
| "使用 POSIX 线程实现..." | **Pthreads** |
| "用编译指令并行化下面的 for 循环" | **OpenMP** |
| "写一个 CUDA Kernel 在 GPU 上执行" | **CUDA** |
| "多个节点/集群" | MPI |
| "单机多核，已有串行程序" | OpenMP |
| "单机多核，需要复杂线程控制" | Pthreads |
| "大规模数据并行计算" | CUDA |

---

# 八、常见笔试题型速查

## 题型 1：补全并行程序

**思路：** 先判断是哪种编程模型，套对应骨架，填关键 API。

## 题型 2：找出并修正错误

**常见错误：**
- MPI：忘记 `MPI_Init/Finalize`、所有人同时 Send 死锁、集体通信参数不兼容
- Pthreads：忘加锁、条件变量不用 while 用 if、忘记 destroy
- OpenMP：忘了 `default(none)` 导致隐含 shared 竞争、`critical` 和 `atomic` 混用
- CUDA：缺少 `if (idx < n)` 边界检查、线程索引公式写错、设备指针在主机端解引用

## 题型 3：性能计算

**给定串行时间、并行时间、核心数 → 算加速比、效率 → 判断是否可扩展。**

## 题型 4：代码输出分析

**要能模拟多个进程/线程的并发执行，注意输出顺序不确定。**

## 题型 5：Foster 四步法设计

**对给定的新问题，写出：划分 → 通信 → 聚合 → 映射 四个步骤。**
