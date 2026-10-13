import torch.nn as nn
import matplotlib.pyplot as plt
import torch
from torchvision import transforms, datasets
from torch.utils.data import DataLoader

# 定义数据预处理的转换器
transform = transforms.Compose([
    transforms.Resize((224, 224)),                                               # 调整图像大小
    transforms.ToTensor(),                                                       # 将图像转换为张量
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])  # 标准化
])

class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()
        self.conv1 = nn.Sequential(         # 输入224*224
            nn.Conv2d(
                in_channels=3,              # 彩色图片 三通道 即3*224*224
                out_channels=16,            # 输出维度
                kernel_size=5,              # 过滤器5*5
                stride=1,                   # 步长
                padding=0,                  # 四周填充
            ),                              # 输出 16*220*220
            nn.ReLU(),                      # 激活函数
            nn.MaxPool2d(kernel_size=2),    # 2*2区域取最大值->输出 16*110*110
        )
        self.conv2 = nn.Sequential(         # 输入16*110*110
            nn.Conv2d(16, 32, 5, 1, 0),     # 输出32*106*106
            nn.ReLU(),                      # 激活函数
            nn.MaxPool2d(2),                # 输出32*53*53
        )
        # 自动计算展平后的特征维度 (原视频代码为指定维度)
        self._initialize_fc()

    def _initialize_fc(self):                    # 自动计算展平后的特征维度
        with torch.no_grad():
            x = torch.zeros(1, 3, 224, 224)      # 虚拟输入
            x = self.conv1(x)
            x = self.conv2(x)
            n_features = x.view(1, -1).shape[1]  # 展平后的特征维度
        self.out = nn.Linear(n_features, 5)      # 输出5类

    def forward(self, x):
        x = self.conv1(x)         # 第一层
        x = self.conv2(x)         # 第二层
        x = x.view(x.size(0), -1) # 展品
        output = self.out(x)      # 全连接层
        return output, x    

if __name__ == '__main__':  
    # 加载训练数据集
    train_dataset = datasets.ImageFolder(root='train', transform=transform)
    train_loader = DataLoader(train_dataset, batch_size=16, shuffle=True, num_workers=4)
    # 获取标签名列表
    label_names = train_dataset.classes

    # 加载测试数据集
    test_dataset = datasets.ImageFolder(root='test', transform=transform)
    test_loader = DataLoader(test_dataset, batch_size=1, shuffle=True, num_workers=1)

    # 训练
    cnn = CNN()  # 实例化CNN模型
    optimizer = torch.optim.Adam(cnn.parameters(), lr=0.0001)   # 定义Adam优化器，设置学习率为0.0001
    loss_func = nn.CrossEntropyLoss()  # 定义交叉熵损失函数（适用于多分类任务）
    loss_list = []      # 记录损失值
    accuracy_list = []  # 记录准确率
    for epoch in range(15):  # 10个epoch
        for step, (b_x, b_y) in enumerate(train_loader):   # 遍历训练集的每个batch
            output = cnn(b_x)[0]               # 前向传播，获得模型输出
            loss = loss_func(output, b_y)      # 计算当前batch的损失
            optimizer.zero_grad()              # 梯度清零
            loss.backward()                    # 反向传播，计算梯度
            optimizer.step()                   # 更新模型参数
            test_output, last_layer = cnn(b_x) # 再次前向传播，获取输出和特征
            pred_y = torch.max(test_output, 1)[1].data.numpy()  # 获取预测类别
            accuracy = float((pred_y == b_y.data.numpy()).astype(int).sum()) / float(b_y.size(0))  # 计算准确率
            loss_list.append(loss.data.numpy())      # 记录损失
            accuracy_list.append(accuracy)           # 记录准确率
        # 测试 符合标准则提前结束（可选）
        count=0
        for step, (t_x, t_y) in enumerate(test_loader):
            test_output, _ = cnn(t_x)
            pred_y = torch.max(test_output, 1)[1].data.numpy()
            if [label_names[i] for i in pred_y] == [label_names[i] for i in t_y.numpy()]:
                count+=1
        if count==10 :
            break 

    # 绘图
    plt.figure(figsize=(10,5))
    plt.plot(range(1, len(loss_list)+1), loss_list, label='Loss')
    plt.plot(range(1, len(accuracy_list)+1), accuracy_list, label='Accuracy')
    plt.xlabel('Step')
    plt.ylabel('Value')
    plt.title('Loss and Accuracy')
    plt.legend()
    plt.show()

    # 测试
    count=0
    for step, (t_x, t_y) in enumerate(test_loader):
        test_output, _ = cnn(t_x)
        pred_y = torch.max(test_output, 1)[1].data.numpy()
        # 输出预测的标签名
        pred_names = [label_names[i] for i in pred_y]
        real_names = [label_names[i] for i in t_y.numpy()]
        print("第",step+1,"张图片测试：")
        print('prediction:', pred_names)
        print('real:', real_names)
        if pred_names == real_names:
            count+=1
    print("测试集准确率" ,count/len(test_loader))