#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main()
{
	//内存存放的时二进制序列 统一使用补码 可以将符号和数值统一处理
	int a = 20;
	//原码00000000000000000000000000010100
	// 0x00 00 00 14
	//反码00000000000000000000000000010100
	//补码00000000000000000000000000010100

	int b = -10;
	//原码10000000000000000000000000001010
	// 0x 80 00 00 0a
	//反码11111111111111111111111111110101
	// 0x ff ff ff f5
	//补码11111111111111111111111111110100
	// 0x ff ff ff f6
	//原码得到补码，补码得到原码是一样的操作 取反加一

	return 0;
}


/*
数据在内容中存储的顺序：
大端字节序存储：吧一个数据的低位字节序内容放在高地址处
小端字节序存储：吧一个数据的地位字节序内容放在低地址处
*/

int main()
{
	int a = 1;
	char* p = (char*)&a;
	*p = 0;
	if (a == 1)
		printf("大端");
	if (a == 0)
		printf("小端");
	return 0;
}

///*
// signed char在数据中存储的值-128到127
// signed short在数据中存储的值-32768到32767
//*/
//int main()
//{
//	unsigned char c = -1;
//	//原码10000000000000000000000000000001
//	//反码11111111111111111111111111111110
//	//补码11111111111111111111111111111111
//	//char 八个字节取11111111
//
//	printf("%d", c);//进行整型提升补：00000000000000000000000011111111
//	return 0;
//}

int main()
{
	char a = -128;
	/*
	10000000000000000000000010000000
	11111111111111111111111101111111
	11111111111111111111111110000000
	a此时存储的是10000000

	整型提升之后为 补充的是符号位
	11111111111111111111111110000000
	*/
	printf("%u\n", a);//无符号数此时将a补码当成正数的补码

	printf("%d\n", a);//有符号数(默认) 整型提升除了符号位补0
	return 0;
}

int main()
{
	unsigned int i;
	for (i = 9; i >= 0; i--)
	{
		printf("%d", i);
	}
	/*
	死循环：9 8 7 6 5 4 3 2 1 0 4294967295 429497294
	0到-1时 -1的补码被无符号整型当成正数的补码 数值非常大
	*/
 
	return 0;
}

#include <string.h>
int main()
{
	char a[1000];
	int i;
	for (i = 0; i < 1000; i++)
	{
		a[i] = -1 - i;
	}
	/*
	-128减1得到127
	10000000减一截断得到01111111整型提升得到127
	一直减到0时停止计算
	*/
	printf("%d", strlen(a));//128+127=255

	return 0;
}

int main()
{
	unsigned char i = 0;
	//unsigned char 取值范围在0-255
	for (i = 0; i <= 255; i++)
	{

		printf("hello world");//死循环
	}

	return 0;
}

int main()
{
	if (strlen("abc") - strlen("abcdef")>=0)
		printf(">\n");
	else
		printf("<\n");
	/*
	strlen的返回值时size_t时unsigned int
	无符号和无符号数相减得到的是无符号的数
	*/
	return 0;
}

//浮点型数据存储方式和整型数据存储方式不同
//存储和解用不能搞混
int main()
{
	int n = 9;
 //0000000000000000000000001001
 //当成float储存 0 00000000 00000000000000000001001得到0.000000
	float* pFloat = (float*)&n;

	printf("%d\n", n);
	printf("%f\n", *pFloat);

	*pFloat = 9.0;
 //1001.0
 //1.001*2^3
 //S=0 E=3 M=1.001
 //0 10000010 001000000000000000000000
 
	printf("%d\n", n);
	printf("%f\n", *pFloat);

	return 0;
}


int main()
{
	/*
	9.5f在内存中存放时
	=1001.1  小数点后1在十进制表示1^-1
	=1.0011*2^3
	=(-1)^0*1.0011*2^3形式

	9.6f无法用 (-1)^S*M*2^E形式准确保存
	只能用二进制不断接近

	(-1)^S*M*2^E形式
	32位 S占最高的1个比特位 E占8个比特位 M占23个比特位
	64位 S占最高的1个比特位 E占11个比特位 M占52个比特位
	M中小数点前的1默认 不储存 让出一位使得存储数据更精确
	E是一个无符号整数 但E是可以为负数的 因此
		float使用E(真实值)+127(中间值)存储到E的比特位中
		double使用E(真实值)+1023(中间值)存储到E的比特位中
	*/
	float f = 5.5;
	/*
	* 存储
	5.5转化成存储形式1.011*2^2
	S=0 M=1.011 E=2
	S存储值为0
	E的存储值为2+127=129
	  0x40 b0 00 00因此存储时（倒序）为00 00 b0 40

	M的存储值为011后面补0满23位:01100000000000000000000
	*/

	/*
	*取出
	S存储值为0 M存储为01100000000000000000000 E存储为00 00 b0 40
	S=0
	M为1.011
	E为（00 00 b0 40转化）129减去127=2
	*/

	/*
	特例
	1.E不为全0或者全1 如上述操作
	2.E为全0 E(真实值)=1-127(或者1-1023) 有效数字M 变成0.xxxxx的小数
	3.E为全1 E(真实值)=255-127=128 表示正负无穷大的数字(正负取决于S)
	*/
	return 0;
}

/*
输入一个整数数组，实现一个函数
来调整数组中数字的顺序使得数组中所有奇数位于前半部分 偶数位于后半部分
*/

void move(int arr[], int sz)
{
	int left = 0;
	int right = sz-1;
	
	while (left < right)
	{
		while ((left<right) && arr[left] % 2 == 1)
		{
			left++;
		}
		while ((left<right) && arr[right] % 2 == 0)
		{
			right--;
		}
		if (left < right)
		{
			int tmp = arr[left];
			arr[left] = arr[right];
			arr[right] = tmp;
		}
	}

}

int main()
{
	int arr[10] = { 0 };

	int i = 0;
	int sz = sizeof(arr) / sizeof(arr[0]);

	for (i = 0; i < sz; i++)
	{
		scanf_s("%d", arr + i);
	}

	move(arr, sz);
	
	for (i = 0; i < sz; i++)
	{
		printf("%d ", arr[i]);
	}

	return 0;
}

int main()
{
	int n = 0;
	int m = 0;
	scanf("%d %d", &n, &m);

	int arr1[n];
	int arr2[m];//c99变长数组

	int i = 0;
	for (i = 0; i < n; i++)
	{
		scanf("%d", &arr1[i]);
	}

	for (i = 0; i < m; i++)
	{
		scanf("%d", &arr2[i]);
	}

	i = 0;
	int j = 0;
	while (i < n && j < m)
	{
		if (arr1[i] < arr2[j])
		{
			printf("%d ", arr1[i]);
			i++;
		}
		if (arr2[j] < arr1[i])
		{
			printf("%d ", arr2[j]);
			j++;
		}
	}
	if (i < n)
	{
		for (i; i < n; i++)
		{
			printf("%d ", arr1[i]);
		}
	}
	else
	{
		for (j; j < m; j++)
		{
			printf("%d ", arr2[j]);
		}
	}


	return 0;
}
