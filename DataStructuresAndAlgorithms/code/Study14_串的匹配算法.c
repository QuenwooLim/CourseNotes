#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#define MAXLEN 255//串最大长度
typedef struct
{
	char ch[MAXLEN + 1];//第一个不用
	int length;//串当前长度
}SString;//顺序串

int Index_BF(SString S, SString T,int pos)//BF算法
{
	int i = pos;//从主串的某个位置开始查找
	int j = 1;
	while (i <= S.length && j <= T.length)
	{
		if (S.ch[i] == T.ch[i])
		{
			i++;
			j++;
		}
		else
		{
			i = i - j + 2;//主串回到到下一个   i-(j-1)+1 移动多少位就回去并且+1跳到下一个
			j = 1;//字串回溯到起始
		}
	}
	if (j >= T.length)
		return i - T.length;//返回匹配的第一个字符的下标
	else
		return 0;//模式匹配不成功
}
/*
设主串长n 字串长m 
最坏匹配情况：主串前面n-m个位置都比较了m次 还有最后一次配对 主串后m位1次
总次数 (n-m)*m+m=(n-m+1)*m 当m<<n时 算法复杂度为O(n*m)
*/

void get_next(SString T, int* next)//KMP算法找next值
{
	int i = 1;
	next[1] = 0;//首位的next值为0
	int j = 0;
	while (i < T.length)
	{
		if (j == 0 || T.ch[i] == T.ch[j])
		{
			++i;
			++j;
			next[i] = j;
		}
		else
			j = next[j];
	}
}
int Index_KMP(SString S, SString T, int pos,int* next)//KMP算法
{
	int i = pos;
	int j = 1;
	while (i < S.length && j < T.length)
	{
		if (j == 0 || S.ch[i] == T.ch[j])
		{
			i++;
			j++;
		}
		else
			j = next[j];  //i不变 j回溯
	}
	if (j > T.length)
		return i - T.length;//匹配成功
	else
		return 0;//匹配不成功
}
void get_nextval(SString T, int* nextval)//KMP求next值的算法优化
{
	int i = 1;
	nextval[1] = 0;
	int j = 0;
	while (i < T.length)
	{
		if (j == 0 || T.ch[i] == T.ch[j])
		{
			++i;
			++j;
			if (T.ch[i] != T.ch[j])
				nextval[i] = j;
			else
				nextval[i] = nextval[j];
		}
		else j = nextval[j];
	}
}