#define _CRT_SECURE_NO_WARNINGS


//结构是一种集合  一组相同类型元素的集合
//复杂对象 人：名字+电话+性别+身高
//        书：书名+作者+定价+出版时间
struct person
{
    char name[20];
    char tel[12];
    char sex[5];//女 男 保密(一个中文占用两个字节+'\0'共五个字节)
    float high;
}p1, p2;//p1 p2是使用结构体people创建的两个全局的结构体变量

//结构嵌套
struct ST
{
    struct person p3;
    char place[10];
    float weight;
};

//函数打印 两种方法效果等同
void print1(struct person* p)//引入地址
{
    printf("%s %s %s %.2f", p->name, p->tel, p->sex, p->high);//箭头操作符 结构体指针->成员变量
}
void print2(struct person p)
{
    printf("%s %s %s %.2f", p.name, p.tel, p.sex, p.high);//点操作符 结构体变量.成员变量
}

int main()
{
    struct person p3 = { "张三","13421145433","男",178.5f };//178.5f表示浮点数
    //创建的结构体变量

    //结构体嵌套
    struct ST s = { {"张三","13421145433","男",178.5f},"guangdong",68.5f };

    //打印
    printf("%s %s %s %.2f\n", p3.name, p3.tel, p3.sex, p3.high);
    printf("%s %s %s %.2f %s %.2f\n", s.p3.name, s.p3.tel, s.p3.sex, s.p3.high, s.place, s.weight);

    print1(&p3);//结构体传参 效率更高
    print2(p3);

    return 0;
}

//结构体访问
struct Stu
{
    char name[20];
    int age;
    double score;
};

void set_stu(struct Stu* ps)
{
    //  ps.name="zhangsan" //error: name 是数组名表首元素地址 zhangsan不是地址
    strcpy((*ps).name, "zhangsan");//字符串赋值需要使用strcpy
    (*ps).age = 20;//使用.来访问结构体  结构体对象.成员
    //等效 ps->age 结构体指针->成员

    (*ps).score = 100.0;
}
void print_stu(struct Stu ss)
{
    printf("%s %d %.2lf", ss.name, ss.age, ss.score);
}

int main()
{
    struct Stu s = { 0 };
    set_stu(&s);//传地址 可修改实参
    print_stu(s);

    return 0;
}