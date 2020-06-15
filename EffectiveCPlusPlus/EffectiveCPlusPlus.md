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
#define CALL_MAX(a,b) f(((a)>(b)) ? (a) :(b))
CALL_MAX(++a,b);// a会被累加2次
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
