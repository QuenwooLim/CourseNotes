#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <time.h>

//扫雷游戏

void menu()
{
    printf("********************\n");
    printf("*******1.paly*******\n");
    printf("*******0.exit*******\n");
    printf("********************\n");
}

void game()
{
    //雷图
    char mine[ROWS][COLS] = { 0 };
    //展示图
    char show[ROWS][COLS] = { 0 };
    //初始化 雷图为0 展示图为*
    InitBoard(mine, ROWS, COLS, '0');
    InitBoard(show, ROWS, COLS, '*');
    //DisplayBoard(mine,ROW,COL);
    DisplayBoard(show, ROW, COL);

    //设置雷
    SetMine(mine, ROW, COL);
    //DisplayBoard(mine,ROW,COL);
    //DisplayBoard(show,ROW,COL);

    //排查雷
    FindMine(mine, show, ROW, COL);

}

int main()
{
    int input = 0;
    srand((unsigned int)time(NULL));
    do
    {
        menu();
        printf("输入选择:\n");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            printf("开始游戏\n");
            game();
            break;
        case 0:
            printf("退出游戏\n");
            break;
        default:
            printf("输入错误，请重新输入你的选择\n");
            break;
        }
    } while (input);
    return 0;
}