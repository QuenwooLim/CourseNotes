#include<iostream>
using namespace std;
#include<vector>
#include<fstream>
#include<string>
#include"test.hpp"
#include <windows.h>
#include<time.h>
wstring GbkToWstring(const string& gbkStr) {
	int len = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, nullptr, 0);
	wstring wstr(len - 1, L'\0');
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wstr[0], len);
	return wstr;
}
int main()
{    
	clock_t start, end;
	double cpu_time_used;
	start = clock();//开始计时的时间

	string filepath = "C:\\Users\\Lin Hongyu\\Desktop\\数据结构与算法实验报告\\数据结构与算法Project\\测试文本(中文和英文)\\text0.txt";
	cout << "查找单词个数：" << endl;
	int num;
	cin >> num;

	cout << "查找的单词：" << endl;
	string word;
	cin >> word;//先输入一个单词来判断时中文还是英文
	num--;

	int flag = 1;//0表示英文 1表示英文
	if (word[0] >= 65 && word[0] <= 122)
		flag = 0;

	if (flag == 0)
	{
		vector<string> Query;
		Query.push_back(word);//将刚才的单词输入
		
		while (num--)
		{
			cin >> word;
			Query.push_back(word);
		}
		Hashing0 H(Query.size());
		vector<string> Sentence = ReadText0(filepath);
		Traversal0(Sentence, Query, H);
		print0(Query, H);
	}
	else
	{	vector<wstring> Query;
		Query.push_back(GbkToWstring(word)); // 转换为宽字符

		while (num--)
		{
			cin >> word;
			Query.push_back(GbkToWstring(word)); // 转换为宽字符
		}
		Hashing1 H(Query.size());
		vector<wstring> Sentence = ReadText1(filepath);
		Traversal1(Sentence, Query, H);
		print1(Query, H);
	}

	end = clock();//结束计时的时间
	cpu_time_used = ((double)(end - start));
	cout << cpu_time_used << endl;

	return 0;
}