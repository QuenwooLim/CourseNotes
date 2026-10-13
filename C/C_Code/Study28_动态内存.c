#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


/*
栈区申请空间：局部变量 形式参数
堆区申请空间：动态内存管理函数：malloc  calloc  realloc  free
*/

int main()
{
	//空间开辟完成之后无法修改
	int a = 10;
	int arr[10];

	return 0;
}

//malloc
int main()
{
	//动态内存开辟
	//如果开辟成功 返回开辟好空间的指针；如果开辟失败 返回一个NUll指针，因此要检查malloc的返回值
	int* p = (int*)malloc(40);  //void* malloc(size_t) 因此要强制类型转换
	if (p == NULL)//判断内存开辟是否成功
	{
		printf("%s\n", strerror(errno));
		return 1;
	}
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		*(p + i) = i;
	}
	for (i = 0; i < 10; i++)
	{
		printf("%d ", *(p + i));
	}

	//内存释放 不释放会导致内存泄露
	free(p);
	p = NULL;

	return 0;
}

//calloc
int main()
{
	int* p = calloc(10, sizeof(int));
	if (p == NULL)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		printf("%d ", *(p + i));//calloc将变量自动初始化为0 malloc不进行初始化
	}

	return 0;
}


//realloc
int main()
{
	int* p = (int*)malloc(40);
	if (p == NULL)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		*(p + i) = i;
	}

	//realloc扩容 +40个字节
	//方式1：在原地址后接上40字节空间 方式2：重新开辟一块80字节空间(后面空间不足时)
	int* ptr = (int*)realloc(p, 80);//realloc(首地址，新容量)
	if (ptr == NULL)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}
	else if (ptr != NULL)
	{
		p = ptr;
	}
	for (i = 0; i < 10; i++)
	{
		printf("%d ", *(p + i));//原来的0 1 2 3 4 5 6 7 8 9不变
	}
	free(p);
	p = NULL;
	return 0;
}


//常见错误
//1.对NULL指针的解引用操作
int main()
{
	int* p = (int*)malloc(40);
	//第一步判断是否为野指针
	if (p == NULL)
	{
		return 1;
	}
	//第二步再进行赋值
	*p = 20;//不能直接进行这一步
	//第三步释放内存
	free(p);
	p = NULL;

	return 0;
}

//2.对动态内存空间的越界访问
int main()
{
	int* p = (int*)malloc(40);
	if (p == NULL)
	{
		printf("%s\n", sterror(errno));
		return 1;
	}
	int i = 0;
	for (i = 0; i <= 10; i++)
	{
		p[i] = i;//i=10时候越界访问
	}
	free(p);
	p = NULL;

	return 0;
}

//3.对非动态开辟内存空间使用free释放
int main()
{
	int a = 10;
	int* p = &a;


	free(p);//p此时不是动态内存,是栈区内存  不能释放!!
	p = NULL;
	return 0;
}

//4.使用free释放一块动态开辟内存的一部分
int main()
{
	int* p = (int*)malloc(40);
	if (p == NULL)
	{
		return 1;
	}
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		*p = i;
		p++;//这一步
	}

	free(p);//导致p已经不再是原来的地址了 此时只能释放一部分;内存释放只能用于原来开辟的地址
	p = NULL;

	return 0;
}

//5.对同一块动态内存多次释放
int main()
{
	int* p = (int*)malloc(40);

	free(p);
	free(p);//两次释放 系统懵逼; 因此习惯在free后p=NULL,free(NULL)为无影响的操作

	return 0;
}


//6.动态内存忘记释放（内存泄露）
void test()
{
	int* p = (int*)malloc(100);

	int a;
	if (a == 1)
		return;//一旦条件执行 则p无法释放了 内存泄露（白白占用空间）

	free(p);
	p = NULL;
}
int main()
{
	test();
	return 0;
}

void getmemory(char** p)
{
	*p = (char*)malloc(100);
}

int main()
{
	char* str = NULL;
	getmemory(&str);
	//传址调用 传一级指针要用二级指针;不然直接传str将无法在getmemory函数出去后开辟str内存(出了函数布局变量销毁)

	strcpy(str, "hello world");
	printf(str);//打印可以直接打印字符串和数组

	free(str);//“你开辟 我释放”
	str = NULL;

	return 0;
}