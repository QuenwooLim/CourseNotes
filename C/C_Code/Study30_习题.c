#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
int main()
{
	int a = 16;
	printf("%o\n", a);//以八进制输出
	printf("%#o\n", a);//以带前缀的八进制输出
	printf("%x\n", a);//以十六进制输出
	printf("%#x\n", a);//以带前缀的十六进制输出

	int b = 0b101;//二进制(0b是前缀)转换成十进制
	int c = 015;//八进制(0是前缀)转换成十进制  ps:平时十进制数字前不要乱加0,会被误以为是八进制
	int d = 0x2A;//十六进制(0x是前缀)转换成十进制
	printf("%d", d);
	return 0;
}

#include<stdbool.h>
int main()
{
	bool a = 10;
	printf("%d\n", a);//1
	_Bool b = 10;
	printf("%d\n", b);//1
	a = (bool)!b;
	printf("%d", a);//0
	return 0;
}

#include<stdlib.h>
int main()
{
	char a =getchar();
	putchar(a);

	getchar();//清空上一次输入时 缓冲区里的\n回车
	char b = getchar();
	putchar(b);

	char ch;
	while ((ch = getchar()) != 'EOF')
		putchar(ch);//一直输出输入框里的字符

	return 0;
}

int main()
{
	int a = 10;
	int* b = &a;
	*b = 20; 
	//如果两个变量名指向同一个内存地方,则对内存地址操作可以影响到其他变量的值
	printf("%d", a);
	return 0;
}

int main()
{
	double a = 1.0000002;
	double b = 1.000003;
	double dif = 1.0e-5;
	if ((a - b) <= dif)
		printf("a=b");
	else
		printf("a!=b");
	return 0;
}

int main()//math.h
{
	int a = -5;
	printf("%d\n", abs(a));//整数的绝对值
	float b = -5.564;
	printf("%f\n", fabs(b));//浮点数的绝对值
	printf("%d\n", (int)ceil(5.5));
	printf("%d\n", (int)floor(5.5));
}

int main()//二分查找
{
	int arr[] = {-10,-5,-2,-1,0,2,5,7,8,9};
	int find;
	scanf("%d", &find);
	int left = 0;
	int right = 10;
	while (left <= right)
	{
		int mid = (left + right) / 2;
		if (find > arr[mid])
			left = mid + 1;
		else if (find < arr[mid])
			right = mid - 1;
		else
		{
			printf("%d", mid);
			break;
		}
	}
	return 0;
}

int main()//辗转相除法求最大公约数
{
	int a = 0;
	int b = 0;
	scanf("%d %d", &a, &b);
	int max = 0;
	while (1)
	{
		int c = a % b;//a>b
		if (c == 0)
		{
			max = b;
			break;
		}
		else
		{
			a = b;
			b = c;//!
		}
	}
	printf("%d", max);
	return 0;
}

//qsort排序 冒泡排序
int compare(const void* a, const void* b)//函数类型int(不变!) 函数名 函数参数const void* a,b 照抄
{
	// a,b转换成的类型和排序的元素类型有关
	//return *(int*)a - *(int*)b;//返回a-b 就是默认的从小到大排序
	return *(int*)b - *(int*)a;//b-a 就是从大到小排序
}
#include<stdlib.h>
int main()
{
	int arr[] = { 1,2,5,7,9,8,16,4,10,12 };
	qsort(arr, sizeof(arr) / sizeof(arr[0]), sizeof(arr[0]), compare);//数组地址，元素个数，元素大小，比较函数
	for (int i = 0; i < 10; i++)
	{
		printf("%d ", arr[i]);
	}
	return 0;
}

#include<stdlib.h>
//qsort对字符串数组排序
int compare(const void* a, const void* b)
{
	char** pa = (char**)a;//数组里存的是字符串的地址 要用地址的地址char**
	char** pb = (char**)b;
	return strcmp(*pa, *pb);//字符串比较要用strcmp函数  对字符串的地址的地址解引用得到字符串地址
}
int main()
{
	char* arr[] = { "aaa","ggg","sss","eee","xxx","ccc" };
	qsort(arr, sizeof(arr) / sizeof(arr[0]), sizeof(arr[0]), compare);
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		printf("%s\n", arr[i]);
	}
	return 0;
}

//qsort排序 对结构体的成员进行排序
#include<stdlib.h>
struct Stu
{
	char name[10];
	int age;
};
int compare(const void* a, const void* b)
{
	struct Stu* pa = (struct Stu*)a;
	struct Stu* pb = (struct Stu*)b;
	strcmp(pa->name, pb->name);
}
int main()
{
	struct Stu arr[] = { {"hh",18},{"xx",2},{"aa",99},{"bb",1} };
	for (int i = 0; i < 4; i++)
	{
		printf("%s %d\n", arr[i].name, arr[i].age);
	}
	qsort(arr, 4, sizeof(struct Stu), compare);//数据元素是数组成员即一整个结构体
	for (int i = 0; i < 4; i++)
	{
		printf("%s %d\n", arr[i].name, arr[i].age);
	}
	return 0;
}

/*
汉诺塔游戏
两层：先把前一层 从目标杆移动到中转杆
三层：先把前两层 从目标杆移动到中转杆..
...
n层： 把前n-1层移动到中转杆
      再把第n层移动到目标杆
      再把n-1层移动到目标杆
*/
void hnooi(int n, char start, char mid, char end)
{
    if (n == 1)
        printf("%c->%c\n", start, end);
    else
    {
        hnooi(n - 1, start, end, mid);//把n-1从起始杆移动到中转杆
        printf("%c->%c\n", start, end);//第n层从起始杆到目标杆
        hnooi(n - 1, mid, start, end);//把n-1移动到目标杆
    }
}
int main()
{
    printf("汉诺塔的层数:");
    int n = 0;
    scanf("%d", &n);
    char A = 'A';//起始杆
    char B = 'B';//中转杆
    char C = 'C';//目标杆
    hnooi(n,A,B,C);
    return 0;
}

/*
分割平衡字符串
RLRRLLRLRL->RL RRLL RL RL 每个字符串中LR个数相同
*/
int main()
{
    char s[] = "RLRRLLRLRL";
    int count = 0;
    int flag = 0;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == 'R')
            flag++;
        else
            flag--;//flag作用是一个摆针 每次回到平衡位置代表一个平衡字符串

        if (!flag)
            count++;
    }
    return 0;
}



int* getRow(int n, int* size) 
{

    int* ret = malloc((n + 1) * sizeof(int));//函数中的数组要用malloc分配内存
    //...
    return ret;
}