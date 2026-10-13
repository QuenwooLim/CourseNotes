import os
import cv2
import numpy as np
import pandas as pd
import torch
import torch.nn as nn
import re
import random
import matplotlib.pyplot as plt  # 新增
from siamese_network import SiameseBambooNetwork, extract_contour_features, extract_texture_features, predict_similarity, visualize_matching
import shutil

#支持中文
plt.rcParams['font.sans-serif'] = ['SimHei']  # 设置中文字体
plt.rcParams['axes.unicode_minus'] = False  # 解决负号显示问题



def load_image(img_path):
    """加载图片"""
    img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    return img

def resize_image(img, target_size=(256, 256)):
    """调整图片大小"""
    if img is None:
        return None
    return cv2.resize(img, target_size)

def extract_jianhao(fname):
    """从文件名中提取简号"""
    # 匹配数字开头的简号，如 0004, 0005 等
    match = re.match(r'^(\d+)', fname)
    if match:
        return match.group(1)
    return None

def normalize_jian_name(fname):
    """标准化简名，去除彩色/红外后缀，统一格式"""
    # 移除彩色后缀 _c
    fname = re.sub(r'_c\.(jpg|jpeg|png|bmp)$', r'.\1', fname)
    # 移除彩色后缀 _c
    fname = re.sub(r'_c$', '', fname)
    return fname

def get_base_jian_name(fname):
    """获取简的基础名称（去除所有后缀）"""
    # 移除文件扩展名
    base = os.path.splitext(fname)[0]
    # 移除彩色后缀
    base = re.sub(r'_c$', '', base)
    return base

def get_jian_number(fname):
    """获取简的纯数字编号，用于分组"""
    match = re.match(r'^(\d+)', fname)
    if match:
        return match.group(1)
    return None

def is_complete_bamboo(img):
    """判断是否为完整简 - 大幅放宽标准版"""
    if img is None:
        return False
    
    # 基本特征提取
    h, w = img.shape
    
    # 边缘检测
    edges = cv2.Canny(img, 50, 150)
    
    # 轮廓检测
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if not contours:
        return False
    
    # 找到最大轮廓
    contour = max(contours, key=cv2.contourArea)
    
    # 计算轮廓的边界矩形
    x, y, width, height = cv2.boundingRect(contour)
    
    # 计算轮廓面积与边界矩形面积的比值（完整度）
    contour_area = cv2.contourArea(contour)
    rect_area = width * height
    completeness = contour_area / rect_area if rect_area > 0 else 0
    
    # 检查边缘平整度 - 大幅放宽
    # 计算轮廓的周长
    perimeter = cv2.arcLength(contour, True)
    # 计算轮廓的近似多边形
    epsilon = 0.05 * perimeter  # 放宽近似精度
    approx = cv2.approxPolyDP(contour, epsilon, True)
    
    # 边缘平整度：近似多边形的顶点数越少，边缘越平整
    edge_smoothness = len(approx)
    
    # 检查宽度一致性
    width_consistency = width / height if height > 0 else 0
    
    # 检查轮廓的凸性
    hull = cv2.convexHull(contour)
    hull_area = cv2.contourArea(hull)
    solidity = contour_area / hull_area if hull_area > 0 else 0
    
    # 检查边缘的规则性
    # 计算轮廓的边界框
    rect = cv2.minAreaRect(contour)
    box = cv2.boxPoints(rect)
    box = box.astype(np.int32)
    
    # 计算边界框的面积
    box_area = cv2.contourArea(box)
    box_ratio = contour_area / box_area if box_area > 0 else 0
    
    # 检查边缘的规则性 - 大幅放宽
    # 计算轮廓的边界矩形
    rect_x, rect_y, rect_w, rect_h = cv2.boundingRect(contour)
    
    # 检查是否为矩形（竹简通常是矩形）
    aspect_ratio = rect_w / rect_h if rect_h > 0 else 0
    
    # 检查边缘的直线度 - 放宽要求
    # 使用霍夫变换检测直线
    lines = cv2.HoughLines(edges, 1, np.pi/180, threshold=30)  # 降低阈值
    line_count = 0 if lines is None else len(lines)
    
    # 检查轮廓的规则性
    # 计算轮廓的周长与面积的比值
    perimeter_area_ratio = perimeter / contour_area if contour_area > 0 else 0
    
    # 完整简的判断标准 - 大幅放宽
    # 主要判断条件 - 只要基本符合竹简形状即可
    basic_conditions = (
        completeness > 0.3 and  # 轮廓完整度（大幅降低）
        edge_smoothness <= 20 and  # 边缘顶点数（大幅放宽）
        0.05 < width_consistency < 0.95 and  # 宽度比例（大幅放宽）
        solidity > 0.5 and  # 轮廓凸性（大幅放宽）
        box_ratio > 0.4  # 边界框填充率（大幅放宽）
    )
    
    # 竹简特有的判断条件 - 大幅放宽
    bamboo_conditions = (
        0.02 < aspect_ratio < 0.9 and  # 长宽比（大幅放宽）
        perimeter_area_ratio < 1.0 and  # 周长面积比（大幅放宽）
        line_count >= 1  # 至少有1条直线边缘（降低要求）
    )
    
    # 综合判断 - 只要满足基本条件即可
    if basic_conditions and bamboo_conditions:
        return True
    
    # 特殊情况：如果基本条件较好，直接认为是完整简
    if (completeness > 0.4 and 
        edge_smoothness <= 15 and 
        solidity > 0.6 and 
        box_ratio > 0.5):
        return True
    
    # 更宽松的判断：只要形状大致符合竹简特征
    if (completeness > 0.25 and 
        aspect_ratio > 0.02 and aspect_ratio < 0.95 and
        width_consistency > 0.02 and width_consistency < 0.98):
        return True
    
    return False

def extract_group_order(excel_path):
    """提取缀合组与简号顺序信息"""
    try:
        df = pd.read_excel(excel_path, sheet_name=0)
        group_order = {}
        for idx, row in df.iterrows():
            group = str(row.iloc[0]).strip()
            jian_list = []
            for v in row[1:-1]:
                if pd.isna(v):
                    continue
                v = str(v).strip()
                if v and v != 'nan':
                    jian_list.append(v)
            if jian_list:
                group_order[group] = jian_list
        return group_order
    except Exception as e:
        print(f"读取Excel文件失败: {e}")
        return {}

def find_image_by_jianhao(folder, jianhao):
    """根据简号查找图片"""
    if not os.path.exists(folder):
        return None
        
    # 简号预处理
    match = re.search(r'-(\d+)', jianhao)
    if match:
        number = match.group(1).zfill(4)
    else:
        number = jianhao
    
    # 查找图片文件
    img_candidates = [f for f in os.listdir(folder)
                     if number in f and f.lower().endswith(('.jpg', '.png', '.jpeg'))]
    
    if img_candidates:
        img_path = os.path.join(folder, img_candidates[0])
        img = load_image(img_path)
        if img is not None:
            return resize_image(img)
    return None

def create_training_pairs_from_all_yizhuihe(yizhuihe_folder, group_order):
    """从所有已缀合数据创建训练对，提高训练效果"""
    positive_pairs = []
    negative_pairs = []
    
    print("从所有已缀合组创建训练数据...")
    
    # 收集所有图片用于负样本生成
    all_images = []
    all_jian_numbers = []
    
    for group, jian_list in group_order.items():
        group_folder = os.path.join(yizhuihe_folder, group)
        if not os.path.exists(group_folder):
            continue
            
        for jianhao in jian_list:
            img = find_image_by_jianhao(group_folder, jianhao)
            if img is not None:
                all_images.append((group, jianhao, img))
                all_jian_numbers.append(jianhao)
    
    print(f"总共收集到 {len(all_images)} 张图片")
    
    # 创建正样本对（同一组内相邻的简）
    for group, jian_list in group_order.items():
        group_folder = os.path.join(yizhuihe_folder, group)
        if not os.path.exists(group_folder):
            continue
            
        # 创建正样本对（相邻的简）
        for i in range(len(jian_list) - 1):
            jian1, jian2 = jian_list[i], jian_list[i+1]
            
            img1 = find_image_by_jianhao(group_folder, jian1)
            img2 = find_image_by_jianhao(group_folder, jian2)
            
            if img1 is not None and img2 is not None:
                positive_pairs.append((img1, img2))
    
    # 创建负样本对（不同组的简，确保多样性）
    negative_count = 0
    max_negative = len(positive_pairs) * 2  # 负样本数量为正样本的2倍
    
    for i, (group1, jian1, img1) in enumerate(all_images):
        for j, (group2, jian2, img2) in enumerate(all_images):
            if i >= j:  # 避免重复
                continue
                
            # 不同组的简作为负样本
            if group1 != group2:
                negative_pairs.append((img1, img2))
                negative_count += 1
                
                if negative_count >= max_negative:
                    break
        
        if negative_count >= max_negative:
            break
    
    print(f"创建训练对完成：正样本 {len(positive_pairs)} 对，负样本 {len(negative_pairs)} 对")
    return positive_pairs, negative_pairs

def data_augmentation(img):
    """数据增强：旋转、翻转、亮度调整"""
    augmented = []
    
    # 原图
    augmented.append(img)
    
    # 水平翻转
    augmented.append(cv2.flip(img, 1))
    
    # 垂直翻转
    augmented.append(cv2.flip(img, 0))
    
    # 亮度调整
    bright = np.clip(img * 1.2, 0, 255).astype(np.uint8)
    augmented.append(bright)
    
    dark = np.clip(img * 0.8, 0, 255).astype(np.uint8)
    augmented.append(dark)
    
    # 小角度旋转
    h, w = img.shape
    center = (w // 2, h // 2)
    for angle in [-5, 5]:
        M = cv2.getRotationMatrix2D(center, angle, 1.0)
        rotated = cv2.warpAffine(img, M, (w, h))
        augmented.append(rotated)
    
    return augmented

def create_augmented_training_pairs(positive_pairs, negative_pairs, augment_ratio=0.3):
    """创建增强的训练数据"""
    augmented_positive = []
    augmented_negative = []
    
    # 增强正样本
    for img1, img2 in positive_pairs:
        augmented_positive.append((img1, img2))
        
        # 随机增强
        if random.random() < augment_ratio:
            aug1_list = data_augmentation(img1)
            aug2_list = data_augmentation(img2)
            
            # 随机选择增强版本
            aug1 = random.choice(aug1_list)
            aug2 = random.choice(aug2_list)
            augmented_positive.append((aug1, aug2))
    
    # 增强负样本
    for img1, img2 in negative_pairs:
        augmented_negative.append((img1, img2))
        
        # 随机增强
        if random.random() < augment_ratio:
            aug1_list = data_augmentation(img1)
            aug2_list = data_augmentation(img2)
            
            aug1 = random.choice(aug1_list)
            aug2 = random.choice(aug2_list)
            augmented_negative.append((aug1, aug2))
    
    return augmented_positive, augmented_negative

class ContrastiveLoss(nn.Module):
    """对比损失函数，更适合孪生网络"""
    def __init__(self, margin=1.0):
        super(ContrastiveLoss, self).__init__()
        self.margin = margin
        
    def forward(self, similarity, label):
        # 正样本：相似度应该高
        # 负样本：相似度应该低
        pos_loss = (1 - label) * torch.pow(similarity, 2)
        neg_loss = label * torch.pow(torch.clamp(self.margin - similarity, min=0.0), 2)
        loss = pos_loss + neg_loss
        return torch.mean(loss)

def train_siamese_network(positive_pairs, negative_pairs, num_epochs=10):
    """训练孪生网络"""
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    print(f"使用设备: {device}")
    
    # 获取特征维度
    if positive_pairs:
        img1, img2 = positive_pairs[0]
        contour_feat, _ = extract_contour_features(img1)
        texture_feat, _ = extract_texture_features(img1)
        contour_dim = contour_feat.shape[1]
        texture_dim = texture_feat.shape[1]
        print(f"特征维度: contour={contour_dim}, texture={texture_dim}")
    else:
        contour_dim = 5
        texture_dim = 18
        print("使用默认特征维度")

    # 初始化网络
    model = SiameseBambooNetwork(contour_dim=contour_dim, texture_dim=texture_dim)
    model = model.to(device)
    
    # 使用对比损失函数
    criterion = ContrastiveLoss(margin=2.0)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.0001, weight_decay=1e-4)
    scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=10, gamma=0.5)
    
    # 创建增强的训练数据
    print("创建增强训练数据...")
    aug_positive, aug_negative = create_augmented_training_pairs(positive_pairs, negative_pairs)
    
    # 准备训练数据
    train_pairs = []
    train_labels = []
    
    # 正样本 (label=0表示相似)
    for img1, img2 in aug_positive[:50]:
        train_pairs.append((img1, img2))
        train_labels.append(0.0)  # 相似
    
    # 负样本 (label=1表示不相似)
    for img1, img2 in aug_negative[:50]:
        train_pairs.append((img1, img2))
        train_labels.append(1.0)  # 不相似
    
    print(f"训练样本数: {len(train_pairs)} (正样本: {len([l for l in train_labels if l == 0])}, 负样本: {len([l for l in train_labels if l == 1])})")
    
    # 训练
    model.train()
    best_accuracy = 0.0
    
    for epoch in range(num_epochs):
        total_loss = 0
        correct = 0
        processed = 0
        
        # 打乱训练数据
        indices = list(range(len(train_pairs)))
        random.shuffle(indices)
        
        for idx in indices:
            (img1, img2), label = train_pairs[idx], train_labels[idx]
            
            try:
                # 提取特征
                contour_feat1, adj1 = extract_contour_features(img1)
                texture_feat1, adj1_texture = extract_texture_features(img1)
                
                contour_feat2, adj2 = extract_contour_features(img2)
                texture_feat2, adj2_texture = extract_texture_features(img2)
                
                if (contour_feat1 is None or contour_feat2 is None or 
                    texture_feat1 is None or texture_feat2 is None):
                    continue
                
                # 转换为张量
                x1_contour = torch.FloatTensor(contour_feat1).to(device)
                x1_texture = torch.FloatTensor(texture_feat1).to(device)
                adj1 = torch.FloatTensor(adj1).to(device)
                
                x2_contour = torch.FloatTensor(contour_feat2).to(device)
                x2_texture = torch.FloatTensor(texture_feat2).to(device)
                adj2 = torch.FloatTensor(adj2).to(device)
                
                label_tensor = torch.FloatTensor([label]).to(device)
                
                # 前向传播
                optimizer.zero_grad()
                similarity, _, _ = model(x1_contour, x1_texture, adj1, x2_contour, x2_texture, adj2)
                similarity = similarity.unsqueeze(0)
                loss = criterion(similarity, label_tensor)
                
                # 反向传播
                loss.backward()
                optimizer.step()
                
                total_loss += loss.item()
                
                # 计算准确率 (相似度阈值0.5)
                pred = (similarity < 0.5).float()  # 相似度低表示不相似
                correct += (pred == label_tensor).float().item()
                processed += 1
                
            except Exception as e:
                print(f"训练样本出错: {e}")
                continue
        
        scheduler.step()
        
        if processed > 0:
            accuracy = correct / processed
            avg_loss = total_loss / processed
            print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {avg_loss:.4f}, Accuracy: {accuracy:.4f}, LR: {scheduler.get_last_lr()[0]:.6f}')
            
            # 保存最佳模型
            if accuracy > best_accuracy:
                best_accuracy = accuracy
                torch.save(model.state_dict(), 'best_siamese_model.pth')
                print(f"保存最佳模型，准确率: {best_accuracy:.4f}")
    
    # 加载最佳模型
    model.load_state_dict(torch.load('best_siamese_model.pth'))
    print(f"训练完成，最佳准确率: {best_accuracy:.4f}")
    
    return model

def classify_complete_and_incomplete_bamboo(weizhuihe_folder, output_folder='classified_bamboo'):
    """利用形状特性判断完整和不完整简，只保存完整简"""
    print(f"\n=== 利用形状特性分类竹简（只保存完整简）===")
    
    # 创建输出目录
    complete_folder = os.path.join(output_folder, 'complete')
    os.makedirs(complete_folder, exist_ok=True)
    
    # 获取所有图片
    img_files = [f for f in os.listdir(weizhuihe_folder) 
                 if f.lower().endswith(('.jpg', '.jpeg', '.png', '.bmp'))]
    
    print(f"找到 {len(img_files)} 张图片")
    
    # 按简号分组，避免同一支简的不同版本被重复处理
    jian_groups = {}
    for fname in img_files:
        jian_number = get_jian_number(fname)
        if jian_number:
            if jian_number not in jian_groups:
                jian_groups[jian_number] = []
            jian_groups[jian_number].append(fname)
    
    print(f"分组后得到 {len(jian_groups)} 支不同的简")
    
    # 分析每支简
    complete_count = 0
    incomplete_count = 0
    
    for jian_number, files in jian_groups.items():
        # 优先选择红外版本（无_c后缀）
        preferred_file = None
        for f in files:
            if not f.endswith('_c.jpg') and not f.endswith('_c.jpeg') and not f.endswith('_c.png'):
                preferred_file = f
                break
        
        if not preferred_file:
            preferred_file = files[0]  # 如果没有红外版，选择第一个
        
        img_path = os.path.join(weizhuihe_folder, preferred_file)
        img = load_image(img_path)
        
        if img is not None:
            # 使用原图进行判断
            if is_complete_bamboo(img):
                # 复制到完整简文件夹
                output_path = os.path.join(complete_folder, preferred_file)
                shutil.copy2(img_path, output_path)
                complete_count += 1
                print(f"简号{jian_number}: 完整简 -> {output_path}")
            else:
                incomplete_count += 1
                print(f"简号{jian_number}: 残断简（不保存）")
    
    print(f"\n分类完成！")
    print(f"完整简数量: {complete_count}，保存在 {complete_folder}")
    print(f"残断简数量: {incomplete_count}（未保存）")
    
    return complete_folder, None  # 返回None表示没有不完整简文件夹

def auto_stitch_with_siamese_from_classified(incomplete_folder, model, threshold=0.3, vis_top_n=10):
    """使用孪生网络对分类后的不完整简进行自动缀合"""
    print(f"\n使用孪生网络对不完整简进行自动缀合...")
    
    # 获取所有不完整简图片
    img_files = [f for f in os.listdir(incomplete_folder) 
                 if f.lower().endswith(('.jpg', '.jpeg', '.png', '.bmp'))]
    
    if len(img_files) < 2:
        print("不完整简数量不足，无法进行缀合")
        return []
    
    print(f"对 {len(img_files)} 支不完整简进行缀合分析...")
    
    # 加载不完整简图片
    broken_images = []
    for fname in img_files[:50]:  # 限制数量以提高效率
        jian_number = get_jian_number(fname)
        img_path = os.path.join(incomplete_folder, fname)
        img = load_image(img_path)
        if img is not None:
            img_resized = resize_image(img)  # 用于模型预测
            broken_images.append((jian_number, fname, img, img_resized))
    
    print(f"成功加载 {len(broken_images)} 支不完整简图片")
    
    # 两两比较不完整简
    stitch_pairs = []
    similarities = []
    
    for i in range(len(broken_images)):
        for j in range(i+1, len(broken_images)):
            jian_number1, fname1, img1_orig, img1_resized = broken_images[i]
            jian_number2, fname2, img2_orig, img2_resized = broken_images[j]
            
            try:
                similarity = predict_similarity(model, img1_resized, img2_resized)
                similarities.append((jian_number1, jian_number2, fname1, fname2, similarity))
                
                if similarity < threshold:  # 相似度低表示可缀合
                    stitch_pairs.append((jian_number1, jian_number2, fname1, fname2, similarity))
                    print(f"可缀合: 简号{jian_number1} <--> 简号{jian_number2} (相似度: {similarity:.4f})")
                    
            except Exception as e:
                print(f"比较简号{jian_number1} 和简号{jian_number2} 时出错: {e}")
                continue
    
    # 按相似度排序
    similarities.sort(key=lambda x: x[4])
    print(f"\n相似度最高的5对:")
    for jian_number1, jian_number2, fname1, fname2, sim in similarities[:5]:
        print(f"  简号{jian_number1} <--> 简号{jian_number2} (相似度: {sim:.4f})")
    
    # 可视化找到的可缀合简对
    if stitch_pairs:
        visualize_stitching_pairs_from_folder(stitch_pairs, incomplete_folder, vis_top_n)
    
    return stitch_pairs

def visualize_stitching_pairs_from_folder(stitch_pairs, folder_path, top_n=10):
    """从指定文件夹可视化找到的可缀合简对"""
    if not stitch_pairs:
        print("没有找到可缀合的简对")
        return
    
    print(f"\n=== 可视化前 {min(top_n, len(stitch_pairs))} 个可缀合简对 ===")
    
    # 创建输出目录
    os.makedirs('stitching_visualizations', exist_ok=True)
    
    # 选择前N个最可能缀合的简对（相似度最低的）
    top_pairs = sorted(stitch_pairs, key=lambda x: x[4])[:top_n]
    
    for idx, (jian_number1, jian_number2, fname1, fname2, similarity) in enumerate(top_pairs):
        print(f"\n可视化简对 {idx+1}: 简号{jian_number1} <--> 简号{jian_number2} (相似度: {similarity:.4f})")
        
        # 加载图片 - 使用原图，不调整大小
        img1_path = os.path.join(folder_path, fname1)
        img2_path = os.path.join(folder_path, fname2)
        
        img1 = load_image(img1_path)
        img2 = load_image(img2_path)
        
        if img1 is not None and img2 is not None:
            # 创建可视化 - 垂直拼接格式
            fig, ax = plt.subplots(1, 1, figsize=(12, 16))
            fig.suptitle(f'可缀合简对 {idx+1}: 简号{jian_number1} <--> 简号{jian_number2} (相似度: {similarity:.4f})', 
                        fontsize=16, fontweight='bold')
            
            # 垂直拼接两张图片 - 使用原图
            if img1.shape[1] != img2.shape[1]:
                # 统一宽度，保持原图质量
                min_width = min(img1.shape[1], img2.shape[1])
                h1, w1 = img1.shape
                h2, w2 = img2.shape
                new_h1 = int(h1 * min_width / w1)
                new_h2 = int(h2 * min_width / w2)
                img1_resized = cv2.resize(img1, (min_width, new_h1), interpolation=cv2.INTER_LANCZOS4)
                img2_resized = cv2.resize(img2, (min_width, new_h2), interpolation=cv2.INTER_LANCZOS4)
            else:
                img1_resized = img1
                img2_resized = img2
            
            # 垂直拼接
            combined_img = np.vstack([img1_resized, img2_resized])
            
            # 显示拼接后的图片
            ax.imshow(combined_img, cmap='gray')
            ax.set_title('缀合结果 (自上而下)', fontsize=14, fontweight='bold')
            ax.axis('off')
            
            plt.tight_layout()
            
            # 保存结果
            output_path = f'stitching_visualizations/pair_{idx+1}_jian{jian_number1}_jian{jian_number2}_vertical.png'
            plt.savefig(output_path, dpi=300, bbox_inches='tight')
            print(f"  保存到: {output_path}")
            
            plt.close()  # 关闭图形以节省内存
        else:
            print(f"  无法加载图片: {fname1} 或 {fname2}")
    
    print(f"\n可视化完成！共生成 {len(top_pairs)} 个可视化文件，保存在 stitching_visualizations/ 目录下")

def visualize_all_yizhuihe_groups(yizhuihe_folder, excel_path):
    """可视化yizhuihe文件夹中所有缀合组的结果"""
    print("\n=== 可视化所有已缀合组的结果 ===")
    
    # 提取缀合组信息
    group_order = extract_group_order(excel_path)
    print(f"找到 {len(group_order)} 个缀合组")
    
    # 创建输出目录
    os.makedirs('yizhuihe_visualizations', exist_ok=True)
    
    # 遍历每个缀合组
    for group_name, jian_list in group_order.items():
        print(f"\n处理缀合组 {group_name}...")
        
        group_folder = os.path.join(yizhuihe_folder, group_name)
        if not os.path.exists(group_folder):
            print(f"  缀合组 {group_name} 文件夹不存在，跳过")
            continue
        
        # 获取该组的所有图片文件
        img_files = [f for f in os.listdir(group_folder) 
                     if f.lower().endswith(('.jpg', '.jpeg', '.png', '.bmp'))]
        
        if not img_files:
            print(f"  缀合组 {group_name} 没有图片文件，跳过")
            continue
        
        print(f"  找到 {len(img_files)} 张图片")
        
        # 分离正面(a)和背面(b)图片
        front_images = []
        back_images = []
        
        for img_file in img_files:
            if img_file.endswith('_a.jpg') or img_file.endswith('_a.jpeg') or img_file.endswith('_a.png'):
                front_images.append(img_file)
            elif img_file.endswith('_b.jpg') or img_file.endswith('_b.jpeg') or img_file.endswith('_b.png'):
                back_images.append(img_file)
        
        # 检查是否有正反面图片
        has_front_back = len(front_images) > 0 and len(back_images) > 0
        
        if has_front_back:
            # 有正反面图片，按原来的方式处理
            print(f"  检测到正反面图片格式")
            
            # 按缀合顺序排序图片
            ordered_front = []
            ordered_back = []
            
            for jianhao in jian_list:
                # 从简号中提取数字部分
                match = re.search(r'-(\d+)', jianhao)
                if match:
                    number = match.group(1)
                    # 查找对应的正面图片
                    for front_img in front_images:
                        if number in front_img:
                            ordered_front.append(front_img)
                            break
                    # 查找对应的背面图片
                    for back_img in back_images:
                        if number in back_img:
                            ordered_back.append(back_img)
                            break
            
            # 如果按缀合顺序找不到，就按文件名排序
            if not ordered_front:
                ordered_front = sorted(front_images)
            if not ordered_back:
                ordered_back = sorted(back_images)
            
            print(f"  正面图片: {len(ordered_front)} 张")
            print(f"  背面图片: {len(ordered_back)} 张")
            
            # 创建双列可视化
            if ordered_front or ordered_back:
                # 创建图形
                fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 12))
                # fig.suptitle(f'缀合组 {group_name} 可视化结果', fontsize=16, fontweight='bold')
                
                # 处理正面图片
                if ordered_front:
                    front_combined = []
                    for i, img_file in enumerate(ordered_front):
                        img_path = os.path.join(group_folder, img_file)
                        img = load_image(img_path)
                        if img is not None:
                            front_combined.append(img)
                    
                    if front_combined:
                        # 统一宽度
                        min_width = min(img.shape[1] for img in front_combined)
                        resized_front = []
                        for img in front_combined:
                            h, w = img.shape
                            new_h = int(h * min_width / w)
                            resized = cv2.resize(img, (min_width, new_h), interpolation=cv2.INTER_LANCZOS4)
                            resized_front.append(resized)
                        
                        # 垂直拼接
                        combined_front = np.vstack(resized_front)
                        ax1.imshow(combined_front, cmap='gray')
                        ax1.set_title(f'正面 (共{len(ordered_front)}张)', fontsize=14, fontweight='bold')
                        ax1.axis('off')
                        
                        # # 添加分隔线
                        # y_pos = 0
                        # for i, img in enumerate(resized_front[:-1]):
                        #     y_pos += img.shape[0]
                        #     ax1.axhline(y=y_pos, color='red', linestyle='--', linewidth=1, alpha=0.7)
                        
                        # # 添加标签
                        # y_pos = 0
                        # for i, img_file in enumerate(ordered_front):
                        #     ax1.text(10, y_pos + 30, f'{img_file}', fontsize=10, color='red', 
                        #            bbox=dict(boxstyle="round,pad=0.2", facecolor="white", alpha=0.8))
                        #     y_pos += resized_front[i].shape[0]
                    else:
                        ax1.text(0.5, 0.5, '无正面图片', ha='center', va='center', transform=ax1.transAxes, fontsize=14)
                        ax1.axis('off')
                else:
                    ax1.text(0.5, 0.5, '无正面图片', ha='center', va='center', transform=ax1.transAxes, fontsize=14)
                    ax1.axis('off')
                
                # 处理背面图片
                if ordered_back:
                    back_combined = []
                    for i, img_file in enumerate(ordered_back):
                        img_path = os.path.join(group_folder, img_file)
                        img = load_image(img_path)
                        if img is not None:
                            back_combined.append(img)
                    
                    if back_combined:
                        # 统一宽度
                        min_width = min(img.shape[1] for img in back_combined)
                        resized_back = []
                        for img in back_combined:
                            h, w = img.shape
                            new_h = int(h * min_width / w)
                            resized = cv2.resize(img, (min_width, new_h), interpolation=cv2.INTER_LANCZOS4)
                            resized_back.append(resized)
                        
                        # 垂直拼接
                        combined_back = np.vstack(resized_back)
                        ax2.imshow(combined_back, cmap='gray')
                        ax2.set_title(f'背面 (共{len(ordered_back)}张)', fontsize=14, fontweight='bold')
                        ax2.axis('off')
                        
                        # # 添加分隔线
                        # y_pos = 0
                        # for i, img in enumerate(resized_back[:-1]):
                        #     y_pos += img.shape[0]
                        #     ax2.axhline(y=y_pos, color='red', linestyle='--', linewidth=1, alpha=0.7)
                        
                        # # 添加标签
                        # y_pos = 0
                        # for i, img_file in enumerate(ordered_back):
                        #     ax2.text(10, y_pos + 30, f'{img_file}', fontsize=10, color='red', 
                        #            bbox=dict(boxstyle="round,pad=0.2", facecolor="white", alpha=0.8))
                        #     y_pos += resized_back[i].shape[0]
                    else:
                        ax2.text(0.5, 0.5, '无背面图片', ha='center', va='center', transform=ax2.transAxes, fontsize=14)
                        ax2.axis('off')
                else:
                    ax2.text(0.5, 0.5, '无背面图片', ha='center', va='center', transform=ax2.transAxes, fontsize=14)
                    ax2.axis('off')
                
                plt.tight_layout()
                
                # 保存结果
                output_path = f'yizhuihe_visualizations/group_{group_name}_visualization.png'
                plt.savefig(output_path, dpi=300, bbox_inches='tight')
                print(f"  保存到: {output_path}")
                
                plt.close()  # 关闭图形以节省内存
            else:
                print(f"  缀合组 {group_name} 没有有效图片，跳过")
        
        else:
            # 没有正反面图片，将所有图片显示在一面
            print(f"  未检测到正反面图片格式，将所有图片显示在一面")
            
            # 按缀合顺序排序所有图片
            ordered_images = []
            
            for jianhao in jian_list:
                # 从简号中提取数字部分
                match = re.search(r'-(\d+)', jianhao)
                if match:
                    number = match.group(1)
                    # 查找对应的图片
                    for img_file in img_files:
                        if number in img_file:
                            ordered_images.append(img_file)
                            break
            
            # 如果按缀合顺序找不到，就按文件名排序
            if not ordered_images:
                ordered_images = sorted(img_files)
            
            print(f"  图片总数: {len(ordered_images)} 张")
            
            # 创建单列可视化
            if ordered_images:
                # 创建图形
                fig, ax = plt.subplots(1, 1, figsize=(12, 16))
                # fig.suptitle(f'缀合组 {group_name} 可视化结果', fontsize=16, fontweight='bold')
                
                # 处理所有图片
                all_combined = []
                for i, img_file in enumerate(ordered_images):
                    img_path = os.path.join(group_folder, img_file)
                    img = load_image(img_path)
                    if img is not None:
                        all_combined.append(img)
                
                if all_combined:
                    # 统一宽度
                    min_width = min(img.shape[1] for img in all_combined)
                    resized_images = []
                    for img in all_combined:
                        h, w = img.shape
                        new_h = int(h * min_width / w)
                        resized = cv2.resize(img, (min_width, new_h), interpolation=cv2.INTER_LANCZOS4)
                        resized_images.append(resized)
                    
                    # 垂直拼接
                    combined_all = np.vstack(resized_images)
                    ax.imshow(combined_all, cmap='gray')
                    ax.set_title(f'缀合结果 (共{len(ordered_images)}张)', fontsize=14, fontweight='bold')
                    ax.axis('off')
                    
                    # # 添加分隔线
                    # y_pos = 0
                    # for i, img in enumerate(resized_images[:-1]):
                    #     y_pos += img.shape[0]
                    #     ax.axhline(y=y_pos, color='red', linestyle='--', linewidth=1, alpha=0.7)
                    
                    # # 添加标签
                    # y_pos = 0
                    # for i, img_file in enumerate(ordered_images):
                    #     ax.text(10, y_pos + 30, f'{img_file}', fontsize=10, color='red', 
                    #            bbox=dict(boxstyle="round,pad=0.2", facecolor="white", alpha=0.8))
                    #     y_pos += resized_images[i].shape[0]
                else:
                    ax.text(0.5, 0.5, '无有效图片', ha='center', va='center', transform=ax.transAxes, fontsize=14)
                    ax.axis('off')
                
                plt.tight_layout()
                
                # 保存结果
                output_path = f'yizhuihe_visualizations/group_{group_name}_visualization.png'
                plt.savefig(output_path, dpi=300, bbox_inches='tight')
                print(f"  保存到: {output_path}")
                
                plt.close()  # 关闭图形以节省内存
            else:
                print(f"  缀合组 {group_name} 没有有效图片，跳过")
    
    print(f"\n所有缀合组可视化完成！结果保存在 yizhuihe_visualizations/ 目录下")

def main():
    """主程序"""
    print("=== 竹简自动缀合系统 - 基于孪生神经网络（增强版）===")
    
    # 1. 数据准备
    yizhuihe_folder = 'yizhuihe'
    weizhuihe_folder = 'weizhuihe'
    excel_path = 'zhuihetongji.xlsx'
    
    # 2. 可视化所有已缀合组的结果
    #visualize_all_yizhuihe_groups(yizhuihe_folder, excel_path)
    
    # 3. 提取已知缀合信息
    group_order = extract_group_order(excel_path)
    print(f"提取到 {len(group_order)} 个缀合组")
    
    # 4. 使用所有已缀合数据创建训练数据
    print("\n创建增强训练数据...")
    positive_pairs, negative_pairs = create_training_pairs_from_all_yizhuihe(yizhuihe_folder, group_order)
    print(f"正样本对: {len(positive_pairs)}, 负样本对: {len(negative_pairs)}")
    
    # 5. 训练孪生网络
    if positive_pairs and negative_pairs:
        print("\n开始训练孪生网络...")
        model = train_siamese_network(positive_pairs, negative_pairs, num_epochs=10)  # 增加训练轮数
        
        # 6. 利用形状特性分类竹简
        complete_folder, incomplete_folder = classify_complete_and_incomplete_bamboo(weizhuihe_folder)
        
        print(f"\n分类完成！")
        print(f"完整简已保存到: {complete_folder}")
        print(f"由于只保存完整简，不进行缀合操作")
        
        # 7. 保存结果
        with open('classification_results_enhanced.txt', 'w', encoding='utf-8') as f:
            f.write("竹简分类结果（增强版）\n")
            f.write("=" * 50 + "\n")
            f.write(f"完整简已保存到: {complete_folder}\n")
            f.write(f"分类标准：大幅放宽，只要形状大致符合竹简特征即可\n")
            f.write("-" * 50 + "\n")
            f.write("分类完成！\n")
        
        print("结果已保存到 classification_results_enhanced.txt")
        
    else:
        print("训练数据不足，无法训练模型")

if __name__ == '__main__':
    main() 