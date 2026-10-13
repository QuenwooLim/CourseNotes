## 多态

### 1. 基本概念

- 多态分为两类
  - 静态多态: 函数重载 和 运算符重载属于静态多态，复用函数名
  - 动态多态: 派生类和虚函数实现运行时多态
- 静态多态和动态多态区别：
  - 静态多态的函数地址早绑定 - 编译阶段确定函数地址
  - 动态多态的函数地址晚绑定 - 运行阶段确定函数地址
- 多态满足条件： 
  - 有继承关系
  - 子类重写父类中的虚函数(重写：函数返回值类型 函数名 参数列表 完全一致称为重写)
- 多态使用：
  - 父类指针或引用指向子类对象

```cpp
class Animal
{
public:
	//虚函数 那么编译器在编译的时候就不能确定函数调用了。
	virtual void speak()
	{
		cout << "动物在说话" << endl;
	}
};

class Cat :public Animal//继承关系
{
public:
	void speak()//子类重写父类的虚函数
	{
		cout << "小猫在说话" << endl;
	}
};

class Dog :public Animal
{
public:
	void speak()
	{
		cout << "小狗在说话" << endl;
	}
};

void DoSpeak(Animal & animal)//使用父类（Animal）对象指向子类（Cat Dog）对象
{
	animal.speak();
}

int main() {
	Cat cat;
	DoSpeak(cat);
	Dog dog;
	DoSpeak(dog);

	system("pause");
	return 0;
}
```

### 2. 多态样例：计算器类

- 抽象计算器类
- 多态优点：代码组织结构清晰，可读性强，利于前期和后期的扩展以及维护

```cpp
class AbstractCalculator
{
public :
	virtual int getResult()
	{
		return 0;
	}

	int m_Num1;
	int m_Num2;
};

//加法计算器
class AddCalculator :public AbstractCalculator
{
public:
	int getResult()
	{
		return m_Num1 + m_Num2;
	}
};

//减法计算器
class SubCalculator :public AbstractCalculator
{
public:
	int getResult()
	{
		return m_Num1 - m_Num2;
	}
};

//乘法计算器
class MulCalculator :public AbstractCalculator
{
public:
	int getResult()
	{
		return m_Num1 * m_Num2;
	}
};


void test02()
{
	//创建加法计算器
	AbstractCalculator *abc = new AddCalculator; // 父类指针指向子类对象
	abc->m_Num1 = 10;
	abc->m_Num2 = 10;
	cout << abc->m_Num1 << " + " << abc->m_Num2 << " = " << abc->getResult() << endl;
	delete abc;  //用完了记得销毁

	//创建减法计算器
	abc = new SubCalculator;
	abc->m_Num1 = 10;
	abc->m_Num2 = 10;
	cout << abc->m_Num1 << " - " << abc->m_Num2 << " = " << abc->getResult() << endl;
	delete abc;  

	//创建乘法计算器
	abc = new MulCalculator;
	abc->m_Num1 = 10;
	abc->m_Num2 = 10;
	cout << abc->m_Num1 << " * " << abc->m_Num2 << " = " << abc->getResult() << endl;
	delete abc;
}

int main() {
	test02();
	system("pause");
	return 0;
}
```


### 3. 纯虚函数和抽象类

- 纯虚函数：
  - 在多态中，通常父类中虚函数的实现是毫无意义的，主要都是调用子类重写的内容
  - 因此可以将虚函数改为纯虚函数
  - 纯虚函数语法：virtual 返回值类型 函数名 （参数列表）= 0 ;
- 抽象类
  - 当类中有了纯虚函数，这个类也称为==抽象类=
  - 无法实例化对象
  - 子类必须重写抽象类中的纯虚函数，否则也属于抽象类
```cpp
class Base
{
public:
	virtual void func() = 0;//纯虚函数
};

class Son :public Base
{
public:
	virtual void func() //子类必须重写抽象类中的纯虚函数
	{
		cout << "func调用" << endl;
	};
};

void test01()
{
	Base * base = NULL;
	//base = new Base; // 错误，抽象类无法实例化对象
	base = new Son;//父类对象接收子类对象
	base->func();
	delete base;//记得销毁
}

int main() {
	test01();
	system("pause");
	return 0;
}
```


### 4. 虚析构和纯虚析构

- 背景
  - 虚析构函数是用来解决通过父类指针释放子类对象
  - 基类和子类的虚构函数都会在结束的时候被调用
- 问题：多态使用时，如果子类中有属性开辟到堆区，那么父类指针在释放时无法调用到子类的析构代码
- 解决：将父类中的析构函数改为虚析构或者纯虚析构
- 虚析构和纯虚析构共性：
  - 可以解决父类指针释放子类对象
  - 都需要有具体的函数实现
- 虚析构和纯虚析构区别：
  - 如果是纯虚析构，该类属于抽象类，无法实例化对象
- 虚析构语法：virtual ~类名(){}
- 纯虚析构语法：virtual ~类名() = 0;
- 纯虚析构还可以进行实现
```cpp
class Animal {
public:

	Animal()
	{
		cout << "Animal 构造函数调用！" << endl;
	}
	virtual void Speak() = 0;
	virtual ~Animal() = 0;//纯虚析构
};

Animal::~Animal()//纯虚析构还可以进行实现
{
	cout << "Animal 纯虚析构函数调用！" << endl;
}

class Cat : public Animal {
public:
	Cat(string name)
	{
		cout << "Cat构造函数调用！" << endl;
		m_Name = new string(name);//子类中有属性开辟到堆区
	}
	virtual void Speak()
	{
		cout << *m_Name <<  "小猫在说话!" << endl;
	}
	~Cat()
	{
		cout << "Cat析构函数调用!" << endl;
		if (this->m_Name != NULL) {
			delete m_Name;
			m_Name = NULL;
		}
	}
public:
	string *m_Name;
};

void test01()
{
	Animal *animal = new Cat("Tom");
	animal->Speak();
	delete animal;
}

int main() {
	test01();
	system("pause");
	return 0;
}
```