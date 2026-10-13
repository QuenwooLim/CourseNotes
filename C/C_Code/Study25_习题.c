#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

/*
高精度算法
数组模拟高精度算法
*/

//高精度加法
int main()
{
	//a,b<10^500
	char s1[505], s2[505];//接收的数组
	int a[505], b[505], c[505] = {0};//计算的数组
	int la, lb, lc;//长度 表位数
	
	scanf("%s", s1);
	scanf("%s", s2);//把大数字当作一个字符串

	la = strlen(s1);
	lb = strlen(s2);//位数

	for (int i = 0; i < la; i++)
		a[la - i] = s1[i] - '0';//将字符转换成数字，并且将字符转置便于计算
	for (int i = 0; i < lb; i++)
		b[lb - i] = s2[i] - '0';

	lc = (la > lb ? la : lb) + 1;
	for (int i = 1; i <= lc; i++)
	{
		c[i] = a[i] + b[i] + c[i];
		c[i + 1] = c[i] / 10;//进位
		c[i] = c[i] % 10;
	}

	if (c[lc] == 0 && lc > 0)//lc>0 防止0+0=0 保留至少一个0
		lc--;//最后一个没有进位 删除前导0 
	for (int i = lc; i > 0; i--)
		printf("%d", c[i]);//从后往前输入

	return 0;
}



//高精度减法
int compare(char s1[], char s2[])
{
	int l1 = strlen(s1);
	int l2 = strlen(s2);
	if (l1 != l2)//长度不一样
		return l1 > l2;//s1大则返回1 s1小则返回0
	else//长度一样
	{
		for (int i = 0; i < l1; i++)
			if (s1[i] != s2[i])
				return s1[i] > s2[i];
	}
	return 1;//两个数相等
}
int main()
{
	char s1[10000], s2[10000], s3[10000];
	int a[10000] = { 0 }, b[10000] = {0}, c[10000] = { 0 };
	int flag = 0;//判断符号正负

	int la, lb, lc;
	scanf("%s", s1);
	scanf("%s", s2);
	if (!compare(s1, s2))//如果s1<s2 接收到0 交换两个数
	{
		flag = 1;//表示负数
		strcpy(s3, s1);
		strcpy(s1, s2);
		strcpy(s2, s3);
	}//此时s1>=s2

	la = strlen(s1);
	lb = strlen(s2);
	for (int i = 0; i < la; i++)
		a[la - i] = s1[i] - '0';
	for (int i = 0; i < lb; i++)
		b[lb - i] = s2[i] - '0';

	lc = (la > lb ? la : lb);
	for (int i = 1; i <= lc; i++)
	{
		if (a[i] < b[i])
		{
			a[i + 1]--;//高位借一
			a[i] += 10;
		}
		c[i] = a[i] - b[i];
	}
	while (c[lc] == 0 && lc > 1)
		lc--;
	if (flag)
		printf("-");
	for (int i = lc; i > 0; i--)
		printf("%d", c[i]);

	return 0;
}

高精度乘法

int main()
{
	char s1[1000], s2[1000];
	int a[1000] = { 0 }, b[1000] = { 0 }, c[1000] = { 0 };
	int la, lb, lc;
	scanf("%s", s1);
	scanf("%s", s2);

	la = strlen(s1);
	lb = strlen(s2);
	for (int i = 0; i < la; i++)
		a[la - i] = s1[i] - '0';
	for (int i = 0; i < lb; i++)
		b[lb - i] = s2[i] - '0';
	lc = la + lb;

	for (int i = 1; i <= la; i++)
	{
		for (int j = 1; j <= lb; j++)
		{
			c[i + j - 1] += a[i] * b[j];
			c[i + j] += c[i + j - 1] / 10;
			c[i + j - 1] %= 10;
		}//进位
	}
	 if(c[lc] == 0 && lc > 0)
		lc--;//删除前导0
	 for (int i = lc; i > 0; i--)
		 printf("%d", c[i]);
	return 0;
}

//洛谷计算阶乘之和
int main()
{
	int n = 0;
	scanf("%d", &n);
	int a[1000] = { 0 }, b[1000] = {0};
	a[1] = 1;
	b[1] = 0;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= 1000; j++)
		{
			a[j] = a[j] * i;
		}
		for (int j = 1; j <= 1000; j++)
		{
			a[j + 1] = a[j] / 10+a[j+1];
			a[j] = a[j] % 10;
		}
		for (int j = 1; j <= 1000; j++)
		{
			b[j] = b[j] + a[j];
			b[j + 1] = b[j + 1] + b[j] / 10;
			b[j] = b[j] % 10;
		}
	}
	int k = 1000;
	while (b[k] == 0 && k > 0)
		k--;
	for (int i = k; i > 0; i--)
		printf("%d", b[i]);

	return 0;
}

//高精度除法 高精度除以低精度（大数除以小数）
int main()
{
	char s1[5005];//大数(被除数)
	int a[5005] = { 0 };
	long long b,x=0;//小数(除数),余数
	int c[5005] = { 0 };//商
	int la, lc;//长度
	scanf("%s %lld", s1, &b);
	la = strlen(s1);

	for (int i = 1; i <= la; i++)
		a[i] = s1[i - 1] - '0';//不需要逆序

	for (int i = 1; i <= la; i++)
	{
		c[i] = (x * 10 + a[i]) / b;
		x = (x * 10 + a[i]) % b;
	}
	lc = 1;
	while (c[lc] == 0 && lc < la)
		lc++;//删除前导0
	for (int i = lc; i <= la; i++)
		printf("%d", c[i]);

	return 0;
}

//高精度除法 高精度除以高精度（大数除以大数）