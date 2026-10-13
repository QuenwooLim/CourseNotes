#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include<string.h>

//奇数变为1 偶数变为1
int main()
{
	int input = 0;
	scanf("%d", &input);
	int ret = 0;
	int i = 0;
	for (i = 0; input != 0; i++)
	{
		if (input % 10 % 2 == 1)//奇数
		{
			ret = ret + 1 * pow(10, i);
		}
		else if (input % 10 % 2 == 0)//偶数
		{
			ret = ret + 0 * pow(10, i);
		}
		input = input / 10;
	}
	printf("%d", ret);

	return 0;
}


//打印边长为n的等腰直角三角形
int main()
{
	int n = 0;
	scanf("%d", &n);
	int i = 1;
	int j = 1;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
			if (i + j <= n)
			{
				printf("  ");
			}
			else
			{
				printf(" *");
			}
		}
		printf("\n");
	}

	return 0;
}

int main()
{
	double price = 0.0;//double类型表示小数 初始化为0.0
	int month = 0;
	int day = 0;
	int i = 0;
	scanf("%lf %d %d %d", &price, &month, &day, &i);
	if (month == 11 && day == 11)
	{
		price = price * 0.7;
	}
	else if (month == 12 && day == 12)
	{
		price = price * 0.8;
	}
	if (i == 1)
		price = price - 50;
	printf("%.2lf", price);

	return 0;
}

int main()
{
	int a, b, c, d, e;

	for (a = 1; a <= 5; a++)
	{
		for (b = 1; b <= 5; b++)
		{
			for (c = 1; c <= 5; c++)
			{
				for (d = 1; d <= 5; d++)
				{
					for (e = 1; e <= 5; e++)
					{
						if (
							(b == 2) + (a == 3) == 1 &&//表示有一真一假
							(b == 2) + (e == 4) == 1 &&
							(c == 1) + (d == 2) == 1 &&
							(c == 5) + (d == 3) == 1 &&
							(e == 4) + (a == 1) == 1 &&
							a * b * c * d * e == 120//条件筛选 防止名词重复
							)
						{
							printf("%d %d %d %d %d\n", a, b, c, d, e);
						}
					}
				}
			}
		}
	}
	return 0;
}


//找凶手
//解法一
int main()
{
	int a, b, c, d;
	for (a = 0; a <= 1; a++)
	{
		for (b = 0; b <= 1; b++)
		{
			for (c = 0; c <= 1; c++)
			{
				for (d = 0; d <= 1; d++)
				{
					if ((a == 0) + (c == 1) + (d == 1) + (d == 0) == 3
						&& a + b + c + d == 1)
					{
						if (a == 1)
							printf("a");
						else if (b == 1)
							printf("b");
						else if (c == 1)
							printf("c");
						else if (d == 1)
							printf("d");
						printf("\n");
					}
				}
			}
		}
	}
	return 0;
}
//解法二
int main()
{
	int killer = 0;
	for (killer = 'a'; killer <= 'd'; killer++)
	{
		if ((killer != 'a') + (killer == 'c') + (killer == 'd') + (killer != 'd') == 3)
			printf("%c", killer);
	}
	return 0;
}


//杨辉三角
int main()
{
	int i = 0;
	int j = 0;
	int arr[10][10] = { 0 };
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j <= i; j++)
		{
			if (j == 0 || i == j)
				arr[i][j] = 1;
			else
				arr[i][j] = arr[i - 1][j - 1] + arr[i - 1][j];

		}
	}
	for (i = 0; i < 10; i++)
	{
		int k = 0;
		for (k = 0; k < 10 - i; k++)
		{
			printf("   ");
		}
		for (j = 0; j <= i; j++)
		{
			printf("%3d   ", arr[i][j]);

		}
		printf("\n");
	}

	return 0;
}


//实现一个函数，可以左旋字符串中的k个字符
void left_rotate(char arr[], int k)
{
	int i = 0;
	int len = strlen(arr);
	k %= len;//防止k过大 浪费运算时间
	for (i = 0; i < k; i++)//循环k次 每次实现左旋一个字符
	{
		int tmp = arr[0];
		int j = 0;
		for (j = 0; j < len - 1; j++)
		{
			arr[j] = arr[j + 1];
		}
		arr[len - 1] = tmp;
	}
}
int main()
{
	char arr[] = "abcdef";
	int k = 0;
	scanf("%d", &k);
	left_rotate(arr, k);

	printf("%s", arr);
	return 0;
}


//杨氏矩阵 
//利用右上角的数字的特点：一行中最大 一列中最小
//写一个函数来寻找二维数组里的数字

//方法一 传址调用来修改指针指向内容 
struct Ponit//方法二 创建结构体来传回两个变量
{
	int x;
	int y;
};
struct Ponit find(int arr[3][3], int row, int col, int n)//函数返回值类型相应改变
{
	int x = 0;
	int y = col - 1;
	struct Ponit p = { -1,-1 };
	while (x <= row && y >= 0)
	{
		if (n > arr[x][y])
		{
			x++;
		}
		else if (n < arr[x][y])
		{
			y--;
		}
		else if (n == arr[x][y])
		{
			p.x = x;
			p.y = y;
			return p;//利用结构体来返回两个变量
		}
	}
	return p;
}
int main()
{
	int arr[3][3] = { 1,3,5,7,9,11,13,15,17 };
	int n = 0;
	scanf("%d", &n);
	struct Ponit ret = find(arr, 3, 3, n);//函数返回值类型相应改变
	printf("%d %d", ret.x, ret.y);
	return 0;
}

//判断一个字符串是不是另一个字符串左旋之后的结果
//法一 利用字符串不断左旋来判断是否有相等的机会
//法二 字符串给自己追加一个字符串（追加之后包含所有左旋的结果）
//再判断另一字符串是不是追加之后字符串的字串 
int is_left_move(char arr1[], char arr2[])
{
	int len = strlen(arr1);
	strncat(arr1, arr1, len);
	char* ret = strstr(arr1, arr2);
	if (ret == NULL)
		return 0;
	else
		return 1;
}
int main()
{
	char arr1[] = "abcdef";
	char arr2[] = "cdefab";
	int ret = is_left_move(arr1, arr2);

	if (ret == 1)
		printf("yes");
	else
		printf("no");
	return 0;
}

//判断一个序列是否为有序(都相等也表示有序)
int main()
{
	int arr[10] = { 0 };
	int n = 0;
	scanf("%d", &n);
	int i = 0;
	int flag1 = 0;//0表示初始状态时候是无序
	int flag2 = 0;
	for (i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
		if (i > 0);
		{
			if (arr[i] > arr[i - 1])//升序
			{
				flag1 = 1;
			}
			if (arr[i] < arr[i - 1])
			{
				flag2 = 1;
			}
			else
			{
				;//前后数字相等时保持flag1 flag2都不变
			}
		}
	}
	if (flag1 + flag2 <= 1)
	{
		printf("sorted");
	}
	else
		printf("unsorted");
}