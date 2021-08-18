# 深度探索 C++ 对象模型

## Chap 1: 关于对象
C 语言中, 数据和处理数据的操作(函数)是分开声明的, 语言没有支持数据和函数之间的关联性. C++ 加上封装之后, 成本增加了多少? <br>
实际上, C++ 在布局以及存取之间的额外负担主要是由 _virtual_ 引起的:
* **虚函数机制**: 用以支持一个有效率的执行期绑定
* **虚基类**: 用以实现多次出现在继承体系中的base class, 有一个单一而被共享的实例.

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

上面是一个简单模型, **以空间和执行效率为代价降低编译器复杂度**,一个对象是一系列slot(位置,狭槽), 每个指向一个成员, 因此一个对象的大小就是指针大小乘以 member 个数, 这个模型没有用到实际的编译器中, 但是该观念影响后面的设计.

#### 表格驱动对象模型
另一种思路是将与 member 相关信息抽出来, 放在一个 data member table 和 一个 member function table 中, 后者是一系列的slots, 这种观念成为支持 virtual function 的一个有效方案.
<p align="center">
<img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210331140727.png"/>
</p>

#### C++ 对象模型
这种模型从 C++ 简单对象模型派生而来, **non-static 数据成员配置在每个类对象之中**, static 数据成员放在 individual class object 之外, static 和 non-static 成员函数也被放在 class object 之外. 而虚函数由下面两个步骤支持:
1. 每个class 产生一堆指向 virtual function 的指针, 放在 vtbl 之中
2. 每个 class object 被安插一个指针, 指向 vtbl, 被称为 vptr. 同时关联的 type_info 对象(用于支持RTTI), 也由vtbl 指出(通常放在第一个slot).
   <p align="center">
   <img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210331141408.jpg"/>
   </p>

这种模型带来一个问题, 如果代码本身没有修改, 但是用到的类对象的 non-static data members 有所修改, 那么这些应用程序代码就同样需要重新编译.

#### 加上继承
C++ 支持多继承, 甚至继承关系也可以指定为虚拟(virtual, 共享的意思). 在虚拟继承的情况下, base class 不管在继承链中被派生多少次, **永远只会存在一个实例**:
   <p align="center"><img src="https://i.loli.net/2021/08/18/clmGEtZ4y76bqUr.png"/></p>

```C++
class istream : virtual public ios { ... };
class ostream : virtual public ios { ... };
```
有了继承之后, 在简单对象模型中, 每个 base class 可以被 derived class object 中的某个 slot 指出,该体制的缺点在于因为间接性而导致的空间和存取时间上的额外负担, 优点是 class object 的大小不会因为 base classes 的改变而受影响.  
另一种就是所谓 base table 模型, **和虚函数类似**, 它同样因为间接性而导致的空间和存取时间上的额外负担, 优点是每个 class object 中对于继承都有一致的表现方式, 与 base class 的大小或者个数(相对于前面)无关, 并且无需改变 class objects 本身, 就可以放大/缩小或者更改 base class table.  
C++ 最初采用的继承模型并不运用任何间接性, base class subobject 的数据成员被直接放置在 derived class object 中, 这提供了最紧凑且有效的存取, 缺点就是 base class members 的任何改变(增加/移除/改变类型), **都使得所有用到它的 derived class 的对象必须重新编译**.  
C++ 2.0 引入virtual base class, 初始模型是为每一个有关联的 virtual base class 加上一个指针, 其他演化出来的模型可能是导入一个 virtual base class table, 也可能是扩充原已存在的 virtual table.  
#### 对象模型如何影响程序
这时候我们写的一个函数在内部可能和我们所写的完全不同:
```C++
// code we write
X foobar()
{
    X xx;
    X *px = new X;
    // foo() is virtual function
    xx.foo();
    px->foo();
    // ~X() is a virtual dtor
    delete px; 
    return xx;
};
// Probable internal transformation
// Pseudo C++ code
void foobar( X &_result )
{
    // construct _result
    // _result replaces local xx ...
    _result.X::X();
    // expand X *px = new X;
    px = _new( sizeof( X ));
    if ( px != 0 )
        px->X::X();
    // expand xx.foo(): suppress virtual mechanism
    // replace xx with _result
    foo( &_result );
    // expand px->foo() using virtual mechanism
    ( *px->_vtbl[ 2 ] )( px )
    // expand delete px;
    if ( px != 0 ) {
        ( *px->_vtbl[ 1 ] )( px ); // destructor
        _delete( px );
    }
    // replace named return statement
    // no need to destroy local object xx
    return;
};
```
因为对象模型的内部如下:
   <p align="center"><img src="https://i.loli.net/2021/08/18/a2FBXOTNxzjItE8.png"/></p>

### 关键词带来的差异
