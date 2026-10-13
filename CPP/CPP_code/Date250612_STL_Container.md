## 2.容器
- 顺序容器、关联容器、适配器

### 2.1 顺序容器
- array、vector、deque、forward_list、list

#### 2.1.1 构造函数
- vector<T> v;  //采用模板实现类实现，默认构造函数
- vector(const vector &vec); //即vector<T> v(v1)拷贝构造函数 使用v作为v1的副本构造
- vector(v.begin(), v.end());    //将v[begin(), end())区间中的元素拷贝给本身。
- vector(n); //构造函数，指定长度为n（默认值）
- vector(n, elem); //构造函数将n个elem拷贝给本身。
- vector<T> c({a1,a2...}); 或vector<T> c={a1,a2...};  //使用初始化列表创建

```cpp
void printVector(vector<int>& v) {

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

int main() {
	vector<int> v1; //无参构造
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
	}
	printVector(v1);

	vector<int> v2(v1.begin(), v1.end());
	printVector(v2);

	vector<int> v3(10, 100);
	printVector(v3);
	
	vector<int> v4(v3);
	printVector(v4);
	system("pause");
	return 0;
}
```

#### 2.1.2 访问元素（存取）
- v.front()
- v.back()
- v[index] //list容器不支持该操作 越界没有定义
- c.at(index) //list容器不支持该操作 会进行越界检查

#### 2.1.3 迭代器
- 带c的方法，返回的是常迭代器（只读迭代器）
- v.begin()  或 v.cbegin()  指向第0个元素
- v.end()    或 v.cend()    指向最后一个元素下一位置
- c.rbegin() 或 c.crbegin() 逆向迭代器，指向最后一个元素
- c.rend()   或 c.crend     逆向迭代器，指向第0个元素的迁移位置

#### 2.1.4 插入元素
- push_back(ele); //尾部插入元素ele
- push_front(ele); //在头部插入元素ele
- insert(const_iterator pos, ele); //迭代器指向位置pos插入元素ele
- insert(const_iterator pos, int count,ele);//迭代器指向位置pos插入count个元素ele
- insert(const_iterator pos,begin,end) //在pos前插入begin和end范围内的元素

```cpp
int main()
{
	vector<int> ivec;		// 创建空的vector容器，用于存放int型对象
	deque<string> sdeq; // 创建空的deque容器，用于存放string型对象
	int iarr[] = {100, 100, 100};

	// 在vector容器中增加元素：在尾端增加10个元素：值为1~10
	for (int i = 1; i < 11; i++)
	{
		ivec.push_back(i);
	}

	// 在vector容器头端再增加一个元素，值为20
	ivec.insert(ivec.begin(), 20);

	// 在vector容器的第四个元素后再增加两个元素，值均为30
	ivec.insert(ivec.begin() + 4, 2, 30);

	//将数组iarr中的元素增加到vector容器尾端。注意：被插入的元素不包括
	//第三个参数所指向的元素因此，要插入iarr中的所有元素，第三个参数应
	//该为iarr加3
	ivec.insert(ivec.end(), iarr, iarr + 3);

	// 在deque容器中增加元素
	sdeq.push_back("is");
	sdeq.push_front("this");
	sdeq.insert(sdeq.end(), "an");
	sdeq.insert(sdeq.end(), "example");

	// 输出vector容器中的元素
	cout << "vector:" << endl;
	for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); it++)
		cout << *it << ' ';
	cout << endl;

	// 输出deque容器中的元素
	cout << "double-ended queue:" << endl;
	for (deque<string>::iterator it = sdeq.begin(); it != sdeq.end(); it++)
		cout << *it << ' ';
	cout << endl;
	
	return 0;
}
```

#### 2.1.5 删除元素
- pop_back(); //删除最后一个元素
- pop_front(); //删除第一个元素
- erase(const_iterator pos); //删除迭代器指向的元素 返回值指向下一个元素
- erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
- clear(); //删除容器中所有元素

```cpp
template<typename T>
void print(T beg, T end){
	for (auto it = beg; it != end; it++)
		cout << *it << ' ';
	cout << endl;
}

int main()
{
	deque<int> ideq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	// 输出删除操作之前deque容器中的所有元素
	cout << "before delete:" << endl;
	print(ideq.begin(),ideq.end());

	// 删除容器中的第一个及最后一个元素
	ideq.pop_front();
	ideq.pop_back();

	cout << "the first and last element are deleted:" << endl;
	print(ideq.begin(),ideq.end());

	auto iter = ideq.begin();			// iter指向ideq中现存的第0个元素
	ideq.erase(ideq.erase(iter + 1)); 	// 删除ideq中现存的第1、第2个元素
									  	// 输出删除操作之后list容器中的所有元素
	cout << "the second and third element are deleted:" << endl;
	print(ideq.begin(),ideq.end());

	// 删除容器中现存的前三个元素
	ideq.erase(ideq.begin(), ideq.begin() + 3);

	// 输出删除操作之后list容器中的所有元素
	cout << "three elements at front are deleted:" << endl;
	print(ideq.begin(),ideq.end());

	// 删除剩余的所有元素
	ideq.clear();
	cout << "after clear:" << endl;
	if (ideq.empty()) // 容器为空
		cout << "no element in double-ended queue" << endl;

	return 0;
}
```

### 容器的比较操作
- 1. ==
- 元素个数相同且每个位置元素相等
- 2. !=
- 和==相反
- 3. <、<=、>、>=
- 若一个容器中的所有元素与另一个容器中开头一段元素对应相等，则较短的容器小于另一个容器
- 否则，两个容器中第一对不相等的元素就是比较结果

```cpp
template<typename T>
void print(T beg, T end){
	for (auto it = beg; it != end; it++)
		cout << *it << ' ';
	cout << endl;
}
int main()
{
    int iarr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    list<int> ilist1(iarr, iarr+10);//1 2 3 4 5 6 7 8 9 10
    list<int> ilist2(iarr, iarr+5);// 1 2 3 4 5
    list<int> ilist3(ilist2);// 1 2 3 4 5

    list<int> ilist4(ilist2);
    ilist4.push_back(12);
    ilist4.push_back(7);// 1 2 3 4 5 12 7
    
    //print all
    vector<string> name = {"list1","list2","list3","list4"};
    vector<list<int>> vec_list = {ilist1,ilist2,ilist3,ilist4};
    for (int i = 0; i < 4; i++) {
    	cout << name[i] << ": ";
		print(vec_list[i].begin(),vec_list[i].end());
	}
	
	//containerCompare
	cout << "ilist2 == ilist3 : ";//true
	if (ilist2 == ilist3)
		 cout << "true" << endl;
	else
		cout << "false" << endl;
	
    cout << "ilist1 < ilist2 : ";//false
	if (ilist1 < ilist2)
		 cout << "true" << endl;
	else
		cout << "false" << endl;
	
    cout << "ilist3 > ilist4 : ";//false
	if (ilist3 > ilist4)
		 cout << "true" << endl;
	else
		cout << "false" << endl;
		
	cout << "ilist1 < ilist4 : ";//true
	if (ilist1 < ilist4)
		 cout << "true" << endl;
	else
		cout << "false" << endl;
	
    cout << "ilist2 != ilist4 : ";//true
	if (ilist2 != ilist4)
		 cout << "true" << endl;
	else
		cout << "false" << endl;
	system("pause");
	return 0;
}
```

### 容器的大小
- empty();  //判断容器是否为空
- capacity(); //容器的容量
- size(); //返回容器中元素的个数
- max_size(); //返回容器中可存放元素的最大数目
- resize(int num); //重新指定容器的长度为num，若容器变长，则以默认值填充新位置。
- //如果容器变短，则末尾超出容器长度的元素被删除。
- resize(int num, elem); //重新指定容器的长度为num，若容器变长，则以elem值填充新位置。
- //如果容器变短，则末尾超出容器长度的元素被删除

```cpp
void printVector(vector<int>& v) {

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
int main() {
	vector<int> v1;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
	}
	printVector(v1);
	if (v1.empty())
	{
		cout << "v1为空" << endl;
	}
	else
	{
		cout << "v1不为空" << endl;
		cout << "v1的容量 = " << v1.capacity() << endl;
		cout << "v1的大小 = " << v1.size() << endl;
	}

	//resize 重新指定大小 ，若指定的更大，默认用0填充新位置，可以利用重载版本替换默认填充
	v1.resize(15,10);
	printVector(v1);

	//resize 重新指定大小 ，若指定的更小，超出部分元素被删除
	v1.resize(5);
	printVector(v1);
	system("pause");
	return 0;
}
```

### 容器的赋值和交换
- c1=c2;//容器c2赋值给c1 vector& operator=(const vector &vec);重载等号操作符
- assign(beg, end); //将[beg, end)区间中的数据拷贝赋值给本身。
- assign(n, elem); //将n个elem拷贝赋值给本身。
- c1.swap(c2); //交换c1和c2

```cpp
void printVector(vector<int>& v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

int main() {
	vector<int> v1; //无参构造
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
	}
	printVector(v1);

	vector<int>v2;
	v2 = v1;
	printVector(v2);

	vector<int>v3;
	v3.assign(v1.begin(), v1.end());
	printVector(v3);

	vector<int>v4;
	v4.assign(10, 100);
	printVector(v4);

	system("pause");
	return 0;
}
```

### 2.2关联容器
- <map>头文件：
- map 通过键进行元素存取
- multimap （支持重复元素）
- <set>头文件：
- set 任意元素的集合
- multiset （支持重复元素）
- <unordered_map>头文件：
- unorderde_map 无序的map，类似于哈希表
- <unordered_set>头文件：
- unorderde_set 无序的set，类似于哈希表

### pair（map的元素为pair类型的）
- pair<T1,T2> 代表由类型T1和类型T2组成的有序对，第一个元素key键值，第二个元素value实值

- 语法构造：
- pair<T1,T2>(a1,a2);
- auto p=make_pair(v1,v2);
- p.first可以访问v1，p.second可以访问v2

#### 2.2.1 构造函数
- map<T1, T2> mp; //map默认构造函数:
- map<T1, T2> mp(m); //拷贝构造函数
- map<T1, T2> m(begin,end); //使用迭代器begin到end的值进行初始化
- map<T1, T2> mp={{a1,a2},{b1,b2}...} //初始化列表

```cpp
void printMap(map<int,int>&m)
{
	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++)
	{
		cout << "key = " << it->first << " value = " << it->second << endl;
	}
	cout << endl;
}

int main() {

	map<int,int>m; //默认构造
	m.insert(pair<int, int>(1, 10));
	m.insert(pair<int, int>(2, 20));
	m.insert(pair<int, int>(3, 30));
	printMap(m);

	map<int, int>m2(m); //拷贝构造
	printMap(m2);

	map<int, int>m3;
	m3 = m2; //赋值
	printMap(m3);

	system("pause");
	return 0;
}
```

#### 2.2.2 插入元素
- m.insert(elem); //在容器中插入元素。
- m[key]=value; //最直接的插入方式 使用operator[]

#### 2.2.3 删除元素
- clear(); //清除所有元素
- erase(pos); //删除pos迭代器所指的元素，返回下一个元素的迭代器。
- erase(beg, end); //删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
- erase(key); //删除容器中值为key的元素。

```cpp
void printMap(map<int,int>&m)
{
	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++)
	{
		cout << "key = " << it->first << " value = " << it->second << endl;
	}
	cout << endl;
}

int main() {
	//插入
	map<int, int> m;
	//第一种插入方式
	m.insert(pair<int, int>(1, 10));
	//第二种插入方式
	m.insert(make_pair(2, 20));
	//第三种插入方式
	m.insert(map<int, int>::value_type(3, 30));
	//第四种插入方式
	m[4] = 40; 
	printMap(m);

	//删除
	m.erase(m.begin());
	printMap(m);

	m.erase(3);
	printMap(m);

	//清空
	m.erase(m.begin(),m.end());
	m.clear();
	printMap(m);

	system("pause");
	return 0;
}
```

#### 2.2.4 查找与计数
- m.find(key) 查找键值，并返回迭代器
- m.count(key) 统计键值出现次数

```cpp
//查找和统计
int main() {
	map<int, int>m; 
	m.insert(pair<int, int>(1, 10));
	m.insert(pair<int, int>(2, 20));
	m.insert(pair<int, int>(3, 30));

	//查找
	map<int, int>::iterator pos = m.find(3);

	if (pos != m.end())
	{
		cout << "找到了元素 key = " << (*pos).first << " value = " << (*pos).second << endl;
	}
	else
	{
		cout << "未找到元素" << endl;
	}

	//统计
	int num = m.count(3);
	cout << "num = " << num << endl;
	system("pause");
	return 0;
}
```

#### 2.2.5 multimap
- 不支持下标操作
- 键值相同的元素相邻存放
- erase删除键值为key的所有元素 并返回删除数目
- 函数：
- m.lower_bound(k) 指向容器中第一个键 >=k 的元素
- m.upper_bound(k) 指向容器中第一个键 >k  的元素
- m.equal_range(k) 返回一对迭代器的pair对象，first等价于lower_bound，second等价于upper_bound

#### 2.2.6 map与set
- map支持的操作set都支持
- set不支持下标操作
- set不是pair类型

### 2.3 容器适配器
- 头文件<stack>：stack
- 头文件<queue>：queue、priority_deque
- 标准库中的容器适配器是基于顺序容器建立的
- 在创建适配器对象时可以选择相应的基础容器类
- stack适配器：vector、deque(默认)、list
- queue适配器：list、deque(默认)  //没有vector
- priority_queue适配器：vector(默认)、deque

#### 2.3.1 Stack
- push()
- pop()
- top()
- empty()
- size()
- 没有front()

#### 2.3.2 Queue
- push()
- pop()
- front()
- back()
- empty()
- size()

#### 2.3.3 priority_queue
- push()
- pop()
- top()
- empty()
- size()
- 没有back()
