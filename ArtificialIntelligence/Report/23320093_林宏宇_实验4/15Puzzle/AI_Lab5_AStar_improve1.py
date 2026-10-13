import heapq #用于实现优先队列
import time  #用于计算运行时间

#目标状态，用一维数组表示状态图，0代表空格
GOAL_STATE = (1, 2, 3, 4, 
              5, 6, 7, 8, 
              9, 10, 11, 12, 
              13, 14, 15, 0) 

#允许的移动方向（左、右、上、下）
#比如 "L": -1表示向左移动，在一维数组中索引-1，在二维数组中表示向左移动一格
MOVES = {
    "L": -1, "R": 1,
    "U": -4, "D": 4
}


#计算曼哈顿距离
def manhattan(state):
    distance = 0
    for i, val in enumerate(state):
        if val == 0:#空格不计算曼哈顿距离
            continue  
        target_x, target_y = (val - 1) % 4, (val - 1) // 4
        #对一维数组的索引：%4取模得到横坐标，//4整除得到纵坐标 

        current_x, current_y = i % 4, i // 4
        distance += abs(target_x - current_x) + abs(target_y - current_y)
        #曼哈顿距离公式 = abs(目标x - 当前x) + abs(目标y - 当前y)

    return distance

#拓展当前状态发生移动后的所有可能的状态
def get_neighbors(state):
    zero_index = state.index(0) #找到空格的位置，空格0可以进行上下左右移动
    neighbors = []
    for move, delta in MOVES.items():
        new_index = zero_index + delta
        new_x, new_y = new_index % 4, new_index // 4 #得到新的空格坐标
        
        if 0 <= new_x < 4 and 0 <= new_y < 4:#判断是否在边界内，防止0移动到边界外
            new_state = list(state)
            new_state[zero_index], new_state[new_index] = new_state[new_index], new_state[zero_index]
            neighbors.append((move, tuple(new_state)))
    return neighbors

#f=a*g+b*h a、b分别为权重系数
a=1
b=1
#A* 算法求解
def solve_puzzle(start_state):
    priority_queue = []
    h=0 #已走步数，表示初始状态到当前状态代价
    g=manhattan(start_state) #计算曼哈顿距离，表示当前状态到目标状态的代价
    f=a*g+b*h #启发函数f=曼哈顿距离g+已走步数h
    heapq.heappush(priority_queue, (f, h, start_state, []))
    #将元素加入优先队列（最小堆）
    #最小堆函数 输入参数为：曼哈顿距离，已走步数，当前状态，路径
    #优先比较输入的第一个参数，最小的在前面

    visited = set()
    while priority_queue:
        f, h, state, path = heapq.heappop(priority_queue) #取出启发值f最小的情况
        if state in visited: #如果当前状态已经访问过，则跳过
            continue
        visited.add(state)
        if state == GOAL_STATE: #如果当前状态等于目标状态，则返回路径
            return path
        for move, new_state in get_neighbors(state): #拓展当前状态的所有可能的状态
            if new_state not in visited:
                new_h = h + 1  #记录已走步数
                new_g = manhattan(new_state)  #计算新的曼哈顿距离
                new_f = a*new_h + b*new_g  # 计算f=h+g
                heapq.heappush(priority_queue, (new_f, new_h, new_state, path + [move]))
    
    return None  #队列为空 无解

#测试用例
"""
initial_state = (1, 2, 3, 4, 
                 5, 6, 7, 8, 
                 9, 10, 11, 12, 
                 13, 14, 0, 15)
"""
#initial_state = (1,2,4,8,5,7,11,10,13,15,0,3,14,6,9,12)
#initial_state = (14,10,6,0,4,9,1,8,2,3,5,11,12,13,7,15)
#initial_state = (5,1,3,4,2,7,8,12,9,6,11,15,0,13,10,14)
initial_state = (6,10,3,15,14,8,7,11,5,1,0,2,13,12,9,4)
#initial_state = (11,3,1,7,4,6,8,2,15,9,10,13,14,12,5,0)
#initial_state = (0,5,15,14,7,9,6,13,1,2,12,10,8,11,4,3)

#记录开始时间
start_time = time.time()

solution = solve_puzzle(initial_state)

#记录结束时间
end_time = time.time()

print("初始状态：")
print(initial_state[0:4])
print(initial_state[4:8])
print(initial_state[8:12])
print(initial_state[12:])
print("权重系数：a =", a, "b =", b)
if solution:
    print("移动步数：", len(solution))
    print("移动时间：", format(end_time - start_time, ".10f"), "秒")
else:
    print("该状态无解")