import heapq  # 用于实现优先队列
import time  # 用于计算运行时间

# 目标状态，用一维数组表示状态图，0代表空格
GOAL_STATE = (1, 2, 3, 4, 
              5, 6, 7, 8, 
              9, 10, 11, 12, 
              13, 14, 15, 0) 

# 允许的移动方向（左、右、上、下）
MOVES = {
    "L": -1, "R": 1,
    "U": -4, "D": 4
}

#线性冲突启发式函数
def calculate_heuristic(state):
    manhattan_distance = 0  #曼哈顿距离
    linear_conflict = 0  #线性冲突

    for row in range(4):
        for col in range(4):
            value = state[row * 4 + col]
            if value != 0:
                #计算目标位置的行和列
                target_row, target_col = (value - 1) // 4, (value - 1) % 4
                manhattan_distance += abs(row - target_row) + abs(col - target_col)

                #计算行冲突
                if col == target_col:
                    for next_col in range(col + 1, 4):
                        next_value = state[row * 4 + next_col]
                        if next_value != 0 and (next_value - 1) // 4 == row and (next_value - 1) % 4 < col:
                            linear_conflict += 2

                #计算列冲突
                if row == target_row:
                    for next_row in range(row + 1, 4):
                        next_value = state[next_row * 4 + col]
                        if next_value != 0 and (next_value - 1) % 4 == col and (next_value - 1) // 4 < row:
                            linear_conflict += 2

    return manhattan_distance + linear_conflict


# 拓展当前状态发生移动后的所有可能的状态
def get_neighbors(state):
    zero_index = state.index(0)  # 找到空格的位置，空格0可以进行上下左右移动
    neighbors = []
    for move, delta in MOVES.items():
        new_index = zero_index + delta
        new_x, new_y = new_index % 4, new_index // 4  # 得到新的空格坐标
        
        if 0 <= new_x < 4 and 0 <= new_y < 4:  # 判断是否在边界内，防止0移动到边界外
            new_state = list(state)
            new_state[zero_index], new_state[new_index] = new_state[new_index], new_state[zero_index]
            neighbors.append((move, tuple(new_state)))
    return neighbors

# A* 算法求解
def solve_puzzle(start_state):
    priority_queue = []
    h = 0  # 已走步数，表示初始状态到当前状态代价
    g = calculate_heuristic(start_state)  # 计算线性冲突启发式
    f = g + h  # 启发函数 f = 线性冲突启发式 g + 已走步数 h
    heapq.heappush(priority_queue, (f, h, start_state, []))  # 将元素加入优先队列（最小堆）

    visited = set()
    while priority_queue:
        f, h, state, path = heapq.heappop(priority_queue)  # 取出启发值 f 最小的情况
        if state in visited:  # 如果当前状态已经访问过，则跳过
            continue
        visited.add(state)
        if state == GOAL_STATE:  # 如果当前状态等于目标状态，则返回路径
            return path
        for move, new_state in get_neighbors(state):  # 拓展当前状态的所有可能的状态
            if new_state not in visited:
                new_h = h + 1  # 记录已走步数
                new_g = calculate_heuristic(new_state)  # 计算新的线性冲突启发式
                new_f = new_h + new_g  # 计算新的 f = 已走步数 h + 线性冲突启发式 g
                heapq.heappush(priority_queue, (new_f, new_h, new_state, path + [move]))
    
    return None  # 队列为空，无解

# 测试用例
#initial_state = (1,2,4,8,5,7,11,10,13,15,0,3,14,6,9,12)
#initial_state = (5,1,3,4,2,7,8,12,9,6,11,15,0,13,10,14)
initial_state = (14,10,6,0,4,9,1,8,2,3,5,11,12,13,7,15)


# 记录开始时间
start_time = time.time()

# 调用 A* 算法
solution = solve_puzzle(initial_state)

# 记录结束时间
end_time = time.time()

# 打印结果
print("初始状态：")
print(initial_state[0:4])
print(initial_state[4:8])
print(initial_state[8:12])
print(initial_state[12:])

if solution:
    print("最优路径下空格的移动方向:", " -> ".join(solution))
    print("移动步数：", len(solution))
    print("移动时间：", format(end_time - start_time, ".10f"), "秒")
else:
    print("该状态无解")
