import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
import cv2
from sklearn.neighbors import NearestNeighbors
import matplotlib.pyplot as plt

class GraphFeatureExtractor(nn.Module):
    """图网络特征提取器"""
    
    def __init__(self, input_dim=5, hidden_dim=128, output_dim=256):
        super(GraphFeatureExtractor, self).__init__()
        self.conv1 = nn.Linear(input_dim, hidden_dim)
        self.conv2 = nn.Linear(hidden_dim, hidden_dim)
        self.conv3 = nn.Linear(hidden_dim, output_dim)
        self.dropout = nn.Dropout(0.3)
        
    def forward(self, x, adj_matrix):
        # 简化的图卷积操作
        x = F.relu(self.conv1(x))
        x = self.dropout(x)
        x = F.relu(self.conv2(x))
        x = self.dropout(x)
        x = self.conv3(x)
        
        # 全局池化
        x = torch.mean(x, dim=0)
        return x

class SiameseBambooNetwork(nn.Module):
    """孪生竹简缀合网络"""
    
    def __init__(self, contour_dim=5, texture_dim=18, hidden_dim=128, output_dim=256):
        super(SiameseBambooNetwork, self).__init__()
        
        # 特征提取器
        self.contour_extractor = GraphFeatureExtractor(contour_dim, hidden_dim, output_dim)
        self.texture_extractor = GraphFeatureExtractor(texture_dim, hidden_dim, output_dim)
        
        # 融合层
        self.fusion_layer = nn.Linear(output_dim * 2, output_dim)
        self.similarity_layer = nn.Linear(output_dim, 1)
        
    def forward(self, x1_contour, x1_texture, adj1, x2_contour, x2_texture, adj2):
        
        # 提取特征
        contour_feat1 = self.contour_extractor(x1_contour, adj1)
        texture_feat1 = self.texture_extractor(x1_texture, adj1)
        
        contour_feat2 = self.contour_extractor(x2_contour, adj2)
        texture_feat2 = self.texture_extractor(x2_texture, adj2)
        
        # 融合特征
        fused_feat1 = self.fusion_layer(torch.cat([contour_feat1, texture_feat1]))
        fused_feat2 = self.fusion_layer(torch.cat([contour_feat2, texture_feat2]))
        
        # 计算相似性
        similarity = self.similarity_layer(torch.abs(fused_feat1 - fused_feat2))
        
        return similarity.squeeze(), fused_feat1, fused_feat2

def extract_contour_features(img):
    """提取轮廓特征"""
    edges = cv2.Canny(img, 50, 150)
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    if not contours:
        return None, None
    
    contour = max(contours, key=cv2.contourArea)
    contour_points = contour.reshape(-1, 2).astype(np.float32)
    
    # 限制轮廓点数量，避免维度过大
    if len(contour_points) > 1000:
        indices = np.linspace(0, len(contour_points)-1, 1000, dtype=int)
        contour_points = contour_points[indices]
    
    # 计算特征
    features = []
    for i in range(len(contour_points)):
        pos_feat = contour_points[i] / np.array([img.shape[1], img.shape[0]])
        
        if i > 0 and i < len(contour_points) - 1:
            prev = contour_points[i-1]
            curr = contour_points[i]
            next_pt = contour_points[i+1]
            v1 = curr - prev
            v2 = next_pt - curr
            angle = np.arctan2(np.cross(v1, v2), np.dot(v1, v2))
            curvature = np.abs(angle)
        else:
            curvature = 0.0
        
        if i < len(contour_points) - 1:
            direction = contour_points[i+1] - contour_points[i]
            direction = direction / (np.linalg.norm(direction) + 1e-8)
        else:
            direction = np.array([0.0, 0.0])
        
        feature = np.concatenate([pos_feat, [curvature], direction])
        features.append(feature)
    
    features = np.array(features)
    
    # 构建邻接矩阵
    nbrs = NearestNeighbors(n_neighbors=min(8, len(features)), algorithm='ball_tree').fit(features)
    distances, indices = nbrs.kneighbors(features)
    
    adj_matrix = np.zeros((len(features), len(features)))
    for i, neighbors in enumerate(indices):
        for j in neighbors[1:]:
            adj_matrix[i, j] = 1
            adj_matrix[j, i] = 1
    
    return features, adj_matrix

def extract_texture_features(img):
    """提取纹理特征"""
    def gabor_filter(img, ksize=15, sigma=3, theta=0, lambda_=10, gamma=0.5, psi=0):
        kernel = cv2.getGaborKernel((ksize, ksize), sigma, theta, lambda_, gamma, psi, ktype=cv2.CV_32F)
        filtered = cv2.filter2D(img, cv2.CV_8UC3, kernel)
        return filtered
    
    angles = [0, np.pi/4, np.pi/2, 3*np.pi/4]
    texture_features = []
    
    for angle in angles:
        filtered = gabor_filter(img, theta=angle)
        texture_features.append(filtered)
    
    h, w = img.shape
    features = []
    positions = []
    
    step = 20
    for y in range(step, h-step, step):
        for x in range(step, w-step, step):
            local_features = []
            for tf in texture_features:
                local_patch = tf[y-step:y+step, x-step:x+step]
                local_features.extend([
                    np.mean(local_patch),
                    np.std(local_patch),
                    np.max(local_patch),
                    np.min(local_patch)
                ])
            
            pos_feat = np.array([x/w, y/h])
            feature = np.concatenate([pos_feat, local_features])
            features.append(feature)
            positions.append([x, y])
    
    features = np.array(features)
    positions = np.array(positions)
    
    # 限制特征点数量
    if len(features) > 500:
        indices = np.linspace(0, len(features)-1, 500, dtype=int)
        features = features[indices]
        positions = positions[indices]
    
    # 构建邻接矩阵
    nbrs = NearestNeighbors(n_neighbors=min(6, len(features)), algorithm='ball_tree').fit(positions)
    distances, indices = nbrs.kneighbors(positions)
    
    adj_matrix = np.zeros((len(features), len(features)))
    for i, neighbors in enumerate(indices):
        for j in neighbors[1:]:
            adj_matrix[i, j] = 1
            adj_matrix[j, i] = 1
    
    return features, adj_matrix

def predict_similarity(model, img1, img2):
    """预测两张图片的相似度"""
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model.eval()
    
    with torch.no_grad():
        contour_feat1, adj1 = extract_contour_features(img1)
        texture_feat1, adj1_texture = extract_texture_features(img1)
        
        contour_feat2, adj2 = extract_contour_features(img2)
        texture_feat2, adj2_texture = extract_texture_features(img2)
        
        if (contour_feat1 is None or contour_feat2 is None or 
            texture_feat1 is None or texture_feat2 is None):
            return 0.0
        
        # 转换为张量
        x1_contour = torch.FloatTensor(contour_feat1).to(device)
        x1_texture = torch.FloatTensor(texture_feat1).to(device)
        adj1 = torch.FloatTensor(adj1).to(device)
        adj1_texture = torch.FloatTensor(adj1_texture).to(device)
        
        x2_contour = torch.FloatTensor(contour_feat2).to(device)
        x2_texture = torch.FloatTensor(texture_feat2).to(device)
        adj2 = torch.FloatTensor(adj2).to(device)
        adj2_texture = torch.FloatTensor(adj2_texture).to(device)
        
        similarity, _, _ = model(x1_contour, x1_texture, adj1, x2_contour, x2_texture, adj2)
        
        return similarity.item()

def visualize_matching(img1, img2, similarity_score, save_path=None):
    """可视化匹配结果"""
    fig, axes = plt.subplots(1, 2, figsize=(12, 6))
    
    axes[0].imshow(img1, cmap='gray')
    axes[0].set_title('Image 1')
    axes[0].axis('off')
    
    axes[1].imshow(img2, cmap='gray')
    axes[1].set_title('Image 2')
    axes[1].axis('off')
    
    plt.suptitle(f'Similarity Score: {similarity_score:.4f}')
    
    if save_path:
        plt.savefig(save_path, dpi=300, bbox_inches='tight')
    plt.show() 