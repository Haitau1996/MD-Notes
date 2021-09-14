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
因为对象模型的内部如下, 同时可能在内部有 RVO:
   <p align="center"><img src="https://i.loli.net/2021/08/18/a2FBXOTNxzjItE8.png"/></p>

### 关键词带来的差异
#### 关键词的困扰
struct 通常的意思是一个数据集合体, 没有 private date, 也没有对应的操作(成员函数), C 的 Struct 和 C++ 支持的 class 之间有观念上的重要差异, 但是**关键词本身并不提供这些差异**:例如我们前置声明时候使用的是其中一个, 实现的时候使用另外一个, 这并不会造成矛盾, 只是不符合"一致性用法"这种风格而已.
```C++
// illegal? no ... simply inconsistent
class node;
...
struct node { ... };
```
但是在模板参数列表中, 现代编译器都只能使用 class/typename, 而无法使用 struct. **关键词 class 的引入并不只是关键词, 还有所支持的封装和继承的哲学**,而保留 struct 更多是方便 C 程序员迁移.

#### 策略正确的 struct
C 程序员的巧计在 C++ 中可能被视为陷阱, 过去把单一元素的数组放在一个结构体尾端, 结构体的对象就可以拥有可变大小的数组:
```C++
struct mumble {
    /* other stuff */
    char pc[ 1 ];
};
```
但是 C++ 中, 处于同一个 access section 的数据必定保证按其声明顺序出现在内存中, 但是**多个 access section 中的各笔数据顺序就是未定义的**. 
```C++
class stumble {
public:
    // operations ...
protected:
    // protected stuff
private:
    /* private stuff */
    char pc[ 1 ];
};
```
如果我们希望实现成那样, 最好将那部分抽取出来成为一个独立的 struct 声明, 从 C struct 中派生出 C++ 的部分:
```C++
struct C_point { ... };
class Point : public C_point { ... };
// C 和 C++ 两种用法都获得支持
extern void draw_line( Point, Point );
extern "C" void draw_rect ( C_point, C_Point );

draw_line( Point( 0, 0 ), Point( 100, 100 ));
draw_rect( Point( 0, 0 ), Point( 100, 100 ));
```
现在, **组合而非继承, 才是将 C 和 C++ 结合在一起的唯一可行方法**, 可以实现类型转换将一种转为另一种:
```C++
struct C_point { ... };
class Point {
public:
    operator C_point() { return _c_point; }
    // ...
private:
    C_point _c_point;
    // ...
};
```

### 对象的差异
C++ 程序设计模型支持多种范式
* 过程模型
* 抽象数据类型模型
* 面向对象模型
* 函数式编程模型

在 OO 中, **只有通过  reference 或者 pointer 的间接处理, 才支持所需的多态特性**.被指定的 Object 的真实类型在每一个特定点执行之前, 是无法解析的. 而在 ADT范式中, 程序处理的是一个拥有固定而单一类型的实例, 在编译时期就已经完全确定好了. 
```C++
void rotate(
    X datum,
    const X *pointer,
    const X &reference )
{
    // cannot determine until run-time
    // actual instance of rotate() invoked
    (*pointer).rotate();
    reference.rotate();
    // always invokes X::rotate()
    datum.rotate();
}
main() {
    Z z; // a subtype of X
    rotate( z, &z, z );
    return 0;
}
```
上段代码中使用对象副本调用的操作是没有多态行为的.   
在 C++ 中, 多态只存在于一个个 public class 体系中, Non-Public 的派生行为以及类型为 void* 的指针也可以说是多态的, 但是他们必须经由显式的类型转换操作来管理. C++ 以下列方法支持多态:
* 经由一组隐式类型转换
    ```C++
    shape *shape_ptr = new Circle();
    ```
* 经由虚函数机制
    ```C++
    shape_ptr-> rotate()
    ```
* 经由 dynamic_cast 和 typeid 运算符
    ```C++
    if ( circle *pc =dynamic_cast< circle* >( ps )) {}
    ```

多态的主要用途是经由一个共同的接口来影响类型的封装, 接口通常被定义在一个 抽象的 base class 中. 这使得 当类型有所增加、修改、或删减时，我们的程序代码不需改变, 同时, sub class 的供应者不需要重新写出“对继承体系中的所有类飞都共涌＂的行为和操作。  
而一个 class 对象的空间占用包含下面的部分:
* 其 nonstatic data members 的总和大小；
* 加上任何由于 alignment（对齐, 32位计算机上通常为 4 byte）的需求而填补(padding) 上去的空间（可能存在千members 之间，也可能存在于集合体边界）．
* 加上为了支待virtual 而由内部产生的任何额外负担(overhead).

#### 指针的类型
实际上, 指向不同类型的指针(包括内置类型和用户自定义类型), 他们之间的差异不在于内容不同, 而是其所寻址出来的对象不同: **指针类型会教导编译器如何解释某个特定地址中的内容及其大小**.   
所谓的 cast 其实是一种编译器指令, 大部分时候并不改变一个指针所包含的真正地址, 只是影响被指出内存的带下和其内容的解释方式. 
#### 加上多态之后
```C++
Bear b;
ZooAnimal *pz = &b;
Bear *pb = &b;
```
<div align=center><img src="https://i.imgur.com/YwpOwzr.png"/></div>

这个时候, pb 涵盖的地址包含整个 Bear Object 而 pz 只是包含其中的 ZooAnimal subobject.
```C++
// 不合法： cell_block 不是ZooAnimal 的一个member,
// 虽然我们知道 pz 当前指向一个Bear object.
pz->cell_block;

// ok: 经过一个显式的 c-style 的downcast 操作就没有问题！
((Bear*)pz)->cell_block;

// better: 但它是一个 runtime operation （成本较高）
if (Bear* pb2 = dynarnic_cast< Bear*>(pz))
pb2->cell_block;

// ok: 因为cell_block 是Bear 的一个member.
pb->cell_block;
```
pz 的类型将在编译期决定下面两点:
* 固定的可用接口(ptrToZooAnimal 只能调用 ZooAnimal 的接口)
* 该接口的 access level

在执行点, pz 所指向的 object 类型决定函数所调用的实例, 类型信息的封装并不是在 pz 中维护, 而是在一个 link 中, 它存在于 object 的 vptr 和 vptr 所指向的 vtbl 中.  
```C++
Bear b;
ZooAnimal za = b;
za.rotate(); // ZooAnimal::rotate() invoked
```
OO 程序设计并不支持对 object 的直接处理, 多态造成一个以上类型(既是基类又是派生类)的力量并不发生在直接存取 object 这件事情上, 故 za 是一个 ZooAnimal 实例而非 Bear 实例. 此外, 编译器在 initialization 以及 assignment  of one class object with another 两件事情上做了折衷, 确保当某个对象有一个或者以上的 vptr 时候, ptr 的内容不会被源对象初始化或者改变.   
当一个 base class boject 被直接初始化为(或者赋值为)一个 derived class object 时候, ==**derived object 就会被切割以便塞入较小的 base type 内存中, derived type 不会留下任何蛛丝马迹, 多态于是不在呈现**. 而 reference 和 pointer 致所有只支持多态, 是因为他们没有 involve any type-dependent commitment of resources, **只会改变所指向内存的大小和内容的解释方式**==.  
