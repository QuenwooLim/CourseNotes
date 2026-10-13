#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

//字符串逆序（递归实现）
void reverse(char* str)
{
    char tmp = *str;
    int len = strlen(str);//strlen求字符串长度 不包括\0
    *str = *(str + len - 1);
    *(str + len - 1) = '\0';
    if (strlen(str + 1) >= 2)
        reverse(str + 1);
    *(str + len - 1) = tmp;
}
int main()
{
    char arr[] = "abcdefg";//字符串默认结束是\0
    reverse(arr);
    printf("%s\n", arr);
    return 0;
}

int DigitSum(int n)
{
    if (n > 9)
        return DigitSum(n / 10) + n % 10;
    if (n < 10)
        return n;
}
int main()
{
    int n = 0;
    printf("输入一个数字，求各位之和");
    scanf("%d", &n);
    int ret = DigitSum(n);
    printf("%d", ret);

    return 0;
}

//实现n的k次方
double Mi(int n, int k)
{
    if (k > 0)
        return n * Mi(n, k - 1);
    else if (k == 0)
        return 1;
    else//k<0
        return 1.0 / Mi(n, -k);//1.0才能结果计算有负数
}
int main()
{
    int n = 0;
    int k = 0;
    printf("输入n和k:");
    scanf("%d %d", &n, &k);
    double ret = Mi(n, k);

    printf("%lf\n", ret);

    return 0;
}

int main()
{
    //逗号表达式 从左到右访问 取4
    int arr1[] = { 1,2,(3,4),5 };

    printf("%d\n", sizeof(arr1));//1 2 4 5

    //数组也有类型 类型为int[10]:元素数据类型+数组长度
    int arr[10] = { 0 };
    printf("%d\n", sizeof(arr));
    printf("%d\n", sizeof(int[10]));


    //sizeof是操作符，计算内容所占空间
    //strlen是一个库函数，专门用来求字符串长度，统计\0之前字符长度
    char str[] = { "hello bit" };//10
    printf("%d %d", sizeof(str), strlen(str));//40 9

    //数组长度即数组内元素个数 不要混淆字符串长度
    char acx[] = "abcdefg";//8
    char acy[] = { 'a','b','c','d','e','f','g' };//7

    return 0;
}

//将AB数组内容交换
int main()
{
    int arr1[] = { 1,3,5,7,9 };
    int arr2[] = { 2,4,6,8,0 };
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        int tmp = arr1[i];
        arr1[i] = arr2[i];
        arr2[i] = tmp;
    }

    for (i = 0; i < 5; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    for (i = 0; i < 5; i++)
    {
        printf("%d ", arr2[i]);
    }

    return 0;
}


//初始化函数
void init(int arr[], int sz)
{
    int i = 0;
    for (i = 0; i < sz; i++)
    {
        arr[i] = 0;
    }
}

//打印函数
void print(int arr[], int sz)
{
    int i = 0;
    for (i = 0; i < sz; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

//倒序函数
void reserve(int arr[], int sz)
{
    int i = 0;
    int tmp = 0;
    for (i = 0; i < sz / 2; i++)
    {
        tmp = arr[i];
        arr[i] = arr[sz - 1 - i];
        arr[sz - 1 - i] = tmp;
    }
}

int main()
{
    int arr[10] = { 0,1,2,3,4,5,6,7,8,9 };
    int sz = sizeof(arr) / sizeof(arr[0]);//计算数组长度

    print(arr, sz);

    reserve(arr, sz);
    print(arr, sz);

    init(arr, sz);
    print(arr, sz);

    return 0;
}