## 构建小型领域知识图谱 作业 TODO

你是一个学生，正在完成一个关于“构建小型领域知识图谱”的作业。以下是你需要完成的任务列表：

---

### 0. 实验环境  
- **当前目录**: `/workspace/homework/`  
- **操作系统**: Linux 系统  
- **Python 环境管理工具**: 使用 `uv` 包管理器  
  - 已执行：`uv venv .venv` 创建虚拟环境  
- **项目位置**: `/workspace/homework/Large_Data/homework2/code/`  
  - [操作指引 TODO.md](/workspace/homework/Large_Data/homework2/code/TODO.md)  
  - [数据存放位置](/workspace/homework/Large_Data/homework2/code/data/)  
    - 用于存放你选择的数据集（如文本文件、网页爬取内容等）  
- **报告位置**: `/workspace/homework/Large_Data/homework2/`  

---

### 1. 整体流程  

#### ✅ 任务一：选择并说明数据集  
- [x] 选择**小型领域**的文本数据集：中山大学教师信息
- [x] 明确标注**数据来源**（中山大学官网获取教师个人主页）  
- [x] 原始数据保存在：`code/data/raw_data`   

#### ✅ 任务二：使用 NLP 工具提取实体与关系  
- [x] 安装并配置 `spaCy`（推荐中文模型 `zh_core_web_sm`）  
  - 若用中文，需确保安装支持中文的 spacy 模型  
- [x] 编写 Python 脚本实现以下功能：  
  - 文本分句与分词  
  - 实体识别（Named Entity Recognition, NER）  
  - 依存句法分析（Dependency Parsing）  
  - 关系抽取（可基于规则 + 依存路径，或使用 spaCy 的 `nlp.pipe()` 提取三元组）  
- [x] 输出结构化结果（csv 格式）存储在：`code/data/preprocess_data.csv`  
  - 示例格式：
    ```csv
        school_name,college_name,derived_teacher_name
        中山大学,中国语言文学系,丁文俊
        中山大学,中国语言文学系,丛珊
        中山大学,中国语言文学系,伍秋萍
        中山大学,中国语言文学系,倪彩霞
        中山大学,中国语言文学系,刘卫国
    ```

#### ✅ 任务三：提取 Neo4J 可用的图谱数据 
- [x] 安装并启动 Neo4j 数据库 
  - 我已完成，你重点关注后续脚本编写即可
- [ ] 编写 Python 脚本 读取 `code/data/preprocess_data.csv` 文件，将提取出的实体和关系转换为 Neo4j 可用的格式
  - 输出格式示例（JSON 或 CSV）：
    ```json
    {
      "nodes": [
        {"id": "1", "label": "Person", "properties": {"name": "丁文俊"}},
        {"id": "2", "label": "Organization", "properties": {"name": "中国语言文学系"}},
        {"id": "3", "label": "University", "properties": {"name": "中山大学"}}
      ],
      "edges": [
        {"source": "1", "target": "2", "label": "WORKS_AT"},
        {"source": "2", "target": "3", "label": "BELONGS_TO"}
      ]
    }
    ```
- [ ] 编写脚本将提取出的实体和关系导入 Neo4j  
  - 使用 Cypher 语句创建节点和边  
  - 示例：
    ```cypher
    CREATE (p:Person {name: "王昌栋"})
    CREATE (o:Organization {name: "计算机学院"})
    CREATE (u:University {name: "中山大学"})
    CREATE (p)-[:WORKS_AT]->(o)
    CREATE (o)-[:BELONGS_TO]->(u)
    ```
- [ ] 设计合理的标签（Label）和属性（Property）结构  
  - 如：`Person`, `Organization`, `University` 等  

#### ✅ 任务四：依赖管理与代码组织  
- [ ] 在 `pyproject.toml` 中添加所需依赖：
  ```toml
  [build-system]
  requires = ["setuptools>=45", "wheel"]
  build-backend = "setuptools.build_meta"

  [project.dependencies]
  spacy = "^3.7.0"
  neo4j = "^5.10.0"
  pandas = "^1.5.0"
  requests = "^2.28.0"
  ```
- [ ] 执行命令安装依赖：
  ```bash
  uv sync
  ```
  或按需添加：
  ```bash
  uv add spacy neo4j
  ```
- [ ] 所有代码放在 `code/` 目录下，结构清晰：
  ```
  code/
  ├── data/
  │   ├── raw_data.txt
  │   └── entities_relations.json
  ├── extract_entities.py
  ├── load_to_neo4j.py
  └── visualize_graph.py
  ```

#### ✅ 任务五：调试与验证  
- [ ] 运行脚本测试是否成功提取实体与关系  
- 以下部分不需要你完成，由我来验证：
    - [ ] 验证 Neo4j 是否正确加载数据  
    - [ ] 检查图谱中是否存在逻辑错误或重复节点  
    - [ ] 对关键节点进行查询验证（如：`MATCH (p:Person)-[:WORKS_AT]->(o:Organization) RETURN p.name, o.name`）

---

### 2. 报告撰写 [报告位置](/workspace/homework/Large_Data/homework2/)  

- [ ] 撰写简要报告（**不少于 2 页**），格式参考 LaTeX 模板  
  - 使用提供的模板文件：
    - [main.tex](/workspace/homework/Large_Data/homework2/main.tex)
    - [abstract.tex](/workspace/homework/Large_Data/homework2/abstract.tex)
    - [chap01.tex](/workspace/homework/Large_Data/homework2/chap01.tex)
    - [appendix1.tex](/workspace/homework/Large_Data/homework2/appendix1.tex)

- [ ] 报告内容应包括：  
  - **问题描述**：说明本次构建的知识图谱的主题及意义  
  - **方法实现**：  
    - 数据来源与预处理  
    - NLP 工具（spaCy）如何提取实体与关系  
    - Neo4j 图谱构建过程  
  - **实验内容**：  
    - 展示部分原始文本片段  
    - 列举提取出的典型实体与关系  
    - 展示 Neo4j 图谱的关键节点与连接  
  - **实验结果与分析**：  
    - 分析图谱覆盖范围、准确性、潜在改进点  
    - 可附上图谱截图或可视化效果  
  - **总结与体会**：对知识图谱构建流程的理解与收获  
  - **附录**：关键代码段（如实体抽取函数、Cypher 查询语句）


✅ **最终检查清单**  
- [ ] 数据集已选定并注明来源  
- [ ] 实体与关系已通过 spaCy 成功提取  
- [ ] Neo4j 图谱已构建并可视化  
- [ ] 报告完整且超过 2 页  
- [ ] 文件命名规范，打包正确  
- [ ] 提交前确认无抄袭风险  
