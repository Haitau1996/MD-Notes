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
