# 计算机体系结构

本目录整理了计算机体系结构课程的三次实验资料，内容涉及处理器性能仿真、Cache 一致性协议验证和图像算法性能优化。

## 📂 目录

| 内容 | 说明 |
|:---|:---|
| [第一次作业](lab_1/) | CPU 仿真参数分析与性能数据可视化 |
| [第二次作业](lab_2/) | 基于 Murphi 的 MSI Cache 一致性协议建模与验证 |
| [大作业](lab_final/) | 图像处理算法实现、优化与性能对比 |

## 🖥️ 第一次作业：CPU 仿真与参数分析

[lab_1](lab_1/)目录包含：

- [作业手册](lab_1/作业手册.pdf)
- [实验报告](lab_1/README.md)
- `O3CPU.py` 和 `O3CPU_pro.py`：CPU 仿真程序
- `daxpy.cpp`：DAXPY 测试程序
- `ayalyze_results.py` 和 `analyze_results_pro.py`：结果分析程序
- `draw.py` 和 `draw_pro.py`：数据可视化程序
- `summary.txt` 和 `summary_pro.txt`：实验结果汇总

实验材料围绕 IQ、ROB 和物理寄存器等参数对 CPU 模拟时钟数的影响展开，并保留了热力图、折线图和实验截图。

## 🔄 第二次作业：MSI Cache 一致性协议

[lab_2](lab_2/)目录包含：

- [作业手册](lab_2/作业手册.pdf)
- [实验报告](lab_2/README.md)
- [Murphi3.1](lab_2/Murphi3.1/)：协议建模与验证工具及其源码
- [MSI 协议模型](lab_2/Murphi3.1/lab2/msi.m)
- [MESI 协议模型](lab_2/Murphi3.1/lab2/mesi.m)
- [Ping-Pong 测试模型](lab_2/Murphi3.1/lab2/pingpong.m)
- [实验截图](lab_2/src/)

实验报告记录了 MSI 状态转换、处理器行为建模、缓存一致性不变式和 Murphi 验证结果。

## ⚡ 大作业：图像算法优化

[lab_final](lab_final/)目录包含：

- [作业手册](lab_final/作业手册.pdf)
- [实验报告](lab_final/README.md)
- [项目说明](lab_final/problem/README.md)
- [图像算法头文件](lab_final/problem/include/image_algorithms.hpp)
- [基础实现](lab_final/problem/src/image_algorithms.cpp)
- [优化实现](lab_final/problem/src/image_algorithms_optimize.cpp)
- [性能测试程序](lab_final/problem/benchmarks/image_benchmarks.cpp)
- [CMake 配置](lab_final/problem/CMakeLists.txt)

该项目围绕图像算法的基础实现和优化实现展开，报告中包含正确性验证、优化方法、运行时间、CPU 时间和吞吐量等结果。
