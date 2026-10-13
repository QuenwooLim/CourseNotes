# 并行程序设计 API 与概念速查手册

> 按章节划分，覆盖 ch01~ch08 全部常用 API 和核心概念。

---

# ch01 并行计算导论

## 核心概念

| 概念 | 定义 |
|------|------|
| **并发 Concurrent** | 同一时间段多个任务在进行中，不一定同一时刻执行（单核时间片轮转） |
| **并行 Parallel** | 同一时刻多个任务在不同核心上同时执行 |
| **分布式计算** | 任务分配到不同计算节点，程序需与其他程序协作 |
| **共享内存** | 核心共享访问同一内存，通过读写共享变量协调 |
| **分布式内存** | 每个核心有自己的私有内存，通过网络显式发送消息通信 |
| **任务并行** | 将不同任务分配给各核心 |
| **数据并行** | 将数据分块分配给各核心，每个核心执行类似操作 |
| **协调三要素** | 通信、负载均衡、同步 |

---

# ch02 并行硬件和软件

## 核心概念

| 概念 | 定义 |
|------|------|
| **Flynn 分类法** | SISD / SIMD / MISD / MIMD（按指令流+数据流分类） |
| **SIMD** | 单指令多数据，适用于向量/矩阵运算 |
| **MIMD** | 多指令多数据，多核CPU、分布式系统 |
| **共享内存** | MIMD 的一大类，处理器间通过共享内存通信 |
| **分布式内存** | MIMD 的另一大类，处理器间通过消息传递通信 |
| **CSM / UMA / SMP** | 集中共享内存 = 统一内存访问 = 对称多处理器。所有 CPU 通过总线访问一个集中内存，访问延迟相同，扩展性受限 |
| **DSM / NUMA** | 分布式共享内存 = 非统一内存访问。内存物理分布在各节点但逻辑统一编址，访问本地快、远程慢，用 LOAD/STORE 访问远程内存 |
| **CC-NUMA** | 具有缓存一致性的 NUMA |
| **NC-NUMA** | 无缓存一致性的 NUMA |
| **MPP** | 大规模并行处理器（分布式内存，定制高速网络，数百~数千节点） |
| **COW** | 工作站机群系统（分布式内存，商用网络+商用芯片，如数据中心） |
| **SIMT** | 单指令多线程，SIMD+多线程结合（GPU） |
| **哈佛结构** | 程序存储和数据存储分开 |
| **改进哈佛** | 地址总线和数据总线合并，分时复用 |
| **虚拟存储器** | 用磁盘扩展内存，页表映射虚拟→物理地址 |
| **TLB** | 页表缓存，快查虚拟→物理地址映射 |
| **ILP** | 指令级并行，流水线+多发射 |
| **硬件多线程** | 细粒度/粗粒度/SMT |
| **SPMD** | 单程序多数据，一份代码靠 rank/ID 分支做不同事 |
| **冯诺依曼瓶颈** | 指令和数据共用总线，串行执行 |

## 互连网络概念

| 概念 | 定义 |
|------|------|
| **ICN** | 互连网络，节点间传输数据 |
| **拓扑** | 网络形状（全连接/环/超立方体等） |
| **直径** | 网络中任意两节点最大跳数 |
| **等分宽度** | 将网络切成两半时跨越切口的链路带宽之和 |
| **直接互连** | 每个交换机直接连处理器-内存对 |
| **间接互连** | 交换机组成独立网络，不一定直接连处理器 |
| **Crossbar** | 交叉开关，N×N 交叉点，每个输入连每个输出 |
| **存储转发** | 整包收完再转发 |
| **直通式 Cut-Through** | 边收边发（流水线），降低拓扑重要性 |
| **死锁** | 多消息循环等待缓冲区，通过路由限制避免 |
| **Memory Bank** | 内存体，把一个大内存切成多个小的，并行访问 |
| **Scatter/Gather** | 分散/聚集，处理不规则内存访问 |

## 并行体系结构层次关系图

```
并行计算
├── 按执行方式
│   ├── 并发 Concurrent ── 同一时间段，逻辑上同时
│   └── 并行 Parallel ─ 同一时刻，物理上同时
│
├── 按内存模型
│   ├── 共享内存 ── Pthreads / OpenMP
│   │   └── 核心通过共享变量通信
│   └── 分布式内存 ─── MPI
│       └── 核心通过消息传递通信
│
├── 按硬件并发方式
│   ├── ILP 指令级并行
│   │   ├── 流水线 Pipeline
│   │   ├── 多发射 Multiple Issue
│   │   │   ├── 静态多发射（编译时调度）
│   │   │   └── 动态多发射（运行时调度 = 乱序执行）
│   │   └── 推测 Speculation
│   ├── 硬件多线程
│   │   ├── 细粒度 Fine-grained ── 每条指令切换
│   │   ├── 粗粒度 Coarse-grained ── 长停顿时切换
│   │   └── 同时多线程 SMT ── 同周期多线程发射
│   └── 数据并行（向量/SIMD/GPU）
│
├── 按编程模型
│   ├── MPI ─ 分布式内存，消息传递，进程
│   ├── Pthreads ─ 共享内存，显式线程管理
│   ├── OpenMP ─── 共享内存，编译指令自动并行
│   └── CUDA ── 异构计算，GPU海量线程
│
├── 互连网络 ICN（按距离）
│   ├── 片上网络 On-Chip ── 毫米级，芯片内部
│   ├── 系统区域网络 SAN ── 分米~百米，机柜/机器内
│   ├── 局域网 LAN ── 数米~数十公里
│   └── 广域网 WAN ── 数千公里，全球
│
└── 非冯·诺依曼体系结构
    ├── 哈佛结构 ─ 指令/数据存储器分离
    │   └── 改进哈佛 ── 地址/数据总线合并，分时复用
    ├── 数据流计算机 ── 数据就绪即执行
    ├── 归约机
    ├── 量子计算机
    └── 光子计算机
```


```
├── Flynn 分类法（按指令流×数据流）
│   ├── SISD ─ 传统串行计算机
│   ├── SIMD ─ 向量机 / SIMD扩展(SSE/AVX) / GPU
│   │   ├── 阵列计算机
│   │   ├── 向量计算机 ── Cray-1, RV64V
│   │   └── GPU (SIMT) ── NVIDIA CUDA
│   ├── MISD ─ 理论存在，实际极少
│   └── MIMD ─ 当前主流
│       ├── 共享内存（按物理结构）
│       │   ├── CSM = UMA = SMP ── 集中式，统一延迟
│       │   └── DSM = NUMA ─ 分布式，非统一延迟
│       │       ├── CC-NUMA ── 有缓存一致性
│       │       └── NC-NUMA ── 无缓存一致性
│       └── 分布式内存（消息传递）
│           ├── MPP ── 大规模并行处理，定制网络
│           └── COW ── 工作站机群，商用网络
```

## 性能公式

| 公式 | 含义 |
|------|------|
| `S = T_serial / T_parallel` | 加速比 |
| `E = S / p` | 效率 |
| `T_parallel = T_serial / p + T_overhead` | 真实并行时间 |
| **阿姆达尔定律** | `S ≤ 1 / (f + (1-f)/p)`，f=串行比例 |
| **强可扩展性** | 问题规模固定，增加处理器 → 时间减少 |
| **弱可扩展性** | 问题规模随处理器同比增加 → 时间不变 |

## Foster 方法论（四步法）

1. **划分 Partitioning** — 计算和数据分解为小任务
2. **通信 Communication** — 确定任务间的数据依赖
3. **聚合 Agglomeration** — 合并任务减少通信开销
4. **映射 Mapping** — 将任务分配给处理器

---

# ch03 MPI 分布式内存编程

## MPI 基础 API

| 函数 | 功能 |
|------|------|
| `MPI_Init(&argc, &argv)` | 初始化 MPI 环境，必须是第一个 MPI 调用 |
| `MPI_Finalize()` | 清理 MPI 资源，必须是最后一个 MPI 调用 |
| `MPI_Comm_size(comm, &size)` | 获取通信器中的进程总数 |
| `MPI_Comm_rank(comm, &rank)` | 获取当前进程的秩（0 到 size-1） |
| `MPI_Wtime()` | 返回墙上时钟时间（秒），用于计时 |
| `MPI_Barrier(comm)` | 同步屏障：所有进程到达后才继续 |

## 点对点通信

```c
int MPI_Send(void* buf, int count, MPI_Datatype type,
             int dest, int tag, MPI_Comm comm);

int MPI_Recv(void* buf, int count, MPI_Datatype type,
             int source, int tag, MPI_Comm comm, MPI_Status* status);

int MPI_Ssend(void* buf, int count, MPI_Datatype type,
              int dest, int tag, MPI_Comm comm);          // 同步发送，保证阻塞直到接收开始

int MPI_Sendrecv(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                 int dest, int sendtag,
                 void* recvbuf, int recvcount, MPI_Datatype recvtype,
                 int source, int recvtag,
                 MPI_Comm comm, MPI_Status* status);       // 合并发送+接收，避免死锁
```

### MPI 数据类型

| MPI 类型 | C 类型 |
|---------|--------|
| `MPI_CHAR` | signed char |
| `MPI_INT` | signed int |
| `MPI_FLOAT` | float |
| `MPI_DOUBLE` | double |
| `MPI_LONG` | signed long int |

### 通配符

| 通配符 | 含义 |
|--------|------|
| `MPI_ANY_SOURCE` | 接收任意来源的消息 |
| `MPI_ANY_TAG` | 接收任意标签的消息 |

### Status 查询

| 字段/函数 | 获取信息 |
|-----------|---------|
| `status.MPI_SOURCE` | 消息来源进程 rank |
| `status.MPI_TAG` | 消息标签 |
| `MPI_Get_count(&status, type, &count)` | 获取实际接收的数据量 |

## 归约操作符

| 操作符 | 运算 |
|--------|------|
| `MPI_MAX` | 最大值 |
| `MPI_MIN` | 最小值 |
| `MPI_SUM` | 求和 |
| `MPI_PROD` | 乘积 |
| `MPI_LAND` | 逻辑与 |
| `MPI_LOR` | 逻辑或 |
| `MPI_LXOR` | 逻辑异或 |
| `MPI_BAND` | 按位与 |
| `MPI_BOR` | 按位或 |
| `MPI_BXOR` | 按位异或 |

## 集体通信 API

```c
// 归约：所有进程的数据 → 运算 → 结果存到 dest 进程
int MPI_Reduce(void* sendbuf, void* recvbuf, int count,
               MPI_Datatype type, MPI_Op op, int dest, MPI_Comm comm);

// 归约 + 广播结果给所有进程
int MPI_Allreduce(void* sendbuf, void* recvbuf, int count,
                  MPI_Datatype type, MPI_Op op, MPI_Comm comm);

// 广播：root → 所有进程
int MPI_Bcast(void* data, int count, MPI_Datatype type,
              int root, MPI_Comm comm);

// 分散：root 将数据分成块分发给所有进程
int MPI_Scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                void* recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm);

// 收集：所有进程数据 → root
int MPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
               void* recvbuf, int recvcount, MPI_Datatype recvtype,
               int dest, MPI_Comm comm);

// 收集 + 广播给所有进程
int MPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                  void* recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm);
```

## 集体通信规则（必考）

1. 通信器中**所有进程必须调用相同的集体函数**
2. 各进程传递的参数必须**兼容**（count, type, op, dest/root 必须一致）
3. `recvbuf` 仅在 dest 进程有意义，但所有进程仍需传实参（可为 NULL）
4. 集体通信**不使用 tag**，仅基于通信器和调用顺序匹配
5. **调用顺序决定匹配**，不靠变量名

## 派生数据类型

```c
int MPI_Type_create_struct(int count, int blocklens[],
                           MPI_Aint displacements[], MPI_Datatype types[],
                           MPI_Datatype* newtype);
int MPI_Get_address(void* location, MPI_Aint* address);
int MPI_Type_commit(MPI_Datatype* type);
int MPI_Type_free(MPI_Datatype* type);
```

## 高级通信

```c
int MPI_Scatterv(...);     // Scatter 变体，不同进程可接收不同数量
int MPI_Gatherv(...);      // Gather 变体，不同进程可发送不同数量
int MPI_Pack(...);         // 数据打包到连续缓冲区
int MPI_Unpack(...);       // 数据解包
int MPI_Iprobe(...);       // 非阻塞探测是否有消息到达
int MPI_Rsend(...);        // 就绪发送，要求接收方必须先准备好
int MPI_Bsend(...);        // 缓冲发送
```

## MPI 核心概念

| 概念 | 含义 |
|------|------|
| **通信器 Communicator** | 能相互发送消息的进程集合，默认 `MPI_COMM_WORLD` |
| **秩 Rank** | 进程在通信器中的唯一编号（0 ~ p-1） |
| **SPMD** | 所有进程跑同一份代码，靠 rank 分支做不同事 |
| **不安全程序** | 依赖 `MPI_Send` 缓冲行为的程序（大消息可能阻塞→死锁） |
| **死锁避免** | 奇偶交替发送接收 / 用 `MPI_Sendrecv` |

---

# ch04 Pthreads 共享内存编程

## 线程管理

```c
int pthread_create(pthread_t* thread, const pthread_attr_t* attr,
                   void* (*start_routine)(void*), void* arg);
int pthread_join(pthread_t thread, void** retval);
```

编译：`gcc -g -Wall -o prog prog.c -lpthread`

## 互斥锁 Mutex

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
```

## 信号量 Semaphore

```c
#include <semaphore.h>
int sem_init(sem_t* sem, int pshared, unsigned int value);
int sem_wait(sem_t* sem);      // P 操作：减 1；值为 0 则阻塞
int sem_post(sem_t* sem);      // V 操作：加 1；有等待线程则唤醒
int sem_destroy(sem_t* sem);
```

- **sem=1**：等同互斥锁，保护临界区
- **sem=N**：管理资源池，允许 N 个线程同时访问

## 条件变量 Condition Variable

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
    // 原子操作：释放 mutex + 阻塞等待
    // 被唤醒后自动重新获取 mutex 再返回
int pthread_cond_signal(pthread_cond_t* cond);        // 唤醒一个等待线程
int pthread_cond_broadcast(pthread_cond_t* cond);     // 唤醒所有等待线程
int pthread_cond_destroy(pthread_cond_t* cond);
```

**条件变量总是与互斥锁关联**：条件本身是共享数据需保护，且 `wait` 的原子"放锁+阻塞"消除了丢失唤醒窗口。

## 读写锁 Read-Write Lock

```c
pthread_rwlock_t rwlock;
int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock);   // 读锁：多线程可同时持有
int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock);   // 写锁：独占
int pthread_rwlock_unlock(pthread_rwlock_t* rwlock);
```

适用：读多写少场景（如链表 Member 操作占 99.9%）。

## 线程安全

```c
char* strtok_r(char* str, const char* sep, char** saveptr);
// 线程安全版 strtok，saveptr 替代 static 变量
```

## Pthreads 核心概念

| 概念 | 含义 |
|------|------|
| **竞态条件 Race Condition** | 多线程无序访问共享变量 → 结果不确定 |
| **临界区 Critical Section** | 一次只能一个线程执行的代码块 |
| **忙等待 Busy Waiting** | while 循环空转等待条件 → 浪费 CPU |
| **互斥锁** | 限制临界区一次一个线程访问，有所有权 |
| **信号量** | 计数器+阻塞机制，无所有权（谁都能 post） |
| **屏障 Barrier** | 所有线程到达才继续 |
| **伪共享 False Sharing** | 不同变量在同一缓存行 → 一个核修改导致另一核缓存失效 |
| **线程安全 Thread-Safety** | 被多线程同时调用不会出错的代码 |
| **条件变量 wait 模式** | 永远用 `while(!condition) wait()` 而非 `if`（防止假醒） |

---

# ch05 OpenMP 共享内存编程

## 编译

```bash
gcc -g -Wall -fopenmp -o prog prog.c
```

## 指令 Directives

| 指令 | 功能 |
|------|------|
| `#pragma omp parallel` | 创建线程团队 |
| `#pragma omp parallel for` | 创建团队 + 并行化 for 循环 |
| `#pragma omp for` | 用已有线程团队并行化 for（必须在 parallel 块内） |
| `#pragma omp critical` | 临界区（一次一个线程） |
| `#pragma omp critical(name)` | 命名临界区（不同名可同时执行） |
| `#pragma omp atomic` | 原子操作（仅单条语句 `x op= expr`） |
| `#pragma omp barrier` | 显式屏障 |
| `#pragma omp single` | 仅一个线程执行（内部有隐式 barrier） |
| `#pragma omp master` | 仅主线程执行（无隐式 barrier） |

## 子句 Clauses

| 子句 | 作用 |
|------|------|
| `num_threads(n)` | 指定线程数 |
| `private(var1, var2)` | 变量每个线程私有 |
| `shared(var1, var2)` | 变量所有线程共享 |
| `default(none)` | 强制显式声明每个变量的作用域 |
| `reduction(op: var)` | 归约：op=`+`,`*`,`-`,`&`,`|`,`^`,`&&`,`||` |
| `schedule(type, chunk)` | 循环调度策略 |

## 调度类型

| 类型 | 行为 |
|------|------|
| `schedule(static, 1)` | 循环分区（轮询发牌），负载均衡最好 |
| `schedule(static)` | 默认块分区（连续分配） |
| `schedule(static, N)` | 块循环分区（每块 N 个轮询） |
| `schedule(dynamic, N)` | 运行时动态分配 |
| `schedule(guided, N)` | 块从大到小递减 |
| `schedule(auto)` | 编译器/运行时决定 |
| `schedule(runtime)` | 环境变量 `OMP_SCHEDULE` 决定 |

## 运行时函数

```c
int omp_get_thread_num();    // 获取线程编号
int omp_get_num_threads();   // 获取线程总数
```

## 锁

```c
omp_lock_t lock;
void omp_init_lock(omp_lock_t* lock);
void omp_set_lock(omp_lock_t* lock);
void omp_unset_lock(omp_lock_t* lock);
void omp_destroy_lock(omp_lock_t* lock);
```

## OpenMP 核心概念

| 概念 | 含义 |
|------|------|
| **变量默认作用域** | parallel 块前声明的变量默认 shared；循环变量在 parallel for 中默认 private |
| **数据依赖** | 循环携带依赖（如 Fibonacci）不能直接 `parallel for` |
| **`critical` vs `atomic` vs 锁** | 三者互不通信，同一变量不可混用 |
| **归约原理** | 每线程私有副本 + 最后合并，无需 critical |
| **Fork-Join 模型** | 主线程 fork 出团队 → 并行执行 → join 回主线程 |
| **不要混用互斥** | critical 不排斥 atomic，atomic 不排斥 lock |

---

# ch06 并行程序开发

## N-body 问题

| 概念 | 含义 |
|------|------|
| **约简算法** | 利用牛顿第三定律：`k>q` 只算一次，同时更新两个粒子的力 |
| **环传递 Ring Pass** | MPI 中粒子位置沿环传递，每阶段收到新位置就计算与本地粒子的力 |
| **两阶段法** | OpenMP 中: 阶段I各线程算到自己的局部力数组，阶段II汇总 |

## TSP 旅行商问题

| 概念 | 含义 |
|------|------|
| **静态并行化** | 主线程 BFS 生成初始路径分给各线程 |
| **动态并行** | 工作窃取：没工作的线程向有工作的请求任务 |
| **终止检测** | 能量守恒法：每个进程 1 单位能量，汇总到进程 0 |
| **lock-then-test-again** | 先加锁再重新检查最优值，防止用旧值覆盖新值 |

## 高级 MPI 功能

```c
MPI_Iprobe(source, tag, comm, &flag, &status);    // 非阻塞探测消息
MPI_Allreduce(&loc, &global, 1, MPI_2INT, MPI_MINLOC, comm);  // 找最小代价及其 rank
MPI_Pack / MPI_Unpack                               // 打包/解包工作数据
```

---

# ch07 向量化编程

## 向量体系结构

| 概念 | 含义 |
|------|------|
| **向量寄存器** | 一个寄存器存多个数据元素 |
| **向量指令** | 一条指令操作整个向量寄存器 |
| **通路 Lane** | 向量单元中的一条并行数据通道 |
| **Convoy 船队** | 可同时执行的一组无结构冲突的向量指令 |
| **向量链接 Chaining** | 前一个结果直接传递给下一个功能单元（寄存器旁路） |
| **条分带 Strip Mining** | 当 n > MVL 时，循环处理：每轮处理 MVL 个，最后处理余数 |
| **VL 寄存器** | 向量长度寄存器，控制向量操作的元素个数 |
| **掩码寄存器** | 控制向量指令中哪些元素参与操作（条件执行） |
| **跨步 Stride** | 向量寄存器中相邻元素在内存中的地址间距 |
| **Gather/Scatter** | 按索引向量收集/分散不规则数据 |
| **稀疏矩阵** | 用索引向量 K, M 间接访问非零元素 |
| **内存银行冲突** | 步长为银行数整数倍时所有访问打同一银行，变成串行了 |

## 向量指令示例 (RV64V)

| 指令 | 功能 |
|------|------|
| `vld v0, x5` | 向量加载（连续） |
| `vldx v1, x5, v0` | 向量索引加载（Gather） |
| `vst v3, x6` | 向量存储 |
| `vstx v1, x5, v0` | 向量索引存储（Scatter） |
| `vmul v1, v0, f0` | 向量-标量乘法 |
| `vadd v3, v1, v2` | 向量-向量加法 |
| `vsub v0, v0, v1` | 向量减法 |
| `vpne p0, v0, f0` | 向量比较（不等则置掩码位） |
| `setvl t0, a0` | 设置向量长度 = min(mvl, n) |
| `vsetdcfg N*FP64` | 配置 N 个双精度向量寄存器 |
| `vdisable` | 禁用向量寄存器 |

## SIMD 扩展 (SSE/AVX)

| 架构 | 宽度 | 一次处理 |
|------|------|---------|
| MMX (1996) | 64 位 | 8×8bit / 4×16bit |
| SSE (1999) | 128 位 | 4×32bit fp |
| AVX (2010) | 256 位 | 4×64bit fp |
| AVX-512 (2017) | 512 位 | 8×64bit fp |

**SIMD 扩展局限性 vs 向量机：** 无掩码寄存器、不支持跨步/聚集、操作数数量编码在操作码中。

## GPU 核心概念

| 概念 | 含义 |
|------|------|
| **异构计算** | CPU(主机) + GPU(设备) 协作 |
| **吞吐量换延迟** | 用大量线程的计算填充内存等待时间 |
| **细粒度多线程** | 每周期切换线程，隐藏内存延迟 |
| **宽 SIMD/SIMT** | 一条指令同时操作 8/16/32 个数据 |
| **简化核心** | 砍掉预测器/乱序/大缓存，面积全堆 ALU |
| **砖墙** | 功耗墙 + 内存墙 + ILP 墙 |
| **SM** | 流式多处理器，GPU 的计算单元 |
| **PE** | 处理单元（ALU） |
| **FMA** | 融合乘加单元 `a×b+c` |
| **SFU** | 特殊功能单元（sin/cos/sqrt） |

---

# ch08 CUDA 编程

## 内存管理

```c
cudaError_t cudaMalloc(void** devPtr, size_t size);
cudaError_t cudaFree(void* devPtr);
cudaError_t cudaMemcpy(void* dst, const void* src, size_t count,
                        enum cudaMemcpyKind kind);
// kind: cudaMemcpyHostToDevice / cudaMemcpyDeviceToHost / cudaMemcpyDeviceToDevice
cudaError_t cudaMemcpyToSymbol(const void* symbol, const void* src,
                                size_t count);  // 初始化常量内存
cudaError_t cudaMemset(void* devPtr, int value, size_t count);
```

## 设备管理

```c
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp* prop, int device);
cudaError_t cudaSetDevice(int device);
cudaError_t cudaDeviceSynchronize();    // Host 等待 Device 所有操作完成
cudaError_t cudaDeviceReset();          // 销毁并清理当前设备资源
```

## 函数限定符

| 限定符 | 执行位置 | 可调用方 |
|--------|---------|---------|
| `__global__` | Device | Host（或 CC≥3.5 从 Device 调用） |
| `__device__` | Device | Device only |
| `__host__` | Host | Host only（默认，可省略） |

**Kernel 限制：** void 返回、不支持可变参数/静态变量/函数指针、仅访问设备内存。

## 变量声明限定符

| 限定符 | 内存位置 | 作用域 | 生命周期 |
|--------|---------|--------|---------|
| 自动变量（无修饰） | 寄存器（或溢出到本地内存） | 单线程 | Kernel |
| `__shared__` | 片上共享内存 | Block 内所有线程 | Block |
| `__constant__` | 常量内存+缓存 | 全局（只读） | 程序 |
| `__device__` | 全局内存 | 全局 | 程序 |

## 线程层次变量（内置，只读）

| 变量 | 类型 | 含义 |
|------|------|------|
| `threadIdx.x/y/z` | uint3 | 线程在块内的索引 |
| `blockIdx.x/y/z` | uint3 | 块在网格内的索引 |
| `blockDim.x/y/z` | dim3 | 每块的线程数 |
| `gridDim.x/y/z` | dim3 | 网格的块数 |

## Kernel 启动

```c
kernelName<<<gridDim, blockDim, sharedMemBytes, stream>>>(args);
// gridDim, blockDim 是 dim3 类型
// 启动是异步的
```

## 索引公式（必考）

```c
// 1D: 全局线程索引
int i = blockIdx.x * blockDim.x + threadIdx.x;

// 2D → 1D 行优先矩阵:
int row = blockIdx.y * blockDim.y + threadIdx.y;
int col = blockIdx.x * blockDim.x + threadIdx.x;
int idx = row * N + col;                       // (row, col) 的 1D 偏移

// Block 数向上取整:
int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;
```

## 边界检查（必考）

```c
if (idx < n) {  // 或 if (row < N && col < N)
    // 安全访问
}
// 不写则越界 → 结果错误或 Kernel 崩溃
```

## 同步

```c
__device__ void __syncthreads();   // Block 内同步屏障
// 无 Block 间同步原语（只能在 Kernel 边界同步）
```

## 原子操作

```c
int atomicAdd(int* address, int val);
float atomicAdd(float* address, float val);
// 还有 atomicSub, atomicExch, atomicMin, atomicMax, atomicAnd, atomicOr, atomicXor, atomicCAS
```

## Warp

- Warp = 32 个线程，GPU 硬件执行的基本单位
- Warp 内所有线程以 SIMT 方式执行相同指令
- **Warp 发散**：同一 Warp 内不同分支 → 串行执行 → 避免（分支粒度对齐 Warp 大小）

```
Divergent Loop 是 Warp 发散的一种特例。 

Warp 发散（父概念）
├── if-else 分支发散      
|   同一 warp 内，部分线程走 if，部分走 else
├── 循环发散 (Divergent Loop) 
|   同一 warp 内，部分线程继续循环，部分退出循环，循环次数不同
└── 其他控制流发散          
    switch-case 等
```
## 共享内存配置

```c
cudaError_t cudaFuncSetCacheConfig(const void* func, enum cudaFuncCache config);
// cudaFuncCachePreferNone   — 默认
// cudaFuncCachePreferShared — 48KB 共享 + 16KB L1
// cudaFuncCachePreferL1     — 16KB 共享 + 48KB L1
// cudaFuncCachePreferEqual  — 32KB + 32KB
```

## 性能分析

```bash
nvprof ./app                            # 命令行分析
nvprof --metrics gld_efficiency ./app   # 全局加载效率
nvprof --metrics branch_efficiency ./app # 分支效率
nvidia-smi                              # GPU 状态查询
```

## CUDA 核心概念

| 概念 | 含义 |
|------|------|
| **内核 Kernel** | 在 GPU 上运行的函数 |
| **Grid** | 执行同一 Kernel 的所有线程块集合 |
| **Block** | 分配到同一 SM 上的一组线程 |
| **Warp** | 32 线程硬件执行组 |
| **占用率 Occupancy** | 活跃 Warp / 最大 Warp |
| **SIMT** | 单指令多线程，标量线程隐藏向量宽度 |
| **动态并行** | Kernel 内部启动子 Kernel（CC≥3.5） |
| **合并访问 Coalesced** | 连续线程访问连续地址，最大化带宽 |

## GPU 架构演进关键词

| 架构 | 亮点 |
|------|------|
| Fermi (2010) | 首个完整计算架构，16 SM，可配置 64KB 片上内存 |
| Kepler (2012) | 动态并行，Hyper-Q |
| Maxwell (2014) | 能效优化 |
| Pascal (2016) | NVLink, HBM2 |
| Volta (2017) | Tensor Core |
| Ampere/Hopper | AI 加速 |

---

## 三种编程模型速记对比

| | MPI | Pthreads | OpenMP | CUDA |
|---|---|---|---|---|
| **内存模型** | 分布式 | 共享 | 共享 | 异构 |
| **并行单元** | 进程 | 线程 | 线程 | 线程（海量） |
| **通信方式** | 消息传递 | 共享变量+锁 | 共享变量+pragma | 全局/共享内存 |
| **扩展性** | 跨节点 | 单机 | 单机 | 单 GPU / 多 GPU |
| **编程难度** | 高 | 中 | 低 | 中 |
| **同步方式** | Barrier/集体通信 | mutex/sem/cond | critical/barrier | __syncthreads |
| **编译** | mpicc | gcc -lpthread | gcc -fopenmp | nvcc |
| **运行** | mpiexec -n N | ./prog N | ./prog | ./prog |
