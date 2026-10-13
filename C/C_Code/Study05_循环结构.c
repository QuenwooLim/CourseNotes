#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
/*
if()的多次使用-->while()
*/
int main()
{
    int i = 1;
    while (i < 11)
    {
        if (i == 5)
            break;//只要有break终止整个循环，break作用于最近的一个while循环
        //continue  如果有continue循环直接回到开始,提前结束循环
        printf("%d\n", i);
        i++;
    }
    return 0;
}

int main()
{
    /*
        int ch=0;
        while((ch=getchar())!=EOF)
        //getchar可以读取输入缓冲区中的字符 直到\n时结束
        //getchar获得字符的ASCII值,等效scanf获取输入的字符(但scanf有"abc def"遇到空格不再读)
        {
            putchar(ch);//打印
        }
    */

    char password[20] = { 0 };
    printf("请输入密码:");
    scanf("%s", password);
    /*
        getchar();//清除输入缓冲区的/n
    */
    int a = 0;
    while ((getchar()) != '\n')
    {
        ;//什么都不用干 保持循环 清空读取缓冲区
    }

    printf("请确认密码:");

    int b = getchar();

    if ('Y' == b)//单个字符使用'',字符串使用""默认携带/0,eg:"A"为2个字节的字符串
        printf("正确的");
    else
        printf("错误的");

    return 0;
}

int main()
{
    int n = 0;
    printf("input a string:");//输入一个字符串
    while (getchar() != '\n')//读取字符个数 直到\n字符串结束退出循环
        n++;
    printf("number of characters:%d", n);

    return 0;
}

int main()
{
again:
    printf("1\n");
    printf("2\n");
    goto again;//循环

    return 0;
}

#include<stdlib.h>
#include<time.h>
int main()
{
    char input[20] = { 0 };
    //system("shutdowm -s -t 60");//电脑将在六十秒内关机
again:
    printf("电脑即将在一分钟后关机,输入“woshizhu”取消关机程序\n");
    scanf("%s", input);
    if (strcmp(input, "woshizhu") == 0)
    {
        //system ("shutdowm -a");//取消关机
        printf("取消成功，你是猪");
    }
    else
    {
        goto again;
    }
    return 0;
}

/*
在试运行过程中可以先使用printf等
注意在提交最终结果时候注释掉
*/

/*
do while 语句结束时判断条件 至少do一次
while 语句开始时判断条件
*/
int main()
{
    int x;
    int n = 0;
    scanf("%d", &x);
    do
    {
        n /= 10;
        n++;
        //printf("%d",n);
    } while (x > 0);
    printf("%d", n);

    return 0;
}

//逗号表达式
int main()
{
    /*
    a=get_val();
    count_val(a);
    while(a>0)
    {
        数据处理
        a=get_val();
        count_val(a);
    }
    */
    //使用逗号表达式可以转化为
    while (a = get_val(), count_val(a), a > 0)//直接看a>0来判断循环是否执行
    {
        //数据处理
    }

    return 0;
}