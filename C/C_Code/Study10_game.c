//游戏运行
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"

void InitBoard(char board[ROW][COL], int row, int col)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < row; j++)
        {
            board[i][j] = ' ';//棋盘为空
        }
    }
}

void DisplayBoard(char board[ROW][COL], int row, int col)
{
    int i = 0;
    for (i = 0; i < row; i++)
    {
        int j = 0;
        for (j = 0; j < col; j++)
        {
            printf(" %c ", board[i][j]);
            if (j < col - 1)
                printf("|");
        }
        printf("\n");
        if (i < row - 1)
        {
            for (j = 0; j < col - 1; j++)
                printf("---|");
            if (j = col - 1)
                printf("---");
        }
        printf("\n");
    }
}

void Playermove(char board[ROW][COL], int row, int col)
{
    int x = 0;
    int y = 0;
    while (1)
    {
        printf("玩家开始下棋 +\n");
        printf("请输入坐标(第几行第几列):");

        scanf("%d %d", &x, &y);

        if (x >= 1 && x <= row && y >= 1 && y <= col)
        {
            if (board[x - 1][y - 1] == ' ')
            {
                board[x - 1][y - 1] = '+';
                break;
            }
            else
                printf("坐标已经被占用，请重新输入\n");

        }
        else
            printf("输入错误，请重新输入\n");
    }
}

void Computermove(char board[ROW][COL], int row, int col)
{
    printf("电脑下棋 -\n");

    int x = 0;
    int y = 0;
    while (1)
    {
        x = rand() % row;//模使坐标在0~2
        y = rand() % col;
        if (board[x][y] == ' ')
        {
            board[x][y] = '-';
            break;
        }
    }

}

//判断棋盘是否已经满了 满了为1 不满为0
int Isfull(char board[ROW][COL], int row, int col)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            if (board[j][j] != ' ')
                return 0;
        }
    }
    return 1;
}

char Iswin(char board[ROW][COL], int row, int col)
{
    //判断行结局
    int i = 0;
    int j = 0;
    for (i = 0; i < row; i++)
    {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != ' ')
        {
            return board[i][0];
        }
    }

    //判断列结局
    for (j = 0; j < col; j++)
    {
        if (board[0][j] == board[1][j] && board[0][j] == board[2][j] && board[0][j] != ' ')
        {
            return board[0][j];
        }
    }

    //判断对角线结局
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')
        return board[1][1];

    //判断平局
    if (Isfull(board, row, col))
    {
        return '#';
    }
    return '*';

}