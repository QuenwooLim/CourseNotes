#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
/*
第七节 函数进阶与练习
*/

//搜索插入位置
/*
解题1 
*/
int Ins_Loc(int* arr, int target,int len)
{
	for (int i = 0; i < len; i++)
	{
		if (arr[i] >= target)
			return i;
	}
	return len;
}
/*
解题2 二分
*/
int Ins_Loc2(int* arr, int target, int len)
{
	int left = 0;
	int right = len;
	while (left <= right)
	{
		int mid = (left + right) / 2;
		if (arr[mid] == target)
			return mid;
		else if (arr[mid] < target)
			left = mid + 1;
		else
			right = mid - 1;
	}
	
}
int main()
{
	int arr[] = { 1,3,5,6 };
	int len = sizeof(arr) / sizeof(arr[0]);
	int target = 6;
	printf("Answer:%d", Ins_Loc2(arr,target,len));
	return 0;
}

//删除有序数组中的重复项
//解法一
int main()
{
	int arr[] = { 0,0,1,1,1,2,2,3,3,4 };
	int len = sizeof(arr)/sizeof(arr[0]);
	for (int i = 1; i < len;)
	{
		if (arr[i] == arr[i - 1])//前移
		{
			for (int j = i; j < len - 1; j++)
				arr[j] = arr[j + 1];
			len--;
		}
		else
			i++;
	}
	for (int k = 0; k < len; k++)
	{
		printf("%d ", arr[k]);
	}

	return 0;
}
//解法二 设置快慢指针
int main()
{
	int arr[] = { 0,0,1,1,1,2,2,3,3,4 };
	int len = sizeof(arr) / sizeof(arr[0]);
	int slow = 0;
	int fast = 1;
	for (fast = 1; fast < len;fast++)
		if (arr[fast] != arr[slow])
			arr[++slow] = arr[fast];
	for (int k = 0; k <= slow; k++)
	{
		printf("%d ", arr[k]);
	}
	return 0;
}

//两数之和
//暴力破解
int main()
{
	int arr[] = { 2,7,11,15 };
	int target = 9;
	int len = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < len-1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (arr[i] + arr[j] == target)
			{
				printf("%d %d", i, j);
				return 0;
			}
		}
	}
	return 0;
}

//打印杨辉三角
//解法一 要求使用一个最多n个元素的数组
void line(int arr[],int i)
{
	if (i == 0)
	{
		arr[0] = 1;
	}
	else if (i == 1)
	{
		arr[0] = 1;
		arr[1] = 1;
	}
	else
	{
		arr[0] = 1;
		int num1 = arr[0];
		int num2 = arr[1];//先保存
		for (int j = 1; j < i; j++)
		{
			arr[j] = num1 + num2;
			num1 = num2;
			num2 = arr[j + 1];
		}
		arr[i] = 1;
	}
}
void print_YH()
{
	int n = 10;
	int arr[10];
	for (int i = 0; i < 10; i++)
	{
		line(arr,i);
		for (int k=10-i;k>0;k--)
		{
			printf("  ");
		}
		for (int j = 0; j <= i; j++)
		{
			printf("%-4d", arr[j]);
		}
		printf("\n");
	}
}
//解法二 可以使用二维数组
void print_YH2()
{
	int arr[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j <=i; j++)
		{
			if (j == 0 || j == i)
				arr[i][j] = 1;
			else
			{
				arr[i][j] = arr[i - 1][j] + arr[i - 1][j - 1];
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		for (int k=10-i;k>0;k--)
		{
			printf("  ");
		}
		for (int j = 0; j <= i; j++)
		{
			printf("%-3d ", arr[i][j]);

		}
		printf("\n");
	}
}
int main()
{
	int n = 10;
	print_YH();
	return 0;
}

/*
第八讲
*/
//凯撒密码
int main()
{
	char s[] = "STUDENT";
	int key = 8;//移动
	for (int i = 0; s[i] != '\0'; i++)
	{
		s[i] = (s[i] - 'A' + key+26) % 26 + 'A';
	}
	printf("%s", s);
	return 0;
}

//最长公共前缀
int Find_Com(char s1[], char s2[], char s3[], int min_len)
{
	for (int i = 0; i < min_len; i++)
	{
		if (!(s1[i] == s2[i] && s2[i] == s3[i]))
			return i; // 返回第一个不同的位置
	}
	return min_len; // 如果所有字符都匹配，则返回最短长度
}
int main()
{
	char str1[] = "flower";
	char str2[] = "flow";
	char str3[] = "flight";
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int len3 = strlen(str3);
	int min_len = (len1 < len2) ? (len1 < len3 ? len1 : len3) : (len2 < len3 ? len2 : len3);
	int index = Find_Com(str1, str2, str3, min_len);
	printf("%d", index);
	return 0;
}

//单词拼写
int main()
{
	char* words[] = {"hello","world","leetcode"};
	char cs[] = "welldonehoneyr";
	int sz = sizeof(words) / sizeof(words[0]);
	for (int i = 0; i < sz; i++)
	{
		int arr[26] = {0};
		for (int j = 0; cs[j] != '\0'; j++)
			arr[cs[j] - 'a']++;
		for (int j = 0; words[i][j] != '\0'; j++)
			arr[words[i][j] - 'a']--;
		int j = 0;
		for (j = 0; j < 26; j++)
			if (arr[j] < 0)
				break;
		if(j==26)
			printf("%s\n", words[i]);
	}
	return 0;
}

//最大单词数 键盘故障
int main()
{
	char text[] = "leet code";
	char breokenLetters[] = "lt";
	int count = 0;
	for (int i = 0; text[i] != '\0'; i++)
	{
		if (text[i] == ' ')
			count++;
		if (strchr(breokenLetters, text[i]) != NULL)
		{
			while (text[i] != ' ' && text[i]!='\0')
				i++;
			continue;
		}
		if (text[i + 1] == '\0')
			count++;
	}
	printf("%d", count);
	return 0;
}

//分割平衡字符串
int main()
{
	char s[] = "RLRRLLRLRL";
	int flag = 0;
	int count = 0;
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (s[i] == 'R')
			flag++;
		else
			flag--;
		if (flag == 0)
			count++;
	}
	printf("%d", count);
	return 0;
}

//第一个出现两次的字母
int main()
{
	char s[] = "abccbaacz";
	int arr[26] = { 0 };
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (arr[s[i] - 'a'] == 1)
		{
			printf("%c", s[i]);
			return 0;
		}
		else
			arr[s[i] - 'a']++;
	}
	return 0;
}

/*
第九讲 指针
*/
//void指针 函数泛类型参数
void* S(void* s, void* n)
{
	char* p = s;
	int num = *(int*)n;
	p += num;
	return p;
}
int main()
{
	char* res;
	char s1[] = "hello world";
	int n = 2;
	res=S(s1, &n);
	printf("%s", res);
	return 0;
}

//函数指针
int add(int x, int y)
{
	return x + y;
}
int sub(int x, int y)
{
	return x - y;
}
int main()
{
	int a = 10;
	int b = 5;
	int (*p)(int, int) = sub;
	int res=p(a, b);//无需写*
	printf("%d", res);
	return 0;
}

//回调函数
int cmpfunc(const void* a,const void* b)
{
	return *(int*)a - *(int*)b;
}
int main()
{
	int arr[] = { 2,3,3,2,0,0,9,3 };
	int nelem = sizeof(arr) / sizeof(arr[0]);
	int width = sizeof(arr[0]);
	qsort(arr, nelem, width, cmpfunc);//直接调用

	int (*p)(const void *,const void* ) = cmpfunc;
	qsort(arr, nelem, width, p);//间接调用

	for (int i = 0; i < nelem; i++)
	{
		printf("%d ", arr[i]);
	}
	return 0;
}

//罗马数字转整数
int main()
{
    char Rom_Tab[][5] = { "CM","CD","XC","XL","IX","IV","M","D","C","L","X","V","I" };
    int Rom_Num[] = { 900, 400, 90, 40, 9, 4, 1000, 500, 100, 50, 10, 5, 1 };
    char str[] = "MCMXCIV";
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        int matched = 0;
        // 遍历罗马数字表  比较两个字符的时候可能越界访问
        for (int j = 0; j < 13; j++)
        {
            int len = strlen(Rom_Tab[j]);
            // 处理两字符的罗马数字（如CM, CD, XC等）
            if (i + len <= strlen(str) && strncmp(str + i, Rom_Tab[j], len) == 0)
            {
                sum += Rom_Num[j];
                i += len - 1; // 如果匹配了两字符的罗马数字，跳过这两个字符
                matched = 1;
                break;
            }
        }

        // 如果没有匹配到两字符的罗马数字，则匹配一字符的罗马数字
        if (!matched)
        {
            for (int j = 6; j < 13; j++)
            {
                if (str[i] == Rom_Tab[j][0])  // 匹配单个字符的罗马数字
                {
                    sum += Rom_Num[j];
                    break;
                }
            }
        }
    }
    printf("%d\n", sum);
    return 0;
}

//验证回文串 移除非字母 不区分大小写
int main()
{
	//char s[] = "A man,a plan,a canal:Panama";
	char s[] = "race a car";
	int slow = 0;
	int fast = 0;
	for (fast = 0; s[fast] != '\0'; fast++)
	{
		if (isalpha(s[fast]) !=0 )
			s[slow++] = tolower(s[fast]);
	}
	s[slow] = '\0';
	for (int i = 0; i < slow; i++)
	{
		if (s[i] != s[slow-1 - i])
		{
			printf("FLASE");
			return 0;
		}
	}
	printf("TRUE");
	return 0;
}

//二进制求和
void reverse(char* s,int len)
{
	for (int i = 0; i < len / 2; i++)
	{
		char ch = s[i];
		s[i] = s[len - 1 - i];
		s[len - 1 - i] = ch;
	}
}
int main()
{
	char a[100] = "1010";
	char b[100] = "1011";
	int lena = strlen(a);
	reverse(a,lena);
	int lenb = strlen(b);
	reverse(b,lenb);
	char res[100] = {0};
	int len_res = 0;

	int indexa = 0;
	int indexb = 0;
	int indexres = 0;
	int carry = 0;
	for (;indexa<lena && indexb<lenb;)
	{
		if (carry == 0)
		{
			if (a[indexa] == '1' && b[indexb] == '1')
			{
				carry = 1;
				res[indexres] = 0;
				indexa++;
				indexb++;
			}
		}
		else
		{
			;
		}
	}
	return 0;
}

//合并两个有序数组
int main()
{
	int nums1[] = { 1,2,3,0,0,0};
	int nums2[] = { 2,5,6 };
	int m = 3;
	int n = 3;
	int index = m;
	for (int i=0; index < m + n; index++,i++)
	{
		nums1[index] = nums2[i];
	}
	for (int i = 0; i < m + n; i++)
	{
		printf("%d", nums1[i]);
	}
	return 0;
}

/*
第十讲 结构体
*/
//计算距离之和
struct Dis
{
	int feet;//英尺
	double inch;//英寸
};
int main()
{
	struct Dis p1 = { 23,8.6 };
	struct Dis p2 = { 33,2.4 };
	struct Dis res;
	res.feet = p1.feet + p2.feet;
	res.inch = p1.inch + p2.inch;
	if (res.inch >= 10)
	{
		res.inch -= 10;
		res.feet += 1;
	}
	printf("%d英尺%.2f英寸", res.feet, res.inch);
	return 0;
}

//计算时间差
struct Time
{
	int hour;
	int minute;
	int second;
};
int main()
{
	struct Time t1 = {23,02,50 };
	struct Time t2 = {23,01,50 };
	struct Time res;
	if (t1.second > t2.second)
	{
		t2.second += 60;
		t2.minute -= 1;
	}
	res.second = t2.second - t1.second;
	if (t1.minute > t2.minute)
	{
		t2.minute += 60;
		t2.hour -= 1;
	}
	res.minute = t2.minute - t1.minute;
	if (t1.hour > t2.hour)
		t2.hour += 24;
	res.hour = t2.hour - t1.hour;
	printf("%d %d %d", res.hour, res.minute, res.second);
	return 0;
}
