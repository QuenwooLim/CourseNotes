#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int fib(int x)
{
    if (x == 1)
        return 1;
    else if (x == 2)
        return 2;
    else
        return fib(x - 1) + fib(x - 2);
}
int main()
{
    int n = 0;
    scanf("%d", &n);

    printf("%d", fib(n));

    return 0;
}

int main()
{
    int n = 0;
    scanf("%d", &n);
    int arr[50];

    int i = 0;
    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int del = 0;
    scanf("%d", &del);
    //使用j来减少开辟空间
    int j = 0;
    for (i = 0; i < n; i++)
    {
        if (arr[i] != del)
        {
            arr[j] = arr[i];
            j++;
        }
    }

    for (i = 0; i < j; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}

int main()
{
    int n = 0;
    scanf("%d\n", &n);
    int arr[10000];
    int i = 0;

    int max = 0;
    int min = 100;

    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
        if (arr[i] > max)
            max = arr[i];
        if (arr[i] < min)
            min = arr[i];
    }

    printf("%d\n", max - min);


    return 0;
}

int main()
{
    char a = 0;
    while (scanf("%c", &a) == 1)//条件判断 读取成功返回1 失败返回EOF
    {
        if (a >= 'a' && a <= 'z')
        {
            printf("%c\n", a - 32);
        }
        else
        {
            printf("%c\n", a + 32);
        }
        getchar();//消去每次输入字母之后的回车换行\n
    }
    return 0;
}

int main()
{
    char ch = 0;
    while (scanf("%c", &ch) == 1)
    {
        if (ch >= 'A' && ch <= 'z')
            printf("%c is a alphant\n", ch);
        else
            printf("%c is not a alphant\n", ch);
        getchar();//或者将scanf中"%c" 换成" %c"加空格可以自动跳过前面的空格
    }
    return 0;
}

int main()
{
    /*
    int a=0;
    int b=0;
    int c=0;
    scanf("%d %d %d",&a,&b,&c);
    int max=a;
    if(b>a)
    {
        max=b;
        if(c>b)
          max=c;
    }
    else
    {
        if(c>a)
          max=c;
    }
    printf("%d",max);
    */
    int i = 0;
    int score = 0;
    int max = 0;
    for (i = 0; i < 3; i++)
    {
        scanf("%d", &score);
        if (score > max)
            max = score;
    }
    printf("%d", max);
    return 0;
}

int test(int x)
{
    return ((x / 10) * (x % 10) + (x / 100) * (x % 100) + (x / 1000) * (x % 1000) + (x / 10000) * (x % 10000));
}

int main()
{
    int a = 0;

    for (a = 10000; a < 100000; a++)
    {
        if (a == test(a))
            printf("%d ", a);
    }

    return 0;
}