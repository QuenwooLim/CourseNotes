#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct Lnode
{
	int data;
	struct Lnode* next;
}Lnode,*Linklist;
//3.递归实现逆序打印
void reverseOutput(Linklist p)
{
	if (p == NULL)
		return;
	else
	{
		reverseOutput(p->next);
		printf("%d ", p->data);
	}
}
//4.删除链表的最小值的结点
void deleteMin(Linklist L)//p为首元节点
{
	Linklist p = L->next;
	int min = p->data;//先预设最小值为首元结点的数据
	Linklist pre=L;//要删除的结点的前赴
	
	while (p->next)
	{
		if (p->next->data < min)
		{
			min = p->next->data;
			pre = p;
		}
		p = p->next;
	}

	Linklist q = pre->next;
	pre->next = pre->next->next;
	free(q);
}
int main()
{
	int n = 0;//结点个数
	printf("请输入创建链表的结点个数:");
	scanf("%d", &n);
	Linklist L = (Linklist)malloc(sizeof(Lnode));
	L->next = NULL;
	Linklist pre=L;// 每一个结点的前赴
	for (int i = 1; i <= n; i++)
	{
		Linklist p = (Linklist)malloc(sizeof(Lnode));//创建临时结点
		printf("请输入第%d个结点的值:", i);
		scanf("%d", &p->data);
		p->next = NULL;
		pre->next = p;
		pre = pre->next;//成为下一个结点的前赴
	}
	pre->next = NULL;//结尾置空

	//printf("逆序后的链表:");
	//reverseOutput(L->next);//逆序

	deleteMin(L);//删除最小值
	Linklist q = L->next;
	for (int i = 0; i < n - 1; i++)
	{
		printf("%d ", q->data);
		q = q->next;
	}

	return 0;
}