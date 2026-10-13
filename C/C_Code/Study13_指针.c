#include <stdio.h>
#include <string.h>

int main()
{
    int a = 10;//a是整型变量 占用4个字节的内存空间
    int* pa = &a;
    //pa是一个指针变量 64占用8个字节 来存放地址
    //本质上指针就是地址 口语中说的指针，其实是指针变量，是用来存放地址的

    char* pc = (char*)&a;//强制类型转换 指针本为整形类型
    // *pc=0; //报错 指针类型不会改变地址所占空间大小 64位都是8个字节
              //但是影响 指针在被解引用时被访问几个字节
              // int*解引用访问 4个字节； char*解应用访问1个字节

    printf("pa=%p", pa);//0061FF18
    printf("pa=%x", pa);//  61FF18 
    //都可以打印地址 p打印所有位

    printf("pa=%p\n", pa);
    printf("pa+1=%p\n", pa + 1);//跳过四个字节

    printf("pc=%p\n", pc);
    printf("pc+1=%p\n", pc + 1);//跳过一个字节


    int* pi = &a;//访问4个字节
    float* pf = (float*)&a;//访问4个字节
    //不能混用 *pi=100 和*pf=100是不同概念 存储方式不同

    return 0;
}

int main()
{
    int* a;//没有初始化 没有明确指向 随机值
    *a = 10;//a在这里是野指针 可能越界访问

    int* p2 = NULL;
    // *p2=100; 错位操作 没有权限访问空指针0 系统崩溃

    return 0;
}

int* test()
{
    int n = 10;
    return &n;
}

int main()
{
    int* p = test();

    if (p != NULL)
    {
        printf("%d\n", *p);//可以打印 函数出来之后内容销毁 但是地址还没被覆盖
    }
    return 0;
}
#define N_VALUES 10
int main()
{
    float values[N_VALUES];
    float* vp;
    for (vp = &values[0]; vp < &values[N_VALUES];)
    {
        *vp++ = 0;//地址从低位到高位 *vp++是地址++ 再使对象0  ;(*vp)++是对象++
    }

    for (vp = &values[N_VALUES]; vp > &values[0];)
    {
        *--vp = 0;//前置--
    }

    //纸面规定：允许指向数组元素的指针与指向数组最后一个元素后面的那个内存未知的指针比较；
    //但不能与前面的进行比较

    return 0;
}

int main()
{
    int arr[10] = { 0 };
    printf("%d\n", &arr[9] - &arr[0]);
    //指向空间相同的元素的指针才能相减
    //指针和指针相减得到指针和指针之间元素的个数

    return 0;
}

int main()
{
    int arr[10] = { 1,2,3,4,5,6,7,8,9,0 };
    int* p = arr;//首元素的地址
    int i = 0;
    scanf("%d", &i);
    printf("%d\n", *(p + i));//效果等同
    printf("%d\n", arr[i]);//效果等同

    printf("%d\n", arr[2]);//效果等同
    printf("%d\n", 2[arr]);//效果等同

    return 0;
}

int main()
{
    int a = 10;
    int* pa = &a;//pa一级指针变量
    int** ppa = &pa;//ppa二级指针变量 int*(第一个*)说明类型 *(第二个*)说明ppa是指针
    //              用来存放一级指针变量的地址

    **ppa = 20;//a=20

    return 0;
}

int main()
{
    int a = 10;
    int b = 20;
    int c = 30;
    int* pa = &a;
    int* pb = &b;
    int* pc = &c;

    int* parr[10] = { pa,pb,pc };    //指针数组 (int*是元素的类型)

    int i = 0;
    for (i = 0; i < 3; i++)
    {
        printf("%d\n", *(parr[i]));
    }
    return 0;
};

int main()
{
    int(*pt)[4];//指针变量pt的基类型是有四个整形元素组成的一维数组

    int n[2][3] = { 1,2,3,4,5,6 };
    int(*p)[3] = &n[1];
    printf("%d %d", p[-1][2], (-1)[p][2]);//3 3

    return 0;
}

int main()
{
    int arr1[4] = { 1,2,3,4 };
    int arr2[4] = { 2,3,4,5 };
    int arr3[4] = { 3,4,5,6 };

    int* parr[3] = { arr1,arr2,arr3 };//二维数组
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        int j = 0;
        for (j = 0; j < 4; j++)
        {
            printf("%d ", parr[i][j]);//parr[1]即arr1 也是首元素的地址
        }
        printf("\n");
    }
    return 0;
}

int main()
{
    int m[3][4] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
    int(*p)[4] = m;//指向数组类型数据的指针
    int* (mp[3]) = { m[0],m[1],m[2] };//()可省略  (指向数组的)指针的数组 
    int** pp = mp;//(指向)指针(数组)的指针
    //这里通过地址pp[0]或者通过数组名mp[0]可以得到二维数组的第一行的地址m[0]
    
    return 0;
}