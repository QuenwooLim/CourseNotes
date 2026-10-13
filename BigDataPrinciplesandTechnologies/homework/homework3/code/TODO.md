## 聚类算法对比实验 作业 TODO  

### 0. 实验环境  
- **当前目录**: `/workspace/homework/Large_Data/homework3/`  
- **操作系统**: Linux 系统  
- **Python 环境管理工具**: 使用 `uv` 包管理器  
  - 已执行：`uv venv .venv` 创建虚拟环境  
- **项目位置**: `/workspace/homework/Large_Data/homework3/code/`  
  - [操作指引 TODO.md](/workspace/homework/Large_Data/homework3/code/TODO.md)  
- **报告位置**: `/workspace/homework/Large_Data/homework3/`  
    - abstract.tex
    - appendix1.tex
    - chap01.tex
    - main.tex

> 🔒 **重要约束**：  
> - ❌ 禁止直接调用 `sklearn.cluster.KMeans` 或 `DBSCAN` 等完整聚类接口；  
> - ✅ 必须**手写 K-means 和 DBSCAN 的核心逻辑**（初始化、迭代更新、邻域搜索、簇合并等）；  
> - 可使用 `numpy`, `pandas`, `matplotlib`, `scipy.spatial.distance` 等基础库辅助计算与可视化。


### 1. 整体流程  

#### ✅ 任务一：数据准备与预处理  
- [x] 加载鸢尾花数据集（4维特征 + 1维真实标签）  
- [x] 数据标准化（Z-score）：对每维特征做 `(x - mean) / std`  
- [x] 将预处理后数据保存至：`code/data/iris_scaled.npy` 或 `iris_scaled.csv`

#### ✅ 任务二：实现 K-means 核心算法  
- [ ] 编写函数：  
  - 输入：数据矩阵 `X`（n×d），簇数 `k`  
  - 输出：簇标签数组 `labels`（长度 n）、聚类中心 `centroids`（k×d）  
  - **核心步骤必须手写**：  
    1. 随机初始化 k 个中心点（可用 `np.random.choice` 选样本点）  
    2. 迭代：  
       - 计算每个样本到各中心的欧氏距离（用 `np.linalg.norm` 或手写）  
       - 分配最近中心 → 更新 `labels`  
       - 重新计算每个簇的均值作为新中心  
       - 若中心变化 < `tol` 或达 `max_iters`，终止  
- [ ] 支持超参数调整：`k` 值（如 2, 3, 4, 5），记录不同 `k` 下的聚类结果

#### ✅ 任务三：实现 DBSCAN 核心算法  
- [ ] 编写函数：  
  - 输入：数据矩阵 `X`，邻域半径 `eps`，最小点数 `min_samples`  
  - 输出：簇标签数组 `labels`（-1 表示噪声点）  
  - **核心步骤必须手写**：  
    1. 计算所有点对间距离矩阵（可优化为按需计算）  
    2. 对每个未访问点：  
       - 找出其 `eps` 邻域内所有点（含自身）  
       - 若 ≥ `min_samples` → 创建新簇，递归扩展（用栈/队列模拟）  
       - 否则标记为噪声（临时）  
    3. 处理边界点归属（若邻域中存在已分配簇的点，则加入该簇）  
- [ ] 支持超参数调整：`eps`（如 0.3, 0.5, 0.8, 1.0）、`min_samples`（如 3, 5, 7）

#### ✅ 任务四：性能评估与结果分析  
- [ ] 实现三种评估指标函数（**手写，不可调用 sklearn.metrics**）：  
  - **准确率 (Accuracy)**：基于真实标签 `y` 计算（需先用匈牙利算法对齐簇标签与真实类，或采用最大匹配策略）  
    > 提示：可写 `align_labels(pred, true)` 函数，穷举所有标签映射取最大匹配数  
  - **轮廓系数 (Silhouette Coefficient)**：  
    ```python
    s(i) = (b(i) - a(i)) / max(a(i), b(i))
    其中 a(i)=同簇平均距离，b(i)=最近异簇平均距离
    ```
  - **Calinski-Harabasz 指数 (CH Index)**：  
    ```
    CH = (between-cluster dispersion / within-cluster dispersion) * ((n_samples - k) / (k - 1))
    ```
    - between = 各簇中心到全局中心的加权平方距离和  
    - within = 各簇内点到簇中心的平方距离和  
- [ ] 对每组参数组合（K-means: k∈{2,3,4,5}；DBSCAN: eps∈{0.3,0.5,0.8}, min_samples∈{3,5}），计算并记录三项指标  
- [ ] 生成对比表格与折线图（如：k vs 轮廓系数；eps vs 噪声点比例）

#### ✅ 任务五：可视化与报告支撑  
- [ ] 降维可视化（仅用于报告展示，**不参与算法逻辑**）：  
  - 使用 PCA 将 4D 数据降至 2D（手写 PCA 或用 `numpy.linalg.svd`）  
  - 绘制：  
    - 真实标签分布图  
    - K-means（k=3）聚类结果图  
    - DBSCAN（最优参数）聚类结果图（标出噪声点）  


#### ✅ 任务六：依赖管理与打包  
- [ ] 在 `pyproject.toml` 中添加依赖（**仅基础库**）：  
  ```toml
  [project.dependencies]
  numpy = "^1.26.0"
  pandas = "^2.2.0"
  matplotlib = "^3.8.0"
  scipy = "^1.12.0"
  ```
- [ ] 执行安装：  
  ```bash
  uv sync
  ```

#### ✅ 运行指令

```bash
cd /workspace/homework/Large_Data/homework3/code/
source .venv/bin/activate
python main.py  # 运行主程序，执行所有步骤并生成结果
```

---

### 2. 报告撰写 [报告位置](/workspace/homework/Large_Data/homework3/)  
- [x] 原来的报告作为模版，使用新的内容进行替换，并注意latex格式正确性
- [x] main.tex 是主文件，包含章节引用，作为指引
- [ ] chap01.tex 是主体需要完成的部分，需要完成
  - **实验目标**：对比 K-means 与 DBSCAN 在鸢尾花数据上的表现  
  - **算法原理简述**：手写伪代码级描述（非复制教材）  
  - **实现细节**：重点说明你如何手写核心逻辑（如 DBSCAN 的递归扩展、K-means 的收敛判断）  
  - **参数调优过程**：表格展示不同参数下的指标变化（附截图）  
  - **结果分析**：  
    - 为何 K-means 在 k=3 时接近真实标签？  
    - DBSCAN 的噪声点是否合理？哪些样本被误判？  
    - 三种指标是否一致？若有冲突，解释原因  
  - **结论与反思**：两种算法适用场景、局限性、你的实现难点与优化尝试  
  - **实验数据或图表的展示**
- [ ] appendix1.tex 可用于附加代码或额外分析，需要完成
- 将需要展示的图片在报告中正确引用展示出来

✅ **最终检查清单**  
- [ ] K-means 与 DBSCAN 均为**手写核心逻辑**（无 sklearn.cluster 调用）  
- [ ] 鸢尾花数据已标准化，且仅用 `X` 聚类、`y` 仅用于评估  
- [ ] 三项评估指标（Accuracy / 轮廓系数 / CH指数）均为手写实现  
- [ ] 完成至少 2 组超参数对比实验（K-means k∈{2,3,4,5}；DBSCAN eps/min_samples 组合）  
- [ ] 报告 ≥2 页，含图表、分析、手写算法说明  
