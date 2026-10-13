#include<stdio.h>
/*
2.1
线性表的定义与特点:
a1,a2,a3,a4,a5,...an
线性表是具有相同特征的数据元素的一个有限序列
a1线性起点(起始结点) 与 a2线性终点(终端结点)
前趋与后继 起点没有前趋 终点没有后继
n为表的长度 n=0时为空表
*/

/*
2.2
线性表的案例引入
Pn(x)=p0+p1*x+p2*x^2+...pn*x^n
吧幂指数作为下标i 0,1,2,3...n
系数作为结点 p0,p1,p2,...pn
*/
//例子1 计算Pn(x)+Qn(x)=Tn(x)
int main()
{
	int arr1[10] = { 7,8,9,4,5,6,1,2,3,0 };//Pn(x)
	int arr2[10] = { 0,1,4,7,2,5,8,3,6,9 };//Qn(x)
	int arr3[10];
	for (int i = 0; i < 10; i++)
	{
		arr3[i] = arr1[i] + arr2[i];
		printf("%dx^%d + ", arr3[i], i);
		if (i == 9)
		{
			printf("%dx^%d=Tn(x)",arr3[i],i);
		}
	}
	return 0;
}
//例子2 稀疏多项式
//Pn(x)=1+10x^10000+20x^20000
//可以创建数组时默认其他项的系数为0

//例子3 信息管理系统

/*
2.3
线性表的类型定义
ADT List
{
    数据对象 D
	数据关系 R
	基本操作:
	          InitList;
			  DestoryList;
			  InsertList;
			  DeleteList;
			  ...
}
*/

/*
2.4
线性表的顺序存储表示与实现
顺序存储结构或者顺序映像 :数据元素关系逻辑上相邻转化为物理上(内存空间)相邻
*/



#define MAXSIZE 100
#define INFEASIBLE -1
#define OVERFLOW -2
typedef char ElemType;

typedef struct
{
	ElemType* elem;//数组
	int length;
}SqList;//定义顺序表类型

int InitList(SqList* L)//初始化线性表
{
	L->elem = malloc(sizeof(ElemType) * MAXSIZE);
	if (!L->elem)//分配失败时L->elem为空
	{
		return OVERFLOW;//溢出返回值
	}
	L->length = 0;
	return 1;
}

void Destory(SqList* L)//销毁线性表
{
	if (L->elem)
		free(L->elem);
}

void ClearList(SqList* L)//清空线性表
{
	L->length = 0;
}

int GetLength(SqList L)//求线性表L长度
{
	return L.length;
}

int is_Empty(SqList L)//判断线性表是否为空
{
	if (L.length == 0)
		return 1;//为空
	else
		return 0;
}

int GetElem(SqList L,int i, ElemType* ret)//得到线性表中的元素
{
	if (i<1 && i>L.length)
	{
		return 0;
	}
	ret = L.elem[i-1]; 
}

int LocateElem(SqList L, ElemType e)//查找对应元素的序号(从1开始)
{
	for (int i = 0; i < L.length; i++)
	{
		if (L.elem[i] == e)
		{
			return (i+1);//查找成功返回序号
		}
	}
	return 0;
}
//平均查找长度ASL（数学期望）:length/2

int InsertList(SqList* L, int i, ElemType e)//顺序表插入新元素
{
	if (i<1 && i>L->length)
	{
		return 0;
	}
	if (L->length == MAXSIZE)
	{
		return OVERFLOW;
	}
	for (int k = L->length - 1; k >= i-1; k--)//插入位置之后的元素往后挪位置
	{
		L->elem[k+1] = L->elem[k];
	}
	L->elem[i - 1] = e;
	L->length = L->length + 1;//更新长度
	return 1;
} 
//耗费时间长度1/(n+1)  *  (n*(n+1)/2) = n/2  时间复杂度 O(n)

int DeleteList(SqList* L, int i, ElemType* e)//顺序表删除一个元素
{
	if (i<1 && i>L->length)
	{
		return 0;
	}
	for (int k = i-1; k < L->length - 1; k++)
	{
		L->elem[k] = L->elem[k + 1];
	}
	L->length = L->length - 1;
	return 1;
 //耗费时间长度  ( (n-1)*n/2 )/n 时间复杂度 O(n)

//上述函数的空间复杂度都是1 (没有使用辅助空间)
/*
优点与缺点
优点:1.存储密度大(结点本身所占存储量/结点结构所占存储量 为1/1)
     2.可以随机存取表中任一个元素
缺点:1.在插入，深处某一个元素时候，需要移动大量元素 (时间效率低)
     2.浪费存储空间
	 3.属于静态存储形式(数组)，数据元素的个数不能自由扩充
*/