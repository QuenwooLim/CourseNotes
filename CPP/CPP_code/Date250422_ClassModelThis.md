## 对象模型和this指针

- 静态联编技术
  - 指的是在编译阶段，就能直接使用代码段函数地址调用动态对象的方法。
  - 该方法只需要向非静态成员函数传送this指针，即可用静态函数调用实现动态调用的效果
- 优势： 
  - 对象布局与C结构内存布局一致，使得内存中对象便与其他语言程序库兼容
  - 高效率，高性能

### 1. 成员变量和成员函数分开存储

- 只有非静态成员变量才属于类的对象上
- 静态成员变量不建议使用this指针访问

```cpp
class Person {
public:
    Person() {
        mA = 0;
    }
    int mA;//非静态成员变量占对象空间
    static int mB; //静态成员变量不占对象空间
    void func() {//函数也不占对象空间，所有函数共享一个函数实例
        cout << "mA:" << this->mA << endl;//非static成员
        //cout <<this->mB<<endl;  //static成员不建议使用this访问 同时友元也没有this
        cout<<mB<<endl;
        cout<<Person::mB<<endl;
    }
    static void sfunc() {//静态成员函数也不占对象空间
        //cout << this->mA;//静态成员函数没有this指针
        //cout<<mA<<endl;//error
        cout<<mB<<endl;//只能访问类的静态数据和函数成员
    }
};
int Person::mB = 10;    //static需要在类外初始化 不带static保留字
int main()
{
    Person p1;
    p1.func();
    cout<<p1.mA<<endl;
    //cout<<Person::mA<<endl; //error mA非static属于变量
    cout<<p1.mB<<endl; //对象.访问static成员变量
    cout<<Person::mB<<endl; //类::访问static成员变量
    system("pause");
    return 0;
}
```

### 2. this指针

- this指针指向被调用的成员函数所属的对象
- this指针是隐含每一个非静态成员函数内的一种指针
- this指针不需要定义，直接使用即可
- this指针的用途：
  - 当形参和成员变量同名时，可用this指针来区分
  - 在类的非静态成员函数中返回对象本身，可使用return *this

```cpp
class Person
{
public:
	Person(int age)
	{
		this->age = age;//1、当形参和成员变量同名时，可用this指针来区分
	}
	Person& PersonAddPerson(Person p)
	{
		this->age += p.age;
		return *this;//返回对象本身
	}
	int age;
};

void test01()
{
	Person p1(10);
	cout << "p1.age = " << p1.age << endl;
	Person p2(10);
	p2.PersonAddPerson(p1).PersonAddPerson(p1).PersonAddPerson(p1);
	cout << "p2.age = " << p2.age << endl;
}

int main() {
	test01();
	system("pause");
	return 0;
}
```

### 3. 类的空指针访问成员函数

```cpp
class Person {
public:
    void ShowClassName() { 
        cout << "我是Person类!" << endl;
    }
    void ShowPerson() {
        if (this == NULL) {
            return;
        }
        cout << mAge << endl;
    }
public:
    int mAge;
};

void test01()
{
    Person * p = NULL;
    p->ShowClassName(); //空指针，可以调用成员函数
    p->ShowPerson();  //但是如果成员函数中用到了this指针，就不可以了
}

int main() {
    test01();
    system("pause");
    return 0;
}
```

### 4. C++ 中的const知识

- const常量
  - 相当于c++的constexpr，即编译期常量
- const与指针
  - 不能修改 **p: const int **p; 
  - 不能修改 *p: int* const *p;
  - 不能修改 p: int** const p;
- const与函数
  - 不能修改函数参数: int fun(const int i);
  - 返回const指针，必须强制类型转换赋值给非const指针: const int* fun();
- const引用
  - const引用右值对象（为右值对象开辟空间，并赋予别名）
    - const int& size=15;
    - const int& size=i+7;
  - const引用左值对象（直接赋予别名）
    - const int& size=i;
  - const引用隐式转型左值对象（开辟临时空间，并赋予别名）
    - const int& size=pi;
  - const引用作为函数参数和返回值
    - 作为函数形式参数，实参包含是右值、是左值、被隐式转换三种情况
    - 作为返回值，是返回临时对象的别名

```cpp
int main()
{
    // const引用右值 
    const int &c = 15;
    //编译器会给常量15一个临时空间，并赋予别名
    //等价: int temp=rValue; const int &c=temp; 
    int* p = (int *)&c;  //p 指向临时空间
    *p = 10;//临时空间指向10（操作可能违法）
    cout << c << endl; //10

    int b = 10;
    const int &a = b;
    //a = 12;  //a不能被赋值 只读
    b = 11;//别名a也跟着一起改
    printf("a=%d,b=%d\n", a, b);//11 11：a和b等价 输出相同

    double e = 3.14;
    const int &d = e; //类型不一样等价于： int temp = e; const int &d = temp
    //const引用绑定右值或类型不一致时，编译器会生成临时变量
    e = 11;
    printf("d=%d,e=%lf\n", d, e);//3 11.0000
    system("pause");
    return 0;
}
```

- const限定标识符是常量，不能取地址
- 可以显示转型获取右值指针
- 不能初始化为左值引用
- 可以隐式转型右值为常引用
- 如该常量的右值需要更改，则必要右值引用（课程不要求右值引用的内容）

```cpp
int main()
{
    const double pi = 3.14;  //编译没有分配空间，pi是右值 
    //double *r_pi = &pi;    //const限定标识符是常量，不能取地址
    double *p_pi = (double *)&pi;//可以显示转型获取右值指针 等价：
    //double temp = pi; double *p_pi = &temp;
    *p_pi = 4.0;
    printf("%lf,%lf\n",*p_pi,pi); //4 3.14
    
    //double &r_pi = pi; //语法错误，普通的左值引用（double &）不能绑定到常量或只读对象。只有 const double &（常量引用）才能绑定到常量。
    const double &r_pi = pi; //引用了pi的temp
    //r_pi = 4.0; //但无法修改（指向同一块内存）
    printf("%lf,%lf\n",*p_pi,r_pi); // 4 4

    double &&rr_pi = (double)pi; //(double)pi是右值。这里引用了pi的temp。可修改，即rr_pi指向临时的3.14
    //double &&t_pi = *p_pi;  //不能绑定左值到右值引用
    rr_pi = 5.0;
    printf("%lf,%lf\n",rr_pi,pi); // 5 3.14
    system("pause");
    return 0;
}
```

### 5. const修饰成员函数

- 常函数：
  - 成员函数后加const后我们称为这个函数为常函数
  - 常函数内不可以修改成员属性
  - 成员属性声明时加关键字mutable后，在常函数中依然可以修改
- 常对象：
  - 声明对象前加const称该对象为常对象
  - 常对象只能调用常函数

```cpp
class Person {
public:
    Person() {
        m_A = 0;
        m_B = 0;
    }
    //this指针的本质是一个指针常量，指针的指向不可修改
    //如果想让指针指向的值也不可以修改，需要声明常函数
    void ShowPerson() const {
        //const Type* const pointer;
        //this = NULL; //不能修改指针的指向 Person* const this;

        //this->m_A = 100; //常函数中this指向的值不可以修改的。read only
        this->m_B = 100; //mutable修饰的变量可以修改
        cout<<m_B<<endl;
    }
    void MyFunc() const {
        cout<<m_A<<endl;
    }
public:
    int m_A;
    mutable int m_B; // mutable 可修改
};

void test01() {
    const Person person; //在声明前加const修饰对象  常对象
    cout << person.m_A << endl;//常对象可以访问成员变量的值
    //person.mA = 100; //常对象不能修改成员变量的值
    person.m_B = 100; //但是常对象可以修改mutable修饰成员变量
    
    person.MyFunc(); //常对象只能调用const常函数
    person.ShowPerson();
}

int main() {
    test01();
    system("pause");
    return 0;
}
```