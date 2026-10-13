#define _CRT_SECURE_NO_WARNINGS

/*
图的逻辑结构是多对多的
图没有顺序结构，但可以利用二维数组表示元素之间的关系

存储结构：
  数组表示法：邻接矩阵√
  多重链表的链式存储：1.邻接表√
                      2.邻接多重表
                      3.十字链表
*/

/*
二维数组（邻接矩阵）表示法
建立一个一维的顶点表（记录各个顶点的信息）
和一个二维的邻接矩阵（表示各个顶点之间的关系）

设图A=（V，E）有n个顶点
  顶点表Vexs[n] 存储顶点的数据
  邻接矩阵A.arcs[n][n]: if( (i,j)∈E ) A.arcs[i][j]=1
                        else           A.arcs[i][j]=0

无向图的邻接矩阵：1.是对称矩阵
                  2.顶点i的度=第i行或者i列中值为1的个数
                  3.对角线元素为0，完全图的邻接矩阵除了对角元素其他都为1
有向图的邻接矩阵：1.第i行定义：以结点vi为尾的弧（出度边）
                  2.第j列定义：以结点vi为头的弧（入度边）
                 （出度看行 入度看列）
网（有权的图）的邻接矩阵：if( (i,j)∈E || <i,j>∈E ) A.arcs[i][j]=weight
                          else                       A.arcs[i][j]=∞
*/

#define VerTexType char  //设定顶点的数据类型为字符型
#define ArcType int  //设边的类型为整形（图的边类型是0和1 网的边类型是权值）
#define MVNum 100  //最大顶点数
#define Maxlnt 32767 //定义网的边的权值为极大值

typedef struct
{
    VerTexType vexs[MVNum];//顶点表
    ArcType arcs[MVNum][MVNum];//邻接矩阵 数组的类型是边的类型
    int vexnum, arcnum;//图当前点数和边数
}AMGraph;//Adjacency Matrix Graph

void CreateUDN(AMGraph* G)//创建无向网
{
    printf("请输入总顶点个数，总边的个数\n");
    scanf("%d %d", &G->vexnum, &G->arcnum);

    int i = 0;
    int j = 0;
    printf("请依次输入各顶点的数据\n");
    for (i = 0; i < G->vexnum; ++i)
        scanf("%d", &G->vexs[i]);//构造顶点表

    for (i = 0; i < G->vexnum; ++i)
        for (j = 0; j < G->vexnum; ++j)
            G->arcs[i][j] = Maxlnt;//边的权值均初始化为极大值

    for (i = 0; i < G->arcnum; ++i)
    {
        printf("请输入每一条边所依附的顶点以及边的权值\n");
        char v1, v2;
        int weight;
        scanf("%c %c %d", &v1, &v2, &weight);

        int m = LocateVex(G, v1);
        int n = LocateVex(G, v2);//查找v1和v2在G中的位置
        G->arcs[i][j] = weight;//设置权值
        G->arcs[j][i] = G->arcs[i][j];//无向图为对称矩阵
    }
    return 1;
}
int LocateVex(AMGraph* G, VerTexType u)
{
    int i = 0;
    for (i = 0; i < G->vexnum; ++i)
        if (u == G->vexs[i])
            return i;//找到下标
    return -1;
}

/*
利用无向网存储创建无向图和有向网
无向网->无向图：1.初始化邻接矩阵时，w=0
                2.构造邻接矩阵时，w=1
无向网->有向网:1.邻接矩阵是非对称矩阵，出度看行，入度看列
               2.只需要为G.arcs[i][j]赋权，不需要为G.arcs[i][j]赋权
*/

/*
邻接矩阵的缺点：
1.不利于增加和删除顶点
2.如果图的边很少的话，会浪费大量的空间
3.统计稀疏图中一共有多少条边时浪费时间
*/