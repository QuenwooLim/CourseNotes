#define _CRT_SECURE_NO_WARNINGS_
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//菜单
void menu()
{
    printf("***************\n");
    printf("**   1.play  **\n");
    printf("**   0.exit  **\n");
    printf("***************\n");
}

void game()
{
    // RAND_MAX;(0-32767)
    int a = rand() % 100 + 1;//生成随机整数 且模范围在0-99 +1使得1-100

    while (1)//一直为真 一直循环下去
    {
        int guess = 0;
        printf("请猜数字:");
        scanf("%d", &guess);

        if (guess < a)
        {
            printf("猜小了\n");
        }
        else if (guess > a)
        {
            printf("猜大了\n");
        }
        else
        {
            printf("猜对了\n");
            break;
        }
    }
}

int main()
{
    int input = 0;
    srand((unsigned int)time(NULL));//时间戳生成随机,放在主函数使的随机数不变
    do
    {
        menu();
        printf("请选择:");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            printf("猜数字\n");
            game();
            break;
        case 0:
            printf("退出游戏\n");
            break;
        default:
            printf("选择错误,请重现选择\n");
            break;
        }

    } while (input);//0为假 循环结束 



    return 0;
}