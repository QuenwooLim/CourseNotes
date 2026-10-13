#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct Lnode
{
	int data;
	struct Lnode* pre;//前赴
	struct Lnode* next;//后继
	int freq;//设置访问频度参数
}*Linklist,Lnode;
Linklist CreateDouLoopLink()
{
	int n = 0;//链表结点个数
	Linklist L = (Linklist)malloc(sizeof(Linklist));
	L->next = NULL;
	L->pre = NULL;
	Linklist preP = L;//新结点的前赴
	L->next = NULL;

	printf("创建双循环链表的结点个数:");
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
	{
		Linklist p = (Linklist)malloc(sizeof(Linklist));
		printf("请输入第%d个结点的数据域:",i);
		scanf("%d", &p->data);
		p->pre = preP;
		preP->next = p;
		preP = p;//更新前赴
	}
	//跳出循环时候preP代表最后一个结点
	preP->next = L;//尾指针指向头节点
	L->pre = preP;
	return L;
}
void is_symmetry(Linklist L)
{
	Linklist nexP = L->next;
	Linklist preP = L->pre;
	while (preP != nexP && nexP->next!=preP)//奇数个和偶数个结点链表的判定
	{
		if (nexP->data != preP->data)
		{
			printf("该双循环链表结点不对称\n");
			return;
		}
		else
		{
			preP = preP->pre;
			nexP = nexP->next;
		}
	}
	if (nexP->data != preP->data)//偶数个结点链表判断
	{
		printf("该双循环链表结点不对称\n");
		return;
	}
	printf("该双循环链表结点对称\n");
}
Linklist CreateSinLoopLink()
{
	int n = 0;
	printf("请输入要创建的链表的结点个数:");
	scanf("%d", &n);
	Linklist L = (Linklist)malloc(sizeof(Linklist));
	Linklist r = L;//尾指针
	for (int i = 1; i <= n; i++)
	{
		Linklist p = (Linklist)malloc(sizeof(Linklist));
		printf("请输入第%d个结点的数据域:",i);
		scanf("%d", &p->data);
		p->freq = 0;
		r->next = p;
		r = p;
	}
	r->next = L;//尾结点的next指向头节点

	return L;
}
void LinkTwoList(Linklist L1, Linklist L2)
{
	Linklist p = L1->next;
	while (p->next != L1)
		p = p->next;//使得p1走到尾指针
	p->next = L2->next;
	while (p->next != L2)
	{
		p = p->next;
	}
	p->next = L1;
}
void CoutAndDeleteLink(Linklist L)
{
	Linklist pre = L;//结点的前赴
	while (pre->next != L)
	{
		Linklist minpre = pre;
		Linklist minP = pre->next;
		while (pre->next!=L)
		{
			if (pre->next->data  <  minP->data)
			{
				minP = pre->next;
				minpre = pre;
			}
			pre = pre->next;
		}
		printf("%d ", minP->data);
		minpre->next = minpre->next->next;
		free(minP);
		pre = L;
	}
}
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
void LocateAndRank(Linklist L, int num)
{
	Linklist p = L->next;
	Linklist pre = L;//p的前赴
	Linklist tmp=L->next;
	int is_find = 1;
	while (p)
	{
		if (p->data == num)//查找到了该元素
		{
			printf("查找到该元素\n");
			tmp = p;
			tmp->freq++;
			pre->next = p->next;//将该结点抠出来
			is_find = 0;
			break;
		}
		p = p->next;
		pre = pre->next;
	}
	if (is_find)
	{
		printf("没找到\n");
		return;
	}
	p = L->next;
	pre = L;
	while (p)
	{
		if (tmp->freq >= p->freq)//选择频度大于等于的进行插入 频度相同 最新访问的靠前
		{
			tmp->next = p;
			pre->next = tmp;
			break;
		}
		p = p->next;
		pre = pre->next;
	}
}

int FindBackcount(Linklist L, int k)
{
	int len = 0;//计算链表的长度
	Linklist p = L->next;
	while (p)
	{
		len++;
		p = p->next;
	}
	p = L->next;

	if (k > len)
	{
		printf("该结点不存在\n");
		return 0;
	}

	Linklist q = L->next;//创建辅助结点
	while (k--) 
	{
		q = q->next;
	}

	while (q)
	{
		p = p->next;
		q = q->next;
	}
	return p->data;

}
int FindComStart(Linklist L1,Linklist L2)
{
	int len1 = 0;
	int len2 = 0;
	Linklist p1 = L1->next;
	Linklist p2 = L2->next;
	while (p1)
	{
		p1 = p1->next;
		len1++;
	}
	while (p2)
	{
		p2 = p2->next;
		len2++;
	}

	(len1 > len2) ? (p1 = L1->next, p2 = L2->next) : (p2 = L1->next, p1 = L2->next);//p1指向长的链表首元节点

	int gap = abs(len1 - len2);
	while (gap--)
	{
		p1 = p1->next;
	}
	while (p1->data != p2->data)
	{
		p1 = p1->next;
		p2 = p2->next;

		if (!p1)
		{
			printf("没有公共部分");
			return  -1;
		}
	}
	return p1->data;
}
void DeleteRep(Linklist L)
{
	int max = 100+1;//找出链表中元素的最大值+1
	int* arr = (int*)malloc(sizeof(int) * max);//创建数组
	while (max--)
	{
		*(arr + max) = 0;//初始化为0
	}

	Linklist pre = L;
	Linklist p = L->next;
	while (p)
	{
		int num = abs(p->data);//元素的绝对值
		if (*(arr+num) == 0)
		{
			*(arr + num) = 1;//一个元素出现 在数组对应元素下标的值设置1 表示该元素已经出现过一次
			p = p->next;
			pre = pre->next;
		}
		else//该元素已经出现过了
		{
			Linklist q = p;
			p = p->next;
			pre->next = p;
			free(q);
		}
	}
}
int is_Loop(Linklist L)
{
	Linklist fast = L;//设置快指针 一次走两个
	Linklist slow = L;//设置慢指针 一次走一个

	while (fast->next && fast && slow)
	{
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast)//相遇
			break;
	}
	if ((!fast->next)||(!fast))//没有环 fast先走到尽头
		return -1;
	
	fast = L;//fast回到开头
	while (fast != slow)
	{
		fast = fast->next;
		slow = slow->next;
	}
	return fast->data;
}

void rotate(Linklist L)//逆置函数
{
	Linklist pre = L;//前赴
	Linklist p = L->next;
	Linklist nex = p->next;//后继
	Linklist r = L->next;//尾结点置空
	
	while (nex)
	{
		p->next = pre;

		pre = p;
		p = nex;
		nex = nex->next;
	}
	p->next = pre;
	L->next = p;
	r->next = NULL;

}
void CrossTheLink(Linklist L)
{
	Linklist fast = L;
	Linklist slow = L;//利用快慢指针找到链表中点
	while (fast->next && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}//中止时候slow刚好到达中点
	
	rotate(slow);//将中点后面的链表逆序 中点slow作为逆序链表的头指针

	fast = L->next;
	Linklist mid = slow->next;
	slow->next = NULL;
	slow = mid;//将链表分断

	while (slow)
	{
		mid = slow->next;
		slow->next = fast->next;
		fast->next = slow;
		slow = mid;
		fast = fast->next->next;
	}
}
int main()
{
 	Linklist L=CreateDouLoopLink();//创建带有头节点L的双循环链表

	is_symmetry(L);//判断一个双向循环链表的结点数据域是否对称

	Linklist L1 = CreateSinLoopLink();//创建带有头节点L的单循环链表
	Linklist L2 = CreateSinLoopLink();//创建带有头节点L的单循环链表
	LinkTwoList(L1, L2);//链接两个链表放到L1中

	Linklist L = CreateSinLoopLink();

	CoutAndDeleteLink(L);//不断重复找出并且输出链表的最小值 再将该结点删除释放

	Linklist L = createlist();//非循环单链表

	int num = 0;
	printf("请输入你要查找的元素:");
	scanf("%d", &num);
	LocateAndRank(L,num);//在非循环链表中查找一个元素 并且每次按查找频度对链表排序

	int k = 0;
	printf("请输入你要查找倒数第几个元素:");
	scanf("%d", &k);
	int ret= FindBackcount(L, k);//找到了返回该元素值 没找到返回0
	printf("%d", ret);

	Linklist L1 = createlist();
	Linklist L2 = createlist();
	Linklist Lcom = createlist();
	Linklist p1 = L1->next;
	Linklist p2 = L2->next;
	while (p1->next)
		p1 = p1->next;
	while (p2->next)
		p2 = p2->next;

	p1->next = Lcom->next;
	p2->next = Lcom->next;//链接公共部分

	p1 = L1->next;
	p2 = L2->next;

	while (p1)
	{
		printf("%d", p1->data);
		p1 = p1->next;
	}
	printf("\n");
	while (p2)
	{
		printf("%d", p2->data);
		p2 = p2->next;
	}
	printf("\n");
	int comstart= FindComStart(L1, L2);//找到两个合并链表的公共部分起点
	printf("%d", comstart);

	DeleteRep(L);//删除链表中绝对值相等的元素 只保留第一个

	Linklist r = L;
	while (r->next)//尾指针r
	{
		r = r->next;
	}
	r->next = L->next->next->next->next->next;//人为设置一个环
	int ret = is_Loop(L);//判断一个链表是不是环 是环返回环入口的值 不是返回-1
	printf("%d", ret);

	CrossTheLink(L);//交叉链表 从a1 a2 a3 ... an-2 an-1 an 变成 a1 an a2 an-1...

	return 0;
}
