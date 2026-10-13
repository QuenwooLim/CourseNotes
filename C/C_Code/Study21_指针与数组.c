#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

//数组名的理解
//指针的运算和指针类型的意义
int main()
{
	int a[] = { 1,2,3,4 };

	printf("%d\n", sizeof(a));//a是数组名

	printf("%d\n", sizeof(a + 0));//a并非单独放在sizeof中，也没有取地址,a是首元素地址 

	printf("%d\n", sizeof(*a));//a是首元素的地址,*a表示第一个元素

	printf("%d\n", sizeof(a + 1));//第二个元素

	printf("%d\n", sizeof(&a));//数组的地址

	printf("%d\n", sizeof(*&a));//a取出的是数组 ，& 和 *可以相互抵消

	printf("%d\n", sizeof(&a + 1));//a是数组名

	return 0;
}

//二维数组
int main()
{
	int arr[3][4] = { 1,2,3,4,2,3,4,5,3,4,5,6 };
	/*
	1 2 3 4
	2 3 4 5
	3 4 5 6
	*/
	printf("%d\n", sizeof(arr));//数组名表示二维数组第一行
	printf("%d\n", sizeof(arr[0][0]));//首元素 4
	printf("%d\n", sizeof(arr[0]));//arr[0]表示第一行 在sizeof中表述首元素的地址
	printf("%d\n", sizeof(arr[0] + 1));//arr不是单独在arr中 表示数组名 arr[0]+1是第一行第二个元素地址 4/8
	printf("%d\n", sizeof(*(arr[0] + 1)));//第一行第二个元素 4
	printf("%d\n", sizeof(arr + 1));//arr是数组名 但是没有单独放在sizeof内部 表示第一行地址 +1表示第二行地址
	printf("%d\n", sizeof(*(arr + 1)));//对第二行和地址解引用 表示第二行数组 16
	printf("%d\n", sizeof(&arr[0] + 1));//第二行地址
	printf("%d\n", sizeof(*(&arr[0] + 1)));//数组第二行 16
	printf("%d\n", sizeof(*arr));//对第一行地址解引用 得到第一行
	printf("%d\n", sizeof(arr[3]));//第四行 并不会真的访问第四行且造成越界访问 16
	//类似sizeof(int) 知道类型就可以 不需要知道内容

	return 0;
}

int main()
{
	int a[5] = { 1,2,3,4,5 };
	int* ptr = (int*)(&a + 1);//a的类型是数组指针 +1是跳过数组

	printf("%d %d", *(a + 1), *(ptr - 1));
	//a是首元素地址 *（a+1）是对第二个元素地址的解引用;
	//prt是int类型指针 -1向后跳过一个元素 可行的操作

	return 0;
}


int main()
{
	int a[3][2] = { (0,1),(2,3),(3,4) };//逗号表达式 此时内存中的二维数组为 1 3   4 0   0 0
	int* p;
	p = a[0];//首元素的地址
	printf("%d", p[0]);//p[0]相当于 *（p+0） 就是首元素 1

	return 0;
}

int main()
{
	int arr[5][5];//二维
	int(*p)[4];//一维 基类型是元素为4个整形的数组
	p = arr;//类型不同 会报错 但还是可以强行赋值
	printf("%p %d", &p[4][2] - &arr[4][2], &p[4][2] - &arr[4][2]);//指针相减得到元素个数
	//元素个数-4 %p打印的是地址 是-4的补码
	/*
	arr二维数组
	1 2 3 4 5    2 3 4 5 6   3 4 5 6 7   4 5 6 7 8   5 6 7！ 8 9

	p二维数组
	1 2 3 4    5 2 3 4   5 6 3 4   5 6 7 4   5 6 7！ 8    5 6 7 8   9
	&p[4][2] 可以依次向后访问
	p[4][2]相当于 *(*(p+4)+2) p的类型是int(*)[4] +1跳过的是4个元素的数组
	*/
	return 0;
}

int main()
{
	int aa[2][5] = { 1,2,3,4,5,6,7,8,9,10 };
	int* ptr1 = (int*)(&aa + 1);//aa是二维数组名 
	int* ptr2 = (int*)(*(aa + 1));//*(aa+1)得到第二行的数组名 表示第二行首元素6的地址
	printf("%d %d", *(ptr1 - 1), *(ptr2 - 1));

	return 0;
}

int main()
{
	char* a[] = { "work","at","ailbaba" };
	//每个元素都是 char*类型 存储字符串首元素的地址
	//第一个char*存储w 第二个char*存储a 第三个char*存储a

	char** pa = a;
	//pa二级指针存储a的首元素w的char*的地址  
	pa++;
	//pa++ 指向下一个元素a的char*的地址
	printf("%s\n", *pa);
	//*pa解引用得到a的地址 通过地址就可以找到at这一字符串

	return 0;
}

//存储字符串的数组
int main()
{
	char* c[] = { "ENTER","NEW","POINT","FIRST" };//存储的都是首元素E N P F的地址
	char** cp[] = { c + 3,c + 2,c + 1,c };
	char*** cpp = cp;

	printf("%s\n", **++cpp);
	//先 ++cpp 再*++cpp 再**++cpp找到POINT
	printf("%s\n", *-- * ++cpp + 3);
	//先++cpp 再*++cpp 再--*++cpp 再*--*++cpp 再*--*++cpp+3找到POINT
	printf("%s\n", *cpp[-2] + 3);
	//之前的操作已经把cpp指向了cp数组第三个元素的地址
	//*cpp[-2]相当于 *(*(cpp-2))得到FIRST的地址 就是首元素F的地址 +3找到S的地址
	//此时字符串打印出来 ST
	printf("%s\n", cpp[-1][-1] + 1);
	//cpp[-1][-1]相当于 *(*(cpp-1)-1)
	//cpp-1找到cp第二个元素地址 *(cpp-1)找到cp第二个元素c+2
	//*(cpp-1)-1找到NEW
	//*（*（cpp-1）-1）找到c+1指向的cp中的元素NEW 表示NEW的首元素N的地址
	//最后再+1 找到NEW中E的地址
	//字符串最终打印EW

	return 0;
}