## 从 C 到 C++ 

### 头文件
```cpp
/* iostream-basic.cpp */
#include<iostream>
using namespace std;//表示可使用std命名空间内成员名称的指令 不需要std::
//标准库中的符号（类型、遍历、常量、函数等）都在std命名空间块中声明
#include<string>
#include<iomanip>//定义了一些有参的操纵符函 setw(n)输出宽度 setprecision(n)浮点精度
```


### IO流

- endl 换行
- dec、hex、oct 进制输出
- left、right 左右对齐
- fixed 浮点IO的格式化
- showpoint、noshowpoint 控制浮点表示是否始终包含小数点
- showpoint、noshowpoint 控制是否使用+号

```cpp
int main() {
    double myFloat=123.4578;
    int myInt=5;
    
    cout << fixed << showpoint << setprecision(3);//浮点数精度为小数点后3  对后面所有的内容都有效
    
    cout << setw(10) << left << "Float"<<endl;//宽度10 左对齐
    cout << setw(12) << right << myFloat << endl;//宽度12 右对齐
    cout << setw(10) << left << "Int"<<endl;
    cout << setw(12) << right << myInt << endl;    
    system("pause");
    return 0;
}
```

```cpp
int main()
{
//     string name;
//     cin>>name;
    string name2;
    getline(cin,name2);//使用getline读取 不会遇到空格停止
    cout<<name2<<endl;
    system("pause");
    return 0;
}
```

```cpp
namespace sysu {
    namespace students {
        int collegeCount;
        void printColleges();
    }
}
void sysu::students::printColleges() {
    cout << "Colleges " << collegeCount << endl;
}
int main() {
    using namespace sysu::students;
    collegeCount = 23;
    printColleges();
    system("pause");
    return 0;
}
```


### string类

```cpp
int main() {
    //初始化
    string s1;
    string s2 = "c++";//是初始化 不是赋值操作
    string s3 = s2;
    string s4 (5, 's');
    return 0;
}
```

```cpp
int main() {
    string s1="hello ",s2,s3;
    cin >> s2;
    s3 = s1 + s2;// +
    cout << s3 << endl;
    
    s1 += {'s','y','s','u'}; //没\0 
    cout << s1 << endl;
    //请自己编程测试其他运算     
    system("pause");
    return 0;
}
```


#### string的基本操作
```cpp
int main() {
    string s = "sysu-computer";
    int len = s.length();//求长度
    const char* cs = s.c_str();//返回const char*的C风格字符串, cs1和s共享空间
    cout << cs << " len is " << len << endl;
    system("pause");
    return 0;
}
```

```cpp
int main() {
    string s = "sysu-computer";
    int pos = s.find("computer");//查找 返回字串的第一个字符
    s.replace(pos,8,"software");//替换从pos起长度为8的字符串为software
    cout << s << endl;
    return 0;
}
```

```cpp
int main() {
    string s = "ysu";
    s.insert(0,1,'S').append(1,'-').append("Xcomputer");//插入:insert在第0个位置加1个S； append追加n个字符-、追加Xcomputer
    s.erase(5,1);//删除 从第5个下标开始的1个长度的字符串
    cout << s << endl;
    s.clear();//清空
    system("pause");
    return 0;
}
```

```cpp
int main() {
    string s = "Sysu-Computer";
    cout << s.substr(0,4) << " " << endl;//substr返回s从下标0开始长度为4的字串
    if (s.compare(5,8,"Computer")==0) {//字串比较
        cout << "OK!" << endl;  
    } 
    system("pause");
    return 0;
}
```


### const与constexpr

- const:表示“只读”
- constexpr:表示“常量” 在编译器就确定值

```cpp
void dis_1(const int x)
{
    //错误，x是只读的变量
    array<int, x> myarr{1, 2, 3, 4, 5};//变量无法用来初始化array
    cout << myarr[1] << endl;
}

int len(int a,int b)//错误
{
    return a+b;
}
```

```cpp
void dis_2()
{
    const int x = 5;//常量
    array<int, x> myarr1{1, 2, 3, 4, 5};
    cout << myarr1[1] << endl;
}

constexpr int len(int a,int b)
{
    return a+b;
}

int main()
{
    //dis_1(5);
    dis_2();
    array<int,len(2,3)> myarray2;
    system("pause");
    return 0;
}
```

### auto 占位类型说明符

在编译期根据表达式自动推导类型

```cpp
int main()
{
    string s="hello world";
    for(auto i:s)//自动推导类型 遍历s
    {
        cout<<i<<" ";//输出s的每一个元素
    }
    cout<<endl;
    system("pause");
    return 0;
}
```