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
logging.basicConfig(filename='task1_pro_math.log', encoding='utf-8', level=logging.DEBUG, filemode = 'w')  # 创建一个日志文件

# 1. 加载数据
dataset = load_dataset("gsm8k", "main", split="test")
dataset = dataset.shuffle(seed=42).select(range(50))

# 2. 加载模型
# model_name = "Qwen/Qwen2.5-7B-Instruct"
model_name = "Qwen/Qwen2.5-Math-7B"
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
你是一位顶尖的Python程序员，擅长将复杂的数学应用题转换成可执行的Python代码来解决。

## 任务
数学题: {question}

请为以上数学题编写一段Python代码来解决它。

## 限制
1.  你必须将代码编写在 ```python ... ``` 代码块中。
2.  代码的最后一行必须是一个可以输出最终答案的表达式，不要使用 `print()` 函数。
3.  代码应该是自包含的，不要引用任何外部库（除非题目明确要求）。

## 例子
### Prompt
数学题: "Natalia sold 48/2 = 24 clips in May. She sold 24+3 = 27 clips in June. She sold 24+27 = 51 clips in total. How many clips did Natalia sell in May and June?"

### 你的回答
```python
clips_may = 48 / 2
clips_june = clips_may + 3
total_clips = clips_may + clips_june
total_clips
```
"""
MAX_RETRIES = 3
for i, item in enumerate(dataset):

    prompt = prompt_template.format(question=item['question'])
    conversation = [
        {"role": "system", "content": "You are Qwen, created by Alibaba Cloud. You are a helpful assistant."},
        {"role": "user", "content": prompt},
    ]

    pred = None
    response = ""

    for attempt in range(MAX_RETRIES):
        text = tokenizer.apply_chat_template(
            conversation,
            tokenize=False,
            add_generation_prompt=True,
        )
        model_inputs = tokenizer([text], return_tensors="pt").to(model.device)

        generated_ids = model.generate(
            **model_inputs,
            max_new_tokens=512,
        )
        generated_ids = [
            output_ids[len(input_ids):] for input_ids, output_ids in zip(model_inputs.input_ids, generated_ids)
        ]

        response = tokenizer.batch_decode(generated_ids, skip_special_tokens=True)[0]
        conversation.append({"role": "assistant", "content": response})

        code_match = re.search(r'```python\s*([\s\S]*?)\s*```', response)
        if not code_match:
            logging.warning("在输出中未找到Python代码块，尝试重新生成")
            conversation.append(
                {
                    "role": "user",
                    "content": "请严格按照要求提供 Python 代码，并确保使用 ```python``` 代码块，最后一行是表达式。",
                }
            )
            continue

        code = code_match.group(1)
        try:
            local_scope = {}
            lines = [line for line in code.strip().split('\n') if line.strip()]
            if not lines:
                raise ValueError("代码块为空")
            if len(lines) > 1:
                exec('\n'.join(lines[:-1]), {}, local_scope)
            pred = eval(lines[-1], {}, local_scope)
            break
        except Exception as e:
            logging.error(f"执行代码失败: {code}")
            logging.error(f"错误信息: {e}")
            conversation.append(
                {
                    "role": "user",
                    "content": f"刚才的代码运行出错，错误信息: {e}。请修复问题并重新给出符合要求的 Python 代码块。",
                }
            )
            pred = None

    if pred is None:
        pred = ""

    gt = item['answer'].split('####')[-1].strip()
    try:
        pred_float = float(pred)
        gt_float = float(gt)
        if abs(pred_float - gt_float) < 1e-5:
            correct += 1
            logging.info(f"答案正确! pred: '{pred}', gt: '{gt}'")
        else:
            logging.info(f"答案错误。pred: '{pred}', gt: '{gt}', 原始输出: {response}")
    except (ValueError, TypeError):
        if str(pred).strip() == str(gt).strip():
            correct += 1
            logging.info(f"答案正确! pred: '{pred}', gt: '{gt}'")
        else:
            logging.info(f"答案错误。pred: '{pred}', gt: '{gt}', 原始输出: {response}")

    if (i + 1) % 10 == 0:
        logging.info(f"已处理 {i + 1}/{len(dataset)} 个样本")

accuracy = correct / len(dataset)
logging.info(f"准确率：{accuracy:.2%}")
print(f"最终准确率: {accuracy:.2%}")
