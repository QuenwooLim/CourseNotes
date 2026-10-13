# 中山大学计算机院本科生实验报告

（2025学年春季学期）

课程名称：人工神经网络

|实验| 人工神经网络大作业1  |专业（方向）|计算机科学与技术 |
|---|---|---|---|
|学号|23320093 |姓名|林宏宇 |

---

# 激活函数（8分）

## 实现tanh激活函数的前向传播和反向传播（4分）

tanh公式为 $tanh(x) = \frac{e^x - e^{-x}}{e^x + e^{-x}}$，前向传播时，套用公式即可。

```python
    # 套公式计算: tanh(x) = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
    exp_x = np.exp(x)
    exp_neg_x = np.exp(-x)
    out = (exp_x - exp_neg_x) / (exp_x + exp_neg_x)
```

![alt text](image/image.png)

tanh求导公式为 $tanh'(x) = 1 - tanh^2(x)$，反向传播时，套用公式即可。

```python
    # tanh(x)的导数是1 - tanh²(x)
    tanh_x = cache
    dx = dout * (1 - tanh_x**2)
```

![alt text](image/image-1.png)


## 实现 sigmoid 激活函数的前向传播和反向传播（4分）

Sigmoid公式为 $\sigma(x) = \frac{1}{1 + e^{-x}}$，前向传播时，套用公式计算即可。

```python
    # 套用Sigmoid公式计算: σ(x) = 1 / (1 + exp(-x))
    out = 1 / (1 + np.exp(-x))
```

![alt text](image/image-2.png)

Sigmoid求导公式为 $\sigma'(x) = \sigma(x) \cdot (1 - \sigma(x))$，反向传播时，利用前向传播的结果计算导数。

```python
    # Sigmoid函数的导数是 σ(x) * (1 - σ(x))
    # cache中存储了前向传播的输出，即σ(x)
    sigmoid_x = cache
    dx = dout * sigmoid_x * (1 - sigmoid_x)
```

![alt text](image/image-3.png)

# 全连接神经网络（15分）

## 实现 affine layer 的前向传播和反向传播（3分）

全连接层（affine layer）的前向传播需要将输入数据 x 重塑为二维数组，然后与权重矩阵 w 相乘并加上偏置 b。

前向传播的数学表达式为：
$$Z = XW + b$$

其中，$X$ 是输入矩阵，$W$ 是权重矩阵，$b$ 是偏置向量。

```python
# 将输入x重塑为二维数组，其中第一维是样本数N，第二维是将所有特征展平后的维度D
N = x.shape[0]
x_reshaped = x.reshape(N, -1)

# 计算全连接层的前向传播: out = x * w + b
out = np.dot(x_reshaped, w) + b
```
![alt text](image/image-4.png)

全连接层的反向传播需要计算关于输入 x、权重 w 和偏置 b 的梯度。根据链式法则，可以得到以下梯度计算公式：

$$\frac{\partial L}{\partial W} = X^T \cdot \frac{\partial L}{\partial Z}$$

$$\frac{\partial L}{\partial b} = \sum_{i=1}^{N} \frac{\partial L}{\partial Z_i}$$

$$\frac{\partial L}{\partial X} = \frac{\partial L}{\partial Z} \cdot W^T$$

其中，$\frac{\partial L}{\partial Z}$ 是从上层传回的梯度，$N$ 是样本数量。

```python
# 获取缓存中的数据
N = x.shape[0]

# 将输入x重塑为二维数组，用于计算梯度
x_reshaped = x.reshape(N, -1)

# 计算关于权重的梯度: dw = x^T * dout
dw = np.dot(x_reshaped.T, dout)

# 计算关于偏置的梯度: db = sum(dout, axis=0)
db = np.sum(dout, axis=0)

# 计算关于输入的梯度: dx = dout * w^T
dx_reshaped = np.dot(dout, w.T)

# 将dx重塑回与原始输入x相同的形状
dx = dx_reshaped.reshape(x.shape)
```
![alt text](image/image-5.png)

## 实现 ReLU 激活函数的前向传播和反向传播，并在 Jupyter Notebook 上回答问题 1（3分）

ReLU（Rectified Linear Unit）激活函数的数学表达式为：
$$f(x) = \max(0, x)$$

前向传播时，只需要将输入中的负值变为0，保留所有非负值即可。

```python
# ReLU函数: f(x) = max(0, x)
out = np.maximum(0, x)
```
![alt text](image/image-6.png)

ReLU的导数很简单：
$$f'(x) = \begin{cases}
1, & \text{if } x > 0 \\
0, & \text{if } x \leq 0
\end{cases}$$

反向传播时，只需要将输入为负数的位置的梯度置为0，其他位置的梯度保持不变。

```python
# ReLU的导数: 当x > 0时为1，当x <= 0时为0
dx = dout * (x > 0)
```

![alt text](image/image-7.png)

![alt text](image/image-8.png)

问题已经在Jupyter Notebook中回答。

## 利用你实现的 affine layer 和 ReLU 激活函数构建一个两层的全连接神经网络（3分）

本次实验实现了一个两层全连接神经网络，其架构为：

```
affine - relu - affine - softmax
```

这个网络包含以下组件：
1. 第一层：全连接层（affine）+ ReLU激活函数
2. 第二层：全连接层（affine）
3. 损失函数：softmax交叉熵损失

### 初始化参数

在网络初始化阶段，需要设置权重和偏置参数：

```python
# 初始化第一层的权重和偏置
# W1的形状为(input_dim, hidden_dim)，从均值为0，标准差为weight_scale的高斯分布中采样
self.params['W1'] = np.random.normal(0.0, weight_scale, (input_dim, hidden_dim))
# b1的形状为(hidden_dim,)，初始化为0
self.params['b1'] = np.zeros(hidden_dim)

# 初始化第二层的权重和偏置
# W2的形状为(hidden_dim, num_classes)，从均值为0，标准差为weight_scale的高斯分布中采样
self.params['W2'] = np.random.normal(0.0, weight_scale, (hidden_dim, num_classes))
# b2的形状为(num_classes,)，初始化为0
self.params['b2'] = np.zeros(num_classes)
```

### 前向传播

前向传播过程按照以下步骤进行：

```python
# 获取权重和偏置参数
W1, b1 = self.params['W1'], self.params['b1']
W2, b2 = self.params['W2'], self.params['b2']

# 第一层：affine - relu
# 使用affine_relu_forward函数计算第一层的输出和缓存
hidden_layer, hidden_cache = affine_relu_forward(X, W1, b1)

# 第二层：affine
# 使用affine_forward函数计算第二层的输出和缓存
scores, output_cache = affine_forward(hidden_layer, W2, b2)

# 保存中间结果用于反向传播
self.cache = (hidden_cache, output_cache)
```

### 损失计算和反向传播

损失计算和反向传播过程包括：

```python
# 获取权重参数
W1, W2 = self.params['W1'], self.params['W2']

# 计算softmax损失和梯度
loss, dscores = softmax_loss(scores, y)

# 添加L2正则化损失
# 正则化损失 = 0.5 * reg * (||W1||^2 + ||W2||^2)
reg_loss = 0.5 * self.reg * (np.sum(W1 * W1) + np.sum(W2 * W2))
loss += reg_loss

# 反向传播
hidden_cache, output_cache = self.cache

# 第二层的反向传播（affine层）
dhidden, dW2, db2 = affine_backward(dscores, output_cache)

# 第一层的反向传播（affine-relu层）
dx, dW1, db1 = affine_relu_backward(dhidden, hidden_cache)

# 添加L2正则化的梯度
dW1 += self.reg * W1
dW2 += self.reg * W2

# 将梯度存储在grads字典中
grads = {
    'W1': dW1,
    'b1': db1,
    'W2': dW2,
    'b2': db2
}
```

运行截图

![alt text](image/image-9.png)


## 训练你实现的两层全连接神经网络，使测试结果的准确率达到 50% 以上（3分）

调整参数，隐藏层1000，学习率5e-4，正则化0.02

```python
# 创建的TwoLayerNet模型
model = TwoLayerNet(hidden_dim=1000, weight_scale=5e-4, reg=0.02)

# 创建一个Solver实例
solver = Solver(
    model=model,
    data=data,
    update_rule='adam',
    optim_config={
        'learning_rate': 5e-4,  # 较低的学习率以保持训练稳定
    },
    lr_decay=0.95,  # 学习率衰减因子，防止训练后期震荡
    num_epochs=12,
    batch_size=400,
    print_every=100
)

# 训练模型
solver.train()
```

运行截图：
![alt text](image/image-10.png)
损失函数图像：
![alt text](image/image-11.png)


## 构建多层的全连接网络，满足 `FullConnectedNetwork.ipynb` 中的测试要求（3分）

在这一部分中，实现了一个具有任意数量隐藏层的全连接神经网络。这个网络支持多种功能，包括不同数量的隐藏层、dropout正则化，并为批量归一化和层归一化做了准备。

### 网络架构

实现的全连接网络架构如下：
```
{affine - [batch/layer norm] - relu - [dropout]} x (L - 1) - affine - softmax
```
其中：
- L是网络的总层数（包括输入层、隐藏层和输出层）
- batch/layer norm和dropout是可选的
- {...}块在网络中重复L-1次

### 参数初始化

在`__init__`方法中，初始化了网络的所有参数：

```python
# 存储所有层的维度，包括输入层、隐藏层和输出层
dims = [input_dim] + hidden_dims + [num_classes]

# 初始化所有层的权重和偏置
for i in range(self.num_layers):
    # 初始化第i层的权重，从均值为0，标准差为weight_scale的高斯分布中采样
    self.params[f'W{i+1}'] = np.random.normal(0.0, weight_scale, (dims[i], dims[i+1]))
    # 初始化第i层的偏置为0
    self.params[f'b{i+1}'] = np.zeros(dims[i+1])

    # 如果使用批量归一化，初始化gamma和beta参数（除了最后一层）
    if self.normalization in ['batchnorm', 'layernorm'] and i < self.num_layers - 1:
        # gamma初始化为1
        self.params[f'gamma{i+1}'] = np.ones(dims[i+1])
        # beta初始化为0
        self.params[f'beta{i+1}'] = np.zeros(dims[i+1])
```

- 权重从均值为0、标准差为`weight_scale`的高斯分布中采样
- 偏置初始化为0
- 对于批量归一化和层归一化，gamma初始化为1，beta初始化为0

### 前向传播

在`loss`方法中，实现了网络的前向传播：

```python
# 存储每一层的输入和缓存，用于反向传播
x = X
caches = []

# 如果使用dropout，需要存储dropout的缓存
dropout_caches = []

# 前向传播通过所有隐藏层
for i in range(self.num_layers - 1):
    # 获取当前层的权重和偏置
    W = self.params[f'W{i+1}']
    b = self.params[f'b{i+1}']

    # 使用普通的前向传播
    x, cache = affine_relu_forward(x, W, b)

    # 存储缓存
    caches.append(cache)

    # 如果使用dropout，应用dropout
    if self.use_dropout:
        x, dropout_cache = dropout_forward(x, self.dropout_param)
        dropout_caches.append(dropout_cache)

# 最后一层（输出层）的前向传播
W = self.params[f'W{self.num_layers}']
b = self.params[f'b{self.num_layers}']
scores, cache = affine_forward(x, W, b)
caches.append(cache)
```

1. 对于每个隐藏层，执行affine变换后接ReLU激活
2. 如果启用了dropout，在ReLU激活后应用dropout
3. 对于输出层，只执行affine变换得到最终的分数

### 反向传播

反向传播的实现如下：

```python
# 计算softmax损失
loss, dscores = softmax_loss(scores, y)

# 添加L2正则化损失
for i in range(self.num_layers):
    W = self.params[f'W{i+1}']
    loss += 0.5 * self.reg * np.sum(W * W)

# 反向传播
dout = dscores

# 最后一层（输出层）的反向传播
dx, dw, db = affine_backward(dout, caches[self.num_layers - 1])
grads[f'W{self.num_layers}'] = dw + self.reg * self.params[f'W{self.num_layers}']
grads[f'b{self.num_layers}'] = db

# 反向传播通过所有隐藏层
for i in range(self.num_layers - 2, -1, -1):
    # 如果使用dropout，先通过dropout层的反向传播
    if self.use_dropout:
        dx = dropout_backward(dx, dropout_caches[i])

    # 使用普通的反向传播
    dx, dw, db = affine_relu_backward(dx, caches[i])

    # 存储梯度，并添加L2正则化的梯度
    grads[f'W{i+1}'] = dw + self.reg * self.params[f'W{i+1}']
    grads[f'b{i+1}'] = db
```

1. 计算softmax损失和L2正则化损失
2. 从输出层开始，计算每一层的梯度
3. 对于每个隐藏层，如果使用了dropout，先通过dropout层的反向传播
4. 然后通过affine-relu层的反向传播
5. 最后将L2正则化的梯度添加到权重梯度中

### 测试结果

> 作为合理性检查，运行以下代码来检查初始损失，并对有正则化和无正则化的网络进行梯度检查。初始损失看起来合理吗？

> 对于梯度检查，你应该期望看到误差在 1e-7 或更小。

运行结果符合预期

![alt text](image/image-12.png)

---

> 作为另一个合理性检查，确保你可以对一个包含50张图像的小数据集进行过拟合。首先，将尝试一个每个隐藏层有100个单元的三层网络。在下面的单元格中，调整**学习率**和**权重初始化尺度**，以实现过拟合并在20个训练周期内达到100%的训练准确率。

```python
weight_scale = 5e-2   # 增大权重初始化尺度
learning_rate = 1e-3  # 增大学习率
```

![alt text](image/image-13.png)

---

> 现在尝试使用一个每层有100个单元的五层网络来过拟合50个训练样本。同样，你需要调整学习率和权重初始化的尺度，但你应该能够在20个训练周期内实现100%的训练准确率。

```python
learning_rate = 1e-2
weight_scale = 5e-2
```
![alt text](image/image-14.png)


# 归一化（10分）


## 实现 batch normalization 的前向传播和反向传播（3分）

### 前向传播

> 在文件 `annp/layers.py` 中，实现函数 `batchnorm_forward` 中的批量归一化前向传播。

训练模式
当 `mode` 为 `"train"` 时：
1. **计算统计量**：借助 `np.mean` 和 `np.var` 函数，分别算出输入数据 `x` 在每个特征维度上的批次均值 `sample_mean` 与批次方差 `sample_var`。
2. **数据标准化**：把输入数据 `x` 减去均值再除以标准差（方差加上一个小的常数 `eps` 后开方），得到标准化后的数据 `x_normalized`。
3. **缩放和平移**：对标准化后的数据 `x_normalized` 进行缩放（乘以 `gamma`）与平移（加上 `beta`）操作，最终结果存于 `out` 中。
4. **更新运行时统计量**：按照 `momentum` 参数来更新运行时均值 `running_mean` 和运行时方差 `running_var`。
5. **缓存中间值**：把反向传播时需要用到的中间值（如输入数据、标准化后的数据、均值、方差、`gamma`、`beta` 和 `eps`）存于 `cache` 里。


```python
if mode == "train":
        #######################################################################
        # TODO: Implement the training-time forward pass for batch norm.      #
        # Use minibatch statistics to compute the mean and variance, use      #
        # these statistics to normalize the incoming data, and scale and      #
        # shift the normalized data using gamma and beta.                     #
        #                                                                     #
        # You should store the output in the variable out. Any intermediates  #
        # that you need for the backward pass should be stored in the cache   #
        # variable.                                                           #
        #                                                                     #
        # You should also use your computed sample mean and variance together #
        # with the momentum variable to update the running mean and running   #
        # variance, storing your result in the running_mean and running_var   #
        # variables.                                                          #
        #                                                                     #
        # Note that though you should be keeping track of the running         #
        # variance, you should normalize the data based on the standard       #
        # deviation (square root of variance) instead!                        #
        # Referencing the original paper (https://arxiv.org/abs/1502.03167)   #
        # might prove to be helpful.                                          #
        #######################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # 计算批次均值 (N, D) -> (1, D)
        sample_mean = np.mean(x, axis=0)

        # 计算批次方差 (N, D) -> (1, D)
        sample_var = np.var(x, axis=0)

        # 标准化数据 (x - mean) / sqrt(var + eps)
        x_normalized = (x - sample_mean) / np.sqrt(sample_var + eps)

        # 缩放和平移标准化后的数据
        out = gamma * x_normalized + beta

        # 更新运行时均值和方差
        running_mean = momentum * running_mean + (1 - momentum) * sample_mean
        running_var = momentum * running_var + (1 - momentum) * sample_var

        # 存储反向传播所需的中间值
        cache = (x, x_normalized, sample_mean, sample_var, gamma, beta, eps)

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        #######################################################################
        #                           END OF YOUR CODE                          #
        #######################################################################
```

当 `mode` 为 `"test"` 时：
1. **数据标准化**：使用已经计算好的运行时均值 `running_mean` 和运行时方差 `running_var` 对输入数据 `x` 进行标准化，得到 `x_normalized`。
2. **缩放和平移**：对标准化后的数据 `x_normalized` 进行缩放（乘以 `gamma`）与平移（加上 `beta`）操作，结果存于 `out` 中。
3. **无缓存中间值**：由于测试模式下不需要进行反向传播，所以 `cache` 被设为 `None`。
![alt text](image/image-15.png)

```python
    elif mode == "test":
        #######################################################################
        # TODO: Implement the test-time forward pass for batch normalization. #
        # Use the running mean and variance to normalize the incoming data,   #
        # then scale and shift the normalized data using gamma and beta.      #
        # Store the result in the out variable.                               #
        #######################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # 在测试模式下，使用运行时均值和方差进行标准化
        x_normalized = (x - running_mean) / np.sqrt(running_var + eps)

        # 缩放和平移标准化后的数据
        out = gamma * x_normalized + beta

        # 测试模式下不需要存储反向传播的中间值
        cache = None

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        #######################################################################
        #                          END OF YOUR CODE                           #
        #######################################################################
```

![alt text](image/image-16.png)

### 反向传播

> 现在在函数 `batchnorm_backward` 中实现批量归一化的反向传播。

根据链式法则结果完成相关代码如下：

```python
    # 从缓存中获取前向传播的中间值
    x, x_normalized, mean, var, gamma, beta, eps = cache
    N, D = x.shape

    # 计算关于beta的梯度 - 简单地对dout求和
    dbeta = np.sum(dout, axis=0)

    # 计算关于gamma的梯度 - dout * x_normalized的乘积求和
    dgamma = np.sum(dout * x_normalized, axis=0)

    # 计算关于标准化输入的梯度
    dx_normalized = dout * gamma

    # 计算关于方差的梯度
    std_inv = 1.0 / np.sqrt(var + eps)
    dvar = np.sum(dx_normalized * (x - mean) * (-0.5) * std_inv**3, axis=0)

    # 计算关于均值的梯度
    dmean = np.sum(dx_normalized * (-std_inv), axis=0) + dvar * np.sum(-2 * (x - mean), axis=0) / N

    # 计算关于输入x的梯度
    dx = dx_normalized * std_inv + dvar * 2 * (x - mean) / N + dmean / N
```

![alt text](image/image-17.png)


## 修改你之前实现的全连接神经网络，添加 batch normalization，回答问题 1（3分）


### 前向传播修改

在前向传播中，当`normalization`参数设置为`'batchnorm'`时，需要在每个ReLU非线性激活层之前插入一个批量归一化层：

```python
# 根据是否使用归一化选择不同的前向传播方式
if self.normalization == 'batchnorm':
    # 获取当前层的gamma和beta参数
    gamma = self.params[f'gamma{i+1}']
    beta = self.params[f'beta{i+1}']
    # 使用带批量归一化的前向传播
    x, cache = affine_bn_relu_forward(x, W, b, gamma, beta, self.bn_params[i])
else:
    # 使用普通的前向传播
    x, cache = affine_relu_forward(x, W, b)
```

这段代码的主要变化是：
1. 检查是否启用了批量归一化
2. 如果启用，从参数字典中获取当前层的缩放参数`gamma`和平移参数`beta`
3. 调用`affine_bn_relu_forward`函数，该函数会依次执行仿射变换、批量归一化和ReLU激活
4. 将当前层对应的批量归一化参数`self.bn_params[i]`传递给函数，这些参数包含了运行模式（训练或测试）以及运行时的均值和方差

### 反向传播修改

在反向传播中，同样需要根据是否使用批量归一化来选择不同的反向传播方式：

```python
# 根据是否使用归一化选择不同的反向传播方式
if self.normalization == 'batchnorm':
    # 使用带批量归一化的反向传播
    dx, dw, db, dgamma, dbeta = affine_bn_relu_backward(dx, caches[i])
    # 存储gamma和beta的梯度
    grads[f'gamma{i+1}'] = dgamma
    grads[f'beta{i+1}'] = dbeta
else:
    # 使用普通的反向传播
    dx, dw, db = affine_relu_backward(dx, caches[i])
```

这段代码的主要变化是：
1. 检查是否启用了批量归一化
2. 如果启用，调用`affine_bn_relu_backward`函数，该函数会计算关于输入、权重、偏置以及批量归一化参数`gamma`和`beta`的梯度
3. 将计算得到的`gamma`和`beta`的梯度存储在梯度字典中，以便优化器更新这些参数

---

可视化运行结果：带norm的结果显loss下降更快且准确率往往更高。
![alt text](image/image-18.png)

---

> 现在将进行一个小实验，研究批量归一化（batch normalization）和权重初始化之间的相互作用。

![alt text](image/image-19.png)

结果
- 验证集准确率：无批量归一化（BaseLine）模型波动大，先升后降；有批量归一化（batchnorm）模型更平滑，前期上升，后期下降，且整体更高。
- 训练集准确率：BaseLine模型波动大，batchnorm模型先升后降，多数情况batchnorm更高。
- 最终训练损失：BaseLine模型先降后剧增，batchnorm模型更稳定，损失更低，后期增幅小。

原因
- 无批量归一化：小尺度易梯度消失，大尺度易梯度爆炸，影响训练。
- 有批量归一化：能减少内部协变量偏移，稳定梯度，让模型适应更广尺度范围 。

## 探究 batch normalization 和 batch size 的关系，回答问题 2（2分）

执行结果：
![alt text](image/image-20.png)

实验结果：

随着训练轮数（Epoch）增加，使用批量归一化（Batch Normalization）的模型（`with_norm5`、`with_norm10`、`with_norm50` ），其训练准确率总体上比 `baseline5` 更高。其中，`with_norm50` 的训练准确率最高，其次是 `with_norm10` 和 `with_norm5` 。

一开始，使用批量归一化的模型的验证准确率存在波动。但总体而言，`with_norm50` 和 `with_norm10` 表现相对较好。`baseline5` 的验证准确率相对较低，且波动更大。

批量归一化与批量大小的关系：

随着批量大小（如 `with_norm5`、`with_norm10`、`with_norm50` 中的 5、10、50  ）增大，使用批量归一化的模型性能（训练和验证准确率）提升。就提高准确率而言，较大的批量大小与批量归一化配合效果似乎更好。

出现这种关系的原因：

批量归一化会对神经网络各层的输入进行归一化。较大的批量大小能更准确地估计小批量数据的真实分布，从而使归一化更稳定、有效。较小的批量大小对数据分布的估计可能更嘈杂，会限制批量归一化的效果。

较大的批量大小能带来更一致的梯度更新。批量归一化有助于减少内部协变量偏移，在较大批量大小下，模型在优化过程中能采取更稳定、有意义的步骤，进而提升整体性能。

## 实现 layer normalization 的前向传播和反向传播，并将 layer normalization 添加到你之前实现的全连接神经网络中（1分）

### 在 `annp/layers.py` 中，实现层归一化的前向传播函数 `layernorm_forward`。

具体实现如下

```python
    # 获取输入数据的形状
    N, D = x.shape

    # 计算每个数据点的均值 (N, D) -> (N, 1)
    # 注意：与批量归一化不同，这里沿着特征维度（axis=1）计算均值
    sample_mean = np.mean(x, axis=1, keepdims=True)  # (N, 1)

    # 计算每个数据点的方差 (N, D) -> (N, 1)
    sample_var = np.var(x, axis=1, keepdims=True)  # (N, 1)

    # 标准化数据 (x - mean) / sqrt(var + eps)
    x_normalized = (x - sample_mean) / np.sqrt(sample_var + eps)  # (N, D)

    # 缩放和平移标准化后的数据
    # 注意：gamma和beta的形状是(D,)，与x_normalized的形状(N, D)直接广播运算
    out = gamma * x_normalized + beta  # (N, D)

    # 存储反向传播所需的中间值
    cache = (x, x_normalized, sample_mean, sample_var, gamma, beta, eps)
```

此外，原始注释疑似笔误

原始注释为：
```
    - gamma: Scale parameter of shape (N,)
    - beta: Shift paremeter of shape (N,)
```

但是LayerNorm应该是每个样本用同样的参数归一化，应该改为(D,)，特征维度

![alt text](image/image-21.png)


### 在 `annp/layers.py` 中，实现层归一化的反向传播函数 `layernorm_backward`。


具体实现如下

```python
    # 从缓存中获取前向传播的中间值
    x, x_normalized, mean, var, gamma, beta, eps = cache
    N, D = x.shape

    # 计算关于beta的梯度 - 求和得到每个特征维度的梯度
    dbeta = np.sum(dout, axis=0)  # (D,)

    # 计算关于gamma的梯度 - 每个特征维度的标准化值与上游梯度的乘积求和
    dgamma = np.sum(dout * x_normalized, axis=0)  # (D,)

    # 计算关于标准化输入的梯度
    dx_normalized = dout * gamma  # (N, D)

    # 以下计算关于输入x的梯度
    # 注意：层归一化中，求的是每个数据点在特征维度上的均值和方差
    # 因此需要沿着特征维度（axis=1）计算梯度

    # 标准差的倒数
    std_inv = 1.0 / np.sqrt(var + eps)  # (N, 1)

    # 计算关于方差的梯度
    dvar = np.sum(dx_normalized * (x - mean) * (-0.5) * std_inv**3, axis=1, keepdims=True)  # (N, 1)

    # 计算关于均值的梯度
    dmean = np.sum(dx_normalized * (-std_inv), axis=1, keepdims=True) + \
            dvar * np.sum(-2 * (x - mean), axis=1, keepdims=True) / D  # (N, 1)

    # 计算关于输入x的梯度
    dx = dx_normalized * std_inv + dvar * 2 * (x - mean) / D + dmean / D  # (N, D)
```

![alt text](image/image-22.png)

### 修改 `annp/classifiers/fc_net.py`，在 `FullyConnectedNet` 中添加层归一化。当构造函数中的 `normalization` 参数设置为 `"layernorm"` 时，你应该在每个 ReLU 非线性激活层之前插入一个层归一化层。

#### 前向传播修改部分

辅助函数

```python
def affine_ln_relu_forward(x, w, b, gamma, beta, ln_param):
    """
    执行仿射变换后接层归一化（Layer Normalization）和ReLU的便捷层。
    """
    # 仿射变换：计算线性组合 a = xW + b
    a, fc_cache = affine_forward(x, w, b)

    # 层归一化：对仿射变换的输出进行归一化
    ln, ln_cache = layernorm_forward(a, gamma, beta, ln_param)

    # ReLU 激活：对归一化后的结果应用 ReLU 激活函数
    out, relu_cache = relu_forward(ln)

    # 将所有中间结果存储在缓存中，用于反向传播
    cache = (fc_cache, ln_cache, relu_cache)
    return out, cache
```

修改`fc_net.py`

```python
            elif self.normalization == 'layernorm':
                # 获取当前层的gamma和beta参数
                gamma = self.params[f'gamma{i+1}']
                beta = self.params[f'beta{i+1}']
                # 使用带层归一化的前向传播
                x, cache = affine_ln_relu_forward(x, W, b, gamma, beta, self.bn_params[i])
```

#### 反向传播修改部分


辅助函数

```python
def affine_ln_relu_backward(dout, cache):
    """
    仿射-层归一化-ReLU便捷层的反向传播过程。
    """
    # 从缓存中提取前向传播的中间结果
    fc_cache, ln_cache, relu_cache = cache

    # ReLU 激活的反向传播
    dln = relu_backward(dout, relu_cache)

    # 层归一化的反向传播
    da, dgamma, dbeta = layernorm_backward(dln, ln_cache)

    # 仿射变换的反向传播
    dx, dw, db = affine_backward(da, fc_cache)
    return dx, dw, db, dgamma, dbeta
```

修改`fc_net.py`

```python
            elif self.normalization == 'layernorm':
                # 使用带层归一化的反向传播
                dx, dw, db, dgamma, dbeta = affine_ln_relu_backward(dx, caches[i])
                # 存储gamma和beta的梯度
                grads[f'gamma{i+1}'] = dgamma
                grads[f'beta{i+1}'] = dbeta
```



## 探究 layer normalization 和 batch size 的关系，回答问题 3（1分）

![alt text](image/image-23.png)

当特征维度非常小时，层归一化可能效果不佳。

原因在于层归一化是对单个样本的所有特征进行归一化操作。如果特征维度很小，那么在这些少量特征上进行归一化操作，可能会过度改变数据的分布特征，破坏数据原本的信息结构，无法有效起到归一化稳定训练的作用，还可能引入噪声等负面影响。

而在非常深的网络中，层归一化有助于缓解梯度消失等问题；高正则化项与层归一化本身是否能良好工作并无直接关联。

# CNN（32分）

## 实现 CNN 的前向传播和反向传播（5分）

### 前向传播

修改`conv_forward_naive`

```python
    # 获取输入数据的维度
    N, C, H, W = x.shape
    # 获取卷积核的维度
    F, _, HH, WW = w.shape
    # 获取卷积参数
    stride = conv_param['stride']
    pad = conv_param['pad']

    # 计算输出特征图的尺寸
    H_out = 1 + (H + 2 * pad - HH) // stride
    W_out = 1 + (W + 2 * pad - WW) // stride

    # 初始化输出数组
    out = np.zeros((N, F, H_out, W_out))

    # 对输入数据进行填充
    x_padded = np.pad(x, ((0, 0), (0, 0), (pad, pad), (pad, pad)), mode='constant')

    # 执行卷积操作
    for n in range(N):  # 遍历每个样本
        for f in range(F):  # 遍历每个卷积核
            for h_out in range(H_out):  # 遍历输出特征图的高度
                for w_out in range(W_out):  # 遍历输出特征图的宽度
                    # 计算当前卷积窗口的左上角位置
                    h_start = h_out * stride
                    w_start = w_out * stride

                    # 提取当前卷积窗口
                    x_window = x_padded[n, :, h_start:h_start+HH, w_start:w_start+WW]

                    # 计算卷积结果（点积 + 偏置）
                    out[n, f, h_out, w_out] = np.sum(x_window * w[f]) + b[f]
```

![alt text](image/image-24.png)

![alt text](image/image-25.png)

### 反向传播

修改`conv_backward_naive`

```python
    # 获取缓存中的数据
    x, w, b, conv_param = cache

    # 获取卷积参数
    stride = conv_param['stride']
    pad = conv_param['pad']

    # 获取输入数据的维度
    N, C, H, W = x.shape
    # 获取卷积核的维度
    F, _, HH, WW = w.shape
    # 获取输出梯度的维度
    _, _, H_out, W_out = dout.shape

    # 初始化梯度
    dx = np.zeros_like(x)
    dw = np.zeros_like(w)
    db = np.zeros_like(b)

    # 对输入数据进行填充（与前向传播相同）
    x_padded = np.pad(x, ((0, 0), (0, 0), (pad, pad), (pad, pad)), mode='constant')
    # 初始化填充后的输入梯度
    dx_padded = np.zeros_like(x_padded)

    # 计算偏置项的梯度
    # db的形状是(F,)，对于每个卷积核，将相应的dout所有元素求和
    for f in range(F):
        db[f] = np.sum(dout[:, f, :, :])

    # 计算权重和输入的梯度
    for n in range(N):  # 遍历每个样本
        for f in range(F):  # 遍历每个卷积核
            for h_out in range(H_out):  # 遍历输出特征图的高度
                for w_out in range(W_out):  # 遍历输出特征图的宽度
                    # 计算当前卷积窗口的左上角位置
                    h_start = h_out * stride
                    w_start = w_out * stride

                    # 提取当前卷积窗口
                    x_window = x_padded[n, :, h_start:h_start+HH, w_start:w_start+WW]

                    # 计算权重的梯度：将输入窗口与上游梯度相乘
                    dw[f] += x_window * dout[n, f, h_out, w_out]

                    # 计算填充后输入的梯度：将权重与上游梯度相乘
                    dx_padded[n, :, h_start:h_start+HH, w_start:w_start+WW] += w[f] * dout[n, f, h_out, w_out]

    # 从填充后的梯度中提取原始输入的梯度
    dx = dx_padded[:, :, pad:pad+H, pad:pad+W]
```

![alt text](image/image-26.png)

## 实现 max pooling 的前向传播和反向传播（5分）

### 前向传播

```python
    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 获取池化参数
    pool_height = pool_param['pool_height']
    pool_width = pool_param['pool_width']
    stride = pool_param['stride']

    # 计算输出特征图的尺寸
    H_out = 1 + (H - pool_height) // stride
    W_out = 1 + (W - pool_width) // stride

    # 初始化输出数组
    out = np.zeros((N, C, H_out, W_out))


    # 执行最大池化操作
    for n in range(N):  # 遍历每个样本
        for c in range(C):  # 遍历每个通道
            for h_out in range(H_out):  # 遍历输出特征图的高度
                for w_out in range(W_out):  # 遍历输出特征图的宽度
                    # 计算当前池化窗口的左上角位置
                    h_start = h_out * stride
                    w_start = w_out * stride

                    # 提取当前池化窗口
                    pool_window = x[n, c, h_start:h_start+pool_height, w_start:w_start+pool_width]

                    # 找到最大值
                    max_val = np.max(pool_window)

                    # 存储最大值
                    out[n, c, h_out, w_out] = max_val

    cache = (x, pool_param)
```

![alt text](image/image-27.png)

### 反向传播

```python
    # 获取缓存中的数据
    x, pool_param = cache

    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 获取池化参数
    pool_height = pool_param['pool_height']
    pool_width = pool_param['pool_width']
    stride = pool_param['stride']

    # 获取输出梯度的维度
    _, _, H_out, W_out = dout.shape

    # 初始化输入梯度
    dx = np.zeros_like(x)

    # 执行最大池化的反向传播
    for n in range(N):  # 遍历每个样本
        for c in range(C):  # 遍历每个通道
            for h_out in range(H_out):  # 遍历输出特征图的高度
                for w_out in range(W_out):  # 遍历输出特征图的宽度
                    # 计算当前池化窗口的左上角位置
                    h_start = h_out * stride
                    w_start = w_out * stride

                    # 提取当前池化窗口
                    pool_window = x[n, c, h_start:h_start+pool_height, w_start:w_start+pool_width]

                    # 找到最大值的位置
                    max_idx = np.argmax(pool_window.flatten())
                    max_i, max_j = np.unravel_index(max_idx, (pool_height, pool_width))

                    # 将上游梯度传递给最大值对应的输入位置
                    dx[n, c, h_start + max_i, w_start + max_j] += dout[n, c, h_out, w_out]
```

![alt text](image/image-28.png)

## 实现 avg pooling 的前向传播和反向传播（5分）

### 前向传播

```python
    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 获取池化参数
    pool_height = pool_param['pool_height']
    pool_width = pool_param['pool_width']
    stride = pool_param['stride']

    # 计算输出特征图的尺寸
    H_out = 1 + (H - pool_height) // stride
    W_out = 1 + (W - pool_width) // stride

    # 初始化输出数组
    out = np.zeros((N, C, H_out, W_out))

    # 执行平均池化操作
    for n in range(N):  # 遍历每个样本
        for c in range(C):  # 遍历每个通道
            for h_out in range(H_out):  # 遍历输出特征图的高度
                for w_out in range(W_out):  # 遍历输出特征图的宽度
                    # 计算当前池化窗口的左上角位置
                    h_start = h_out * stride
                    w_start = w_out * stride

                    # 提取当前池化窗口
                    pool_window = x[n, c, h_start:h_start+pool_height, w_start:w_start+pool_width]

                    # 计算平均值
                    avg_val = np.mean(pool_window)

                    # 存储平均值
                    out[n, c, h_out, w_out] = avg_val
```
![alt text](image/image-29.png)

### 反向传播

```python
    # 获取缓存中的数据
    x, pool_param = cache

    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 获取池化参数
    pool_height = pool_param['pool_height']
    pool_width = pool_param['pool_width']
    stride = pool_param['stride']

    # 获取输出梯度的维度
    _, _, H_out, W_out = dout.shape

    # 初始化输入梯度
    dx = np.zeros_like(x)

    # 执行平均池化的反向传播
    for n in range(N):  # 遍历每个样本
        for c in range(C):  # 遍历每个通道
            for h_out in range(H_out):  # 遍历输出特征图的高度
                for w_out in range(W_out):  # 遍历输出特征图的宽度
                    # 计算当前池化窗口的左上角位置
                    h_start = h_out * stride
                    w_start = w_out * stride

                    # 平均池化的反向传播是将上游梯度均匀分配给池化窗口中的每个元素
                    # 池化窗口中的每个元素都会收到相同的梯度值，该值等于上游梯度除以池化窗口的元素数量
                    dx[n, c, h_start:h_start+pool_height, w_start:w_start+pool_width] += \
                        dout[n, c, h_out, w_out] / (pool_height * pool_width)

```
![alt text](image/image-30.png)

## 实现一个三层卷积神经网络（5分）

架构为：

```
conv - relu - 2x2 pool - affine - relu - affine - softmax
```

这个网络包含一个卷积层、两个全连接层，以及相应的ReLU激活函数和池化层。

### 初始化参数

首先，在`__init__`方法中初始化网络的权重和偏置：

```python
# 获取输入维度
C, H, W = input_dim

# 初始化卷积层的权重和偏置
# W1的形状为(num_filters, C, filter_size, filter_size)
self.params['W1'] = np.random.normal(0.0, weight_scale, (num_filters, C, filter_size, filter_size))
# b1的形状为(num_filters,)
self.params['b1'] = np.zeros(num_filters)

# 计算卷积层输出的尺寸（由于padding设置，宽高保持不变）
# 池化层会将尺寸减半
conv_output_height = H // 2
conv_output_width = W // 2

# 计算卷积层输出展平后的维度
conv_output_size = num_filters * conv_output_height * conv_output_width

# 初始化第一个全连接层的权重和偏置
# W2的形状为(conv_output_size, hidden_dim)
self.params['W2'] = np.random.normal(0.0, weight_scale, (conv_output_size, hidden_dim))
# b2的形状为(hidden_dim,)
self.params['b2'] = np.zeros(hidden_dim)

# 初始化第二个全连接层（输出层）的权重和偏置
# W3的形状为(hidden_dim, num_classes)
self.params['W3'] = np.random.normal(0.0, weight_scale, (hidden_dim, num_classes))
# b3的形状为(num_classes,)
self.params['b3'] = np.zeros(num_classes)
```

### 前向传播

接下来，在`loss`方法中实现前向传播过程：

```python
# 第一层：卷积 - ReLU - 池化
if self.pooling == 'maxpool':
    # 使用最大池化
    conv_relu_pool_out, conv_relu_pool_cache = conv_relu_maxpool_forward(X, W1, b1, conv_param, pool_param)
else:
    # 使用平均池化
    conv_relu_pool_out, conv_relu_pool_cache = conv_relu_avgpool_forward(X, W1, b1, conv_param, pool_param)

# 第二层：全连接 - ReLU
# 首先将卷积层的输出展平
N = X.shape[0]
conv_relu_pool_out_flat = conv_relu_pool_out.reshape(N, -1)
affine_relu_out, affine_relu_cache = affine_relu_forward(conv_relu_pool_out_flat, W2, b2)

# 第三层：全连接（输出层）
scores, affine_cache = affine_forward(affine_relu_out, W3, b3)
```

### 反向传播

最后，实现反向传播过程来计算梯度：

```python
# 计算softmax损失
loss, dscores = softmax_loss(scores, y)

# 添加L2正则化损失
loss += 0.5 * self.reg * (np.sum(W1 * W1) + np.sum(W2 * W2) + np.sum(W3 * W3))

# 反向传播
# 第三层（输出层）的反向传播
daffine_relu_out, dW3, db3 = affine_backward(dscores, affine_cache)

# 第二层（全连接-ReLU）的反向传播
dconv_relu_pool_out_flat, dW2, db2 = affine_relu_backward(daffine_relu_out, affine_relu_cache)

# 将梯度重塑回卷积层输出的形状
dconv_relu_pool_out = dconv_relu_pool_out_flat.reshape(conv_relu_pool_out.shape)

# 第一层（卷积-ReLU-池化）的反向传播
if self.pooling == 'maxpool':
    # 使用最大池化的反向传播
    dx, dW1, db1 = conv_relu_maxpool_backward(dconv_relu_pool_out, conv_relu_pool_cache)
else:
    # 使用平均池化的反向传播
    dx, dW1, db1 = conv_relu_avgpool_backward(dconv_relu_pool_out, conv_relu_pool_cache)

# 添加L2正则化的梯度
dW1 += self.reg * W1
dW2 += self.reg * W2
dW3 += self.reg * W3

# 存储梯度
grads['W1'] = dW1
grads['b1'] = db1
grads['W2'] = dW2
grads['b2'] = db2
grads['W3'] = dW3
grads['b3'] = db3
```


### 部分运行结果截图
绘制损失、训练准确率和验证准确率应该会显示出明显的过拟合：
![alt text](image/image-31.png)

通过将三层卷积网络训练一个轮次，你应该在训练集上达到超过40%的准确率。

![alt text](image/image-32.png)
您可以通过运行以下代码来可视化已训练网络的第一层卷积滤波器：
![alt text](image/image-33.png)

## 实现 spatial batch normalization（5分）

### 前向传播

```python
    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 将输入数据重塑为适合层归一化的形状
    # 对于空间层归一化，需要对每个样本的C、H、W维度进行归一化
    # 因此将C、H、W维度合并，使得每个样本N有一个特征向量
    x_reshaped = x.reshape(N, -1)  # 形状变为 (N, C*H*W)

    # 创建适合层归一化的gamma和beta参数
    # 需要将gamma和beta从形状(C, H, W)扩展为形状(C*H*W,)，以便在layernorm_forward中正确广播
    gamma_reshaped = gamma.reshape(-1)  # 形状变为 (C*H*W,)
    beta_reshaped = beta.reshape(-1)  # 形状变为 (C*H*W,)

    # 调用普通的层归一化函数
    out_reshaped, cache = layernorm_forward(x_reshaped, gamma_reshaped, beta_reshaped, ln_param)

    # 将输出重塑回原始形状
    out = out_reshaped.reshape(N, C, H, W)
```
![alt text](image/image-34.png)
![alt text](image/image-35.png)
### 反向传播

```python
    # 获取输入数据的维度
    N, C, H, W = dout.shape

    # 将上游梯度重塑为与前向传播中相同的形状
    dout_reshaped = dout.reshape(N, -1)  # 形状变为 (N, C*H*W)

    # 调用普通的层归一化反向传播函数
    dx_reshaped, dgamma_reshaped, dbeta_reshaped = layernorm_backward(dout_reshaped, cache)

    # 将梯度重塑回原始形状
    dx = dx_reshaped.reshape(N, C, H, W)
    dgamma = dgamma_reshaped.reshape(C, H, W)
    dbeta = dbeta_reshaped.reshape(C, H, W)
```

![alt text](image/image-36.png)

## 实现 spatial layer normalization（4分）


### 前向传播

```python
    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 将输入数据重塑为适合层归一化的形状
    # 对于空间层归一化，需要对每个样本的C、H、W维度进行归一化
    # 因此将C、H、W维度合并，使得每个样本N有一个特征向量
    x_reshaped = x.reshape(N, -1)  # 形状变为 (N, C*H*W)

    # 创建适合层归一化的gamma和beta参数
    # 需要将gamma和beta从形状(N,)扩展为形状(N, 1)，以便在layernorm_forward中正确广播
    gamma_reshaped = gamma.reshape(N, 1)
    beta_reshaped = beta.reshape(N, 1)

    # 调用普通的层归一化函数
    out_reshaped, cache = layernorm_forward(x_reshaped, gamma_reshaped, beta_reshaped, ln_param)

    # 将输出重塑回原始形状
    out = out_reshaped.reshape(N, C, H, W)
```
![alt text](image/image-37.png)
### 反向传播

```python
    # 获取输入数据的维度
    N, C, H, W = dout.shape

    # 从缓存中获取前向传播的中间值
    x, x_normalized, mean, var, gamma, beta, eps = cache

    # 将上游梯度重塑为与前向传播中相同的形状
    dout_reshaped = dout.reshape(N, -1)  # 形状变为 (N, C*H*W)

    # 直接计算标准化数据的梯度
    dx_normalized = dout_reshaped * gamma  # (N, C*H*W)

    # 计算关于gamma的梯度 - 每个样本的标准化值与上游梯度的乘积求和
    dgamma = np.sum(dout_reshaped * x_normalized, axis=1)  # (N,)

    # 计算关于beta的梯度 - 每个样本的上游梯度求和
    dbeta = np.sum(dout_reshaped, axis=1)  # (N,)

    # 以下计算关于输入x的梯度
    # 标准差的倒数
    std_inv = 1.0 / np.sqrt(var + eps)  # (N, 1)

    # 计算关于方差的梯度
    dvar = np.sum(dx_normalized * (x - mean) * (-0.5) * std_inv**3, axis=1, keepdims=True)  # (N, 1)

    # 计算关于均值的梯度
    D = C * H * W  # 特征维度的总数
    dmean = np.sum(dx_normalized * (-std_inv), axis=1, keepdims=True) + \
            dvar * np.sum(-2 * (x - mean), axis=1, keepdims=True) / D  # (N, 1)

    # 计算关于输入x的梯度
    dx_reshaped = dx_normalized * std_inv + dvar * 2 * (x - mean) / D + dmean / D  # (N, C*H*W)

    # 将输入梯度重塑回原始形状
    dx = dx_reshaped.reshape(N, C, H, W)
```
![alt text](image/image-38.png)
## 实现 instance normalization（3分）


### 前向传播

```python
    # 获取输入数据的维度
    N, C, H, W = x.shape

    # 获取数值稳定性参数
    eps = in_param.get('eps', 1e-5)

    # 对每个样本的每个通道独立地计算均值和方差（在H和W维度上）
    # 重塑x以便于计算：(N*C, H*W)
    x_reshaped = x.reshape(N * C, -1)

    # 计算每个通道的均值和方差
    mean = np.mean(x_reshaped, axis=1, keepdims=True)  # (N*C, 1)
    var = np.var(x_reshaped, axis=1, keepdims=True)    # (N*C, 1)

    # 标准化
    x_norm = (x_reshaped - mean) / np.sqrt(var + eps)  # (N*C, H*W)

    # 将标准化后的数据重塑回原始形状
    x_norm = x_norm.reshape(N, C, H, W)

    # 应用缩放和平移参数
    # 注意：gamma和beta的形状是(N, C)，需要广播到(N, C, H, W)
    gamma_expanded = gamma.reshape(N, C, 1, 1)
    beta_expanded = beta.reshape(N, C, 1, 1)

    out = gamma_expanded * x_norm + beta_expanded

    # 存储反向传播所需的中间值
    cache = (x, x_norm, mean, var, gamma_expanded, beta_expanded, eps, N, C, H, W)

```
![alt text](image/image-39.png)
### 反向传播

```python
   # 从缓存中获取前向传播的中间值
    x, x_norm, mean, var, gamma_expanded, beta_expanded, eps, N, C, H, W = cache

    # 计算关于beta的梯度 - 求和得到每个(N,C)对的梯度
    dbeta = np.sum(dout, axis=(2, 3))  # (N, C)

    # 计算关于gamma的梯度 - 每个(N,C)对的标准化值与上游梯度的乘积求和
    dgamma = np.sum(dout * x_norm, axis=(2, 3))  # (N, C)

    # 重塑以便于计算
    dout_reshaped = dout.reshape(N * C, -1)  # (N*C, H*W)
    x_reshaped = x.reshape(N * C, -1)  # (N*C, H*W)
    x_norm_reshaped = x_norm.reshape(N * C, -1)  # (N*C, H*W)
    gamma_reshaped = gamma_expanded.reshape(N * C, -1)  # (N*C, 1)

    # 计算关于标准化输入的梯度
    dx_norm_reshaped = dout_reshaped * gamma_reshaped  # (N*C, H*W)

    # 标准差的倒数
    std_inv = 1.0 / np.sqrt(var + eps)  # (N*C, 1)

    # 计算关于方差的梯度
    dvar = np.sum(dx_norm_reshaped * (x_reshaped - mean) * (-0.5) * std_inv**3, axis=1, keepdims=True)  # (N*C, 1)

    # 计算关于均值的梯度
    HW = H * W  # 空间维度的总数
    dmean = np.sum(dx_norm_reshaped * (-std_inv), axis=1, keepdims=True) + \
            dvar * np.sum(-2 * (x_reshaped - mean), axis=1, keepdims=True) / HW  # (N*C, 1)

    # 计算关于输入x的梯度
    dx_reshaped = dx_norm_reshaped * std_inv + \
                 dvar * 2 * (x_reshaped - mean) / HW + \
                 dmean / HW  # (N*C, H*W)

    # 将输入梯度重塑回原始形状
    dx = dx_reshaped.reshape(N, C, H, W)
```
![alt text](image/image-40.png)
# 实现 ConvNet（35分）

根据 `ConvolutionalNetwork.ipynb` 中 `Train your best model` 的要求，利用 `annp` 文件夹中的模块实现用于分类 CIFAR-10 数据集的卷积神经网络。需要注意的是，只能用 `annp` 文件夹中的模块实现你的模型，不允许使用额外的深度学习框架。请在 `annp/classifiers/cnn.py` 中实现你的模型，在 Jupyter Notebook 对应位置实现你的训练过程、实验结果以及可视化分析。请各位同学仔细阅读 `annp` 文件夹中每个模块的用法。

整理你实现的 ConvNet，写一份实验报告描述你的模型架构、调参的过程、分析实验结果以及不同的参数对实验结果的影响，最好是对实验结果进行可视化的分析。实验报告占 20 分。

## 网络结构代码

### 结构图

<img src="./frame.svg\" style="height:500px">

### 类定义与初始化参数
```python
class DeepConvNet(object):
    def __init__(self,
                 input_dim=(3, 32, 32),
                 num_filters1=32,
                 filter_size1=3,
                 num_filters2=64,
                 filter_size2=3,
                 hidden_dim=100,
                 num_classes=10,
                 dropout=0.5,
                 weight_scale=1e-3,
                 reg=0.0,
                 dtype=np.float32):
        self.params = {}  # 存储所有可学习参数
        self.reg = reg  # L2正则化强度
        self.dtype = dtype  # 数据类型
        self.use_dropout = dropout < 1  # 是否启用Dropout
        self.dropout_param = {'mode': 'train', 'p': dropout}  # Dropout参数
        C, H, W = input_dim  # 输入数据维度：通道数、高度、宽度
```
网络接收CIFAR-10的输入尺寸`(3, 32, 32)`，支持自定义卷积层滤波器数量、全连接层维度、Dropout概率等参数。`use_dropout`根据`dropout`参数判断是否启用正则化，避免过拟合。


### 第一层卷积层（Conv1）
```python
        # 第一层卷积：3x3滤波器，"same"填充保持尺寸不变
        self.params['W1'] = np.random.normal(0.0, weight_scale,  # 权重初始化
                                             (num_filters1, C, filter_size1, filter_size1))
        self.params['b1'] = np.zeros(num_filters1)  # 偏置初始化为0
```
权重形状为`(num_filters1, 输入通道, 滤波器高, 滤波器宽)`，本例中输入通道为3（RGB），滤波器数量为32，生成32个特征图。使用正态分布初始化权重，偏置全零初始化。


### 第二层卷积层（Conv2）
```python
        # 第二层卷积：3x3滤波器，输入通道为第一层输出通道（num_filters1）
        self.params['W2'] = np.random.normal(0.0, weight_scale,
                                             (num_filters2, num_filters1, filter_size2, filter_size2))
        self.params['b2'] = np.zeros(num_filters2)

        # 池化后尺寸：32x32 -> 16x16（2x2最大池化，stride=2）
        self.pooled_height = H // 2
        self.pooled_width = W // 2
        fc_input_dim = num_filters2 * self.pooled_height * self.pooled_width  # 全连接层输入维度
```
第二层卷积的输入通道为第一层的输出通道（32），输出通道为64，进一步提取高层特征。池化层将空间尺寸减半，计算全连接层输入维度时需展平所有特征图。


### 全连接层（FC1 & FC2）
```python
        # 第一层全连接：隐藏层，含ReLU激活
        self.params['W3'] = np.random.normal(0.0, weight_scale, (fc_input_dim, hidden_dim))
        self.params['b3'] = np.zeros(hidden_dim)

        # 第二层全连接：输出层，直接映射到类别数
        self.params['W4'] = np.random.normal(0.0, weight_scale, (hidden_dim, num_classes))
        self.params['b4'] = np.zeros(num_classes)

        # 转换数据类型
        for k, v in self.params.items():
            self.params[k] = v.astype(dtype)
```
`FC1`将卷积层输出展平后映射到`hidden_dim`（默认500），引入非线性变换。`FC2`为输出层，直接输出10类的分类分数，无需激活函数。


### 前向传播（损失函数）
```python
    def loss(self, X, y=None):
        X = X.astype(self.dtype)
        mode = 'test' if y is None else 'train'
        if self.use_dropout:
            self.dropout_param['mode'] = mode  # 训练/测试模式切换

        # 卷积参数："same"填充（pad=(filter_size-1)//2）
        conv_param1 = {'stride': 1, 'pad': (self.params['W1'].shape[2] - 1) // 2}
        conv_param2 = {'stride': 1, 'pad': (self.params['W2'].shape[2] - 1) // 2}
        pool_param = {'pool_height': 2, 'pool_width': 2, 'stride': 2}  # 最大池化参数
```
根据输入数据类型转换格式，区分训练/测试模式（Dropout行为不同）。自动计算卷积层的填充大小，确保输入输出空间尺寸不变（除池化层）。


### 卷积-激活-池化流程
```python
        # 第一层：Conv1 + ReLU
        out1, cache1 = conv_relu_forward(X, self.params['W1'], self.params['b1'], conv_param1)

        # 第二层：Conv2 + ReLU + MaxPool
        out2, cache2 = conv_relu_maxpool_forward(out1, self.params['W2'], self.params['b2'], conv_param2, pool_param)

        # Dropout（训练模式下激活）
        if self.use_dropout:
            out2, dropout_cache = dropout_forward(out2, self.dropout_param)
            caches['dropout'] = dropout_cache  # 缓存Dropout状态
```
`conv_relu_forward`组合卷积和ReLU激活，输出32个特征图（尺寸32x32）。`conv_relu_maxpool_forward`进一步卷积、激活并池化，输出64个特征图（尺寸16x16）。Dropout随机失活神经元，减少过拟合，仅在训练时生效。


### 全连接层流程
```python
        # 展平池化输出：(N, 64, 16, 16) -> (N, 64*16*16=16384)
        N = X.shape[0]
        out2_flat = out2.reshape(N, -1)

        # 第三层：FC1 + ReLU
        out3, cache3 = affine_relu_forward(out2_flat, self.params['W3'], self.params['b3'])

        # 第四层：FC2（输出层）
        scores, cache4 = affine_forward(out3, self.params['W4'], self.params['b4'])
```
池化后的特征图展平为一维向量，作为全连接层输入。`affine_relu_forward`组合全连接和ReLU，引入非线性；输出层仅全连接，生成分类分数。


### 反向传播与梯度计算
```python
        # 计算Softmax损失和正则化项
        loss, dscores = softmax_loss(scores, y)
        for i in range(1, 5):  # 对所有W参数添加L2正则化
            W = self.params[f'W{i}']
            loss += 0.5 * self.reg * np.sum(W * W)

        # 反向传播：从输出层到输入层
        # 第四层（FC2）梯度
        dout3, dW4, db4 = affine_backward(dscores, caches['fc2'])
        grads['W4'] = dW4 + self.reg * self.params['W4']  # 正则化梯度
        grads['b4'] = db4

        # 第三层（FC1）梯度
        dout2_flat, dW3, db3 = affine_relu_backward(dout3, caches['fc1'])
        grads['W3'] = dW3 + self.reg * self.params['W3']
        grads['b3'] = db3
```
损失函数包含Softmax交叉熵和L2正则化，防止过拟合。反向传播按层反向计算梯度，正则化项通过`reg * W`叠加到权重梯度。


### 卷积层与池化层梯度
```python
        # 恢复池化输出形状：(N, 16384) -> (N, 64, 16, 16)
        dout2 = dout2_flat.reshape(N, self.params['W2'].shape[0], self.pooled_height, self.pooled_width)

        # Dropout反向传播（训练模式）
        if self.use_dropout:
            dout2 = dropout_backward(dout2, caches['dropout'])

        # 第二层（Conv2+Pool）梯度
        dout1, dW2, db2 = conv_relu_maxpool_backward(dout2, caches['conv2'])
        grads['W2'] = dW2 + self.reg * self.params['W2']
        grads['b2'] = db2

        # 第一层（Conv1）梯度
        dx, dW1, db1 = conv_relu_backward(dout1, caches['conv1'])
        grads['W1'] = dW1 + self.reg * self.params['W1']
        grads['b1'] = db1
```
池化层反向传播时，需将全连接层梯度恢复为特征图形状。`conv_relu_maxpool_backward`组合卷积、ReLU、池化的反向传播，计算滤波器和偏置梯度。


## 训练代码
### 数据加载与预处理
```python
from annp.data_utils import get_CIFAR10_data
data = get_CIFAR10_data()  # 加载CIFAR-10数据集
```


### 模型参数配置
```python
model = DeepConvNet(
    input_dim=(3, 32, 32),
    num_filters1=32,       # 第一层32个3x3滤波器
    num_filters2=64,       # 第二层64个3x3滤波器
    hidden_dim=500,        # 全连接隐藏层500神经元
    dropout=0.5,           # Dropout丢弃50%神经元
    weight_scale=1e-2,     # 权重初始化尺度（增大以增强梯度）
    reg=1e-3               # L2正则化强度（控制过拟合）
)
```
`weight_scale=1e-2`：较原始1e-3更大，避免浅层梯度消失，适合深层网络。`reg=1e-3`：平衡模型复杂度，防止过拟合，验证集准确率最优时的参数。


### 训练参数配置（Solver）
```python
from annp.solver import Solver

solver_kwargs = {
    'update_rule': 'adam',       # 自适应学习率优化器
    'optim_config': {'learning_rate': 1e-3},  # 初始学习率
    'lr_decay': 0.95,            # 每epoch学习率衰减5%，防止后期震荡
    'num_epochs': 8,             # 训练8个epoch（平衡时间与性能）
    'batch_size': 300,           # 批量大小，较大batch提升稳定性
    'print_every': 20,           # 每20次迭代打印进度
    'verbose': True
}
```
`Adam`优化器结合RMSprop和动量，收敛速度快于SGD。学习率衰减防止后期学习率过高导致震荡，提升收敛精度。


### 模型训练与可视化
```python
solver = Solver(model, data, **solver_kwargs)
solver.train()  # 启动训练

# 绘制损失曲线与准确率
plt.figure(figsize=(15, 12))
plt.subplot(2, 1, 1)
plt.title('Training Loss')
plt.plot(solver.loss_history, 'o-', label='Loss')
plt.xlabel('Iteration')
plt.ylabel('Loss')

plt.subplot(2, 1, 2)
plt.title('Accuracy')
plt.plot(solver.train_acc_history, '-o', label='Train')
plt.plot(solver.val_acc_history, '-o', label='Validation')
plt.axhline(y=0.6, color='r', linestyle='--', label='60% Threshold')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.legend()
plt.show()
```


### 测试集评估
```python
test_acc = solver.check_accuracy(data['X_test'], data['y_test'])
print(f'Test Accuracy: {test_acc * 100:.2f}%')  # 输出：Test Accuracy: 64.89%
```

## 结果展示

![alt text](image/image-41.png)


![alt text](image/image-42.png)



在训练过程中，训练损失随着迭代推进显著下降，反映出模型对数据模式的有效学习。

从准确率图表来看，训练准确率持续上升，在第二个epoch就已经测试集和验证集合准确率都超过了60%，而验证准确率呈现先升后降的趋势，这表明模型可能出现了一定程度的过拟合，即在训练数据上表现良好，但在验证数据上的泛化能力有所减弱。