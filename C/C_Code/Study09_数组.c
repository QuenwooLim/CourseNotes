#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS_
#include<stdlib.h>
#include<time.h>
#include<math.h>

/*
一维数组的定义，需要指定数组长度 int a[100]

一维数组初始化
1.对所有元素赋值 int a[5]={0,1,2,3,4}
2.对部分元素赋值 int a[5]={0,1} 剩余元素自动赋0
3.不指定数组长度 int a[]={0,1,2}
ps:如果数组长度与提供初值个数不同，则[]内数组长度不能省略
*/

int main()
{
    // 数组元素类型 数组名 存放元素个数(必须是常量) 
    int arr[10];//数组存整形
    char ch[5];//字符
    double data[20];

    //不完全初始化，剩下的元素默认为\0即是0;
    int arr2[10] = { 1,2,3 };

    char ch1[10] = { 'a','b','c' };//字符集不会自动加\0
    // a b c 0 0 0 0 0 0 0
    char ch2[10] = "abc";//字符串自动加\0
    // a b c \0 0 0 0 0 0 0

    char ch3[] = "i am happy";
    //数组的长度是11（还有'\0'）

    char ch4[] = {"i am happy"};
    //等价 char ch4[]={'i',' ','a','m',' ','h','a','p','p','y'};

    char ch5[3] = { 'H','i','!' };//不是字符串 没有结束标识符
    char ch6[4] = { 'H','i','!' ,'\0' };//是字符串

    //字符数组输入 How are you?
    char str1[5], str2[5], str3[5];
    //scanf("%s", str1);//error 只能接收到How 系统将空格字符作为输入的字符分隔符
    //遇到空白字符时，输入停止，仅获得空白字符之前的字符串
    scanf("%s%s%s", str1, str2, str3);//有空格分隔符 作为三个字符串输入

    printf("%s", str1);
    //字符串可以利用%s整体输出
    //但对于其他类型(eg:int float..)的数组，只能循环逐个输出，不能整体输出

    return 0;
}

//数组下标
int main()
{
    int arr[10] = { 0 };
    arr[7] = 8;//可以执行
    //等效操作  
//   7[arr]=8

    //arr 7 [] 这三个都是操作数
    //arr[7]=*(arr+7)=*(7+arr)=7[arr]

    return 0;
}

//二维数组三行四列 都是从零开始的
// 1 2 3 4
// 2 3 4 5
// 3 4 5 6

int main()
{
    //二维数组初始化
    int arr1[3][4] = { 1,2,3,4,2,3,4,5,3,4,5,6 };//三行四列,没有放元素即默认0
    int arr2[3][4] = { {1,2},{2,3},{3,4} };//手动分三组
    // char arr3[][5];//二维数组行数可以省略 列数不可以省略
    //对于多维数组 除了第一维 其他维度不可以省略

    printf("%d", arr1[2][0]);//行列从0开始计算，即左下角的3

    //数组访问不能越界 超过下标会随机打印数值(编译器并不会报错)
    //在内存中是线性存储的 一行一行存放

    return 0;
}

//把数组排列成升序
//冒泡排序的算法，对数组进行排序]
//冒泡排序核心思想是两个相邻的元素进行比较

//数组传参有两种写法：1.数组 2.指针
// 数组形参是一个变量，可保存数组的地址，不定长数组 a[] 多维数组仅第一维用不定长数组 int[][10]
//               int* arr形参也可以 表示首元素的地址
void bubble_sort(int arr[], int sz)
{
    int i = 0;
    for (i = 0; i < sz - 1; i++)
    {
        int j = 0;
        for (j = 0; j < sz - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            else
                continue;
        }
    }
}
int main()
{
    int arr[] = { 9,8,7,6,5,4,3,2,1,0 };
    int sz = sizeof(arr) / sizeof(arr[1]);

    bubble_sort(arr, sz);//数组传参 数组名本质是首元素的地址

    int i = 0;
    for (i = 0; i < sz; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}

//数组名能表示首元素的地址
//但是有两个意外；1.sizeof（数组名）计算整个数组的大小
//                2.&数组名 表示整个数组 取的是整个数组的地址
int main()
{
    int arr[10] = { 0 };
    printf("%p\n", arr);//arr就是首元素的地址  相同
    printf("%p\n", arr + 1);//  地址+4

    printf("%p\n", &arr[0]);//首元素的地址  相同
    printf("%p\n", &arr[0] + 1);//  地址+4

    printf("%p\n", &arr);//数组的地址  相同
    printf("%p\n", &arr + 1);//  地址+40

    //地址是用16进制表示的

    return 0;
}


int main()
{
    int arr[3][4];

    arr;//二维数组的数组名表示数组首(行)元素的地址 即第一行
    //1 2 3 4 第一行√
    //2 3 4 5 第二行
    //3 4 5 6 第三行

    arr + 1;//第二行地址

    printf("%p\n", arr);
    printf("%p\n", arr + 1);//相比arr arr+1多加了四个整形地址的字节(16) 

    //计算几行
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("%d\n", n);

    //计算几列
    int m = sizeof(arr[0]) / sizeof(arr[0][0]);
    printf("%d\n", m);


    return 0;
}