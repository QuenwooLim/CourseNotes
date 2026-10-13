#include<stdio.h>
#include<stdlib.h>
/*
循环指针:最后一个元素的指针域和头指针相同 指向头节点
循环链表没有NULL 判断结束时看指针是否等于头指针

时间复杂度：利用头指针找元素:a1 O(1) , 找an O(n)
            利用尾指针找元素:a1 O(1) , 找an O(1)
使用循环链表时候多利用尾指针
*/

//合并带尾指针的非空单循环链表Ta,Tb;合并后Tb在Ta后
#define ElemType float
typedef struct Lnode
{
    ElemType data;
    Lnode* next;
}Lnode,* Linklist;

Linklist Connect(Linklist Ta, Linklist Tb)
{
    Linklist p = Ta->next;//用p存Ta头节点  Ta表示尾部指针 Ta->next时头指针 再->next首元素
    Ta->next= Tb->next->next;
    free(Tb->next);//释放Tb表头结点
    Tb->next = p;
    return Tb;//此时Tb表示合并链表的尾指针
}
int main()
{
    Linklist Ta;
    Linklist Tb;
    Connect(Ta, Tb);
    return 0;
}

//双向链表
//一个元素包含三个部分:数据域data 指向前赴结点指针prior 指向后继结点指针next
//结构对称性 p表示某一结点  p->prior->next==p==p->next->prior
//双向循环链表
//头节点的prior指向最后一个结点data 最后一个结点的next指向头节点的prior
//双向链表结构体定义
#define Elemtype float
typedef struct DuLnode
{
    Elemtype data;
    DuLnode* prior;
    DuLnode* next;
}DuLnode,*Dulinklist;
void ListInsert(Dulinklist L, int i, ElemType e)//在带头节点的双向循环链表L中第i个位置插入元素e
{
    DuLnode* p;//待插入的位置的元素结点保留
    DuLnode* s;//插入的结点
    if (!(p = GetElem(L, i)))//没有该待插入的位置
        return 0;
    s = (DuLnode*)malloc(sizeof(DuLnode));
    s->data = e;
    s->prior = p->prior;
    p->prior->next = s;
    s->next = p;
    p->prior = s;
    return 1;
}
void ListDelete(Dulinklist L, int i, ElemType* e)//删除带头节点的双向循环链表L的第i个元素 并且用e返回
{
    DuLnode* p;//待插入的位置的元素结点保留
    if (!(p = GetElem(L, i)))//没有该待插入的位置
        return 0;
    e = &p->data;
    p->prior->next = p->next;
    p->next->prior = p->prior;
    free(p);
    return 1;
}

/*
顺序表和链表的比较
链表存储结构的优点：1.结点空间可以动态申请和释放；2.插入和删除结点时不需要移动数据元素
链表存储结构的缺点：1.存储密度小，指针需要占用额外存储空间；2.非随机存取，必须从头尾节点开始找
*/

//例 线性表 合并集合 La={7,5,3,11} Lb={2,6,3} 合并成一个没有重复元素的新集合
void Listunion(Linklist La, Linklist Lb)
{
    int La_len = ListLength(La);
    int la_len = Listlength(Lb);
    for (int i = 1; i < La_len; i++)
    {
        ElemType e;
        GetElem(Lb, i, e);
        if (!locateElem(La, e))
            Listinsert(La, ++La_len, e);
    }
}//时间复杂度O(La_len)*O(Lb_len)

//有序表的合并-用顺序表实现 两个非递减的有序表合并为一个新的非递减有序表
typedef struct Sqlist
{
    int* elem;//数组
    int length;
}Sqlist;
void MergeList_Sq(Sqlist La, Sqlist Lb, Sqlist* Lc)
{
    int* pa = La.elem;
    int* pb = Lb.elem;//pa pb指向两个表的首元素
    Lc->length = La.length + Lb.length;//新表长度
    Lc->elem = malloc(sizeof(Lc->length));//开辟新表空间
    int* pc = Lc->elem;//pc指向新表的第一个元素
    int* pa_last = La.elem + La.length - 1;
    int* pb_last = Lb.elem + Lb.length - 1;//pa_last pb_last指向两个表的最后一个元素
    while (pa <= pa_last && pb <= pb_last)//两个表非空 依次摘取较小结点放入新表
    {
        if (*pa <= *pb)
            *pc++ = *pa++;
        else
            *pc++ = *pb++;
    }
    while (pa <= pa_last)
        *pc++ = *pa++;
    while (pb <= pb_last)
        *pc++ = *pb++;
}//时间复杂度和空间复杂度O(Listlength(La)+Listlength(Lb)

//有序表合并-用链表实现
void MergeList_L(Linklist La, Linklist Lb, Linklist Lc)
{
    Linklist pa = La->next;
    Linklist pb = Lb->next;
    Linklist pc = pa;
    Lc = pa;//用La的头节点作为Lc的头节点
    while (pa && pb)
    {
        if (pa->data <= pb->data)
        {
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }
        else
        {
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }
    while(pa || pb)
    pc->next = pa ? pa : pb;//剩余部分

    free(Lb);
}//空间复杂度O(1) 时间复杂度O( Listlength(La)+Listlength(Lb) )