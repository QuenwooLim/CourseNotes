## 2. STL认识

### 2.1 STL诞生
- 面向对象和泛型编程思想，目的就是复用性的提升

### 2.2 基本概念
- STL(Standard Template Library,标准模板库)
- STL 从广义上分为: 容器(container) 算法(algorithm) 迭代器(iterator)
- 容器和算法之间通过迭代器进行无缝连接。
- STL 几乎所有的代码都采用了模板类或者模板函数

### 2.3 STL六大组件
- 分别是:容器、算法、迭代器、仿函数、适配器（配接器）、空间配置器
- 容器：各种数据结构，如vector、list、deque、set、map等,用来存放数据。
- 算法：各种常用的算法，如sort、find、copy、for_each等
- 迭代器：扮演了容器与算法之间的胶合剂。
- 仿函数：行为类似函数，可作为算法的某种策略。
- 适配器：一种用来修饰容器或者仿函数或迭代器接口的东西。
- 空间配置器：负责空间的配置与管理。

### 2.4 STL中容器、算法、迭代器
- 容器：
- STL容器就是将运用最广泛的一些数据结构实现出来
- 常用的数据结构：数组, 链表,树, 栈, 队列, 集合, 映射表 等
- 这些容器分为序列式容器和关联式容器两种:
- 序列式容器:强调值的排序，序列式容器中的每个元素均有固定的位置。
- 关联式容器:二叉树结构，各元素之间没有严格的物理上的顺序关系

- 算法：
- 有限的步骤，解决逻辑或数学上的问题，这一门学科我们叫做算法(Algorithms)
- 算法分为:质变算法和非质变算法。
- 质变算法：是指运算过程中会更改区间内的元素的内容。例如拷贝，替换，删除等等
- 非质变算法：是指运算过程中不会更改区间内的元素内容，例如查找、计数、遍历、寻找极值等等

- 迭代器：容器和算法之间粘合剂
- 提供一种方法，使之能够依序寻访某个容器所含的各个元素，而又无需暴露该容器的内部表示方式。
- 每个容器都有自己专属的迭代器
- 迭代器使用非常类似于指针，初学阶段我们可以先理解迭代器为指针
- 迭代器种类：
- 输入迭代器：
- 对数据的只读访问
- 只读，支持++、==、！=
- 输出迭代器：
- 对数据的只写访问
- 只写，支持++
- 前向迭代器：
- 读写操作，并能向前推进迭代器
- 读写，支持++、==、！=
- 双向迭代器（常用）：
- 读写操作，并能向前和向后操作
- 读写，支持++、--，
- 随机访问迭代器（常用）：
- 读写操作，可以以跳跃的方式访问任意数据，功能最强的迭代器
- 读写，支持++、--、[n]、-n、<、<=、>、>=

### 2.5 容器算法迭代器例子
- 容器： vector
- 算法： for_each
- 迭代器： vector<int>::iterator

### auto和decltype关键字
- auto：占位符类型说明符
- auto x=expr;//从初始化器推导类型
- decltype：说明符
- decltype(expr);//什么从表达式得到的类型

```cpp
#include <vector>
#include <iostream>                                                 
using namespace std;
int main()
{
    vector<int> ivec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};      

    vector<int>::iterator it=ivec.begin();//直接求迭代器
    cout<<*it<<endl;

    auto itt=ivec.begin();//auto得到迭代器
    cout<<*itt<<endl;

    decltype(ivec.begin()) ittt=ivec.begin();//decltype得到迭代器
    cout<<*ittt<<endl;
    for(auto i:ivec)
    {
        cout<<i<<" ";
    }    
    system("pause");
    return 0;
}


int main()
{
	vector<int> ivec(10, 2);			 // 创建含10个值为2的元素的vector容器
	vector<int>::iterator iter;			 // 声明迭代器对象
	//ex:auto and decltpye
	//decltype(ivec.begin())  iter;
	vector<int>::reverse_iterator riter; // 声明迭代器对象

	iter = ivec.begin(); // 获取指向第0个元素的迭代器
	*iter += 10;		 // 将第0个元素的值加10

	riter = ivec.rend(); // 逆向迭代器 riter指向第0个元素的前一位置
	*(riter - 1) += 10;	 // 将第0个元素的值加10

	iter = ivec.end(); // iter指向最后一个元素的下一位置
	*(iter - 1) = 100; // 将最后一个元素的值改为100

	riter = ivec.rbegin(); // 逆向迭代器 riter指向最后一个元素
	*riter -= 20;		   // 将最后一个元素的值减20
	
	for (auto it = ivec.begin(); it != ivec.end(); it++ ) {
		std::cout  << *it << "," ;
	}
	std::cout << std::endl;
	
	for (auto it = ivec.rbegin(); it != ivec.rend(); it++ ) {
		std::cout  << *it << "," ;
	}
	std::cout << std::endl;	
}
```
