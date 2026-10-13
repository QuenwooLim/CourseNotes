## 1.模板

### 1.1模板
- 概念
- 通用的模具，大大提高复用性
- 模板的特点：
- 模板不可以直接使用，它只是一个框架
- 模板的通用并不是万能的

### 1.2 函数模板
- C++另一种编程思想称为泛型编程，主要利用的技术就是模板
- 泛型编程：
- 独立于任何特定数据类型的编程，使得不同类型的数据可以被相同的代码操作
- 使用模板进行泛型编程，包括：C++提供两种模板机制:函数模板和类模板
- 从通用模板到实例代码，具体数据类型才能确定
- 泛型编程是一种编译时多态（静态多态），数据类型本身是参数化的

#### 1.2.1 函数模板语法
- template<typename T1,typename T2>
- T为虚拟类
- 提高代码复用性，处理未知的类型

```cpp
//利用模板提供通用的交换函数 不再需要根据每个类型编写一个函数
template<typename T>
void mySwap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

int main() {
	int a = 10;
	int b = 20;

	//利用模板实现交换
	mySwap(a, b);//1、自动类型推导
	mySwap<int>(a, b);//2、显示指定类型

	cout << "a = " << a << endl;
	cout << "b = " << b << endl;

	system("pause");
	return 0;
}
```

#### 1.2.2 注意事项
- 必须使用一致的数据类型T
- 模板必须要确定出T的数据类型

```cpp
template<class T>
void mySwap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}
// 1、自动类型推导，必须推导出一致的数据类型T,才可以使用
void test01()
{
	int a = 10;
	int b = 20;
	char c = 'c';
	mySwap(a, b); // 正确，可以推导出一致的T
	//mySwap(a, c); // 错误，推导不出一致的T类型
}


// 2、模板必须要确定出T的数据类型，才可以使用
template<class T>
void func()
{
	cout << "func 调用" << endl;
}
void test02()
{
	//func(); //错误，模板不能独立使用，必须确定出T的类型 要么按照test01例子的默认推导
	func<int>(); //利用显示指定类型的方式，给T一个类型，才可以使用该模板
}

int main() {
	test01();
	test02();
	system("pause");
	return 0;
}
```

#### 1.2.4 普通函数和模板函数的区别
- 普通函数调用：
- 可以发生自动类型转换（隐式类型转换）
- 函数模板调用时
- 如果利用自动类型推导的方式，不会发生隐式类型转换
- 如果利用显示指定类型的方式，可以发生隐式类型转换

```cpp
//普通函数
int myAdd01(int a, int b)
{
	return a + b;
}

//函数模板
template<class T>
T myAdd02(T a, T b)  
{
	return a + b;
}

int main() {
	int a = 10;
	int b = 20;
	char c = 'c';
	cout << myAdd01(a, c) << endl; //正确，普通函数调用 可类型转换

	//myAdd02(a, c); // 报错，函数模板调用，使用自动类型推导时，不会发生隐式类型转换
	cout<<myAdd02<int>(a, c); //正确，如果用显示指定类型，可以发生隐式类型转换

	system("pause");
	return 0;
}
```

#### 1.2.5 调用规则
- 可以通过空模板参数列表来强制调用函数模板
- 函数模板也可以发生重载
- 如果函数模板可以产生更好的匹配,优先调用函数模板

```cpp
void myPrint(int a, int b)
{
	cout << "调用的普通函数" << endl;
}

template<typename T>
void myPrint(T a, T b) 
{ 
	cout << "调用的模板" << endl;
}

template<typename T>
void myPrint(T a, T b, T c) 
{ 
	cout << "调用重载的模板" << endl; 
}

int main() {
	//1、如果函数模板和普通函数都可以实现，优先调用普通函数
	// 注意 如果告诉编译器  普通函数是有的，但只是声明没有实现，或者不在当前文件内实现，就会报错找不到
	int a = 10;
	int b = 20;
	myPrint(a, b); //调用普通函数

	//2、可以通过空模板参数列表来强制调用函数模板
	myPrint<>(a, b); //空模板 <> 

	//3、函数模板也可以发生重载
	int c = 30;
	myPrint(a, b, c); //调用重载的函数模板

	//4、 如果函数模板可以产生更好的匹配,优先调用函数模板
	char c1 = 'a';
	char c2 = 'b';
	myPrint(c1, c2); //调用函数模板
	system("pause");
	return 0;
}
```

### 调用优先级：
- 如果函数模板和普通函数都可以实现，优先调用普通函数
- 如果能找到具体化（特化）的模板，调用之
- 如果能从同名的函数模板实例化一个函数实例，若形参类型和实参类型完全一致则调用之
- 对函数调用的实参进行隐式类型转换后与非模板函数进行匹配，若能一致则调用之

```cpp
// 函数模板demoPrint
template <typename T>
void demoPrint(const T v1, const T v2)
{
    cout << "the first version of demoPrint()" << endl;
    cout << "the arguments: " << v1 << " " << v2 << endl;
}
// 函数模板demoPrint的指定特殊
template <>
void demoPrint(const char v1, const char v2)
{
    cout << "the specify special of demoPrint()" << endl;
    cout << "the arguments: " << v1 << " " << v2 << endl;
}
// 函数模板demoPrint重载的函数模板
template <typename T>
void demoPrint(const T v)
{	
    cout << "the second version of demoPrint()" << endl;
    cout << "the argument: " << v << endl;
}
// 非函数模板demoPrint
void demoPrint(const double v1, const double v2)
{	
    cout << "the nonfunctional template version of demoPrint()" << endl;
    cout << "the arguments: " << v1 << " " << v2 << endl;
}

int main()
{
    string s1("rabbit"), s2("bear");
    char c1('k'), c2('b');
    int iv1 = 3, iv2 = 5;
    double dv1 = 2.8, dv2 = 8.5;
    // 调用第一个函数模板
    demoPrint(iv1, iv2);
    // 调用第一个函数模板的指定特殊
    demoPrint(c1, c2);
    // 调用第二个函数模板
    demoPrint(iv1);
    // 调用非函数模板(普通函数优先匹配)
    demoPrint(dv1, dv2);
    // 隐式转换后调用非函数模板
    demoPrint(iv1, dv2);    
    // 调用第一个函数模板 模板指定类型，可以进行隐式转换
    demoPrint<int>(iv1, dv2);    
    system("pause");
    return 0;
}
```

#### 1.2.6 模板的局限性与具体化（特化）
- 不使用自定义类

- 模板具体化（特化）：
- 自定义类模板来拓展原来的普通模板
- 语法 template<> bool myCompare(Person &p1, Person &p2)
- 多加了个template<>

```cpp
class Person //自定义类
{
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
	string m_Name;
	int m_Age;
};

//普通函数模板
template<class T>
bool myCompare(T& a, T& b)
{
	if (a == b)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//具体化，显示具体化的原型和定意思以template<>开头，并通过名称来指出类型
//具体化优先于常规模板
template<> bool myCompare(Person &p1, Person &p2)
{
	if ( p1.m_Name  == p2.m_Name && p1.m_Age == p2.m_Age)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void test01()
{
	int a = 10;
	int b = 20;
	//内置数据类型可以直接使用通用的函数模板
	bool ret = myCompare(a, b);
	if (ret)
	{
		cout << "a == b " << endl;
	}
	else
	{
		cout << "a != b " << endl;
	}
}

void test02()
{
	Person p1("Tom", 10);
	Person p2("Tom", 10);
	//自定义数据类型，不会调用普通的函数模板
	//可以创建具体化的Person数据类型的模板，用于特殊处理这个类型
	bool ret = myCompare(p1, p2);
	if (ret)
	{
		cout << "p1 == p2 " << endl;
	}
	else
	{
		cout << "p1 != p2 " << endl;
	}
}

int main() {
	test01();
	test02();
	system("pause");
	return 0;
}
```

### 1.3 类模板
- 建立一个通用类，类中的成员 数据类型可以不具体制定，用一个虚拟的类型来代表

#### 1.3.1 类模板的语法
- template<typename T>
- 类

- 类模板是一个通用类模板，不能用于创建对象，只有经过实例化才可以得到类去创建对象

```cpp
#include <string>
//类模板
template<class NameType, class AgeType> 
class Person
{
public:
	Person(NameType name, AgeType age)
	{
		this->mName = name;
		this->mAge = age;
	}
	void showPerson()
	{
		cout << "name: " << this->mName << " age: " << this->mAge << endl;
	}
public:
	NameType mName;
	AgeType mAge;
};

int main() {
	Person<string, int>P1("孙悟空", 999);// 指定NameType 为string类型，AgeType 为 int类型
	P1.showPerson();
	Person<string,string>P2("猪八戒","666");// 指定NameType 为string类型，AgeType 为 string类型
	P2.showPerson();
	system("pause");
	return 0;
}
```

#### 1.3.2 类模板和函数模板的区别
- 类模板没有自动类型推导的使用方式
- 类模板在模板参数列表中可以有默认参数

```cpp
template<class NameType, class AgeType = int> 
class Person
{
public:
	Person(NameType name, AgeType age)
	{
		this->mName = name;
		this->mAge = age;
	}
	void showPerson()
	{
		cout << "name: " << this->mName << " age: " << this->mAge << endl;
	}
public:
	NameType mName;
	AgeType mAge;
};

//1、类模板没有自动类型推导的使用方式
void test01()
{
	// Person p("孙悟空", 1000); // 错误 类模板使用时候，不可以用自动类型推导
	Person <string ,int>p("孙悟空", 1000); //必须使用显示指定类型的方式，使用类模板
	p.showPerson();
}

//2、类模板在模板参数列表中可以有默认参数
void test02()
{
	Person <string> p("猪八戒", 999); //指定默认参数，不需要传第二个int
	p.showPerson();
}

int main() {
	test01();
	test02();
	system("pause");
	return 0;
}
```

#### 1.3.3 类模板中成员函数创建时机
- 类模板中成员函数和普通类中成员函数创建时机是有区别的：
- 普通类中的成员函数一开始就可以创建
- 类模板中的成员函数在调用时才创建

```cpp
class Person1//类1
{
public:
	void showPerson1()
	{
		cout << "Person1 show" << endl;
	}
};

class Person2//类2
{
public:
	void showPerson2()
	{
		cout << "Person2 show" << endl;
	}
};

template<class T>//类模板
class MyClass
{
public:
	T obj;
	void fun1() { obj.showPerson1(); }
	void fun2() { obj.showPerson2(); }//可以创建 但是编译时候会报错

};

int main() {
	MyClass<Person1> m;
	m.fun1();
	//m.fun2();//编译会出错，说明函数调用才会去创建成员函数
	system("pause");
	return 0;
}
```

#### 1.3.4类模板对象做函数参数
- 类模板实例化出的对象，向函数传参的三种传入方式：
- 指定传入的类型 --- 直接显示对象的数据类型
- 参数模板化 --- 将对象中的参数变为模板进行传递
- 整个类模板化 --- 将这个对象类型 模板化进行传递

```cpp
template<class NameType, class AgeType = int> 
class Person
{
public:
	Person(NameType name, AgeType age)
	{
		this->mName = name;
		this->mAge = age;
	}
	void showPerson()
	{
		cout << "name: " << this->mName << " age: " << this->mAge << endl;
	}
public:
	NameType mName;
	AgeType mAge;
};

//1、指定传入的类型
void printPerson1(Person<string, int> &p) 
{
	p.showPerson();
}
void test01()
{
	Person <string, int >p("孙悟空", 100);
	printPerson1(p);
}

//2、参数模板化
template <class T1, class T2>
void printPerson2(Person<T1, T2>&p)
{
	p.showPerson();
	cout << "T1的类型为： " << typeid(T1).name() << endl;
	cout << "T2的类型为： " << typeid(T2).name() << endl;
	//typeid(T1).name() 会返回模板参数 T1 的类型名称。
	//typeid(T2).name() 会返回模板参数 T2 的类型名称
}
void test02()
{
	Person <string, int >p("猪八戒", 90);
	printPerson2(p);
}

//3、整个类模板化
template<class T>
void printPerson3(T & p)
{
	cout << "T的类型为： " << typeid(T).name() << endl;
	p.showPerson();

}
void test03()
{
	Person <string, int >p("唐僧", 30);
	printPerson3(p);
}

int main() {
	test01();
	test02();
	test03();
	system("pause");
	return 0;
}
```

#### 1.3.6 类模板成员函数类外实现
- 类模板中成员函数类外实现时，需要加上模板参数列表
- 语法：
- template<模板形参表>
- 返回值类型 类模板名<模板形参名列表>::函数名(函数形参列表)

```cpp
template<class T1, class T2>
class Person {
public:
	//成员函数类内声明
	Person(T1 name, T2 age);
	void showPerson();

public:
	T1 m_Name;
	T2 m_Age;
};

//构造函数 类外实现
template<class T1, class T2>
Person<T1, T2>::Person(T1 name, T2 age) {
	this->m_Name = name;
	this->m_Age = age;
}

//成员函数 类外实现
template<class T1, class T2>
void Person<T1, T2>::showPerson() {
	cout << "姓名: " << this->m_Name << " 年龄:" << this->m_Age << endl;
}
```

### 类型模板形参和非类型模板形参
- 非类型模板形参：
- 相当于模板内部的常量
- 形式上类似于普通的函数形参
- 队模板进行实例化时，非类型形参由相应模板实参的值代替
- 对应的模板实参必须是编译时常量表达式

### 非模板形参实例1

### 类模板：使用数组实现的栈

```cpp
template <typename ElementType, int N>                  
class Stack {
public:
    Stack();
    ~Stack();
    void push(ElementType obj);      
    void pop();                      
    ElementType getTop() const;     
    bool isEmpty() const;

private:
```

### 栈节点类型

```cpp
    ElementType elements[N];
    int count;
};
```

### 构造函数

```cpp
template <typename ElementType, int N>    
Stack<ElementType, N>::Stack()
{
    memset(elements, 0, sizeof(elements));
    count = 0;
}
```

### 析构函数

```cpp
template <typename ElementType, int N>    
Stack<ElementType, N>::~Stack()
{
    
}
```

### 向栈内压入元素

```cpp
template <typename ElementType, int N>    
void Stack<ElementType, N>::push( ElementType obj )
{
    elements[count++] = obj;
}
```

### 从栈顶弹出元素

```cpp
template <typename ElementType, int N>    
void Stack<ElementType, N>::pop()
{
    count--;
}
```

### 获取栈顶元素

```cpp
template <typename ElementType, int N>    
ElementType Stack<ElementType, N>::getTop() const 
{
    return elements[count-1];
}
```

### 判断栈是否为空

```cpp
template <typename ElementType, int N>    
bool Stack<ElementType, N>::isEmpty() const
{
    return count == 0;
}

int main()
{
    Stack<int, 10> stack;//传入实参数10
    
    for (int i = 1; i < 9; i++)
	stack.push(i);
	
    while (!stack.isEmpty()) {//输出	      
	    cout << stack.getTop() << " ";      
	    stack.pop();			          
    }
    system("pause");
    return 0;
}
```

### 数组的引用或指针

```cpp
template <typename T, int N>
void printValues(T (&arr)[N]) {//这里接收数组时传入的参数要与N相等，即保证传入的数组和接收的数组长度一致
	cout << "(T&)[" << N << "]\n";
    for (int i = 0; i != N; ++i)
	    cout<< arr[i] << ","; 
	cout << endl;
}


template <typename T, int N>
void printValues(T (*arr)[N]) {//使用指针接收元素
	cout << "(T*)[" << N << "]\n"; 
    for (int i = 0; i != N; ++i)
	    cout<< *arr[i] << ","; 
	cout << endl;
}
```

### 不使用非模板形参实现

```cpp
template <typename T>
void printValues(T arr[], int N) {//这里接收数组时不指定长度，但需要另外有一个参数用于后续访问防越界
//void printValues(T *arr, int N) {//等效
	cout << "T*" << "\n";
    for (int i = 0; i != N; ++i)
	    cout<< arr[i] << ","; 
	cout << endl;
}

int main()
{
    int intArr[6] = {1, 2, 3, 4, 5, 6};
    double dblArr[4] = {1.2, 2.3, 3.4, 4.5};

    // 生成函数实例printValues(int (&) [6])
    printValues(intArr);	
    // 生成函数实例printValues(double (&) [4])
    printValues(dblArr);
	//重载 ?
	//printValues(&dblArr);
	//  
	printValues(intArr, 6);	
    system("pause");
    return 0;
}
```

#### 1.3.7类模板分文件编写
- 与一般情况不同，模板要进行实例化，编译器必须能够访问模板定义的源代码
- 为了在模板中实现一般的声明定义分离，C++提供两种方法能够访问模板定义的源代码
- 解决方式1：包含编译模式
- 直接包含.cpp源文件（原来只需要包含.h文件） 在头文件+#include<xxx.cpp>
- 或 将声明和实现写到同一个文件中，并更改后缀名为.hpp，hpp是约定的名称，并不是强制
- 解决方式2：分离编译模式
- 声明和定义分离，在实现文件中使用保留字export告诉编译器需要记住那些模板定义（不是所有编译器都支持）

#### 1.3.8 类模板和友元
- 全局函数类内实现 - 直接在类内声明友元即可 （建议全局函数做类内实现，用法简单）
- 全局函数类外实现 - 需要提前让编译器知道全局函数的存在

```cpp
template<class T1, class T2> class Person;
// //如果声明了函数模板，可以将实现写到后面，否则需要将实现体写到类的前面让编译器提前看到
// template<class T1, class T2> void printPerson2(Person<T1, T2> & p); //----------------------

template<class T1, class T2>
void printPerson2(Person<T1, T2> & p)
{
	cout << "类外实现 ---- 姓名： " << p.m_Name << " 年龄：" << p.m_Age << endl;
}

template<class T1, class T2>
class Person
{
	//1、全局函数配合友元   类内实现
	friend void printPerson(Person<T1, T2> & p)
	{
		cout << "类内实现 ---- 姓名： " << p.m_Name << " 年龄：" << p.m_Age << endl;
	}
	//全局函数配合友元  类外实现
	friend void printPerson2<>(Person<T1, T2> & p);
public:
	Person(T1 name, T2 age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
private:
	T1 m_Name;
	T2 m_Age;

};

// //搭配前面 先声明后实现
// template<class T1, class T2>
// void printPerson2(Person<T1, T2> & p)//------------------------------------------------------
// {
// 	cout << "类外实现 ---- 姓名： " << p.m_Name << " 年龄：" << p.m_Age << endl;
// }


//1、全局函数在类内实现
void test01()
{
	Person <string, int >p("Tom", 20);
	printPerson(p);
}

//2、全局函数在类外实现
void test02()
{
	Person <string, int >p("Jerry", 30);
	printPerson2(p);
}

int main() {
	test01();
	test02();
	system("pause");
	return 0;
}
```
