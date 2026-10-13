#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
/*
图的遍历：
深度优先探索(Depth_First Search--DFS)
广度优先探索(Breadth_First Search--BFS)
*/

/*
深度优先探索DFS
从起点开始，for依次访问邻接结点，先访问它的一个邻接结点，再访问这个邻接结点的邻接结点..递归循环
没有邻接结点或者邻接结点已经访问则返回
创建结点的辅助数组 visited[n] ;保证遍历过程每个结点只被访问一次
初始化数组每个元素值为0（表示未被访问），当结点被访问时则元素值为1
*/

/*
算法实现：邻接矩阵表示的图的深度优先探索遍历
*/
void DFS(AMGraph G, int v)//深度遍历G 遍历的起点是v
{
	printf("%d", v);
//  visited[n];//辅助数组 并且将数组的每个元素的值初始化为0

	visited[v] = 1;//第v个结点是遍历访问的起点 已访问 标记为1

	for (int w = 0; w < G.vexnum; w++)//一行一行扫描
	{
		if ((G.arcs[v][w] != 0) && (visited[w] == 0))//条件:有边 并且 还没被访问
		DFS(G, w);//递归调用
	}
}
/*
邻接矩阵
时间复杂度O(n^2)
遍历图中的每一个顶点都要从头扫描一个顶点的所在行

邻接表
时间复杂度O(n+e)
有n个头结点和2e个表结点，但只需扫描n个头结点+e个表结点
*/

/*
深度优先搜索BFS
从一个起点出发，访问它的所有邻接结点，再访问邻接结点的所有邻接结点...递归
利用辅助数组
利用辅助队列：每一层深度遍历时 将邻接结点入队
              第二层访问时访问这些结点的没访问过的邻接结点并入队，同时上一层访问过的出队
		      最后若队为空则遍历结束
*/
void BFS(Graph G, int v)//深度遍历G 起点是v
{
	printf("%d", v);
	visited[v] = 1;//辅助数组
	initQuene(G);//辅助队列
	EnQueue(Q, v);//第一遍遍历时 Q结点进队
	while (!QueueEmpty(Q))//队列非空 队列为空时表示全部遍历完了
	{
		DeQueue(Q, u);//队头元素出队 并将每一个出队的结点置为u
		for (w = FirstAdjVex(G, u); w >= 0; w = NextAdjVex(G, u, w))//w是u的邻接结点
			if (!visited[w])//w没被访问过
			{
				printf("%d", w);//w出队
				visited[w] = 1;//标记已访问
				EnQueue(Q, w);//w入队
			}
	}
}
/*
邻接矩阵
时间复杂度O(n^2)
依次访问矩阵中的每一行

邻接表
时间复杂度O(n+e)
有n个头结点和2e个表结点，但只需扫描n个头结点+e个表结点
*/

/*
DFS和BFS算法效率
空间复杂度都相同O(n)
时间复杂度取决于存储结构，和搜索路径无关
*/