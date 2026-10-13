#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

/*
哈夫曼树
判断树：用于描述分类过程的二叉树
当判断的对象量足够大时，每一种判断树的效率是不一样的
当要找出效率最高的判断树，即哈夫曼树（最优二叉树）
*/

/*
基本概念：
路径：从一个结点到另一个结点之间的分支构成这两个结点的路径
结点的路径长度：两结点之间分支数
树的路径长度：从树根到每一个结点的路径长度之和 记作TL
              结点数目相同的二叉树中，完全二叉树是路径长度最短的二叉树
权：将树中结点赋给一个有着某种含义的数值，则这个数值称为该结点的权
结点的带权路径长度：路径长度与该节点的权的乘积 记作wl
树的带权路径长度：树中的所有叶子结点带权路径长度之和 记作WPL=Σwl
                  哈夫曼树（最优树）就是带权路径长度WPL最短的树 （WPL最短的比较是在结点相同条件下比较）

满二叉树不一定是哈夫曼树
具有带相同权值结点的哈夫曼树不一定唯一
*/

/*
哈夫曼树的构造算法
哈夫曼树中权越大的叶子离根越近
-》贪心算法：构造哈夫曼树时首先选择权支较小的叶子结点
哈夫曼算法：1.构造森林全是根
              n个给定权值的结点，构造成n个根节点的森林
            2.选用两小造新树
             选择两个权值最小的根结点作为左右子树构造新树，
             且新树的根节点权值为左右子树根节点权值之和
            3.删除两小添新树
             删除选择的两棵树，并在森林中加入新二叉树
            4.重复2，3剩单根
             不断重复2，3的操作，最后剩下的树就是哈夫曼树

特点：1.哈夫曼树的结点的度数（度即子树的个数）为0或2，没有度为1的结点
      2.包含n个叶子结点的哈夫曼树中共有2n-1个结点
      3.n棵树的森林要经过n-1次合并才能形成哈夫曼树，共产生n-1个新结点
*/

/*
哈夫曼算法实现：
采用顺序存储结构-》一维结构数组HuffmanTree H
一维数组的长度：2n （哈夫曼树共产生2n-1个结点，不使用0下标)

*/
typedef struct
{
    int weight;//权重
    int parent, lch, rch;//在数组中的下标位置
}HTNode,*HuffmanTree;//结点类型定义

/*
初始化：
1.构建数组
2.数组的每个结点的 lch,rch,parent都置为0
3.输入初始每个根节点的权重
*/
void CreatHuffmanTree(HuffmanTree HT, int n)
{
    if (n <= 1)
        return;
    int m = 2 * n - 1;//共有2*n-1个元素
    HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
    int i = 0;
    for (i = 1; i <= m; ++i)
    {
        HT[i].lch = 0;
        HT[i].rch = 0;
        HT[i].parent = 0;
    }
    for (i = 1; i <= n; ++i)
        scanf("%d", &HT[i].weight);

/*
进行n-1次合并，依次产生n-1个结点 下标从n+1->2n-1
1.在数组中选两个未选过(parent=0)的weight最小的两个结点HT[s1] HT[s2] s1,s2为下标值
2.修改HT[s1]和HT[s2]的parent
3.修改新树HT[i]的weight和lch,rch
*/
    int s1, s2;//weight最小的两个结点HT[s1] HT[s2] s1,s2为下标值
    for (i = n + 1; i <= m; i++)
    {
        Select(HT, i - 1, s1, s2);
        //Select函数 找出从下标1到i-1中未被挑选过的结点(parent=0)，weitght最小的两个s1,s2
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lch = s1; HT[i].rch = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}
/*
前n个结点的lch,rch以及2n-1(最后一个)结点的parent 都为0
*/


/*
编码中A-0,B-00,C-1,D-01 （频率高，长度短，节省空间）
则0000有三种可能AAAA ABA BB
因此要设计长度不等的编码，必须要使任一个字符的编码都不是另一个字符的编码的前缀
这种编码则成为前缀编码
*/
/*
哈夫曼编码
1.每个字符出现概率为权重
2.利用每个字符权重构造哈夫曼树，权重叶子离根近
3.哈夫曼树上左分支标0 右分支标1（形成前缀编码）
4.叶子路径就是编码

哈夫曼编码是最优！前缀！码
*/
void CreateHuffmanCode(HuffmanTree HT, HuffmanCode* HC, int n)//HuffmanCode是一个二维字符数组
{
    HC = malloc((n + 1) * sizeof(char*));//此数组存放n个编码的头指针
    char* cd = (char*)malloc(n * sizeof(char));//存放编码的数组
    cd[n - 1] = '\0';//编码结束标识符
    for (int i = 1; i <= n; ++i)
    {
        int start = n - 1;//编码存放在数组cd中倒着来
        int child = i;
        int par = HT[i].parent;
        while (par != 0)//当par=0时 则已经回到了根节点,表示此叶子结点路径标完了
        {
            --start;
            if (HT[par].lch == child)
                cd[start] = '0';//左分支为0
            else
                cd[start] = '1';//右分支为1

            child = par;
            par = HT[par].parent;//向上回溯
        }
        HC[i] = malloc((n - start) * sizeof(char));//为第i个字符串的编码数组分配空间
        strcpy(HC[i], &cd[start]);//将求得的编码从临时空间cd复制到HC的当前行去
    }
    free(cd);
}

/*
编码与解码
*/