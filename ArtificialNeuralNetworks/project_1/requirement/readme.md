# 大作业

## 环境配置

大作业需要安装的配置：Jupyter Notebook，Python 3.7。
大作业需要安装的库已经放在了 `requirements.txt` 文件中，进入你的 Python 环境，运行以下命令安装：

```bash
pip install -r requirements.txt
```

强烈建议使用 Anaconda 新建一个 Python 3.7 的虚拟环境，在环境中安装以避免不必要的麻烦。创建虚拟环境的教程：[点击这里](https://blog.csdn.net/lyy14011305/article/details/59500819)

除了 `requirements.txt` 中的必要库，还需要运行以下命令：

```bash
conda install -n your-environment-name libpython
conda install -n your-environment-name -c msys2 m2w64-toolchain
```

这是编译 CPython 文件需要的库，因为卷积神经网络需要有效的实现，运行所需的函数都使用 CPython 写好了。在使用之前还需要进入 `setup.py` 所在文件夹，使用运行以下指令进行编译：

```bash
python setup.py build_ext --inplace
```

数据集需要下载并解压到 `annp/dataset/` 文件夹下。

## 内容

### 激活函数（8分）

依照 `ActivationFunction.ipynb` 中的要求：

1. 实现 sigmoid 激活函数的前向传播和反向传播（4分）
2. 实现 tanh 激活函数的前向传播和反向传播（4分）

### 全连接神经网络（15分）

依照 `FullConnectedNetwork.ipynb` 中的要求：

1. 实现 affine layer 的前向传播和反向传播（3分）
2. 实现 ReLU 激活函数的前向传播和反向传播，并在 Jupyter Notebook 上回答问题 1（3分）
3. 利用你实现的 affine layer 和 ReLU 激活函数构建一个两层的全连接神经网络（3分）
4. 训练你实现的两层全连接神经网络，使测试结果的准确率达到 50% 以上（3分）
5. 构建多层的全连接网络，满足 `FullConnectedNetwork.ipynb` 中的测试要求（3分）

### 归一化（10分）

依照 `BatchNormalization.ipynb` 中的要求：

1. 实现 batch normalization 的前向传播和反向传播（3分）
2. 修改你之前实现的全连接神经网络，添加 batch normalization，回答问题 1（3分）
3. 探究 batch normalization 和 batch size 的关系，回答问题 2（2分）
4. 实现 layer normalization 的前向传播和反向传播，并将 layer normalization 添加到你之前实现的全连接神经网络中（1分）
5. 探究 layer normalization 和 batch size 的关系，回答问题 3（1分）

### CNN（32分）

依照 `ConvolutionalNetwork.ipynb` 中的要求：

1. 实现 CNN 的前向传播和反向传播（5分）
2. 实现 max pooling 的前向传播和反向传播（5分）
3. 实现 avg pooling 的前向传播和反向传播（5分）
4. 实现一个三层卷积神经网络（5分）
5. 实现 spatial batch normalization（5分）
6. 实现 spatial layer normalization（4分）
7. 实现 instance normalization（3分）

### 实现 ConvNet（35分）

根据 `ConvolutionalNetwork.ipynb` 中 `Train your best model` 的要求，利用 `annp` 文件夹中的模块实现用于分类 CIFAR-10 数据集的卷积神经网络。需要注意的是，只能用 `annp` 文件夹中的模块实现你的模型，不允许使用额外的深度学习框架。请在 `annp/classifiers/cnn.py` 中实现你的模型，在 Jupyter Notebook 对应位置实现你的训练过程、实验结果以及可视化分析。请各位同学仔细阅读 `annp` 文件夹中每个模块的用法。

## 实验报告

整理你实现的 ConvNet，写一份实验报告描述你的模型架构、调参的过程、分析实验结果以及不同的参数对实验结果的影响，最好是对实验结果进行可视化的分析。实验报告占 20 分。

## 需要提交的文件

1. 你实现的代码，包括 `annp` 中的代码和 Jupyter Notebook 的代码。
2. 你的实验报告。

### 截止时间

大作业截止时间为第十九周周一晚上 12 点。将上述文件打包，命名格式为“姓名+学号.zip”发到助教邮箱。
