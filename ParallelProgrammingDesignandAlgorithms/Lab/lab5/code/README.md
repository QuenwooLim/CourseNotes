# Lab5 — 基于OpenMP的并行矩阵乘法

## 项目结构

```
code/
├── omp_matmul.c          # OpenMP 并行矩阵乘法程序
├── pthread_matmul.c      # Pthreads 并行矩阵乘法程序（选做对比用）
├── run_experiments.sh    # 批量运行脚本
├── results.txt           # OpenMP 全部实验结果（75条）
├── pthread_results.txt   # Pthreads 全部实验结果（25条）
└── README.md
```

## 编译

```bash
make
```

或者

```bash
# OpenMP 版本
gcc -fopenmp -O2 omp_matmul.c -o omp_matmul

# Pthreads 版本（选做）
gcc -pthread -O2 pthread_matmul.c -o pthread_matmul
```

## 运行

```bash
bash run_experiments.sh
```
