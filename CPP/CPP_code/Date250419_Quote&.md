## 引用&

### 1. 给变量起别名

语法： 数据类型 &别名=原名

```cpp
int main()
{
    int a=10;
    int &b=a; // b是a的别名，b和a指向同一块内存
    cout<<a<<" "<<b<<endl;
    b=100;
    cout<<a<<" "<<b<<endl;
    system("pause");
    return 0;
}
```


### 2. 引用的注意事项

- 引用必须初始化
- 初始化后，可以改变值，但不可以改变引用的指向

```cpp
int main() {

	int a = 10;
	int b = 20;
	//int &c; //错误，引用必须初始化
	int &c = a; //一旦初始化后，就不可以更改
	c = b; //这是赋值操作，不是更改引用

	system("pause");
	return 0;
}
```

### 3. 引用做函数参数

函数传参，使用引用可以直接给实参

```cpp
//值传递 不会改变实参
void mySwap01(int a, int b) {
	int temp = a;
	a = b;
	b = temp;
}
//地址传递 会改变实参
void mySwap02(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
//引用传递 会改变实参，兼得上述两者优点
void mySwap03(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}
int main() {

	int a = 10;
	int b = 20;
	mySwap01(a, b);
	mySwap02(&a, &b);
	mySwap03(a, b);

	system("pause");
	return 0;
}
```

### 4. 引用做函数返回值

- 注意不要返回局部变量的引用
- 使用函数调用作为左值

```cpp
//返回局部变量引用
int& test01() {
	int a = 10; //局部变量
	return a;
}

//返回静态变量引用
int& test02() {
	static int a = 20;
	return a;
}

int main() {
	//不能返回局部变量的引用
	int& ref = test01();

	//如果函数做左值，那么必须返回引用
	int& ref2 = test02();

	test02() = 1000; //修改的就是ref2同一个指向的值

	cout << "ref2 = " << test02() << endl;
	cout << "ref2 = " << ref2 << endl;

	system("pause");

	return 0;
}
```


### 5. 引用的本质

- 在C++内部是一个指针常量
- 所有的指针操作编译器都帮我们做了


### 6. 常量引用

常量引用主要来修饰形参 防止对实参造成误操作

```cpp
void showValue(const int& v) { //参数const+引用
	//v += 10;//无法修改
	cout << v << endl;
}

int main() {

	//int& ref = 10;  //引用本身需要一个合法的内存空间，而不是指向数值，因此这行错误

	const int& ref = 10; //加入const

	//ref = 100;  //加入const后不可以修改变量
	cout << ref << endl;

	//函数中利用常量引用防止误操作修改实参
	int a = 10;
	showValue(a);

	system("pause");
	return 0;
}
```