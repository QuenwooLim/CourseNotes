## 友元

- 友元的目的就是让一个函数或者类 访问另一个类中私有成员
- 只有类内和友元函数可以访问私有成员
- 关键字friend
- 友元的三种实现
  - a.全局函数做友元
  - b.类做友元
  - c.成员函数做友元
- 友元函数没有this指针 因为友元不是类的成员

### 1. 全局函数做友元

```cpp
class Building
{
	//告诉编译器 goodGay全局函数 是 Building类的好朋友，可以访问类中的私有内容
	friend void goodGay(Building * building);
public:
	Building()
	{
		this->m_SittingRoom = "客厅";
		this->m_BedRoom = "卧室";
	}
public://公共成员
	string m_SittingRoom; 
private://私有成员 友元可以访问
	string m_BedRoom; 
};
void goodGay(Building * building)//全局函数
{
	cout << "好基友正在访问： " << building->m_SittingRoom << endl;
	cout << "好基友正在访问： " << building->m_BedRoom << endl;
}
void test01()
{
	Building b;
	goodGay(&b);
}

int main(){
	test01();
	system("pause");
	return 0;
}
```

### 2. 类做友元

```cpp
class Building;//先声明
class goodGay//类
{
public:
	goodGay();
	void visit();
private:
	Building *building; //前面先声明这里才能用
};

class Building
{
	friend class goodGay;//goodGay可以访问到Building类中私有内容
public:
	Building();
public:
	string m_SittingRoom; 
private: // private 成员只允许 类自身的成员函数 和 被显式声明的友元 访问
	string m_BedRoom;
};

Building::Building()
{
	this->m_SittingRoom = "客厅";
	this->m_BedRoom = "卧室";
}

goodGay::goodGay()
{
	building = new Building;
}

void goodGay::visit()
{
	cout << "好基友正在访问" << building->m_SittingRoom << endl;
	cout << "好基友正在访问" << building->m_BedRoom << endl;
}

void test01()
{
	goodGay gg;
	gg.visit();
}

int main(){
	test01();
	system("pause");
	return 0;
}
```

### 3. 成员函数做友元

```cpp
class Building;
class goodGay
{
public:
	goodGay();
	void visit(); //只让visit函数作为Building的好朋友，可以访问Building中私有内容
	void visit2(); //不是友元函数 无法访问Building中的私有成员
private:
	Building *building;
};

class Building
{
	friend void goodGay::visit();//goodGay可以访问Building私有内容
public:
	Building();
public:
	string m_SittingRoom;
private:
	string m_BedRoom;
};

Building::Building()
{
	this->m_SittingRoom = "客厅";
	this->m_BedRoom = "卧室";
}

goodGay::goodGay()
{
	building = new Building;
}

void goodGay::visit()
{
	cout << "好基友正在访问" << building->m_SittingRoom << endl;
	cout << "好基友正在访问" << building->m_BedRoom << endl;
}

void goodGay::visit2()
{
	cout << "好基友正在访问" << building->m_SittingRoom << endl;
	//cout << "好基友正在访问" << building->m_BedRoom << endl;
}

void test01()
{
	goodGay  gg;
	gg.visit();
}

int main(){
	test01();
	system("pause");
	return 0;
}
```