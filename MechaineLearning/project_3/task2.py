import os
os.environ['HF_ENDPOINT'] = 'https://hf-mirror.com'
os.environ['CUDA_VISIBLE_DEVICES'] = '5,6,7' # 分布在多张卡上以满足内存需求
os.environ.setdefault("TRANSFORMERS_DISABLE_TORCHVISION_IMPORT_ERROR", "1")
from datasets import load_dataset
import random
import re
import logging
from transformers import AutoProcessor
from modelscope import Qwen2VLForConditionalGeneration, AutoProcessor
from qwen_vl_utils import process_vision_info

# 配置日志
logger = logging.getLogger(__name__)  # 创建一个日志记录器
logging.basicConfig(filename='task2.log', encoding='utf-8', level=logging.DEBUG, filemode = 'w')  # 创建一个日志文件

# 1. 加载ScienceQA数据集，只保留有图片的样本
logger.info("加载数据集...")
dataset = load_dataset("derek-thomas/ScienceQA", split="test")
dataset = [item for item in dataset if item.get('image', None)]
logger.info(f"原始测试集有图片的样本数: {len(dataset)}")
# 随机抽取50条
random.seed(42)
dataset = random.sample(dataset, 50)

# 2. 加载多模态模型（以Qwen/Qwen2-VL-7B-Instruct为例）
model_name = "Qwen/Qwen2-VL-7B-Instruct"
model = Qwen2VLForConditionalGeneration.from_pretrained(model_name, device_map="auto", trust_remote_code=True)
processor = AutoProcessor.from_pretrained(model_name)
logger.info(model)

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
```json
{{
    "answer": "选项字母",
    "explanation": "简要说明选择该答案的理由"
}}
```
请仔细观察上面的图片，并结合图片内容回答上述问题，按照上述格式返回答案。
"""

correct = 0
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

    # 提取答案并计算准确率
    match = re.search(r'([A-Z])', response)
    predicted_answer = -1
    if match:
        predicted_char = match.group(1)
        predicted_answer = ord(predicted_char) - ord('A')

    if predicted_answer == answer_index:
        correct += 1
    
    logger.info(f"样本 {i+1}: 问题: {question}, 正确答案: {chr(65+answer_index)}, 模型输出: {response.strip()}, 预测: {chr(65+predicted_answer) if predicted_answer != -1 else 'N/A'}")


accuracy = correct / len(dataset)
logger.info(f"准确率：{accuracy:.2%}")
