#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

int main()
{
	int i = 10;
	int j = 20;
	int k = 3;
	k *= i + j;//赋值操作符优先级比较低  先算+再算*=
	printf("%d", k);

	return 0;
}


//are you ok 让每个单词内部变得有序
void reverse(char* left, char* right)
{
	while (left < right)
	{
		char tmp = *left;
		*left = *right;
		*right = tmp;
		left++;
		right--;
	}
}
int main()
{
	char arr[101] = { 0 };
	gets(arr);
	int len = strlen(arr);

	reverse(arr, arr + len - 1);

	char* start = arr;

	while (*start)
	{
		char* end = start;
		while (*end != ' ' && *end != '\0')
		{
			end++;
		}

		reverse(start, end - 1);

		if (*end != '\0')
			end++;
		start = end;
	}
	printf("%s", arr);

	return 0;
}


int main()
{
	int a[4] = { 1,2,3,4 };

	int* ptr1 = (int*)(&a + 1);//a是数组名 &a是数组地址 +1跳过一个数组
	int* ptr2 = (int*)((int)a + 1);
	/*
	数组在小端内存中存储为
	01 00 00 00  02 00 00 00  03 00 00 00  04 00 00 00
	(int)a将地址a强制类型转换为整型 +1跳过一个字节的地址 此时内存中ptr2地址指向 00 00 00  02
	则ptr2为整型02 00 00 00的地址
	*ptr2解引用为2000000
	*/

	printf("%x %x", ptr1[-1], *ptr2);
	return 0;
}
