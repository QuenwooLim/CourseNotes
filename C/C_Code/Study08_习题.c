#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS_
#include<stdlib.h>
#include<time.h>
#include<math.h>

void swap(int* px, int* py)
{
    int tmp = 0;
    tmp = *py;
    *py = *px;
    *px = tmp;
}

//输入三个数字 按顺序从大到小输出
int main()
{
    int a = 0;
    int b = 0;
    int c = 0;
    scanf("%d %d %d\n", &a, &b, &c);
    /*
        int tmp=0;

    //三个重复的交换函数的操作 考虑函数
        if(a<b)
        {
            tmp=b;
            b=a;
            a=tmp;
        }
        if(a<c)
        {
            tmp=c;
            c=a;
            a=tmp;
        }
        if(b<c)
        {
            tmp=c;
            c=b;
            b=tmp;
        }
    */

    if (a > b)
        swap(&a, &b);
    if (a < c)
        swap(&a, &c);
    if (b < c)
        swap(&b, &c);

    printf("%d %d %d", a, b, c);

    return 0;
}


int main()
{
    int i = 0;
    for (i = 1; i <= 100; i++)
    {
        if (i % 3 == 0)
            printf("%d\n", i);
    }

    return 0;
}


//1到100的所有整数中出现多少个数字9
int main()
{
    int i = 0;
    int count = 0;
    for (i = 1; i <= 100; i++)
    {
        if (i % 10 == 9)
            count++;
        if (i / 10 == 9)
            count++;
    }
    printf("%d\n", count);


    return 0;
}

//求两个数字的最大公约数
int main()
{
    int a = 0;
    int b = 0;
    scanf("%d %d", &a, &b);

    //法一 暴力解决 思路明确 计算量大
    /*
        int c=(a<b)?a:b;
        for(c;1;c--)
        {
            if(a%c==0 && b%c==0)
            {
                printf("%d\n",c);
                break;
            }
        }
    */

    //方法二 辗转相除法
    while (1)
    {
        int c = a % b;
        if (c == 0)
        {
            printf("%d", b);
            break;
        }
        a = b;
        b = c;
    }

    return 0;
}

//计算 1/1-1/2+1/3-1/4+1/5....-1/100
int main()
{
    int i = 0;
    double sum = 0;
    for (i = 1; i <= 100; i++)
    {
        if (i % 2 == 0)
            sum = sum - 1.0 / i;//1要变成1.0 计算更精确
        else
            sum = sum + 1.0 / i;
    }
    printf("%lf", sum);

    return 0;
}

//10个整数找出最大值
int main()
{
    int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
    int i = 0;
    int max = arr[0];
    for (i = 1; i < 10; i++)
    {
        if (arr[i] > max)
            max = arr[i];
    }
    printf("%d", max);
    return 0;
}

//99乘法表
int main()
{
    int a = 0;
    int b = 0;

    for (a = 1; a < 10; a++)
    {
        for (b = 1; b <= a; b++)
        {
            printf("%d*%d=%-2d ", a, b, a * b);//-2d左对齐 2d右对齐
        }
        printf("\n");//换行
    }

    return 0;
}

//实现一个函数，打印乘法口诀表，口诀表的行数和列数自己指定
void print_table(int n)
{
    int i = 0;
    for (i = 1; i <= n; i++)
    {
        int j = 0;
        for (j = 1; j <= i; j++)
        {
            printf("%d*%d=%-2d ", i, j, i * j);
        }
        printf("\n");
    }
}
int main()
{
    int n = 0;
    scanf("%d", &n);
    print_table(n);

    return 0;
}

//全局变量 可以调用函数的值
int a = 0;
int b = 0;

void test()
{
    a = 3;
    b = 4;//函数直接调用全局变量
}
int main()
{
    test();

    printf("a=%d b=%d\n", a, b);
    return 0;
}

