import random
import copy
import numpy as np
import torch
from torch import nn, optim
from agent_dir.agent import Agent
import matplotlib.pyplot as plt

class QNetwork(nn.Module):  #定义Q网络，继承自PyTorch的nn.Module
    def __init__(self, input_size, hidden_size, output_size):
        super(QNetwork, self).__init__()           # 调用父类构造函数
        self.fc1 = nn.Linear(input_size, hidden_size)   # 第一层全连接层，输入为状态维度，输出为隐藏层维度
        self.fc2 = nn.Linear(hidden_size, output_size)  # 第二层全连接层，输入为隐藏层维度，输出为动作数

    def forward(self, inputs):                     # 前向传播函数
        x = torch.relu(self.fc1(inputs))           # 输入经过第一层并激活
        x = self.fc2(x)                            # 经过第二层输出Q值
        return x                                   # 返回Q值

class ReplayBuffer:                               # 定义经验回放池
    def __init__(self, buffer_size):
        self.buffer = []                          # 用于存储经验的列表
        self.buffer_size = buffer_size            # 最大容量

    def __len__(self):
        return len(self.buffer)                   # 返回当前存储的经验数量

    def push(self, obs, action, reward, next_obs, done):  # 存储一条经验
        obs = np.array(obs, dtype=np.float32)            # 转换当前状态为float32数组
        next_obs = np.array(next_obs, dtype=np.float32)  # 转换下一个状态为float32数组
        self.buffer.append((obs, action, reward, next_obs, done))  # 添加到经验池
        if len(self.buffer) > self.buffer_size:           # 如果超出容量
            self.buffer.pop(0)                            # 移除最早的经验

    def sample(self, batch_size):                         # 随机采样一批经验
        indices = np.random.choice(len(self.buffer), batch_size, replace=False)  # 随机选取索引
        batch = [self.buffer[idx] for idx in indices]      # 根据索引取出经验
        # 用 np.stack 保证输出为合适的 float/int 数组
        return [np.stack(items) for items in zip(*batch)]  # 分别堆叠每一项，返回批量数据

    def clean(self):                                      # 清空经验池
        self.buffer = []

class AgentDQN(Agent):
    def __init__(self, env, args):
        super(AgentDQN, self).__init__(env) # 调用父类Agent的构造函数，保存环境
        self.args = args    # 保存参数
        obs_dim = env.observation_space.shape[0]  # 状态空间维度
        act_dim = env.action_space.n              # 动作空间维度
        hidden_dim = 256                          # 隐藏层维度
        self.q_net = QNetwork(obs_dim, hidden_dim, act_dim).to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))    # 主Q网络
        self.target_q_net = copy.deepcopy(self.q_net)    # 目标Q网络
        self.optimizer = optim.Adam(self.q_net.parameters(), lr=5e-4) #主网络的优化器
        self.buffer = ReplayBuffer(buffer_size=50000)  # 经验回放池
        self.gamma = 0.98          # 折扣因子
        self.batch_size = 256       # 批量大小
        self.epsilon = 1.0         # 探索率
        self.epsilon_min = 0.05    # 最小探索率
        self.epsilon_decay = 0.95 # 探索率衰减
        self.update_target_steps = 50    # 目标网络更新频率
        self.learn_step_counter = 0       # 学习步数计数
        self.num_episodes = 150  # 训练回合数

    def init_game_setting(self):
        self.epsilon = 1.0  # 每局游戏开始时重置探索率

    def train(self):
        if len(self.buffer) < self.batch_size:
            return
        obs, action, reward, next_obs, done = self.buffer.sample(self.batch_size) #从经验回放池中采样一个batch
        #将采样经验转为tensor便于运算
        obs = torch.FloatTensor(obs)
        action = torch.LongTensor(action)
        reward = torch.FloatTensor(reward)
        next_obs = torch.FloatTensor(next_obs)
        done = torch.FloatTensor(done)

        q_values = self.q_net(obs).gather(1, action.unsqueeze(1)).squeeze(1) #当前的Q值

        """普通DQN算法"""
        # next_q_values = self.target_q_net(next_obs).max(1)[0] #取max 下一状态的Q值
        # expected_q = reward + self.gamma * next_q_values * (1 - done) #target期望的Q值

        """Double DQN: 用主网络选动作，用目标网络评估Q值"""
        next_actions = self.q_net(next_obs).argmax(1, keepdim=True)  # 主网络选动作
        next_q_values = self.target_q_net(next_obs).gather(1, next_actions).squeeze(1)  # 目标网络评估Q值
        expected_q = reward + self.gamma * next_q_values * (1 - done)

        loss = nn.MSELoss()(q_values, expected_q.detach()) #loss函数
        self.optimizer.zero_grad()  # 梯度清零
        loss.backward()  # 反向传播
        self.optimizer.step()  # 更新参数
        self.learn_step_counter += 1  # 学习步数加一
        if self.learn_step_counter % self.update_target_steps == 0:  # 定期同步目标网络
            self.target_q_net.load_state_dict(self.q_net.state_dict())

    def make_action(self, observation, test=True):  # 根据当前状态选择动作
        if (not test) and (random.random() < self.epsilon):  # 训练时按epsilon-greedy策略探索
            return self.env.action_space.sample()
        obs = torch.FloatTensor(observation).unsqueeze(0)  # 状态转为Tensor并增加batch维
        q_values = self.q_net(obs)  # 计算所有动作的Q值
        return q_values.argmax().item()  # 选择Q值最大的动作

    def run(self):
        reward_list=[] #用于保存每回合的奖励
        for episode in range(self.num_episodes):
            obs,_ = self.env.reset()  # 重置环境，获得初始状态 # 新版 Gym 返回 (obs, info)
            total_reward = 0  # 本回合累计奖励
            done = False  # 回合是否结束
            while not done:
                action = self.make_action(obs, test=False)#选择下一个动作
                next_obs, reward, terminated, truncated, _  = self.env.step(action) #进行下一个动作，返回值位置、奖励、是否结束、额外信息 # 新版 Gym 返回 (obs, reward, terminated, truncated, info)
                done = terminated or truncated #terminated表示游戏结束，truncated表示时间步数超过限制
                self.buffer.push(obs, action, reward, next_obs, float(done)) #存储经验：当前状态，采取动作，奖励值，下一个动作
                obs = next_obs  # 状态更新
                total_reward += reward  # 累计奖励
                self.train()  # 训练网络
            self.epsilon = max(self.epsilon * self.epsilon_decay, self.epsilon_min)  # 衰减探索率
            self.target_q_net.load_state_dict(self.q_net.state_dict())  # 每回合同步目标网络
            reward_list.append(total_reward)
            print(f"Episode {episode}, Reward: {total_reward}")  # 打印本回合信息
        # 训练结束后绘制并保存奖励曲线为SVG
        plt.figure()
        plt.plot(reward_list)
        plt.xlabel('Episode')
        plt.ylabel('Reward')
        plt.title('DQN Reward Curve')
        plt.savefig('reward_curve.svg', format='svg')
        print("奖励曲线已保存为 reward_curve.svg，请用浏览器打开该文件查看。")