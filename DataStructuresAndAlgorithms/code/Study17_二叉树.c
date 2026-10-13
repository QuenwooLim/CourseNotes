#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#define TElemType char
#define Status int
typedef struct BiNode
{
    TElemType data;
    struct BiNode* lchild, * rchild;
}BiTNode, * BiTree;

Status CreateBiTree(BiTree T)//按照先序遍历创建二叉树
{
    char ch;
    scanf(&ch);
    if (ch == '#')
        T = NULL;
    else
    {
        if (!(T = (BiTNode*)malloc(sizeof(BiTNode))))
            return 0;
        T->data = ch;
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
    return 1;
}
/*
复制二叉树
如果是空树，递归结束
否则，申请新空间复制根结点
      递归复制左子树
      递归复制右子树
*/
int CopyBiTree(BiTree T, BiTree NewT)
{
    if (T == NULL)
    {
        NewT = NULL;
        return 0;
    }
    else
    {
        NewT = (BiTNode*)malloc(sizeof(BiTNode));
        NewT->data = T->data;
        CopyBiTree(T->lchild, NewT->lchild);
        CopyBiTree(T->rchild, NewT->rchild);
    }
}
/*
计算二叉树的深度
如果是空树，则深度为0
否则，递归计算左子树深度m
      递归计算右子树深度n
      二叉树的深度为m与n的较大则
*/
int DepthBiTree(BiTree T)
{
    if (T == NULL)
        return 0;
    else
        /*
        {
            m = DepthBiTree(T->lchild);
            n = DepthBiTree(T->rchild);
            if (m > n)
                return (m + 1);//返回时要+1(如叶子结点就是:0(空)+1)
            else
                return (n + 1);
        }
        */
        return (DepthBiTree(T->lchild) > DepthBiTree(T->rchild)) ? (DepthBiTree(T->lchild) + 1) : (DepthBiTree(T->rchild) + 1);
}
/*
计算二叉树结点总数
如果是空树，则结点个数为0
否则，结点个数为左子树的结点个数+右子树的结点个数
*/
int CountNode(BiTree T)
{
    if (T == NULL)
        return 0;
    else
        return CountNode(T->lchild) + CountNode(T->rchild) + 1;
}

/*
计算叶子结点的个数
如果是空树。则叶子结点个数为0
否则，叶子结点个数为左子树叶子结点数+右子树结点数
*/
int LeadCount(BiTree T)
{
    if (T == NULL)
        return 0;
    if (T->lchild == NULL & T->rchild == NULL)//叶子结点
        return 1;
    else
        return LeadCount(T->lchild) + LeadCount(T->rchild);
}

/*
线索二叉树，如果某个结点的左孩子为空，则将左孩子的指针域改为指向其前驱
            如果某个结点的右孩子为空，则将右孩子的指针域改为指向其后继
            （这里的前驱后继：在先序（或中序或后序）序列中结点的前驱后继
              eg:ABCDEF A没有前驱 F没有后继 D的左孩子若空指向C，右孩子若空指向E）
            这种改变指向的指针称为“线索”
这种改变的过程叫做“线索化”

为了区分lchild和rchild指针到底指向孩子还是前驱后继，则增设两个标志域ltag和rtag
并约定  ltag=0 lchild指向该结点的左孩子
        ltag=1 lchild指向该结点的前驱
        rtag=0 rchild指向该结点的右孩子
        rtag=1 rchild指向该结点的后继
*/
typedef struct BiThrNode
{
    int data;
    int ltag, rtag;//增加ltag和rtag
    struct BiThrNode* lchild, * rchild;
}BiThrNode,*BiThrTree;
/*
由于序列的第一个结点没有前驱和最后一个结点没有后继
增设头结点:ltag=0,lchild指向根结点,rtag=1,rchild指向序列的最后一个结点
序列的第一个结点前驱和最后一个结点后继指向头结点
*/