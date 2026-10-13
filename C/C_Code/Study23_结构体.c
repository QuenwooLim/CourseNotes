#define _CRT_SECURE_NO_WARNINGS

//已知这个结构体大小是20个字节
struct Teat
{
	int Num;
	char* pcName;
	short sDate;
	char cha[2];
	short sBa[4];
}*p = (struct Test*)0x100000;//强制类型转换防止报错
int main()
{
	//0x1是16进制的1 也是数字1
	printf("%p\n", p + 0x1);//p是结构体类型 +1跳过一个结构体20个字节
	printf("%p\n", (unsigned long)p + 0x1);//p强制类型转化为长整型 +1 就是整型数字+1
	printf("%p\n", (unsigned int*)p + 0x1);//p强制类型转化为指针类型 +1就是+4/8个字节

	return 0;
}


/*
除了内置类型之外
为了分类其他复杂对象 需要使用到自定义类型
自定义类型:结构体 联合体 枚举
*/
struct stu
{
	char name[20];
	int age;
}s1 = { "lhy",18 }, s2;//创建全局变量 初始化
#include<string.h>
int main()
{
	struct stu s3 = { "lhy",18 };//创建局部变量 定义时赋值

	struct stu s4;//定义后逐个赋值
	strcpy(s4.name, "lhy");
	s4.age = 18;

	return 0;
}

//匿名结构体类型
struct
{
	int a;
	char b;
	float c;
}s;//匿名结构体类型只能创建一个全局变量
struct
{
	int a;
	char b;
	float c;
}a[20], * p;//a[20]是创捷结构体类型的数组 p是结构体类型的指针

int main()
{
	//	struct s;//匿名结构体类型无法创建局部变量
	//	p = &s;//匿名结构体的弊端 编译器处理时是不同类型
	return 0;
}


/*
结构体字节对齐（平台原因防止异常 和性能原因便于读取数据）
规则:
1.第一个成员在结构体变量偏移量为0（起始）的地址处
2.其他成员变量要对齐到某个数字（对齐数）的整数倍的偏移量地址处 即地址是对齐数的整数倍
  对齐数=min{编译器默认的一个对齐数（vs是8 其他编译器没有默认），该成员字节大小}
3.结构体字节总大小为最大对齐数（每个成员变量都有一个对齐数）的整数倍
4.如果嵌套了结构体的情况，嵌套的结构体对齐到自己的最大对齐数的整数倍处，
  结构体的总大小就是所有最大对齐数（含嵌套结构体的对齐数）的整数倍
tips:在设计结构体时候 既要满足对齐，又要节省空间 让占用空间小的成员尽量集中在一起
*/

#include <stddef.h>//offsetof的库函数 
//pragma pack(2) //使结构体按照对齐值2对齐 修改默认对齐数（有利于减少内存空间浪费）
struct s1
{
	char c1;//先放到0地址处 占用0
	//1 2 3 地址处闲置
	int i;//字节对齐放到4(成员字节4和vs默认8的较小值 即对齐数的倍数) 占用4 5 6 7地址处
	char c2;//字节放到8（成员字节1和vs默认8的较小值 即对齐数的倍数) 占用8
	//9 10 11闲置
	//结构体总字节大小为12（c1对齐数是1 i对齐数是4 c2对齐数是1） 是最大对齐数4的倍数
};
struct s2
{
	char c1;//偏移量地址0
	char c2;//偏移量地址1
	int i;//偏移量地址4
	struct s1 a;//偏移量地址8
};
int main()
{
	printf("%d\n", sizeof(struct s1));//12
	printf("%d\n", sizeof(struct s2));//8+12

	//打印偏移位
	printf("%d\n", offsetof(struct s1, c1));//0 //offsetof(类型,结构体成员)  
	printf("%d\n", offsetof(struct s1, i));//4
	printf("%d\n", offsetof(struct s1, c2));//8

	return 0;
}


/*
位段：为了节省空间
1.成员必须是int，unsigned int，signed int
2.成员名的后边必须有一个冒号和一个数字
3.内存分配 按照需要以int（4个字节）或者char(1个字节)来开辟的
4.不支持跨平台 不可以移植
eg 如果flag表示真假int flag只需要0或者1两个值 只需要一个比特位
*/
struct A
{
	//先开辟四个字节(32个比特位）用来存储
	int _a : 2;//表示内存只需要两个比特位就足够了 节省空间
	int _b : 5;
	int _c : 10;
	//32个比特位不够用 再另外开辟4个字节
	int _d : 30;
};
int main()
{
	printf("%d", sizeof(struct A));
	
	return 0;
}
struct SS
{
	char a : 3;
	char b : 4;
	char c : 5;
	char d : 4;
};
int main()
{
	struct SS s = { 0 };
	printf("%d", sizeof(struct SS));//3
	/*
	存储方式
		  ........ ........ ........
	先存a .....000 ........ ........开辟第一个字节
	再存b .0000000 ........ ........第一个字节
	再存c .0000000 ...00000 ........开辟第二个字节
	再存d .0000000 ...00000 ....0000开辟第三个字节
	小端存储
	*/
	s.a = 10;//取出10的低3位 赋给a
	s.b = 12;
	s.c = 3;
	s.d = 4;

	return 0;
}

/*
如果几位位域被顺序写在一起，那么他们会被打包拼合在一起
位域对齐值按整数对齐值，及第一个位域必须按整数对齐，一个包是一个整数
0--t3--10--t2--21--t1--31  --> 一个int
拼合的特性使得位域可以用来表示寄存器一类的结构
*/
struct B
{
	unsigned t1 : 10;
	unsigned t2 : 11;
	unsigned t3 : 11;
};
int main()
{
	printf("%lu\n", sizeof(struct B));
	return 0;
}
struct C
{
	int a : 4;
	int : 2;
	int b : 5;

	int c : 4, : 2, : 0;
	int d : 4;
};
/*
匿名位域表示跳过若干位
0宽度位域表示当前整数包已满，下一个位域分配在新整数单元
位域不能求地址！！！建议与union配合获得位域的地址
*/

/*
位段跨平台问题
1.int 位段被当成有无符号是不确定的
2.尾端最大位的数目是不确定的 （16位机器最大16个字节 32位最大32个字节写成27
3.大小端不确定
4.当一个结构包含两个位段，第二个位成员比较大，无法容纳第一个位段剩余的位时 舍弃还是利用不确定
*/

/*
数据结构：数据在内存中的存储结构
线形：顺序表 链表
树形：二叉树
*/
struct Node
{
	int data;
	struct Node next;//自引用
};
int main()
{
	sizeof(struct Node);//计算出错 结构体内部无限套娃 无法计算字节
	return 0;
}

//结构体自引用 
//结构体包含自己的结构体指针
struct Node
{
	int data;
	struct Node* next;
};

/*
* 错误  先有鸡先有蛋？？
typedef struct
{
	int data;
	Node* next;
}Node;
*/

//可行 使用时候struct Node s1 等效Node s2
typedef struct Node
{
	int data;
	struct Node* next;
}Node;
//或者 typedef struct Node Node;//把struct Node 重新定义为Node


//结构体传参数
struct S
{
	int data[1000];
	int num;
};
void print1(struct S ss)//结构体传参 接收
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		printf("%d ", ss.data[i]);
	}
	printf("%d", ss.num);
}
void print2(const struct S* ps)//防止传地址后被错误修改 改变原来的内容
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		printf("%d ", ps->data[i]);
	}
	printf("%d", ps->num);
}
int main()
{

	struct S s = { {1,2,3},100 };
	print1(s);//传结构体变量 传值调用(传参的时候会压栈 使用大量空间 使效率下降)
	print2(&s);//传结构体指针 传址调用

	return 0;
}