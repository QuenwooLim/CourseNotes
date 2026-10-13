#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

//语句：1.表达式 2.函数调用 3.控制 4.复合 5.空
//控制语句：
//c语言是结构化的程序设计语言：顺序 
//                          选择 if,switch 
//                          循环 for,while,do while
//                          转向 break,goto,continue,return  

#include<stdbool.h> 
/*
布尔表达式
if(表达式) if中的表达式是布尔表达式
在计算机内部：true->1 false->0
true 1 ;false 0 类型bool _Bool
返回值属于0 1的表达式称为布尔表达式：1.比较表达式 
                                     2.逻辑表达式 
                                     3.使用(_Bool)强制类型转换 非零->1
bool占用一个字节 sizeof(bool)==1
*/
int main()
{
    printf("%d\n", sizeof(bool));

    bool x = 7;//bool类型的值只有0 1
    printf("%d\n", x);//得到 x为1   

    int y = (_Bool)7;//强制类型转换 int类型的7(非0) 变成bool类型的1
    printf("%d", y);

    return 0;
}

int main()
{
    int age = 19;
    scanf("%d", &age);

    //if中有多个语句要使用大括号
    if (age < 18)//且注意没有分号
    {
        printf("未成年\n");
        printf("少打游戏\n");
    }
    else if (age >= 18 && age < 28)
        printf("青年\n");
    else if (age >= 28 && age < 55)
        printf("壮年\n");
    else
        printf("老年\n");

    return 0;
}

int main()

{
    int a = 10;
    int b = 20;
    if (a == 0)
        if (b == 20)
            printf("he\n");
    //没有括号时 else自动与最近的if匹配
        else
            printf("hehe\n");

    return 0;
}

int main()
{
    int num = 0;
    scanf("%d", &num);

    if (num % 2 == 1)
        printf("奇数");
    else
        printf("偶数");

    return 0;
}


int main()
{
    int day = 0;
    scanf("%d", &day);
    switch (day)
    //case后的值必须是整数的结果 常量或常量表达式 
    //eg:整形"1",整形表达式"1+1",字符(字符和整数可以相互转换)'A'=65
    {
    case 1:
        printf("星期一\n");
        break;
    case 2:
        printf("星期二\n");
        break;
    case 3:
        printf("星期三\n");
        break;
    case 4:
        printf("星期四\n");
        break;
    case 5:
        printf("星期五\n");
        break;
    case 6:
        printf("星期六\n");
        break;
    case 7:
        printf("星期日\n");
        break;
        //switch是允许嵌套的
    }

    return 0;
}

int main()
{
    int day = 0;
    scanf("%d", &day);

    switch (day)
    {
    case 1://直接进去指定case， 没有break依次进入后面的case
    case 2:
    case 3:
    case 4:
    case 5:
        printf("weekday");
        break;
    case 6:
    case 7:
        printf("weekend");
        break;
    default:
        printf("输入错误");
        break;
    }

    return 0;
}