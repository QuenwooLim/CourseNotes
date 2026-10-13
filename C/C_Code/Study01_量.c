#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
int main()
{
    //a作用域为整个范围 两个a输出都为10 ;生命周期为工程结束时
    int a = 10;
    {
        //a作用域为括号内所在范围
        int a = 10;
        printf("%d\n", a);
    }//a出{}未声明

    printf("%d\n", a);
    return 0;
}


int a = 100;//全局变量 全局变量不初始化默认为0
int main()
{
    //变量
    short age = 18;
    int high = 168;
    float weight = 65;

    //局部变量 局部变量不初始化默认随机值
    int b = 200;

    //局部变量优先全局变量
    int a = 150;
    printf("%d\n", a);//150

    return 0;
}


//define定义的标识符常量
#define MAX 100

//枚举常量
enum Color
{
    RED,
    GREEN,
    BLUE
};
int main()
{
    //字面常量 eg 1； 3.14；

    //  arr[n]//中n为本质为变量 除了C99一般不支持

    const int a = 10;    //const修饰的常变量 a本质是变量 不能直接被修改，有常量的属性
    printf("%d\n", a);

    printf("%d\n", MAX);

    //向内存申请空间 c为颜色red
    int num = 10;
    enum Color c = RED;

    return 0;
}

int main()
{
    //初始化
    int num1 = 0;
    int num2 = 0;


    //求和； &取地址
    scanf("%d %d", &num1, &num2);
    //输入 eg:10 20

    scanf("num1=%d num2=%d", &num1, &num2);
    //若scanf""中有num1=则在输入框输入也要打出来
    //输入框必须eg:num1=10 num2=20

    int sum = num1 + num2;
    printf("%d\n", sum);

    return 0;
}

int main()
{
    int age = 20;
    double price = 66.6;
    //类型的本质 向内存申请空间放入变量
    return 0;
}

int main()
{
    printf("%d\n", sizeof(char));
    printf("%d\n", sizeof(short));
    printf("%d\n", sizeof(int));
    printf("%d\n", sizeof(long));
    printf("%d\n", sizeof(long long));
    printf("%d\n", sizeof(float));
    printf("%d\n", sizeof(double));
    return 0;
}

/*
十进制Octal %o 的基是10，有10个数字0，1，2，3......9
二进制Binary 的基是2，有两个数字0，1
十六进制Hexa %x 的基是16，有16个数字0，1...9，A，B，...F
八进制的基是8，有8个数字0，1，...7

十进制转化R进制
除R取余(数)倒排法
89除以2得44 取余得1
44除以2得22 取余得0
22除以2得11 取余得0
11除以2得5 取余得1
5除以2得2 取余得1
2除以2得1 取余得0
1除以2得0(end) 取余得1
则89(十进制)->1001101(二进制)
*/

/*
整形常量的书写
前缀：1.二进制0b
      2.八进制0
      3.十六进制0x
后缀：1.无
      2.u（大小写一样） 无符号
      3.l 长整形
      4.lu 无符号长整形
      5.ll 长长整形
      6.f 浮点型

浮点常量
小数：必须包含整数部分、小数部分，或者同时包含两者
指数：e前面的是尾数部分，必须有
      e后面的是指数部分，为整数（可以没有 则默认10）
      eg 1.5e-10 表示1.5*10^-10
      科学表示法的输出eg: %.4e 表示（小数点后位数）精度为4
*/
int main()
{
    double x, y, z;
    x = -2.263;
    y = 13.367;//先定义再初始化

    double m = -2.263, n = 13.367;//定义和赋初值一起 且多个变量一起操作也可以

    z = x + y;
    printf("%-8.2f + %08.2f = %.4e", x, y, z);
    /*
    -8.2f：-表示左边对齐 宽度为8 小数点后保留2位 浮点数输出
    08.2f：空格位置用0补齐 宽度为8 精度2位 浮点数
    .4e： 精度4位 科学计算法输出
    */

    return 0;
}

/*
%：字符c 整形d 科学计算法e 浮点数f
   double浮点数lf  long double浮点数Lf
   无符号u 八进制o 十六进制x 指针p 字符串s 百分号%

   左对齐- 显示正负符号+ 
   转换说明# 如%#o 则以0dddd格式打印
*/

/*
计算机中连续的实数用 float 或 double 表示。由于离散表示，使得变量中存储的计算结果很多时
候是实际实数的近似值。
因此，两个实数变量不能使用 == 或 != 直接比较，或判断实数变量为零。特别是循环语句，循环条件
表示式中使用类似 (x==y) 这样的比较，可能导致不确定的死循环（有时能循环中止，有时死循环）
实战中，实数比较运算一般仅使用 > 和 < 两个比较运算符

两个实数相等使用 fabs(x-y) < eps， eps 是合适的小数如 1e-7
实数等于零使用 fabs(x) < eps，eps 又称精度控制量（值）
*/
#include<math.h>
int main()
{
    const double eps = 1e-6;
    double a = 10.0;
    double b = 10.0;
    if (fabs(a - b) < eps)
        printf("a==b");
    if (fabs(a) < eps)
        printf("a==0");
    return 0;
}


/*
extern 外部变量
1.当一个变量是全局变量时，extern才能起作用
2.extern+类型+变量 指明类型和变量，不需要重新赋值
3.extern 函数 需要指明返回值类型 函数名 参数
多文件编译中，其初始值由其在其他文件中的定义决定，而不是默认零值
*/
//引用同一个文件中的变量
int main()
{
    extern int num;
    printf("%d", num);
    return 0;
}
int num = 10;

//引用其他文件的变量或函数
extern int add(int x, int y);//引用外部函数
int main()
{
    int a = 10;
    extern int b;//引用外部变量
    int sum = add(a, b);
    printf("%d\n", sum);

    return 0;
}