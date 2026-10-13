# Pthreads 并行方程求解与蒙特卡洛实验

## 项目结构

```
code/
├── task1_quadratic.c          # 一元二次方程并行求解器（C + Pthreads）
├── task2_mcPI.c               # 蒙特卡洛 π 并行估算器（C + Pthreads）
├── Makefile                   # 编译规则
├── run_experiments.py         # 实验运行脚本（Python）
├── analyze_results.py         # 结果分析与可视化脚本（Python）
├── results_quadratic_trials.csv   # 方程求解逐次实验数据
├── results_pi_trials.csv          # 蒙特卡洛 π 逐次实验数据
└── README.md                  # 本文件
```

## 编译

```bash
cd code
make
```

编译生成两个可执行文件：
- `task1_quadratic`：一元二次方程求解器
- `task2_mcPI`：蒙特卡洛 π 估算器

编译选项：`gcc -O2 -march=native -pthread -lm`

## 运行实验

```bash
python3 run_experiments.py
```

参数配置：
- 方程求解：5 组系数 × 3 种线程数(1,2,3) × 5 次重复 = 75 次运行
- 蒙特卡洛 π：5 种线程数(1,2,4,8,16) × 4 种点数(1024,4096,16384,65536) × 5 次重复 = 100 次运行

## 结果分析

```bash
python3 analyze_results.py
```

该脚本从 CSV 数据中计算统计量（均值、标准差、加速比、效率），生成：
- 3 个 LaTeX 表格文件（由 `report.tex` 自动引用）
- 5 张分析图表（PDF + PNG 格式）
