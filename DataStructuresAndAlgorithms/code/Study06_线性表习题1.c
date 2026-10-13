#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//1.删除顺序表中的最小值元素 并用顺序表最后一个元素替换 并返回删除的元素
int delmin(int* arr,int n)
{
	if (!arr)
		return;
	int min = *arr;
	int pos = 0;
	for (int i = 1; i < n; i++)
	{
		if (*(arr + i) < min)
		{
			min = *(arr + i);
			pos = i;
		}
	}
	int ret= *(arr + pos);

	*(arr + pos) = *(arr + n - 1);

	return ret;
}
int main()
{
	int n;
	printf("请输入元素个数:");
	scanf("%d", &n);
	int* arr = (int*)malloc(sizeof(int));//动态内存分配

	int i = 0;
	printf("请输入n个元素:");
	for (i = 0; i < n; i++)
	{
		scanf("%d", arr+i);
	}
	for (i = 0; i < n; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");

	int ret = delmin(arr, n);
	printf("%d\n", ret);

	for (i = 0; i < n-1; i++)
	{
		printf("%d ", arr[i]);
	}
	
	free(*arr); 

	return 0;
}


//2.删除顺序表中的某个元素 并用后面的元素补齐 并返回删除的个数
int delx(int* arr, int n,int x)
{
	int k = 0;//记录删除元素个数
	for (int i = 0; i < n; i++)
	{
		if (*(arr + i) == x)
		{
			k++;
		}
		else
		{
			arr[i - k] = arr[i];
		}
	}
	for (int i = n - k; i < n; i++)
	{
		*(arr + i) = NULL;
	}
	return k;
}
int main()
{
	int arr[10] = { 1,2,3,4,5,6,1,2,3,4 };
	int n = sizeof(arr)/sizeof(arr[0]);
	int k = delx(arr, n, 2);
	printf("%d\n", k);
	for (int i = 0; i < n - k; i++)
	{
		printf("%d ", *(arr + i));
	}

	return 0;
}

//3.从顺序表中删除给定区间s-t之间的值，若给定区间不合理或顺序表为空，则显示错误信息并退出
void deletepoint(int* arr, int s, int t, int n)
{
	int k = 0;
	int i = 0;
	for (i = 0; i < n && *(arr + i) < s; i++);
	for (k = i; k <= n && *(arr + k) <= t; k++);
	for (; k < n; i++, k++)
		*(arr + i) = *(arr + k);
	for (int j = 0; j<i; j++)
	{
		printf("%d ", arr[j]);
	}
}
int main()
{
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int n = sizeof(arr) / sizeof(*arr);

	int s = 0;
	int t = 0;
	scanf("%d %d", &s, &t);
	if (s >= *arr && s < t && t <= *(arr + n-1))
	{
		deletepoint(arr,s,t,n);
	}
	else
		printf("Error");


	return 0;
}

//4.从有序顺序表中删除重复多次的值，只保留一个，使得每一个元素都不相同
void deletex(int* arr, int n)
{
	int k = 0;//用来记录新顺序表的下标
	for (int i = 0; i < n; i++)
	{
		if (*(arr + i) - *(arr + i + 1))
			*(arr + k++) = *(arr + i);
	}
	if (*(arr + n - 1) == *(arr + n))//如果数组最后一个元素和越界的第一个未知元素相同
		*(arr + k++) = *(arr + n - 1);
	for (int i = 0; i< k; i++)
	{
		printf("%d ", arr[i]);
	}
}
int main()
{
	int arr[10] = { 1,2,3,3,4,5,5,6,7,8 };
	int n = sizeof(arr) / sizeof(*arr);
	deletex(arr, n);

	return 0;
}