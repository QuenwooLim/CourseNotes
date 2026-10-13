#define _CRT_SECURE_NO_WARNINGS

/*
标识符必须是数字，下划线以及字母组成 (字母大小写有区别，不能以数字开头
包括：对象（变量） 函数 标签（struct union enum） typedf名 标号名 宏名 宏形参名

表达式：1.类型
        2.值类别：左值表达式（如变量），表达式可以取地址，可以使用自增运算符
                  右值表达式（如常量），表达式计算结果只能是一个数值，没有储存位置
        3.运算符：优先级：！> 算术运算符 > 关系运算符 > && > || > 赋值运算符
                  结合性：多个同级运算符，先执行左边称左结合性
                  （优先级和结合性要记住一个表格！！）
        4.隐式转换：两个操作数转化为相同的类型
                   整形的i,j需要进行浮点除法 eg:1.0*i/j 或(double)i/j

关键字是c语言内置的 不能自己创造
      变量的名字不能是关键字eg int for错误,
      变量必须有意义，由下划线 数字 字母组成 不能以数字 开头
循环语句 for,while,do while,break,continue
分支语句 if,else,switch,case,default
类型char,short,int,long,float,double,
signed有符号的,unsigned无符号的,enum枚举，struct结构体,union联合体
const常属性
extern声明外部符号
register寄存器  static静态的
return函数返回值
void 无 函数的返回类型 函数参数
sizeof 计算大小
typedef类型重命名
*/

int main()
{
    int a = 7 / 2;//求商
    printf("%d\n", a);

    int b = 7 % 2;//取模 只能是整数
    printf("%d\n", b);

    float c = 7.0 / 2;//只要有浮点数就进行浮点数除法
    printf("%f\n", c);
    //printf("%.2f\n",c);“.2”表示小数点后两位

    int d = 0;
    d = 20;//等号赋值
    d += 3;//d=d+3 +=为赋值操作符
    printf("%d\n", d);

    return 0;
}

int main()
{
    int flag = 2;//!为逻辑反操作符 因此无反应
    if (!flag)
    {
        printf("sb\n");
    }

    //sizeof是单目操作符 单位是字节
    int arr[10] = { 0 };
    printf("%d\n", sizeof(arr));//10个0 结果40

    int a = 10;
    int b = a++;//后置++ 先使用 后++
    printf("%d\n", a);//a=a-1
    printf("%d\n", b);//b=a

    int c = ++a;//前置++ 先++ 后使用
    printf("%d\n", a);//a=a+1
    printf("%d\n", c);//c=a

    //3.14默认为double类型浮点数 ；使用int强行转为整形3
    int d = (int)3.14;
    printf("%d\n", d);

    return 0;
}

int Add(int x, int y)
{
    return(x + y);
}
int main()
{
    //关系操作符:  !=测试“不相等” 
    //             ==测试“相等”

    // &&:与
    // ||:或
    int a = 10;
    int b = 20;
    if (a && b)//a与b都为非0数字
    {
        printf("wocao\n");
    }

    //三目操作符 exp1?exp1:exp3 真:exp2 假:exp3
    int c = 100;
    int d = 200;
    int r = (c > d ? c : d);
    printf("%d\n", r);

    int f = 10;
    int g = 20;
    int h = 0;
    int i = (h = f + 2, f = g + h, h - 5);
    printf("%d\n", i);
    //逗号表达式 从左往右依次计算 整个表达式的最后一个结果即总结果


    int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
    arr[3];//[]就是下标引用操作符 []内部可以为常变量 

    int sum = Add(2, 3);
    // ()为函数引用操作符 2，3是操作数
    return 0;
}

//数组
int main()
{
    //向内存申请空间 数组第一个下标为0开始
    int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };

    printf("%d\n", arr[8]);

    int i = 0;
    while (i < 10)
    {
        printf("%d\n", arr[i]);
        i = i + 1;
    }

    return 0;
}

//string length头文件
#include<string.h>
int main()
{
    //char 字符类型
    //'a'字符
    //char ch='w'

    //字符串 "abcdefg" 第一个从0开始； 结束标志是\0
    char arr1[] = "abcdef";
    //这里有初始化申请空间可以不写

    char arr2[] = { 'a','b','c','d','e','f' };
    //打印字符串 未知\0结束标志 乱码
    printf("%s\n", arr2);

    char arr3[] = { 'a','b','c','d','e','f','\0' };
    //有\0终止 abcdef
    printf("%s\n", arr3);

    //求字符串长度 不包含\0(sizeof包含\0) 包含空格 转义字符为为一个字符; arr2未知\0 随机
    int len = strlen("abcdef");
    printf("%d\n", len);

    return 0;
}

int main()
{
    //\n表示换行
    printf("abcdef\n");

    //\0表示结束
    printf("abc\0def\n");

    //三字符词 ??)表示] ??(表示[ ;\?可单纯表示？

    //%:d整形；c字符；s字符串；f float类型数据；lf double类型数据

    // \'防止其他‘干扰，同\"
    printf("%c\n", '\'');

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

//选择
int main()
{
    int input = 0;
    printf("尊嘟假嘟(1/0)?\n");
    scanf("%d", &input);
    if (input == 1)
    {
        printf("0.0");
    }
    else
    {
        printf("o.o");
    }
    return 0;
}

//循环
int main()
{
    int line = 0;
    printf("学习\n");

    while (line < 10);
    {
        printf("写代码:%d\n", line);
        line++;
    }

    if (line >= 10);
    {
        printf("牛逼\n");
    }

    return 0;
}

//define是预处理指令 不是关键词
//define定义标识符常量
#define NUM 100

//define定义宏 函数
    // 宏名 参数   宏体
#define ADD(x,y) ((x)+(y))
/*
int Add(int x,int y)
{
    return x+y;
}
*/
int main()
{
    int a = 10;
    int b = 20;
    int c = ADD(a, b);
    printf("%d\n", c);

    return 0;
}

int main()
{
    //寄存器变量 建议3存放寄存中 一般用于变量大量使用
    register int num = 3;
    return 0;
}

//void表示函数不需要返回 
void test()
{
    //static修饰局部变量，
    //可以让a不会被销毁，改变存储位置影响生命周期
    //static使全局变量的外部链接属性变成内存链接属性
    static int a = 1;;
    a++;
    printf("%d\n", a);
}
int main()
{
    int i = 0;
    while (i < 10)
    {
        test();
        i++;
    }

    return 0;
}


//描述学生的结构体
struct stu
{
    char name[20];
    int age;
    char sex[10];
    char tele[12];
};

void print(struct stu* pA)
{
    //->  结构体指针->成员名
    printf("%s %d %s %s\n", pA->name, pA->age, pA->sex, pA->tele);
}

int main()
{
    struct stu A = { "linhongyu" , 18 , "male" , "13421145433" };
    printf("%s %d %s %s\n", A.name, A.age, A.sex, A.tele);
    //结构体对象.成员名

    print(&A);
    return 0;
}

//重新起名字 只能对类型
typedef unsigned int uint;

int main()
{
    unsigned int num1 = 0;
    //等价 创建方式,可以简化操作
    uint num2 = 1;

    return 0;
}

int main()
{
    int a = 10;
    &a;//取地址
    printf("%p\n", &a);//%p打印地址
    int* p = &a; //p就是指针变量，指针也叫地址
    //*说明p是指针变量，int*是一个指针变量的类型

    //一个指针变量的大小取决于存放地址的需要,64位为8个字节 32位为4个字节
    printf("%zu\n", sizeof(int*));

    *p = 20;//*p就是通过地址找到所指对象,即让a=20
    printf("%d\n", a);

    return 0;
}