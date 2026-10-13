
import numpy as np
import pandas as pd


class QLearning:
    def __init__(self, actions, learning_rate=0.01, reward_decay=0.9, e_greedy=0.1):
        self.actions = actions  # a list
        self.lr = learning_rate
        self.gamma = reward_decay
        self.epsilon = e_greedy

        ''' build q table'''
        ############################

        # YOUR IMPLEMENTATION HERE #

        ############################

    def choose_action(self, observation):
        ''' choose action from q table '''
        ############################

        # YOUR IMPLEMENTATION HERE #

        ############################

    def learn(self, s, a, r, s_):
        ''' update q table '''
        ############################

        # YOUR IMPLEMENTATION HERE #

        ############################

    def check_state_exist(self, state):
        ''' check state '''
        ############################

        # YOUR IMPLEMENTATION HERE #

        ############################