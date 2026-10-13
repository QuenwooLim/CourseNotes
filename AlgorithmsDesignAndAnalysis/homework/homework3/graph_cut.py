import cv2
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from collections import deque, defaultdict
import sys
def get_resized_image(img, width=60):
    """
    为了演示算法，我们将图像缩小，否则NetworkX构建图和计算最大流会非常慢。
    在实际工程中，通常使用C++实现的专门库（如PyMaxflow）。
    """
    h, w = img.shape[:2]
    r = width / float(w)
    dim = (width, int(h * r))
    resized = cv2.resize(img, dim, interpolation=cv2.INTER_AREA)
    return resized

def build_graph(img):
    """
    构建图：
    1. 节点：每个像素是一个节点，加上源点 'S' 和汇点 'T'。
    2. n-links (邻域边)：连接相邻像素，权重基于颜色相似度。
    3. t-links (终端边)：连接像素与S/T，权重基于亮度阈值（针对手掌图优化）。
    """
    # 图像像素与图结构的映射
    h, w = img.shape[:2] # 图像高度和宽度
    G = nx.DiGraph() # 有向图初始化
    
    # 转换为灰度图用于简单的亮度阈值判断
    if len(img.shape) == 3:
        gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    else:
        gray = img
        
    # 高斯模糊减少噪声
    gray = cv2.GaussianBlur(gray, (5, 5), 0)
    
    # 定义源点和汇点
    S = 'source'
    T = 'sink'
    
    # 阈值与参数设定
    FG_THRESH = 100  # 前景亮度阈值
    BG_THRESH = 40   # 背景亮度阈值
    sigma = 8.0      # n-link 权重高斯参数

    # 边的容量越大，算法越不愿意割断这条边；容量越小，越容易被割断。
    FG_Capacity = 10000        # t-link 硬约束大权重
    Edge_Capacity = 10      # 模糊区域 t-link 权重

    
    # 遍历所有像素
    for y in range(h):
        for x in range(w):
            node_id = (y, x)
            pixel_color = img[y, x]
            gray_val = gray[y, x]

            # 添加 t-links (Terminal links)（像素与 S/T 的边）
            
            # 优化：不再仅依赖几何中心，而是依赖亮度
            if gray_val > FG_THRESH:
                # 亮区域 -> 前景种子
                G.add_edge(S, node_id, capacity=FG_Capacity)
                G.add_edge(node_id, T, capacity=0) # 容量0 “绝不可能”被分到背景（T），即使割断这条边也没有任何代价
            elif gray_val < BG_THRESH:
                # 暗区域 -> 背景种子
                G.add_edge(S, node_id, capacity=0)
                G.add_edge(node_id, T, capacity=FG_Capacity)
            else:
                # 模糊区域 (边缘) -> 由最小割决定
                # 给予较小的均匀权重，让 n-links 起主导作用
                G.add_edge(S, node_id, capacity=Edge_Capacity) 
                G.add_edge(node_id, T, capacity=Edge_Capacity)

            # 添加 n-links (Neighbor links)（像素与邻居的边）

            neighbors = []
            if x < w - 1: neighbors.append(((y, x+1), img[y, x+1])) # 右
            if y < h - 1: neighbors.append(((y+1, x), img[y+1, x])) # 下
            
            for n_id, n_color in neighbors:
                # 计算颜色差异
                dist = np.linalg.norm(pixel_color - n_color)
                # 高斯函数权重公式 颜色越接近，weight越大，算法越不愿意割断这条边
                weight = 80 * np.exp(- (dist**2) / (2 * sigma**2))
                
                G.add_edge(node_id, n_id, capacity=weight)
                G.add_edge(n_id, node_id, capacity=weight)

    return G, S, T, gray

def Dinic(G, source, sink):
    """
    使用 Dinic 算法计算最大流和最小割
    """
    print("正在使用自定义 Dinic 算法计算最大流...")
    sys.setrecursionlimit(200000) # 增加递归深度以防止深层图遍历溢出
    
    # 1. 初始化残余图
    residual = defaultdict(dict)
    for u, v, data in G.edges(data=True):
        residual[u][v] = data['capacity']
        if u not in residual[v]:
            residual[v][u] = 0
            
    # 预处理邻接表 (因为 residual 的结构不会变，只有权值变)
    adj = {u: list(residual[u].keys()) for u in residual}
    
    max_flow = 0
    level = {}

    def bfs():
        """构建分层图"""
        level.clear()
        level[source] = 0
        queue = deque([source])
        while queue:
            u = queue.popleft()
            if u == sink:
                return True
            for v in adj[u]:
                cap = residual[u][v]
                if cap > 0 and v not in level:
                    level[v] = level[u] + 1
                    queue.append(v)
        return False

    def dfs(u, pushed, ptr):
        """在分层图中寻找增广路径 (多路增广)"""
        if pushed == 0 or u == sink:
            return pushed
        
        neighbors = adj[u]
        for i in range(ptr[u], len(neighbors)):
            ptr[u] = i # 当前弧优化
            v = neighbors[i]
            cap = residual[u][v]
            
            if v in level and level[v] == level[u] + 1 and cap > 0:
                tr = dfs(v, min(pushed, cap), ptr)
                if tr == 0:
                    continue
                
                residual[u][v] -= tr
                residual[v][u] += tr
                return tr
        return 0
    
    while bfs():
        ptr = {u: 0 for u in residual} # 每次构建分层图后重置当前弧
        while True:
            pushed = dfs(source, float('inf'), ptr)
            if pushed == 0:
                break
            max_flow += pushed
            
    # 5. 寻找最小割划分 (S集合: 从源点在残余图中可达的节点)
    reachable = set()
    queue = deque([source])
    visited = {source}
    while queue:
        u = queue.popleft()
        reachable.add(u)
        for v, cap in residual[u].items():
            if v not in visited and cap > 0:
                visited.add(v)
                queue.append(v)
                
    non_reachable = set(G.nodes()) - reachable
    
    return max_flow, (reachable, non_reachable)

def visualize_graph_structure(G, gray_img, h, w, output_path='source/graph_structure.png'):
    """
    可视化图结构：展示压缩后全图的节点和边（大图建议只显示部分边/节点标签以防爆炸）
    """
    print("正在生成全图结构可视化...")
    # 全部像素节点
    all_nodes = [(y, x) for y in range(h) for x in range(w)]
    pos = {(y, x): (x, -y) for y in range(h) for x in range(w)}
    labels = {(y, x): f"{gray_img[y, x]}" for y in range(h) for x in range(w)}
    colors = [gray_img[y, x] for y in range(h) for x in range(w)]

    # 只保留像素节点（不画source/sink）
    pixel_nodes = [n for n in G.nodes if isinstance(n, tuple)]
    subG = G.subgraph(pixel_nodes).copy()

    # 动态调整画布大小
    fig_w = max(8, w // 8)
    fig_h = max(6, h // 10)
    plt.figure(figsize=(fig_w, fig_h))
    ax = plt.gca()

    # 节点
    nx.draw_networkx_nodes(subG, pos, node_size=30, node_color=colors, cmap='gray', vmin=0, vmax=255, edgecolors='black')
    # 只在小图时显示标签
    if h * w <= 400:
        nx.draw_networkx_labels(subG, pos, labels=labels, font_color='red', font_weight='bold', font_size=8)

    # 边
    edges = []
    weights = []
    for u, v, data in subG.edges(data=True):
        edges.append((u, v))
        weights.append(data['capacity'])
    if weights:
        max_w = max(weights) + 1e-5
        widths = [0.5 + 2 * (w / max_w) for w in weights]
        edge_colors = [w for w in weights]
    else:
        widths = 1
        edge_colors = 'gray'
    edges_drawn = nx.draw_networkx_edges(subG, pos, edgelist=edges, width=widths, edge_color=edge_colors, edge_cmap=plt.cm.Blues, edge_vmin=0, edge_vmax=max(weights) if weights else 1, alpha=0.7, arrows=False)
    # 颜色条
    if edges_drawn and not isinstance(edges_drawn, list):
        plt.colorbar(edges_drawn, ax=ax, label='Edge Capacity (Weight)', orientation='vertical', fraction=0.046, pad=0.04)
    elif edges_drawn and isinstance(edges_drawn, list) and len(edges_drawn) > 0:
        sm = plt.cm.ScalarMappable(cmap=plt.cm.Blues, norm=plt.Normalize(vmin=0, vmax=max(weights) if weights else 1))
        sm.set_array([])
        plt.colorbar(sm, ax=ax, label='Edge Capacity (Weight)', orientation='vertical', fraction=0.046, pad=0.04)
    plt.title(f"Graph Structure Visualization (All {h}x{w} Pixels)", fontsize=14)
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(output_path, dpi=200)
    plt.close()
    print(f"全图结构可视化已保存至: {output_path}")

def visualize_cut_result(G, gray_img, partition, h, w, output_path='source/cut_result_structure.png'):
    """
    可视化最小割结果：展示全图的分割归属和割边
    """
    print("正在生成全图最小割结果可视化...")
    reachable, non_reachable = partition
    # 全部像素节点
    all_nodes = [(y, x) for y in range(h) for x in range(w)]
    pos = {(y, x): (x, -y) for y in range(h) for x in range(w)}
    labels = {(y, x): f"{gray_img[y, x]}" for y in range(h) for x in range(w)}
    # 节点着色
    node_colors = ['lightgreen' if (y, x) in reachable else 'lightgray' for y in range(h) for x in range(w)]
    # 只保留像素节点
    pixel_nodes = [n for n in G.nodes if isinstance(n, tuple)]
    subG = G.subgraph(pixel_nodes).copy()
    fig_w = max(8, w // 8)
    fig_h = max(6, h // 10)
    plt.figure(figsize=(fig_w, fig_h))
    ax = plt.gca()
    nx.draw_networkx_nodes(subG, pos, node_size=30, node_color=node_colors, edgecolors='black')
    # 小图显示标签
    if h * w <= 400:
        nx.draw_networkx_labels(subG, pos, labels=labels, font_color='black', font_weight='bold', font_size=8)
    # 边分类
    cut_edges = []
    internal_edges = []
    for u, v, data in subG.edges(data=True):
        u_in_S = u in reachable
        v_in_S = v in reachable
        if u_in_S != v_in_S:
            cut_edges.append((u, v))
        else:
            internal_edges.append((u, v))
    # 内部边
    nx.draw_networkx_edges(subG, pos, edgelist=internal_edges, width=1, edge_color='blue', alpha=0.5, arrows=False)
    # 割边
    if cut_edges:
        nx.draw_networkx_edges(subG, pos, edgelist=cut_edges, width=2, edge_color='red', style='dashed', arrows=False)
    from matplotlib.lines import Line2D
    legend_elements = [
        Line2D([0], [0], marker='o', color='w', label='Foreground (Source Set)', markerfacecolor='lightgreen', markersize=10, markeredgecolor='black'),
        Line2D([0], [0], marker='o', color='w', label='Background (Sink Set)', markerfacecolor='lightgray', markersize=10, markeredgecolor='black'),
        Line2D([0], [0], color='blue', lw=2, label='Internal Link'),
        Line2D([0], [0], color='red', lw=2, linestyle='--', label='Cut Edge (Min-Cut)')
    ]
    ax.legend(handles=legend_elements, loc='upper right')
    plt.title(f"Min-Cut Result Visualization (All {h}x{w} Pixels)", fontsize=14)
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(output_path, dpi=200)
    plt.close()
    print(f"全图最小割结果可视化已保存至: {output_path}")

def main():
    # 0. 读取图像
    print("第零步：读取图像")
    img_name = 'source/test'
    img_path = img_name + '.png'
    img = cv2.imread(img_path)
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    
    # 1. 缩小图像 (为了演示速度)
    print("第一步：缩小图像以加快处理速度")
    process_width = 1000   # 像素调整
    small_img = get_resized_image(img_rgb, width=process_width)
    h, w = small_img.shape[:2]
    print(f"处理图像大小: {w}x{h}")
    # plt.imsave(img_name + '_step1_resized.png', small_img)

    # 2. 构建图
    print("第二步：构建图结构")
    G, S, T, gray = build_graph(small_img)
    # plt.imsave(img_name + '_step2_gray.png', gray, cmap='gray')
    # visualize_graph_structure(G, gray, h, w, output_path=img_name + '_step2_structure.png')

    
    # 3. 计算最大流 / 最小割
    print("第三步：计算最大流 / 最小割")
    cut_value, partition = Dinic(G, S, T)
    reachable, non_reachable = partition
    # visualize_cut_result(G, gray, partition, h, w, output_path=img_name + '_step3_minimumcut.png')

    # 4. 生成分割
    print("第四步：生成分割")
    mask = np.zeros((h, w), dtype=np.uint8)
    
    for y in range(h):
        for x in range(w):
            node_id = (y, x)
            if node_id in reachable:
                # 属于源点集合 (前景)
                mask[y, x] = 1
            else:
                # 属于汇点集合 (背景)
                mask[y, x] = 0
    # 将掩膜放大回原图尺寸
    original_h, original_w = img.shape[:2]
    full_mask = cv2.resize(mask, (original_w, original_h), interpolation=cv2.INTER_NEAREST)
    # 提取轮廓
    mask_uint8 = (full_mask * 255).astype(np.uint8)
    # 改用 RETR_TREE 以便检测所有轮廓，防止因背景被识别为前景（掩膜反转）导致只检测到最外层边框
    contours, _ = cv2.findContours(mask_uint8, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    # 在原图上绘制轮廓，而不是全黑背景
    contour_img = img_rgb.copy()
    # 另外创建一个纯黑背景用于单独展示轮廓
    contour_only = np.zeros_like(img_rgb)
    
    valid_contours = []
    img_area = original_w * original_h
    for cnt in contours:
        x, y, w_box, h_box = cv2.boundingRect(cnt)
        # 如果轮廓面积接近全图面积，说明是图像边框（通常发生在背景被误判为前景时），将其忽略
        if w_box * h_box > 0.90 * img_area:
            continue
        valid_contours.append(cnt)

    # 使用绿色 (0, 255, 0) 绘制轮廓，线条加粗
    cv2.drawContours(contour_img, valid_contours, -1, (0, 255, 0), 2)
    # 在纯黑背景上绘制白色轮廓
    cv2.drawContours(contour_only, valid_contours, -1, (255, 255, 255), 2)
    
    # 保存轮廓
    plt.imsave(img_name + '_contour.png', contour_only)

    # 5. 生成对比图
    print("第五步：生成对比图像")
    # plt.imsave(img_name + '_step5_compare.png', contour_img)
    plt.imsave(img_name + '_compare.png', contour_img)


if __name__ == "__main__":
    main()
