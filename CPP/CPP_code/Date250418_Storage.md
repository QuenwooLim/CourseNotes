## 存储

### 内存分区模型

将内存大方向划分为4个区域

1. 代码区：
- 存放函数体的二进制代码，由操作系统进行管理的
- 共享的、只读的

2. 全局区（数据段）：
- 存放全局变量、静态变量、常量区（字符串常量和其他常量）

3. 栈区：
- 由编译器自动分配释放, 存放函数的参数值,局部变量等
- 注意不要返回局部变量的地址，栈区开辟的数据由编译器自动释放

4. 堆区：
- 由程序员分配和释放,若程序员不释放,程序结束时由操作系统回收
- 在C++中主要利用new在堆区开辟内存，delete释放


#### 代码示例
```cpp
//全局变量
int g_a = 10;

//全局常量
const int c_g_a = 10;

int main() {

	//局部变量
	int a = 10;

    string s="hello world";
    //字符串常量
    cout << "字符串常量地址为： " << &s<< endl;

	//静态变量
	static int s_a = 10;

    //new开辟动态内存
    int* new_a=new int(10);
    //delete释放内存
    delete new_a;


	system("pause");
	return 0;
}
```


### new操作符

- C++中使用new操作符在堆区开辟内存，返回值是指针类型
- 使用delete释放内存


```cpp
int* func()
{
    //在堆区开辟内存
    int *p = new int(10);
    //返回指针
    return p;
}
int main()
{
    int *p=func();

    //释放堆区内存
    delete p;

    system("pause");
    return 0;
}
```


### 开辟数组

```cpp
int main() {

	int* arr = new int[10];//开辟10个int类型的数组

	for (int i = 0; i < 10; i++)
	{
		arr[i] = i + 100;
	}

	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << endl;
	}
	//释放数组 delete 后加 []
	delete[] arr;

	system("pause");

	return 0;
}
```