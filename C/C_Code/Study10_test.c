#define CRT_SECURE_NO_WARNINGS
#include "game.h"

void menu()
{
    printf("******************\n");
    printf("*****1. play *****\n");
    printf("*****0. exit *****\n");
    printf("******************\n");
}

void game()
{
    printf("三子棋游戏开始\n");
    //创建棋盘
    char board[ROW][COL] = { 0 };

    //判断输赢返回值
    char ret = 0;
    InitBoard(board, ROW, COL);
    DisplayBoard(board, ROW, COL);

    while (1)
    {
        Playermove(board, ROW, COL);
        ret = Iswin(board, ROW, COL);
        if (ret != '*')
        {
            break;
        }
        DisplayBoard(board, ROW, COL);
        Computermove(board, ROW, COL);
        ret = Iswin(board, ROW, COL);
        if (ret != '*')
        {
            break;
        }
        DisplayBoard(board, ROW, COL);
    }
    if (ret == '+')
        printf("玩家胜利，棋盘结局如下：\n");
    else if (ret == '-')
        printf("电脑胜利，棋盘结局如下：\n");
    else
        printf("游戏平局,棋盘结局如下：\n");
    DisplayBoard(board, ROW, COL);
}

int main()
{
    srand((unsigned int)time(NULL));//设置生成随机数的起点
    int input = 0;
    do
    {
        menu();
        printf("请输入你的选择:");
        scanf_s("%d", &input);
        switch (input)
        {
        case 1:
            game();
            break;
        case 0:
            printf("退出游戏\n");
            break;
        default:
            break;
        }
    } while (input);

    return 0;
}