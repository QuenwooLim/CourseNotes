# 大数据原理与技术

本目录按资料类型整理了大数据原理与技术课程的理论课件、七次课程作业和团队项目材料。

## 📂 目录

| 内容 | 说明 |
|:---|:---|
| [理论课件](Theory/) | 大数据、数据科学、知识图谱、多模态分析、文本检测和推荐系统课件 |
| [课程作业](homework/) | 七次作业的报告、源代码、数据和实验结果 |
| [团队项目](homework/teamproject/) | 团队项目演示课件和项目要求、评分材料 |

## 📚 理论课件

[Theory](Theory/) 目录包含以下课程章节：

- [第一章：绪论](<Theory/第一章 绪论.pptx>)
- [第二章：数据科学基础理论](<Theory/第二章 数据科学基础理论.pptx>)
- [第三章：知识图谱构建与挖掘实践](<Theory/第三章 知识图谱构建与挖掘实践.pptx>)
- [第四章：文本检测实践](<Theory/第四章 文本检测实践.pptx>)
- [第五章：多模态数据分析实践](<Theory/第五章 多模态数据分析实践.pptx>)
- [第六章：推荐系统实践](<Theory/第六章 推荐系统实践.pptx>)

## ✏️ 课程作业

[homework](homework/) 目录按作业编号整理了七次课程作业。各目录通常包含 PDF 报告、Python 源代码、数据文件、绘图结果和 LaTeX 报告源文件。

### 作业 1：图像处理与分类

- [作业报告](<homework/homework1/23320093+林宏宇+作业1.pdf>)
- [边缘检测程序](homework/homework1/code/edge_detection.py)
- [图像分类程序](homework/homework1/code/image_classification.py)

代码包含 Sobel 边缘检测，以及基于 SVM 和 MLP 的图像分类实验。

### 作业 2：小型领域知识图谱

- [作业报告](<homework/homework2/23320093+林宏宇+作业2.pdf>)
- [预处理程序](homework/homework2/code/pre_process.py)
- [实体关系转换程序](homework/homework2/code/process_2_json.py)
- [Neo4j 导入程序](homework/homework2/code/import_to_neo4j.py)

该作业围绕中山大学教师信息构建小型领域知识图谱，代码包含数据预处理、实体关系转换和 Neo4j 导入过程。

### 作业 3：聚类算法对比实验

- [代码目录](homework/homework3/code/)
- [作业说明](homework/homework3/code/README.md)
- [K-Means 实现](homework/homework3/code/Kmeans.py)
- [DBSCAN 实现](homework/homework3/code/DBSCAN.py)
- [主程序](homework/homework3/code/main.py)

该作业对比手写 K-Means 和 DBSCAN，并使用 Accuracy、Silhouette 和 Calinski-Harabasz 等指标进行评估，同时生成可视化结果。

### 作业 4：SVM 与分类实验

- [作业代码](homework/homework4/code/)
- [主程序](homework/homework4/code/main.py)
- [模型实现](homework/homework4/code/models.py)
- [超参数搜索](homework/homework4/code/hyperparam_search.py)
- [作业报告](homework/homework4/main.pdf)

代码包含手写 SVM、Pegasos 方法、序列训练、验证集评估、超参数搜索和结果绘图。

### 作业 5：时间序列预测

- [作业代码](homework/homework5/code/)
- [ARIMA 模型](homework/homework5/code/arima_model.py)
- [LSTM 实现](homework/homework5/code/lstm_numpy.py)
- [主程序](homework/homework5/code/main.py)
- [作业报告](homework/homework5/main.pdf)

该作业对比 ARIMA 和基于 NumPy 实现的 LSTM，并保存预测指标和可视化结果。

### 作业 6：多模态数据分析

- [作业代码目录](homework/homework6/code/)
- [ViLT 程序](homework/homework6/code/ViLT.py)
- [作业报告](homework/homework6/code/main.pdf)

### 作业 7：推荐系统

- [推荐系统程序](homework/homework7/code/movie_recommendation_system.py)
- [作业报告](homework/homework7/main.pdf)

## 👥 团队项目

团队项目资料位于 [homework/teamproject](homework/teamproject/)：

- [项目演示课件](<homework/teamproject/presentation/演示课件.pptx>)
- [项目要求截图](<homework/teamproject/requirement/TeamProject.png>)
- [项目评分材料](<homework/teamproject/requirement/TeamProjectScore.png>)
