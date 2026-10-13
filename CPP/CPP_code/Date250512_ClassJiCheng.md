## 继承

- 对象之间的关系：友元关系、组合关系、继承关系
- 继承与派生是面向对象的基础，通过封装分离了开发者和使用者，通过继承和派生提供了构造事物的方法，多态，提高编程的可重用性


### 1. 继承的语法

- class A : public B;
  - A 类称为子类 或 派生类
  - B 类称为父类 或 基类
- 派生类中的成员，包含两大部分：
  - 一类是从基类继承过来的，一类是自己增加的成员。
  - 从基类继承过过来的表现其共性，而新增的成员体现了其个性。


### 2 访问控制与继承

- 基类的成员访问控制与继承：
  - public；可以被继承和访问
  - protected：类的外部不可以访问，可以被继承，派生类可以访问
  - private：不可被继承，子类不能访问父类的私有private成员，只能通过父类的函数访问
- 继承方式：基类的private无法被继承，考虑public和protected的继承结果
  - public 继承：基类的public和protected成员在派生类中仍然是public和protected
  - protected 继承：基类的public和protected成员在派生类中变为protected
  - private 继承：基类的public和protected成员在派生类中变为private

```cpp
class Base1
{
public: 
	int m_A;
protected:
	int m_B;
private:
	int m_C;
};

//公共继承
class Son1 :public Base1//语法
{
public:
	void func()
	{
		m_A; //可访问 public权限
		m_B; //可访问 protected权限
		//m_C; //不可访问
	}
};

void myClass()
{
	Son1 s1;
	s1.m_A; //其他类只能访问到公共权限
}

//保护继承
class Base2
{
public:
	int m_A;
protected:
	int m_B;
private:
	int m_C;
};
class Son2:protected Base2
{
public:
	void func()
	{
		m_A; //可访问 protected权限
		m_B; //可访问 protected权限
		//m_C; //不可访问
	}
};
void myClass2()
{
	Son2 s;
	//s.m_A; //不可访问
}

//私有继承
class Base3
{
public:
	int m_A;
protected:
	int m_B;
private:
	int m_C;
};
class Son3:private Base3
{
public:
	void func()
	{
		m_A; //可访问 private权限
		m_B; //可访问 private权限
		//m_C; //不可访问
	}
};
class GrandSon3 :public Son3
{
public:
	void func()
	{
		//Son3是私有继承，所以继承Son3的属性在GrandSon3中都无法访问到
		//m_A;
		//m_B;
		//m_C;
	}
};
```

### 3. 访问控制权限的修改

- 可以使用“访问声明”恢复原来的访问控制方式
- 语法：using 基类名::成员名; //代码放在成员访问控制后
- 还可通过这种使派生类对象直接使用基类的构造函数
```cpp
class BASE {
public:
    void set_i(int x)
    {
        i = x;
    }
    int get_i()
    {
        return i;
    }
protected:
    int i;
};
class DERIVED : private BASE {//private继承：public->private，protected->private
public:
    using BASE::set_i;   // 访问声明 public->private->(恢复)public
    using BASE::i;  // 访问声明 protected->private->(恢复)public
    void set_j(int x)
    {
        j = x;
    }
    int get_ij()
    {
        return i + j;
    }
protected:
    int j;
};
```

- 派生类对象的初始化与构造
  - 基类的构造函数不被继承，派生类需要声明自己的构造函数
  - 在派生类的构造函数中：先进行基类部分的初始化，再新增自己成员的初始化
	- 基类部分的初始化：
      - 默认使用无参构造或拷贝构造，如果基类没有默认构造则报错
      - 使用有参构造函数时(即基类的构造函数带参数)，必须显示在初始化器列表中使用，不能在构造函数体内实现

- 派生与成员函数
  - 1.重载：
	- 具有相同的作用域
	- 函数名字相同
	- 参数类型，顺序，数目不同
  - 2.覆盖（修改基类函数定义）
  - 3.隐藏（屏蔽基类的函数定义）：
	- 派生类的函数与基类的函数同名，但参数列表有差异
	- 派生类的函数与基类的函数同名，参数列表也相同，但是基类函数没有virtual关键字
	- 应用：
      - 修改派生类成员函数的旧功能
      - 增加派生类成员函数的新功能
  - 4.继承：
	- 没有被覆盖或屏蔽的基类函数，包括基类中重载的函数

```cpp
class Time
{
public:
    void Set(int hours, int minutes, int seconds);//将被修改
    void Increment();//将被继承
    void Write() const;
    Time(int initHrs, int initMins, int initSecs);  // constructor 有参构造！不被继承
    Time(); 	   //  default constructor 不被继承

private:
    int hrs;
    int mins;
    int secs;
};

enum  ZoneType { EST, CST, MST, PST, EDT, CDT, MDT, PDT };//枚举类型

class ExtTime: public Time	// Time is the base class
{
public:
    ExtTime(int initHrs, int initMins, int initSecs, ZoneType initZone);      // constructor 派生类构造函数
    ExtTime(); 			             // default constructor 派生类析构函数
    void Set(int hours, int minutes, int seconds, ZoneType timeZone);//改变参数列表和函数体; 隐藏了基类的Set
    void Write() const;//定义没变 函数体重写; 基类没有virtual关键字 隐藏了基类的Write

private:
    ZoneType  zone; 	//  added data member
};

Time::Time(int initHrs, int initMins, int initSecs)
{
    hrs = initHrs;
    mins = initMins;
    secs = initSecs;
}

Time::Time()
{
    hrs = 0;
    mins = 0;
    secs = 0;
}

void Time::Set(int hours, int minutes, int seconds)
{
    hrs = hours;
    mins = minutes;
    secs = seconds;
}

void Time::Increment()    //  IMPLEMENTATION FILE ( time.cpp )
{
    secs++;
    if (secs > 59)
    {
        secs = 0;
        mins++;
        if (mins > 59)
        {
            mins = 0;
            hrs++;
            if (hrs > 23)
                hrs = 0;
        }
    }
}

void Time::Write() const
{
    if (hrs < 10)
        cout << '0';
    cout << hrs << ':';
    if (mins < 10)
        cout << '0';
    cout << mins << ':';
    if (secs < 10)
        cout << '0';
    cout << secs;
}

//子类的有参构造
ExtTime::ExtTime(int initHrs, int initMins, int initSecs, ZoneType initZone) : Time(initHrs, initMins, initSecs)//在初始化器列表中完成基类构造
{
    zone = initZone;
}

//子类的无参构造
ExtTime::ExtTime()
{
    zone = EST;
}

void ExtTime::Set(int hours, int minutes, int seconds, ZoneType timeZone)
{
    Time::Set(hours, minutes, seconds);   //Set重写了 ，但还是调用基类的Set函数（被隐藏了，需要用作用域Time::访问）
    zone = timeZone;
}

void ExtTime::Write() const
{
    static string zoneString[8] = {"EST", "CST", "MST", "PST", "EDT", "CDT", "MDT", "PDT"};

    Time::Write();
    cout << ' ' << zoneString[zone];
}

int main()
{
    ExtTime time1(5, 30, 0, CDT);
    ExtTime time2;
    int     count;
//time1.Set(1,1,1);
    cout << "time1: ";
    time1.Write();
    cout << endl;
    
    cout << "Incrementing time2:" << endl;
    for (count = 1; count <= 10; count++)
    {
        time2.Write();
        cout << endl;
        time2.Increment();
    }

    Time time3(1, 2, 3);
    cout << "time3: ";
    time3.Write();
    cout << endl << endl;

    //客户代码直接访问派生类继承的基类的public 成员

    time1.Time::Set(3, 4, 5);
    time1.Time::Write();

    cout << endl;
    system("pause");
    return 0;
}
```

### 4. 继承中的对象模型

- 从父类继承过来的成员，私有成员会被隐藏无法访问（在内存中还是继承下去了）

### 5. 继承中的构造和析构顺序

- 构造顺序：父类，再子类
- 析构顺序：子类，再父类

### 6. 继承同名成员处理方式

- 直接访问：会调用到子类的成员
- 添加作用域，才可以访问到父类的成员（即子类隐藏了父类的成员函数）
- 子类中访问父类的同名成员同上，需要添加作用域

```cpp
class Base {
public:
	Base()
	{
		m_A = 100;
	}
	void func()
	{
		cout << "Base - func()调用" << endl;
	}
	void func(int a)
	{
		cout << "Base - func(int a)调用" << endl;
	}
public:
	int m_A;
};

class Son : public Base {
public:
	Son()
	{
		m_A = 200;
	}
	//当子类与父类拥有同名的成员函数，子类会隐藏父类中所有版本的同名成员函数
	//如果想访问父类中被隐藏的同名成员函数，需要加父类的作用域
	void func()
	{
		cout << "Son - func()调用" << endl;
	}
public:
	int m_A;
};

int main() {
	Son s;
	cout << "Son下的m_A = " << s.m_A << endl;
	cout << "Base下的m_A = " << s.Base::m_A << endl;
	s.func();
	s.Base::func();
	s.Base::func(10);

	system("pause");
	return EXIT_SUCCESS;
}
```

- 类型兼容性（向上兼容）：可以将子类的对象赋值给父类的对象，反之不可。（因为每个派生类对象包含了基类的一部分）
- 该规则只适用于公有派生
- 类型的转换
  - upcasting向上/父类型转换，直接使用上面的类型兼容性规则，语法：基类对象=派生类对象
  - downcasting向下/子类型转换，使用dynamic_cast<T*>，语法：基类对象=dynamic_cast<基类*>子类对象;
```cpp
class BASE {
protected:
	int x;
public:
	BASE(int x=1):x(x) {};
	void print() {
		cout << x << endl;
	};
};

class DERIVED:public BASE {
	int y;
public:
	DERIVED(int y=2,int x=0):BASE(x),y(y) {}; 
	void print() {
		cout << x << "," << y << endl;
	};	
};

class IndirectDerived:public DERIVED {
	int z = 10;
public:
	void print() {
		cout << x << "," << z << endl;
	};
};

void func(BASE &r) {
	r.print();
};

void func(const int &p) {
	int &q = (int &)p;
	q = 21;
}

void func(int *p) {
	*p = 31;
	cout << *p << endl;
}
int main() {
	BASE obj1;
	obj1.print(); // 1
	DERIVED obj2;
	obj2.print();// 0 2

    //类型兼容
	obj1 = obj2; //基类对象=派生类对象 obj1获得了obj2的基类部分的数据
    obj1.print(); //0
    //obj2=obj1; //error
    DERIVED* pd = static_cast<DERIVED*>(&obj1);//可以转换 但不安全
    pd->print();//0 39199984(非法数据)

    BASE obj0;
    obj0.print();//1
	*(BASE*)(&obj2) = obj0;// 只对 obj2 的父类数据部分操作，令其复制obj1（父类）的值
	obj2.print();//1 2
	
	//兼容兼容 指针形式也可以 
	BASE obj3(5);
	BASE *pB = &obj3;//父类=父类
	pB = &obj2;//父类=子类
	pB->print();
	
	// 对象作为参数传递，向上类型转换或赋值 
	IndirectDerived obj4;
	func(obj4);//0 ;接收类型是基类（从子类->基类）
	
    system("pause");
    return 0;
}
```

- 四种常用类型转换关键字：
  - static_cast：
    - 用于基本类型之间的转换、类层次结构中向上/向下转换（安全前提下），
    - 编译期检查，常用于数值类型转换和父子类指针/引用转换。
  - const_cast：
    - 去除或添加 const/volatile 属性，常用于需要修改本应只读的数据（不推荐，危险）。
  - reinterpret_cast：
    - 用于强制转换不同类型的指针或引用（极其危险）
    - 比如把 int* 转为 char*，或把对象指针转为其他类型指针。
  - dynamic_cast：
    - 用于多态类型的安全向下转换（基类指针/引用转派生类指针/引用），
    - 运行时检查类型安全，失败时返回 nullptr 或抛异常。
```cpp
void func(int* j)
{
    *j = 100; // 修改指针指向的值
    cout<<*j<<endl;
}
int main()
{
    double d = 3.14;
    int i = static_cast<int>(d); // double 转 int
    cout<<i<<endl;// 3

    int k = 20;
    func(&k); //100 传递普通变量指针，可以修改

    const int j = 30;
    func(const_cast<int*>(&j)); //100 去掉const属性，试图修改j（未定义行为）
    cout << j << endl; //30; j 依然不会被修改（因为j是常量，修改属于未定义行为）

    int a = 10;
    char* p = reinterpret_cast<char*>(&a); // int* 转 char*
    cout<<*p<<endl;

    system("pause");
    return 0;
}
```

### 7. 多重继承

- 继承关系：单继承、多重继承、菱形继承
- 继承多个父类
  - 语法： class 子类 ：继承方式 父类1 ， 继承方式 父类2...
```cpp
class Device1 {
public:
    Device1()
    {
        volume = 5;
        powerOn = false;
    }
    Device1(int vol, bool onOrOff)
    {
        volume = vol;
        powerOn = onOrOff;
    }
    void showPower()
    {
        cout << "The status of the power is :";
        switch (powerOn) {
        case true:
            cout << "Power on. \n";
            break;
        case false:
            cout << "power off. \n";
            break;
        }
    }
    void showVol()
    {
        cout << "Volume is " << volume << endl;
    }
protected:
    int volume;                
    bool powerOn;     
};

class Device2 {
public:
    Device2()
    {
        talkTime = 10;
        standbyTime = 300;
        power = 100;
    }
    Device2(int newTalkTime, int newStandbyTime, float powerCent)
    {
        talkTime = newTalkTime;
        standbyTime = newStandbyTime;
        power = powerCent;
    }
    void showProperty()
    {
        cout << "The property of the device : " << endl;
        cout << "talk time: " << talkTime << " hours" << endl;
        cout << "standbyTime: " << standbyTime << " hours" << endl;
    }
    void showPower()
    {
        cout << " Power: " << power << endl;
    }
protected:
    int  talkTime;                  
    int  standbyTime;      
    float power;               
};

class DeviceNew : public Device1, public Device2 {//多重继承
public:
    DeviceNew()
    {
        weight = 0.56;
    }
    DeviceNew(float newWeight, int vol, bool onOrOff, int newTalkTime,
        int newStandbyTime, float powerCent) :
        Device2(newTalkTime, newStandbyTime, powerCent),
        Device1(vol, onOrOff)
    {
        weight = newWeight;
    }
    float getWeight()
    {
        return weight;
    }
private:
    float weight;
};

int main()
{
    DeviceNew  device(0.7, 3, false, 10, 250, 80);         //声明派生类对象

    // getWeight()函数是DEVICE_NEW类自身定义的
    cout << "The weight of the device : " << device.getWeight() << endl;

    // showVol()函数是从DEVICE1类继承来的
    device.showVol();

    // showProperty()函数是从DEVICE2类继承来的
    device.showProperty();
    return 0;
}
```

### 8. 菱形继承

- 两个派生类（羊、驼）继承同一个父类（动物）
- 另一个类（羊驼）同时继承这两个派生类

- 问题：羊驼使用数据时会产生二义性，比如：通过羊还是驼使用父类动物的数据呢？
- 解决：虚继承
  -  在父类前加上关键字 virtual 那么基类就是一个虚拟基类
  -  在子类中使用父类的成员时，添加作用域
  -  只会在内存中存在一份父类的成员，即virtual关键字保证了虚拟类的唯一副本只初始化一次
- 普通基类vs虚拟基类：在派生类重复继承了某一基类时表现出区别
- 菱形继承中的构造函数和析构函数：
  - 动物类的虚构函数需要使用virtual
  - 羊驼类的构造函数需要先调用父类（动物）的构造函数，再调用子类（羊、驼）的构造函数
- 创建派生类对象时构造函数的调用次序：
  - 最先调用虚基类的构造函数
  - 调用普通基类的构造函数（多继承按照声明次序）
  - 调用对象成员的构造函数
  - 执行派生类的构造函数体
```cpp
class Animal
{
public:
	int m_Age;
    virtual ~Animal() {} //虚析构函数
};

//继承前加virtual关键字后，变为虚继承
//此时公共的父类Animal称为虚基类
class Sheep : virtual public Animal {};//virtual
class Tuo   : virtual public Animal {};//virtual
class SheepTuo : public Sheep, public Tuo {};

int main() {

	SheepTuo st;
	st.Sheep::m_Age = 100;
	st.Tuo::m_Age = 200;//都是对同一个数据操作
	cout << "st.Sheep::m_Age = " << st.Sheep::m_Age << endl;
	cout << "st.Tuo::m_Age = " <<  st.Tuo::m_Age << endl;
	cout << "st.m_Age = " << st.m_Age << endl;
	system("pause");
	return 0;
}
```