#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/*
enum 枚举就是一一列举
枚举的优点:
1.增加代码的可读性质(通讯录的1 2 3 4 5 6)和可维护性
2.和宏定义的标识符比较 枚举有类型检查，更加严谨 enum Day d=5在cpp中会报错
3.防止命名污染（分装）
4.便于调试 宏定义直接替换不便于调试检查
5.使用方便，依次可以定义多个常量
*/

enum Day
{
	//成员是枚举常量 只能赋值=? 不能修改
	Mon,//第一位默认为0  即默认Mon=0
	Tues,
	Wed,
	Thur,
	Fri,
	Sat,
	Sun //最后一个不需要,
};


int main()
{
	enum Day d = Fri;//将Fri标识符常量定义为d d的类型为enum Day =两边类型相同
	printf("%d\n", Mon);//在枚举中 第一个成员默认为0
	printf("%d\n", Tues);//1
	printf("%d\n", Wed);//2

	return 0;
}

enum Color
{
	Red = 1,//可将第一个成员的值0赋值更改为1 后面的枚举常量也顺势赋值
	Blue,//2
	puple//3
};

/*
联合（共用体）
特点:
1.所有成员共享同一块空间 一个联合变量的大小至少是最大的成员的内存
*/

union Un
{
	int a;
	char c;
};

struct St
{
	int a;
	char c;
};

int main()
{
	union Un u;//引入共用体变量
	printf("%d\n", sizeof(u));//4

	//共用体以及所有成员的地址都相同  实质是a和c公用同一内存空间
	printf("%p\n", &u);
	printf("%p\n", &(u.a));
	printf("%p\n", &(u.c));

	u.a = 0x11223344;
	u.c = 0x00;//c修改时将a的内存高位（小端）的第一个字节44也修改了也修改了

	return 0;
}

int check_sys()
{
	union Un
	{
		char c;
		int i;
	}u;
	u.i = 1;
	return u.c;//小端则返回01 00 00 00地址的第一个字节01  大端则返回00 00 00 01的第一个字节00
}
int main()
{

	int ret = check_sys();
	if (ret == 1)
		printf("小端\n");
	else
		printf("大端\n");

	return 0;
}


//联合体也存在类型对齐
union Un
{
	char arr[5];//对齐数按照char来看 对齐数是1
	int i;//对齐数是4
};
int main()
{
	printf("%d\n", sizeof(union Un));//8 最大对齐数的整数倍
	return 0;
}