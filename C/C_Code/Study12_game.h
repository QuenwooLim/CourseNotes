#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define ROW 9
#define COL 9

#define ROWS ROW+2
#define COLS COL+2

#define easy_count 10

//初始化雷图
void InitBoard(char board[ROWS][COLS], int rows, int cols, int a);


//打印
void DisplayBoard(char board[ROWS][COLS], int row, int col);

//布置雷
void SetMine(char board[ROWS][COLS], int row, int col);

//排雷
void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col);