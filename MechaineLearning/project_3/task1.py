import os
os.environ['HF_ENDPOINT'] = 'https://hf-mirror.com'
os.environ['CUDA_VISIBLE_DEVICES'] = '7'  # 指定使用的GPU设备
import re
import json
import logging
from datasets import load_dataset
from transformers import AutoTokenizer, AutoModelForCausalLM, pipeline, BitsAndBytesConfig

# 配置日志
logger = logging.getLogger(__name__)  # 创建一个日志记录器
logging.basicConfig(filename='task1.log', encoding='utf-8', level=logging.DEBUG, filemode = 'w')  # 创建一个日志文件

# 1. 加载数据
dataset = load_dataset("gsm8k", "main", split="test")
dataset = dataset.shuffle(seed=42).select(range(50))

# 2. 加载模型
model_name = "Qwen/Qwen2.5-7B-Instruct"
# quant_config = BitsAndBytesConfig(load_in_4bit=True)
# model = AutoModelForCausalLM.from_pretrained(model_name, quantization_config=quant_config, dtype=torch.float16)
model = AutoModelForCausalLM.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)
print(model)
correct = 0
logging.info("开始推理...")

# # 3. 使用 Pipeline 进行推理
# pipe = pipeline("text-generation", model=model, tokenizer=tokenizer)

prompt_template = """
# 角色

你是一位专业的数学题解答者，擅长解答各种数学问题，并能提供详细的解题步骤，下面我会给你一道关于加减乘除相关的数学题，请做出解答。

## 任务

数学题: {question}

请解答以上数学题，并将答案以 JSON 格式返回，格式如下：
```json
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
```

## 例子
### Prompt

"请解答以下数学题：10-(1^2 + 34) = ?"

### 你的回答
```json
{{
    "answer": "-25",
    "explanation": "计算 1 的平方得到 1 , 计算 1^2 + 34 得到 35, 最后计算 10 - 35 得到 -25"
    "steps": [
        "1^2 = 1",
        "1^2 + 34 = 35",
        "10 - 35 =-25"
    ]
}}
```

## 限制

1. 输出内容必须严格遵循 JSON 格式，并且包含在一个 ```json ... ``` 代码块中。
2. 生成的 JSON 对象必须包含 `answer` 和 `explanation` 两个字段。
"""
for i, item in enumerate(dataset):

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

    pred = ""
    # 改进正则表达式，寻找最后一个 ```json ... ``` 代码块
    json_matches = re.findall(r'```json\s*(\{[\s\S]*?\})\s*```', response)
    if json_matches:
        json_str = json_matches[-1]  # 取最后一个匹配项
        # 移除JSON中最后一个逗号
        json_str = re.sub(r',\s*(\}|\])', r'\1', json_str)
        # 修复未转义的反斜杠
        json_str = json_str.replace('\\', '\\\\')
        try:
            data = json.loads(json_str)
            pred = data.get("answer", "")
        except json.JSONDecodeError as e:
            logging.error(f"无法解码JSON: {json_str}")
            logging.error(f"错误信息: {e}")
    else:
        logging.warning("在输出中未找到JSON代码块")

    gt = item['answer'].split('####')[-1].strip()
    logging.info(f"Debug pred: '{pred}' ; gt: '{gt}' ; 原始输出: {response}")
    if str(pred) == str(gt):
        correct += 1
    
    if (i + 1) % 10 == 0:
        logging.info(f"已处理 {i + 1}/{len(dataset)} 个样本")

accuracy = correct / len(dataset)
logging.info(f"准确率：{accuracy:.2%}")
