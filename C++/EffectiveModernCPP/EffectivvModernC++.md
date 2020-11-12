# Effective Modern C++
  <font size=4> Scott Meyers</font>

## Chap 1: Deducing Types

传统的C++支持function Template中的类型推导，在C++ 11中引入了auto 和decltype，在 C++14 中加入了decltype(auto) , 这个可能导致函数更难reason about， 因此在这里将讨论更加现代的类型推导用法。

### Item 1 理解Template 类型推导
```C++
template<typename T> 
void f(ParamType param);
...
f(expr);  
```
一般我们认为T 和 _ParamType_ 是一个类型, 但是可能有三种情况:
* _ParamType_ 是一个 pointer或者reference, 但是不是一个 Universal reference.
* _ParamType_ 是一个 universal reference
* _ParamType_ 既不是pointer又不是reference

#### _ParamType_ 是一个引用或指针, 但不是通用引用
这种情况最简单, 类型推导方式如下:
1. _expr_ 的类型是引用的话, 忽略 reference的部分
2. 用 _expr_ 的pattern和 _ParamType_ 对比决定 T 的类型

重要的是如果传入一个`const` object 到一个 reference参数中, 他们是希望传入的对象 remain unmodifiable. 
```C++
const int& rx = x;       // as before
f(rx);                   // T is int, param's type is const int&
```
从这个例子中可以看到, 传入一个 `const int&` 和 一个 `const int` 对于推导 T的类型都是一样的.

#### _ParamType_ 是一个 Universal reference
当template 带有 universal reference parameter,如参数被声明为右值引用时候,分两种情况讨论:
1. _expr_ 是一个 lvalue, T 和 _ParamType_ 都被推导为左值引用
2. 如果 _expr_ 是右值, 上一节的规则同样适用
```C++
template<typename T>
void f(T&& param);       // param is now a universal reference
int x = 27;              // as before 
const int cx = x;        // as before 
const int& rx = x;       // as before
f(x);                    // x is lvalue, so T is int&, 
                         // param's type is also int&
f(cx);                   // cx is lvalue, so T is const int&                       
                         // param's type is also 
const int& f(rx);        // rx is lvalue, so T is const int&, 
                         // param's type is also 
const int& f(27);        // 27 is rvalue, so T is int,
                         // param's type is therefore int&&
```
总的来说, 适用了universal reference之后, 传入左值和右值产生的类型推导结果就会不同.

#### _ParamType_ 不是指针也不是引用
这时候我们处理的是pass by value的情况, 如:
```C++
template<typename T>
void f(T param);         // param is now passed by value
const char* const ptr =  // ptr is const pointer to const object
                        "Fun with pointers";
f(ptr);                  // pass arg of type const char * const
```
* 如果传入的 _expr_ 是reference, 忽略引用的部分
* 忽略之后如果 _expr_ 是const, 也忽略

因为传值时候传入的是const, 函数使用的是它的副本, 修改副本的值不会影响传入参数的const性质.上面例子中, __constness of _ptr_ itself is ignored when copying it to create the new pointer, param.__<br>

#### Array/function 参数
从C语言继承而来的特性, array parameter的声明被当成是 pointer的parameter,下面的会被推导成一个 `const char *`:
```C++
const char name[] = "J. P. Briggs";
f(name);  // T被推导成 const char*
```
但是如果 template with by-reference parameter:
```C++
template<typename T>
void f(T& param);      // template with by-reference parameter
f(name);
```
<font color=red>这个时候, 就会将T推导成为array的实际类型,并且包含array size, 这个T就是 `const char[13]`</font>.<br>
此外,函数也会 **_decay_ 成 pointer, 这个时候的类型推导和array类似**:没有&时候传入函数名则推导成为函数指针,否则是函数的引用(其实两者在实际使用过程中的差别很小).

### Item2: 理解 _auto_ 的类型推导
除了一个意外情况, `auto` 的推导和前面的Template机制是一样的.按照前面的三种分类,依旧得到下面的结果:
```C++
auto x = 27;          // case 3 (x is neither ptr nor reference) 
const auto cx = x;    // case 3 (cx isn't either) 
const auto& rx = x;   // case 1 (rx is a non-universal ref.)

auto&& uref1 = x;     // x is int and lvalue, 
                      // so uref1's type is int& 
auto&& uref2 = cx;    // cx is const int and lvalue, 
                      // so uref2's type is const int& 
auto&& uref3 = 27;    // 27 is int and rvalue, 
                      // so uref3's type is int&&
```
下面我们开始讨论那个唯一的例外情况, 在C++11中引入了一致初始化(uniform initialization),下面4个结果在auto类型推导中结果不是完全相同:
```C++
auto	x1 = 27;    // type is int, value is 27
auto	x2(27);
auto	x3 = { 27 };// type is std::initializer_list<int>,value is { 27 }
auto	x4{ 27 };
```
如果是使用这样的一个一致初始化做类型推导, 那么结果就是`std::initial izer_list`, 如果大括号中的type不同, 无法做类型推导, 代码就会出错:
```C++
auto x5 = { 1, 2, 3.0 };	// error! can't deduce T for 
                            // std::initializer_list<T>
```
处理这种花括号中的初始化器就是Template机制和auto推导的唯一区别:_auto_ assumes that a braced initializer represents a `std::initializer_list`, but template type deduction doesn’t.
```C++
auto x = { 11, 23, 9 };	//	x's type is
	                      //	std::initializer_list<int>
template<typename T>	//	template with parameter
void f(T param);	//	declaration equivalent to
	                //	x's declaration
f({ 11, 23, 9 });	//	error! can't deduce type for T
```
since c++14, auto 可以 indicate函数的返回值可以类型推导, 同时在 _lambda_ 表达式中用作参数声明, 这两个情况用的是 **template type deduction**, 不是 auto type deduction. 这时候不接受 `std::initializer_list`作为参数.

### Item 3: 理解 _decltype_
_decltype_ 告诉你name或者表达式的type,它的结果非常直观:
```C++
const int i = 0;           // decltype(i) is const int 
if (v[0] == 0) …           // decltype(v[0]) is int&
```
在C++11中, decltype的主要用途是声明返回值和参数类型有关的函数模板,如我们用`operator []` 得到容器的返回值类型, 一般而言返回的是 `T&`, 但是 `std::vector<bool>` 返回的不是 `bool&`, 而是一个新的 `bool`对象, 就可以使用下面的做法:
```C++
template<typename Container, typename Index> 
auto authAndAccess(Container& c, Index i) -> decltype(c[i]) 
{
    authenticateUser(); 
    return c[i]; 
}
```
C++11 permits return types for single-statement lambdas to be deduced, and C++14 extends this to both all lambdas and all functions, including those with multiple statements. 意味着编译器可以从函数的实现来推导函数的返回值, 前面的 `->decltype(c[i])` 就可以省略.<br>
```C++
template<typename Container, typename Index>    // C++14; 
auto authAndAccess(Container& c, Index i) {
    authenticateUser();
    return c[i];  
}
std::deque<int> d; 
…
authAndAccess(d, 5) = 10; // ERROR
```
此外, 还有一个细节就是 `d[5]` 返回的是一个 `int&`, 但是auto的类型推导会剥去reference,所以两个都是`rvalue`, 是无法这样赋值的, 故编译器会报错.为了避免用auto这种推导, since c++14, 我们可以使用 `decltype(auto)`:
```C++
template<typename Container, typename Index>  
decltype(auto)  authAndAccess(Container& c, Index i)   
{   // C++14, works, still require refinement 
    authenticateUser(); 
    return c[i]; 
}
```
此外, `decltype(auto)` 可以用于声明变量, 此外上面的实现看起来非常好, 但是还有一个问题,容器是通过 `lvalue-reference-to-non-const`, 这意味着可以修改这个容器的内容, 所以 Rvalue 没办法和这种`lvalue-reference`绑定, 除非是`lvalue-reference-to-const`.当然我们可以用一个函数重载,写出两个函数来实现, 但是更简单的一个方式是写出同时可供`lvalue`和`rvalue`使用的版本:
```C++
template<typename Container, typename Index> 
decltype(auto) authAndAccess(Container&& c,Index i);  
{//final C++14 version 
    authenticateUser();
    return std::forward<Container>(c)[i];
}
template<typename Container, typename Index>
auto authAndAccess(Container&& c, Index i)
-> decltype(std::forward<Container>(c)[i])
{//final C++11 version
    authenticateUser(); 
    return std::forward<Container>(c)[i]; 
}
```
对于`decltype`去求name的类型, 直觉得到的结果就是对的, 但是对于表达式, `decltype` 一般会确保得到的结果是一个`lvalue reference`: Putting parentheses around a name can change the type that decltype reports for it!<br>

### Item 4: 知道怎样查看推导的类型
在三个阶段得到推导类型的信息: 编辑代码的时候, 编译代码的时候, 运行代码的时候.<br>
在编辑代码的时候, IDE(如Clion)可以显示出代码的类型, 但是当涉及更多复杂类型的时候, 来自编译器的提示可能就不那么有用了.<br>
在编译期, 可以用导致编译错误的方式去诊断type的类型,如:
```C++
const int theAnswer = 42;
auto x = theAnswer;
auto y = &theAnswer;
template<typename T>	//	declaration only for TD;
class TD;	//	TD == "Type Displayer"
TD<decltype(x)> xType;	// elicit errors containing
TD<decltype(y)> yType;	// x's and y's types
```
这个时候,可能编译器就会提示出错误的类型, 并且帮助推导得到相应的类型信息:<br>error: 'xType' uses undefined class 'TD<int>' <br>
error: 'yType' uses undefined class 'TD<const int *>' <br>
#### 运行时输出
`typeid` 和 `std::type_info::name` 可以产生类型的 C-style string (i.e., a const char*) representation.但是这种结果不一定很直观, 如在 clang/gcc 中就会说 x,y的类型分别是: “i” 和 “PKi”(pointer to konst(const) i). 另外, `std::type_info::name`的结果并不可靠, 它得到的类型和将结果用 _by-value_ 的方式传入一个模板函数是相同的, 会将 reference-ness/ constness 忽略. 如果需要更精确的结果, 我们可能要使用 Boost TypeIndex库(`Boost.TypeIndex`):
```C++
#include <boost/type_index.hpp>
template<typename T> void f(const T& param) { 
    using std::cout; 
    using boost::typeindex::type_id_with_cvr;	
    //with cvr意思是保留 const, volatile, or reference 标识
// show	T
    cout <<	"T =	"<<	type_id_with_cvr<T>().pretty_name()<<	'\n';
// show	param's type
    cout <<	"param ="<<	type_id_with_cvr<decltype(param)>().pretty_name()<<	'\n';
    … 
}	
```
这样的话在clang/gcc/Microsoft的编译器产生一致的并且human-friendly representation.
***
## Chap 2: auto
### Item 5 : 使用 _auto_ 优于显式类型声明
auto类型推导的一个好处是强制使用初始化, 如:
```C++
auto x2; //error, 要求初始化
auto x3 = 0; // 正确
```
此外, 只有编译器知道的类型(如 lambda 表达式类型),也可以使用auto推导:
```C++
auto derefUPLess =	// comparison func.
    [](const std::unique_ptr<Widget>& pl,	// for Widgets
       const std::unique_ptr<Widget>& p2)	// pointed to by
    { return *p1 < *p2; };	// std::unique_ptrs
//since c++ 14, lambda 表达式的参数也可以使用类型推导
auto derefLess =	// C++14 comparison
    [](const auto& pl,	// function for
    const auto& p2)	    // values pointed
    { return *p1 < *p2; };	// to by anything  pointer-like
```
在C++中, `std::function` Objects 可以 refer to 任何 可调用的对象,i.e., 任何可以像函数一样使用的对象,正如我们使用函数指针的时候需要知道参数的类型, 上面的东西我们可以用`std::function`的template parameter写出, 
```C++
std::function<bool(const std::unique_ptr<Widget>&, 
                   const std::unique_ptr<Widget>&)>
    derefUPLess = [](const std::unique_ptr<Widget>& pl,
                     const std::unique_ptr<Widget>& p2)
                  { return *p1 < *p2; };
```
即便如此, 使用 `std::function`还是和auto不太一样, 使用auto的结果和封装的东西类型一样, 需要的内存也是一样的,但是使用 `std::function` 得到的closure是一个 `std::function`模板的实例,该实例对于任何的函数签名都有固定的size,这个size可能不够存放整个closure,_std::function_ 可能从堆中要内存存放这个closure, 因此会使用更多的内存: **the std::func tion approach is generally bigger and slower than the auto approach**, 并且可能爆出 _out-of-memory_ 异常.<br>
此外, auto也会避免 "type shortcut" 问题:
```C++
std::vector<int> v; 
…
unsigned sz = v.size();
```
实际上 `v.size()` 返回的是一个 `std::vector<int>::size_type`, 在32位系统中它的size和Unsigned相同, 但是在64-bit 的系统中, Unsigned 依旧是32 bits, while `std::vec tor<int>::size_type` is 64 bits.此外, 从下面的例子中也可以看出auto使用起来更加精确:<br>
```C++
std::unordered_map<std::string, int> m; 
...
for (const std::pair<std::string, int>& p : m)
 { 
     …	
     // do something with p
 }
```
但是, `std::unordered_map` 的key部分应该是 **_const_**,所以在hash table中其实是 `std::pair
<const std::string, int>.` 结果就是编译器产生一个类型p临时对象然后将m中的每个对象拷贝给那个对象, 然后将p的引用和那个临时对象绑定.特别是将对象取地址的时候, 使用auto取得的地址是对的, 而用上面那种声明取得地址是临时变量的地址.auto并不完美, 前面item 1提到 initializing expressions相关的问题,但是因为auto的类型可以传播, 而且并没有过分减少程序的可读性.

### Item 6: 如果使用auto推导出不想要的type,使用显示的类型声明
有的时候, auto的类型推导得到的结果是 _zags_ 而我们想要的是 _zag_, 这时候就需要手动写指定我们想要的类型.
```C++
std::vector<bool> features(const Widget& w);
Widget w; 
auto highPriority = features(w)[5];  // is w high priority? no, explained as followed
processWidget(w, highPriority);      // undefined behavior!
```
除了bool之外, opetator[] 返回的都是 T&, 但是 **C++不允许reference to bits**, 这个auto返回的是一个`std::vector<bool>::reference`,它的行为像是 `bool&`,此外, 它可以隐式转换成bool, 所以下面的做法是没有问题的:
```C++
bool highPriority = features(w)[5];  // declare highPriority's type explicitly
```
前面 _auto_ 的错误用法的结果取决于`std::vector<bool>::reference` 是怎样实现的, 如果是包含指向保存引用位的机器word的指针，以及该位在该word中的偏移量,那么auto的 highPriority 包含一个 pointer to a word in temp, 同时有对 bit 5的偏移量. 在那句话结束后, temp就被销毁, 然后 highPriority 包含的指针就相当于是一个 dangling pointer.<br>
实际上, 这是一个 _Proxy_ 的例子, 有的 proxy非常直观(`std::shared_ptr`), 但是有的proxy更像是invisible, 一般来说, **"invisible"的proxy不太适合用auto做类型推导**,因此我们要**避免**下面形式的auto语句:<br>
<font color=#000fff size=4>auto someVar = expression of "invisible" proxy class type;</font><br>
怎样确定一个返回值是否是proxy, 一般而言文档中会有很好的说明, 就算是没有的话, 也很难从源代码中把这种性质藏起来, 我们可以根据这来判断, 如:<br>

```C++
namespace std {	// from C++ Standards
    template <class Allocator> 
    class vector<bool, Allocator> { 
        public:	
            … 
            class reference {…};	
            reference operator[](size_type n);	
            … 
    };
}	
```
这样的话就可以发现, 一般的operator[] 都是返回的一个`T&`, 而这是返回一个类内定义的reference,显然就能看出是一个 _Proxy_.如果依旧想要使用auto, 那么可能用 _the explicitly typed initializer idiom_:<br>
```C++
auto highPriority = static_cast<bool>(features(w)[5]);
```
同样的, 在别的时候也可以使用这个特性:
```C++
double calcEpsilon();            // return tolerance value
// 如果float精度已经够了,就可以这么做
float ep = calcEpsilon();        // impliclitly convert double → float
auto ep = static_cast<float>(calcEpsilon());
```
## Chap 3: Moving to Modern C++
C++ 11/14中引入了很多新的东西, `auto`,智能指针, move语义, lambda 表达式, 并发, 接下来深入理解这里面的东西.<br>
### Item 7: 创建对象的时候分清楚 `( )` 和 `{ }`
C++中实际上有三种创建并初始化对象的方式, 使用 parenthess, 等号 或者 brace:
```C++
int x(0); 
int y = 0; 
int z{ 0 }; 
// 在很多时候, 等号和braces可以一起合作使用
int z = { 0 };        // initializer uses "=" and braces,
                      // C++ 把这种和直接使用braces相同处理
```
对于 built-in 类型(如int), 这几种只有理论上的区别, 但是对于 _user-define-type_, 他们实际上调用的是不同的构造函数:
```C++
Widget w1;	// call default constructor
Widget w2 = w1;	// not an assignment; calls copy ctor
w1 = w2;	// an assignment; calls copy operator=
```
之前的初始化依旧不够好, C++11 introduce _uniform initialization_: 
1. 可以更好的对容器指定每个元素做初始化
    ```C++
    std::vector<int> v{ 1, 3, 5 }; // v's initial content is 1, 3, 5
    ```
2. 可以用于类内非静态元素的初始化(和`=`类似, 但是不允许小括号这么做)
    ```C++
    class Widget { 
        …		
    private:		
        int x{ 0 };	//	fine, x's default value is 0
        int y = 0;	//	also fine
        int z(0);	//	error!
    };
3. 对于uncopyable的对象, 可以使用 小括号和一致初始化, 但是不能用`=`
    ```C++
    std::atomic<int> ai1{ 0 };	// fine
    std::atomic<int> ai2(0);	// fine
    std::atomic<int> ai3 = 0;	// error!
    ```

同时, 一致初始化可以防止built-in types隐式narrowing conversions, 而等号不能做此保证.<br>
过去的C++是要求 _vexing parse_:anything that can be parsed as a declaration must be interpreted as one. 而在调用默认构造函数的时候最可能出现这个问题,现在这个问题被一致初始化解决:
```C++
Widget w2();	// most vexing parse! declares a function named w2 that returns a Widget!
```
但是这个东西并不是完美的, 括号表达式实际上是一个 `std::initializer_list`,然后构造函数对它做了重载, 如果没有做`std::initializer_list`版本的构造函数, 它将和括号调用同样的构造函数, 但是重载之后行为就有所不同:
```C++
class Widget	{			
public:				
    Widget(int	i,	bool b);	//	as before
    Widget(int	i,	double d);	//	as before
    Widget(std::initializer_list<long double> il); // added
};
Widget w1(10, true);	// uses parens and, as before,  calls first ctor
Widget w2{10, true};	// uses braces, but now calls std::initializer_list ctor 
                        // (10 and true convert to long double)
Widget w3(10, 5.0);	    // uses parens and, as before, // calls second ctor
Widget w4{10, 5.0};	    // uses braces, but now calls // std::initializer_list ctor 
                        // (10 and 5.0 convert to long double)
```
这种情况下, copy / move 构造器也可能被 `std::initializer_list` 劫持, 如果Widget有operator float(), 那么下面就会出现一个诡异的行为:
```C++
class Widget	{			
public:				
    ...                     //as before
    operator float() const; // convert  to float(允许类型转换)
};
Widget w6{w4};	//	uses braces, calls
                //	std::initializer_list ctor
                //	(w4 converts to float, and float
                //	converts to long double)
Widget w8{std::move(w4)};   // use braces, calls
                            //	std::initializer_list ctor
                            //	(for	same reason as w6)
```
这时候如果**窄化类型转换之后可以匹配 `std::initializer_list`,就会报错**(如int /float　可以转成bool), 但是没有办法转换之后匹配的话, 就会吧控制权交回到其他的构造函数中. 值得注意的是,**you use an empty set of braces to construct an object that supports default construction and also supports `std::initializer_list` construction**.
```C++
Widget w1;	// calls default ctor
Widget w2{};	// also calls default ctor
Widget w3();	// most vexing parse! declares a function!
Widget w4({});	// calls std::initializer_list ctor with empty list
Widget w5{{}};	// 和上面一样
```
在使用标准库的时候, 小括号和大括号**可能有不同的意思**:
```C++
std::vector<int> v1(10, 20);	// use non-std::initializer_list
                                // ctor: create 10-element
                                // std::vector， all elements have
                                // value of 20
std::vector<int> v2{10, 20};	// use std::initializer_list ctor:
                                // create 2-element std::vector，
                                // element values are 10 and 20
```
因为这个问题, 一般的程序员默认一种分界符, 然后在只能使用另一种的时候少量用另一个. 而在 STL 的函数`std::make_unique`和 `std::make_shared` 则内部使用小括号并且在文档中详细说明这个 _interface_.

### Item 8: Prefer nullptr to 0 and NULL
在C++98中,如果一个函数三种方式重载:
```C++
void f(int); 
void f(bool); 
void f(void*);	// three overloads of f
f(0)；   // calls f(int) not f(void*)
f(NULL); // might not compile, but typically calls  f(int). Never calls f(void*)
```
如果NULL是用`0L`定义的, 那么从 long 到 int/bool/void的转换都被认为是equally good. `nullptr`的好处是它没有integral的类型, 它的类型实际上是`std::nullptr_t`,并且 **可以隐式转换成所有的raw pointer**类型.<br>
此外, 他还有一个好处是, 在搭配 _auto_ 使用的时候可以方便阅读, 没有二义性. 而当使用template的时候, 它的好处就更明显了.在之前可能需要各种代码的duplication,现在可以用模板来做:
```C++
int    f1(std::shared_ptr<Widget> spw);  // call these only when 
double f2(std::unique_ptr<Widget> upw);  // the appropriate 
bool   f3(Widget* pw);                   // mutex is locked
std::mutex f1m, f2m, f3m;         // mutexes for f1, f2, and f3 
template<typename FuncType, 
         typename MuxType,
         typename PtrType>
auto lockAndCall(FuncType func,MuxType& mutex,PtrType ptr) 
                -> decltype(func(ptr)) 
{
    using MuxGuard = std::lock_guard<std::mutex>; 
    MuxGuard g(mutex);
    return func(ptr);
}
auto result3 = lockAndCall(f3, f3m, nullptr);    // fine, 但是传入0或者NULL的时候
                                                 //编译都无法通过
```
因为 `std::nullptr_t` 可以被隐式转换成为各种指针类型, 因此使用裸指针和智能指针都可以,因此上面的过程提高了代码的复用性.

### Item9: Prefer alias declarations to typedefs
在涉及函数指针的时候, 我们发现alias声明比 _typedef_ 更加方便理解:
```C++
typedef void (*FP)(int, const std::string&);	// typedef
// same meaning as above	
using FP = void (*)(int, const std::string&);	// alias declaration
```
而alias更好的情况在涉及template的时候,它 **可以被 templatized, 但是 `typedef` 不能**,因此可以轻易写出C++98要typedefs nested inside templatized structs才可以实现的东西.
```C++
template<typename T>                           // MyAllocList<T>  is synonym for
using MyAllocList = std::list<T, MyAlloc<T>>;  // std::list<T,MyAlloc<T>>
MyAllocList<Widget> lw;                 // client code
// 如果使用typedef , 那么就要用struct hack
template<typename T>  
struct MyAllocList {
    typedef std::list<T, MyAlloc<T>> type;
};
MyAllocList<Widget>::type lw;            // client code
```
如果遇到想要在template 中用一个装有 type specified by template的list的时候, 就要在前面的基础上再加 `typename`:
```C++
template<typename T> 
class Widget {//Widget<T> contains
private:	  //a MyAllocList<T> as a data member
    typename MyAllocList<T>::type list;
    … 
};
```
因为 `MyAllocList<T>::type` 指向的类型depend on template type parameter (T), 它本身也是一个dependant tyoe, **dependent types must be preceded by `typename`.**<br>
但是如果我们使用alias实现的话, 就比这个要轻松许多:
```C++
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;  // as before

template<typename T> class Widget { 
private:
    MyAllocList<T> list;                         // no "typename",
    …                                            // no "::type"
};
```
这看上去和使用typedef一样是依赖于 template parameter T, 但是编译器处理alias的时候知道  `MyAllocList` 是 alias template: `MyAllocList<T>` 一定是在 name a type, 是 non-dependent type.在某些例子中, `MyAllocList<T>::type list` 可能是类的一个数据成员, 所以编译器要求加上`typename` 的前缀.<br>
