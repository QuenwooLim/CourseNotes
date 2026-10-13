#include<stdio.h>
#include <stdlib.h>
/*
线性表
链表：数据域 指针域（后继节点的存储地址）

单链表只有一个指针域 是由头指针唯一确定，因此单链表可以用头指针的名字来命名
双链表由两个指针域 一个前赴 一个后继
首尾相接的链表成为循环链表

头指针：指向链表中第一个结点的指针
首元结点：链表中存储第一个数据元素a1的接结点
头结点：链表的首元结点之前附设的一个结点，数据域可以任意，不算入链表长度

无头节点时，头指针为空表示空表
有头节点时，头节点的指针域为空时表示空表

链表特点：1.结点在存储器的位置是任意的
          2.访问时只能通过头指针进入链表，并通过每个结点的指针域依次向后
*/

//使用结构体来包含 数据域和指针域两部分

//例1
#define Elemtype float
typedef struct Lnode
{
    Elemtype data;//数据域
    struct Lnode* next;//指针域
}Lnode,*Linklist;//类型  定义链表 LinkList L  定义结点指针 Lnode *p



//例2
typedef struct
{
    char num[8];
    char name[8];
    int score;
}ElemType;//将数据域里的东西先包含到一个结构体中
typedef struct Lnode
{
    ElemType data;
    struct Lnode* next;
}Lnode,*Linklist;


#define ElemType float
typedef struct Lnode
{
    ElemType data;//数据域
    struct Lnode* next;//指针域
}Lnode,*Linklist;//类型  

int initlist_l(Linklist L)//单链表初始化
{
    L = (Linklist)malloc(sizeof(Lnode));
    L->next = NULL;
    return 1;
}

int is_listempty(Linklist L)
{
    if (L->next)//非空
        return 0;
    else
        return 1;
}
int destorylist(Linklist L)
{
    while (L)//L非空
    {
        Linklist p = L;
        L = L->next;//让L指向下一节点 此时第一个节点就找不到了
        free(p);
    }
    return 1;
}

int clearlist(Linklist L)//保留头指针和头结点
{
    Linklist p, q;
    p = L->next;
    while (p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;//头结点指针域为空
    return 1;
}

int countlength(Linklist L)//计算表长
{
    Linklist p;
    p = L->next;
    int i = 0;
    while (p)
    {
        i++;
        p = p->next;
    }
    return i;
}

int getelem(Linklist L, int i, ElemType* e)//获取链表中的第i个元素内容 通过变量e返回
{
    Linklist p = L->next;
    int j = 1;//计数
    while (p && j < i)//表示p非空 且还没找到对应下标
    {
        p = p->next;
        j++;
    }
    if (!p || j > i)//地址为空 或者 超过对应下标
    {
        return 0;
    }
    e = &p->data;
    return 1;
}

Linklist findelem_p(Linklist L,ElemType e)//按值查找，根据指定数据获取该数据所在的位置,找到的时候返回地址
{
    Linklist p= L->next;
    while (p && p->data != e)
    {
        p = p->next;
    }
    return p;//找不到时返回空 找到时返回该值的地址
}
int findelem_n(Linklist L, ElemType e)//按值查找 返回元素对应下标
{
    Linklist p = L->next;
    int j = 1;
    while (p && p->data != e)
    {
        p = p->next;
        j++;
    }
    if (p)//表示找到了
        return j;
    else//表示没找到
        return 0;
}
//查找时要从头指针找起 时间复杂度O(n)

int listinsert(Linklist L, int i, ElemType e)//在第i个位置插入e
{
    e = 1.1f;
    Linklist p = L;
    int j = 0;
    while (p && j < i - 1)//找i-1的位置
    {
        p = p->next;
        ++j;
    }
    if (!p || j > i - 1)//找不到
        return 0;
    Linklist s = 0;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return 1;
}
//不需要移动元素 只需要修改指针 时间复杂度O(1)

int deleteleem(Linklist L, int i)//删除一个元素
{
    Linklist p = L;
    int j = 0;
    while (p && j < i - 1)//找i-1的位置
    {
        p = p->next;
        ++j;
    }
    if (!p || j > i - 1)//找不到
        return 0;
    Linklist q = p->next;
    p->next = p->next->next;
    free(q);
    return 1;
}
//时间复杂度O(1)

//头插法 创建一个有n个元素的链表
void CreatList_H(Linklist L, int n)
{
    L = (Linklist)malloc(sizeof(Linklist));
    L->next = NULL;//先建立一个带头结点的单链表
    for (n; n > 0; n--)
    {
        Linklist p = (Linklist)malloc(sizeof(Linklist));
        scanf("%f", &p->data);

        p->next = L->next;//将新元素插入到表头
        L->next = p;
    }
}//时间复杂度O(n)

//尾插法 
void CreateList_R(Linklist L, int n)
{
    L = (Linklist)malloc(sizeof(Linklist));
    L->next = NULL;
    Linklist r = L;//尾指针
    for (n; n > 0; n--)
    {
        Linklist p = (Linklist)malloc(sizeof(Linklist));
        scanf("%d", &p->data);
        p->next = NULL;
        r->next = p;//将新元素插入到表尾
        r = p;//改变尾指针
    }   
}//时间复杂度O(n)

int main()
{
    Linklist L;
    int n;
    CreatList_H(L, n);

    return 0;
}