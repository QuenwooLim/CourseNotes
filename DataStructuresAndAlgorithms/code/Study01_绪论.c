#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
/*
基本概念和术语:
1.数据:能够被计算机处理的各种符号的集合 
2.数据元素:在计算机中作为一个整体进行考虑(如类
3.数据对象:是性质相同的数据元素的集合,是数据的一个子集(如:自然数,学籍表
4.数据结构:
  逻辑结构:分法一:线性结构:线性表，栈，队列，传(一对一)
                  非线性结构:树，图(一对多，网状)
           分法二:集合结构:数据元素同属于一个集合
                  线性结构:一对一
                  数形结构:一对多
                  图状或网状结构:多对多
  存储结构:顺序存储:用一组连续的存储单元依次存储元素，元素的逻辑关系由存储位置表示(数组
           链式存储:用任意的存储单元，元素之间的逻辑关西用指针表示(存储元素时同时存储下一个元素指针
           索引存储:存储结点信息时，顺便建立索引表
           散列存储:使用结点的关键字计算出它的地址
  数据的元素和实现
5.类型:
  数据类型:值的集合以及值的一组操作集合 int,数组
  抽象数据类型(ADT):三元组:数据对象D，数据对象的关系集S,数据对象的基本操作集P
                    ADT 抽象数据类型名
                    {
                        数据对象的定义;
                        数据关系的定义;
                        基本操作;
                    }ADT 抽象数据类型名
*/

/*
抽象数据类型的表示与实现
复数
*/
typedef struct
{
    float realpart;
    float imagpart;
}Complex;
void assign(Complex* A, float real, float imag)//赋值
{
    A->realpart = real;
    A->imagpart = imag;
}
void add(Complex* ad, Complex A, Complex B)//加法
{
    ad->realpart = A.realpart + B.realpart;
    ad->imagpart = A.realpart + B.imagpart;
}
void Sub(Complex* su, Complex A, Complex B)//减法
{
    su->realpart = A.realpart - B.realpart;
    su->imagpart = A.realpart - B.imagpart;
}
void Mul(Complex* mu, Complex A, Complex B);//乘法
void Div(Complex* di, Complex A, Complex B);//除法（分母不为0）

int main()
{
    Complex z1, z2, z3, z4, z5;
    assign(&z1, 8.0, 6.0);
    assign(&z2, 4.0, 3.0);
    add(&z3,z1, z2);
    printf("z3=%f+%fi", z3.realpart, z3.imagpart);

    return 0;
}

/*
算法和算法分析
算法特性:确定性，可行性，有穷性，输入与输出
*/

/*
算法时间性:算法运行总时间=所有语句执行次数*执行一条语句的单位时间;
           一般通过比较数量级和时间复杂度的渐近表示法
*/
//例1
int main()
{
    int i,j, k,n;
    int a[100][100], b[100][100], c[100][100];
    for (i = 1; i <= n; i++)//n+1: 执行n次+判断1次
    {
        for (j = 1; j <= n; j++)//n(n+1)
        {
            c[i][j] = 0;//n*n
            for (k = 0; k < n; k++)//n*n*(n+1)
            {
                c[i][j] = c[i][j] + a[i][k] * b[i][k];//n*n*n 基本语句 也循环嵌套层次最深的
            }
        }
    }

    return 0;
}
//循环耗费时间 T(n)=2*n^3+3*n^2+2n+1  只需要考虑最高次项(基本操作 执行次数最多)且不考虑系数n^3
//n->∞,T(n)/n^3->2 同阶级函数 T(n)=O(n^3)
//O(f(n))为算哒的渐进时间复杂度

//例2
int main()
{
    float arr[10][10];
    int m;
    int n;
    float sum[10];
    int i;
    int j;
    for (i = 0; i < m; i++)
    {
        sum[i] = 0.0;
        for (j = 0; j < n; j++)
        {
            sum[i] += arr[i][j];//嵌套层次最深 循环次数最多 f(n)=m*n
        }
    }
    for (i = 0; i < m; i++)
    {
        printf("%d:%f", i, sum[i]);
    }
    return 0;
}

//例3
int main()
{
    int n;
    int x;
    for (int i; i <= n; i++)
        for (int j; j <= i; j++)
            for (int k = 1; k <= j; k++)
                x = x + 1;
}
//                  n    i    j
//使用求和符  sigm  Σ   Σ   Σ 1
//                  i=1  j=1  k=1

//例4
int main()
{
    int i = 1;
    int n;
    while (i <= n)
        i = i * 2;
}
//执行次数为x时候 i=2^x;    2^x<=n求得 x<= ln(n)/ln(2)

//例5
int main()
{
    int i,n;
    int a[26];
    for (i = 0; i < n; i++)
        if (a[i] == 'e')
            return i + 1;
    return 0;
}
//执行次数随问题的输入数据集不同而不同
//执行 最好情况 1次 最差情况 n次 平均情况(n+1)/2次

//例6 略
//对于复杂的算法 可以将它分成几个容易估算的部分 利用大O的加法和乘法法则
//加法T(n)=T1+T2=O(f(n))+O(g(n))=O(max{f(n),g(n)})
//乘法T(n)=T1*T2=O(f(n))*O(g(n))=O(f(n)*g(n))

//算法空间性:本身占据的空间，输入输出，指令，常数，变量 以及辅助空间
             //空间复杂度 S(n)=O(1)
void test01()
{
    int i, n, t,a[100];
    for (i = 0; i < n / 2; i++)
    {
        t = a[i];
        a[i] = a[n - i - 1];
        a[n - i - 1] = t;
    }
}
void test02()
{
    int i, n, a[100], b[100];
    for (i = 0; i < n; i++)
        b[i] = a[n - i - 1];
    for (i = 0; i < n; i++)
        a[i] = b[i];
}
int main()
{
    test01();
    test02();
    return 0;
}