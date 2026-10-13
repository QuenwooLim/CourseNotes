#include<stdio.h>

int main()
{
    int flag=2;//!为逻辑反操作符 因此无反应
    if(!flag)
    {
        printf("sb\n");
    }

    //sizeof是单目操作符 单位是字节
    int arr[10]={0};
    printf("%d\n",sizeof(arr));//10个0 结果40

    int a=10;
    int b=a++;//后置++ 先使用 后++
    printf("%d\n",a);//a=a+1
    printf("%d\n",b);//b=a

    int c=++a;//前置++ 先++ 后使用
    printf("%d\n",a);//a=a+1
    printf("%d\n",c);//c=a
    
    //3.14默认为double类型浮点数 ；使用int强行转为整形3
    int d=(int)3.14;
    printf("%d\n",d);

    return 0;
}