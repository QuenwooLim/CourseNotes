# 计算机组成原理

本目录整理了计算机组成原理课程的理论课件、课程作业、FPGA/Vivado 实验、MIPS 汇编代码、实验报告、考试资料和参考资料。

## 📂 资料目录

| 内容 | 说明 |
|:---|:---|
| [理论课件](Theory/) | 按章节整理的课程课件和习题课资料 |
| [课程作业](Homework/) | 理论课程作业，提供 DOCX 和 PDF 文件 |
| [实验指导](Lab/) | 基于开发板、Verilog 和 MIPS 的实验指导材料 |
| [实验报告](Lab_Report/) | 各次实验的报告文件 |
| [Vivado 实验代码](Lab_Vivado/) | FPGA 工程、Verilog 源码和 Vivado 项目文件 |
| [MARS 代码](MARS_Code/) | MIPS 汇编实验代码 |
| [考试资料](Exam/) | 选择题、综合题和课程答案资料 |
| [参考资料](Reference_Source/) | 教材、指令集手册和 FPGA/MIPS 参考文档 |

## 📖 理论课件

[`Theory/`](Theory/) 中保存了课程概况以及第一章至第五章的课件，包含：

- 课程概况与计算机组成基础
- 指令系统与 MIPS 相关内容
- 算术运算、处理器和数据通路
- 存储器层次与相关设计
- 处理器实现及配套习题课

代表性资料包括：

- [课程概况](<Theory/ch1-1 课程概况.pdf>)
- [第一章课件](Theory/ch1-2.pdf)
- [第二章课件](Theory/ch2-1.pdf)
- [第四章课件](Theory/ch4-1.pdf)
- [第五章课件](Theory/ch5-1.pdf)
- [习题课](<Theory/习题课（第二章）.pdf>)

## ✍️ 课程作业与实验报告

[`Homework/`](Homework/) 中保存了第三次至第六次理论作业，同时提供可编辑的 DOCX 文件和导出的 PDF 文件。

[`Lab_Report/`](Lab_Report/) 中保存了多次实验报告，覆盖实验第二次至第十次，部分报告同时提供 DOCX 和 PDF 格式。

## 🧪 实验指导与代码

### FPGA、Verilog 与 MIPS 实验

[`Lab/`](Lab/) 中整理了实验课程的指导材料，包括：

- 开发板、烧录和实验环境说明
- 逻辑门、组合逻辑、多路选择器、七段译码和计数器
- FSM 状态机与 UART 串口实验
- MIPS 汇编程序、存储器、寄存器堆和 ALU
- 单周期处理器实验

代表性实验资料：

- [实验 3：串口 UART](<Lab/实验3-串口UART实验.pdf>)
- [实验 4：计算机结构与组成 MIPS 汇编程序](<Lab/实验4计算机结构与组成MIPS汇编程序.pdf>)
- [实验 7：寄存器堆设计与 ALU](<Lab/实验7-寄存器堆设计ALU实验.pdf>)
- [实验 9：单周期处理器](<Lab/实验9-单周期处理器实验.pdf>)
- [实验 10：单周期处理器应用](<Lab/实验10-单周期处理器应用实验(new)(1)(1).pdf>)

### Vivado 工程

[`Lab_Vivado/`](Lab_Vivado/) 中保存了多个 Vivado 工程，主题包括：

- 跑马灯、逻辑门和组合逻辑
- 多路选择器、补码转换、七段译码和计数器
- 伪随机码发生器与 FSM 状态机
- UART 数据收发、UART 寄存器和 UART ALU
- 单周期处理器

### MARS 汇编代码

[`MARS_Code/`](MARS_Code/) 中保存了 MIPS 汇编练习和实验代码，例如：

- [`mips1.asm`](MARS_Code/mips1.asm) 至 [`mips5.asm`](MARS_Code/mips5.asm)
- [`lab4_ex1.asm`](MARS_Code/lab4_ex1.asm) 至 [`lab4_ex3.asm`](MARS_Code/lab4_ex3.asm)
- [`swap.s`](MARS_Code/swap.s)

## 📝 考试资料

[`Exam/`](Exam/) 中包含计算机组成原理选择题、综合题和参考答案资料：

- [(带书签)计算机组成原理完整版答案](<Exam/(带书签)计算机组成原理完整版答案 .pdf>)
- [(紧凑版)选择题做题本](<Exam/(紧凑版)25·WD计算机组成原理选择题做题本 .pdf>)
- [[A4 版]综合题做题本](<Exam/[A4版]25WD-计算机组成原理综合题做题本.pdf>)

## 📚 参考资料

[`Reference_Source/`](Reference_Source/) 中整理了教材、MIPS 指令集、FPGA 调试和机器码相关资料，包括：

- [计算机组成与设计（原书第 6 版）](<Reference_Source/计算机组成与设计 硬件软件接口 MIPS版 原书第6版 (David A.Patterson, John Hennessy).pdf>)
- [MIPS 基准指令集手册](<Reference_Source/MIPS基准指令集手册_v1.00.pdf>)
- [指令及对应机器码](<Reference_Source/指令及对应机器码_2018.pdf>)
- [FPGA 在线调试教程](<Reference_Source/FPGA在线调试教程.pdf>)
- [其他组件实现](<Reference_Source/其他组件实现.pdf>)