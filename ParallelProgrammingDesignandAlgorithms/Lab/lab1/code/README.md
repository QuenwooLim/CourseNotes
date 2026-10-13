## 项目结构
- `parallel_matmul_p2p.c`：基于 MPI 点对点的 C 实现。
- `block_matmul`：分块实验。
- `sparse_matmul`：稀疏矩阵实验。
- `Makefile`：构建 C 程序（生成 `bin/` 下的可执行文件）。
- `run_basic.sh`：基础问题脚本。
- `run_discussion.sh`：讨论问题脚本。


## 复现步骤

### 构建

```bash
cd code
make
```

### 运行基础问题的 C 实验

```bash
./run_c_bench.sh
```

### 运行讨论题的 C 实验（分块 + 稀疏）

```bash
./run_discussion_c.sh
```
