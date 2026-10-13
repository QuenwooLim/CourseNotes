<div align="center">
  <img src="./images/sysu.jpeg" alt="中山大学校徽" width="500"/>  

<br><br><br>
</div>
<div style="font-size:1.6em; font-weight:normal; line-height:1.6;">
<div style="text-align:center; font-size:2.9em; font-weight:normal; letter-spacing:0.1em;">实验作业报告</div>
<br/>
<br>
<div style="text-align:center; font-size:1.3em; line-height:1.8;">
  <table style="margin: 0 auto; font-size:1.1em;">
  <tr><td align="right">实验：</td><td align="left">机器学习与数据挖掘</td></tr>
  <tr><td align="right">学号：</td><td align="left">23320093</td></tr>
  <tr><td align="right">姓名：</td><td align="left">林宏宇</td></tr>
  <tr><td align="right">专业：</td><td align="left">计算机科学与技术</td></tr>
  <tr><td align="right">班级：</td><td align="left">计科1班</td></tr>
  <tr><td align="right">指导教师：</td><td align="left">林倞</td></tr>
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年11月24日</td></tr>
  </table>
</div>
</div>

<div STYLE="page-break-after: always;"></div>

# 机器学习与数据挖掘实验报告

## ✏️ 作业要求

提交方式：报告（包含模型说明、实验设置、实验结果）和源代码打包成【学号_姓名_第三次大作业.zip】提交

### 任务一：LLM 数学推理评测 (GSM8K)

数据集：https://huggingface.co/datasets/openai/gsm8k (GSM8K, Grade School Math 8K)
测试集规模：为了节省算力，请从 Test set 中随机抽取 50 条数据进行评测
模型要求：选择一个 7B 参数量或者蒸馏后更小参数量的开源模型（推荐 Qwen2.5-7B-Instruct, Llama-2-7B）
任务：构建 Pipeline，输入问题，让模型输出推理过程和答案，并自动提取答案与 Ground Truth 对比计算准确率

### 任务二：MLLM 多模态科学问答 (ScienceQA)

数据集：https://huggingface.co/datasets/derek-thomas/ScienceQA (ScienceQA)
筛选条件：仅筛选包含 Image (图片) 信息的题目（即 image 字段不为空的样本）。
测试集规模：为了节省算力，请从 Test set 中随机抽取 50 条数据进行评测
模型要求：选择一个7B 参数量或者蒸馏后的开源多模态模型（推荐 Qwen2-VL-7B-Instruct 或 LLaVA-v1.5-7b）
任务：输入“图片 + 问题 + 选项”，让模型选择正确选项（A/B/C/D），计算准确率

### 补充说明

要求说明所选择模型的结构（根据该模型的原论文）
为了节省推理资源可以选择 4-bit 量化推理（例如 https://github.com/aahouzi/llama2-chatbot-cpu ）
> Don't just call the API, understand the pipeline.

## 🧑‍💻 任务一：LLM 数学推理评测 (GSM8K)

### 题目

数据集：https://huggingface.co/datasets/openai/gsm8k (GSM8K, Grade School Math 8K)
测试集规模：为了节省算力，请从 Test set 中随机抽取 50 条数据进行评测
模型要求：选择一个 7B 参数量或者蒸馏后更小参数量的开源模型（推荐 Qwen2.5-7B-Instruct, Llama-2-7B）
任务：构建 Pipeline，输入问题，让模型输出推理过程和答案，并自动提取答案与 Ground Truth 对比计算准确率

### Qwen2.5 模型介绍

Qwen2.5 大模型旨在打造更优的大语言模型，解决过往模型在数据、规模、应用等方面的局限，提升模型的通用性、准确性和效率，以满足多样化的使用需求，推动大语言模型在各领域的应用与发展

#### 1. 创新点

- 数据处理创新：**预训练**数据从7万亿token扩展到**18万亿token**，通过优化数据筛选、融入高质量领域数据、生成合成数据以及平衡数据分布等手段，并利用Qwen2-Instruct模型进行数据质量过滤和内容分类，**提升数据质量**。

- 训练方法创新：开发超参数缩放定律，确定**不同规模模型**的最优训练超参数，如批量大小和学习率；采用**两阶段预训练**和**渐进式上下文长度扩展策略**，提升模型对长序列的处理能力；**后训练阶段**，通过扩展监督微调数据覆盖范围和采用两阶段强化学习，增强模型多方面的能力。

- 模型架构创新：开源的密集模型采用**基于Transformer**的解码器架构，并融入多种优化组件；基于密集模型扩展出**MoE模型架构**，通过替换FFN层和采用创新路由机制提升性能；扩展控制令牌，统一词汇表，减少兼容性问题。

#### 2. 网络结构

- Qwen2.5系列模型整体架构分类
  - 开源的密集模型（Qwen2.5-0.5B/1.5B/3B/7B/14B/32B/72B）
  - 用于API服务的MoE模型（Qwen2.5-Turbo和Qwen2.5-Plus）

- 密集模型架构细节
  - 基础架构：采用基于**Transformer的解码器**架构，融入多种关键组件提升性能。
  - 注意力机制：运用**分组查询注意力（GQA）**提升KV缓存利用效率，降低内存占用和计算量。
  - 激活函数：使用 **SwiGLU激活函数** 增强模型非线性表达能力，学习复杂数据模式。
  - 位置编码：借助 **旋转位置嵌入（RoPE）** 对位置信息编码，使模型有效捕捉文本序列顺序特征。
  - 归一化方式：采用 **RMSNorm** 进行预归一化，确保训练过程稳定，加速收敛并提高模型泛化能力。
  - 参数设置：各模型在**层数、头数、嵌入层共享**等方面有差异。层数从0.5B模型的24层到72B模型的80层不等；头数（查询/键值）也不同，如0.5B模型为14/2，7B模型为28/4；部分模型（0.5B、1.5B、3B）共享嵌入层，7B及以上模型不共享

- MoE模型架构扩展：基于密集模型架构，用专门的MoE层替换标准前馈网络（FFN）层。MoE层含多个FFN专家和路由机制，根据输入动态分配计算资源，将token分配给前K个专家处理，提升模型效率和性能。（还是传统 MoE方法）

- tokenizer设置：使用Qwen的tokenizer，采用**字节级字节对编码（BBPE）**，词汇表含151,643个常规token。控制token从3个扩展到22个，为工具功能新增2个token，其余用于扩展模型能力，统一了所有Qwen2.5模型的词汇表，减少兼容性问题

#### 3. 预训练（Pre-Training）

Qwen2.5语言模型的预训练过程，涵盖数据处理、超参数确定以及长上下文训练等关键环节，旨在提升模型的性能和泛化能力。

- 预训练数据
  - 数据质量提升：利用Qwen2-Instruct模型进行多维度数据质量评估和筛选，相较于Qwen2有显著改进，能更好地保留高质量数据并过滤低质量样本。
  - 数据混合优化：对不同领域数据进行分类和平衡，对网络数据中过度代表和代表性不足的领域分别进行下采样和上采样，构建更平衡、信息更丰富的训练数据集。
- 缩放定律确定超参数：基于Qwen2.5的预训练数据制定超参数缩放定律，通过大量实验研究不同模型架构下最优学习率和批量大小与模型规模、数据规模的关系。
- 长上下文预训练
  - 多数模型训练策略：多数Qwen2.5模型采用两阶段预训练方法，先在4096 token上下文长度下训练，最后阶段将上下文长度扩展到32768 token，并使用ABF技术将RoPE基础频率从10,000提高到1,000,000。
  - Qwen2.5-Turbo的特殊策略： Qwen2.5-Turbo 在训练过程中采用渐进式上下文长度扩展策略，分四个阶段将上下文长度逐步扩展到262,144 token ，每个阶段使用特定比例的不同长度序列进行训练，并将RoPE基础频率设为10,000,000。同时，利用YARN和Dual Chunk Attention（DCA）技术，使Qwen2.5-Turbo能处理长达100万token的序列，其他模型能处理长达131,072 token的序列，且在处理长序列时降低困惑度，保持对短序列的良好性能。

#### 4.后训练（Post-Training）

Qwen2.5语言模型的后训练过程，涵盖监督微调、离线强化学习、在线强化学习以及长文本微调多个关键环节。

- 监督微调（SFT）
  - 数据构建：构建超100万高质量样本的数据集，覆盖长序列生成、数学、编码、指令遵循等多领域，针对Qwen2不足进行优化。
  - 训练策略：模型以32,768令牌序列长度微调两个epoch，学习率从7×10^-6渐降至7×10^-7，采用0.1的权重衰减和梯度裁剪（最大值1.0）防止过拟合。
  - 任务优化：通过开发长响应数据集、引入思维链数据、多语言协作框架等方法，分别提升模型长序列生成、数学、编码、指令遵循、结构化数据理解、逻辑推理、跨语言转移、鲁棒系统指令和响应过滤等能力。
- 离线强化学习：聚焦数学、编码等难以用奖励模型评估的领域，利用SFT模型 重采样生成正负例 ，构建约150,000训练对，使用在线合并优化器以7×10^−7 学习率训练一个epoch，确保训练信号可靠且与人类期望一致。
- 在线强化学习
  - 奖励模型训练：依据真实性、有用性等标准标注数据，使用开源数据和专有复杂查询集 训练奖励模型 ，偏好对通过人工和自动标注生成，并 整合DPO 训练数据。
  - 模型训练：采用 GRPO 算法，依据奖励模型评估的响应分数方差确定训练顺序，每个查询采样8个响应，以2048的全局批量大小和每集2048个样本进行训练。
- 长上下文微调：为扩展Qwen2.5-Turbo的上下文长度，SFT阶段分两阶段训练，先仅用短指令，再结合长短指令，提升长上下文任务能力并保持短任务性能；RL阶段仅用短指令训练，在提升长上下文任务的同时减少计算成本。

#### 实验结果

- 基准测试表现优异：在涵盖语言理解、推理、数学、编码等多领域的基准测试中，Qwen2.5系列模型**性能卓越**。例如：
  - Qwen2.5-72B 基础模型在多项任务上超越同类别模型，与参数规模大五倍的 Llama-3-405B 相当
  - 指令调整模型 Qwen2.5-72B-Instruct 在多个关键基准测试中甚至超过 Llama-3.1-405B-Instruct

- **长文本处理**能力突出：借助**YARN**和**DCA**等技术，Qwen2.5模型长文本处理能力显著提升。Qwen2.5-Turbo在1M令牌的密钥检索任务中准确率达100%，Qwen2.5-72B-Instruct在长文本基准测试中表现强劲，超越众多开源和专有模型。

- **奖励模型**效果良好：在多个奖励模型评估基准测试中，Qwen2.5-RM-72B表现出色，在PPE和内部收集的中文人类偏好基准测试中领先，在其他基准测试中也名列前茅

#### 不足之处

- 当前**奖励模型评估基准存在局限性**，可能引发Goodhart定律，导致模型在其他基准测试中性能下降；
- 奖励模型评估基准得分**与强化学习模型性能之间的关联不紧密**，难以准确预测强化学习模型的性能，需要进一步研究更具预测性的评估方法

#### Qwen2.5-7B-Instruct 模型的特点

- **7B**：表示该模型拥有约70亿个参数，属于中等规模的大语言模型，适合在资源有限的环境中使用，同时仍能提供强大的语言理解和生成能力。
- **Instruct**：表示该模型经过指令微调，专门优化以更好地理解和执行用户指令。这使得模型在处理各种任务时更加高效和准确，能够更好地满足用户的需求。
- 特点
  - 类型：因果语言模型
  - 训练阶段：预训练与后训练
  - 架构：带有 RoPE、SwiGLU、RMSNorm 和 Attention QKV 偏置的 transformers
  - 参数数量：76.1 亿
  - 非嵌入参数数量：65.3 亿
  - 层数：28 层
  - 注意力头数（GQA）：Q 为 28 个，KV 为 4 个
  - 上下文长度：完整 131,072 tokens，生成 8192 token

### Qwen2.5 模型代码解析

#### 模型结构
> 加载模型之后使用`print(model)`命令查看模型结构，结果如下：
```bash
Qwen2ForCausalLM(
  (model): Qwen2Model(
    (embed_tokens): Embedding(152064, 3584)
    (layers): ModuleList(
      (0-27): 28 x Qwen2DecoderLayer(
        (self_attn): Qwen2Attention(
          (q_proj): Linear(in_features=3584, out_features=3584, bias=True)
          (k_proj): Linear(in_features=3584, out_features=512, bias=True)
          (v_proj): Linear(in_features=3584, out_features=512, bias=True)
          (o_proj): Linear(in_features=3584, out_features=3584, bias=False)
        )
        (mlp): Qwen2MLP(
          (gate_proj): Linear(in_features=3584, out_features=18944, bias=False)
          (up_proj): Linear(in_features=3584, out_features=18944, bias=False)
          (down_proj): Linear(in_features=18944, out_features=3584, bias=False)
          (act_fn): SiLUActivation()
        )
        (input_layernorm): Qwen2RMSNorm((3584,), eps=1e-06)
        (post_attention_layernorm): Qwen2RMSNorm((3584,), eps=1e-06)
      )
    )
    (norm): Qwen2RMSNorm((3584,), eps=1e-06)
    (rotary_emb): Qwen2RotaryEmbedding()
  )
  (lm_head): Linear(in_features=3584, out_features=152064, bias=False)
)
```

#### 分析

`Qwen2.5-7B-Instruct` 模型是一个基于 Transformer 解码器架构的因果语言模型。通过在 `task1.py` 中加载模型后打印出的结构，我们可以清晰地看到其内部组件，这与 `README.md` 中对模型特点的描述是完全一致的。

1.  **顶层结构 (`Qwen2ForCausalLM`)**

这是 `transformers` 库中用于因果语言建模（比如文本生成）的标准封装。我们在 `task1.py` 中通过 `AutoModelForCausalLM.from_pretrained(...)` 加载的就是这个类。它包含核心的 `Qwen2Model` 和一个用于生成最终预测词汇的 `lm_head`。

2.  **词嵌入层 (`embed_tokens`)**

`Embedding(152064, 3584)`：这一层负责将输入的 token ID 转换为维度为 3584 的向量。`152064` 是词汇表的大小，`3584` 是模型的隐藏层维度（hidden dimension）。

3.  **Transformer 解码器层 (`layers`)**

`ModuleList((0-27): 28 x Qwen2DecoderLayer)`：模型的核心部分，由 28 个相同的解码器层堆叠而成。这与 `README.md` 中提到的 `Qwen2.5-7B-Instruct` 模型拥有“28层”的描述相符。

4.  **解码器层详解 (`Qwen2DecoderLayer`)**

每个解码器层都包含一个自注意力模块 (`self_attn`) 和一个前馈网络 (`mlp`)，并辅以层归一化 (`input_layernorm`, `post_attention_layernorm`)。

**自注意力 (`self_attn`)**: 它使用了**分组查询注意力 (GQA)**，这可以从 `q_proj` (查询) 的输出维度 `3584` 与 `k_proj` (键) 和 `v_proj` (值) 的输出维度 `512` 不同中看出。GQA 是一种优化，可以有效降低推理时的内存占用，这与 `README.md` 中提到的技术点一致。

**前馈网络 (`mlp`)**: 这是一个 `Qwen2MLP` 模块，内部包含 `gate_proj`、`up_proj` 和 `down_proj`。它使用了 **SwiGLU 激活函数** (`act_fn: SiLUActivation`)，这是一种高效的激活函数，能够提升模型的性能，也与 `README.md` 中的描述相符。

**层归一化 (`layernorm`)**: 模型使用了 `Qwen2RMSNorm`，即 **RMSNorm**。这是一种计算效率比传统 LayerNorm 更高的归一化方法，有助于稳定训练过程。

**位置编码 (`rotary_emb`)**: `Qwen2RotaryEmbedding`：模型采用**旋转位置嵌入 (RoPE)** 来处理序列中 token 的位置信息，这是一种在现代大模型中广泛使用的高效位置编码技术。

**输出层 (`lm_head`)**: `Linear(in_features=3584, out_features=152064, bias=False)`：这是一个线性层，它将最后一层解码器输出的 3584 维向量映射回 `152064` 大小的词汇表空间，从而为下一个 token 的生成提供概率分布。

总而言之，`task1.py` 中加载的模型实例的打印结构，精确地印证了 `README.md` 中关于 `Qwen2.5-7B-Instruct` 模型架构的描述，包括其层数、GQA 注意力机制、SwiGLU 激活函数、RMSNorm 和 RoPE 等关键技术。这表明我们成功加载了预期的模型，并可以利用其先进的架构进行后续的数学推理任务。

> 模型推理过程分析课参考：[大模型前向推理过程方法](https://blog.csdn.net/weixin_43799388/article/details/142152707?spm=1001.2014.3001.5501)

### 模型调用

实验作业要求，调用 Qwen2.5-7B-Instruct 模型对 GSM8K 数据集随机选择50个样本进行数学推理评测，构建 Pipeline，输入问题，让模型输出推理过程和答案，并自动提取答案与 Ground Truth 对比计算准确率。

> 参考`task1.py`代码实现

#### 1. 数据集

GSM8K（小学数学8K）是一个包含8500道高质量、语言多样化的小学数学应用题的数据集。该数据集旨在支持对需要多步骤推理的基础数学问题进行问答的任务。
- 这些问题需要**2-8 个步骤**才能解决。
- 解决方案主要涉及使用**基本算术运算**（`+` `-` `*` `/`）进行一系列初等计算，以得出最终答案。
- 解决方案以自然语言而非纯数学表达式的形式提供。
- 实例包含问题和答案。如下：
```python
{
  'question': 'Natalia sold clips to 48 of her friends in April, and then she sold half as many clips in May. How many clips did Natalia sell altogether in April and May?',
  'answer': 'Natalia sold 48/2 = <<48/2=24>>24 clips in May.\nNatalia sold 48+24 = <<48+24=72>>72 clips altogether in April and May.\n#### 72',
}
```
- 代码
```python
dataset = load_dataset("gsm8k", "main", split="test")
dataset = dataset.shuffle(seed=42).select(range(50))
```

#### 2. 模型加载

使用**Transformers 库**加载 Qwen2.5-7B-Instruct 模型，并进行**4-bit 量化**以节省显存和计算资源。
  
```python
model_name = "Qwen/Qwen2.5-7B-Instruct"
tokenizer = AutoTokenizer.from_pretrained(model_name)
quant_config = BitsAndBytesConfig(load_in_4bit=True)
model = AutoModelForCausalLM.from_pretrained(model_name, quantization_config=quant_config, dtype=torch.float16)
model.to("cuda:0")
```
#### 3. 模型量化

根据[transformer文档](https://hugging-face.cn/docs/transformers.js/guides/dtypes)，在 Transformers.js v3 之前，使用 quantized 选项来指定是使用模型的量化 (q8) 版本还是全精度 (fp32) 版本，通过将 quantized 分别设置为 true 或 false 来实现。现在，我们新增了使用 dtype 参数从更长的列表中进行选择的功能。

> 下载 accelerate 和 bitsandbytes 库以支持量化加载

- BitsAndBytesConfig 类用于配置模型的量化设置。它允许用户指定量化的位数（如 4-bit、8-bit）以及其他相关参数，以便在加载模型时应用量化技术，从而减少模型的内存占用和计算需求。
- quantization_config: 可以是一个 BitsAndBytesConfig 对象，或者是一个字符串，指定要使用的量化配置文件的路径或名称。
- dtype: 指定模型权重的数据类型。可以是 torch.float16、torch.bfloat16、torch.int8、torch.int4 等。
- model.to("cuda:0")：将模型移动到指定的 GPU 设备上进行计算，指定GPU设备以保证环境正确。

#### 4. prompt 工程

结合**GSM8K数据集**的特点，设计适合数学推理的 prompt 模板，指导模型生成包含推理过程和答案的响应。

- 输出为 JSON 结构，便于后续解析和评测
- 明确角色和任务，确保模型理解问题背景和要求
- 提供示例，帮助模型理解预期的回答格式
- 设定输出限制，包含答案和详细解题步骤
- prompt示例
```txt
# 角色
你是一位专业的数学题解答者，擅长解答各种数学问题，并能提供详细的解题步骤，下面我会给你一道关于加减乘除相关的数学题，请做出解答。

## 任务
数学题: {question}
请解答以上数学题，并将答案以 JSON 格式返回，格式如下：
``json
{{
    "answer": "最终答案",
    "explanation": "逻辑说明"
    "steps": [
        "步骤1说明",
        "步骤2说明",
        "...",
        "步骤N说明"
    ]
}}
``

## 例子
"请解答以下数学题：10-(1^2 + 34) = ?"
### 你的回答
``json
{{
    "answer": "-25",
    "explanation": "计算 1 的平方得到 1 , 计算 1^2 + 34 得到 35, 最后计算 10 - 35 得到 -25"
    "steps": [
        "1^2 = 1",
        "1^2 + 34 = 35",
        "10 - 35 =-25"
    ]
}}
``

## 限制

1. 输出内容必须严格遵循 JSON 格式，并且包含在一个`json`代码块中。
2. 生成的 JSON 对象必须包含 `answer` 和 `explanation` 两个字段。
``` 

#### 5. 构建 Pipeline

- 使用 Transformers 库的 `pipeline` 功能，结合自定义的 prompt 模板，构建一个完整的推理流程。
- pipeline 的底层实现
  - Tokenizer：负责将输入文本转换为模型可理解的 token 序列。
  - Model：加载并运行 Qwen2.5-7B-Instruct 模型，生成响应。
  - Post-processing：对模型输出进行处理，提取所需信息
```python
pipe = pipeline("text-generation", model=model, tokenizer=tokenizer)

prompt = prompt_template.format(question=item['question'])

# 使用 pipeline 生成
outputs = pipe(prompt, max_new_tokens=512, pad_token_id=pipe.tokenizer.eos_token_id)
output = outputs[0]['generated_text']
```

#### 6. pipeline 的替换

使用pipeline时经常出现`显存不足`的问题，可以直接调用模型和tokenizer进行推理，替换pipeline的功能。

> Don't just call the API, understand the pipeline.

```python
# 构建 prompt
prompt = prompt_template.format(question=item['question'])

# 构建 messages
messages = [
{"role": "system", "content": "You are Qwen, created by Alibaba Cloud. You are a helpful assistant."},
{"role": "user", "content": prompt} ]   

text = tokenizer.apply_chat_template(
messages,
tokenize=False,
add_generation_prompt=True
)
model_inputs = tokenizer([text], return_tensors="pt").to(model.device)

generated_ids = model.generate(
**model_inputs,
max_new_tokens=512
)
generated_ids = [
    output_ids[len(input_ids):] for input_ids, output_ids in zip(model_inputs.input_ids, generated_ids)
]

response = tokenizer.batch_decode(generated_ids, skip_special_tokens=True)[0]
```
> 参考ModelScape的[Qwen2.5-7B-Instruct使用指南](https://www.modelscope.cn/models/Qwen/Qwen2.5-7B-Instruct)

#### 7. 结果解析

- 使用正则表达式从模型生成的文本中提取 JSON 代码块。
- 解析 JSON，获取答案和解题步骤。
- 将提取的答案与数据集中的 Ground Truth 进行对比，计算准确率。
> 这部分代码较简单，参考`task1.py`中的实现。

### 结果

#### 1. 测试准确率

在随机抽取的50个GSM8K测试样本上，使用Qwen2.5-7B-Instruct模型进行数学推理评测，最终计算得到的准确率为 **12%**。
```bash
INFO:root:已处理 50/50 个样本
INFO:root:准确率：12.00%
```

#### 2. 错误示例输出
```
INFO:root:Debug pred: '21' ; gt: '17' ; 原始输出:
{
    "answer": "21",
    "explanation": "首先计算总共有多少个未成熟的橙子，即20%的25个橙子；然后直接从总数中减去坏橙子、未成熟橙子和酸橙的数量。",
    "steps": [
        "总共有25个橙子",
        "未成熟的橙子数量为25个橙子的20%，即25 * 20% = 5个",
        "已知有1个坏橙子",
        "已知有2个酸橙",
        "好橙子的数量为总数减去坏橙子、未成熟橙子和酸橙的数量，即25 - 1 - 5 - 2 = 21个"
    ]
}
INFO:root:Debug pred: '2.2222222222222223' ; gt: '1' ; 原始输出:
{
    "answer": "2.2222222222222223",
    "explanation": "首先计算4包袋子的总费用。每包袋子的价格为$10.00，所以4包的原价是$40.00。由于有10%的折扣，折扣金额是$40.00的10%，即$4.00。因此，实际支付的金额是$40.00 - $4.00 = $36.00。因为每包有9个袋子，4包共有36个袋子。所以，每个袋子的实际成本是$36.00 / 36 = $1.00。",
    "steps": [
        "每包袋子的价格为$10.00，4包的原价是$10.00 * 4 = $40.00。",
        "10%的折扣是$40.00的10%，即$40.00 * 0.10 = $4.00。",
        "实际支付的金额是$40.00 - $4.00 = $36.00。",
        "每包有9个袋子，4包共有9 * 4 = 36个袋子。",
        "每个袋子的实际成本是$36.00 / 36 = $1.00。"
    ]
}
INFO:root:Debug pred: '116' ; gt: '89' ; 原始输出:
{
    "answer": "116",
    "explanation": "Lorraine首先将90%的小贴纸换成大按钮，然后将50%的大贴纸换成大按钮，剩下的大贴纸换成小按钮。最后计算总共有多少个按钮。",
    "steps": [
        "Lorraine有30个小贴纸，90%即27个小贴纸被换成大按钮（每个大按钮相当于3个小按钮），因此获得27/3=9个大按钮。",
        "Lorraine有40个大贴纸，50%即20个大贴纸被换成大按钮，因此获得20个大按钮。",
        "剩下的大贴纸是40-20=20个，这些大贴纸被换成小按钮（每个大贴纸相当于1个小按钮），因此获得20个小按钮。",
        "现在Lorraine拥有的按钮总数为9个大按钮+20个大按钮+27个小按钮/3（因为每个大按钮等于3个小按钮）+20个小按钮=9+20+9+20=58个大按钮和27个小按钮。",
        "总共的按钮数量是58个大按钮+27个小按钮=85个大按钮（每个大按钮等于3个小按钮）+27个小按钮=85*3+27=255+27=282/3=94个大按钮+27个小按钮=116个按钮。"
    ]
}
```

#### 3. 错误分析

观察llm输出的结果，发现模型在处理多步骤数学问题时，存在以下几类错误：
- 返回格式错误：模型未能严格按照指定的 JSON 格式输出答案，导致解析失败。
- 概念错误：模型对某些数学概念的理解存在偏差，导致解题思路错误。
- 计算错误：模型在执行基本算术运算时出现错误，导致最终答案不正确。
- 推理步骤缺失：模型未能完整展示解题步骤，遗漏了关键的计算过程。
- 理解偏差：模型对问题的理解存在偏差，导致解题方向错误。
- 结果匹配：单纯的字符串匹配存在误差，如"120.0"和"120"被视为不同答案。

### 思考

由于实验测试结果的准确率较低，可以从以下几个方面进行优化：

#### 1. 重试机制与格式匹配

在模型推理过程中，可能会遇到返回格式不匹配、网络问题、超时或生成无效响应等情况。为了提高系统的鲁棒性，设计并实现重试机制，确保在出现错误时能够自动重新尝试请求，直到成功或达到最大重试次数为止，并且设置精度小于1e-5为相等以匹配不同精度的答案。

实验结果 显示，通过引入重试机制和精度匹配，模型在 GSM8K 数据集上的准确率从初始的**12%**提升到了**54%**
```bash
INFO:root:已处理 50/50 个样本
INFO:root:准确率：54.00%
```

关键代码：
```python
for attempt in range(3):  # 最多重试3次
  ...
            if pred:  # 如果成功获取到答案，则跳出重试循环
                break
        except json.JSONDecodeError as e:
            logging.error(f"样本 {i} 第 {attempt + 1} 次尝试解码JSON失败: {json_str}")
            logging.error(f"错误信息: {e}")
    else:
        logging.warning(f"样本 {i} 第 {attempt + 1} 次尝试在输出中未找到JSON代码块")
    
    if attempt < 2:
        logging.info(f"样本 {i} 将在第 {attempt + 2} 次尝试中重试")

if not pred:
    logging.error(f"样本 {i} 在3次尝试后仍然无法获取答案。")
```
```python
if abs(pred_float - gt_float) < 1e-5:
    correct += 1
    logging.info(f"答案正确! pred: '{pred}', gt: '{gt}'")
```

#### 2. 构建知识图谱

观察测试问题：
```
James creates a media empire. He creates a movie for $2000. Each DVD cost $6 to make. He sells it for 2.5 times that much. He sells 500 movies a day for 5 days a week. How much profit does he make in 20 weeks?
```
- 该问题涉及多个计算步骤和概念，模型可能难以直接从训练数据中学习到所有相关知识。
- 通过构建一个包含相关数学概念和公式的知识图谱，可以帮助模型更好地理解和解决类似问题。
- 知识图谱可以包括基本的数学运算规则、常用公式以及与问题相关的背景知识。
> 语料库收集较为不容易，所以只做思考不做展示

#### 3. 工具增强（Tool Augmentation）

GSM8K 数据集中的问题通常需要多步骤推理和复杂计算。为了提升模型的推理能力，可以引入工具增强技术，使模型能够调用外部计算工具或函数来辅助解决问题。例如，在本项目中，实现一个可以计算**加、减、乘、除**的计算器工具，并将其集成到模型的推理流程中。当模型遇到需要进行数学计算的问题时，可以调用该计算器工具来获取准确的计算结果，从而提高整体的解题准确率。
> 参考 `task1_pro.py` 代码

实验结果显示，通过引入工具增强技术，模型在 GSM8K 数据集上的准确率显著提升，从初始的**54%**提高到**84%**。这表明工具增强在提升大模型数学推理能力方面具有显著效果。
```bash
INFO:root:已处理 50/50 个样本
INFO:root:准确率：84.00%
```

#### 4. 模型微调

针对 GSM8K 数据集的特点，对 Qwen2.5-7B-Instruct 模型进行微调。通过在该数据集上进行有针对性的训练，模型可以更好地学习到解决数学问题所需的知识和技能，从而提升推理能力和准确率。但是由于算力限制，无法在本地进行微调训练，开源的**Qwen2.5-7B-Math**模型是专门针对数学任务微调的版本，可以使用该模型进行评测。

> 但是在经过实验测试后的准确率为**76%**，考虑到测试集样本的个别差异，在这个项目性能提鲜上相差不大。

#### 5. 结果

使用上述优化方法后，预计模型在 GSM8K 数据集上的准确率将有显著提升，在重新设计**prompt**、引入**重试机制**、使用**模型微调**方法后，大模型的测试结果达到了**84%**的准确率，显著优于初始12%。

## 🧑‍💻 任务二：MLLM 多模态科学问答 (ScienceQA)

数据集：https://huggingface.co/datasets/derek-thomas/ScienceQA (ScienceQA)
筛选条件：仅筛选包含 Image (图片) 信息的题目（即 image 字段不为空的样本）。
测试集规模：为了节省算力，请从 Test set 中随机抽取 50 条数据进行评测
模型要求：选择一个7B 参数量或者蒸馏后的开源多模态模型（推荐 Qwen2-VL-7B-Instruct 或 LLaVA-v1.5-7b）
任务：输入“图片 + 问题 + 选项”，让模型选择正确选项（A/B/C/D），计算准确率

### 题目

数据集：https://huggingface.co/datasets/derek-thomas/ScienceQA (ScienceQA)
筛选条件：仅筛选包含 Image (图片) 信息的题目（即 image 字段不为空的样本）。
测试集规模：为了节省算力，请从 Test set 中随机抽取 50 条数据进行评测
模型要求：选择一个7B 参数量或者蒸馏后的开源多模态模型（推荐 Qwen2-VL-7B-Instruct 或 LLaVA-v1.5-7b）
任务：输入“图片 + 问题 + 选项”，让模型选择正确选项（A/B/C/D），计算准确率

### Qwen2-VL 模型介绍

#### 1. Qwen-VL 系列

- 位置感知视觉语言适配器
  - 为了缓解长图像特征序列带来的效率问题，Qwen-VL 引入了一种压缩图像特征的视觉语言适配器（Adapter）。
  - 2D绝对位置编码被纳入交叉注意机制的 query-key对中，以减轻压缩过程中位置细节的潜在损失。长度为 256 的压缩图像特征序列随后被输入到大语言模型中
- 三阶段训练方式
  - 预训练（pre-training）：主要利用大规模、弱标记、网络爬行的图像文本对
  - 多任务预训练（multi-tasks pre-training）：引入了具有更大输入分辨率，更高质量、以及更细粒度的视觉语言标注数据和交错的图文数据
  - 监督微调（SFT）：通过指令微调对Qwen-VL预训练模型进行微调，增强其指令跟随和对话能力

#### 2. Qwen2-VL 模型的创新点

- 重新定义了视觉处理中传统的预定分辨率方法，能够对真实世界中的任意分辨率图片输入进行处理
- 统一了单帧图片，多图以及视频输入的视觉处理流程（即都当做视频来处理，单帧图片通过复制变成连续相同的两帧图片），更好的适配不同类型的视觉输入
- 多模态旋转位置编码，在时间和空间维度上也考虑视觉token的RoPE，更好的对多模态信息进行位置编码

#### 3. 模型架构

- patch_embed层，使用了一个3D卷积层，其中卷积核（Kernel）大小为(2, 14, 14)，步长（Stride）同样为(2, 14, 14)，表示卷积核在时间维度上的大小为2，在空间维度上的大小为14x14
- rotary_pos_emb层，用于对视觉输入做时间和空间上的旋转位置编码
- 对齐层PatchMerger使用了普通的MLP层，包含两层Linear，与Qwen-VL使用的Cross-attention不同，这里并不是通过可学习的Query来减少视觉token数，而是在PatchMerger层中，对相邻的视觉token进行合并（减少token数，同时会增加每个token的特征维度）来实现的。

#### 4. 统一视觉处理方式

- 以每秒两帧的频率对每个视频进行采样
- 每个图像都被视为两个相同的帧
- 为了平衡长视频处理的计算需求和整体训练效率，我们动态调整每个视频帧的分辨率，将每个视频的token总数限制在16384

#### 5. 原生动态分辨率处理

- 与Qwen-VL不同，Qwen2-VL可以处理任意分辨率的图像，将其动态转换为可变数量的视觉标记
- 引入了2D RoPE来捕获图像的二维位置信息
- 直接对图像进行patch化，然后直接过image encoder进行特征提取

#### 6. 多模态旋转位置编码
- M-RoPE有效地对多模态输入的位置信息进行了建模
- 将原始的旋转嵌入分解为三个部分来实现的：时间、高度和宽度
  - 于文本输入，这些组件使用相同的位置ID，使M-RoPE在功能上等同于1D RoPE。
  - 在处理图像时，每个视觉标记的时间ID保持不变，而根据标记在图像中的位置为高度和宽度分量分配不同的ID。
  - 对于被视为帧序列的视频，每帧的时间ID都会递增，而高度和宽度分量遵循与图像相同的ID分配模式。
  - 在模型的输入包含多个模态的情况下，通过将前一个模态的最大位置ID加1来初始化每个模态的位置编号

### Qwen2-VL 代码解析

### 模型代码分析

#### print(model)
```bash
Qwen2VLForConditionalGeneration(
  (model): Qwen2VLModel(
    (visual): Qwen2VisionTransformerPretrainedModel(
      (patch_embed): PatchEmbed(
        (proj): Conv3d(3, 1280, kernel_size=(2, 14, 14), stride=(2, 14, 14), bias=False)
      )
      (rotary_pos_emb): VisionRotaryEmbedding()
      (blocks): ModuleList(
        (0-31): 32 x Qwen2VLVisionBlock(
          (norm1): LayerNorm((1280,), eps=1e-06, elementwise_affine=True)
          (norm2): LayerNorm((1280,), eps=1e-06, elementwise_affine=True)
          (attn): VisionAttention(
            (qkv): Linear(in_features=1280, out_features=3840, bias=True)
            (proj): Linear(in_features=1280, out_features=1280, bias=True)
          )
          (mlp): VisionMlp(
            (fc1): Linear(in_features=1280, out_features=5120, bias=True)
            (act): QuickGELUActivation()
            (fc2): Linear(in_features=5120, out_features=1280, bias=True)
          )
        )
      )
      (merger): PatchMerger(
        (ln_q): LayerNorm((1280,), eps=1e-06, elementwise_affine=True)
        (mlp): Sequential(
          (0): Linear(in_features=5120, out_features=5120, bias=True)
          (1): GELU(approximate='none')
          (2): Linear(in_features=5120, out_features=3584, bias=True)
        )
      )
    )
    (language_model): Qwen2VLTextModel(
      (embed_tokens): Embedding(152064, 3584)
      (layers): ModuleList(
        (0-27): 28 x Qwen2VLDecoderLayer(
          (self_attn): Qwen2VLAttention(
            (q_proj): Linear(in_features=3584, out_features=3584, bias=True)
            (k_proj): Linear(in_features=3584, out_features=512, bias=True)
            (v_proj): Linear(in_features=3584, out_features=512, bias=True)
            (o_proj): Linear(in_features=3584, out_features=3584, bias=False)
            (rotary_emb): Qwen2VLRotaryEmbedding()
          )
          (mlp): Qwen2MLP(
            (gate_proj): Linear(in_features=3584, out_features=18944, bias=False)
            (up_proj): Linear(in_features=3584, out_features=18944, bias=False)
            (down_proj): Linear(in_features=18944, out_features=3584, bias=False)
            (act_fn): SiLUActivation()
          )
          (input_layernorm): Qwen2RMSNorm((3584,), eps=1e-06)
          (post_attention_layernorm): Qwen2RMSNorm((3584,), eps=1e-06)
        )
      )
      (norm): Qwen2RMSNorm((3584,), eps=1e-06)
      (rotary_emb): Qwen2VLRotaryEmbedding()
    )
  )
  (lm_head): Linear(in_features=3584, out_features=152064, bias=False)
)
```

#### 模型分析

- **视觉编码器**：`Qwen2VisionTransformerPretrainedModel` 先使用 3D 卷积 `Conv3d(3, 1280, kernel_size=(2,14,14))` 对原始帧做 patch 化，并借助 `VisionRotaryEmbedding` 将时间与空间位置信息注入到视觉 token 中。随后 32 层 `Qwen2VLVisionBlock` 通过自注意力 (`VisionAttention`) 与前馈网络 (`VisionMlp`) 提取多尺度语义特征，末端的 `PatchMerger` 以 MLP 方式压缩 token 数并将特征维度提升到 3584，保证视觉信息能够与文本端对齐。
- **文本解码器**：`Qwen2VLTextModel` 延续 Qwen2.5 架构，包含 28 层 `Qwen2VLDecoderLayer`。注意力模块采用分组查询注意力（Q=3584, KV=512）降低 KV 缓存开销；前馈层由 `Qwen2MLP` + SwiGLU 激活（实现为 `SiLUActivation` + `gate_proj`）组成；`Qwen2RMSNorm` 负责层前后归一化，提高训练稳定性；`Qwen2VLRotaryEmbedding` 将 RoPE 扩展到时间/空间维度，使得视觉 token 与文本 token 在同一坐标系下交互。
- **跨模态融合**：视觉侧经过 `PatchMerger` 压缩后直接与文本序列拼接，由统一的解码器进行跨模态自注意力，实现图文信息的无缝融合，无需额外的 cross-attention。
- **输出层**：`lm_head` 是一个 3584→152064 的线性层，对应 Qwen 共享词表，用于预测下一 token，从而完成“图片+问题”到答案字母的生成任务。

### 模型调用
> 参考 modelspace 的 [Qwen2-VL-7B-Instruct使用指南](https://www.modelscope.cn/models/qwen/Qwen2-VL-7B-Instruct)

#### 1. 数据集

- 使用 Huggingface Datasets 库加载 ScienceQA 数据集
- 仅保留包含图片的样本
- 随机抽取50条进行评测
```python
# 1. 加载ScienceQA数据集，只保留有图片的样本
logger.info("加载数据集...")
dataset = load_dataset("derek-thomas/ScienceQA", split="test")
dataset = [item for item in dataset if item.get('image', None)]
logger.info(f"原始测试集有图片的样本数: {len(dataset)}")
# 随机抽取50条
random.seed(42)
dataset = random.sample(dataset, 50)
```

#### 2. 模型加载

- 使用 Transformers 库加载 Qwen2-VL-7B-Instruct 模型
- 使用 AutoProcessor 处理文本和图像输入
- print(model) 查看模型结构
```python
# 2. 加载多模态模型（以Qwen/Qwen2-VL-7B-Instruct为例）
model_name = "Qwen/Qwen2-VL-7B-Instruct"
model = Qwen2VLForConditionalGeneration.from_pretrained(model_name, device_map="auto", trust_remote_code=True)
processor = AutoProcessor.from_pretrained(model_name)
logger.info(model)
```
#### 3. prompt 工程

- 设计适合多模态科学问答的 prompt 模板，指导模型生成包含答案的响应
- 输出为 JSON 结构，便于后续解析和评测
```
prompt_template = """
你是一位专业的科学问答专家，擅长解答各种科学相关的问题。下面我会给你一个包含图片的科学问题，请根据图片和问题内容进行解答。
## 问题
{question}

## 图片
{image}

## 选项
{options}

## 回答要求
以json格式返回答案，选项字母只能是A、B、C或D的其中一个。
格式如下：
``json
{{
    "answer": "选项字母",
    "explanation": "简要说明选择该答案的理由"
}}
``
请仔细观察上面的图片，并结合图片内容回答上述问题，按照上述格式返回答案。
"""
```
#### 4. 模型推理

- 遍历数据集，构建输入 prompt 和 messages
- 使用 processor 处理文本和图像，准备模型输入
- 调用模型进行推理，生成答案
```python
for i, item in enumerate(dataset):
    image = item['image'].convert("RGB")
    question = item["question"]
    choices = item["choices"]
    answer_index = item["answer"]
    
    prompt = prompt_template.format(
        question=question,
        image="[图片]",
        options="\n".join([f"{chr(65 + idx)}. {choice}" for idx, choice in enumerate(choices)])
    )
    messages = [
        {
            "role": "user",
            "content": [
                {"type": "image", "image": image},
                {"type": "text", "text": prompt},
            ],
        }
    ]
    
    # 准备模型输入
    image = item['image'].convert("RGB")
    text = processor.apply_chat_template(messages, tokenize=False, add_generation_prompt=True)
    inputs = processor(text=text, images=[image], return_tensors="pt")

    # 模型推理
    generated_ids = model.generate(**inputs, max_new_tokens=10)
    generated_ids = generated_ids[:, inputs['input_ids'].shape[1]:]
    response = processor.batch_decode(generated_ids, skip_special_tokens=True, clean_up_tokenization_spaces=False)[0]
```

#### 5. 结果解析

- 使用正则表达式从模型生成的文本中提取 JSON 代码块。  
- 解析 JSON，获取答案。
- 将提取的答案与数据集中的 Ground Truth 进行对比，计算准确率。
```python
    # 提取答案并计算准确率
    match = re.search(r'([A-Z])', response)
    predicted_answer = -1
    if match:
        predicted_char = match.group(1)
        predicted_answer = ord(predicted_char) - ord('A')

    if predicted_answer == answer_index:
        correct += 1
```

### 结果

由于输出较为单一，且不像数学推理那样复杂，模型在多模态科学问答任务上的表现相对较好。在随机抽取的50个ScienceQA测试样本上，使用Qwen2-VL-7B-Instruct模型进行评测，最终计算得到的准确率为 **86%**。
```bash
INFO:__main__:准确率：86.00%
```

## 💡 实验总结

### Qwen 系列大模型

本次实验深入探索了阿里通义千问（Qwen）系列的两个代表性模型：纯文本大模型 `Qwen2.5-7B-Instruct` 和多模态大模型 `Qwen2-VL-7B-Instruct`。

- **Qwen2.5** 在架构上采用了成熟的 Transformer 解码器，并融合了分组查询注意力（GQA）、SwiGLU 激活函数、旋转位置嵌入（RoPE）和 RMSNorm 等多项优化技术，旨在提升模型性能和推理效率。其通过海量高质量数据进行预训练，并经过指令微调，使其具备强大的通用语言理解和指令遵循能力。

- **Qwen2-VL** 则在多模态领域展现了创新。它通过统一的视觉处理流程，能够灵活处理任意分辨率的单图、多图乃至视频输入。其核心创新点在于**多模态旋转位置编码（M-RoPE）**，将时间和空间位置信息有效编码，使得视觉 Token 和文本 Token 可以在统一的解码器中无缝交互，实现了高效的跨模态信息融合。

这两个模型都体现了 Qwen 系列在模型架构、训练方法和数据处理上的持续演进，是当前开源社区中极具竞争力的大模型。

### LLM 使用的思考

在本次实验中，尤其是在任务一（GSM8K 数学推理）的探索过程中，我对大型语言模型（LLM）的使用有了更深刻的理解：

1.  **LLM 并非全能计算器**：LLM 的核心优势在于语言理解、模式识别和逻辑组织，而非精确的数值计算。在初次实验中，模型频繁出现计算错误，导致初始时准确率仅有 12%。这表明，对于需要严格数学精度的任务，不能完全依赖模型的内置计算能力。

2.  **“人机协作”的重要性**：提升 LLM 应用效果的关键在于扬长避短。
    - **Prompt 工程是基础**：精心设计的 Prompt（包含角色、任务、示例和格式限制）是引导模型正确理解意图、生成期望输出的第一步。
    - **鲁棒的后处理是保障**：模型输出可能不稳定或格式不完全合规。通过引入重试机制、正则表达式解析和浮点数精度匹配，我们将准确率从 12% 提升至 54%，这证明了健壮的工程实践对于弥补模型不确定性的重要性。
    - **工具增强是能力放大器**：当模型遇到其能力边界（如复杂计算）时，赋予其调用外部工具（如计算器）的能力是最高效的解决方案。通过工具增强，模型可以将复杂的数学问题分解，并将计算步骤“外包”给精确的工具，自身则专注于推理和规划，最终将准确率提升至 84%。

总而言之，成功应用 LLM 的关键在于将其视为一个强大的“推理引擎”而非“全知全能的专家”，并通过 Prompt、后处理和工具增强等手段，构建一个高效的“人机（或模型-工具）协作”系统。

### 实验心得

通过本次实验，我深刻体会到大模型在不同任务领域的能力差异及其背后的原因。

在**任务二（ScienceQA 多模态问答）**中，`Qwen2-VL` 取得了 **86%** 的高准确率。这得益于其强大的视觉-语言联合理解能力。模型能够有效关联图片内容与文本问题，并在给定的选项中做出判断。这类任务更侧重于模型的**感知、识别和匹配**能力，这正是当前多模态大模型经过大规模图文对预训练后所擅长的。

相比之下，**任务一（GSM8K 数学推理）**则更具挑战性。`Qwen2.5` 的初始准确率仅为 **12%**，经过多轮工程优化和引入工具后才达到 **84%**。这揭示了通用 LLM 在**符号推理和精确计算**方面的固有短板。数学问题要求严格的逻辑链条和零错误的计算，任何一个环节的“幻觉”或偏差都会导致最终结果的失败。这也反证了，对于依赖精确推理的任务，单纯依靠模型自身是不可靠的，**模型与工具的结合（Tool Augmentation）**才是未来发展的必然趋势。

总的来说，本次实验让我认识到，大模型的发展并非要创造一个无所不能的“通用人工智能”，而是构建一个以模型为核心、可灵活扩展的“智能系统”。未来的研究和应用方向，不仅要关注模型本身的性能提升，更要探索如何让模型更高效地学习和使用工具，将模型的语言天赋与外部工具的专业能力结合，从而在更广泛的领域解决更复杂的问题。

## 📚 参考资料

- [Qwen2.5论文](https://arxiv.org/abs/2412.15115 （2024.12.14）)
- [Qwen2.5模型](https://huggingface.co/Qwen https://modelscope.cn/organization/qwen)
- [Qwen2.5代码](https://github.com/QwenLM/Qwen2.5)
- [Qwen2-VL论文](https://arxiv.org/abs/2409.12191)
- [Qwen2-VL模型](https://www.modelscope.cn/models/qwen/Qwen2-VL-7B-Instruct)
- [Qwen2-VL代码](https://github.com/QwenLM/Qwen3-VL)
- [大模型前向推理过程详解](https://blog.csdn.net/weixin_43799388/article/details/142152707?spm=1001.2014.3001.5501)

## 附件

- task1.py：Qwen2.5-7B-Instruct 数学推理评测代码
- task1_pro.py：Qwen2.5-7B-Instruct 数学推理评测（工具增强版）代码
- task2.py：Qwen2-VL-7B-Instruct 多模态科学问答评测代码
- task1_origin.log：Qwen2.5-7B-Instruct 数学推理评测原始日志
- task1_tool.log：Qwen2.5-7B-Instruct 数学推理工具增强版日志
- task1_math.log：Qwen2.5-7B-Math 数学微调模型评测日志
- task2.log：Qwen2-VL-7B-Instruct 多模态科学问答评测日志
