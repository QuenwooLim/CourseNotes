## 函数

### 1. 函数的参数列表中的形参是可以有默认值的

- 如果某个位置参数有默认值，那么从这个位置往后，从左向右，必须都要有默认值
- 如果函数声明有默认值，函数实现的时候就不能有默认参数
```cpp
int func(int a, int b = 10, int c = 10) {
	return a + b + c;
}
int func2(int a = 10, int b = 10);//函数声明
int func2(int a, int b) {//函数实现 不能有默认参数
	return a + b;
}

int main() {

	cout << "ret = " << func(20, 20) << endl;
	cout << "ret = " << func(100) << endl;

	system("pause");
	return 0;
}
```

### 2. 函数占位参数

- 函数的形参列表里可以有占位参数，只写类型用来做占位
- 调用函数时必须填补该位置

```cpp
void func(int a, int) {//第二个参数为占位参数
	cout << "this is func" << endl;
}

int main() {
	func(10,10); //占位参数必须填补
	system("pause");
	return 0;
}
```


### 3. 函数重载

#### 函数重载满足条件

- a.同一个作用域下
- b.函数名称相同
- c.函数参数类型不同 或者 个数不同 或者 顺序不同
- 注意: 函数的返回值不可以作为函数重载的条件，即不能只改变返回值类型来实现函数重载

```cpp
void func()
{
	cout << "func 的调用！" << endl;
}
void func(int a)
{
	cout << "func (int a) 的调用！" << endl;
}
void func(int a ,double b)
{
	cout << "func (int a ,double b) 的调用！" << endl;
}

int main() {

	func();
	func(10);
	func(10,3.14);
	
	system("pause");
	return 0;
}
```


#### 函数重载注意事项

- a.引用作为重载条件
- b.函数重载碰到函数默认参数的情况需要避免

```cpp
void func(int &a)//引用作为重载条件
{
	cout << "func (int &a) 调用 " << endl;
}
void func(const int &a)
{
	cout << "func (const int &a) 调用 " << endl;
}

void func2(int a, int b = 10)//函数重载碰到函数默认参数
{
	cout << "func2(int a, int b = 10) 调用" << endl;
}
void func2(int a)
{
	cout << "func2(int a) 调用" << endl;
}

int main() {
	int a = 10;
	func(a); //调用无const
	func(10);//调用有const
	//func2(10); //碰到默认参数产生歧义，需要避免

	system("pause");
	return 0;
}
```