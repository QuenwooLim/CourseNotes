import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler, MinMaxScaler
import matplotlib.pyplot as plt

# 激活函数
def sigmoid(Z):
    A = 1 / (1 + np.exp(-Z))
    return A, Z # 返回用于反向传播的缓存值Z

def relu(Z):
    A = np.maximum(0, Z)  # ReLU激活函数
    return A, Z # 返回用于反向传播的缓存值Z


# 初始化参数
def Init_Parameters(layers_dims):
    parameters = {}
    for i in range(1, len(layers_dims)):
        parameters["W" + str(i)] = np.random.randn(layers_dims[i], layers_dims[i - 1]) * np.sqrt(2 / layers_dims[i - 1])  #初始化 防止梯度
        parameters["b" + str(i)] = np.zeros((layers_dims[i], 1))  # 偏置初始化为零
    return parameters

# 参数更新
def update_parameters(parameters, grads, learning_rate):
    L = len(parameters) // 2
    for l in range(L):
        parameters["W" + str(l + 1)] -= learning_rate * grads["dW" + str(l + 1)]
        parameters["b" + str(l + 1)] -= learning_rate * grads["db" + str(l + 1)]
    return parameters

# 前向传播
def linear_forward(A, W, b):
    Z = np.dot(W, A) + b  # 线性计算
    cache = (A, W, b)  # 缓存值包含前一层的激活值A、权重W和偏置b
    return Z, cache


def linear_activation_forward(A_prev, W, b, activation):
    if activation == "sigmoid":
        Z, linear_cache = linear_forward(A_prev, W, b)
        A, activation_cache = sigmoid(Z)
    elif activation == "relu":
        Z, linear_cache = linear_forward(A_prev, W, b)
        A, activation_cache = relu(Z)
    cache = (linear_cache, activation_cache)
    return A, cache


def Model_Forward(X, parameters):
    caches = []
    A = X
    for i in range(1, len(parameters) // 2):
        A_prev = A
        A, cache = linear_activation_forward(A_prev, parameters["W" + str(i)], parameters["b" + str(i)], "relu")
        caches.append(cache)
    AL, cache = linear_activation_forward(A, parameters["W" + str(len(parameters) // 2)], parameters["b" + str(len(parameters) // 2)], "sigmoid")
    caches.append(cache)
    return AL, caches


#反向传播
def sigmoid_backward(dA, cache):
    Z = cache
    s = 1 / (1 + np.exp(-Z))
    dZ = dA * s * (1 - s)
    return dZ

def relu_backward(dA, cache):
    Z = cache
    dZ = np.array(dA, copy=True)
    dZ[Z <= 0] = 0
    return dZ

def linear_backward(dZ, cache):
    A_prev, W, b = cache
    m = A_prev.shape[1]
    dW = np.dot(dZ, A_prev.T) / m
    db = np.sum(dZ, axis=1, keepdims=True) / m
    dA_prev = np.dot(W.T, dZ)
    return dA_prev, dW, db


def linear_activation_backward(dA, cache, activation="relu"):
    linear_cache, activation_cache = cache
    if activation == "relu":
        dZ = relu_backward(dA, activation_cache)
    elif activation == "sigmoid":
        dZ = sigmoid_backward(dA, activation_cache)
    dA_prev, dW, db = linear_backward(dZ, linear_cache)
    return dA_prev, dW, db


def Model_Backward(AL, Y, caches):
    grads = {}
    L = len(caches)
    Y = Y.reshape(AL.shape)
    dAL = (AL - Y)  # 均方误差的梯度
    current_cache = caches[L - 1]
    grads["dA" + str(L - 1)], grads["dW" + str(L)], grads["db" + str(L)] = linear_activation_backward(dAL, current_cache, "sigmoid")
    for l in reversed(range(L - 1)):
        current_cache = caches[l]
        dA_prev_temp, dW_temp, db_temp = linear_activation_backward(grads["dA" + str(l + 1)], current_cache, "relu")
        grads["dA" + str(l)] = dA_prev_temp
        grads["dW" + str(l + 1)] = dW_temp
        grads["db" + str(l + 1)] = db_temp
    return grads


#MLP模型训练
def MLP_Model(X, Y, layers_dims, learning_rate, Iterators_nums):
    parameters = Init_Parameters(layers_dims) #初始化参数
    AL_list = [] #预测值列表
    for i in range(Iterators_nums):
        AL, caches = Model_Forward(X, parameters) #向前传播
        AL_list.append(AL) #记录预测值
        grads = Model_Backward(AL, Y, caches) #向后传播
        parameters = update_parameters(parameters, grads, learning_rate) #更新参数
    return parameters,AL_list #返回训练好的参数和预测值


#数据读取
def read_data(filename, samples_num):
    data = pd.read_csv(filename, sep=",", header=0)
    title = list(data.columns)
    data = data[title]
    data = data[0:samples_num]
    X = data.iloc[:, :-1].values
    Y = data.iloc[:, -1].values
    return X, Y


#主程序
if __name__ == "__main__":
    #读取数据
    X, Y = read_data('MLP_data.csv', 10000)

    #数据预处理、归一化
    features_num = X.shape[1]
    scaler = StandardScaler()
    X = scaler.fit_transform(X).T
    scaler_Y = MinMaxScaler(feature_range=(0, 1))
    Y = scaler_Y.fit_transform(Y.reshape(-1, 1)).T

    #模型训练
    layers_dims = [features_num,20,7,5,1] #感知机层数3和每层神经元个数20、7、5
    parameters, AL_list= MLP_Model(X, Y, layers_dims, learning_rate=1, Iterators_nums=10000)

    # 反归一化 AL_list 并计算损失
    loss_list = []
    dif_list = []
    correct_nums=0
    for AL in AL_list:
        price_pre = scaler_Y.inverse_transform(AL.T).T  #反归一化
        price_real= scaler_Y.inverse_transform(Y)
        loss = np.mean((price_pre-price_real)**2) #均方误差损失
        dif= np.mean(np.abs(price_pre-price_real))
        correct_nums = np.sum(np.abs(price_pre - price_real) < 10000)
        dif_list.append(dif)
        loss_list.append(loss)

    # 绘制损失曲线
    plt.plot(loss_list)
    plt.ylabel('Loss')
    plt.xlabel('Iterations')
    plt.title('Loss curve')
    plt.show()
    # 绘制损失曲线
    plt.plot(dif_list)
    plt.ylabel('Dif')
    plt.xlabel('Iterations')
    plt.title('Dif curve')
    plt.show()
    # 准确率
    print("预测的准确率：%.4f%%" % (correct_nums*100 / Y.shape[1]))