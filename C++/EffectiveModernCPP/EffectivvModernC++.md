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
f(cx);                   // cx is lvalue, so T is const int&,                       
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
