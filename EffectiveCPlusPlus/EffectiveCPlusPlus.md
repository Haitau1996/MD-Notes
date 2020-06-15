# Effective C++ :  改善程序与设计的55个据图做法

## Accustoming Yourself to C++
### Item 1 : 视c++为一个语言联邦
C++为一个多范式的编程语言，同时支持过程形式、面向对象形式、函数形式、泛型形式和元编程形式。为了避免误解，最好将它看成语言的联邦而不是单个语言，互相之间迁移时候，守则可能发生变化。

- C ： 只用其中的C成分工作的时候，高效编程的守则和C相同，没有模板、异常和重载。。。
- Object-Oriented C++ : 面向对象设计的时候，有class(构造、析构)、封装、继承、多态和虚函数（动态绑定）。。。
- Template c++ : 带来了元编程的范式，和TMP的相关规则和主流c++相互之间的影响很小。
- STL：使用template的库，对容器、迭代器、算法和仿函数之间的规约有紧密的配合与协调。

### Item 2 : 以编译器替换预处理器
```c
#define ASPECT_RATIO 1.653
```
在编译前将所有ASPECT_RATIO都替换成了1.653，编译错误的提示信息也是1.653，十分迷惑。
```c
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

### Item 3 : Use const whenever possiable
const允许语义约束，指定变量不可更改，编译器会强制试试这项约束，只要这是事实，就应该明确要求，让编译器保证该约束不会被违反。对于顶层指针和底层指针，只要记住：如果const出现在*左边，则被指向的对象是一个常量，而右边则表示指针本身是一个常量。
```c++
void f1(const Widget* pw);
void f2(Widget const* pw);
//两者意思相同，都是指向一个Widget类型常量的指针
```
对于迭代器，也有类似的做法，如果希望迭代器指向的数据不可变动，可以声明为const_iterator,如果希望迭代器本身不可变，声明为const。当然，const威力最大的场景在于函数声明中：
```c++
class Rational{...};
const Rational operator*(const Rational& lhs,const Rational& rhs);
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

### Item 4: 确定对象使用前已经被初始化
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

## 构造、析构和赋值操作
### Item 5 了解c++默认编写并且调用了哪些函数
如果自己没有声明，那么编译器就会为它声明下面三个函数：
- copy 构造函数
- copy assignment 操作符
- 析构函数

