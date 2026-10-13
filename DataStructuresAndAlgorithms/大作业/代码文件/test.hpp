#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<string>
#include <functional>
#include <algorithm>
#include<fstream>
#include <regex>
#include <codecvt> //for codecvt_utf8
#include<Windows.h>
using namespace std;

class Hashing0//哈希表
{
public:
	Hashing0(int size)//构造函数，定义哈希表大小
	{
		this->table.resize(size);
	}
	/*
	int Hash(string word)//求单词的哈希值:标准哈希函数
	{
		return hash<string>()(word) % (table.size());  
	}
	*/
	/*
	int Hash(string word) //求单词的哈希值:直接定址法
	{
		int hashValue = 0;
		for (char c : word) {
			hashValue += c;  //累加字符的ASCII值
		}
		return hashValue % (table.size());
	}
	*/
	/*
	int Hash(string word) //求单词的哈希值:字符串数值法
	{
		int hashValue = 0;
		int prime = 31; //用质数作为权重因子
		for (int i = 0; i < word.length(); i++) {
			hashValue = hashValue * prime + word[i];  //累加字符的数值权重
		}
		return abs(hashValue) % (table.size()); 
	}
	*/
	int Hash(string word) //求单词的哈希值：除留余数法
	{
		unsigned long hashValue = 0;
		int prime = 37;
		for (int i = 0; i < word.length(); i++) {
			hashValue = (hashValue * prime + word[i]) % (table.size()); 
		}
		return hashValue % (table.size());
	}
	

	void Insert(const string& word, const string& sentence)//插入
	{
		int index = Hash(word);
		table[index].push_back({ word,sentence });
	}
	vector<string> Search(const string& word)//查找
	{
		int index = Hash(word);
		vector<string> result;//将找到的句子全部放进result
		for (list<pair<string, string>>::iterator it=table[index].begin(); it != table[index].end(); it++)
		{
			if (it->first == word)
				result.push_back(it->second);
		}
		return result;
	}
private:
	vector<list<pair<string, string>>> table;//pair<单词，句子> 使用list存储  再将一条条链放入vector中
};
class Hashing1//哈希表
{
public:
	Hashing1(int size)//构造函数，定义哈希表大小
	{
		this->table.resize(size);
	}
	int Hash(wstring word)//求单词的哈希值
	{
		return hash<wstring>()(word) % (table.size());  //使用标准哈希函数对字符串求哈希值
	}
	void Insert(const wstring& word, const wstring& sentence)//插入
	{
		int index = Hash(word);
		table[index].push_back({ word,sentence });
	}
	vector<wstring> Search(const wstring& word)//查找
	{
		int index = Hash(word);
		vector<wstring> result;//将找到的句子全部放进result
		for (list<pair<wstring, wstring>>::iterator it = table[index].begin(); it != table[index].end(); it++)
		{
			if (it->first == word)
				result.push_back(it->second);
		}
		return result;
	}
private:
	vector<list<pair<wstring, wstring>>> table;//pair<单词，句子> 使用list存储  再将一条条链放入vector中
};

vector<string> ReadText0(const string& FilePath)//读取文件并按句号分割成句子
{
	ifstream file(FilePath);//打开文件
	if (!file.is_open()) {
		cerr << "Failed to open file: " << FilePath << endl;
		return {};  // 如果文件打开失败，返回空的句子列表
	}
	file.imbue(locale(""));

	string line;
	string content;
	while (getline(file, line))
		content = content+line;//拼接每一行 句子之间空格隔开

	// 一次性读取文件内容
	//如果没有适当的句子分隔（例如空格），就可能导致匹配的句子没有明显的分隔
	
	// 使用正则表达式分割文本，按句号、问号或感叹号作为结束符.它会忽略开头的空格
	//regex sentenceRegex("([^.!?]+[.!?])"); // 匹配句子的正则表达式（中文）
	regex sentenceRegex(R"(([^.!?]+(?:[.!?]+(?:\")?)?))");//优化 ."结尾的句子判断
	
	vector<string> sentences;

	// 使用正则表达式进行匹配，提取所有符合条件的句子
	auto wordsBegin = sregex_iterator(content.begin(), content.end(), sentenceRegex);
	auto wordsEnd = sregex_iterator();
	/*
	sregex_iterator(content.begin(), content.end(), sentenceRegex)：
		这是正则表达式的迭代器，用于查找 content 中所有与 sentenceRegex 匹配的子串（即句子）。
		它返回一个迭代器，指向匹配的第一个结果。
    sregex_iterator()：这是一个默认构造的迭代器，表示匹配的结束位置。
		这个迭代器的作用是标记所有匹配的范围。
	*/
	// 将匹配到的每个句子加入到结果向量
	for (sregex_iterator i = wordsBegin; i != wordsEnd; ++i)
	{
		string sen = i->str();
		while (sen[0] == ' '||sen[0]=='”')
		{
			sen.erase(sen.begin());
		}
		sentences.push_back(sen); // 添加匹配的句子
	}
	/*
	i->str()：i 是一个迭代器，它指向当前匹配的 std::smatch 对象。
			smatch 类是一个用于存储匹配结果的容器，
			它有一个 str() 成员函数，可以返回当前匹配的字符串，也就是当前的句子。
	*/
	return sentences;
}
vector<wstring> ReadText1(const string& FilePath) 
{
	locale::global(locale(""));//保证加载文件支持中文路径
	wifstream file(FilePath); // 打开文件
	if (!file.is_open()) {
		cerr << "Failed to open file: " << FilePath << endl;
		return {}; // 如果文件打开失败，返回空的句子列表
	}
	wstring content((istreambuf_iterator<wchar_t>(file)), istreambuf_iterator<wchar_t>());
	file.close();
	content = regex_replace(content, wregex(L"\\s+"), L" ");
	wregex sentenceRegex(L"([^。！？]+[。！？])"); // 匹配句子的正则表达式（中文）
	vector<wstring> sentences;
	
	// 使用正则表达式进行匹配，提取所有符合条件的句子
	auto wordsBegin = wsregex_iterator(content.begin(), content.end(), sentenceRegex);
	auto wordsEnd = wsregex_iterator();

	// 将匹配到的每个句子加入到结果向量
	for (wsregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
		wstring wsen = i->str();
		while (wsen[0] == ' ')
		{
			wsen.erase(wsen.begin());
		}
		sentences.push_back(wsen); //添加匹配的句子
	}
	return sentences;
}


void Traversal0(vector<string>& Sentence, vector<string>& Query, Hashing0& H)
{
	for (int i = 0; i < Sentence.size(); i++)
	{
		for (int j = 0; j < Query.size(); j++)
		{
			if (Sentence[i].find(Query[j])!=Sentence[i].npos)//没找到返回npos迭代器
			{
				H.Insert(Query[j], Sentence[i]);
			}
		}
	}
}
void Traversal1(vector<wstring>& Sentence, vector<wstring>& Query, Hashing1& H)
{
	for (int i = 0; i < Sentence.size(); i++)
	{
		for (int j = 0; j < Query.size(); j++)
		{
			if (Sentence[i].find(Query[j]) != Sentence[i].npos)
			{
				H.Insert(Query[j], Sentence[i]);
			}
		}
	}
}

void print0(const vector<string>& Query,Hashing0& H)
{
	for (int i = 0; i < Query.size(); i++)
	{
		vector<string> result = H.Search(Query[i]);
		if (result.size() == 0)
		{
			cout << "找不到包含“"<<Query[i]<<"”的句子" << endl;
			return;
		}
		cout << "包含“" << Query[i] << "”的句子如下：" << endl;

		for (int j = 0; j < result.size(); j++)
		{
			cout <<result[j] << endl;
		}
		cout << endl;
	}
}
void print1(const vector<wstring>& Query, Hashing1& H)
{
	for (int i = 0; i < Query.size(); i++)
	{
		vector<wstring> result = H.Search(Query[i]);
		if (result.size() == 0)
		{
			cout << "找不到包含“";
			wcout << Query[i];
			cout << "”的句子" << endl;
			return;
		}
		cout << "包含“";
		wcout << Query[i];
		cout << "”的句子如下：" << endl;

		for (int j = 0; j < result.size(); j++)
		{
			wcout << result[j] << endl;
		}
		wcout << endl;
	}
}