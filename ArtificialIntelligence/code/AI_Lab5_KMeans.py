# 导入必要的库
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs
 
# 生成模拟数据
n_samples = 300
n_features = 2
n_clusters = 3
 
# 使用make_blobs生成聚类数据
X, y = make_blobs(n_samples=n_samples, n_features=n_features, centers=n_clusters, random_state=42)
 
# 使用KMeans算法进行聚类
kmeans = KMeans(n_clusters=n_clusters)
kmeans.fit(X)
 
# 获取聚类结果和聚类中心
labels = kmeans.labels_
centers = kmeans.cluster_centers_
 
# 绘制原始数据和聚类结果
plt.scatter(X[:, 0], X[:, 1], c=labels, cmap='viridis', edgecolor='k')
plt.scatter(centers[:, 0], centers[:, 1], c='red', marker='X', s=200, label='Centroids')
plt.title('K-means Clustering')
plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.legend()
plt.show()