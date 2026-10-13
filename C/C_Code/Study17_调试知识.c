#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
/*
调试的基本步骤：
1.发现程序错误存在；
2.以隔离，消除等方式对错误进行定位
3.确定纠正错误产生的原因
4.提出解决办法
5.重新调试
*/

/*
debug:调试版本 具有调试信息
release:优化版本 没有调试信息 内存更小
*/
/*
F9取断点
F10逐过程调试
F11逐语句调试 可以进入函数内部
ctrl+F5执行不调试
F5执行并调试
*/
int main()
{
	int i = 0;
	int arr[10] = { 0 };

	for (i = 0; i < 10; i++)
	{
		scanf_s("%d", &arr[i]);
	}
	for (i = 0; i < 10; i++)
	{
		printf("%d ", i);//设置断点 F10逐过程调试 右键断点可以设置条件
	}
	for (i = 0; i < 10; i++)
	{
		printf("%d ", i);
	}

	return 0;
}

int Add(int x, int y)
{
	return x + y;

}
/*
调试开始才可以看见窗口
自动窗口可以看见 逐过程的变量
局部变量可以一直观察变量数值变化 值会乱动
监视功能可以一直观察某一个值的变化 用途广泛
*/
int main()
{
	int a = 10;
	int b = 20;
	int c = Add(a, b);
	printf("%d\n", c);

	return 0;
}


void test(int a[])
{

}
int main()
{
	int arr[10] = { 1,2,3,4,5,6,7,8,9,0 };
	test(arr);//只能传首元素的地址 在监视窗口只能看见a[0]  输入 a,10就可以观察10个变量

	return 0;
}
//在窗口可以打开内存观察 以16进制显示 设置一行四列有四个字节



void test2()
{
	printf("HAHA");
}

void test1()
{
	test2();
}

void test()
{
	test1();
}
int main()
{
	test();

	return 0;
}
//在调试窗口中调用堆栈 有利于观察函数如何调用逻辑
//打开反汇编可以找到汇编的代码
//打开寄存器可以找到汇编代码中数据的变化


int main()
{
	int i = 0;
	int j = 0;
	int n = 0;
	int ret = 0;
	scanf_s("%d", &n);
	int sum = 0;
	for (i = 1; i <= n; i++)
	{
		ret = 1;//调试发现错误
		for (j = 1; j <= i; j++)
		{
			ret = ret * j;
		}
		sum = sum + ret;
	}
	printf("%d", sum);
	return 0;
}



int main()
{
	int i = 0;
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	for (i = 0; i <= 12; i++)
	{
		arr[i] = 0;//x86环境下  arr[12]使得i=0
		printf("hehe\n");
	}
	return 0;
}
/*
死循环原因
1.栈区内存的使用习惯是先使用高地址处的空间再使用低地址处的空间
2.数组随着下标的增长 地址由低到高
3.如果i和arr之间有适当的空间 利用数组的越界操作可能覆盖到i 死循环
*/


#include<string.h>
int main()
{
	char arr1[10] = { "xxxxxxxxxx" };
	char arr2[] = { "hello bit" };

	strcpy(arr1, arr2);

	printf("%s", arr1);
	//通过调试发拷贝是空格和\0都可以被拷贝
	return 0;
}


//ctrl+f进行搜索
/*
编译代码常见错误：
1.编译型错误（常见）
2.链接型错误 找不到符号（不存在或者写错）
3.运行时错误（调试来解决）
*/

//高聚合 低耦合的优秀函数
#include<assert.h>
 //根据库函数 使用char* 返回值可以有利于进行链式访问
 //strcpy返回的是目标空间的起始地址
char* my_strcpy(char* dest, const char* src)//防止出现错误 const常变量无法被修改
{
	char* ret = dest;
	assert(src != NULL);//断言 防止传出空指针 且有利于排错
	while (*dest++ = *src++)//代码的优化 最后结束为\0自动循环结束
	{
		;
	}
	return ret;
}

int main()
{
	char arr1[10] = { "xxxxxxxxxx" };
	char arr2[] = "hello bit";

	printf("%s", my_strcpy(arr1, arr2));

	return 0;
}