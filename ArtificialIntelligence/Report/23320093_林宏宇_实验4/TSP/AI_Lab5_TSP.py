import numpy as np 
import pandas as pd
import matplotlib.pyplot as plt
import math
import random

plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']  #设置中文字体
plt.rcParams['axes.unicode_minus'] = False  #正常显示负号


class GeneticAlgTSP:
    def __init__(self, filename):  #初始化
        self.filename = filename
        self.city_name, self.city_x, self.city_y = self.read_map()  #读取城市信息
        self.city_num = self.cal_distances()  #计算城市数量及城市间距离
        self.origin = 0  #定义出发城市为0
        self.population = self.init_population()  #初始化种群

    def read_map(self):  #读取地图信息
        city_name = []
        city_x = []
        city_y = []
        with open(self.filename, 'r') as file:
            lines = file.readlines()
            reading_coords = False
            for line in lines:
                if line.startswith('COMMENT') or not line.strip():
                    continue
                if line.strip() == 'NODE_COORD_SECTION':
                    reading_coords = True
                    continue
                if line.strip() == 'EOF':
                    break
                if reading_coords:
                    parts = line.split()
                    if len(parts) == 3:
                        city_name.append(parts[0])  #读取城市名称
                        city_x.append(float(parts[1]))  #读取城市横坐标
                        city_y.append(float(parts[2]))  #读取城市纵坐标
        return city_name, city_x, city_y

    def cal_distances(self):  #计算城市之间的距离
        self.city_distance = np.zeros([len(self.city_name), len(self.city_name)])
        for i in range(len(self.city_name)):
            for j in range(len(self.city_name)):
                self.city_distance[i][j] = math.sqrt(
                    (self.city_x[i] - self.city_x[j]) ** 2 +
                    (self.city_y[i] - self.city_y[j]) ** 2
                )
        return len(self.city_name)

    def cal_length(self, path):  #计算路径长度
        distance = self.city_distance[self.origin][path[0]]
        for i in range(len(path)):
            if i == len(path) - 1:
                distance += self.city_distance[path[i]][self.origin]
            else:
                distance += self.city_distance[path[i]][path[i + 1]]
        return distance

    def get_result(self, population):  #获取当前代中最短路径及其长度
        sorted_paths = [[self.cal_length(path), path] for path in population]
        sorted_paths = sorted(sorted_paths)
        return sorted_paths[0][0], sorted_paths[0][1]

    def selection(self, population, retain_rate, live_rate):  #选择操作
        sorted_paths = [[self.cal_length(path), path] for path in population]
        sorted_paths = [path[1] for path in sorted(sorted_paths)]
        retain_num = int(len(sorted_paths) * retain_rate)
        parents = sorted_paths[: retain_num]  #保留优秀个体
        for i in sorted_paths[retain_num:]:
            if random.random() < live_rate:  #按概率保留非最优个体
                parents.append(i)
        return parents

    def improve(self, path, improve_count):  #路径局部优化
        distance = self.cal_length(path)
        for _ in range(improve_count):
            u = random.randint(0, len(path) - 1)
            v = random.randint(0, len(path) - 1)
            if u != v:
                new_path = path.copy()
                new_path[u], new_path[v] = new_path[v], new_path[u]
                new_distance = self.cal_length(new_path)
                if new_distance < distance:
                    path = new_path
                    distance = new_distance
        return path

    def pmx_map(self, child, segment1, segment2, s, t):  #PMX映射操作
        i = 0
        while i < len(child):
            if s <= i <= t:
                i += 1
            else:
                if child[i] in segment2:
                    index = segment2.index(child[i])
                    child[i] = segment1[index]
                else:
                    i += 1
        return child

    def pmx_crossover(self, parents, population_num):  #部分映射交叉
        childen_num = population_num - len(parents)
        children = []
        while len(children) < childen_num:
            m, f = random.sample(parents, 2)
            s, t = sorted(random.sample(range(len(m)), 2))
            child1 = m[:s] + f[s:t+1] + m[t+1:]
            child2 = f[:s] + m[s:t+1] + f[t+1:]
            child1 = self.pmx_map(child1, m[s:t+1], f[s:t+1], s, t)
            child2 = self.pmx_map(child2, f[s:t+1], m[s:t+1], s, t)
            children.extend([child1, child2])
        return children

    def mutation_reverse(self, children, mutation_rate):  #变异操作（翻转）
        for i in range(len(children)):
            if random.random() < mutation_rate:
                s, t = sorted(random.sample(range(len(children[i])), 2))
                children[i][s:t+1] = children[i][s:t+1][::-1]
        return children

    def init_population(self):  #初始化种群
        list_ = [i for i in range(self.city_num)]
        list_.remove(self.origin)
        population = []
        for _ in range(300):  #默认300个体
            path = list_.copy()
            random.shuffle(path)
            path = self.improve(path, 200)  #默认改良200次
            population.append(path)
        return population

    def iterate(self, num_iterations):  #迭代执行遗传算法
        every_gen_best = []
        for i in range(num_iterations):
            parents = self.selection(self.population, 0.3, 0.5)
            children = self.pmx_crossover(parents, len(self.population))
            children = self.mutation_reverse(children, 0.01)
            self.population = parents + children
            distance, result_path = self.get_result(self.population)
            every_gen_best.append(distance)
        self.show_result(i, distance, result_path, every_gen_best)
        return result_path

    def show_result(self, iters, distance, result_path, every_gen_best):  #绘制最终结果
        print("进化次数为", iters, "时的最佳路径长度为：", distance)
        result_path = [self.origin] + result_path + [self.origin]
        X = [self.city_x[i] for i in result_path]
        Y = [self.city_y[i] for i in result_path]
        plt.figure(figsize=(8, 6))
        plt.plot(X, Y, marker='o', color='b', linestyle='-', markersize=6, linewidth=2)
        plt.fill(X, Y, 'b', alpha=0.1)
        plt.xlabel('经度', fontsize=12)
        plt.ylabel('纬度', fontsize=12)
        plt.title(f"GA_TSP (最短路径: {distance:.2f})", fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.gca().spines['right'].set_visible(False)
        plt.gca().spines['top'].set_visible(False)
        plt.show()

        plt.figure(figsize=(8, 6))
        plt.plot(range(len(every_gen_best)), every_gen_best, color='b', linewidth=2)
        plt.xlabel('代数', fontsize=12)
        plt.ylabel('最优路径长度', fontsize=12)
        plt.title('遗传算法最优路径长度变化', fontsize=14)
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.gca().spines['right'].set_visible(False)
        plt.gca().spines['top'].set_visible(False)
        plt.show()


if __name__ == '__main__':  #主函数
    ga = GeneticAlgTSP("dj38.tsp")
    best_path = ga.iterate(1000) #迭代1000次