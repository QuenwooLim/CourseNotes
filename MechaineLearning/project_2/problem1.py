import time

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from numpy import nonzero, array
from sklearn.decomposition import PCA
from sklearn.metrics import silhouette_score
from sklearn.datasets import make_blobs

class MyKmeans:
    def __init__(self, k=4, T=200, epsilon=1e-7, X_main=None, y_main=None):
        self.k = k
        self.T = T
        self.epsilon = epsilon
        self.X_main = X_main
        self.y_main = y_main
        self.labels = None
        self.centers = None

    #从数据集中随机选择k个点作为初始质心
    def initialize_centroids(self, data, k):
        centers = data[np.random.choice(data.shape[0], k, replace=False)]
        return centers

    #计算数据点与质心之间的距离，并将数据点分配给最近的质心
    def get_clusters(self, data, centroids):
        distances = np.linalg.norm(data[:, np.newaxis] - centroids, axis=2)
        cluster_labels = np.argmin(distances, axis=1)
        return cluster_labels

    #计算每个簇的新质心，即簇内数据点的均值
    def update_centroids(self, data, cluster_labels, k):
        new_centroids = np.array([data[cluster_labels == i].mean(axis=0) for i in range(k)])
        return new_centroids


    def k_means(self):
        start = time.time()  #开始时间，计时
        #初始化质心
        centroids = self.initialize_centroids(self.X_main, self.k)
        t = 1  # 迭代次数
        while t <= self.T:
            #分配簇
            cluster_labels = self.get_clusters(self.X_main, centroids)

            #更新质心
            new_centroids = self.update_centroids(self.X_main, cluster_labels, self.k)

            #检查收敛条件
            if np.linalg.norm(new_centroids - centroids) < self.epsilon:
                break
            centroids = new_centroids
            print("第", t, "次迭代")
            t += 1
        print("用时：{0}".format(time.time() - start))
        self.labels = cluster_labels
        self.centers = centroids
        return cluster_labels, centroids

    def calculate_sse(self):
        sse = 0.0
        for i in range(len(self.centers)):
            # 获取属于第 i 个簇的所有数据点
            cluster_points = self.X_main[self.labels == i]
            # 计算这些点到簇中心的距离平方和
            sse += np.sum((cluster_points - self.centers[i]) ** 2)
        return sse

    def draw_cluster(self):
        dataset = array(self.X_main)
        center_array = array(self.centers)
        
        # 根据数据维度选择绘图方式
        if dataset.shape[1] >= 3:
            # --- 绘制三维图 ---
            if dataset.shape[1] > 3:
                pca = PCA(n_components=3)
                dataset = pca.fit_transform(dataset)  # 如果属性数量大于3，降维
                center_array = pca.transform(center_array)
                print("PCA 解释方差比:", pca.explained_variance_ratio_)

            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')
            
            label = array(self.labels)
            colors = np.array(
                ["#FF0000", "#0000FF", "#00FF00", "#FFFF00", "#00FFFF", "#FF00FF", "#800000", "#008000", "#000080", "#808000",
                 "#800080", "#008080", "#444444", "#FFD700", "#008080"])
            
            for i in range(self.k):
                points = dataset[nonzero(label == i)]
                ax.scatter(points[:, 0], points[:, 1], points[:, 2], c=colors[i], s=7, marker='o')

            ax.scatter(center_array[:, 0], center_array[:, 1], center_array[:, 2], marker='x', color='m', s=50, label="Centers")
            ax.set_xlabel('Dimension 1')
            ax.set_ylabel('Dimension 2')
            ax.set_zlabel('Dimension 3')
            ax.legend()
            plt.show()

        elif dataset.shape[1] == 2:
            # --- 绘制二维图 ---
            fig, ax = plt.subplots()
            
            label = array(self.labels)
            colors = np.array(
                ["#FF0000", "#0000FF", "#00FF00", "#FFFF00", "#00FFFF", "#FF00FF", "#800000", "#008000", "#000080", "#808000",
                 "#800080", "#008080", "#444444", "#FFD700", "#008080"])

            for i in range(self.k):
                points = dataset[nonzero(label == i)]
                ax.scatter(points[:, 0], points[:, 1], c=colors[i], s=7, marker='o')

            ax.scatter(center_array[:, 0], center_array[:, 1], marker='x', color='m', s=50, label="Centers")
            ax.set_xlabel('Dimension 1')
            ax.set_ylabel('Dimension 2')
            ax.legend()
            plt.show()
        
        else:
            print("数据维度为1，无法绘制散点图。")

    def draw_comparison(self):
        if self.y_main is None or self.X_main.shape[1] != 2:
            print("无法生成对比图：需要二维数据和真实标签。")
            return

        fig, axes = plt.subplots(1, 2, figsize=(12, 5))
        colors = np.array(
            ["#FF0000", "#0000FF", "#00FF00", "#FFFF00", "#00FFFF", "#FF00FF", "#800000", "#008000", "#000080",
             "#808000", "#800080", "#008080", "#444444", "#FFD700", "#008080"])

        # --- 绘制实际标签图 ---
        axes[0].scatter(self.X_main[:, 0], self.X_main[:, 1], c=colors[self.y_main], s=7)
        axes[0].set_title('Actual Clusters')
        axes[0].set_xlabel('Dimension 1')
        axes[0].set_ylabel('Dimension 2')
        axes[0].legend()

        # --- 绘制预测标签图 ---
        label = array(self.labels)
        for i in range(self.k):
            points = self.X_main[nonzero(label == i)]
            axes[1].scatter(points[:, 0], points[:, 1], c=colors[i], s=7, marker='o')

        center_array = array(self.centers)
        axes[1].scatter(center_array[:, 0], center_array[:, 1], marker='x', color='m', s=50, label="Centers")
        axes[1].set_title('K-Means Predicted Clusters')
        axes[1].set_xlabel('Dimension 1')
        axes[1].set_ylabel('Dimension 2')
        axes[1].legend()

        plt.show()

if __name__ == "__main__":
    # 步骤一：在作业数据集上测试 K-Means 算法
    X=[ [5.9,3.2],
        [4.6,2.9],
        [6.2,2.8],
        [4.7,3.2],
        [5.5,4.2],
        [5.0,3.0],
        [4.9,3.1],
        [6.7,3.1],
        [5.1,3.8],
        [6.0,3.0]
        ]
    kmeans_demo = MyKmeans(k=3, T=1000, epsilon=1e-7, X_main=np.array(X))
    labels, centers = kmeans_demo.k_means()
    print(f"轮廓系数 (Silhouette Coefficient): {silhouette_score(np.array(kmeans_demo.X_main), kmeans_demo.labels):.4f}") # 接近1聚类效果好
    print(f"平方误差和 (SSE): {kmeans_demo.calculate_sse():.4e}")
    kmeans_demo.draw_cluster()
    kmeans_demo.draw_comparison()

    # 步骤二：在 sklearn 生成复杂的数据集上测试 K-Means 算法
    n = 300 #样本数量
    k = 4  #聚类簇数
    m = 1000  #最大迭代数
    epsilon = 1e-9  #收敛阈值
    X_main, y_main = make_blobs(n_samples=n, centers=k, cluster_std=0.7, random_state=42) # 数据集
    kmeans = MyKmeans(k=k, T=m, epsilon=epsilon, X_main=X_main, y_main=y_main) # Kmeans实例化
    kmeans.k_means() # 运行Kmeans算法
    print(f"轮廓系数 (Silhouette Coefficient): {silhouette_score(np.array(kmeans.X_main), kmeans.labels):.4f}") # 接近1聚类效果好
    print(f"平方误差和 (SSE): {kmeans.calculate_sse():.4e}")
    kmeans.draw_cluster()
    kmeans.draw_comparison()