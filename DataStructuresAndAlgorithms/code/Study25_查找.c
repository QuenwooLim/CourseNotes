#define _CRT_SECURE_NO_WARNINGS

/*
关键字：用来标识一个数据元素的某个数据项的值
主关键字：可唯一地标识一个记录 （如学号
次关键字：标识若干记录 （如重复的姓名

查找操作的目的：
1.查询某个数据元素是否在查找表中
2.检索某个数据元素的各种属性
3.在查找表中插入一个数据元素
4.删除查找表中的某个数据元素

静态查找表：只做查找操作
动态查找表：做插入和删除操作

评价指标：关键字的平均比较次数，也称平均查找长度ASL
     n
ASL= Σ pc  (比较次数的期望值)
    i=1
n记录个数 p记录概率 c记录次数
*/

/*
顺序查找表定义
*/
#define KeyType char
typedef struct
{
    KeyType key;//关键字域
    //其他域
}ElemType;//表中结点的类型
typedef struct
{
    ElemType* R;//表基地址
    int length;//表长
}SSTable;//Sequential Search Table顺序查找表
SSTable ST;//定义顺序表ST

int Search_Seq(SSTable ST, KeyType key)
{
    int i = 0;
    for (i = ST.length; i >= 1; --i)//在数组中从后开始查找
        if (ST.R[i].key == key)//判断表中有无要查找的关键字
            return i;//查找到了返回下标的值
    return 0;
}//需要比较两次：1.i>=1 2.ST.R[i].key == key
//优化算法 提高时间效率
int Search_Seq2(SSTable ST, KeyType key)
{
    ST.R[0].key = key;//将待查关键字存入表头(哨兵，监视哨)
    int i = 0;
    for (i = ST.length;; --i)
        if (ST.R[i].key == key)
            return i;
    //当在表中查找不到时，返回表头的下标0
}//只需要比较一次 查找时间减半
/*
时间复杂度 O(n)
空间复杂度 O(1)
ASL=(1+2+3...+n)/n=(n+1)/2
*/

/*
*提高时间效率:
记录每一个关键字的查找频率 访问频度域
数组保持频度非递增有序排列
在数组从后开始查找 频度高的先查找
*/

/*
折半查找：每次将待查记录所在区间缩小一半
表长n
区间下界low
区间上界high
区间中点mid
*/
int Search_Bin(SSTable ST, KeyType key)
{
    int low = 1;
    int high = ST.length;
    int mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (ST.R[mid].key == key)
            return mid;
        else if (key < ST.R[mid].key)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;//low>high
}

//折半查找的递归算法
int Search_Bin(SSTable ST, KeyType key, int low, int high)
{
    if (low > high)
        return 0;
    int mid = (low + high);
    if (key == ST.R[mid].key)
        return mid;
    else if (key > ST.R[mid].key)
        return Search_Bin(ST, key, mid + 1, high);
    else
        return Search_Bin(ST, key, low, mid - 1);
}
/*
将折半查找的性能分析成一棵判定树
比较次数=路径上的结点数=结点的层数
比较一次就查找到的在第一层
比较两次就查找到的在第二层
...

每个结点的比较次数都小于等于树的深度（最后一层结点等于成立）
比较次数<=树的深度=[lgn/lg2]+1  n为结点数
*/
/*
表长2^h-1 h为满二叉树的深度
则每个记录查找概率相等1/n
j表示二叉树的第几层 第j层有2^(j-1)个结点 每个结点需要比较j次
      h  
ASL=( Σj*2^(j-1) )/n 
     j=1

优点：效率更高
缺点：只能适用于顺序存储结构的有序表，对线性链表无效
*/


/*
分块查找（索引顺序查找）
1.将表分成几块，表 有序或者分块有序(块内可以是无序的)
  若i块<j块，则i块所有关键字小于j块关键字
2.建立索引表 每个结点含有最大关键字和指向本块第一个结点的指针


查找：先二分查找到对应的块，再顺序查找
ASL=ASL索引表+ASL块内=(lg(n/s+1))/lg2 + s/2

优点：插入和删除比较方便，无需大量李东元素
缺点：需要增加一个索引表的储存空间并对初始索引表进行排序运算
适用：既要快速查找，又要动态变化
*/