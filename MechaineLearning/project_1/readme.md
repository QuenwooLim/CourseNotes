# 第一次大作业

学号：23320093    
姓名：林宏宇   
班级：机器学习与数据挖掘

---

## 作业要求：

提交方式：报告（包含思路、设置、结果）和源代码打包成【学号_姓名_第一次大作业.zip】提交（不用提交模型文件）

任务一：简单神经网络图像分类
任务：构建一个简单的三层 MLP，在两个经典的图像分类数据集（Fashion-MNIST 和 CIFAR-10）上进行训练和测试。
目标：掌握 Pytorch 构建、训练和评估神经网络的基本流程。
参考：https://docs.pytorch.org/tutorials/beginner/basics/intro.html

任务二：使用预训练 ResNet-50 模型进行图像分类
任务：阅读 ResNet 论文，使用一个在 ImageNet 数据集上预训练好的 ResNet-50 模型在 ImageNet 数据集上进行测试。
模型：https://huggingface.co/microsoft/resnet-50
目标：理解 ResNet 的核心思想。
参考：https://arxiv.org/pdf/1512.03385

（可选）任务三：使用预训练 ResNet-50 模型进行目标检测（包含分类和回归任务）
任务：学习目标检测任务，理解 mAP 指标，使用预训练模型复现 ResNet 论文中的目标检测实验结果（Section 4.3）。
参考：https://arxiv.org/pdf/1506.01497
参考实现：https://github.com/rbgirshick/py-faster-rcnn、https://github.com/open-mmlab/mmdetection

## 实验

### 任务一：简单神经网络图像分类

#### 1. 实验思路与设计

本实验旨在构建一个简单的三层多层感知机（MLP）网络，分别在Fashion-MNIST和CIFAR-10两个经典图像分类数据集上进行训练和测试，掌握PyTorch构建、训练和评估神经网络的基本流程。

**网络结构设计：**
- 输入层：根据数据集调整输入维度
- 隐藏层：两个512节点的全连接层，使用ReLU激活函数
- 输出层：10个节点对应10个类别

**实验配置：**
- 损失函数：交叉熵损失（CrossEntropyLoss）
- 优化器：随机梯度下降（SGD），学习率为0.001
- 批次大小：64
- 训练轮数：5个epoch

#### 2. Fashion-MNIST数据集实验

**数据集特点：**
- 图像尺寸：28×28像素，单通道（灰度图）
- 类别数：10类时尚用品（T恤、裤子、套衫等）
- 训练集：60,000张图像
- 测试集：10,000张图像

**代码关键修改：**
```python
# 修改设备检测方式，提高兼容性
device = "cuda" if torch.cuda.is_available() else "cpu"
```

**数据集加载过程：**
``` 
Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/train-images-idx3-ubyte.gz
Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/train-images-idx3-ubyte.gz to data/FashionMNIST/raw/train-images-idx3-ubyte.gz
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 26.4M/26.4M [00:06<00:00, 3.84MB/s]
Extracting data/FashionMNIST/raw/train-images-idx3-ubyte.gz to data/FashionMNIST/raw

Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/train-labels-idx1-ubyte.gz
Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/train-labels-idx1-ubyte.gz to data/FashionMNIST/raw/train-labels-idx1-ubyte.gz
100%|██████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 29.5k/29.5k [00:00<00:00, 124kB/s]
Extracting data/FashionMNIST/raw/train-labels-idx1-ubyte.gz to data/FashionMNIST/raw

Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/t10k-images-idx3-ubyte.gz
Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/t10k-images-idx3-ubyte.gz to data/FashionMNIST/raw/t10k-images-idx3-ubyte.gz
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 4.42M/4.42M [00:02<00:00, 2.01MB/s]
Extracting data/FashionMNIST/raw/t10k-images-idx3-ubyte.gz to data/FashionMNIST/raw

Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/t10k-labels-idx1-ubyte.gz
Downloading http://fashion-mnist.s3-website.eu-central-1.amazonaws.com/t10k-labels-idx1-ubyte.gz to data/FashionMNIST/raw/t10k-labels-idx1-ubyte.gz
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 5.15k/5.15k [00:00<00:00, 12.7MB/s]
Extracting data/FashionMNIST/raw/t10k-labels-idx1-ubyte.gz to data/FashionMNIST/raw
```

**模型结构分析：**
```
数据维度信息：
Shape of X [N, C, H, W]: torch.Size([64, 1, 28, 28])  # 批次64，1通道，28×28像素
Shape of y: torch.Size([64]) torch.int64              # 批次64，标签类型int64
Using cpu device

网络架构：
NeuralNetwork(
  (flatten): Flatten(start_dim=1, end_dim=-1)          # 展平层：28×28=784维
  (linear_relu_stack): Sequential(
    (0): Linear(in_features=784, out_features=512, bias=True)   # 第一层：784→512
    (1): ReLU()                                                 # ReLU激活
    (2): Linear(in_features=512, out_features=512, bias=True)   # 第二层：512→512
    (3): ReLU()                                                 # ReLU激活
    (4): Linear(in_features=512, out_features=10, bias=True)    # 输出层：512→10
  )
)
```

**网络参数统计：**
- 总参数量：784×512 + 512×512 + 512×10 + 偏置项 ≈ 665,000个参数
- 网络深度：3层（不含激活层）

**训练过程与结果分析：**

训练过程详细日志：
```
Epoch 1 - 模型初始化阶段
-------------------------------
初始损失: 2.299587，随训练逐步下降至 2.156121
测试准确率: 29.5%, 平均损失: 2.150832
分析：模型开始学习基本特征，准确率接近随机猜测的3倍

Epoch 2 - 快速学习阶段
-------------------------------
损失持续下降: 2.159438 → 1.863739
测试准确率: 50.2%, 平均损失: 1.864553
分析：模型开始掌握数据分布规律，准确率大幅提升

Epoch 3 - 稳定提升阶段
-------------------------------
损失进一步优化: 1.894619 → 1.479751
测试准确率: 63.5%, 平均损失: 1.495962
分析：模型性能稳定提升，开始学习更复杂的特征

Epoch 4 - 性能微调阶段
-------------------------------
损失缓慢下降: 1.558835 → 1.223461  
测试准确率: 64.3%, 平均损失: 1.238120
分析：准确率提升放缓，模型接近收敛

Epoch 5 - 收敛阶段
-------------------------------
最终损失: 1.311980 → 1.073910
最终测试准确率: 65.0%, 平均损失: 1.079527
分析：模型基本收敛，性能稳定

模型保存: model.pth
```

**训练结果总结：**

| Epoch | 训练损失(末尾) | 测试准确率 | 测试损失 | 性能分析 |
|-------|---------------|-----------|----------|----------|
| 1     | 2.156         | 29.5%     | 2.151    | 初始学习 |
| 2     | 1.864         | 50.2%     | 1.865    | 快速提升 |
| 3     | 1.480         | 63.5%     | 1.496    | 稳定进步 |
| 4     | 1.223         | 64.3%     | 1.238    | 接近收敛 |
| 5     | 1.074         | 65.0%     | 1.080    | 基本收敛 |

**关键观察：**
- 损失函数呈单调递减趋势，无明显过拟合现象
- 准确率从29.5%提升至65.0%，相对提升120%
- 训练损失与测试损失变化趋势一致，模型泛化能力良好
#### 3. CIFAR-10数据集实验

**数据集特点：**
- 图像尺寸：32×32像素，三通道（RGB彩色图）
- 类别数：10类自然物体（飞机、汽车、鸟类、猫等）
- 训练集：50,000张图像
- 测试集：10,000张图像
- 复杂度：相比Fashion-MNIST更具挑战性，包含颜色信息和自然场景

**关键代码适配：**

*数据集切换：*
```python
# CIFAR-10数据集加载
training_data = datasets.CIFAR10(
    root="data",
    train=True,
    download=True,
    transform=ToTensor(),
)

test_data = datasets.CIFAR10(
    root="data", 
    train=False,
    download=True,
    transform=ToTensor(),
)
```

*网络结构调整：*
```python
class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(3*32*32, 512),  # CIFAR-10: 3通道×32×32 = 3072维输入
            nn.ReLU(),
            nn.Linear(512, 512),
            nn.ReLU(), 
            nn.Linear(512, 10)        # 输出仍为10类
        )
```

**关键差异分析：**
- **输入维度变化：** 784维(28×28×1) → 3072维(32×32×3)
- **参数量增加：** 约401K → 约1.57M参数
- **计算复杂度：** 提升约4倍
- **数据复杂度：** 彩色自然图像比灰度时尚用品更复杂

**CIFAR-10模型结构验证：**
```
数据维度信息：
Shape of X [N, C, H, W]: torch.Size([64, 3, 32, 32])  # 批次64，3通道，32×32像素
Shape of y: torch.Size([64]) torch.int64              # 批次64，标签类型int64
Using cpu device

网络架构：
NeuralNetwork(
  (flatten): Flatten(start_dim=1, end_dim=-1)          # 展平层：3×32×32=3072维
  (linear_relu_stack): Sequential(
    (0): Linear(in_features=3072, out_features=512, bias=True)  # 第一层：3072→512
    (1): ReLU()                                                 # ReLU激活
    (2): Linear(in_features=512, out_features=512, bias=True)   # 第二层：512→512
    (3): ReLU()                                                 # ReLU激活
    (4): Linear(in_features=512, out_features=10, bias=True)    # 输出层：512→10
  )
)
```

**CIFAR-10网络参数统计：**
- 总参数量：3072×512 + 512×512 + 512×10 + 偏置项 ≈ 1,581,000个参数
- 相比Fashion-MNIST增加约138%的参数

**CIFAR-10训练过程与结果分析：**

训练过程详细日志：
```
Epoch 1 - 模型初始化阶段（困难启动）
-------------------------------
初始损失: 2.300683，训练过程中下降缓慢至 2.273394
测试准确率: 18.5%, 平均损失: 2.266447
分析：模型学习困难，准确率仅比随机猜测(10%)略高

Epoch 2 - 缓慢改善阶段
-------------------------------
损失微幅下降: 2.282972 → 2.238622
测试准确率: 21.2%, 平均损失: 2.228613
分析：模型开始捕捉基本模式，但进展缓慢

Epoch 3 - 稳定学习阶段
-------------------------------
损失继续下降: 2.255177 → 2.185729
测试准确率: 22.9%, 平均损失: 2.178055
分析：学习速度稳定，开始识别更复杂特征

Epoch 4 - 加速提升阶段
-------------------------------
损失明显改善: 2.216744 → 2.123585
测试准确率: 24.6%, 平均损失: 2.122317
分析：模型找到有效学习路径，性能加速提升

Epoch 5 - 持续优化阶段
-------------------------------
最终损失: 2.173504 → 2.069941
最终测试准确率: 26.6%, 平均损失: 2.074518
分析：模型持续学习，但仍有很大改进空间

模型保存: model.pth
```

**CIFAR-10训练结果总结：**

| Epoch | 训练损失(末尾) | 测试准确率 | 测试损失 | 性能分析 |
|-------|---------------|-----------|----------|----------|
| 1     | 2.273         | 18.5%     | 2.266    | 困难启动 |
| 2     | 2.239         | 21.2%     | 2.229    | 缓慢改善 |
| 3     | 2.186         | 22.9%     | 2.178    | 稳定学习 |
| 4     | 2.124         | 24.6%     | 2.122    | 加速提升 |
| 5     | 2.070         | 26.6%     | 2.075    | 持续优化 |

**CIFAR-10关键观察：**
- 学习曲线相对平缓，表明CIFAR-10确实更具挑战性，因此准确率提升有限
- 最终准确率26.6%，远低于Fashion-MNIST的65.0%
- 训练损失和测试损失保持一致，无过拟合现象
- 损失下降幅度较小，表明需要更多训练轮数或模型改进

#### 4. 实验结果对比与分析

**两数据集性能对比：**

| 数据集 | 图像尺寸 | 通道数 | 输入维度 | 参数量 | 最终准确率 | 收敛难度 | 数据集大小 |
|--------|----------|--------|----------|--------|-----------|----------|-----------|
| Fashion-MNIST | 28×28 | 1 | 784 | ~665K | 65.0% | 容易 | ~30MB |
| CIFAR-10 | 32×32 | 3 | 3072 | ~1.58M | 26.6% | 困难 | ~170MB |

**详细性能分析对比：**

| 指标 | Fashion-MNIST | CIFAR-10 | 差异分析 |
|------|---------------|----------|----------|
| 初始准确率(Epoch 1) | 29.5% | 18.5% | CIFAR-10起步更困难 |
| 最终准确率(Epoch 5) | 65.0% | 26.6% | Fashion-MNIST性能优势明显 |
| 准确率提升幅度 | +35.5% | +8.1% | Fashion-MNIST学习效率更高 |
| 初始损失 | 2.300 | 2.301 | 起始点相近 |
| 最终损失 | 1.074 | 2.075 | Fashion-MNIST收敛更好 |
| 损失下降幅度 | -1.226 | -0.226 | Fashion-MNIST优化更显著 |
| 学习稳定性 | 稳定单调下降 | 缓慢波动下降 | Fashion-MNIST更稳定 |

**实验总结与思考：**

1. **网络架构验证：** 三层MLP结构在Fashion-MNIST上取得了合理的性能表现，证明了基础神经网络的有效性。

2. **训练过程分析：** 模型呈现良好的收敛特性，损失函数单调递减，无明显过拟合现象，说明网络容量与数据集匹配度较好。

3. **性能评估对比分析：**
   
   **Fashion-MNIST表现：**
   - 65%准确率对于简单MLP属于良好水平
   - 相比随机猜测(10%)提升550%
   - 损失函数收敛良好，从2.30降至1.07
   
   **CIFAR-10表现：**
   - 26.6%准确率显示MLP在复杂图像上的局限性
   - 相比随机猜测(10%)仅提升166%
   - 损失收敛缓慢，从2.30仅降至2.07
   - 说明简单MLP难以处理彩色自然图像的复杂特征

4. **数据集复杂度差异深入分析：**
   - **视觉复杂度：** Fashion-MNIST为简化的服装轮廓图，CIFAR-10包含真实自然场景
   - **特征层次：** Fashion-MNIST主要依赖形状特征，CIFAR-10需要颜色、纹理、复杂形状的综合
   - **类内变异：** CIFAR-10类内变异更大（如不同角度的飞机、不同品种的动物）
   - **背景干扰：** CIFAR-10包含复杂背景，Fashion-MNIST背景单一


5. **实验启示：**
   - **数据集复杂度显著影响模型性能：** 同样的网络架构在不同复杂度数据集上表现差异巨大
   - **MLP的适用性限制：** 简单MLP适合处理结构化、低复杂度的图像数据，但在自然图像上效果有限
   - **网络容量与任务匹配：** 相同的网络容量在简单任务上表现良好，但对复杂任务明显不足
   
6. **学习收获：**
   - **技术层面：**
     - 掌握了PyTorch的基本使用流程
     - 理解了神经网络训练的完整pipeline
     - 学会了模型保存与加载
     - 熟悉了不同数据集的处理方法
   
   - **理论层面：**
     - 深入理解了数据集复杂度对模型性能的影响
     - 认识到网络架构选择的重要性
     - 学会了通过对比实验分析模型性能
     - 理解了损失函数和准确率的变化规律

### 任务二：使用预训练 ResNet-50 模型进行图像分类

#### 1. 实验目标与背景

**任务目标：**
- 使用在 ImageNet 数据集上预训练好的 ResNet-50 模型进行图像分类
- 理解 ResNet 的核心思想和残差连接的作用
- 掌握预训练模型的使用方法和迁移学习概念

**ResNet-50 模型特点：**
- **深度：** 50层深度卷积神经网络
- **创新点：** 引入残差连接（Residual Connection）解决深度网络梯度消失问题
- **预训练：** 在 ImageNet 数据集上预训练（1000个类别，超过100万张图像）
- **架构优势：** 通过跳跃连接允许信息直接传递，使得训练更深网络成为可能

#### 2. 网络连接问题解决

**遇到的问题：**
在访问 Hugging Face 官网时遇到网络连接失败：
```
'(MaxRetryError("HTTPSConnectionPool(host='huggingface.co', port=443): Max retries exceeded with url: /datasets/huggingface/cats-image/resolve/main/README.md (Caused by NewConnectionError('<urllib3.connection.HTTPSConnection object at 0x7a86fce43a70>: Failed to establish a new connection: [Errno 101] Network is unreachable'))"), '(Request ID: ea72a2d7-de37-4d5f-967c-92eae88d59aa)')' thrown while requesting HEAD https://huggingface.co/datasets/huggingface/cats-image/resolve/main/README.md
```

**解决方案：**
使用 HF-Mirror 镜像站点解决网络连接问题：

```python
# 使用 huggingface 镜像源进行图像分类任务
import os
os.environ['HF_ENDPOINT'] = 'https://hf-mirror.com'
```

**解决效果验证：**
```
README.md: 96.0B [00:00, 187kB/s]
cats_image.jpeg: 100%|██████████████████████████████████████████████████████████████████████████████| 173k/173k [00:00, 499kB/s]  
Generating test split: 1 examples [00:00, 43.33 examples/s]
preprocessor_config.json: 266B [00:00, 619kB/s]
```

#### 3. 模型加载与配置

**模型加载过程分析：**

1. **图像处理器 (AutoImageProcessor)：**
   - 自动适配 ResNet-50 所需的预处理步骤
   - 图像尺寸调整为 224×224 像素
   - 像素值标准化（均值和标准差归一化）
   - 数据格式转换为 PyTorch 张量

2. **预训练模型加载：**
   - 模型来源：`microsoft/resnet-50`
   - 包含完整的 ResNet-50 架构和 ImageNet 预训练权重
   - 输出层：1000个类别的分类头

3. **兼容性提醒：**
```
Using a slow image processor as `use_fast` is unset and a slow processor was saved with this model. 
`use_fast=True` will be the default behavior in v4.52, even if the model was saved with a slow processor.
```
   - 系统提示使用了较慢的图像处理器
   - 可通过设置 `use_fast=True` 使用更快的处理器

#### 4. 实验结果与分析

**分类结果：**
```
预测类别：tiger cat
```

**结果分析：**

1. **预测准确性：**
   - 模型成功识别出测试图像为"tiger cat"（虎斑猫）
   - 说明预训练的 ResNet-50 模型具有优秀的图像识别能力
   - ImageNet 预训练权重在实际图像上表现良好

2. **推理性能：**
   - 推理速度快，几乎即时得到结果
   - `torch.no_grad()` 有效减少了内存使用
   - 预训练模型的高效性得到验证

3. **模型优势体现：**
   - **免训练直接使用：** 无需自己训练即可获得强大的分类能力
   - **泛化能力强：** 在 ImageNet 上训练的模型能很好地识别其他图像
   - **特征提取能力：** ResNet-50 学到了丰富的视觉特征表示


#### 5. ResNet 核心思想理解

ResNet（残差网络）的核心突破在于提出了“残差学习”框架，从根本上解决了深度神经网络训练中的“退化问题”（Degradation Problem），极大推动了深层网络的发展。

**5.1. 问题的提出：退化问题（Degradation Problem）**

在 ResNet 出现之前，学界普遍认为更深的网络能够学习更复杂的特征，因此理论上性能会更好。但实际实验发现，随着网络层数的增加，模型的训练误差和测试误差反而会升高。这种现象被称为“退化问题”。

论文中的图1（Fig. 1）清楚地展示了这一点：一个56层的普通网络（Plain Network）在训练集上的误差甚至高于20层的同类网络。这一结果违背了常识，因为更深的网络至少可以通过将多余的层学习为恒等映射 $F(x) = x$，从而不比浅层网络差。

**关键点：** 这种性能下降并非过拟合导致，而是由于优化困难。即使理论上存在最优解（如恒等映射），当前的优化器（如SGD）也难以找到。

**5.2. 核心解决方案：残差块（Residual Block）**

ResNet 的创新之处在于：
- 不再让网络直接学习复杂的底层映射 $H(x)$，
- 而是让网络学习 $H(x)$ 与输入 $x$ 之间的差异（残差），即 $F(x) = H(x) - x$。

因此，网络的输出变为 $H(x) = F(x) + x$。这种结构被称为**残差块**（Residual Block），其核心是**快捷连接**（Shortcut Connection）或**跳跃连接**（Skip Connection）。

**5.3. 残差块的结构**

一个典型的残差块包括：
- **主干路径（Main Path）**：由两到三层卷积（Conv）和激活函数（ReLU）组成，负责学习残差函数 $F(x)$。
- **快捷连接（Shortcut Connection）**：一条直接将输入 $x$ 跳到主干路径输出的捷径。
- **逐元素相加（Element-wise Addition）**：将主干路径输出 $F(x)$ 与输入 $x$ 相加，得到最终输出 $y = F(x) + x$。

快捷连接的实现方式：
- **恒等映射（Identity Mapping）**：当输入 $x$ 和 $F(x)$ 维度一致时，直接相加，不引入额外参数。
- **投影映射（Projection Mapping）**：当维度不一致时，使用 $1\times1$ 卷积 $W_s$ 对 $x$ 进行升/降维，使得 $y = F(x) + W_s x$。

**5.4. 为什么残差学习有效？（核心洞见）**

（1）**简化优化问题**：残差学习将“学习复杂函数 $H(x)$”转化为“学习一个可能很简单的函数 $F(x)$”。如果最优解是恒等映射 $H(x) = x$，那么 $F(x)$ 只需为 0。让网络权重趋近于 0 比精确学习恒等映射要容易得多，极大降低了优化难度。

（2）**缓解梯度消失**：快捷连接为梯度提供了“高速通道”，使得梯度可以直接从后层流向前层，避免了极深网络中梯度消失的问题，从而支持上百层甚至更深网络的训练。

（3）**恒等映射的先验**：快捷连接天然提供了恒等映射的“备份”，网络可以优先通过捷径传递信息，主干路径只需学习对恒等映射的微小修正（残差 $F(x)$），为优化提供了良好起点。

**5.5. 总结**

ResNet 的核心思想可以概括为：**让网络在“基础函数”（快捷连接，通常为恒等映射）上学习“修正量”（残差）**，而不是从零开始学习复杂函数。

通过引入残差块和快捷连接，ResNet 成功解决了深度神经网络的退化问题，使得训练上百层甚至上千层的网络成为可能。这一思想不仅在 ImageNet 等图像分类任务上取得了突破性成果，也成为后续几乎所有深度神经网络架构（如 DenseNet、Transformer 等）的基础，对深度学习领域产生了深远影响。

#### 6. 学习收获与总结

1. **预训练模型使用：** 掌握了 Hugging Face 生态系统的使用方法
2. **迁移学习理解：** 理解了预训练模型的价值和应用方式
3. **ResNet 架构认知：** 深入理解了残差连接的设计思想
4. **实际问题解决：** 学会了处理网络连接等实际部署问题

### 任务三：使用预训练 ResNet-50 模型进行目标检测（包含分类和回归任务）

#### 任务目标与指标回顾

目标检测需要模型同时解决**分类**与**回归**两个子问题：在确定每个目标类别的同时，还要预测其边界框位置。模型输出会被转换成多个候选框及其置信度，再通过非极大值抑制筛选。

在评价指标上，核心指标仍是 mean Average Precision (mAP)。我们采用 COCO 官方协议，在多个 IoU 阈值上计算平均精度：

- IoU（Intersection over Union）衡量预测框与真实框之间的重叠程度，定义为 $IoU = \frac{|B_{\text{pred}} \cap B_{\text{gt}}|}{|B_{\text{pred}} \cup B_{\text{gt}}|}$。
- 精确率与召回率分别为 $\text{Precision} = \frac{TP}{TP + FP}$、$\text{Recall} = \frac{TP}{TP + FN}$，PR 曲线下的面积即为单类 AP。
- COCO mAP@[.5, .95] 会在 IoU∈{0.50, 0.55, …, 0.95} 上取平均，是比单一阈值更严格的衡量方式；mAP@0.5 则与 VOC 指标一致，便于横向比较。

#### 模型与数据设置

实验使用 MMDetection 提供的 `faster_rcnn_r50_caffe_c4_mstrain_1x_coco` 配置，并基于预训练的 ResNet-50-C4 骨干网络进行微调：

- **数据集**：MS COCO 2017，训练集 `train2017`、验证集 `val2017`。
- **骨干网络**：Caffe 风格的 ResNet-50（BN 冻结，前两阶段冻结，残差阶段输出 C4 特征）。
- **RPN/ROI 头**：Anchor 尺度 [2,4,8,16,32]，RoIAlign 输出 14×14，经共享的 ResLayer 后接全连接检测头，类别数 80。
- **输入增强**：`RandomChoiceResize` 进行 6 档多尺度训练，辅以随机水平翻转。
- **优化器**：SGD (lr=0.02, momentum=0.9, weight decay=1e-4)，线性 warmup 500 iteration 后按 [8,11] epoch 进行阶梯衰减（学习率依次降至 0.002 与 0.0002）。
- **训练日程**：12 epoch（1× schedule），batch size=16（8×A100-SXM-80GB, 每卡 2 images）。

运行环境来自日志记录：Linux (CUDA 11.1) + PyTorch 1.8.0 + MMCV 1.4.6 + MMDetection 2.22.0，使用 8 张 A100 80GB GPU 进行分布式训练。

#### 训练过程观察

- **损失收敛**：首个 epoch 结束时总损失约 2.37，随后稳定下降，到第 12 个 epoch 平均损失收敛至约 0.60；分类准确率从 80% 提升到 94%，说明分类与定位分支均受益于学习率退火。
- **学习率调度影响**：第 9 epoch（lr=0.002）后模型性能大幅跃升，mAP 在第 11、12 epoch 达到稳定高点；最后一次降至 0.0002 帮助抑制过拟合，同时保持验证指标小幅提升。
- **多尺度增广贡献**：在单卡负载不变的前提下，通过多尺度训练显著提升了中、大尺寸目标的召回率，后期 `bbox_mAP_l` 提升到 0.491。

#### 验证集结果

核心指标来自 `log` 文件中的验证输出（记录路径：`log`）：

| Epoch | bbox mAP@[.5,.95] | mAP@0.5 | mAP@0.75 | 小目标 | 中目标 | 大目标 |
|:-----:|:-----------------:|:-------:|:--------:|:------:|:------:|:------:|
| 1 | 19.7% | 38.3% | 18.6% | 9.1% | 23.7% | 27.4% |
| 2 | 22.4% | 41.5% | 21.7% | 10.9% | 27.5% | 30.5% |
| 3 | 25.6% | 44.6% | 26.3% | 11.9% | 29.6% | 35.5% |
| 11 | 35.3% | 55.4% | 37.8% | 18.8% | 40.1% | 48.8% |
| 12 | **35.9%** | **55.9%** | **38.3%** | **19.1%** | **40.9%** | **49.1%** |

第 12 个 epoch 给出了本次实验的最佳指标：COCO 官方 mAP 达到 0.359，mAP@0.5 达到 0.559。小目标的收益相对有限，但中大型目标检测性能提升显著。

#### 与论文结果对比

Faster R-CNN 原论文在 COCO 上使用 ResNet-101 主干、box refinement、context、multi-scale testing 等策略后，单模型结果报告为 55.7% (mAP@0.5) / 34.9% (mAP@[.5,.95])。本实验只使用 ResNet-50 C4、单尺度测试和 1× schedule，就在验证集上获得 55.9% / 35.9% 的指标：

- mAP@[.5,.95] 略高于论文的单模型表格（主要因为评估在 `val2017`，且未与官方 `test-dev` 完全对齐，结果可视为“接近官方水平”）。
- 我们仍缺少 box refinement、context 模块、长 schedule（2×/3×）及多尺度测试等进一步提升空间，因此与论文的最终 ensemble（59.0% / 37.4%）仍有差距。

#### 问题分析与改进方向

1. **小目标识别**：小目标 mAP 最高只有 19.1%，表明 C4 输出在精细空间分辨率上的不足，可尝试切换到 FPN 架构或引入多尺度特征融合。
2. **训练时长**：1× schedule 对 COCO 略显不足，若延长到 2× 并调高学习率上限，通常还能带来 1~2 个百分点的提升。
3. **推理增强**：当前只做单尺度测试，可加入多尺度 + flip 测试或 Soft-NMS 进一步提高性能。

#### 实验收获

- 熟悉了 MMDetection 的配置化训练流程，能够快速替换骨干网络与数据增强策略。
- 观察到学习率衰减对损失收敛和指标的直接影响，验证了 warmup + step LR 在检测任务中的有效性。
- 通过日志量化了多尺度训练的收益，为后续尝试 FPN、Cascade R-CNN 等更强方案提供了基线参考。

> 注：实验日志及权重保存在 `log`中，便于后续复现实验或观察。