#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>

typedef struct Lnode
{
	int data;
	struct Lnode* next;
}Lnode, * Linklist;

Linklist createlist()
{
	int n = 0;//结点个数
	printf("请输入创建链表的结点个数:");
	scanf("%d", &n);
	Linklist L = (Linklist)malloc(sizeof(Lnode));
	L->next = NULL;
	Linklist pre = L;// 每一个结点的前赴
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
	return L;
}
void reverselist(Linklist L)
{
	Linklist p=L->next;//初始化为第一个结点
	Linklist pre = L;//逆序时的前赴
	Linklist first = L->next;//保留第一个结点 后续需要指向NULL
	while (p)
	{
		Linklist nex = p->next;
		p->next = pre;
		pre = p;//前赴更新
		p = nex;//p更新
	}
	L->next = pre;//头节点更新
	first->next = NULL;
}
void ranklist(Linklist L)
{
	Linklist pre = L;//前赴
	Linklist p = pre->next;//比较的第一个结点
	
	int len = 0;//计算链表的长度
	while (p)
	{
		len++;
		p = p->next;
	}
	p = pre->next;

	for (int i = 0; i < len-1; i++)
	{
		int flag = 0;//默认一次循环不需要排序
		for (int j=0;j<len-1-i;j++)
		if(p->data > p->next->data)
		{
			Linklist nex = p->next->next;//比较的两个结点的后继 防止断链
			pre->next = p->next;
			p->next->next = p;

			pre = p->next;
			p->next = nex;//更新pre和p

			flag = 1;
		}
		else
		{
			pre = p;
			p = p->next;
		}
		if (!flag)
			break;
		pre = L;
		p = L->next;//重新开始遍历
	}
}
void deletex(Linklist L, int min, int max)
{
	Linklist pre = L;
	while (pre->next)
	{
		Linklist p = pre->next;
		if (p->data > min && p->data < max)
		{
			Linklist q = p;
			pre->next = pre->next->next;
			free(q);
		}
		else
		{
			pre = pre->next;
		}
	}
}
void PrintandDel(Linklist L)
{
	Linklist pre=L;
	Linklist p = L->next;
	while (L->next)
	{
		Linklist q = L->next;//暂存最小值结点
		Linklist tmppre = L;//暂存最小值结点的前赴
		int min = L->next->data;
		pre =L;
		while (pre->next)
		{
			if (pre->next->data < min)
			{
				q = pre->next;//最小值结点
				tmppre = pre;//保存最小值的前赴
			}
			pre = pre->next;
		}
		printf("%d ", q->data);
		tmppre->next = q->next;
		free(q);
	}
}
void divide(Linklist Lji, Linklist Lou)
{
	Linklist p = Lji->next;

	Linklist rji = Lji;
	Linklist rou = Lou;//使用尾插法 设置尾指针
	
	int flag = 1;//flag为1时候为奇数 flag为0时候为偶数
	Lji->next = NULL;//将原来的链表置空

	while (p)
	{
		if (flag)//奇数
		{
			rji->next = p;
			rji = p;
			flag = 0;
		}
		else//偶数
		{
			rou->next = p;
			rou = p;
			flag = 1;
		}
		p = p->next;
	}
	rji->next = NULL;
	rou->next = NULL;
}
void deleteRep(Linklist L)
{
	Linklist p = L->next;
	Linklist tmp = L->next;
	while (p->next)
	{
		if (p->data == p->next->data)
		{
			tmp = p->next;
			p->next = p->next->next;
			free(tmp);
		}
		else
		{
			p = p->next;
		}
	}
}
Linklist Combinelist(Linklist L1,Linklist L2)//降序合并两个链表 使用头插法
{
	Linklist p1 = L1->next;
	Linklist p2 = L2->next;

	Linklist L = L1;//利用原来的L1空间储存L
	L->next = NULL;

	Linklist tmp1 = L1;
	Linklist tmp2 = L2;//记录原链表中的p1 p2位置

	while (p1 && p2)
	{
		if (p1->data < p2->data)//将a头插法到L中
		{
			tmp1 = p1->next;
			p1->next = L->next;
			L->next = p1;

			p1 = tmp1;
		}
		else
		{
			tmp2 = p2->next;
			p2->next = L->next;
			L->next = p2;

			p2 = tmp2;
		}
	}
	while (p1)
	{
		tmp1 = p1->next;
		p1->next = L->next;
		L->next = p1;

		p1 = tmp1;
	}
	while (p2)
	{
		tmp2 = p2->next;
		p2->next = L->next;
		L->next = p2;

		p2 = tmp2;
	}

	return L;
}
Linklist Commonlist(Linklist L1, Linklist L2)
{
	Linklist p1 = L1->next;
	Linklist p2 = L2->next;
	Linklist L = (Linklist)malloc(sizeof(L));//不破坏原来的链表 创建新链表
	Linklist r = L;
	while (p1 && p2)
	{
		if (p1->data == p2->data)
		{
			Linklist p = (Linklist)malloc(sizeof(Linklist));
			p->data = p1->data;//重新创建结点

			r->next = p;//尾插法
			r = p;

			p1 = p1->next;
			p2 = p2->next;
		}
		else
		{
			(p1->data < p2->data) ? (p1 = p1->next) : (p2 = p2->next);
		}
	}
	r->next = NULL;//新链表的结尾置空

	return L;
}
void Commonlist2(Linklist L1, Linklist L2)
{
	Linklist p1 = L1->next;
	Linklist p2 = L2->next;
	Linklist p = L1;//利用L1的内存
	p->next = NULL;

	while (p1 && p2)
	{
		if (p1->data == p2->data)
		{
			p->next = p1;

			p = p1;

			p1 = p1->next;
			p2 = p2->next;
		}
		else
		{
			(p1->data < p2->data) ? (p1 = p1->next) : (p2 = p2->next);
		}
	}
	p->next = NULL;//尾指针置空
}
void is_sonlist(Linklist L1, Linklist L2)
{
	Linklist p1 = L1->next;
	Linklist p2 = L2->next;
	while (p1 && p2)
	{
		if (p1->data == p2->data)//成功 继续比对下一组元素
		{
			p1 = p1->next;
			p2 = p2->next;
		}
		else//失败 L1继续后移动 L2回到起点
		{
			p1 = p1->next;
			p2 = L2->next;
		}
	}
	if (!p2)//判断p2是子序列  p2是子序列的话到最后为空 
		printf("yes\n");
	else
		printf("no\n");
}
int main()
{

	Linklist L = createlist();

	reverselist(L);//在原内存实现逆置

	ranklist(L);//在原内存使得单链表元素递增有序
	
	int min = 0;
	int max = 0;
	printf("请输入要删除的区间的最小值和最大值:");
	scanf("%d %d", &min, &max);
	deletex(L,min,max);//删除指定区间的链表结点
	 
	PrintandDel(L);//按递增顺序打印链表并且删除链表

	Linklist L1 = (Linklist)malloc(sizeof(Lnode));
	divide(L, L1);//将链表按照奇偶下标分成两个链表并做出打印 奇数仍放在L 偶数放在Lb中
	Linklist t = L1->next;
	for (int i = 0; t; i++)
	{
		printf("%d ", t->data);
		t = t->next;
	}

	deleteRep(L);//删除重复的结点

 	Linklist L1 = createlist();
 	Linklist L2 = createlist();

	Linklist q = Combinelist(L1, L2);//降序合并两个链表

	Linklist q = Commonlist(L1, L2);//合并两个链表的共同元素 并且不破坏原链表

	Commonlist2(L1, L2);//合并两个链表的共同元素 并放在L1中;

	is_sonlist(L1, L2);//判断L2是不是L1的子序列 

	Linklist q = L1->next;
	for (int i = 0; q; i++)
	{
		printf("%d ", q->data);
		q = q->next;
	}


	return 0;
}