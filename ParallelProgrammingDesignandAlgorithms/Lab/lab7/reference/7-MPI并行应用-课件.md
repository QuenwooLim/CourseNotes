# 并行程序设计与算法（实验）
## 7 - MPI并行应用

**授课教师：** 吴迪、刘学正  
**单位：** 中山大学计算机学院

---

### 实验概要

#### 实验内容
- MPI点对点通信与集合通信原语使用
- 基于MPI的矩阵-向量乘法实现
- 典型问题的MPI并行化改造

#### 实验目的
- 掌握MPI点对点通信和集合通信编程方法
- 理解不同通信模式对并行程序性能的影响
- 提高MPI并行程序开发与优化能力

---

### MPI简介

**MPI (Message Passing Interface)**
- 消息传递接口标准，用于分布式内存并行计算
- 提供进程间通信的标准化接口
- 支持多种编程语言（C/C++, Fortran, Python等）

**MPI特点**
- 可移植性强，支持多种硬件平台
- 提供丰富的通信原语（点对点、集合通信等）
- 支持异步通信和非阻塞操作

---

### MPI基本概念

#### 进程与通信域
- **进程（Process）**: MPI程序的基本执行单元
- **通信域（Communicator）**: 定义进程组及其通信上下文
  - `MPI_COMM_WORLD`: 默认通信域，包含所有进程

#### 进程标识
- **Rank**: 进程在通信域中的唯一标识符（0到size-1）
- **Size**: 通信域中进程的总数

#### 基本MPI函数
```c
#include <mpi.h>

int MPI_Init(int *argc, char ***argv);
int MPI_Finalize(void);
int MPI_Comm_size(MPI_Comm comm, int *size);
int MPI_Comm_rank(MPI_Comm comm, int *rank);
```

---

### MPI点对点通信

#### 阻塞通信
- **MPI_Send**: 发送消息
- **MPI_Recv**: 接收消息

```c
int MPI_Send(void *buf, int count, MPI_Datatype datatype,
             int dest, int tag, MPI_Comm comm);

int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
             int source, int tag, MPI_Comm comm,
             MPI_Status *status);
```

#### 非阻塞通信
- **MPI_Isend**: 非阻塞发送
- **MPI_Irecv**: 非阻塞接收
- **MPI_Wait**: 等待通信完成

```c
int MPI_Isend(void *buf, int count, MPI_Datatype datatype,
              int dest, int tag, MPI_Comm comm, MPI_Request *request);

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype,
              int source, int tag, MPI_Comm comm, MPI_Request *request);

int MPI_Wait(MPI_Request *request, MPI_Status *status);
```

---

### MPI集合通信

#### 广播（Broadcast）
- **MPI_Bcast**: 将数据从一个进程广播到所有进程

```c
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype,
              int root, MPI_Comm comm);
```

#### 散播（Scatter）
- **MPI_Scatter**: 将数据从一个进程分散到所有进程

```c
int MPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm);
```

#### 聚集（Gather）
- **MPI_Gather**: 将所有进程的数据聚集到一个进程

```c
int MPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm);
```

#### 全聚集（Allgather）
- **MPI_Allgather**: 将所有进程的数据聚集到所有进程

```c
int MPI_Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm);
```

#### 归约（Reduce）
- **MPI_Reduce**: 对所有进程的数据进行归约操作

```c
int MPI_Reduce(void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
```

---

### 矩阵-向量乘法并行化

#### 问题描述
矩阵-向量乘法是线性代数中的基本操作：
$$y = Ax$$

其中：
- $A$ 是 $m \times n$ 矩阵
- $x$ 是 $n$ 维向量
- $y$ 是 $m$ 维向量

#### 数据划分策略
1. **按行划分**: 将矩阵按行分块，每个进程负责部分行的计算
2. **按列划分**: 将矩阵按列分块，每个进程负责部分列的计算
3. **块划分**: 将矩阵划分为子块，每个进程负责一个子块

#### 按行划分实现
- 主进程将矩阵行分发给各工作进程
- 所有进程都需要完整的向量 $x$
- 工作进程计算局部结果，主进程收集最终结果

---

### MPI程序编译与运行

#### 编译
使用MPI编译器包装器：
```bash
# C程序
mpicc -O2 program.c -o program

# C++程序
mpicxx -O2 program.cpp -o program
```

#### 运行
使用MPI运行器：
```bash
# 使用4个进程运行
mpirun -n 4 ./program

# 指定主机列表
mpirun -hostfile hostfile -n 4 ./program
```

#### 环境验证
```bash
# 检查MPI安装
mpicc --version
mpirun --version

# 简单测试程序
cat > test_mpi.c << 'EOF'
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    printf("Hello from process %d of %d\n", rank, size);
    
    MPI_Finalize();
    return 0;
}
EOF

mpicc test_mpi.c -o test_mpi
mpirun -n 4 ./test_mpi
```

---

### 性能分析与优化

#### 性能指标
- **加速比（Speedup）**: $S_p = \frac{T_1}{T_p}$
- **效率（Efficiency）**: $E_p = \frac{S_p}{p} = \frac{T_1}{p \cdot T_p}$

其中：
- $T_1$ 是串行程序执行时间
- $T_p$ 是p个进程并行程序执行时间
- $p$ 是进程数量

#### 优化策略
1. **减少通信开销**
   - 合并小消息
   - 使用非阻塞通信重叠计算与通信
   - 选择合适的通信模式

2. **负载均衡**
   - 确保各进程计算量均衡
   - 考虑数据局部性

3. **内存优化**
   - 减少内存分配/释放次数
   - 优化数据布局提高缓存命中率

---

### 实验要求

#### 1. MPI点对点通信应用
- 实现基于MPI_Send/MPI_Recv的矩阵-向量乘法
- 考虑不同的数据划分策略
- 测试不同进程数量下的程序性能

#### 2. MPI集合通信应用
- 利用MPI集合通信原语重新实现矩阵-向量乘法
- 对比点对点通信和集合通信两种实现方式的性能差异

#### 3. MPI并行应用优化
- 分析当前实现的性能瓶颈
- 尝试不同的优化策略
- 记录优化前后的性能对比

---

### 参考资料

1. **MPI标准文档**: [https://www.mpi-forum.org/docs/](https://www.mpi-forum.org/docs/)
2. **MPICH**: [https://www.mpich.org/](https://www.mpich.org/)
3. **Open MPI**: [https://www.open-mpi.org/](https://www.open-mpi.org/)
4. **《Using MPI》**: William Gropp, Ewing Lusk, Anthony Skjellum

---
