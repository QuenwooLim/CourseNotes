#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
//strcpy strcat stramp

//字符
int main()
{
	char c0 = 'A';//'字符'
	char c1 = 65;//ASCII
	char c2 = '\n';//转义字符
	char c3 = '\x41';//用16进制表示字符
	char c4 = '\101';//用8进制表示字符

	return 0;
}
/*
ASCII表基本特征
控制符：前32个+第127个
可打印字符：1.32是空格
			2.48-57是'0'-'9'
			3.65-90是'A'-'Z'
			4.97-122是'a'-'z'
空白字符：水平制表\t 换行\n ...

中文字符：UTF-8编码：一个汉字占3个char
		  ANSI(即GBK)编码：一个汉字占2个字节
*/

/*
字符串
在C语言中本质是字符数组（系统自动加\0）

字符串abcd
字符串的前缀 eg:a, abc ...
		后缀 eg:d,cd...
*/
/*
字符串
输入 scanf gets
输出printf puts
求长度strlen
连接strcat(tostr,fromstr) strncat(tostr,fromstr,n)
拷贝strcpy(tostr,fromstr)
比较strcmp(str1,str2) strncmp(str1,str2,n)
转换为小写strlwr(str)
转换为大写strupr(str)
字符检索(str,c)
字符串检索(str1,str2)
*/

/*
字符串输入输出
gets()可以获得一整行 包括空格
puts()可以输出一整行
*/
int main()
{
	char str[10];
	puts("Enter a string:\n");
	gets(str);
	puts("The string you enter is:\n");
	puts(str);
	return 0;
}

/*
* strlen与sizeof
strlen 求字符串长度 关注\0之前
strlen 是库函数 只针对字符串

sizeof只关注占用内存空间大小 不关注内容
sizeof是操作符
*/

int main()
{
	char arr[] = { 'a','b','c','d','e','f' };
	printf("%d\n", sizeof(arr));//数组名 6
	printf("%d\n", sizeof(arr + 0));//arr+0是首元素的地址 4/8
	printf("%d\n", sizeof(*arr));//首元素
	printf("%d\n", sizeof(&arr));//arr是数组 &arr是数组地址
	printf("%d\n", sizeof(&arr + 1));//数组后的地址
	printf("%d\n", sizeof(&arr[0] + 1));//第二个元素的地址

	return 0;
}

#include<string.h>
int main()
{
	char arr[] = { 'a','b','c','d','e','f' };//\0不定
	//char arr[]="abcdef";//后面自带\0

	printf("%d\n", strlen(arr));//随机值 从arr首元素地址直到后面寻找的\0
	printf("%d\n", strlen(arr + 0));//还是首元素地址
	printf("%d\n", strlen(*arr));//->strlen('a')->strlen(97) 此时97是野指针 非法访问
	printf("%d\n", strlen(&arr));// arr 和 &arr传给strlen后相同
	printf("%d\n", strlen(&arr + 1));//随机值-6
	printf("%d\n", strlen(&arr[0] + 1));//随机值-1

	return 0;
}

int main()
{
	char arr[] = "abcdef";
	// [a b c d e f \0]
	printf("%d\n", sizeof(arr));//7
	printf("%d\n", sizeof(arr + 0));//首元素地址
	printf("%d\n", sizeof(*arr));//首元素
	printf("%d\n", sizeof(arr[1]));//1
	printf("%d\n", sizeof(&arr));// 4或者8
	printf("%d\n", sizeof(&arr + 1));// 4或者8
	printf("%d\n", sizeof(&arr[0] + 1));//4或者8

	return 0;
}

int main()
{
	char arr[] = "abcdef";
	printf("%d\n", strlen(arr));//首元地址开始的字符串 6
	printf("%d\n", strlen(arr + 0));//首元素地址 向后数 6
	printf("%d\n", strlen(*arr));//表示第一个元素 错误操作error
	printf("%d\n", strlen(arr[1]));//error
	printf("%d\n", strlen(&arr));//arr表示数组 &arr取出数组的地址 同首元素地址
	printf("%d\n", strlen(&arr + 1));//随机值 +1跳过"abcdef\0"
	printf("%d\n", strlen(&arr[0] + 1));//第二个元素

	return 0;
}


int main()
{
	char* p = "abcdef";//首字符a的地址传给了p

	printf("%d\n", sizeof(p));// 4/8
	printf("%d\n", sizeof(p + 1));// 4/8
	printf("%d\n", sizeof(*p));//1
	printf("%d\n", sizeof(p[0]));//首元素 1
	printf("%d\n", sizeof(&p));//二级指针 4/8
	printf("%d\n", sizeof(&p + 1));// 二级指针 4/8
	printf("%d\n", sizeof(&p[0] + 1));//指针 4/8

	//strlen(字符串的起始地址)
	printf("%d\n", strlen(p)); //6
	printf("%d\n", strlen(p + 1));// 6-1=5
	printf("%d\n", strlen(*p));// *p就是‘a’ 错误操作
	printf("%d\n", strlen(&p));//随机值
	printf("%d\n", strlen(&p + 1));//随机值 与上一个无关系
	printf("%d\n", strlen(&p[0] + 1));//5

	return 0;
}

int main()
{
	char arr1[] = "abcdef";
	char arr2[] = { 'a','b','c','d','e','f' };//不知道后面什么时候会出现\0
	printf("%d\n", strlen(arr1));//6
	printf("%d\n", strlen(arr2));//随机值

	return 0;
}

/*
字符串相关常见函数(要求掌握)
ctype.h:字符分类:isspace
		字符操作

stdlib.h:转换成数值

string.h:字符串操作 strcpy strncpy strcat strncat
		 字符串检验 strlen strcmp strchr strstr
		 字符数组操作 memset memcpy
		 杂项 strerror

空终止字节字符串(NTBS)是尾随零值字节的非零字节序列
*/

#include <assert.h>//assert断言
//模拟实现strlen
size_t my_strlen(char* str)
{
	assert(str);//指针为空时报警 保证指针有效
	size_t count = 0;
	while (*str != 0)
	{
		count++;
		str++;
	}
	return count;
}
int main()
{
	char arr[] = "abcdef";
	size_t n = my_strlen(arr);
	printf("%u\n", n);
	return 0;
}


/*
strcpy使用要求
1.原字符串必须存在\0 (字符串默认结尾\0)
2.拷贝的内存空间必须够大
3.内存必须可以修改
*/
int main()
{
	char name[20] = { 0 };

	strcpy(name, "zhang\0san");
	//拷贝遇到\0时就停止 \0之前的字符都可以拷贝

	char arr[] = { 'b','i','t' };
	//	strcpy(name, arr);//无法进行 报错 arr数组字符往后知道什么时候再出现\0

	char a[3] = "";
	char b[] = "abcdef";
	//	strcpy(a, b);//无法进行 移除 a放不下了

	const char* p = "abcdef";//加const防止报警告 p指针所指内容不可修改
	char arr1[] = "bit";
	//	strcpy(p, arr1);//无法进行 p中内容已经无法修改
	printf("%s\n", name);
	return 0;
}

//模拟实现strcpy
#include <assert.h>
char* my_strcpy(char* str2, char* str1)//类型是目的地的起始地址
{
	assert(str1);
	assert(str2);
	char* ret = str2;//暂时存储目的地的起始地址
	while (*str2++ = *str1++)//优化
	{
		;
	}
	return ret;//返回目的地的起始地址
}
int main()
{
	char arr1[] = "abcdef";
	char arr2[20] = { 0 };
	my_strcpy(arr2, arr1);

	printf("%s", arr2);

	return 0;
}

/*
strcat字符串追加
1.追加遇到\0时才停止
2.被追加的内存足够大
3.被追加的内存必须可以修改(即被追加的只能是数组)
*/
int main()
{
	char arr1[20] = "hello";
	strcat(arr1, "world"); 
	printf("%s", arr1);
	return 0;
}

//模拟实现strcat
void my_strcat(char* str1, const char* str2)
{
	while (*str1 != 0)
	{
		str1++;
	}
	while (*str1++ = *str2++)
	{
		;
	}
}
int main()
{
	char arr1[20] = "hello";
	my_strcat(arr1, "world");
	printf("%s", arr1);

	return 0;
}

//strcmp比较 常用于按字符串排序(排序与长度无关)
int main()
{
	char arr1[20] = "zhangsanfeng";
	char arr2[20] = "zhangsan";
	//比较两个数组是否相同 
	//因为是两份地址 所以不能单纯地使用= 
	//需要使用strcmp
	int ret = strcmp(arr1, arr2);
	if (ret < 0)
		printf("<\n");
	else if (ret = 0)
		printf("=\n");
	else
		printf(">\n");

	return 0;
}

//模拟实现strcmp
#include <assert.h>
int my_strcmp(const char* str1, const char* str2)
{
	assert(str1 && str2);
	while (*str1 == *str2)
	{
		if (*str1 == 0)
		{
			return 0;
		}
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}
int main()
{
	char arr1[] = "abcdef";
	char arr2[] = "abcdek";
	int ret = my_strcmp(arr1, arr2);

	if (ret > 0)
		printf(">");
	else if (ret = 0)
		printf("=");
	else
		printf("<");

	return 0;
}

/*
长度受限制的字符串函数
strncpy
strncat
strncmp
*/
//strncpy
int main()
{
	char arr1[20] = "abcdef";
	char arr2[] = "hello bit";
	strncpy(arr1, arr2, 5);//5表示拷贝字符为5个
	strncpy(arr1, arr2, 10);//多的未知自动补充\0
	printf("%s\n", arr1);

	return 0;
}

//strncat
int main()
{
	char arr1[20] = "hello\0xxxxx";
	char arr2[] = "bit";
	strncat(arr1, arr2, 6);

	return 0;
}

//strncmp
int main()
{
	char arr1[] = "abcdef";
	char arr2[] = "abc";
	int ret = strncmp(arr2, arr2, 3);//3表示比较3个字符

	if (ret > 0)
		printf(">");
	else if (ret == 0)
		printf("=");
	else
		printf("<");

	return 0;
}

//strstr寻找子串的函数
int main()
{
	char email[] = "hello world!";
	char substr[] = "world";
	char* ret = strstr(email, substr);//返回类型是char*
	if (ret == NULL)
		printf("子串不存在");
	else
		printf("%s", ret);//从字串开始一直向后打印
	return 0;
}

//模拟实现strstr
#include <assert.h>
char* my_strstr(const char* str1, const char* str2)
//const防止报警告 指针所指内容字符串不可修改 但是指针可以修改
{
	assert(str1 && str2);
	const char* s1 = str1;
	const char* s2 = str2;
	const char* p = str1;
	while (*p)
	{
		s1 = p;
		s2 = str2;
		while (*s1 != 0 && *s2 != 0 && *s1 == *s2)
		{
			s1++;
			s2++;
		}
		if (*s2 == 0)
		{
			return (char*)p;//返回值类型是char*
		}
		p++;
	}
	return NULL;
}
int main()
{
	char arr1[] = "abbbcdef";
	char arr2[] = "bbc";
	char* ret = my_strstr(arr1, arr2);//返回类型是char*
	if (ret == NULL)
		printf("子串不存在");
	else
		printf("%s", ret);//从字串开始一直向后打印
	return 0;
}

/*
strtok切割字符串
函数的第一个参数不为NULL,函数将找到str中第一个标记,并且保存标记的位置
函数的第一个参数为NULL,函数将在同一个字符串中被保存的位置开始，直到找到下一个标记
*/
int main()
{
	const char sep[] = { "@." };
	char email[] = "1289421662@qq.com";
	char cp[30] = { 0 };//strtok会修改原来字符串的内容 因此需要引入其他变量
	strcpy(cp, email);
	//char* ret = strtok(cp, sep);
	//printf("%s\n", ret);

	//ret = strtok(NULL, sep);//strtok有记忆功能 会保存原来读取继续下去
	//printf("%s\n", ret);

	//ret = strtok(NULL, sep);
	//printf("%s\n", ret);

	char* ret = NULL;
	for (ret = strtok(cp, sep); ret != NULL; ret = strtok(NULL, sep))
		printf("%s\n", ret);

	return 0;
}

//strerror c语言的库函数，在执行失败的时候，都会设置错误码eg：0 1 2..
#include <errno.h>//errno c语言设置的一个全局的错误码存放的变量
int main()
{
	FILE* pf = fopen("test.txt", "r");//打开文件夹的函"文件名""r(以读取的形式打开)"
	//打开失败 把错误码存放到errno
	if (pf == NULL)//打开失败
	{
		printf("%s\n", strerror(errno));//失败的原因
	}
	return 0;
}


//字符分类函数
#include <ctype.h>
int main()
{
	//int a = isspace('\f');
	////非空白字符0  空白字符有对应返回值
	//printf("%d\n", a);

	int a = isdigit('x');
	printf("%d\n", a);
	//isdigit判断是不是数字字符 是数字字符返回非0 不是数字字符返回0
	return 0;
}

//字符转换函数
int main()
{
	printf("%c\n", tolower('C'));//放入大写字母 输出小写字母
	printf("%c\n", toupper('c'));//放入小写字母 输出大写字母
	return 0;
}

//memcpy 内存拷贝  与拷贝字符串的strcpy不同
int main()
{
	int arr1[] = { 1,2,3,4,5,6 };
	int arr2[10] = { 0 };
	memcpy(arr2, arr1, 6);//memcpy arr2 arr1在函数中类型都是void*,数字6类型是size_t(字节) 适用各种类型
	//memcpy(arr1+2,arr1,3);memcpy无法进行内部重叠拷贝

	return 0;
}

void* my_memcpy(void* dest, const void* src, size_t num)
{
	assert(dest && src);//是指针不为空 不是对象为空
	void* ret = dest;//保持返回值类型为void*
	while (num--)
	{
		*(char*)dest = *(char*)src;
		dest = (char*)dest + 1;
		src = (char*)src + 1;
	}
	return dest;//返回值
}
int main()
{
	int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
	int arr2[20] = { 0 };
	my_memcpy(arr2, arr1, 20);
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		printf("%d ", arr2[i]);
	}
	return 0;
}
//memcpy不用来处理重叠的内存之间的数据拷贝（但是实际在vs中可以做到
//可以使用memove函数来实现，重叠内存之间的数据拷贝


//模拟实现memove
//不用重新用一个新数组 浪费内存空间，在原数组上进行就可以
void* my_memmove(void* dest, const void* src, size_t num)
{
	assert(dest && src);
	if (dest < src)//从前向后拷贝
	{
		while (num--)
		{
			*(char*)dest = *(char*)src;
			dest = (char*)dest + 1;
			src = (char*)src + 1;
		}
	}
	else//从后向前拷贝
	{
		while (num--)
		{
			*((char*)dest + num) = *((char*)src + num);
			//不需要再去指针加减 num已经自动在循环中--了
		}
	}

}
int main()
{
	int arr[] = { 1,2,3,4,5,6,7,8,9,10 };
	my_memmove(arr, arr + 2, 20);
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		printf("%d ", arr[i]);
	}
	return 0;
}

//memcmp
int main()
{
	int arr1[] = { 1,2,3,4,5 };//01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 05 00 00 00
	int arr2[] = { 1,3,2 };    //01 00 00 00 03 00 00 00 02 00 00 00
	//只有比较前面12个字节 arr1<arr2
	int ret = memcmp(arr1, arr2, 12);
	printf("%d\n", ret);//-1
	return 0;
}
//stringcmp只能用于字符 memcmp可以用于所有类型

//memset
int main()
{
	char arr[] = "hello bit";
	memset(arr, 'x', 5);//待填充内容 字符 字节
	printf("%s\n", arr);

	int arr1[10] = { 0 };
	memset(arr1, 1, 40);
	// 以字节为单位 把每一个字节都初始化为01 每一个整型被初始化为01 01 01 01

	return 0;
}