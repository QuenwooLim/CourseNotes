#include<stdio.h>

//例子1:实现两个多项式的加减运算
int main()
{
	int arr1[100] = { 0 };
	int arr2[100] = { 0 };
	int arr3[200]={ 0 };
	int n1 = 0;
	int n2 = 0;
	scanf_s("%d", &n1);
	scanf_s("%d", &n2);
	int i;
	for (i = 0; i < n1; i++)
	{
		scanf_s("%d", &arr1[i]);
	}
	for (i = 0; i < n2; i++)
	{
		scanf_s("%d", &arr2[i]);
	}
	for (i = 0; i < (n1 > n2 ? n1 : n2); i++)
	{
		arr3[i] = arr1[i] + arr2[i];
		printf("%dx^%d+", arr3[i], i);
	}
	
	return 0;
}

//例子2:实现两个稀疏多项式相加
typedef struct Pnode
{
	float xishu;
	int zhishu;
	struct Pnode* next;
}Pnode;

void createP(Pnode* P, int n)//头插法
{
	P = (Pnode*)malloc(sizeof(Pnode));
	P->next = NULL;
	Pnode* pre = P;//pre用于保存q的前驱 初始值为头结点
	for (int i = 1; i <= n; i++)
	{
		Pnode* s = (Pnode*)malloc(sizeof(Pnode));
		scanf("%d %d", s->xishu, s->zhishu);
		Pnode* q = P->next;//q的初始化 指向首元节点
		while (q && q->zhishu < s->zhishu)//找到第一个大于输入项指数的项*q
		{
			pre = q;
			q = q->next;
		}
		s->next = q;
		pre->next = s;
	}
}
void addP(Pnode* P1, Pnode* P2, Pnode* P3)
{
	Pnode* Pa = P1->next;
	Pnode* Pb = P2->next;
	P3 = P1;
	Pnode* Pc = Pa;
	int i = 1;
	for (i = 1; Pa->next && Pb->next; i++)
	{
		if (Pa->zhishu == Pb->zhishu)
		{
			if (Pa->xishu + Pb->xishu == 0)
			{
	 			deleteP(&P1, i);
	   			deleteP(&P2, i);//删除P1和P2对应结点
			}
			else
				Pa->xishu = Pa->xishu + Pb->xishu;
			Pa = Pa->next;
 			Pb = Pb->next;
 			Pc = Pc->next;
		}
	   	else if (Pa->zhishu < Pb->zhishu)
		{
			Pc->next = Pa;
			Pc = Pc->next;
			Pa = Pa->next;
		}
		else
		{
			Pc->next = Pb;
			Pc = Pc->next;
			Pb = Pb->next;
			deleteP(&P2, i);
		}
	}
	Pc->next = (Pa ? Pa : Pb);
	destorylist(&P2);
}
int main()
{
	Pnode P1, P2, P3;
	int n=5, m=6;
	createP(&P1, n);
	createP(&P2, m);
	addP(&P1, &P2, &P3);
	return 0;
}