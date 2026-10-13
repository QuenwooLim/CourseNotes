# 并行程序设计与算法实验

## 提交格式说明

按照实验报告模板填写报告，需要提供源代码及代码描述至智算习堂。实验报告模板使用PDF格式，命名方式为"并行程序设计_学号_姓名"。如有疑问，请发送邮件至 **yewu@mail2.sysu.edu.cn** 叶武询问细节。

---

## 任务1：直接卷积（滑窗法）

### 任务描述

在信号处理、图像处理和其他工程/科学领域，卷积是一种使用广泛的技术。在深度学习领域，卷积神经网络（CNN）这种模型架构就得名于这种技术。

在本实验中，我们将在GPU上实现卷积操作。注意这里的卷积是指神经网络中的卷积操作，与信号处理领域中的卷积操作不同，它**不需要对Filter进行翻转**，**不考虑bias**。

### 实现要求

- **输入规模**：从256×256增加至4096×4096，或者从32×32增加至512×512
- **输入参数**：Input matrix size 和 Kernel size（例如：32 和 3）

### 问题描述

用直接卷积的方式对Input进行卷积：

- **维度**：只实现2D（height × width）
- **通道数**：channel(depth)设置为3
- **卷积核**：Kernel (Filter)大小设置为3×3
- **卷积核通道**：kernel channel(depth)设置为3
- **步幅**：分别设置为1、2、3
- **填充**：可能需要通过padding配合stride完成CNN操作
- **偏置**：不考虑bias，bias设置为0

### 输出要求

- 输出卷积结果
- 输出计算时间

---

## 任务2：im2col方法结合GEMM

### 任务描述

使用im2col方法结合上次实验实现的GEMM实现卷积操作。

### 实现要求

- **输入规模**：从256×256增加至4096×4096，或者从32×32增加至512×512
- **输入参数**：Input matrix size 和 Kernel size（例如：32 和 3）

### 问题描述

用im2col方法对Input进行卷积：

- **维度**：只实现2D（height × width）
- **通道数**：channel(depth)设置为3
- **卷积核**：Kernel (Filter)大小设置为3×3
- **卷积核通道**：kernel channel(depth)设置为3
- **步幅**：分别设置为1、2、3
- **填充**：可能需要通过padding配合stride完成CNN操作
- **偏置**：不考虑bias，bias设置为0

### 输出要求

- 输出卷积结果
- 输出计算时间

---

## 任务3：使用cuDNN库

### 任务描述

NVIDIA cuDNN是用于深度神经网络的GPU加速库。它强调性能、易用性和低内存开销。

### 实现要求

使用cuDNN提供的卷积方法进行卷积操作，记录其相应Input的卷积时间，与自己实现的卷积操作进行比较。

### 分析要求

如果性能不如cuDNN，用文字描述可能的改进方法。

---

## 卷积神经网络（CNN）操作说明

### 1. CNN Input Image 举例

（此处为图片描述：展示CNN输入图像的示例）

### 2. CNN Input Image 和 Kernel (Filter)

（此处为图片描述：展示输入图像和卷积核的关系）

### 3. CNN 操作过程

（此处为图片描述：展示卷积操作的详细过程）

### 4. CNN 步幅（stride）和填充（padding）

（此处为图片描述：展示stride和padding对卷积结果的影响）

---

## 参考资料

### 主要参考资料

**斯坦福人工智能课件**：Convolutional Neural Networks  
作者：Fei-Fei Li & Andrej Karpathy & Justin Johnson

### 其他参考资料（搜索以下关键词）

1. 如何理解卷积神经网络（CNN）中的卷积和池化
2. [Convolutional Neural Networks (CNNs / ConvNets)](https://cs231n.github.io/convolutional-networks/)
3. im2col的原理和实现
4. cuDNN安装教程
5. [convolutional-neural-networks](https://stanford.edu/~shervine/teaching/cs-230/cheatsheet-convolutional-neural-networks)

---

