Project: 聚类算法对比实验

运行说明：

1. 创建/激活虚拟环境并安装依赖（已在 `pyproject.toml` 中列出）：

```bash
cd /workspace/homework/Large_Data/homework3/code
uv sync
source .venv/bin/activate
```

2. 运行实验：

```bash
python main.py
```

输出：
- `data/iris_scaled.npy` 和 `data/iris_scaled.csv`
- `results_kmeans.csv`, `results_dbscan.csv`
- `plots/` 下的若干 PNG 图

注意：核心算法（KMeans、DBSCAN）均为手写实现，评估指标（Accuracy、Silhouette、Calinski-Harabasz）亦为手写计算，满足作业要求。
