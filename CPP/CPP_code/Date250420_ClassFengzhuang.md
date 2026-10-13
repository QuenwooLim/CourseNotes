## 类和对象


- 面向对象
  - C++面向对象的三大特性为：封装、继承、多态
  - C++万物皆为对象，对象上有其属性和行为
- 类与对象
  - 类是用户定义的数据类型，表示一个抽象数据类型
  - 对象是类定义的变量或实例

### 1. 封装意义

- 将属性和行为作为一个整体，表现生活中的事物
- 将属性和行为加以权限控制

```cpp
#define PI 3.14
class Circle//类 类名
{
    public://访问权限
    int r;//属性
    int S;
    double CalArea()//行为
    {
        S=PI*r*r;
        return S;
    }
    double CalLength();
};//注意要加分号
double Circle::CalLength()//类外实现 不在类的作用域内 需要::
{
    return 2*PI*r;
}
int main()
{
    Circle c1;//通过Circle类创建对象c1
    c1.r=3;//给属性赋值
    cout<<c1.CalArea()<<endl;//调用行为
    cout<<c1.CalLength()<<endl;
    system("pause");
    return 0;
}
```


### 2. 类的属性和行为的访问权限

#### 访问权限有三种

- a.公共权限  public     类内可以访问  类外可以访问
- b.保护权限  protected  类内可以访问  类外不可以访问
- c.私有权限  private    类内可以访问  类外不可以访问

访问权限的使用
- 将所有成员属性设置为私有，可以自己控制读写权限
- 对于写权限，我们可以检测数据的有效性
- 公有成员可以在客户端随意访问（无需通过成员函数），可能破坏封装的逻辑一致性（如修改长度为负数等）

### 3. struct和class的唯一区别在于默认的访问权限不同

- struct 默认公共
- class  默认私有
- 除此之外 class和struct的语法和语义没有区别

