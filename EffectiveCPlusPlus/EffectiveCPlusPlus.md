# Effective C++ :  改善程序与设计的55个据图做法

## Accustoming Yourself to C++

### Item 1 视c++为一个语言联邦
C++为一个多范式的编程语言，同时支持过程形式、面向对象形式、函数形式、泛型形式和元编程形式。为了避免误解，最好将它看成语言的联邦而不是单个语言，互相之间迁移时候，守则可能发生变化。

- C ： 只用其中的C成分工作的时候，高效编程的守则和C相同，没有模板、异常和重载。。。
- Object-Oriented C++ : 面向对象设计的时候，有class(构造、析构)、封装、继承、多态和虚函数（动态绑定）。。。
- Template c++ : 带来了元编程的范式，和TMP的相关规则和主流c++相互之间的影响很小。
- STL：使用template的库，对容器、迭代器、算法和仿函数之间的规约有紧密的配合与协调。

### Item 2 以编译器替换预处理器
```c++
#define ASPECT_RATIO 1.653
```
在编译前将所有ASPECT_RATIO都替换成了1.653，编译错误的提示信息也是1.653，十分迷惑。
```c++
const double AspectRatio = 1.653;
```
常量的定义有两点值得注意的：

- 顶层指针（指针指向同一个可变对象）和底层指针（无法通过指针修改对象），同时string比char * based 字符串要好
- class专属的常量，为了将作用域限制在class内部，让它作为member data，同时为了常量只有一个实体，让它成为一个静态成员

旧的编译器不支持static成员在其声明时候获得初值，则可以将初值放在定义式中
```c++
// in the header file
class ConstEstimate{
    private:
        static const double FudgeFactor;
    ...
};
// in the implementation file
const double ConstEstimate::FudgeFactor = 1.35;
```
还有一种情况，就是在class编译期间需要一个常量，但是编译器不允许static整型完成in class初值，使用**enum hack**的补偿做法：
```c++
class GamePlayer{
    private:
        enum{NumTurns = 5};
        ...
        int Scores[NumTurns];
        ...
};
```
对这种trick，我们需要了解，该行为比较像`#define`而不是const，给const取地址是合法的，但是给enum和define取通常都是不合法的。<br>
预处理会带来一些问题，如
```c++
#define CALL_WITH_MAX(a,b) f(((a)>(b)) ? (a) :(b))
CALL_WITH_MAX(++a,b);// a会被累加2次
```
这种时候可以写出template的inline函数：
```c++
template<typename T>
inline void callWithMax(const T&a, const T&b){
    f(a>b ? a :b);
}
```
- 对于单纯的常量，最好用const和enum替换#define
- 对于函数的宏，最好用inline函数替换#define

### Item 3 Use const whenever possiable
const允许语义约束，指定变量不可更改，编译器会强制试试这项约束，只要这是事实，就应该明确要求，让编译器保证该约束不会被违反。对于顶层指针和底层指针，只要记住：如果const出现在*左边，则被指向的对象是一个常量，而右边则表示指针本身是一个常量。
```c++
void f1(const Widget* pw);
void f2(Widget const* pw);
//两者意思相同，都是指向一个Widget类型常量的指针
```
对于迭代器，也有类似的做法，如果希望迭代器指向的数据不可变动，可以声明为const_iterator,如果希望迭代器本身不可变，声明为const。当然，const威力最大的场景在于函数声明中：
```c++
class Rational{...};
const &Rational operator*(const Rational& lhs,const Rational& rhs);
```
如果不声明为const，那么客户就可以这样写：
```
Rational a,b,c;
...
(a*b) = c; // 用c给a*b的结果赋值，加了const之后返回一个const Rational那么就会找出这个错误
```
#### Const menber function
将成员函数声明为const的一个很大的作用是，使得该函数可以操作const对象。pass by reference-to-const的一个前提就是可以用const修饰成员函数，<font color=red>否则一般的函数操作const对象，编译器无法得知它是否会改变对象的值，因此报错。</font>
```c++
class TextBlock {
public:
    ...
    const char& operator[](std::size_t position) const // operator[] for
        { return text[position]; } // const objects
    char& operator[](std::size_t position) // operator[] for
        { return text[position]; } // non-const objects
private:
    std::string text;
};
```
值得注意的的是，函数返回的是一个reference to char，如果只是返回char，那么它是local变量，对它赋值没有意义。成员函数为const有两个流行概念，物理const 和 逻辑const:

- menber function只有在不改变对象的任何成员变量时候才可以说是const，但是实际上，通过指针可能可以改变对象成员的值。（编译器强制）
- 逻辑const:const成员函数可以修改对象的某些bits，但是只有在客户端侦测不出时候才可以如此（写程序遵循的守则）

C++中有个与const相关的wiggle room叫做mutable, 它将释放掉non-static成员变量的bitwise constness,
```c++
class CTextBlock {
public:
    ...
    std::size_t length() const;
private:
    char *pText;
    mutable std::size_t textLength; // these data members may
    mutable bool lengthIsValid;     // always be modified, even in
};                                  // const member functions
std::size_t CTextBlock::length() const
{
    if (!lengthIsValid) {
    textLength = std::strlen(pText); // now fine
    lengthIsValid = true; // also fine
    }
    return textLength;
}
```
#### const 和 non-const成员函数中避免重复
如果member function不单返回一个reference指向字符，而且做了很多操作，那么重载函数就变得非常复杂并且没有必要，我们真正要做的是实现一次，然后通过常量性转除去实现：
```c++
class TextBlock {
public:
    ...
    const char& operator[](std::size_t position) const // same as before
    {
        ...
        ...
        ...
        return text[position];
    }
    char& operator[](std::size_t position) // now just calls const op[]
    {
        return
        const_cast<char&>( // cast away const on
                           // op[]’s return type;
                static_cast<const TextBlock&>(*this) // add const to *this’s type;
                [position] // call const version of op[]
        );
    }
...
};
```
实际使用的过程中，为了避免no-const operator[]递归调用自己，先将*this从原始的TextBlock& 做static_cast成为const TextBlock&，然后从const operator[]的返回值中移除const。但是，反过来做是不建议的，因为使用const_cast去掉了const的性质之后，操作十分不安全。

### Item 4 确定对象使用前已经被初始化
c++初始化问题：
```c++
int x;  //大多数平台确定了x被初始化
class Point{
    int x,y; 
};
...
Point p;// 数据成员x,y没有被初始化
```
一般而言，C part of c++初始化可能导致运行期成本，不保证发生初始化，non-C parts of c++，规则就相反。内置类型在使用之前将它初始化，对于内置类型之外的类型，初始化<font color=red>由构造函数负责，确保构造函数将对象的每一个成员都初始化。</font>构造函数比较好的写法是member initialization list替换赋值动作.
// todo: 重新看这部分内容 29-33
***
## 构造、析构和赋值操作

### Item 5 了解c++默认编写并且调用了哪些函数
如果自己没有声明，那么编译器就会为它声明下面几个函数：
- 默认构造函数
- copy 构造函数
- copy assignment 操作符
- 析构函数
```c++
Empty e1;       // default constructor;
                // destructor
Empty e2(e1); // copy constructor
e2 = e1;      // copy assignment operator
```
需要注意的是，编译器自身产生的destructor是一个non-virtual的析构函数，除非base class自己声明有virtual的析构函数。<br>
当我们写了一个构造函数的时候，编译器不会生成默认沟赵函数，但是因为没有copy构造函数和copy赋值operator，编译器依旧会自己默认一个。<br>
编译器生成的copy 构造函数将传入的object每个data member为初值设定新object的值。万一有条件不符合，编译器拒绝给class生成operator=.<br>
例如，我们有一个class包含两个data member，一个referce to string，一个const T，那么使用这个class实例化两个实例，a,b，令a = b 就会出问题，因为reference自身不可变动，而const T也不能再赋值，这种情况下编译器不知道如何生成一个copy assignment operator。

### Item 6 如果不想用编译器自动生成的函数，就应该明确拒绝

有的使用场景中，不允许拷贝赋值和拷贝assignment operator，我们不主动拒绝的话，编译器会声明我们不想要的函数。这种时候一个常见的做法是将copy constructor和copy assignment operator声明为private，这样的话member function和friend function还可以调用他们，如果足够聪明，只声明不调用的话不慎调用会得到一个linkage error.
```c++
class HomeForSale{
    public:
        ...
    private:
        HomeForSale(const HomeForSale&);
        HomeForSale& operator=(const HomeForSale&);
};
```
更好的做法是将这种东西放到一个base类中，然后HomeForSale用private的方式继承该类，这样任何人，<font color=red>甚至是member函数或者friend函数</font>都无法尝试拷贝该对象。使用该方法可能会引发多继承的问题，涉及的empty base class optimization，这将在Item 39、40中讨论。

### Item 7 为多态基类声明virtual析构函数
在很多时候，我们使用指针得到一个derived class对象，而那个对象却经过一个base class的指针被删除，带来的问题是诡异的“局部销毁”，而derived class的析构函数没有被调用。解决该问题的方法十分简单，<font color=red>给base class一个virtual析构函数</font>.
```c++
class TimeKeeper{
    public:
        TimeKeeper();//默认构造函数
        virtual ~TimeKeeper();
        ...
};
TimeKeeper* ptk= getTimeKeeper();
...
delete ptk;
```
这并不意味着virtual函数很好，对于不作为base class的情况，virtual析构函数会带来一系列的问题，没有virtual析构函数的情况下，一个自己定义的Point2D对象可以作为一个64-bit的量传给c/fortran,对于virtual函数，对象必须携带某些信息，用于决定哪个virtual函数会被调用，这种事由virtual table pointer(vptr) 的机制实现。因此，一般而言，只有带class内至少有一个virtual函数的的时候，才为它声明virtual析构函数。<br>
同样的，对于不带virtual析构函数的class，包括所有的STL容器（vector,list,set,unordered_map)和std::string，不要从他们那里继承。<br>
class带一个pure virtual析构函数，pure virtual函数导致抽象class，不能被实例化。如果希望得到类似的class又没有其他的纯虚函数，可以给class声明一个纯虚的析构函数：
```c++
class AMOV{
    public:
        virtual ~AMOV()=0;  // pure virtual destructor
}
AMOV::~AMOV(){ }  // 必须为这个pure virtual析构函数提供一份定义
```
当然，这些性质的讨论都是适用于<font color=red>polymorohic base class</font>, 但是并非所有的base class设计都是为了多态用途，他们不需要virtual析构函数。

### Item 8 别让异常逃离析构函数
```c++
class Widget {
    public:
        ...
        ~Widget() { ... } // assume this might emit an exception
};
void doSomething(){
    std::vector<Widget> v;
    ...
}                           // v is automatically destroyed here
```
只要析构函数突出异常，即使并非使用容器或者vector，程序也可能过早结束或者出现不明确的行为。如果析构函数必须执行某个动作，而该动作可能在失败时抛出异常，这种情况该怎么办？<br>
```c++
class DBConnection {
    public:
        ...
        static DBConnection create(); // function to return
        // DBConnection objects; params
        // omitted for simplicity
        void close(); // close connection; throw an
}; // exception if closing fails
class DBConn { // class to manage DBConnection
    public: // objects
        ...
        ~DBConn() // make sure database connections
        { // are always closed
                db.close();
        }
    private:
        DBConnection db;
};
```
一个做法是使用另一个class，这个class包含DBConnection的数据，并且在析构函数中调用close(),如果程序正常运行，那么这个部分没有问题，如果close()出错，那么这依旧会导致异常离开析构函数，有两个办法可以解决这个问题：

- close()抛出异常就结束程序
- 吞下调用close()发生的异常

但是这两种做法都没有吸引力，更好的做法是重新设计DBConn的接口，如提供一个close()，赋予客户操作”因该操作而发生的异常“，并且追踪管理的connect是否关闭，否的话由connect的析构函数将它关闭。
```c++
class DBConn {
    public:
        ...
        void close() // new function for
        { // client use
            db.close();// 这个地方可能导致异常
            closed = true;
        }
        ~DBConn(){
            if (!closed) {
                try { // close the connection
                db.close(); // if the client didn’t
                }
                catch (...) { // if closing fails,
                make log entry that call to close failed; // note that and
                ... // terminate or swallow
                }
            }
        }
    private:
        DBConnection db;
        bool closed;
};
```
### Item 9 不要在构造和析构函数中调用virtual函数
**Java /C#在这方面和c++不同** derived的构造函数调用之前，base的构造函数一定会更早调用。这个时候，我们在derived class构造函数中调用virtual的函数，先构造base class期间virtual函数不会到derived class的那层，<font color=red>这时候构造期间的virtual函数就不是virtual函数（没有多态）</font>。就是在base class构造期间，对象的类型是base class 而不是derived class。<br>
同样的道理，derived class进入base析构函数中就成了一个base对象，C++的任何部分，virtual 函数、dynamic_cast等等也将它视为是一个base class。<br>
避免此类问题的做法是:**确定构造函数和析构函数都没有调用virtual函数，并且它们调用的函数也服从这个约束。**<br>
此外还有其他方案：在base class中的log函数改成non-virtual，然后要求derived class传递必要的信息给base class的构造函数，之后构造函数就可以安全的调用non-virtual版本的log函数同时在不同的derived class中Log不同。

### Item 10 令operator=返回一个reference to *this
赋值的时候，人们常常将这个写成一个连锁形式：
```c++
int x,y,z;
x = y = z = 15;
//赋值时候的右结合，转义为x = (y = (z=15))
```
为了实现这种行为，赋值操作符必须返回一个reference指向操作符左侧实参：
```c++
class Widget{
    public:
        Widget& operator = (const Widget& rhs){
            ...
            return *this;
        }
    ...
};
```
对于其他赋值相关的运算，+=和-=之类，也建议遵循这个规则。

### Item 11 在operator=中处理“自我赋值”
自我赋值就是对象自己给自己赋值,大多数时候自我赋值并不是那么明显：
```c++
class Widget { ... };
Widget w;
...
w = w; // assignment to self
a[i] = a[j] //潜在自我复制，i可能等于j
*px = *py ; // px,py可能指向同一对象
```
在尝试自行管理资源的时候，可能会“在停止使用资源之前意外释放了它”，
//todo: 加入这段代码
在上面的代码里，这个delete不但销毁了当前对象的bitmap，传入rhs的bitmap也被销毁了。传统的解决办法：<font color=red>在operator=最前面加一个证同测试。</font><br>
这个新版本依旧存在异常方面的麻烦，在new Bitmap时候出现异常的话，对象会持有一个指向被删除Bitmap的指针。下面的做法会在new Bitmap之后再删除原来的Bitmap，出现异常后，原来Bitmap没有丢失：<br>
//todo:插入代码
另一个方案是copy and swap技术：

### Item 12 复制对象的时候别忘了它的每一个部分

当我们自己声明copying函数，实现的代码几乎必然出错编译器并不会告诉你。一个典型的例子是，给一个class写好copying函数之后，随着需求的变化**加入了新的data member**，但是copying 函数没有随之变化。<br>
更极端的情况是，随着该类被继承，我们在继承类的copying函数中更加难以发现base class的data member没有被拷贝的情况。此外，我们必须小心地复制base class成分，而这些成分往往是private的，无法直接访问，因此我们要**让derived class的copying函数调用相应的base class copying 函数**。
//todo: add code here
copy构造函数和copy assignment operator有相近的代码，但是**不应该让两者相互调用**,更理想的方式是写一个private的init函数，然后在两个copying函数中都调用它。
***

## 资源管理
C++中最常用的资源就是动态内存分配，除此之外，其他常见资源包括文件描述器、互斥锁、数据库连接以及网络sockets。**无论什么资源，重要的是，当你不再使用它，必须将它归还给操作系统**。<br>

### Item 13 以对象管理资源
```c++
Investment* createInvestment(); //使用工厂函数动态生成Investment对象
void f(){
    Investment *pInv = createInvestment(); // call factory function
    ...                 // use pInv
    delete pInv;        // release object
}
```
这时候，在很多情况下可能没有办法删除pInv指向的对象，比如'...'中有return语句（类似情况在for loop等中由于continue\goto\break 没有释放资源），或者'...'中的语句抛出了异常。为此，需要将资源放入对象中，该对象的析构函数就会释放资源。<font color=red>标准程序库提供auto_ptr（智能指针），析构函数自动对其所指的对象调用delete:</font>
```C++
void f(){
    std::auto_ptr<Investment> pInv(createInvestment()); // call factory function
    ... // use pInv as before
}       // automatically delete pInv via auto_ptr’s dtor
```
* 获得资源后立即放入管理对象内(Resource Acquisition Is Initialization,RAII)
* 管理对象应用析构函数确保资源释放

此外需要注意的是，智能指针被销毁会删除所指之物，不要让多个智能指针指向同一对象。通过copy构造函数和copy assignment operator复制他们后，自己会变成null。<br>
替代方案是"引用计数型指针"，会追踪多少对象指向某个资源，并且在无人指向他们时候自动删除（类似于垃圾回收），但是无法打破环引用（如两个互相指但是不被使用的对象，处于“被使用状态”）。<br>
auto_ptr 和 shared_ptr 析构函数用的是delete而不是delete[],**不要对动态分配得到的array**使用。<br>

### Item 14 在资源管理类中小心copying行为

前面描述的auto_ptr 和 shared_ptr 只适用于heap-based资源，这时候**需要建立自己的资源管理类**。这时候需要注意一些事项,以C API中处理Mutex的互斥对象为例
```C++
//  apis from c programming language
void lock(Mutex *pm);   // lock mutex pointed to by pm
void unlock(Mutex *pm); // unlock the mutex
//  RAII   规则下管理资源
class Lock {
    public:
        explicit Lock(Mutex *pm): mutexPtr(pm)
            { lock(mutexPtr); } // acquire resource
        ~Lock() { unlock(mutexPtr); } // release resource
    private:
        Mutex *mutexPtr;
};
```
但是一个RAII对象被复制的时候，我们需要一些手段来处理它：

- **禁止复制**, 对于这些不能合理拥有“同步化基础器物”副本的对象，private 继承 Uncopyable类，直接禁止复制。
- **底层资源“引用计数”** 如果对上面的lock想要使用reference counting，那么就可以将`Mutex*`改为`shared_ptr<Mutex>`,默认的操作是删除Mutex*, 但是我们希望的是锁定而非删除，而shared_ptr允许指定删除器
```c++
class Lock {
    public:
        explicit Lock(Mutex *pm) // init shared_ptr with the Mutex
        : mutexPtr(pm, unlock)   // to point to and the unlock func
        {                        // as the deleter†
        lock(mutexPtr.get());    // see Item15 for info on “get”
        }
    private:
    std::tr1::shared_ptr<Mutex> mutexPtr; // use shared_ptr instead of raw pointer
};
```
- **复制底部资源** 深度复制 //todo:这部分再加强理解 
- **转移底部资源的拥有权** 希望只有一个RAII对象指向未加工的资源，这时候资源的拥有权会从<u>被复制物</u>转向<u>目标物</u>。

### Item 15 在资源管理类中提供对原始资源的访问

在很多情况下，我们需要将RAII对象转换为其所内含的对象，那么就有两种方式，显式和隐式转换：

- 显式转换：shared_ptr和auto_ptr提供一个get()成员函数，允许返回指针内部的原始指针(的复件)；
- 隐式转换：shared_ptr和auto_ptr重载了指针取值操作符，operator -> 和 operator *,允许隐式转换为底层的指针。

//todo 

### Item 16 成对使用new和delete时采取相同的形式

new的时候，两件事情发生：内存被分配出来，然后对这片内存做一个或者多个的构造函数。同样的，delete的时候，先对此内存做一个或者多个析构函数，再释放内存。最大的问题在于：**即将删除的内存究竟有多少个对象**，数组内存通常还包括数组大小的记录，因此我们需要清楚删除的是一个对象还是一个数组。<br>
规则十分简单，调用new时候使用[ ]，那么在delete的时候也使用[ ],反之亦然。但是如果经常使用typedef，new的时候[ ]被typedef掩盖了，那么就要十分注意。**最好尽量不要对数组形式做typedef动作**。
```C++
typedef std::string AddressLines[4]; // a person’s address has 4 lines, each of which is a string
std::string *pal = new AddressLines;
delete pal;    // undefined!
delete [] pal; // fine
```

### Item 17 以独立语句将newed对象置入智能指针


***

## 设计与声明

### Item 18 让接口容易被正确使用

想要开发满足这种要求的接口，首先需要考虑客户可能做出什么样的错误。例如，一般实现一个用于表现日期的class，一般的构造函数可能会犯两个错误：

- 客户可能以错误的次序传递参数
- 客户可能传递无效的天数或者月份

类型系统是主要防范“undesirable code from compiling”的主要盟友，我们可以导入外覆类型(wrapper types)来区别天，月和年份，然后再在date的构造函数中使用这些类型。、
```c++
Date d(30, 3, 1995);                   // error! wrong types
Date d(Day(30), Month(3), Year(1995)); // error! wrong types
Date d(Month(3), Day(30), Year(1995)); // okay, types are correct
```
一旦类型被定位，那么我们可以限制其取值，一个方法是用enum来表现月份，但是其类型不够安全