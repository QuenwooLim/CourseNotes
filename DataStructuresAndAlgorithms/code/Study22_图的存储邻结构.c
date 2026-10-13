#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

/*
邻接表的表示法（链式）
头节点：data(数据域)+firstarc(结点的一条边)
表结点：adjvex(连接的结点)+nextarc(指向结点另一条边)+info(存储权值等信息)
没有后续的边则指向空

顶点：按编号顺序将顶点数据存储在一维数组中
关联同一顶点的边(或出度边))：用线性链表存储
*/

/*
邻接表
在无向图中：1.邻接表不唯一，表结点可以互换位置
            2.一个结点有n条边（即n个度） 该顶点就有一个头节点+n个表结点
            3.若无向图有n个顶点 e条边 则需要n个头结点和2e个表结点（适合用于稀疏图）

在有向图中：1.顶点的出度个数就是表结点个数
            2.若有向图有n个顶点 e条边 则需要n个头结点和e个表结点
            3.计算出度只需要看表结点 计算入度需要遍历邻接表
            （也可同理用表存储表结点为入度，则称逆邻接表）
邻接表也不便表示任意一对顶点之间是否有边
*/

/*
当邻接表的存储结构形成后，图便唯一确定
*/

#define VerTexType char
#define MVNum 100
typedef struct ArcNode//
{
    int adjvex;//该边指向的结点的位置下标
    struct ArcNode* nextarc;//指向另一条依附顶点的边
    //Otherinfo inof;//和边相关信息
}ArcNode;//定义边结点（表结点）
typedef struct VNode
{
    VerTexType data;//顶点信息
    ArcNode* firstarc;//指向第一条依附该顶点的边的指针
}Vnode,AdjList[MVNum];//定义头结点，邻接表数组
typedef struct
{
    AdjList vertices;//邻接表数组结构
    int vexnum, arcnum;//顶点总数和边总数
}ALGraph;
int main()
{
    ALGraph G;
    G.vexnum = 5;
    G.arcnum = 5;
    G.vertices[1].data = 'b';
    ArcNode* p = G.vertices[1].firstarc;//p是下标为1结点的第一条边指向的表结点
    printf("%d", p->adjvex);//这一条边指向的结点在数组位置下标

    return 0;
}

/*
算法实现：
1.输入总顶点数目和总边数目
2.建立顶点表：1.依次输入头结点的数据域
              2.头结点的指针域初始化为NULL
3.创建邻接表：1.依次输入每条边依附的两个顶点
              2.确定两个顶点的下标i和j，建立新边结点p
              3.将此边结点插入v j对应头结点的链表的头部(头插法)
*/

void CreateUDG(ALGraph* G)//创建无向图G
{
    printf("输入总顶点数和总边数\n");
    scanf("%d %d", &G->vexnum, &G->arcnum);

    int i = 0;
    for (i = 0; i < G->vexnum; ++i)
    {
        printf("输入第%d个结点的数据", i);
        scanf("%c", &G->vertices[i].data);
        G->vertices[i].firstarc = NULL;
    }//头结点

    int k = 0;
    char v1, v2;
    for (k = 0; k < G->arcnum; ++k)
    {
        printf("请输入两个顶点的值\n");
        scanf("%c %c", &v1, &v2);
        int m = LocateVex(G, v1);
        int n = LocateVex(G, v2);//找出两个顶点在数组中的下标位置

        ArcNode* p1 =(ArcNode*)malloc(sizeof(ArcNode));
        p1->adjvex = n;
        p1->nextarc = G->vertices[m].firstarc;
        G->vertices[m].firstarc = p1;//在v1头结点后面接入边结点

        ArcNode* p2 = (ArcNode*)malloc(sizeof(ArcNode));
        p2->adjvex = m;
        p1->nextarc = G->vertices[n].firstarc;
        G->vertices[n].firstarc = p2;//在v2头结点后面接入边结点
        //若有向图则不需要这里的第二次链表插入 只考虑出度一次就可以
    }//边结点在顶点链的插入
}

/*
邻接矩阵与邻接表之间的区别
1.对于任一个确定的无向图，邻接矩阵是唯一的（行列号与顶点编号一致）
  邻接表不唯一，链接次序与顶点编号无关
2.邻接矩阵的空间复杂度为O(n^2),邻接表的空间复杂度为O(n+e)
3.邻接矩阵多用于密集图，邻接矩阵多用于稀疏图
*/

/*
邻接表的缺点
有向图：求结点的度困难->使用十字链表
无向图：每条边都要存储两遍，（如删除等）操作不方便->使用多重表

有向图的十字链表：
1.顶点结点：数据域data+（指向第一个弧）入度指针域firstin+（指向第一个弧）出度指针域
2.弧结点：尾顶点tailvex+头顶点headvex+（指向下一个依附于同一头顶点的弧）指针域hlink+（指向下一个依附于同一尾顶点的弧）指针域tlink

无向图的多重表：
1.顶点结点：数据域data+指向第一条依附于该顶点的边firstedge
2.边结点：该边依附的两个顶点在表头数组中位置下标ivex(i结点)+jvex(v结点)
          +(指向依附于i结点的下一条边结点)指针域ilink
          +(指向依附于j结点的下一条边结点)指针域jlink
          +(标记该边界点是否被搜查过)标志域data
          +(记录边的(如权等)信息)数据域info
*/
