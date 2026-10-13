import time

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from numpy import nonzero, array
from sklearn.decomposition import PCA
from sklearn.metrics import silhouette_score

# 数据保存在 .txt 文件中
iris = pd.read_csv("KMeans.csv", sep=",", header=0)  #读取数据集
df = iris.iloc[0:200, :]  # 读取前 200 行的所有列
#df = iris.iloc[0:1000, :]  # 读取前 1000 行的所有列
#df = iris.iloc[0:10000, :]  # 读取前 10000 行的所有列

columns = list(df.columns)   #获取数据集的第一行的特征名
dataset = df[columns]        #预处理之后的数据，去除掉了第一行的数据的特征名
attributes = len(df.columns) #属性数量（数据集维度）

#从数据集中随机选择k个点作为初始质心
def initialize_centroids(data, k):
    centers = data[np.random.choice(data.shape[0], k, replace=False)]
    return centers

#计算数据点与质心之间的距离，并将数据点分配给最近的质心
def get_clusters(data, centroids):
    distances = np.linalg.norm(data[:, np.newaxis] - centroids, axis=2)
    cluster_labels = np.argmin(distances, axis=1)
    return cluster_labels

#计算每个簇的新质心，即簇内数据点的均值
def update_centroids(data, cluster_labels, k):
    new_centroids = np.array([data[cluster_labels == i].mean(axis=0) for i in range(k)])
    return new_centroids


def k_means(data, k, T, epsilon):
    start = time.time()  #开始时间，计时
    #初始化质心
    centroids = initialize_centroids(data, k)
    t = 1  # 迭代次数
    while t <= T:
        #分配簇
        cluster_labels = get_clusters(data, centroids)

        #更新质心
        new_centroids = update_centroids(data, cluster_labels, k)

        #检查收敛条件
        if np.linalg.norm(new_centroids - centroids) < epsilon:
            break
        centroids = new_centroids
        print("第", t, "次迭代")
        t += 1
    print("用时：{0}".format(time.time() - start))
    return cluster_labels, centroids

def calculate_sse(data, labels, centers):
    sse = 0.0
    for i in range(len(centers)):
        # 获取属于第 i 个簇的所有数据点
        cluster_points = data[labels == i]
        # 计算这些点到簇中心的距离平方和
        sse += np.sum((cluster_points - centers[i]) ** 2)
    return sse

def draw_cluster_3d(dataset, centers, labels):
    center_array = array(centers)
    if attributes > 3:
        pca = PCA(n_components=3)
        dataset = pca.fit_transform(dataset)  #如果属性数量大于3，降维
        center_array = pca.transform(center_array)  #如果属性数量大于3，降维
        print("PCA 解释方差比:", pca.explained_variance_ratio_)
    else:
        dataset = array(dataset)

    #创建 3D 图形
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    #绘制数据点
    label = array(labels)
    colors = np.array(
        ["#FF0000", "#0000FF", "#00FF00", "#FFFF00", "#00FFFF", "#FF00FF", "#800000", "#008000", "#000080", "#808000",
            "#800080", "#008080", "#444444", "#FFD700", "#008080"])
    for i in range(k):
        ax.scatter(dataset[nonzero(label == i), 0], dataset[nonzero(label == i), 1], dataset[nonzero(label == i), 2],
                    c=colors[i], s=7, marker='o')

    #绘制聚类中心
    ax.scatter(center_array[:, 0], center_array[:, 1], center_array[:, 2], marker='x', color='m', s=50, label="Centers")
    ax.legend()
    plt.show()

if __name__ == "__main__":
    k = 8  #聚类簇数
    T = 600  #最大迭代数
    n = len(dataset)  #样本数
    epsilon = 1e-7
    #预测全部数据
    labels, centers = k_means(np.array(dataset), k, T, epsilon)
    
    #计算轮廓系数 接近1聚类效果好
    print(f"轮廓系数 (Silhouette Coefficient): {silhouette_score(np.array(dataset), labels):.4f}")

    #计算 SSE
    sse = calculate_sse(np.array(dataset), labels, centers)
    print(f"平方误差和 (SSE): {sse:.4e}")

    draw_cluster_3d(dataset, centers, labels=labels)