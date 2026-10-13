## 异常处理概述
- 程序终止
- 执行过程终止（故障 系统崩溃）
- 执行中终止（异常 可挽救）

- 采用结构化方法对程序error进行显示管理，处理可预料的error
- 异常检测 和 异常处理 分离

- 抛出异常
- 语法： throw 表达式
- 捕捉异常并处理
- 语法： try...catch...

```cpp
//C语言例子
int main () {
    FILE * pf;
    try {
        pf = fopen ("unexist.txt", "rb");
        if (pf == NULL) throw errno;//抛出异常 找不到文件
        //...
        fclose (pf);
    } 
    catch (int errnum) {//捕获异常 执行下面代码
       if (errnum!=2) fclose (pf);
       else perror("unexist.txt");
    }
    //如果抛出异常但没有匹配的catch处理代码，则自动调用标准库函数terminate
    system("pause");
    return 0;
}
```

### 异常定义和抛出
- throw：
- 语法： throw 表达式
- 操作：
- 复制被抛出的对象为临时对象，作为异常处理
- 如果在try块中，则异常后终止try，执行catch语句处理

### 标准异常基类<exception>
- 其下有多个子类
- 比如：包含bad_alloc 头文件<new>

```cpp
#include <new>          // 头文件 std::bad_alloc
int main () {
    try
    {
        int* myarray= new int[10000];
    }
    catch (bad_alloc& ba)
    {
        cerr << "bad_alloc caught: " << ba.what() << '\n';
    }
    system("pause");
    return 0;
}

#include <stdexcept>      //头文件 std::out_of_range
int main (void) {
    vector<int> myvector(10);
    try {
        myvector.at(20)=100;      // vector::at throws an out-of-range
    }
    catch (const out_of_range& oor) {
        cerr << "Out of Range error: " << oor.what() << '\n';
    }
    return 0;
}
```

### 异常的捕获和处理
- try-catch：
- try块一旦遇到异常则终止try块执行
- try块后至少要有一个异常声明
- 异常声明对象最好使用引用
- 使用...作为异常声明处理未预料的其他异常，放在最后

```cpp
#include <stdexcept>      // std::invalid_argument
#include <cmath>

double mySqrt(double dnum)
{
    if (dnum < 0)//异常
        throw std::invalid_argument("argument dnum must be great than zero");
    return std::sqrt(dnum); //合法
}

int main() {
    try {
        mySqrt(-1);
    }
    catch (std::exception& e) {//使用引用
        std::cout << e.what() << '\n';
    }
    return 0;
}
```

### 在多个catch中 子类必须比父类优先catch
- 用基类引用类型捕获异常，且虚函数才能产生多态

```cpp
class EBase {
//EX1：添加 virtual 比较输出不同 
public: void what() { cout << "base" << endl;};
};

class EDrived:public EBase {
public: void what() { cout << "EDrived" << endl; };
};

int main() {
    try {
        throw EDrived();
    }
    //注意：捕获的次序，子类必须在前面 
    catch (EBase& e) { e.what();}
    catch (EDrived& e) { e.what();}
    return 0;
}
```

### 异常传播
- 从异常抛出到控制转移给合适的异常处理语句的过程就叫异常传播
- 当前函数未处理异常，则交给调用函数，一个个函数传下去

```cpp
void f3(int x){
	switch (x) {
    case 1: throw 3.4;	      // 抛出double型异常
    case 2: throw 2.5f; 	    // 抛出float型异常
    case 3: throw 1;   	      // 抛出int型异常
    case 5: throw exception(); //抛出标准异常
	}
	cout << "End of f3" << endl;
}

void f2(int x) {
	try {
    f3(x);
	}
  catch (int) {	    //int型异常的处理代码
    cout << "An int exception occurred!--from f2" << endl;  
	}
  catch (float) { 	//float型异常的处理代码
    cout << "A float exception occurred!--from f2" << endl;
	}
	cout << "End of f2" << endl;
}

void f1(int x) {
	try {
    f2(x);
	}
  catch (int) {  	            // int型异常的处理代码
    cout << "An int exception occurred!--from f1" << endl;
	}
  catch (float) {  	            // float型异常的处理代码
    cout << "A float exception occurred!--from f1" << endl;
	}
  catch (double) { 	            // double型异常的处理代码
    cout << "A double exception occurred!--from f1" << endl;
	}
	cout << "End of f1" << endl;
}

int main()
{
	for (int i = 1; i < 6; i++) {
    cout << "-------- intput i = " << i << endl;
    f1(i);
	}

	cout << "End of main" << endl;
	system("pause");
	return 0;
}
```

### 限定异常说明
- except specification 指定要抛出什么异常
- noexcept关键字 说明是否允许抛出异常
- 异常重新抛出
- 抛出该catch语句原来所捕获的异常，由函数调用链中更上层的函数处理（所以要使用引用&来声明异常）
- 语法：在catch语句里再使用throw

### 异常编程与实践

```cpp
#include <stdexcept> //从stdexcept包继承异常 常用基类有logic_error和runtime_error等
#include <iostream>
using namespace std;

class Divided_By_Zero:public runtime_error{
public:
    explicit Divided_By_Zero(const string& s = "Divide By Zero"):runtime_error(s){};
    explicit Divided_By_Zero(const char* s):runtime_error(string(s)){};
};

int main() {
    double a = 10, b = 0, res;
    char Operator = '/';
    try {
        if (b == 0) throw Divided_By_Zero();
        res = a / b;
        cout << a << " / " << b << " = " << res;
    }
    catch(const exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}


#include <iostream>
class Dummy {
public:
    ~Dummy() {
        std::cout << "Dummy destructed\n";
    }
};

class Triangle {
public:
    Triangle(int a=3,int b=4,int c=5) {
        if (a < 0) { throw a;}//构造函数异常的对象，不会分配空间，也不会进行后续的析构
    } 
    ~Triangle() {
        std::cout << "Triangle destructed\n";
    }
private:
    int a,b,c;
};

int main() {
    try {
        Dummy dum;
        //if new or alloc something? 
        Triangle tri(-1);
        std::cout << "Triangle created\n"; 
    }
    catch (...) {
        std::cout << "Exception catched\n"; 
    }
}
```
