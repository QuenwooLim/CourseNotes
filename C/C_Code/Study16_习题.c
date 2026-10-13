#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    int a, b, c;
    a = 5;
    c = ++a;//a c 6
    b = ++c, c++, ++a, a++;//c8 b7 a8
    b += a++ + c;//c8 a9 b23
    printf("%d %d %d", a, b, c);
    return 0;
}

int count_num(unsigned int x)//强制类型转换 负数也可以用同样的方法
{
    int count = 0;
    while (x)
    {
        if (x % 2 == 1)//x%2==1说明二进制位的最后一个是1
        {
            count++;
        }
        x = x / 2;//相当于去掉最后一位
    }
    return count;
}

/*奇妙解法 n=n&(n-1)进行一次则去掉一个1
//另外补充 n&(n-1)==0可以判断一个数是2的次方
int count_num(unsigned int x)
{
    int count=0;
    while(x)
    {
        n=n&(n-1);
        count ++;
    }

}
*/
//写一个函数 返回参数二进制中1的个数
int main()
{
    int n = 0;
    scanf("%d", &n);
    int m = count_num(n);
    printf("%d\n", m);

    return 0;
}

//全局变量，静态变量都是放在静态区
//全局变量，静态变量在不初始化的时候，默认会被初始化为0
//局部变量，是放在栈区，不初始化时默认为随机值

int i;//全局变量 没有初始化 默认为0
int main()
{
    i--;
    if (i > sizeof(i))// -1>4 sizeof的返回值是size_t无符号整形
        //类型不同 负数的补码被误当作正数 因此比较时要转化成大的类型
    {
        printf(">\n");
    }
    else
    {
        printf("<\n");
    }
    return 0;
}

int main()
{
    int n = 0;
    scanf("%d", &n);
    int i = 0;
    int j = 0;
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            printf(" ");
            if (i == j || i + j == n + 1)
            {
                printf("*");
            }
        }
        printf("\n");
    }


    return 0;
}

int main()
{
    int a = 0;
    int b = 0;
    int c = 0;
    scanf("%d %d %d", &a, &b, &c);

    if ((a + b > c) && (a + c > b) && (b + c > a))
    {
        if (a == b && b == c)//错写成a=b=c
        {
            printf("lsosceles triangle");
        }
        else if ((a = b) || (a = c) || (b = c))
        {
            printf("equilateral triangle");
        }
        else
        {
            printf("ordinary triangle");
        }

    }
    else
    {
        printf("Not a triangle");
    }

    return 0;
}

/*
0x 00 00 00 01
0x 00 00 00 02
0x 00 00 00 03
0x 00 00 00 04
0x 00 00 00 05

内存存放顺序:(倒)
01 00 00 00,02 00 00 00,03 00 00 00, 04 00 00 00,05 00 00 00
前面8个字节被初始化
*/
int main()
{
    int arr[] = { 1,2,3,4,5 };//整型占用4个字节
    short* p = (short*)arr;//short类型一次访问2个字节
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        *(p + 1) = 0;//一次跳过2个字节
    }
    for (i = 0; i < 5; i++)
    {
        printf("%d ", arr[i]);//0 0 3 4 5
    }

    return 0;
}

int main()
{
    int a = 0x11223344;//0x表示x86的环境
    //存放：44 33 22 11；
    //char类型一次访问一个字节 内存变成00 33 22 11
    // a地址变为 11 22 33 00
    char* pc = (char*)&a;
    *pc = 0;
    printf("%x", a);//x为打印地址

    return 0;
}

void print(int* p)
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        printf("%d ", *(p + i));
    }
}

int main()
{
    int arr[] = { 1,2,3,4,5,6,7,8,9,0 };

    print(arr);

    return 0;
}

int main()
{
    char arr[10001] = { 0 };
    gets(arr);//等同于scanf 但可以获取内容 包括空格; scanf遇到空格便不再读取

    int left = 0;
    int right = strlen(arr) - 1;

    while (left < right)
    {
        char tmp = arr[left];
        arr[left] = arr[right];
        arr[right] = tmp;

        right--;
        left++;
    }

    printf("%s", arr);

    return 0;
}

//Sn=a+aa+aaa+aaaa+aaaaa
int main()
{
    int a = 0;
    int n = 0;
    scanf("%d %d\n", &a, &n);

    int i = 0;
    int sum = 0;
    int k = 0;

    for (i = 0; i < n; i++)
    {
        k = k * 10 + 2;
        sum = sum + k;
    }

    printf("%d", sum);

    return 0;
}

//水仙花数
//计算是几位数
int count(int x)
{
    int i = 1;
    while (x / 10 > 0)
    {
        x = x / 10;
        i++;
    }
    return i;
}


int main()
{
    int num = 0;

    for (num = 1; num <= 100000; num++)
    {
        int sum = 0;

        int tmp = num;//取另一个变量tmp暂时代替num 不改变原来num值   
        while (tmp)
        {
            sum += pow(tmp % 10, count(num));//pow(a,n) 就是计算a的n次方
            tmp /= 10;
        }

        if (sum == num)
        {
            printf("%d ", num);
        }
    }

    return 0;
}

int main()
{
    int line = 0;
    scanf("%d", &line);

    int i = 0;
    int j = 0;

    //菱形上部分
    for (i = 0; i < line; i++)
    {
        //先打空格
        for (j = 0; j < line - 1 - i; j++)
        {
            printf(" ");
        }
        //再打*
        for (j = 0; j < 2 * i + 1; j++)
        {
            printf("*");
        }
        printf("\n");
    }

    //菱形下部分
    for (i = 0; i < line - 1; i++)
    {
        //先打空格
        for (j = 0; j <= i; j++)
        {
            printf(" ");
        }
        //再打*
        for (j = 0; j < 2 * (line - 1 - i) - 1; j++)
        {
            printf("*");
        }
        printf("\n");
    }


    return 0;
}

int main()
{
    int* arr[10];//是一个指针数组
    int(*arr)[10];//是一个数组指针 指向数组的指针
    return 0;
}

//20快买汽水 一元一瓶 两个空瓶换一瓶汽水
int main()
{
    int money = 0;
    scanf("%d", &money);
    int empty = money;
    int total = money;

    while (empty >= 2)
    {
        total = total + empty / 2;
        empty = empty / 2 + empty % 2;
    }
    printf("%d", total);
}