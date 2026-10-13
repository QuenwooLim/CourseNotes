#include<stdio.h>

int main()
{
    //\n表示换行
    printf("abcdef\n");

    //\0表示结束
    printf("abc\0def\n");

    //三字符词 ??)表示] ??(表示[ ;\?可单纯表示？

    //%:d整形；c字符；s字符串；f float类型数据；lf double类型数据

    // \'防止其他‘干扰，同\"
    printf("%c\n",'\'');

    // \\0表示单纯\0
    printf("abcd\\0ef\n");

    // \t水平制表符,符表示路径要"\\";
    printf("c:\\test\n");

    // \a表示蜂鸣
    printf("\a");

    // \ddd表示八进制数字 eg:\130为十进制88为ASCII编码的x
    printf("\130\n");

    // \xdd表示两个十六进制的数字 eg:\x30为48
    printf("\x30\n");

    return 0;
}