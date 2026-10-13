import numpy as np
import random
import pygame # pygame是一个用于编写视频游戏、图像等得模块

"""
走迷宫
1.迷宫状态：起点已知，终点已知，障碍物随机分布
2.奖励条件：如果碰到障碍物得到-1分，如果到达终点，得到10分
"""
# 初始化pygame
pygame.init()

# 设置窗口大小
width, height = 600, 600 # 定义窗口大小，长宽都是600像素
screen = pygame.display.set_mode((width, height)) # 设置窗口
pygame.display.set_caption("Q-learning") # 设置窗口标题

# 设置颜色
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

# 设置网格大小
grid_size = 20
rows, cols = height // grid_size, width // grid_size #计算行列数

# 环境&奖惩
class GridWorld:
    def __init__(self, start, goal, obstacles):
        self.start = start # 起点
        self.goal = goal # 终点
        self.obstacles = obstacles # 障碍物列表
        self.states = [(i, j) for i in range(rows) for j in range(cols)] # 所有状态列表

    def is_valid_state(self, state): # 判断状态是否在网格内部，如果是则返回true，否则返回false
        return 0 <= state[0] < rows and 0 <= state[1] < cols 

    def get_neighbors(self, state): # 获取临近状态
        neighbors = [(state[0]+1, state[1]), (state[0]-1, state[1]), (state[0], state[1]+1), (state[0], state[1]-1)]
        # 0代表向上，1代表向下，2代表向右，3代表向左
        valid_idx=[] 
        for index in range(len(neighbors)):
            if self.is_valid_state(neighbors[index]):
                valid_idx.append(index)
        return neighbors, valid_idx

    def reset(self): # 重置起点
        return self.start 

    def step(self, state, action): # 执行动作函数
        neighbors, valid_idx = self.get_neighbors(state) # 返回有效的下一步动作
        # valid_idx是可走动作的集合
        if action in valid_idx: # 判断下一个动作是否可走
            next_state = neighbors[action] # 下一个状态为相应的临近状态
            reward = 0
        else: 
            next_state = state # 动作无效，保持现有状态
            reward = -0.1 # 设置reward为-1
        if next_state in self.obstacles:
            next_state = state # 动作无效，保持现有状态
            reward =-1 #碰到障碍物
        done = next_state == self.goal # 如果到达目标状态，则表示已经完成，done
        if done:
            reward = 10 # 到达终点奖励10     
        return next_state, reward, done

# Qlearning
class QLearningAgent:
    def __init__(self, env):
        self.env = env
        self.q_table = np.zeros((rows, cols, 4)) # Q表 初始化四个动作
        self.alpha = 0.1 # 学习率
        self.gamma = 0.99 # 折扣率
        self.epsilon = 0.1 #

    def choose_action(self, state): # epsilon-greedy策略选择动作
        if random.uniform(0, 1) < self.epsilon:  # epsilon的概率随机选择动作
            return random.randint(0, 3) 
        else: #1-epsilong的概率选择Q值最大的动作
            return np.argmax(self.q_table[state]) # 返回最大值Q得索引 

    def train(self, episodes):
        #训练是为了找到最优的Q值
        for episode in range(episodes): # 迭代
            # 重置状态
            state = self.env.reset() # 起点
            done = False 
            #total_reward = 0
            while not done:
                action = self.choose_action(state) # 动作索引
                next_state, reward, done = self.env.step(state, action) # 执行动作，并返回奖励
                #total_reward += reward # 累计奖励
                best_next_q = np.max(self.q_table[next_state]) # 下一状态对应的最大Q值
                #Q值更新公式为Q(s,a)=Q(s,a)+α[R+γmaxQ(s',a')] 
                self.q_table[state][action] += self.alpha * (reward + self.gamma * best_next_q - self.q_table[state][action])
                state = next_state

    def test(self):
        state = self.env.reset()
        done = False
        steps = []

        while not done:
            action = np.argmax(self.q_table[state]) # 返回最大Q值对应的动作
            next_state, _, done = self.env.step(state, action) # 执行动作
            steps.append(action) # 存储动作列表放入step中
            state = next_state

        print("Steps taken:", steps)

# 绘制网格
def draw_grid(): 
    for i in range(rows):
        for j in range(cols):
            pygame.draw.rect(screen, WHITE , (j * grid_size, i * grid_size, grid_size, grid_size))

# 绘制智能体
def draw_agent(state):
    x, y = state[1] * grid_size, state[0] * grid_size # 在网格中列索引对应x坐标，行索引对应y坐标
    pygame.draw.circle(screen, RED, (x + grid_size // 2, y + grid_size // 2), grid_size // 2 - 5) # 圆心半径画圆

# 目标位置绘制
def draw_goal(state): 
    x, y = state[1] * grid_size, state[0] * grid_size 
    pygame.draw.circle(screen, GREEN, (x + grid_size // 2, y + grid_size // 2), grid_size // 2 - 5)

# 障碍物绘制  
def draw_obstacles(obstacles): 
    for i in range(len(obstacles)):
        obstacle= obstacles[i]
        x, y = obstacle[1] * grid_size, obstacle[0] * grid_size 
        pygame.draw.circle(screen, BLACK, (x + grid_size // 2, y + grid_size // 2), grid_size // 2 - 5)

# 随机生成障碍物函数
def generate_random_obstacles(num_obstacles, rows, cols):
    obstacles=set()
    while len(obstacles)< num_obstacles:
        x=random.randint(0,rows-1)
        y=random.randint(0,cols-1)
        obstacles.add((x,y))
    return list (obstacles)

# 主函数
def main():  
    obstacles = generate_random_obstacles(100,rows,cols)
    env = GridWorld((0, 0), (rows - 1, cols - 1), obstacles) # 给定起点和终点位置
    agent = QLearningAgent(env) # 用Qlearning对地图进行学习

    agent.train(episodes=100) #迭代训练1000次

    running = True
    state = env.reset()
    steps = []
    # 主循环
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        screen.fill(BLACK)
        draw_grid()
        draw_agent(state)
        draw_goal(env.goal)
        draw_obstacles(env.obstacles)

        pygame.display.flip()
        pygame.time.delay(100)
        
        action = np.argmax(agent.q_table[state]) # 返回最大Q值对应的动作
        next_state, _, done = agent.env.step(state, action) # 执行动作
        steps.append(action) # 存储动作列表放入step中
        state = next_state
        if done:
            print("Finish!")
            break
    pygame.quit()

if __name__ == "__main__":
    main()