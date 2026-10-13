#include "game.h"

//扫雷游戏
void InitBoard(char board[ROWS][COLS], int rows, int cols, int a)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            board[i][j] = a;
        }
    }
}

void DisplayBoard(char board[ROWS][COLS], int row, int col)
{
    int i = 0;
    int j = 0;
    printf("----------扫雷游戏-----------\n");
    for (i = 0; j <= col; j++)
    {
        printf("%d ", j);
    }
    printf("\n");
    for (i = 1; i <= row; i++)
    {
        printf("%d ", i);
        for (j = 1; j <= col; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("----------扫雷游戏-----------\n");

}

void SetMine(char board[ROWS][COLS], int row, int col)
{
    int count = easy_count;
    while (count)
    {
        int x = rand() % row + 1;
        int y = rand() % col + 1;
        if (board[x][y] == '0')
        {
            board[x][y] = '1';
            count--;
        }
    }
}

int get_mine_count(char board[ROWS][COLS], int x, int y)
{
    return (board[x - 1][y] + board[x - 1][y - 1] + board[x][y - 1] +
        board[x + 1][y - 1] + board[x + 1][y] + board[x + 1][y + 1] +
        board[x][y + 1] + board[x - 1][y + 1] - 8 * '0');//数字字符转化成整形字符
}

void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col)
{
    int x = 0;
    int y = 0;
    int win = 0;
    while (win < row * col - easy_count)
    {
        printf("请输入要排查的坐标:\n");
        scanf("%d %d", &x, &y);
        if (x >= 1 && x <= 9 && y >= 1 && y <= 9)
        {
            if (show[x][y] != '*')
            {
                printf("已排查，重新输入\n");
                continue;
            }
            else
            {
                if (mine[x][y] == '1')
                {
                    printf("踩雷啦，游戏结束，结局如下:\n");
                    DisplayBoard(show, ROW, COL);
                    break;
                }
                else//不是雷
                {
                    //统计周围几个雷
                    int count = get_mine_count(mine, x, y);
                    show[x][y] = count + '0';//整形数字转换成数字字符
                    DisplayBoard(show, ROW, COL);
                    win++;
                }
            }

            if (win == row * col - easy_count)
            {
                printf("游戏胜利,结局如下:\n");
                DisplayBoard(mine, ROW, COL);

            }
        }
        else
        {
            printf("输入错误\n");
        }
    }
}



//另外补充:
//标记功能
//当坐标不是雷切周围没有雷，展开周围