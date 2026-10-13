#define _CRT_SECURE_NO_WARNINGS_
//库函数
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int get_max(int x, int y)//返回类型为int，xy接受整形
{
    return (x > y ? x : y);
}
int main()
{
    int a = 0;
    int b = 0;
    scanf("%d %d", &a, &b);

    //函数接受返回值
    int m = get_max(a, b);

    return 0;
}

/*
形式参数即形参 传值
1.函数在声明和定义时用的参数
2.参数定义时由于没有具体值，只是一个符号或可存储特定值的空间
3.申明时可仅申明参数类型，定义时必须给出形式参数名称

实际参数即实参 传地址或引用&
1.函数在使用时实际替代形式参数的值
2.实际参数与声明的形式参数必须数量相同，类型兼容
3.如果实参形参类型兼容但不一致，则发生强制类型转换
*/

/*
地址
变量是存储在计算机的某个地址的
如果两个变量名指向同一个内存地方
则对内存地址操作可以影响到其他变量的值
*/

//不用返回 类型为void
void Swap(int* px, int* py)//传址调用 指针地址，可以改变外部函数，修改实值
//xy是形参,是实参的一种临时拷贝 传值调用 修改形参不会影响实参
//只有函数调用才实例化,函数用完之后自动销毁
{
    //引入第三个数作为中间
    int z = 0;
    z = *px;
    *px = *py;
    *py = z;
}
int main()
{
    int a = 0;
    int b = 0;

    scanf("%d %d", &a, &b);
    printf("交换前a=%d,b=%d\n", a, b);
    //a,b是实参
    Swap(&a, &b);

    printf("交换后a=%d,b=%d\n", a, b);

    return 0;
}

void test(int a, int b, int c)//a=2 b=1 c=0
{
    printf("%d %d %d", a, b, c);
}
int main()
{
    int a = 0;
    test(a++, a++, a++);
    //函数调用时 实参表达式按从右至左顺序赋予形参
}


//判断i是不是素数 拿2到i-1的数字除
#include<math.h>//数学库函数 sqrt
/*
数学库函数
fabs(x) 求绝对值
exp(x) e^x
log(x) ln(x)
log10(x) ln(x)/ln(10)
pow(x,y) x^y
sqrt(x) x开根号
sin(x) cos(x)...
ceil(x) 向上取整 离x最近且大于它的整数
floor(x) 向下取整
*/
//方法一
int main()
{
    int i = 100;
    int count = 0;//count 要放在循环外部
    for (i = 100; i <= 200; i++)
    {
        int c = 1;
        int a = 2;
        for (a = 2; a < sqrt(i); a++)//sqrt(i)是i开平方
        {
            int b = i % a;
            if (b == 0)
            {
                c = 0;
                break;
            }
        }
        if (c == 1)//注意是==!!!!!!!!
        {
            count++;//计算个数
            printf("%d\n", i);
        }
    }
    printf("个数为%d\n", count);
    return 0;
}

//方法二 调用函数
int is_prime(int n)
{
    int a = 0;
    for (a = 2; a < sqrt(n); a++)
    {
        if (n % a == 0)
        {
            return 0;//使得函数直接结束
        }
    }
    return 1;
}
int main()
{
    int i = 0;
    int count = 0;
    for (i = 100; i <= 200; i++)
    {
        if (is_prime(i))
        {
            printf("%d\n", i);
            count++;
        }
    }
    printf("素数个数为%d", count);

    return 0;
}

//找1000~2000之间闰年
int is_leap_year(int n)
{
    if (((n % 4 == 0) && (n % 100 != 0)) || (n % 100 == 0))
        return 1;
    return 0;
}
int main()
{
    int i = 0;
    /*
    方法一
        for(i=1000;i<2001;i++)
        {
            if(i%4==0)
            {
                if(i%100==0)
                {
                    if(i%400==0)
                      printf("%d\n",i);
                    continue;
                }
                printf("%d\n",i);
            }
        }
    */
    /*
    方法二
        for(i=1000;i<2004;i++)
        {
            if(((i%4==0)&&(i%100!=0))||(i%100==0))
             printf("%d\n",i);
        }
    */
    //方法三 函数调用
    for (i = 1000; i < 2001; i++)
    {
        if (is_leap_year(i))
        {
            printf("%d\n", i);
        }
    }
    return 0;
}

// 二分法函数 在有序数组里找具体数字
int binary_search(int arr[], int k, int sz)//数组传参，只传首位元素的地址，arr实质是指针变量
{                                       // 因此不要在函数内部计算数组元素的个数
    int left = 0;
    int right = sz - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (k > arr[mid])
        {
            left = mid + 1;
        }
        else if (k < arr[mid])
        {
            right = mid - 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}
int main()
{
    int arr[] = { 1,2,3,4,5,6,7,8,9,10 };
    int k = 7;
    int sz = sizeof(arr) / sizeof(arr[0]);

    //找到了返回下标，找不到返回-1；
    int ret = binary_search(arr, k, sz);

    if (ret == -1)
        printf("找不到");
    else
    {
        printf("找到了，下标为%d", ret);
    }

    return 0;
}


//嵌套函数使用
void new_line()
{
    printf("hehe\n");
}
void three_line()
{
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        new_line();
    }
}
int main()
{
    three_line();

    return 0;
}

int main()
{
    //链式访问;printf执行的返回值是打印字符的个数
    printf("%d", printf("%d", printf("%d", 43)));
    //                     先打印43 返回值是43的字符个数2
    //          打印2 返回值是1
    //打印1
    //结果 4321
    return 0;
}

//输入一个数字 按顺序打印他的每一位
//递归函数 大事化小
void print(unsigned int n)
{
    if (n > 9)//注意递归函数的限制条件
    {
        print(n / 10);//每次逐渐接近递归条件
    }
    printf("%d,", n % 10);
}
int main()
{
    unsigned int num = 0;
    printf("请输入数字");
    scanf("%u", &num);//&d打印有符号整数 &u打印无符号整数
    /*
        while(num)
        {
            printf("%d",num%10);
            num=num/10;
        }
    */
    print(num);
    return 0;
}


//模拟实现strlen
/*
int my_strlen(char* str)//循环法
{
    int count=0;
    while(*str!='\0')
    {
        count++;
        str++;//地址++表示进入下一个单元
    }
    return count;
}
*/
int my_strlen(char* str)//递归法
{
    if (*str != '\0')
    {
        return 1 + my_strlen(str + 1);//递归函数 先递推再回归
    }
    else
        return 0;
}
int main()
{
    int len = my_strlen("abcd");
    printf("%d", len);

    return 0;
}

//递归函数
int fac(int n)
{
    if (n <= 1)
        return 1;
    else
        return n * fac(n - 1);
}
//迭代函数 非递归
/*
int fac(int n)
{
    int a=1;
    int ret=1;
    for(a=1;a<=n;a++)
    {
        ret*=a;
    }
    return ret;
}
*/
int main()
{
    int n = 0;
    scanf("%d", &n);
    int ret = fac(n);
    printf("%d\n", ret);

    return 0;
}


//求第n个斐波那契数字
/*
//递归计算 思维清晰 计算量比较大
int Fib(int a)
{
    if(a<=2)
      return 1;
    else
    {
        return Fib(a-2)+Fib(a-1);
    }
}
*/
//迭代计算 计算量较小
int Fib(int n)
{
    int a = 1;
    int b = 1;
    int c = 0;
    while (n >= 3)
    {
        c = a + b;
        a = b;
        b = c;
        n--;
    }
    return c;
}
int main()
{
    int n = 0;
    scanf("%d", &n);
    int ret = Fib(n);

    printf("%d", ret);

    return 0;
}

//小青蛙跳台阶
/*
青蛙一次可以跳一级 也可以跳两级
求青蛙跳上n级台阶总共有多少种跳法
当n=1 F(n)=1
当n=2 F(n)=2
当n>2 F(n)=F(n-1)+F(n-2) 青蛙最后一次跳一级或者两级到达终点
...求F(n) 递归 类斐波那契额数字
*/

/*
汉诺塔游戏
*/