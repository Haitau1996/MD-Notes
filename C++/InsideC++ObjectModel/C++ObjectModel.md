# 深度探索 C++ 对象模型

## Chap 1: 关于对象
C 语言中, 数据和处理数据的操作(函数)是分开声明的, 语言没有支持数据和函数之间的关联性. C++ 加上封装之后, 成本增加了多少? <br>
实际上, C++ 在布局以及存取之间的额外负担主要是由 _virtual_ 引起的:
* 虚函数机制: 用以支持一个有效率的执行期绑定
* 虚基类: 用以实现多次出现在继承体系中的base class, 有一个单一而被共享的实例.

### C++ 对象模式
C++ 中有两种class data members: _static_ 和 _nonstatic_ . 同时有三种 class member functions: _static_, _nonstatic_ 和 _virtual_. 
#### 简单对象模型
```C++
class Point
{
public:
    Point( float xval );
    virtual ~Point();
    float x() const;
    static int PointCount();
protected:
    virtual ostream&
        print( ostream &os ) const;
    float _x;
    static int _point_count;
};
```
<p align="center">
<img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210331123916.jpg"/>
</p>
上面是一个简单模型, 以空间和执行效率为代价降低编译器复杂度,一个对象是一系列slot, 每个指向一个成员, 因此一个对象的大小就是指针大小乘以member个数, 这个模型没有用到实际的编译器中, 但是该观念影响后面的设计.<br>

#### 表格驱动对象模型
另一种思路是将与 member 相关信息抽出来, 放在一个 data member table 和 一个 member function table 中, 后者是一系列的slots, 这种观念成为支持 virtual function 的一个有效方案.
<p align="center">
<img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210331140727.png"/>
</p>

#### C++ 对象模型
这种模型从 C++ 简单对象模型派生而来, Nonstatic 数据成员配置在每个类对象之中, static 数据成员放在 individual class object 之外, static 和 non-static 成员函数也被放在 class object 之外, 下面两个步奏用于支持虚函数:
1. 每个class 产生一堆指向 virtual function 的指针, 放在 vtbl 之中
2. 每个 class object 被安插一个指针, 指向 vtbl, 被称为 vptr. 同时关联的 type_info 对象(用于支持RTTI), 也由vtbl 指出(通常放在第一个slot).
   <p align="center">
   <img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210331141408.jpg"/>
   </p>

