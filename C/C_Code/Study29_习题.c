#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<errno.h>

int* teat()
{
	int a = 10;
	return &a;
}
int main()
{
	int* p = test();
	printf("%d", *p);//p内存销毁了 但是地址还没被覆盖 10

	printf("Hehe");//将原来p的地址覆盖了
	printf("%d", *p);//被覆盖 随机值
	return 0;
}



//内存

//数据段（全局数据，静态数据） 即是静态区
int globalVar = 1;
static int staticGlobalVar = 1;
void Test()
{
	//数据段
	static int staticVar = 1;

	//左边为栈（局部变量）(向下增长
	int localVar = 1;
	int num1[10] = { 1,2,3,4 };
	//右边为代码段（可执行代码/只读常量)  
	char char2[] = "abcd";
	char* pChar3 = "abcd";
	//右边为堆（向上增长
	int* ptr1 = (int*)malloc(sizeof(int) * 4);
	int* ptr2 = (int*)calloc(4, sizeof(int));
	int* ptr3 = (int*)realloc(ptr2, sizeof(int) * 4);

	free(ptr1);
	free(ptr3);

}

/*
柔性数组flexible array
c99中结构的最后一个成员允许是未知大小的数组，这就叫做柔性数组成员
特点：
1.柔性数组成员之前必须至少有一个成员
2.sizeof计算不包含柔性数组成员
3.需要用malloc函数开辟空间 并且开辟的空间要大于结构体的大小来满足需要
*/


//方案一 优点：方便内存释放；有利于提高访问速度
struct S
{
	int i;
	int arr[];//柔性数组成员
};
int main()
{
	int sz = sizeof(struct S);
	printf("%d\n", sz);//4 不包含柔性数组成员的大小

	//柔性数组的使用
	struct S* ps = (struct S*)malloc(sizeof(struct S) + 40);//增加的空间来满足数组
	if (ps == NULL)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}
	ps->i = 100;
	int n = 0;
	for (n = 0; n < 10; n++)
	{
		ps->arr[n] = n;
	}

	struct s* ptr = (struct S*)realloc(ps, sizeof(struct S) + 80);
	if (ptr != NULL)
	{
		ps = ptr;
		ptr = NULL;//ptr此时还不可以free 不然指向的空间就销毁了
	}

	free(ps);
	ps = NULL;

	return 0;
}


//方案二
struct S
{
	int i;
	int* arr;//地址  非柔性数组成员
};
int main()
{
	//开辟
	struct S* ps = (struct S*)malloc(sizeof(struct S));
	if (ps = NULL)
	{
		return 1;
	}
	ps->i = 100;
	//第二次开辟
	ps->arr = (int*)malloc(40);
	if (ps->arr = NULL)
	{
		return 1;
	}

	//使用
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		ps->arr[i] = i;
	}

	//扩容
	int* ptr = (int*)realloc(ps->arr, 80);
	if (ptr == NULL)
	{
		return 1;
	}

	//释放
	free(ps->arr);//两次开辟 先释放结构内的成员开辟的
	free(ps);//再释放结构体
	ps = NULL;

	return 0;
}


////offsetof的实现 
//struct S
//{
//	char c1;
//	int i;
//	char c2;
//};
//
//#define OFFSETOF(type,MEM) (size_t)&(((type*)0)->MEM)
//// (type*)0也就是(struct S*)0 将结构体的地址(也是起始地址)强制类型转化为0
////-> 通过地址找到成员; &找到成员的地址（起始地址是0） 成员的地址数值就是偏移量
//// 偏移量是正数 强制类型转化为size_t
//int main()
//{
//	struct S s = { 0 };
//	printf("%d\n", OFFSETOF(struct S, c1));
//	return 0;
//}

////打印箭形图案
int main()
{
	int n = 0;
	scanf("%d", &n);
	int i = 0;
	int j = 0;
	int k = 0;
	//打印上半部分
	for (i = 0; i < n; i++)
	{
		for (j = n; j > i; j--)
		{
			printf("  ");
		}
		if (j == i)
		{
			for (k = 0; k < i + 1; k++)
			{
				printf("*");
			}
		}
		printf("\n");
	}
	//打印中轴线
	for (i = 0; i < n + 1; i++)
		printf("*");
	printf("\n");
	//打印下半部分
	for (i = n; i > 0; i--)
	{
		for (j = n; j > i - 1; j--)
		{
			printf("  ");
		}
		if (j == i - 1)
		{
			for (k = 0; k < i; k++)
			{
				printf("*");
			}
		}
		printf("\n");
	}
	return 0;
}

int main()
{
	unsigned char puc[4];
	struct tagPIM
	{
		unsigned char ucPim1;
		unsigned char ucData0 : 1;
		unsigned char ucData1 : 2;
		unsigned char ucData2 : 3;
	}*pstPimData;
	pstPimData = (struct tagPIM*)puc;
	//让结构体变量地址指向数组地址 (数组中存储的就是结构体)

	memset(puc, 0, 4);//数组初始化

	pstPimData->ucPim1 = 2;//00000010
	pstPimData->ucData0 = 3;//3的二进制表示是 011 但是位段只有一个比特位 所以只存了1
	pstPimData->ucData1 = 4;//4的二进制 100 两个比特位 存00
	pstPimData->ucData2 = 5;//5的二进制 101 三个比特位 存101
	//此时数组内 00000010 00101001 00000000 00000000 即02 29 00 00

	printf("%02x %02x %02x %02x\n", puc[0], puc[1], puc[2], puc[3]);
	//%02x打印16进制的两位 不够拿0填充

	return 0;
}


int main()
{
	union
	{
		short k;
		char i[2];
	}*s, a;
	s = &a;
	s->i[0] = 0x39;//低位
	s->i[1] = 0x38;//高位
	//在小端环境下 低地址放低位的数据 高地址放高位的数据
	//拿出数据时 由高到低是 0x   38 39

	printf("%x\n", a.k);

	return 0;
}


/*
找单身狗：一个数组中只有两个数字只出现了一次 其他都出现了两次
eg{1，2，3，4，5，1，2，3，4，6}
1.将所有数字异或 得到5^6
2.找出5^6不同的二进制位5：101  6：110
3.将数组按找的二进制位分成两组 5 1 1 3 3 ; 6 2 2 3 3
4.分别将两个组内数字异或一次 得到5 和 6
*/
find(int arr[], int sz, int* dog1, int* dog2)
{
	int ret = 0;
	int i = 0;
	//1.异或
	for (i = 0; i < sz; i++)
	{
		ret ^= arr[i];
	}
	//2.找出不同的二进制位
	int pos = 0;
	for (pos = 0; pos < 32; pos++)
	{
		if (((ret >> pos) & 1) == 1)
		{
			break;
		}
	}
	for (i = 0; i < sz; i++)
	{
		if (((arr[i] >> pos)) & 1 == 1)
		{
			*dog1 ^= arr[i];
		}
		else

		{
			*dog2 ^= arr[i];
		}
	}

}
int main()
{
	int arr[] = { 1,2,3,4,5,1,2,3,4,6 };
	int sz = sizeof(arr) / sizeof(arr[0]);
	int dog1 = 0;
	int dog2 = 0;
	find(arr, sz, &dog1, &dog2);
	printf("%d %d", dog1, dog2);

	return 0;
}

//模拟实现atoi函数
#include<assert.h>
#include<ctype.h>
#include<limits.h>
enum Status
{
	INVALID,
	VALID
}sta = INVALID;//默认初始字符串为非法
int my_atoi(const char* str)
{
	int flag = 1;
	assert(str);
	if (*str == '\0')//判断是空内容的字符串 而不是内容为0的字符串
		return 0;
	while (isspace(*str))//一直读取到\0
	{
		str++;
	}
	//判断正负号
	if (*str == '+')
	{
		flag = 1;
		str++;
	}
	else if (*str == '-')
	{
		flag = -1;
		str++;
	}
	long long ret = 0;
	while (*str)
	{
		if (isdigit(*str))
		{
			ret = ret * 10 + flag * (*str - '0');//字符数减去'0'得到整型
			//越界
			if (ret > INT_MAX || ret < INT_MIN)
			{
				return 0;
			}
		}
		else
		{
			return (int)ret;//将long long类型的int强制类型转换 符合函数类型
		}
		str++;
	}
	if (*str == '\0')//保证读取到末尾 使得字符串为合法
	{
		sta = VALID;
	}
	return (int)ret;
}
int main()
{
	char arr[200] = "123abc456";
	int ret = my_atoi(arr);
	if (sta == INVALID)
	{
		printf("非法返回:%d\n", ret);
	}
	else if (sta == VALID)
	{
		printf("合法转换:%d\n", ret);
	}
	return 0;
}


/*
1.fgetc适用   所有输入流  字符输入函数
2.getchar适用 键盘输入流  字符输入函数
3.fputs适用   所有输出流  文本行输出函数
4.fread适用   文件输入流  二进制输入函数
*/


/*
#define INT_PTR int*
INT_PTR a,b;  相当于int* a,b   也相当于int *a,b   a是指针 b不是指针
typedef int* int_ptr;
int_ptr c,d   int_ptr相当于一个新的类型int*  此时c,d都是指针
*/


/*
写一个宏 交换一个数二进制位的奇数和偶数位
0x55555555: 01010101 01010101 01010101 01010101
n:          10100100 10110110 01010001 01010111
0xaaaaaaaa: 10101010 10101010 10101010 10101010
按位与之后可以分别保留 n的二进制位的奇数项和偶数项

*/
#define SWAP(n) n=(((n&0x55555555)<<1)+((n&0xaaaaaaaa)>>1))
int main()
{
	int n = 0;
	scanf("%d", &n);
	SWAP(n);
	printf("%d\n", n);
	return 0;
}

int main()
{
	int a = -3;
	unsigned int b = 2;
	long c = a + b;
	//a+b 有符号数+无符号数 结果转化为无符号数
	//但是c是有符号类型 所以a+b结果存储到c中变为有符号
	printf("%lf", c);

	return 0;
}


//斐波那契数定位
int main()
{
	int n = 0;
	int a = 0;
	int b = 1;
	scanf("%d", &a);
	while (1)
	{
		if (n == b)
		{
			printf("0");
			break;
		}
		else if (n < b)
		{
			int d = ((n - a) > (b - n)) ? (b - n) : (n - a);
			printf("%d", d);
			break;

		}
		int c = a + b;
		a = b;
		b = c;
	}

	return 0;
}

/*
将字符串中出现的空格转换为%20
eg：we are happy. 转化为we%20are%20happy.
*/
/*
接口形题目 只写一个函数
*/
void replaceSpace(char* str, int length)
{
	char* cur = str;
	int space_count = 0;
	while (*cur)
	{
		if (*cur == ' ')
		{
			space_count++;
		}
		cur++;
	}
	int end1 = length - 1;
	int end2 = length + space_count * 2 - 1;
	while (end1 != end2)
	{
		if (str[end1] != ' ')
		{
			str[end2--] = str[end1--];
		}
		else
		{
			str[end2--] = '0';
			str[end2--] = '2';
			str[end2--] = '%';
			end1--;
		}
	}
}