#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

//7.合并两个线性表，并且返回合并后的线性表地址
int* mixarr(int* arr1, int* arr2, int len1, int len2)
{
	int i = 0;
	int j = 0;
	int* arr3 = (int*)malloc((len1 + len2) * sizeof(int));
	int k = 0;
	while (i < len1 && j < len2)
	{
		if (*(arr1 + i) < *(arr2 + j))
			*(arr3 + k++) = *(arr1 + i++);
		else
			*(arr3 + k++) = *(arr2 + j++);
	}
	if(i<len1)
		*(arr3 + k++) = *(arr1 + i++);
	if(j<len2)
		*(arr3 + k++) = *(arr2 + j++);

	return arr3;
}
int main()
{
	int arr1[] = { 1,3,5,9 };
	int arr2[] = { 2,4,4,6,10 };
	int len1 = sizeof(arr1) / sizeof(*arr1);
	int len2 = sizeof(arr2) / sizeof(*arr2);

	int len3 = len1 + len2;

	int* arr3=mixarr(arr1, arr2, len1, len2);

	for (int i = 0; i < len3; i++)
		printf("%d ", *(arr3 + i));

	return 0;
}

//8.逆序一个包含两个数组的数组
void reverse(int* arr, int left, int right)
{
	while (left < right)
	{
		int tmp = *(arr + left);
		*(arr + left) = *(arr + right);
		*(arr + right) = tmp;
		left++;
		right--;
	}
}
int main()
{
	int arr[] = { 1,2,3,4,11,22,33,44,55,66 };
	reverse(arr, 0, 9);

	reverse(arr, 0, 5);
	reverse(arr, 6, 9);

	for (int i = 0; i < 10; i++)
	{
		printf("%d ", *(arr + i));
	}
	return 0;
}

//9.二分法查找有序线性表中的元素 找到了和后一位元素对调 找不到插入元素
int findx(int* arr, int len, int x)
{
	int left = 0;
	int right = len - 1;
	int mid = (left + right) / 2;
	while (left <= right)
	{
		if (x == *(arr + mid))
			break;
		else if (x > *(arr + mid))
		{
			left = mid + 1;
			mid = (left + right) / 2;
		}
		else
		{
			right = mid - 1;
			mid = (left + right) / 2;
		}
	}
	if (left > right)//没找到
	{
		int i = len - 1;
		//延长数组!!!!!!!!!
		for (i = len - 1; i > right; i--)
		{
			*(arr + i+1) = *(arr + i);
		 }
		*(arr + right+1) = x;
		return len + 1;
	}
	else//找到了
	{
		int tmp = *(arr + mid+1);
		*(arr + mid+1) = *(arr + mid);
		*(arr + mid) = tmp;
		return len;
	}
}
int main()
{
	int arr[] = {1,3,5,9,16,55,79,99};
	int x;
	printf("请输入要查找的元素: x=");
	scanf("%d", &x);
	int len = sizeof(arr) / sizeof(int);

	int newlen= findx(arr, len, x);
	for (int i = 0; i < newlen; i++)
	{
		printf("%d ", arr[i]);
	}

	return 0;
}

//11.在两个顺序表的合并中寻找中位数
int findmid(int* arr1, int* arr2, int mid,int len1,int len2)
{
	int i = 0;
	int j = 0;
	int count = 0;//用于记录合并的整体数组的下标 count=mid时找到中位数
	for (i = 0, j = 0;i<len1 && j<len2;)
	{
		if (count++ == mid)
		{
			return *(arr1 + i) < *(arr2 + j) ? *(arr1 + i) : *(arr2 + j);
		}
		else
		{
			*(arr1 + i) < * (arr2 + j) ? (i++) : (j++);
		}
	}
	if (i == len1)
		return *(arr1 + i - 1);
	else
		return *(arr2 + j - 1);
}
int main()
{
	int arr1[] = { 1,1,3,4,5 };
	int arr2[] = { 0,1,2,2,4 };
	int len1 = sizeof(arr1) / sizeof(int);
	int len2 = sizeof(arr2) / sizeof(int);
	int mid = (len1 + len2)/2;
	int midnum=findmid(arr1, arr2, mid,len1,len2);
	printf("中位数是：%d", midnum);

	return 0;
}


//12.寻找主元素 主元素要占一半以上 有就返回主元素 没有就返回-1
int findmainnum(int* arr, int len)
{
	int count = 1;//计数 判断一个元素是否为众数
	int tmpmainnum = *arr;//备选主元素
	int i = 0;
	for (i = 1; i < len; i++)//先找出众数
	{
		if (*(arr + i) == tmpmainnum)
		{
			count++;
		}
		else
		{
			if (count > 0)
			{
				count--;
			}
			else
			{
				count = 1;
				tmpmainnum = *(arr + i);//更换备选主元素
			}
		}
	}

	int maincount = 0;
	for (i = 0; i < len; i++)
	{
		if (*(arr + i) == tmpmainnum)
			maincount++;
	}
	if (maincount > len / 2)
		return tmpmainnum;
	return -1;
}
int main()
{
	int arr[] = { 5,7,5,6,5,5,5,3 };
	int len = sizeof(arr) / sizeof(int);

	int ret=findmainnum(arr, len);
	printf("%d", ret);
	return 0;
}