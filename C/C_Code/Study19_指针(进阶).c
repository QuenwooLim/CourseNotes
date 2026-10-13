#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

//指针
int main()
{
	char ch = 'w';
	char* pc = &ch;
	*pc = 'b';
	printf("%c\n", ch);

	const char* p = "abcdef";//吧字符串首字符a地址赋给了p
	//字符串无法被修改 加上const使语法更完善
	//*p='w' 无法操作 abcdef此时是固定常量

	printf("%s\n", p);//可以打印 

	return 0;
}

//关于const
int main()
{
	//变量
	const int a = 10;
	//a = 20;无法修改常变量
	int* pa = &a;
	*pa = 20;//可以利用指针来修改常变量的值

	int b = 20;
	const int* pb = &b;//const放在*左边 const修饰指针
	//*pb = 0;  //不可行 pb指向的对象不能再使用pb来改变了 这时无法修改b的指针；

	int n = 100;
	pb = &n;//可行  这时候pb本身变量是可以修改的 可以利用指针来改变pb 从而可以修改b的值

	//const放在*右边
	int* const p = &a;
	*p = 0;//可以修改   p指向的对象是可以通过p来改变的
	//p=&n; 不可行    p变量本身的值是不能被修改的

	//数组
	double* const x;//等价	double x[const];
	//不能修改地址

	const double* y;//等价 	const double y[];
	//不能修改地址指向的内容

	return 0;
}

int main()
{
	char* p1 = "abcdef";//p1==p2 此时内容相同 且字符串是固定不能修改的常量 共享一份存储空间
	char* p2 = "abcdef";

	char arr1[] = { "abcdef" };//arr1!=arr2  内容相同 但是数组可以修改 因此存了两份地址
	char arr2[] = { "abcdef" };

	return 0;
}

int main()
{
	int* p1[10];//指针数组 p1先和[]结合 是个元素都是指针的一个数组
	int(*p2)[10];//数组指针 p2可以指向一个数组，该数组有十个元素

	return 0;
}

int main()
{
	int arr[10];
	printf("%p", arr);
	printf("%p", &arr);
	printf("%p", &arr[0]);//三者都相等
	/*
	数组名一般表示首元素的地址
	特例 1.sizeof（数组名） 计算整个数组的大小
		 2.&数组名，这里的数组名表示的依然是整个数组，&数组名表示整个数组的地址
	*/

	//数组指针是用来存放数组的地址
	int(*p2)[10] = &arr;//[]内数字不能省略

	return 0;
}


void print(int(*p)[5], int r, int c)//二维数组接收 地址p接收arr
{
	int i = 0;
	for (i = 0; i < r; i++)
	{
		int j = 0;
		for (j = 0; j < c; j++)
		{
			printf("%d ", *(*(p + i) + j));
		}
		printf("\n");
	}
}
int main()
{
	int arr[3][5] = { 1,2,3,4,5,2,3,4,5,6,3,4,5,6,7 };
	print(arr, 3, 5);//二维数组传参

	return 0;
}


int main()
{
	int arr[5];//arr是整型数组
	int* parr1[10];//parr1是指针数组
	int(*parr2)[10];//parr2是数组指针
	int(*parr3[10])[5];//parr3是存放数组(5个元素)指针的数组(10个元素)

	return 0;
}

//一维数组传参
void test1(int arr[])//传数组 数组接收
{}
void test1(int arr[10])//传数组 数组接收
{}
void test1(int* arr)//首元素地址
{}

void test2(int* arr[20])//传数组 数组接收
{}
void test2(int** arr)//二级指针 存储一级指针变量
{}

int main()
{
	int arr[10] = { 0 };
	int* arr2[20] = { 0 };
	test1(arr);
	test2(arr2);

	return 0;
}

//二维数组传参
void test(int arr[3][5])
{}//可行 传数组 数组接受
void test(int arr[][])
{}  //不可行 行可以省略 列不可以省略
void test(int(*arr)[5])
{}//可行 存放数组的指针
void test(int** arr)
{}  //不可行 二级指针只能用于存放一级指针
void test(int* arr)
{}//不可行 不能存放
void test(int arr[5])
{}//可行 可以存放二维数组的第一行五个元素的数组
void test(int** arr)
{}//不可行
int main()
{
	int arr[3][5] = { 0 };
	test(arr);//二维数组首元素地址 实际上是第一行五个元素的数组

	return 0;
}

//一级指针传参
void print(int* p, int sz)
{
	int i = 0;
	for (i = 0; i < sz; i++)
	{
		printf("%d\n", *(p + i));
	}
}

int main()
{
	int arr[10] = { 1,2,3,4,5,6,7,8,9 };
	int* p = arr;
	int sz = sizeof(arr) / sizeof(arr[0]);

	print(p, sz);

	return 0;
}

test(int** p)//二级指针传参
{}
int main()
{
	int n = 10;
	int* p1;
	test(&p1);

	int** p2;
	test(p2);

	int* arr[10];
	test(arr);

	return 0;
}

int add(int x, int y)
{}
//函数指针
int main()
{
	int a = 1;
	int b = 2;
	int c = add(a, b);

	printf("%p", &add);
	printf("%p", add);//&add和add都是函数的地址

	//pf就是函数指针
	int (*pf)(int, int) = &add;

	int ret = (*pf)(2, 3);//通过指针调用找到函数
	// int ret =pf(2,3)  *可以省略
	printf("%d\n", ret);

	return 0;
}


//函数指针应用
int Add(int x, int y)
{
	return x + y;
}
void calc(int (*pf)(int, int))
{
	int a = 3;
	int b = 5;
	int c = pf(a, b);
	printf("%d", c);
}
int main()
{
	calc(Add);

	return 0;
}


typedef void(*pf_t)(int);
//把void(*)(int)类型重新命名为pf_t
int main()
{
	( *(void(*) () )0 )();
	/*
	以上是一次函数的调用
	0.void(*)()是函数指针类型
	1.把0强制类型转换为：无参，返回类型是void的函数地址
	2.调用0地址处的这个函数
	*/

	void (*signal(int, void(*)(int)))(int);
    //重新定义之后可以写成  
	pf_t signal(int, pf_t);

	return 0;
}
void menu()
{
	printf("*****************\n");
	printf("***1.add*2.sub***\n");
	printf("***3.mul*4.div***\n");
	printf("*****0.exit******\n");
}
int add(int x,int y)
{
	return x + y;
}
int sub(int x, int y)
{
	return x - y;
}
int mul(int x, int y)
{
	return x * y;
}
int div(int x, int y)
{
	return x / y;
}
void calc(int (*pf)(int, int))//函数指针的应用
{
	int x = 0;
	int y = 0;
	printf("请输入两个整型数字:");
	scanf("%d %d", &x, &y);
	int ret = (*pf)(x, y);
	printf("%d\n", ret);
}
int main()
{
	int a = 0;
	do
	{
		menu();
		printf("请输入你的选择:");
		scanf("%d", &a);
		switch (a)
		{
		case 1:
			calc(add);
			break;
		case 2:
			calc(sub);
			break;
		case 3:
			calc(mul);
			break;
		case 4:
			calc(div);
			break;
		default:
			printf("输入错误，请重新输入\n");
		}
	} while (a);
	printf("退出计算器");
	return 0;
}

int main()
{
	int (*pf)(int, int) = add;

	//实现跳转功能 也称转移表
	int (*arr[])(int, int) = { 0, add,sub,mul,div };
	//arr是数组 []
	//数组类型 int (*)(int,int) 函数指针
	//arr是函数指针类型的数组
	int i = 0;
	do
	{
		menu();
		printf("输入你的选择：");
		scanf("%d", &i);
		if (i == 0)
		{
			printf("退出计计算器");
		}
		else if (i >= 1 && i <= 4)
		{
			printf("请输入两个操作数\n");
			int a = 0;
			int b = 0;
			scanf("%d %d", &a, &b);
			int ret = arr[i](a, b);
			printf("%d\n", ret);
		}
		else
		{
			printf("输入错误，请重新输入\n");
		}
	} while (i);

	return 0;
}

void Add()
{}
void Sub()
{}
void Mul()
{}
void Div()
{}

int main()
{
	//函数指针数组
	int (*pfArr[])(int, int) = { 0,Add,Sub,Mul,Div };

	//指向（函数指针数组）的指针
	int (*(*ppfArr)[5])(int, int) = &pfArr;
	return 0;
}

//把数组排成升序
/*
qsort函数(默认排升序)
void qsort(void* base,//要排序的数据的起始位置
		   size_t num,//待排序的数据元素的个数
		   size_t width,//待排序的数据元素的大小（单位是字节)
		   int(* cmp)(const void* e1,const void* e2)//函数指针-比较函数
*/
#include<stdlib.h>//qsort头文件
int cmp_int(const void* e1, const void* e2)
{
	//指向对象e1>e2，返回值>0
	//指向对象e1=e2，返回值=0
	//指向对象e1<e2，返回值<0
	return (*((int*)e1) - *((int*)e2));
}
int main()
{
	int arr[] = { 9,8,7,6,5,4,3,2,1,0 };

	int sz = sizeof(arr) / sizeof(arr[0]);

	qsort(arr, sz, sizeof(arr[0]), cmp_int);

	return 0;
}
/*
void* 是无具体类型的指针，可以接受任意类型的地址
int a=10;
void* pv=&a;

void* 是无具体类型指针，所以不能进行解引用操作，也不能+-整数
*/
#include <stdlib.h>
struct stu
{
	char name[20];
	int age;
};

int cmp_stu_by_age(const void* e1, const void* e2)
{
	return ((struct stu*)e1)->age - ((struct stu*)e2)->age;
}

void test()
{
	struct stu s[] = { {"zhangsan",15},{"lisi",30},{"wangwu",25} };
	int sz = sizeof(s) / sizeof(s[0]);
	qsort(s, sz, sizeof(s[0]), cmp_stu_by_age);

}
int main()
{
	test();
	return 0;
}



//自己写一个程序模拟qsort
#include<stdlib.h>
void swap(char* buf1, char* buf2, int width)
{
	int i = 0;
	for (i = 0; i < width; i++)
	{
		char tmp = *buf1;
		*buf1 = *buf2;
		*buf2 = tmp;
		buf1++;
		buf2++;
	}
}
int cmp_int(const void* e1, const void* e2)
{
	return (*(int*)e1 - *(int*)e2);
}

void bubble_sort(void* base, int sz, int width, int(*cmp)(const void* e1, const void* e2))
{
	int i = 0;
	for (i = 0; i < sz - 1; i++)
	{
		int flag = 1;//表示数组已经排好序

		int j = 0;
		for (i = 0; j < sz - 1; j++)
		{
			if (cmp((char*)base + j * width, (char*)base + (j + 1) * width) > 0)//base是void类型,转换成char比较精细，再进行计算
			{
				swap((char*)base + j * width, (char*)base + (j + 1) * width, width);
				flag = 0;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
}
void test()
{
	int arr[] = { 9,8,7,6,5,4,3,2,1,0 };
	//升序排列
	int sz = sizeof(arr) / sizeof(arr[0]);

	bubble_sort(arr, sz, sizeof(arr[0]), cmp_int);

	int i = 0;
	for (i = 0; i < sz; i++)
	{
		printf("%d", arr[i]);
	}
}
int main()
{
	test();

	return 0;
}