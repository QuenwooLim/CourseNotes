# Nsight Systems 深度学习性能分析指南

**作者：Daniel Horowitz**  
**职位：Engineering Platform Developer Tools Director**  
**日期：2022年6月30日**

---

## 目录

- [一、监控工具与性能分析工具的区别](#一监控工具与性能分析工具的区别)
- [二、单节点性能分析](#二单节点性能分析)
  - [2.1 概述](#21-概述)
  - [2.2 基本功能](#22-基本功能)
  - [2.3 GPU调查](#23-gpu调查)
  - [2.4 CPU调查](#24-cpu调查)
  - [2.5 常见问题修复策略](#25-常见问题修复策略)
- [三、多节点性能分析](#三多节点性能分析)
- [四、性能分析工作流程](#四性能分析工作流程)
- [五、Nsight工具家族](#五nsight工具家族)
- [六、Nsight Systems核心功能](#六nsight-systems核心功能)
- [七、时间线功能详解](#七时间线功能详解)
- [八、深度学习框架集成](#八深度学习框架集成)
- [九、命令行使用](#九命令行使用)
- [十、GPU性能指标采样](#十gpu性能指标采样)
- [十一、优化策略](#十一优化策略)
- [十二、CPU与操作系统分析](#十二cpu与操作系统分析)
- [十三、集群环境下的使用](#十三集群环境下的使用)
- [十四、其他相关工具](#十四其他相关工具)

---

## 一、监控工具与性能分析工具的区别

### 1.1 设计目标与用户群体

| 特性 | 监控工具 (Monitors) | 性能分析工具 (Profilers) |
|------|-------------------|------------------------|
| **目标** | 粗略观察质量、利用率、进度 | 协助程序优化 |
| **用户** | 机器和集群管理员、任务管理用户 | 工程师（需要关联到代码区域） |
| **报告频率** | 低频（~10-1000次/秒） | 高频，详细追踪 |
| **开销** | 低开销，适合持续运行 | 较高开销，不适合100%运行时 |
| **问题追踪** | 可能追踪到作业级别（分钟/秒级） | 可关联到具体代码行 |
| **示例** | DCGM | Nsight Systems, Nsight Compute |

### 1.2 选择合适的工具
- **不要期望一个工具能完全替代另一个**
- **根据具体需求选择正确的工具**
- **存在混合型工具，但通常仍有权衡**

---

## 二、单节点性能分析

### 2.1 概述

性能分析的迭代流程：
```
分析 → 优化 → 性能分析 → 优化 → ... 直到达到期望性能
```

### 2.2 基本功能

#### 2.2.1 系统级应用算法调优
- 定位优化机会
- 在快速GUI时间线上可视化数百万事件
- 查看未使用的CPU和GPU时间间隙
- 在多个GPU、CPU、NIC之间平衡工作负载

#### 2.2.2 支持的追踪范围
- **GPU**：流、内核、内存传输等
- **CPU**：算法、利用率、线程状态
- **用户体验**：CLI、GUI时间线、统计、数据导出

#### 2.2.3 跨平台支持
- Linux & Windows
- x86-64, ARM服务器, Tegra
- Mac（仅主机端）

### 2.3 GPU调查

#### 2.3.1 支持的追踪类型
- GPU硬件指标采样
- CUDA GPU端内核和内存操作范围（与CPU API调用关联）
- 库追踪：cuBLAS, cuDNN, cuDF, TensorRT

#### 2.3.2 时间线可视化
- 放大后类似频谱图的视图
- 从图形转换为范围显示
- 显示GPU和CPU活动

### 2.4 CPU调查

#### 2.4.1 支持的追踪类型
- 线程状态、迁移和调用栈
- OS运行时长调用追踪（>1微秒，pthread, glibc → mmap, file & IO等）
- 调用栈回溯（>80微秒）
- ftrace或WDDM & ETW（页面错误、信号、中断等）

### 2.5 常见问题修复策略

#### 2.5.1 核心调查策略
- **什么是热的（HOT）？**
  - 是否更容易缩小现有内容？
  - 大多数人集中在此（直观但不一定更好）
  
- **什么是冷的（COLD）？**
  - 是否更容易利用未使用的内容？
  - 免费的机会？是的请！

- **是否按预期和预算执行？**
  - 提示：通常不如你想象的那么好

#### 2.5.2 冷点分析
- 冷点通常是清晰、可衡量的机会
- 如何移除或填充它们？
- 哪里存在不正确/不必要/意外的依赖/同步？

#### 2.5.3 热点分析
- 可能可以并行化？
- 可能无法在不影响准确性、内存等的情况下缩小

---

## 三、多节点性能分析

### 3.1 单节点问题（已完成）
### 3.2 多节点新挑战
- 与集群作业/任务调度器协作
- 多报告视图
- 如何从数千个报告中选择？
- 墙钟时间同步
- 网络分析
- 数据分析

---

## 四、性能分析工作流程

### 4.1 分析师驱动的优化流程
```
1. 分析（Profile）
   ↓
2. 分析（Analyze）
   ↓
3. 优化（Optimize）
   ↓
4. 迭代直到达到期望性能
```

### 4.2 为什么从Nsight Systems开始？
- **看到全局视图**
- **查看异步CPU、GPU、NIC和软件如何交互**
- **了解谁在等待谁**
- **测量高级别成本以选择最佳机会**
- **避免基于直觉和假阳性指标的工作**
- **不要假设GPU受限而跳过其他工具**
- **某些同步导向的问题可能看起来像GPU受限**
- **仅统计数据通常不足以理解和解决问题**

---

## 五、Nsight工具家族

### 5.1 工具分类

| 工具 | 功能 | 应用场景 |
|------|------|---------|
| **Nsight Systems** | 系统级应用算法分析（CPU, GPU, CUDA, Graphics） | 系统级性能分析 |
| **Nsight Compute** | CUDA内核调试/优化 | 内核级性能分析 |
| **Nsight Graphics** | Graphics着色器和帧调试/优化 | 图形应用优化 |

---

## 六、Nsight Systems核心功能

### 6.1 时间线可视化
- 放大后类似频谱图的视图（例如：DL模型训练，2次迭代，GPU & CPU）
- 放大后将许多行图转换为范围

### 6.2 关键功能

#### 6.2.1 追踪范围
- **GPU**：GPU硬件指标采样、CUDA GPU端内核和内存操作
- **库**：cuBLAS, cuDNN, cuDF, TensorRT
- **CPU/OS/应用**：线程状态、迁移、调用栈、OS运行时长调用追踪
- **网络**：UCX, MPI, OpenSHMEM, NVSHMEM, NCCL追踪
- **NVIDIA NIC/HCA指标采样**：Infiniband & Ethernet

#### 6.2.2 代码注释API
- **NVTX**（NVIDIA Tools eXtensions）

---

## 七、时间线功能详解

### 7.1 时间线组件

```
进程和线程
├── CPU调用栈采样
├── GPU CUDA内核和内存传输活动
├── NVTX注释
├── CUDA API追踪
├── OpenGL追踪
└── 线程活动
    ├── 线程CPU核心
    └── 线程状态
```

### 7.2 CPU-GPU关联
- API启动到GPU内核的关联
- 在标尺中高亮显示关联
- CPU-GPU关联和位置辅助

### 7.3 NVTX示例
- **Visual Molecular Dynamics (VMD)** 算法在CPU上使用NVTX可视化
- NVTX范围从CPU投影到GPU CUDA流

---

## 八、深度学习框架集成

### 8.1 PyTorch

#### 8.1.1 NVTX范围
- DNN层注释默认禁用
- 添加以下代码启用：
  ```python
  with torch.autograd.profiler.emit_nvtx():
      # 你的代码
  ```

#### 8.1.2 TensorRT后端
- 如果使用TensorRT作为后端，已自动注释

#### 8.1.3 手动添加NVTX
```python
# 参考：
# https://github.com/pytorch/pytorch/blob/master/torch/cuda/nvtx.py
# https://pytorch.org/docs/stable/generated/torch.cuda.nvtx.range_push.html
# https://pytorch.org/docs/stable/generated/torch.cuda.nvtx.range_pop.html
```

### 8.2 TensorFlow

#### 8.2.1 NVTX注释
- 在NVIDIA TF容器中，DNN层默认使用NVTX注释
- 容器地址：https://catalog.ngc.nvidia.com/orgs/nvidia/containers/tensorflow

#### 8.2.2 禁用NVTX
```bash
TF_DISABLE_NVTX_RANGES=1  # 生产环境禁用
```

#### 8.2.3 添加更多细节
- 为setup、eager模式、tf.data.Dataset.from_generator等添加详细信息
- 参考：https://github.com/NVIDIA/NVTX/tree/dev/python

### 8.3 其他框架追踪
- **NVSHMEM & NCCL** 追踪
- **NVIDIA DALI** 追踪（数据加载库）

---

## 九、命令行使用

### 9.1 基本命令示例

```bash
nsys profile \
    --trace=cuda,nvtx,osrt,cudnn,cublas \
    --backtrace=dwarf \
    --capture-range=cudaProfilerApi \
    --gpu-metrics-devices=all \
    --output=oft-profile-dwarf4 \
    sh scripts/expt-singleGPU.sh --profile 50 --profile_start 5000 --profile_epoch 1
```

### 9.2 参数说明

| 参数 | 说明 |
|------|------|
| `--trace` | 选择要追踪的API（cuda, nvtx, osrt, cudnn, cublas等） |
| `--gpu-metrics-device` | 启用GPU内存使用追踪（有额外开销） |
| `--backtrace` | 通过DWARF信息收集线程调用栈回溯样本（更深但收集成本更高） |
| `--capture-range` | 在应用程序中的cudaProfilerStart API上触发收集 |
| `--gpu-metrics-devices` | GPU指标采样（默认10kHz） |
| `--output` | 命名报告文件 |
| `应用命令` | 应用程序命令及配置文件参数 |

### 9.3 多节点/多容器环境

```bash
# 多节点或多容器必需
srun nsys profile ...

# 单节点可选（生成单个报告）
nsys profile mpirun ...
```

### 9.4 完整文档
https://docs.nvidia.com/nsight-systems/UserGuide/index.html#cli-profile-command-switch-options

---

## 十、GPU性能指标采样

### 10.1 解读指标

#### 10.1.1 优势
- 更多信息，无追踪开销，设备级OOP收集
- **但没有内核名称**

#### 10.1.2 关键指标

| 指标 | 说明 | 优化建议 |
|------|------|---------|
| **GR Active** | GPU是否在执行某些工作 | % GPU有任何SM活动 |
| **SM Active** | GPU宽度利用情况 | 如果低，增加批次大小或查看内核网格维度 |
| **SM Instructions Issued** | 是否执行大量指令 | 如果低，可能在等待内存；增加内核块维度 |
| **SM Instructions Tensor Active** | 使用更快的专用硬件 | 性能增益，但SM指令可能下降（架构相关） |
| **Warp Occupancy** | SM代码类型比率 | **不要为此优化！** 超优化内核不一定最大化warp |

### 10.2 注意事项
- **注意1**：需要禁用DCGM和任何DL框架内置性能分析器
- **注意2**：对于低权限访问：https://developer.nvidia.com/ERR_NVGPUCTRPERM

### 10.3 示例分析
- TensorFlow2 ResNet50
- Mask-RCNN
- NCCL使用NVLink

---

## 十一、优化策略

### 11.1 通用优化策略（1）

#### 11.1.1 确保使用Tensor Core
- 使用正确的格式避免转换/转置

#### 11.1.2 增加批次/网格大小
- 更高效地使用GPU宽度

#### 11.1.3 常规并行化
- 增加框架的工作线程（CLI参数）
- Python：async, await, future, tasks, Dask等
- C/C++：OpenMP, OpenACC, pthreads, boost::asio, std::async等

#### 11.1.4 并行流水线
- 阶段之间是否存在数据依赖？没有？并行化！
- 示例：在当前批次/迭代执行时预取下一批次/迭代数据
- 示例：在加载器内：网络传输、映射、解析、加载和转换、上传到GPU

#### 11.1.5 重排序
- 能否更早执行？
- 预取：并行加载数据，使其在需要前就绪

#### 11.1.6 融合小内核
- 融合小内核、复制或memset
- 或使用cudaGraphs

#### 11.1.7 重叠训练（或推理）
- 使用CUDA MPS共享上下文，避免切换开销
- 可能难以将两者都放入内存

### 11.2 内存优化策略（2）

#### 11.2.1 通过指针传递缓冲区
- 避免复制

#### 11.2.2 多缓冲
- 不要让所有人都等待同一块内存
- 通常称为双缓冲或三缓冲；考虑交换模式

#### 11.2.3 避免返回数据到CPU

#### 11.2.4 避免CPU可分页内存
- 优先使用固定/锁定内存

#### 11.2.5 避免不必要的同步
- 避免cuda*Synchronize函数，使用cudaStreamWaitEvent代替
- 避免同步内存操作
- 避免CUDA默认流（如果使用多流）

#### 11.2.6 预分配内存
- 或使用回收策略

#### 11.2.7 最小化CUDA托管内存页面错误
- 在CPU和GPU上使用预取

### 11.3 实际案例

#### 11.3.1 AWS博客案例
**Deepset实现3.9倍加速和12.8倍成本降低**
- 链接：https://aws.amazon.com/blogs/machine-learning/deepset-achieves-a-3-9x-speedup-and-12-8x-cost-reduction-for-training-nlp-models-by-working-with-aws-and-nvidia/

**关键优化**：
- 大量使用Nsight Systems
- 从torch.nn.DataParallel (DP)切换到DistributedDataParallel (DDP)
- 通过切换到自动混合精度(AMP)启用更大的批次大小
- 引入StreamingDataSilo和DALI预取数据

### 11.4 常见问题示例

#### 11.4.1 融合机会
- CPU启动成本 + 小GPU工作量 ≈ GPU稀疏空闲
- 这适用于DNN节点/层

#### 11.4.2 cudaMemcpyAsync表现同步
- 设备到主机可分页内存
- 缓解方法：使用固定内存
- 时间对比：~150微秒（可分页） vs ~1.2微秒（固定）

#### 11.4.3 流同步导致的GPU空闲
- 示例：流同步导致的GPU空闲

---

## 十二、CPU与操作系统分析

### 12.1 权限要求

某些功能与OS权限绑定：

#### 12.1.1 CPU线程状态、核心占用、用户空间调用栈周期采样
- Paranoid级别太高
- 容器SECCOMP阻止perf_event_open
- OS内核样本需要更低的paranoid级别和/或sudo

#### 12.1.2 ftrace
- 需要CAP_SYS_ADMIN | CAP_SYS_PERFMON

**文档参考**：
- https://docs.nvidia.com/nsight-systems/InstallationGuide/index.html#linux-requirements
- https://docs.nvidia.com/nsight-systems/UserGuide/index.html#docker-profiling

### 12.2 OS运行时API追踪

#### 12.2.1 示例：Mask-RCNN
- Map/unmap卡顿
- 通过流水线缓解：
  - 提前映射1个批次
  - 取消映射最后批次
  - 在此处交换指针

#### 12.2.2 包含长运行函数的回溯

### 12.3 ftrace示例
- 展示中断

### 12.4 函数表
- 显示周期调用栈回溯的统计信息

---

## 十三、集群环境下的使用

### 13.1 与集群作业调度器协作

#### 13.1.1 基本命令
```bash
srun <slurm_args> nsys profile <nsys_args> app <app_args>
```

#### 13.1.2 确保报告名称唯一
```bash
--output=friendlyName_%q{SLURM_NODEID}_%q{SLURM_PROCID}.nsys-rep
```

#### 13.1.3 避免从所有rank获取系统级数据
将nsys和app移动到shell脚本中：
```bash
IF [$SLURM_LOCALID == 0] THEN
    # 添加 --nic-metrics=true
    # 添加 --gpu-metrics-device=all
```

#### 13.1.4 限制记录时间
- 否则在笔记本电脑上打开报告时会耗尽内存
- 30秒-1分钟通常足够，甚至更少...捕获几次迭代

### 13.2 多报告视图

#### 13.2.1 加载多个报告到一个时间线
- **2022.1及之前**：通过文件菜单
- **2022.2及之后**：通过打开对话框，选择多个报告
- **注意**：选择报告子集，否则可能耗尽RAM

### 13.3 墙钟时间同步

#### 13.3.1 重要性
- 时间线需要墙钟时间，特别是要准确拟合在一起时
- 数据独立收集
- 依赖系统墙钟时间的准确性
- 否则会出现时间线漂移

#### 13.3.2 准确度从差到好
| 同步方式 | 准确度 |
|---------|--------|
| NTP | 1毫秒 |
| PTP软件 | 10-100微秒 |
| PTP硬件 | 可达~10纳秒 |

### 13.4 数据分析

#### 13.4.1 不要手动查看所有文件
- 太多了！
- 可能随机混合多报告视图？

#### 13.4.2 使用数据分析缩小范围
- 集群迭代时间全局总时间
- 每个rank迭代数据加载时间
- 每个rank迭代处理时间
  - 达到all-reduce的总时间
  - 前向传播
  - 反向传播
- 每个rank迭代通信时间（如果未重叠）
- DNN层统计？

#### 13.4.3 可视化比较
- 比较平均、最小和最大报告，尝试理解差异原因

#### 13.4.4 可能需要添加NVTX
- 获取某些信息可能需要在应用中添加NVTX

#### 13.4.5 参考资源
- GTC演讲：Scaling Transformer in PyTorch Across Multiple Nodes
- 链接：https://developer.nvidia.com/gtc/2020/video/s21351

### 13.5 数据处理不平衡

#### 13.5.1 确保每个rank在迭代中一致

#### 13.5.2 加载时间
- 这个批次是否比正常加载时间更长？
- 如果并行，是否有任何rank需要等待？

#### 13.5.3 处理时间
- 这个批次是否比正常处理时间更长？

#### 13.5.4 "快点等"问题
- 如果有人花更长时间到达all-reduce，所有人都会被卡住！

#### 13.5.5 补救措施
- **修复数据**
  - PNG世界中的孤独JPEG，或WAV中的MP3？
  - 错误的分辨率、采样率、精度等
- **重新组织批次顺序**

---

## 十四、其他相关工具

### 14.1 Nsight Compute

#### 14.1.1 功能
- 交互式CUDA API调试器
- 高级CUDA内核性能分析
- CUDA-C/PTX/SASS关联
- 源代码关联性能指标
- 性能报告差异对比
- 可编程专家系统
- NVTX范围定义的内核性能分析
- 高性能GUI可视化和CLI数据收集

#### 14.1.2 系统要求
- Windows 10
- Linux Ubuntu 16.04/18.04/20.4
- RHEL 7.x

#### 14.1.3 CUDA内核性能分析器

**注意**：参见前面关于与Nsight Systems关系的幻灯片！从那里开始获取全局视图！

### 14.2 Nsight Deep Learning Designer

#### 14.2.1 功能
- 为具有严格性能要求的应用程序添加基于DL的图像处理功能的新工具
- 设计器、检查器、性能分析器用于图像和视频处理模型

#### 14.2.2 下载
https://developer.nvidia.com/nsight-dl-designer

#### 14.2.3 介绍视频
- 快速介绍（2.5分钟）：https://www.youtube.com/watch?v=7AraPM8dhyc
- 扩展介绍（35分钟）：https://www.nvidia.com/en-us/on-demand/session/siggraph2021-s21351/

### 14.3 Nsight Visual Studio Code Edition

#### 14.3.1 功能
- VSCode插件用于CUDA开发、编译和调试

#### 14.3.2 下载
https://developer.nvidia.com/nsight-visual-studio-code-edition

#### 14.3.3 介绍视频
- 快速介绍（4分钟）：https://www.youtube.com/watch?v=gN3XeFwZ4ng
- Microsoft介绍（14分钟）：https://www.youtube.com/watch?v=l6PgYhiQr-I

### 14.4 其他CUDA开发者工具

#### 14.4.1 CUDA-gdb
- 统一的CPU和CUDA调试
- CUDA-C/PTX/SASS支持

#### 14.4.2 Compute Sanitizer
- API和工具
- **memcheck**：报告越界/未对齐内存访问错误
- **racecheck**：识别__shared__内存上的竞争
- **initcheck**：使用未初始化的全局内存
- **synccheck**：识别__syncthreads()和__syncwarp()的无效使用

#### 14.4.3 命令行CUDA工具

---

## 十五、下载与资源

### 15.1 下载地址
https://developer.nvidia.com/nsight-systems

**注意**：网站版本比CUDA Toolkit更新

### 15.2 联系方式
- 分享你的成功案例！

---

**感谢使用Nsight Systems进行深度学习性能分析！**