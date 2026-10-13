# Lab0: 环境设置与串行矩阵乘法

**代码及代码描述**

- `serial_*.c`：多种 C 实现：
	- `serial.c`：基础三重循环（用于 `serial_O0`/`serial_O3`）。
	- `serial_ijk.c`：按 i-j-k 顺序的实现（`IJK`）。
	- `serial_unroll.c`：对内层循环进行循环展开的实现（`Unroll`）。
	- `serial_block.c`：基于块（blocking/tiled）的实现（带可调块大小参数）。
	- `serial_simd.c`：向量化提示的实现（通过编译器提示来启用 SIMD 优化）。
	- `serial_omp.c`：使用 OpenMP 并行化外层块的实现（带 `-fopenmp` 编译）。
	- `serial_blas.c`：基于 BLAS 的实现，编译时尝试启用 `USE_BLAS` 并链接系统 BLAS/MKL（若不可用会回退或给出提示）。
- `matrix.py`：包含两种 Python 矩阵乘法实现：`numpy`（调用 NumPy 的 `dot`）和纯 Python 三重循环 `py`（用于慢速基线）。
- `Makefile`：构建各个 C 版本的规则与便捷目标（`serial_O0`、`serial_O3`、`serial_ijk`、`serial_unroll`、`serial_block`、`serial_simd`、`serial_omp`、`serial_blas`），提供 `generate_and_cleanup` 一键目标。
- `generate_and_cleanup.py`：一键编排脚本。
- `benchmark.sh`：可执行的 Bash 基准脚本，构建并按顺序运行各版本，输出 `results.csv`。

**指令**
```bash
cd code
make generate_and_cleanup
```

**输出**
- `summary_table_1024.csv` — 1024 尺寸的基础实验结果汇总（时间、相对/绝对加速、GFLOPS、占峰值百分比）。
- `results_1024_block.csv` — 1024 尺寸的更多实验结果汇总。
- `c_co0_co3_results_direct_baselined.csv` — 各种尺寸下的 `C_O0`/`C_O3` 结果。
