"""任务三：基于 MMDetection 的 Faster R-CNN 检测示例脚本。

该脚本参考了 `question2.py` 的结构：

1. 使用 Hugging Face 镜像源下载示例图像；
2. 加载预训练的 Faster R-CNN (ResNet-50 C4) 模型；
3. 进行一次前向推理，并输出高置信度的目标检测结果；
4. 将可视化图片保存到本地，便于撰写实验报告或调试。

注意：若运行环境中尚未安装 MMDetection，请先执行
`pip install "mmdet>=3.2" "mmengine>=0.10" "mmcv>=2.1"`。
"""

import os
from pathlib import Path
from typing import List, Tuple

import numpy as np
import torch
from datasets import load_dataset


MULTISCALE_TRAIN_PIPELINE = [
    dict(type="LoadImageFromFile"),
    dict(type="LoadAnnotations", with_bbox=True),
    dict(
        type="RandomChoiceResize",
        scales=[
            (1333, 640),
            (1333, 672),
            (1333, 704),
            (1333, 736),
            (1333, 768),
            (1333, 800),
        ],
        keep_ratio=True,
    ),
    dict(type="RandomFlip", prob=0.5),
    dict(type="PackDetInputs"),
]


def _ensure_dependencies() -> Tuple[object, object]:
    """惰性导入 MMDetection 相关依赖，便于在缺包时给出友好提示。"""

    try:
        from mmengine.registry import init_default_scope  # type: ignore
        from mmdet.apis import DetInferencer  # type: ignore
    except ImportError as exc:  # pragma: no cover - 在缺包环境下给出指引
        print(
            "未检测到 MMDetection 依赖，请先安装所需库：\n"
            "  pip install \"mmdet>=3.2\" \"mmengine>=0.10\" \"mmcv>=2.1\"\n"
            "安装完成后即可运行 question3.py 获取检测结果。"
        )
        raise SystemExit(0) from exc

    return init_default_scope, DetInferencer


def _load_demo_image() -> object:
    """从 Hugging Face 数据集中取一张示例图片。"""

    dataset = load_dataset("huggingface/cats-image")
    return dataset["test"]["image"][0]


def _format_predictions(
    scores: np.ndarray,
    labels: np.ndarray,
    boxes: np.ndarray,
    classes: List[str],
    threshold: float = 0.3,
) -> List[str]:
    """根据置信度筛选预测结果并格式化输出文本。"""

    keep = scores >= threshold
    filtered = []
    for score, label, box in zip(scores[keep], labels[keep], boxes[keep]):
        class_name = classes[int(label)] if classes else str(int(label))
        box_str = ", ".join(f"{coord:.1f}" for coord in box)
        filtered.append(f"{class_name:<12} score={score:.2f} bbox=[{box_str}]")
    return filtered


def main() -> None:
    os.environ.setdefault("HF_ENDPOINT", "https://hf-mirror.com")
    init_default_scope, DetInferencer = _ensure_dependencies()

    # 初始化默认 scope，避免多次调用导致的冲突。
    init_default_scope("mmdet")

    image = _load_demo_image()
    device = "cuda" if torch.cuda.is_available() else "cpu"

    inferencer = DetInferencer(
        model="faster-rcnn_r50-caffe_c4-1x_coco",
        device=device,
        cfg_options={
            "train_pipeline": MULTISCALE_TRAIN_PIPELINE,
            "train_dataloader.dataset.pipeline": MULTISCALE_TRAIN_PIPELINE,
        },
    )

    output_dir = Path("outputs/task3_detection")
    output_dir.mkdir(parents=True, exist_ok=True)

    results = inferencer(
        image,
        return_datasample=True,
        out_dir=str(output_dir),
        pred_score_thr=0.3,
        show=False,
    )

    data_sample = results["predictions"][0]
    pred_instances = data_sample.pred_instances

    scores = pred_instances.scores.detach().cpu().numpy()
    labels = pred_instances.labels.detach().cpu().numpy()
    boxes = pred_instances.bboxes.detach().cpu().numpy()

    classes = data_sample.metainfo.get(
        "classes", inferencer.dataset_meta.get("classes", [])
    )

    lines = _format_predictions(scores, labels, boxes, classes, threshold=0.3)

    if not lines:
        print("未检测到置信度大于 0.3 的目标，可尝试降低阈值或更换图片。")
    else:
        print("检测结果（置信度 ≥ 0.30）：")
        for line in lines[:10]:  # 仅展示前 10 个目标，避免刷屏
            print("  -", line)

    print(f"可视化结果已保存至：{output_dir.resolve()}")


if __name__ == "__main__":
    main()