#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>

/*
* 栈与递归
递归工作栈：递归程序在执行时候需要系统提供栈来实现，递归程序运行期间使用的数据存储区
工作记录：实在参数，局部变量，返回地址

优点：结构清晰，程序易读
缺点：每次调用都会生成工作记录，保存状态信息，入栈；返回时出栈，恢复状态信息，时间开销大
优化：1.使用尾递归，单向递归->循环结构 2.自用栈模拟系统的运行时栈
*/

/*
* 栈和队列是限定插入和删除只能在表的端点进行的线性表
 
栈stack:后进先出的特性;只能在队尾插入和删除
Last In First Out的线性表 LIFO结构
适用问题：1.数据转换 2.表达式求值 3.括号匹配的检验 4.八皇后问题 
          5.型编辑程序 6.函数调用 7.迷宫求解 8.递归调用的实现

表尾也叫栈顶Top指向最后一个元素的下一个  表头也叫栈底Base指向首元素

插入元素到栈顶（表尾）的操作：入栈(压入)PUSH
从栈顶（表尾）删除元素的操作：出栈(弹出)POP

上溢：栈已经满了，还要压入元素 是一种错误
下溢：栈已经空了，还要弹出元素 是一种结束条件base==top

队列:先进先出 后进后出的特性；只能在队头删除和队尾插入
First In First Out系统
适用问题:1.脱机打印 2.多用户排队循环适用cpu和gpu 3.按用户优先级排队
         4.实时控制系统，信号按接受的先后顺序一次处理
         5.网络电文传输，按到达的时间先后顺序依次进行
队列Queuez 在表尾进行插入 在表头进行删除
表尾an称为队尾 表头a1称为队头
入队：插入元素
出队：删除元素
存储结构：链队和顺序队（常使用循环顺序队）

*/ 
#define SElemType int
#define MAXSIZE 100
typedef struct SqStack
{
    SElemType* base;//栈底指针
    SElemType* top;//栈顶指针 指向最后一个元素的下一个
    int stacksize;//栈的最大容量;
}SqStack;//顺序栈
int InitStack(SqStack* S)//初始化顺序栈
{
    S->base = (SElemType*)malloc(MAXSIZE * sizeof(SElemType));//开辟一个数组空间;
    if (!S->base)
        return 0;//开辟失败
    S->top = S->base;//栈顶指针等于栈底指针
    S->stacksize = MAXSIZE;
}
int StackLength(SqStack* S)//计算顺序栈长度
{
    return S->top - S->base;//同一数组的指针相减得到元素个数
}
int ClearStack(SqStack* S)//清空顺序栈
{
    if (S->base)//顺序栈存在
        S->top = S->base;
    return 1;
}
int DestoryStack(SqStack* S)
{
    if (S->base)
    {
        free(S->base);
        S->stacksize = 0;
        S->base = NULL;
        S->top = NULL;
    }
    return 1;
}
int Push(SqStack* S, SElemType e)
{
    if (S->top - S->base == S->stacksize)//栈满了 防止上溢
        return 0;
    *S->top++ = e;//压入 同时top后移
    return 1;
}
int Pop(SqStack* S, SElemType e)//用e来获取栈顶元素
{
    if (S->top == S->base)
        return 0;
    e = --S->top;//不需要释放原来的元素

    return e;
}

typedef struct StackNode
{
    SElemType data;
    struct StackNode* next;
}StackNode,*LinkStack;//链栈
void InitStack(LinkStack S)//初始化
{
    S = NULL;//头指针置空
}
void StackEmpty(LinkStack S)//判断是否为空
{
    if (S == NULL)
        return 1;
    else
        return 0;
}
int Push(LinkStack S, SElemType e)//入栈
{
    LinkStack p = (LinkStack)malloc(sizeof(LinkStack));
    p->data = e;
    p->next = S;
    S = p;
    return 1;
}
int Pop(LinkStack S, SElemType e)//压栈
{
    if (S == NULL)//空栈
        return 0;
    e = S->data;
    LinkStack p = S;//暂时保存用于释放
    S = S->next;
    free(p);
    return 1;
}
SElemType GetTop(LinkStack S)//得到栈顶元素
{
    if (S != NULL)
        return S->data;
}


#define MAXQSIZE 100//最大队列长度
#define QElemType int
typedef struct SqQueue
{
    QElemType* base;//初始化数组
    int front;//头指针 指向第一个元素
    int rear;//尾指针 指向最后一个元素的下一个
}*SqQueue;//顺序队
int InitQueue(SqQueue Q)//初始化
{
    Q->base = (SqQueue)malloc(sizeof(SqQueue) * MAXQSIZE);
    if (!Q->base)//分配失败
        return 0;
    Q->front = Q->rear = 0;//头指针和尾指针置空
    return 1;
}
int QueueLength(SqQueue Q)//计算长度
{
    return ((Q->rear - Q->front + MAXQSIZE) % MAXQSIZE);
}
int EnQueue(SqQueue Q, QElemType e)//入队e
{
    if ((Q->rear + 1) % MAXQSIZE == Q->front)
        return 0;//队满
    Q->base[Q->rear] = e;//直接插入最后
    Q->rear = (Q->rear + 1) % MAXQSIZE;//队尾指针+1 可以防止假溢出
}
int DeQueue(SqQueue Q, QElemType e)//出队e
{
    if (Q->front == Q->rear)//队空
        return 0;
    e = Q->base[Q->front];
    Q->front = (Q->front + 1) % MAXQSIZE;//队头指针+1 可以防止加溢出
    return 1;
}
QElemType GetHead(SqQueue Q, QElemType e)
{
    if (Q->front == Q->rear)//队空
        return 0;
    return Q->base[Q->front];
}

typedef struct Qnode
{
    QElemType data;
    struct Qnode* next;
}Qnode,*QueuePtr;
typedef struct
{
    QueuePtr front;//队头指针 下一个是首元结点
    QueuePtr rear;//队尾指针 最后一个元素
}LinkQueue;//链队
int InitQueue(LinkQueue* Q)//初始化
{
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QueuePtr));
    if (!Q->front)//内存分配失败
        return 0;
    Q->front->next = NULL;//头结点的下一个即首元节点置空
    return 1;
}
int DestoryQueue(LinkQueue* Q)//销毁
{
    while (Q->front)
    {
        QueuePtr p = Q->front->next;
        free(Q->front);
        Q->front = p;
    }
    return 1;
}
int EnQUeue(LinkQueue* Q, QElemType e)//入队
{
    QueuePtr p = (QueuePtr)malloc(sizeof(QueuePtr));
    if (!p)
        return 0;//开辟失败
    p->data = e;
    p->next = NULL;

    Q->rear->next = p;
    Q->rear = p;
}
typedef struct Qnode
{
    QElemType data;
    struct Qnode* next;
}Qnode, * QueuePtr;
typedef struct
{
    QueuePtr front;
    QueuePtr rear;
}LinkQueue;//链队
SElemType DeQueue(LinkQueue* Q, QElemType e)//出队
{
    if (Q->front == Q->rear)//空 
        return 0;
    QueuePtr p = (QueuePtr)malloc(sizeof(QueuePtr));
    p = Q->front->next;
    //QueuePtr p = Q->front->next;
    e = p->data;
    Q->front->next = Q->front->next->next;
    if (Q->rear == p)//删除了尾结点 头尾重置
        Q->rear = Q->front;
    free(p);
    return e;
}
SElemType GetHead(LinkQueue* Q)
{
    if (Q->front == Q->rear)
        return 0;
    return Q->front->next->data;
}