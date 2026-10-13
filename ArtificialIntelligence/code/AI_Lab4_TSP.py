import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math
import random
#设置支持中文的字体
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']  #使用微软雅黑字体
plt.rcParams['axes.unicode_minus'] = False  #防止负号显示为方块

def read_map():
    city_name = []
    city_x = []
    city_y = []
    with open('dj38.tsp', 'r') as file:
        lines = file.readlines()
        reading_coords = False
        for line in lines:
            #跳过注释行
            if line.startswith('COMMENT') or not line.strip():
                continue
            #开始读取节点坐标部分
            if line.strip() == 'NODE_COORD_SECTION':
                reading_coords = True
                continue
            #结束读取节点坐标部分
            if line.strip() == 'EOF':
                break
            #读取坐标数据
            if reading_coords:
                parts = line.split()
                if len(parts) == 3:
                    city_name.append(parts[0])  
                    city_x.append(float(parts[1]))
                    city_y.append(float(parts[2]))
    return city_name, city_x, city_y

#读取城市数据并绘制城市位置图
def read_map0():
    data = pd.read_csv('TSP.csv')  #读取城市数据文件.csv格式
    city_name = data['city'].values  #获取城市名称 city栏
    city_x = data['x'].values  #获取城市的x坐标 x坐标栏
    city_y = data['y'].values  #获取城市的y坐标 y坐标栏
    return city_name, city_x, city_y #返回城市名称和坐标

#计算不同城市间的距离矩阵
def cal_distances(city_name, city_position_x, city_position_y):
    global city_distance #声明城市之间距离为全局变量
    city_num = len(city_name) #城市数量
    city_distance = np.zeros([city_num, city_num]) #初始化城市距离矩阵 np.zeros创建一个全0的矩阵，参数为矩阵的行和列

    for i in range(city_num):#计算两城市之间的欧几里得距离
        for j in range(city_num):
            city_distance[i][j] = math.sqrt((city_position_x[i] - city_position_x[j]) ** 2 + (city_position_y[i] - city_position_y[j]) ** 2)
    return city_num


#计算一条路径的总长度
def cal_length(path, origin):  #参数：具体路径，出发源点
    distance = 0
    distance += city_distance[origin][path[0]]  #从起点到第一个城市的距离
    for i in range(len(path)):
        if i == len(path) - 1:
            distance += city_distance[origin][path[i]]  #从最后一个城市回到起点的距离
        else:
            distance += city_distance[path[i]][path[i + 1]]  #计算城市间的路径距离
    return distance

#得到当前代种群最优个体
def get_result(population, origin):
    sorted_paths = [[cal_length(path, origin), path] for path in population] #存储每个路径的长度和路径
    sorted_paths = sorted(sorted_paths) #按路径长度升序排列取出最优路径
    return sorted_paths[0][0], sorted_paths[0][1]  #路径，长度

#选择父代种群
def selection(population, retain_rate, live_rate, origin):  #参数：种群，适者比例，生命强度，出发点
    sorted_paths = [[cal_length(path, origin), path] for path in population]
    sorted_paths = [path[1] for path in sorted(sorted_paths)]
    retain_num = int(len(sorted_paths) * retain_rate) #对应适者比例的适应性强的个体数量
    parents = sorted_paths[: retain_num]  #保留适应性强的前retain_num个体 成为父代的一部分以便繁殖
    
    for i in sorted_paths[retain_num:]:  #对于适应性弱的染色体
        if random.random() < live_rate:  #随机数小于生命强度，则保留该个体成为父代的一部分以便繁殖
            parents.append(i)
    return parents

#改良初代种群
def improve(path, improve_count, origin):  #参数：具体路径，改良迭代次数，出发源点
    distance = cal_length(path, origin)
    for i in range(improve_count): #随机选择两个城市
        u = random.randint(0, len(path) - 1)
        v = random.randint(0, len(path) - 1)
        if u != v: #交换两个城市u v的位置，生成新路径
            new_path = path.copy()  
            t = new_path[u]
            new_path[u] = new_path[v]
            new_path[v] = t
            new_distance = cal_length(new_path, origin)
            if new_distance < distance:  # 如果新路径更优，则保留该路径，提高种群质量
                distance = new_distance
                path = new_path.copy()
    return path

#部分映射交叉
def pmx_crossover(parents, population_num):  
    childen_num = population_num - len(parents) #计算要生成子代的个数
    children = []
    while len(children) < childen_num:
        #在父母种群中随机选择父母
        male_index = random.randint(0, len(parents) - 1)
        female_index = random.randint(0, len(parents) - 1)

        if male_index != female_index:
            male = parents[male_index]
            female = parents[female_index]

            #随机选择两个下标 s 和 t
            s = random.randint(0, len(male) - 1)
            t = random.randint(s, len(male) - 1)
            if s>t:#确保s和t不相等，且s<t
                s, t = t, s

            #交换两个父代个体中的子串
            child1 = male[:s] + female[s:t+1] + male[t+1:]
            child2 = female[:s] + male[s:t+1] + female[t+1:]

            #通过映射关系调整交叉后的路径，避免重复
            child1 = pmx_map(child1, male[s:t+1], female[s:t+1], s, t)
            child2 = pmx_map(child2, female[s:t+1], male[s:t+1], s, t)

            #将生成的子代添加到子代列表中
            children.append(child1)
            children.append(child2)

    return children

#映射来调整子代的顺序，且确保没有重复元素
def pmx_map(child, segment1, segment2, s, t):
    i=0
    while i<len(child):
        if s<=i<=t:#跳过索引范围 [s, t] 这一部分不需要映射
            i+=1
        else:
            if child[i] in segment2: #找到映射关系，并替换
                index = segment2.index(child[i])
                child[i] = segment1[index]
                #不需要+1 继续检查替换后的元素是否在segment2中
            else:
                i+=1 #处理下一个值
    return child

#倒置变异
def mutation_reverse(children, mutation_rate):  #参数：孩子种群，变异率
    for i in range(len(children)):
        if random.random() < mutation_rate: 
            child = children[i]
            #随机选择下标s和t
            s = random.randint(0, len(child) - 2)
            t = random.randint(s + 1, len(child) - 1)
            #将s到t中间部分倒置
            child[s:t+1] = child[s:t+1][::-1]
            #保存变异后的子代
            children[i] = child
    return children


#结果可视化
def plt_magin(iters, distance, result_path, origin, city_name, city_position_x, city_position_y):
    print("进化次数为", iters, "时的最佳路径长度为：", distance)
    result_path = [origin] + result_path + [origin]
    X = [city_position_x[i] for i in result_path]
    Y = [city_position_y[i] for i in result_path]
    # 创建图像
    plt.figure(figsize=(8, 6))  # 设置图像大小
    plt.plot(X, Y, marker='o', color='b', linestyle='-', markersize=6, linewidth=2)  # 绘制路径
    plt.fill(X, Y, 'b', alpha=0.1)  # 填充路径区域
    # 设置坐标轴标签和标题
    plt.xlabel('经度', fontsize=12)
    plt.ylabel('纬度', fontsize=12)
    plt.title(f"GA_TSP (最短路径: {distance:.2f})", fontsize=14)
    # 设置网格
    plt.grid(True, linestyle='--', alpha=0.5)
    # 去除多余边框
    plt.gca().spines['right'].set_visible(False)
    plt.gca().spines['top'].set_visible(False)
    # 显示图像
    plt.show()

#绘制每一代的最优路径长度
def plot_best_path_length(every_gen_best):
    plt.figure(figsize=(8, 6))  # 设置图像大小
    plt.plot(range(len(every_gen_best)), every_gen_best, color='b', linewidth=2)  # 绘制最优路径长度变化曲线
    plt.xlabel('代数', fontsize=12)
    plt.ylabel('最优路径长度', fontsize=12)
    plt.title('遗传算法最优路径长度变化', fontsize=14)
    plt.grid(True, linestyle='--', alpha=0.5)  # 显示网格
    plt.gca().spines['right'].set_visible(False)  # 去掉右边框
    plt.gca().spines['top'].set_visible(False)  # 去掉上边框
    plt.show()


#遗传算法总流程
def GA_TSP(origin, population_num, improve_count, iter_count, retain_rate, live_rate, mutation_rate):
            #源点，  种群个体数，      改良迭代数,     进化次数，   适者概率，   生命强度，     变异率
    city_name, city_position_x, city_position_y = read_map()
    city_num= cal_distances(city_name, city_position_x, city_position_y)
    list = [i for i in range(city_num)]
    list.remove(origin) #去掉源点,只保留其他城市，假设从源点出发

    population = [] #种群的列表，存储每个个体的路径
    for i in range(population_num): #随机生成种群
        path = list.copy()
        random.shuffle(path)  
        path = improve(path, improve_count, origin)  #优化 提高初始化种群多样性        
        population.append(path)

    every_gen_best = []  #存储每一代最好的路径长度
    for i in range(iter_count):
        #选择繁殖个体群
        parents = selection(population, retain_rate, live_rate, origin)
        #交叉繁殖
        children = pmx_crossover(parents, population_num)
        #倒置变异
        children = mutation_reverse(children, mutation_rate)
        #更新种群
        population = parents + children
        distance, result_path = get_result(population, origin)
        every_gen_best.append(distance)
    #输出结果
    plt_magin(i, distance, result_path, origin, city_name, city_position_x, city_position_y) #最终结果
    plot_best_path_length(every_gen_best)  #绘制每一代的最优路径长度变化曲线
    plt.show()

if __name__ == '__main__':
    #调用遗传算法，传入相关参数
    GA_TSP(5,   300,      200,     2000,    0.3,     0.5,   0.01)  
          #源点，种群个数，改良次数，进化次数，适者概率，生命强度，变异率