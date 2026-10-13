# 导入必要的库
import os
import numpy as np
from sklearn import datasets, svm, metrics
from sklearn.model_selection import train_test_split
import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets as tv_datasets, transforms
from torch.utils.data import DataLoader
import time

# SVM 分类实验
# 使用 sklearn 的 Digits 数据集，评估 SVM 的分类性能
# 输出分类准确率、训练时间、推理时间和吞吐率
def run_svm_on_digits():
    digits = datasets.load_digits()
    X_train, X_test, y_train, y_test = train_test_split(digits.data, digits.target, test_size=0.2, random_state=42)
    clf = svm.SVC(gamma=0.001)
    
    # 记录训练时间
    t0 = time.time()
    clf.fit(X_train, y_train)
    train_time = time.time() - t0
    
    # 记录推理时间
    t0 = time.time()
    y_pred = clf.predict(X_test)
    predict_time = time.time() - t0
    
    # 计算准确率和吞吐率
    acc = metrics.accuracy_score(y_test, y_pred)
    train_throughput = len(X_train) / train_time if train_time > 0 else float('inf')
    predict_throughput = len(X_test) / predict_time if predict_time > 0 else float('inf')
    
    print(f"SVM 分类准确率: {acc:.4f}")
    print(f"SVM 训练耗时: {train_time:.4f}s, 训练吞吐: {train_throughput:.1f} samples/s")
    print(f"SVM 推理耗时: {predict_time:.4f}s, 推理吞吐: {predict_throughput:.1f} samples/s")
    return {
        'method': 'SVM',
        'accuracy': float(acc),
        'train_time': float(train_time),
        'predict_time': float(predict_time),
        'train_throughput': float(train_throughput),
        'predict_throughput': float(predict_throughput)
    }

# MLP 分类实验
# 使用 PyTorch 在 MNIST 数据集上训练和测试 MLP 模型
# 输出分类准确率
def run_mlp_on_mnist():
    # 数据预处理
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5,), (0.5,))
    ])
    train_dataset = tv_datasets.MNIST(root="./data", train=True, transform=transform, download=True)
    test_dataset = tv_datasets.MNIST(root="./data", train=False, transform=transform, download=True)
    train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)

    # 定义 MLP 模型
    class MLP(nn.Module):
        def __init__(self):
            super(MLP, self).__init__()
            self.fc1 = nn.Linear(28 * 28, 128)
            self.relu = nn.ReLU()
            self.fc2 = nn.Linear(128, 10)

        def forward(self, x):
            x = x.view(-1, 28 * 28)
            x = self.relu(self.fc1(x))
            x = self.fc2(x)
            return x

    model = MLP()
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-3)

    # 模型训练（计时）
    model.train()
    t0 = time.time()
    for epoch in range(3):
        for images, labels in train_loader:
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
    train_time = time.time() - t0

    # 模型评估（计时）
    model.eval()
    correct = 0
    total = 0
    t0 = time.time()
    with torch.no_grad():
        for images, labels in test_loader:
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    predict_time = time.time() - t0

    accuracy = correct / total
    predict_throughput = total / predict_time if predict_time > 0 else float('inf')
    print(f"MLP Accuracy: {accuracy:.4f}")
    print(f"MLP 训练耗时: {train_time:.4f}s, MLP 推理耗时: {predict_time:.4f}s, 推理吞吐: {predict_throughput:.1f} samples/s")

    # 保存模型
    os.makedirs(os.path.join(os.path.dirname(__file__), '..', 'models'), exist_ok=True)
    model_path = os.path.join(os.path.dirname(__file__), '..', 'models', 'mlp_mnist.pth')
    try:
        torch.save(model.state_dict(), model_path)
    except Exception:
        pass

    return {
        'method': 'MLP',
        'accuracy': float(accuracy),
        'train_time': float(train_time),
        'predict_time': float(predict_time),
        'predict_throughput': float(predict_throughput)
    }


def run_svm_on_mnist(sub_train=5000, sub_test=2000):
    # 在 MNIST 上运行 SVM（使用子集以控制计算量）
    train_dataset = tv_datasets.MNIST(root="./data", train=True, download=True)
    test_dataset = tv_datasets.MNIST(root="./data", train=False, download=True)
    X_train_full = train_dataset.data.numpy().reshape(-1, 28*28)
    y_train_full = train_dataset.targets.numpy()
    X_test_full = test_dataset.data.numpy().reshape(-1, 28*28)
    y_test_full = test_dataset.targets.numpy()

    # 采样子集
    from sklearn.model_selection import train_test_split
    X_train_sub, _, y_train_sub, _ = train_test_split(X_train_full, y_train_full, train_size=sub_train, stratify=y_train_full, random_state=42)
    X_test_sub, _, y_test_sub, _ = train_test_split(X_test_full, y_test_full, train_size=sub_test, stratify=y_test_full, random_state=42)

    clf = svm.SVC(gamma=0.001)
    t0 = time.time()
    clf.fit(X_train_sub, y_train_sub)
    train_time = time.time() - t0

    t0 = time.time()
    y_pred = clf.predict(X_test_sub)
    predict_time = time.time() - t0

    acc = metrics.accuracy_score(y_test_sub, y_pred)
    train_throughput = len(X_train_sub) / train_time if train_time > 0 else float('inf')
    predict_throughput = len(X_test_sub) / predict_time if predict_time > 0 else float('inf')

    print(f"SVM(MNIST-subset) Accuracy: {acc:.4f}")
    print(f"SVM(MNIST-subset) Train: {train_time:.4f}s, Predict: {predict_time:.4f}s")

    return {
        'method': 'SVM_MNIST_sub',
        'accuracy': float(acc),
        'train_time': float(train_time),
        'predict_time': float(predict_time),
        'train_throughput': float(train_throughput),
        'predict_throughput': float(predict_throughput)
    }


def run_mlp_on_digits():
    # 在 Digits 数据集上训练一个小型 MLP（输入维度 64）
    digits = datasets.load_digits()
    X_train, X_test, y_train, y_test = train_test_split(digits.data, digits.target, test_size=0.2, random_state=42)
    # 转为 torch Dataset
    import torch.utils.data as data_utils
    X_train_t = torch.tensor(X_train, dtype=torch.float32)
    y_train_t = torch.tensor(y_train, dtype=torch.long)
    X_test_t = torch.tensor(X_test, dtype=torch.float32)
    y_test_t = torch.tensor(y_test, dtype=torch.long)

    train_ds = data_utils.TensorDataset(X_train_t, y_train_t)
    test_ds = data_utils.TensorDataset(X_test_t, y_test_t)
    train_loader = DataLoader(train_ds, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_ds, batch_size=32, shuffle=False)

    class SmallMLP(nn.Module):
        def __init__(self):
            super(SmallMLP, self).__init__()
            self.fc1 = nn.Linear(64, 64)
            self.relu = nn.ReLU()
            self.fc2 = nn.Linear(64, 10)

        def forward(self, x):
            x = self.relu(self.fc1(x))
            x = self.fc2(x)
            return x

    model = SmallMLP()
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-3)

    # 训练计时
    model.train()
    t0 = time.time()
    for epoch in range(5):
        for images, labels in train_loader:
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
    train_time = time.time() - t0

    # 评估计时
    model.eval()
    correct = 0
    total = 0
    t0 = time.time()
    with torch.no_grad():
        for images, labels in test_loader:
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    predict_time = time.time() - t0

    accuracy = correct / total
    predict_throughput = total / predict_time if predict_time > 0 else float('inf')
    print(f"MLP(Digits) Accuracy: {accuracy:.4f}")

    return {
        'method': 'MLP_Digits',
        'accuracy': float(accuracy),
        'train_time': float(train_time),
        'predict_time': float(predict_time),
        'predict_throughput': float(predict_throughput)
    }

if __name__ == "__main__":
    results = {}
    os.makedirs(os.path.join(os.path.dirname(__file__), '..', 'results'), exist_ok=True)
    # Run all experiments: SVM on Digits, MLP on Digits, SVM on MNIST(subset), MLP on MNIST
    svm_digits_res = run_svm_on_digits()
    mlp_digits_res = run_mlp_on_digits()
    svm_mnist_res = run_svm_on_mnist(sub_train=5000, sub_test=2000)
    mlp_mnist_res = run_mlp_on_mnist()
    results['svm_digits'] = svm_digits_res
    results['mlp_digits'] = mlp_digits_res
    results['svm_mnist_sub'] = svm_mnist_res
    results['mlp_mnist'] = mlp_mnist_res
    # 写入带时间戳的 JSON
    from datetime import datetime
    ts = datetime.now().strftime('%Y%m%dT%H%M%S')
    out_path = os.path.join(os.path.dirname(__file__), '..', 'results', f'results_{ts}.json')
    try:
        import json
        with open(out_path, 'w') as f:
            json.dump(results, f, indent=2)
        print(f"实验结果已保存: {out_path}")
    except Exception as e:
        print(f"保存结果失败: {e}")
