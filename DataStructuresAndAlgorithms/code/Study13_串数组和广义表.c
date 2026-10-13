#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

/*
栈和队列：操作受限的线性表，是线性结构
串数组和广义表：内容受限（如字符）的线性表，是线性结构的推广
*/
/*
串：零个或多个任意字符组成的有序序列
  S="a1 a2 a3 a4...an"
  串名S  串长 a1 a2 a3 ...an  串长n

字串：任意个连续字符组成的子序列（含空串和自己） 真字串不包含自己
  "abcde":字串"","a","b","abcdef"

主串：包含字串的串

字符位置：字符在序列中的序号

字串位置：字串第一个字符位置

空格穿：由一个或多个空格组成的串 与空串不同（空格算是一个字符）

串相等:"abc"="abc" 所有空串都相等
*/

/*
串的链式存储结构
优点：操作方便
缺点：存储密度低
  ->因此可以将多个字符存放在一个结点中 提高存储密度
*/
#define MAXLEN 255//串最大长度
typedef struct
{
	char ch[MAXLEN + 1];//第一个不用
	int length;//串当前长度
}SSting;//顺序串


#define CHUNKSIZE 80//块的大小（一个块存放80个字符）
typedef struct Chunk
{
	char ch[CHUNKSIZE];
	struct Chunk* next;
}Chunk;
typedef struct
{
	Chunk* head;//头指针
	Chunk* tail;//尾指针
	int curlen;//字符串当前长度
}LString;//块链结构

/*
串的模式匹配算法
算法目的：确定主串中所含字串（模式串）第一次出现的位置（定位）
算法应用：搜索引擎，拼写检查，语法翻译，数据压缩
算法种类：1.BF算法(BRUTE-FORCE) 2.KMP算法
*/