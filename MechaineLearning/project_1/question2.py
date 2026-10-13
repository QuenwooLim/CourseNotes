# 使用 huggingface 镜像源进行图像分类任务
import os
os.environ['HF_ENDPOINT'] = 'https://hf-mirror.com'

from transformers import AutoImageProcessor, ResNetForImageClassification
import torch
from datasets import load_dataset

dataset = load_dataset("huggingface/cats-image")
image = dataset["test"]["image"][0] # 获取测试集中的第一张图像

processor = AutoImageProcessor.from_pretrained("microsoft/resnet-50") # 图像处理器
model = ResNetForImageClassification.from_pretrained("microsoft/resnet-50") # 预训练模型

inputs = processor(image, return_tensors="pt") # 将前面取出的图像转化为模型输入格式 pytorch张量格式

with torch.no_grad():
    logits = model(**inputs).logits

# model predicts one of the 1000 ImageNet classes
predicted_label = logits.argmax(-1).item()
print(model.config.id2label[predicted_label])
