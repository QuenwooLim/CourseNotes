#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//13.寻找有序序列中未曾出现的最小正整数
int findmin(int* arr, int len, int max)
{
	int* arr0 = (int*)malloc(sizeof(int) * max);//创建辅助数组
	int i = 0;
	for (i = 0; i < max; i++)
	{
		*(arr0 + i) = 0;//辅助数组初始化为0
	}
	for (i = 0; i < len; i++)
	{
		if (*(arr + i) > 0)
		{
			arr0[*(arr + i )-1] = 1;
		}
	}
	for (i = 1; i < max; i++)
	{
		if (*(arr0 + i) == 0)
			return i + 1;
	}
	return i + 1;
}
int main()
{
	int arr[] = { -10,-2,1,3,5,7,9 };
	int len = sizeof(arr) / sizeof(int);
	int max = *(arr + len - 1);//元素中的最大值

 	int min= findmin(arr, len, max);
	printf("%d", min);
	return 0;
}

//求解三元组的最小距离
int Distance(int a,int b,int c)//返回当前举例
{
	return abs(a - b) + abs(a - c) + abs(b - c);
}
int minimum(int a,int b,int c)//返回当前三个数中的最小数值
{
	int min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	return min;
}
int findminD(int* arr1, int* arr2, int* arr3,int len1,int len2,int len3)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int Dis = 10000;//距离 初始化为较大值
	while (i < len1 && j < len2 && k < len3 )
	{
		if (Distance(*(arr1 + i), *(arr2 + j), *(arr3 + k)) < Dis)
			Dis = Distance(*(arr1 + i), *(arr2 + j), *(arr3 + k));
		if(*(arr1+i)==minimum(*(arr1+i),*(arr2+j),*(arr3+k)))
			i++;
		else if(*(arr2 + j) == minimum(*(arr1 + i), *(arr2 + j), *(arr3 + k)))
			j++;
		else
			k++;
	}
	return Dis;
}
int main()
{
	int arr1[] = { -1,0,9 };
	int arr2[] = { -25,-10,10,11 };
	int arr3[] = { 2,9,17,30,41 };
	int len1 = sizeof(arr1) / sizeof(int);
	int len2 = sizeof(arr2) / sizeof(int);
	int len3 = sizeof(arr3) / sizeof(int);

	int minD = findminD(arr1, arr2, arr3, len1, len2, len3);
	printf("%d", minD);

	return 0;
}


//14.删除链表中的某一个元素
typedef struct Lnode
{
    int data;
    struct Lnode* next;
}Lnode,*Linklist;

void CreateList_H(Linklist* L, int n)
{
    (*L) = (Linklist)malloc(sizeof(Linklist));
    (*L)->next = NULL;//先建立一个带头结点的单链表
    for (n; n > 0; n--)
    {
        Linklist p = (Linklist)malloc(sizeof(Linklist));//创建节点
        scanf("%d", &p->data);

        p->next = (*L)->next;//将新元素插入到表头
        (*L)->next = p;
    }
}//时间复杂度O(n)

void CreateList_R(Linklist* L, int n)
{
    (*L) = (Linklist)malloc(sizeof(Linklist));
    (*L)->next = NULL;
    Linklist r =(*L);//尾指针
    for (n; n > 0; n--)
    {
        Linklist p = (Linklist)malloc(sizeof(Linklist));
        scanf("%d", &p->data);
        p->next = NULL;
        r->next = p;//将新元素插入到表尾
        r = p;//改变尾指针
    }
}//时间复杂度O(n)

void deletex(Linklist L,int x)//删除某结点
{
    Linklist p = L->next;
    while (p)
    { 
    if (p->data == x)
      {
        p->next = p->next->next;
      }
        p = p->next;
    }

}

int main()
{
    Linklist L1=0;
    int n1 = 10;
    Linklist L2=0;
    int n2 = 8;
    CreateList_H(&L1, n1);//使用头指针创建有n1个元素的链表 
 // CreateList_R(&L2, n2);//使用尾指针创建有n1个元素的链表
    //都要传二级指针 才能改变实参

    int x = 5;
    deletex(L1, x);

    Linklist p = L1->next;
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }

    return 0;
} 