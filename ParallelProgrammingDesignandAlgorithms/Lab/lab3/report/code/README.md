文件结构
 - `main.c`：MPI 模式并行矩阵乘法（原始模板）。
 - `matmul_pthreads.c`：Pthreads 并行矩阵乘法（本实验实现）。
 - `sum_pthreads.c`：Pthreads 并行数组求和（本实验实现）。
 - `Makefile`：构建文件（包含 MPI 与 Pthreads 目标）。
 - `run.sh`：MPI 基准脚本（保留原模板）。
 - `run_bench.sh`：Pthreads 基准脚本（新增）。

程序说明
- `matmul_pthreads`：使用 Pthreads 对矩阵按行进行划分，支持 `--partition block|cyclic`，支持 `--threads` 和 `--runs`。可通过 `--save 1` 输出 `A.bin/B.bin/C.bin`。
- `sum_pthreads`：使用 Pthreads 对数组分段求和，支持 `--partition block|cyclic`，支持 `--threads` 和 `--runs`。

构建
```bash
cd report/code
make
```

运行 Pthreads 基准
```bash
cd code
./run_bench.sh
```

结果
- `results_matmul.csv`
- `results_sum.csv`


