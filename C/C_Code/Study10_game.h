//游戏的定义
#pragma once
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

#define ROW 3
#define COL 3

//初始化棋盘
void InitBoard(char board[ROW][COL], int row, int col);

//打印棋盘
void DisplayBoard(char board[ROW][COL], int row, int col);

//玩家下棋
void Playermove(char board[ROW][COL], int row, int col);

//电脑下棋,没下过棋的地方随便下
void Computermove(char board[ROW][COL], int row, int col);

//判断输赢:返回值:电脑赢'+'  玩家赢'-' 平局'#' 游戏继续'*'
char Iswin(char board[ROW][COL], int row, int col);