## 类初始化和清理

### 1. 构造函数和析构函数基础

默认提供的构造函数和析构函数是空实现，需要自己实现构造函数和析构函数来完成类的初始化和清理工作。

#### 构造函数

- 构造函数语法：类名(){}
- 构造函数，没有返回值也不写void
- 函数名称与类名相同
- 构造函数可以有参数，因此可以发生重载
- 程序在调用对象时候会自动调用构造，无须手动调用,而且只会调用一次

#### 析构函数

- 析构函数语法： ~类名(){}
- 析构函数，没有返回值也不写void
- 函数名称与类名相同,在名称前加上符号 ~
- 析构函数不可以有参数，因此不可以发生重载
- 程序在对象销毁前会自动调用析构，无须手动调用,而且只会调用一次
- 不能直接调用！

```cpp
class Person
{
public:
	Person()//构造函数
	{
		cout << "Person的构造函数调用" << endl;
	}
	~Person()//析构函数
	{
		cout << "Person的析构函数调用" << endl;
	}
};

void test01()
{
	Person p;
}

int main() {
	
	test01();
	system("pause");
	return 0;
}
```



### 2. 构造函数

- 两种分类方式：
  - 按参数分为： 有参构造和无参构造
  - 按类型分为： 普通构造和拷贝构造
- 三种调用方式：
  - 括号法
  - 显示法
  - 隐式转换法

```cpp
class Person {
public:
    //无参（默认）构造函数
    Person() {
        cout << "无参构造函数!" << endl;
    }
    //有参构造函数
    /*     
    Person(int a) {
        age = a;
        cout << "有参构造函数!" << endl;
    } */
    //（默认）有参构造函数
    Person(int a=18){//设置默认参数
        age=a;
        cout<<"默认有参构造函数!"<<endl;
    }
    //拷贝构造函数 传入的参数是一个对象的引用
    Person(const Person& p) {
        age = p.age;
        cout << "拷贝构造函数!" << endl;
    }
    //析构函数
    ~Person() {
        cout << "析构函数!" << endl;
    }
public:
    int age;
};
void test01() {
    Person p; //调用无参构造函数
}
//调用有参的构造函数
void test02() {

    //2.1  括号法，常用
    Person p1(10);
    //Person p2();//调用无参构造函数不能加括号，如果加了编译器认为这是一个函数声明
    //2.2 显式法
    Person p2 = Person(10); 
    Person p3 = Person(p2);
    //Person(10)单独写就是匿名对象  当前行结束之后，马上析构

    //2.3 隐式转换法
    Person p4 = 10; // Person p4 = Person(10); 
    Person p5 = p4; // Person p5 = Person(p4); 
}
int main() {
    test01();
    test02();
    system("pause");
    return 0;
}
```


### 3. 拷贝构造函数调用

- a.使用一个已经创建完毕的对象来初始化一个新对象
- b.值传递的方式给函数参数传值
- c.以值方式返回局部对象

```cpp
class Person {
public:
    Person() {
        cout << "无参构造函数!" << endl;
        mAge = 0;
    }
    Person(int age) {
        cout << "有参构造函数!" << endl;
        mAge = age;
    }
    Person(const Person& p) {
        cout << "拷贝构造函数!" << endl;
        mAge = p.mAge;
    }
    //析构函数在释放内存之前调用
    ~Person() {
        cout << "析构函数!" << endl;
    }
public:
    int mAge;
};

void test01() {
    Person man(100); //p对象已经创建完毕
    Person newman(man); //1. 使用一个已经创建完毕的对象来初始化一个新对象
    Person newman2 = man; //等价
    //newman2 = man; //不是调用拷贝构造函数，赋值操作
}

//2. 值传递的方式给函数参数传值 等价Person p1 = p; 两个对象
void doWork(Person p1) {}
void test02() {
    Person p; //无参构造函数
    doWork(p);
}

Person doWork2()
{
    Person p1;
    cout << (int *)&p1 << endl;
    return p1;//3. 以值方式返回局部对象
}
void test03()
{
    Person p = doWork2();//p和p1是同一个对象(地址相同)
    cout << (int *)&p << endl;
}
int main() {
    //test01();
    //test02();
    test03();
    system("pause");
    return 0;
}
```

### 4. 构造函数调用规则

默认情况下，c++编译器至少给一个类添加3个函数
- a．默认构造函数(无参，函数体为空)
- b．默认析构函数(无参，函数体为空)
- c．默认拷贝构造函数，对属性进行值拷贝

构造函数调用规则如下：
- a.如果用户定义有参构造函数，c++不在提供默认无参构造，但是会提供默认拷贝构造
- b.如果用户定义拷贝构造函数，c++不会再提供其他构造函数
- c.按照无参、有参、拷贝的顺序，有其中一个前面的函数就不提供了

```cpp
class Person {
public:
    //无参（默认）构造函数
    Person() {
        cout << "无参构造函数!" << endl;
    }
    //有参构造函数
    Person(int a) {
        age = a;
        cout << "有参构造函数!" << endl;
    }
    //拷贝构造函数
    Person(const Person& p) {
        age = p.age;
        cout << "拷贝构造函数!" << endl;
    }
    //析构函数
    ~Person() {
        cout << "析构函数!" << endl;
    }
public:
    int age;
};

void test01()
{
    Person p1(18);
    //如果不写拷贝构造，编译器会自动添加拷贝构造，并且做浅拷贝操作
    Person p2(p1);

    cout << "p2的年龄为： " << p2.age << endl;
}

void test02()
{
    //如果用户提供有参构造，编译器不会提供默认构造，会提供拷贝构造
    Person p1; //此时如果用户自己没有提供默认构造，会出错
    Person p2(10); //用户提供的有参
    Person p3(p2); //此时如果用户没有提供拷贝构造，编译器会提供

    //如果用户提供拷贝构造，编译器不会提供其他构造函数
    Person p4; //此时如果用户自己没有提供默认构造，会出错
    Person p5(10); //此时如果用户自己没有提供有参，会出错
    Person p6(p5); //用户自己提供拷贝构造
}

int main() {
    test01();
    system("pause");
}
```


### 5. 步入深拷贝和浅拷贝：动态对象和指针

#### new与delete

- new的使用
  - 指针=new 类型名//创建对象
  - 指针=new 类型名（初始化参数）//创建对象并指定初始化参数
  - 指针=new 类型名[数组长度]//动态分配数组
  - 指针=new 类型名[数组长度]{a,b,c...}//动态分配数组并初始化参数
- 返回该类型的指针，不是void*（不同于C）
- 分配失败抛出异常，不是NULL
```cpp
int main()
{
    int* p;
    p=new int;
    cin>>*p;//*p解析为对象
    cout<<*p;
    delete p;//释放
    system("pause");
    return 0;
}
```

```cpp
int main()
{
    int* p=new int[10];
	for(int i=0;i<10;i++)
	{
		p[i]=i;
	}
	for(int i=0;i<10;i++)
		cout<<p[i]<<" ";
	cout<<endl;
	delete[] p;

	int* pp=new int[10]{1,2,3,4,5,6,7,8,9,10};
	for(int i=0;i<10;i++)
		cout<<pp[i]<<" ";
	cout<<endl;
	delete[] pp;
    system("pause");
    return 0;
}
```

- delete的使用
  - delete 变量名 //基本用法
  - delete[] 变量名 //释放数组
- delete释放空间后，指针仍指向原来的地址，但是指针已经无效
- 重复释放指针和指针使用都是非法的
- delete类的对象时，会同时调用析构函数

```cpp
class Text {
public:	
    Text(const char* s);
    ~Text();
    void Print();
private:
    string s;	
};

Text::Text(const char* s){
    this->s = string(s);
    cout <<"Creating " << this->s << endl; 
}

Text::~Text() {
    cout <<"Releasing " << this->s << endl; 
}

void Text::Print(){
    cout <<"print " << this->s << endl; 
}

int main() {
    Text* str_ptr = new Text[3]{"on heap 1","on heap 2","on heap 3"};
    
    Text* p = str_ptr;
    p++->Print(); // 打印 "on heap 1" 后指针向下一个对象
    p->Print(); // 打印 "on heap 2"
    
    delete []str_ptr; //数组对象析构 同时自动调用对象的析构函数
    //delete str_ptr; 错误  
}
```

```cpp
int main() {
    int* p1=new int(10);     
    int* p2=p1;

    cout << *p1 << endl;//10
    cout << *p2 << endl;//10

    delete p1;      //将使p1r和p2同时失效

    cout << *p1 << endl;    //野指针
    cout << *p2 << endl;    //野指针

    delete p2;       //错误，不能再次释放       
	system("pause");
	return 0;             
}
```


#### default与delete

- default 显示定义默认函数
  - 声明有参构造函数时，编译器不会自动创建默认构造函数
  - =default 在函数后声明 让编译创建该构造函数
  - 仅能用于编译器能对那个生成的特殊函数：默认构造、析构、复制构造、赋值运算等

```cpp
class A
{
public:
    A(int x) {
        cout << "This is a parameterized constructor";
    }
    A() = default;//没有该函数的话下面A a;语句会报错
};

int main(){
    A a;          
    A x(1);       
    cout<<endl;
	system("pause");
    return 0;
}
```

- delete 弃置函数
  - 必须用在函数的首次声明的位置

```cpp
class A { 
public: 
    A(int x): m(x) { } 
    A(const A&) = delete;      
    A& operator=(const A&) = delete;  
    int m; 
}; 
  
int main() { 
    A a1(1), a2(2), a3(3); 
    //a1 = a2;   //error 函数已经被delete
    //a3 = A(a2);  //error 函数已经被delete
    return 0; 
}
```


- 禁止复制构造函数和赋值函数的情况：
  - 1.Stack内部使用链表指针ListNode*（动态开辟）
  - 2.如果允许默认的复制构造和赋值操作，编译器会自动生成浅拷贝
  - 3.delete可以防止使用

```cpp
class Stack {    
    struct ListNode {
        int val;
        struct ListNode *next;
    }; 
    
    public:
    Stack() = default;
    //必须禁止复制构造和赋值 
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack& other) = delete;
    ~Stack() { clear(); }
    
    void push(int val) {
        ListNode *node = new ListNode;
        node->val = val;
        node->next = head; 
        head = node;
    }
    void pop() {
        if (!isEmpty()) {
            ListNode *node = head;
            head = head->next;
            delete node;
        }
    }
    int top() {
        if (!isEmpty()) return head->val;
        throw runtime_error("Accessing empty stack.");
    }
    bool isEmpty() {
        return !head;
    }; 
    void clear() {
        while (!isEmpty()) pop();
    }  
    
    void print(){
        cout << "(";
        for (ListNode *p=head; p!=nullptr; p=p->next) {
            cout<<p->val<<",";
        }
        cout << ")"<<endl;
    } 
    
  private:
    ListNode *head=nullptr;
};
```

### 6. 浅拷贝与深拷贝

- a.浅拷贝：简单的赋值拷贝操作
  - 如果成员对象不含指针，使用默认的拷贝构造函数即可（系统提供），两个对象指向同一块内存
- b.深拷贝：在堆区重新申请空间，进行拷贝操作
  - 在堆区开辟的，一定要自己提供拷贝构造函数，防止浅拷贝带来的问题（重复释放指针）
  - 含指针成员的类一般要重写以下内容：构造函数（及拷贝构造函数）中深拷贝、=操作重写完成深复制策略、析构函数中释放内容

- 隐式调用拷贝构造函数情况：
  - 1.对象作为函数值参数传递
  - 2.对象作为函数返回值

```cpp
class Person {
public:
    //无参（默认）构造函数
    Person() {
        cout << "无参构造函数!" << endl;
    }
    //有参构造函数
    Person(int age ,int height) {
        cout << "有参构造函数!" << endl;
        m_age = age;//非动态分配 在栈上
        m_height = new int(height);//动态分配 在堆区开辟
    }
    //拷贝构造函数  
    Person(const Person& p) {//使用const类型的引用传参
        cout << "拷贝构造函数!" << endl;
        //如果不利用深拷贝在堆区创建新内存，会导致浅拷贝带来的重复释放堆区问题
        m_age = p.m_age;//浅拷贝
        m_height = new int(*p.m_height);//深拷贝
    }

    //析构函数
    ~Person() {
        cout << "析构函数!" << endl;
        if (m_height != NULL)
        {
            delete m_height;//浅拷贝会二次释放内存
        }
    }
public:
    int m_age;
    int* m_height;
};

int main() {
    Person p1(18, 180);
    Person p2(p1);
    //p2=p1;//还是浅拷贝问题 看下文
    cout << "p1的年龄： " << p1.m_age << " 身高： " << *p1.m_height << endl;
    cout << "p2的年龄： " << p2.m_age << " 身高： " << *p2.m_height << endl;
    system("pause");
    return 0;
}
```

- 已经实现了拷贝构造函数的浅拷贝问题
- 但是使用类的默认赋值操作仍然存在浅拷贝问题

```cpp
class Person {
public:
    //无参（默认）构造函数
    Person() {
        cout << "无参构造函数!" << endl;
    }

    //有参构造函数
    Person(int age ,int height) {
        cout << "有参构造函数!" << endl;
        m_age = age;//非动态分配 在栈上
        m_height = new int(height);//动态分配 在堆区开辟
    }

    //拷贝构造函数  
    Person(const Person& p) {//使用const类型的引用传参
        cout << "拷贝构造函数!" << endl;
        //如果不利用深拷贝在堆区创建新内存，会导致浅拷贝带来的重复释放堆区问题
        m_age = p.m_age;//浅拷贝
        m_height = new int(*p.m_height);//深拷贝
    }

    // 重写赋值操作符
    Person& operator=(const Person& other)
    {
        cout<<"= operator"<<endl;
        if(this==&other)//a.不能自身赋值 直接返回 
            return *this;
        this->m_age=other.m_age;
        if(m_height!=nullptr)//b.先释放掉
            delete m_height;
        if(other.m_height==nullptr)//c.不需要深拷贝
        {
            m_height=nullptr;
            return *this;
        }
        m_height=new int(*(other.m_height));
        return *this;
    }

    //析构函数
    ~Person() {
        cout << "析构函数!" << endl;
        if (m_height != NULL)
        {
            delete m_height;//浅拷贝会二次释放内存
        }
    }
public:
    int m_age;
    int* m_height;
};

int main() {
    Person p1(18, 180);
    Person p2(p1);//拷贝构造函数
    Person p3=p1;// = 但也是拷贝构造函数
    p3=p2;//赋值操作 如果没有重写赋值构造函数的话会报错
    cout << "p1的年龄： " << p1.m_age << " 身高： " << *p1.m_height << endl;
    cout << "p2的年龄： " << p2.m_age << " 身高： " << *p2.m_height << endl;
    system("pause");
    return 0;
}
```

案例：对于动态开辟的字符串

```cpp
class NAME {
	public:	
		NAME();
		NAME(const NAME& other);
		NAME& operator=(const NAME& other);
		~NAME();
		void show();
		void set(const char* s);
	private:  
	    char* str;
};

NAME::NAME():str(nullptr){
	cout << "Constructing.\n";
}

NAME::NAME(const NAME& other){
    cout << "Copy Constructing.\n";
    if (other.str == nullptr) {
        str = nullptr;
        return;
    }
    str = new char[strlen(other.str) + 1]; // 开辟空间
    strcpy(str, other.str); // 赋值
} 

NAME:: ~NAME() {
	cout << "Destructing.\n";
	if (str != nullptr)  
		delete []str;
}
     
void NAME::show() {	
    cout << str << "\n";	    
}

void NAME::set(const char* s) {	
	if (str!=NULL)	
	    delete []str;
	
	str = new char[strlen(s) + 1]; // 开辟空间
	
	if (str!=NULL)	
	    strcpy(str, s);
}

NAME& NAME::operator=(const NAME& other) {
	cout << "= operator.\n";
	if (this==&other) return *this; // 不能自身赋值
    if (str != NULL) delete[] str; // 释放掉原来的内存
        
    if (other.str == NULL) { // 如果other.str为空，则将str置为NULL
        str = NULL;
        return *this;
    }

    str = new char[strlen(other.str) + 1]; // 重点！！！  先开辟空间
    if (str != NULL)
        strcpy(str, other.str); // 重点！！！  再字符串赋值
    return *this;
}

void Print(NAME name) {
    name.show();
} 
```

### 7. 初始化列表

- 语法：构造函数()：属性1(值1),属性2（值2）... {}
- 规则：按照成员属性在类中的顺序，而不是在初始化列表的顺序。因此要最好按照成员声明的顺序在初始化列表中

- 必须使用初始化列表：
  - 1.常量成员 只能初始化不能赋值
  - 2.引用成员 必须在定义的时候初始化 不能赋值 
  - 3.没有无参构造函数的类成员 

- 非静态成员初始化的方式：
  - 1.初始化列表
  - 2.默认成员初始化器，它是成员声明中包含的花括号或等号初始化器 eg:string s{'H','C'};   //带默认成员初始化器
  - 3.构造函数体内

- 为什么有初始化器列表和默认成员初始化器：
  - 对象初始化，先初始化成员，然后执行构造函数函数体
  - 对于复杂对象成员，必须先构造才能在构造函数中赋值
  - 对于引用类型成员，const成员需要预初始化
  - 对象成员初始化需要参数

```cpp
class Person {
public:

    ////传统方式初始化
    //Person(int a, int b, int c) {
    //	m_A = a;
    //	m_B = b;
    //	m_C = c;
    //}

    //初始化列表方式初始化
    Person(int a, int b, int c) :m_A(a), m_B(b), m_C(c) {}
    void PrintPerson() {
        cout << "mA:" << m_A << endl;
        cout << "mB:" << m_B << endl;
        cout << "mC:" << m_C << endl;
    }
private:
    int m_A;
    int m_B;
    int m_C;
};
```

```cpp
struct Vector3 {
    Vector3(double x,double y,double z=0):x(x),y(y),z(z){}
    double x,y,z; 
};

struct Ray {
    Ray (Vector3 origin, Vector3 direction):origin(origin),direction(direction) {};
    /*     
    Ray (Vector3 origin, Vector3 direction)//error 因为Vector3不存在默认构造函数 只能在初始化列表中进行初始化
	{
		this->origin=origin;
		this->direction=direction;
	} */
    Vector3 origin;     //射线的原点。
    Vector3 direction;	//射线的方向。 
};

int main() {
    Vector3 vo(0,0,0), v(1,1,1);
    Ray ray(vo,v);
    system("pause");
    return 0;
} 
```

### 8. 类对象作为类成员

- 当类中成员是其他类对象时，我们称该成员为 对象成员
- 构造顺序：先调用对象成员构造，再调用本类构造
- 析构顺序：先调用本类析构，再调用对象成员析构

```cpp
class Phone
{
public:
	Phone(string name)
	{
		m_PhoneName = name;
		cout << "Phone构造" << endl;
	}

	~Phone()
	{
		cout << "Phone析构" << endl;
	}

	string m_PhoneName;
};

class Person
{
public:
	Person(string name, string pName) :m_Name(name), m_Phone(pName)
	{
		cout << "Person构造" << endl;
	}
	~Person()
	{
		cout << "Person析构" << endl;
	}
	void playGame()
	{
		cout << m_Name << " 使用" << m_Phone.m_PhoneName << " 牌手机! " << endl;
	}

	string m_Name;
	Phone m_Phone;
};
void test01()
{
	Person p("张三" , "苹果X");
	p.playGame();
}

int main() {
	test01();
	system("pause");
	return 0;
}
```

### 9. 静态成员

静态成员
- 在成员变量和成员函数前加上关键字static，称为静态成员
- 类的组成部分 但不是任何对象的组成部分
- 遵循正常的共有/私有访问规则，在类作用域外通过类直接（不通过对象）访问静态成员（需要加上类名::）
- static需要在类外初始化有且一次（初始化不带static），不通过构造函数初始化

静态成员分为：
- a.静态成员变量
  - 所有对象共享同一份数据
  - 在编译阶段分配内存
  - 类内声明，类外初始化
- b.静态成员函数
  - 所有对象共享同一个函数
  - 静态成员函数只能访问静态成员变量

```cpp
class Person
{
public:
	static int m_A; //静态成员变量 类内声明
    static void func()
	{
		cout << "func调用" << endl;
		m_A = 100;
		//m_C = 100; //只能访问静态成员变量 不可以访问非静态成员变量
	}
private:
	static int m_B; //静态成员变量也是有访问权限的
    int m_C; //非静态成员变量
};
int Person::m_A = 10;//静态成员变量 类外初始化
int Person::m_B = 10;

void test01()
{
	//静态成员变量两种访问方式
	//1、通过对象
	Person p1;
	p1.m_A = 100;
	cout << "p1.m_A = " << p1.m_A << endl;

	Person p2;
	p2.m_A = 200;
	cout << "p1.m_A = " << p1.m_A << endl; //共享同一份数据
	cout << "p2.m_A = " << p2.m_A << endl;

	//2、通过类名
	cout << "m_A = " << Person::m_A << endl;

    //静态成员变量两种访问方式
	//1、通过对象
	Person p1;
	p1.func();

	//2、通过类名
	Person::func();
}

int main() {
	test01();
	system("pause");
	return 0;
}
```