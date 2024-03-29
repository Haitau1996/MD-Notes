# 现代 C++ 语言核心特性解析<!-- omit in toc -->
读谢丙堃所著《[现代C++语言核心特性](https://book.douban.com/subject/35602582/)》时候做的笔记
## Chap 1: 新基础类型
* `long long`(since C++11)表示**至少 64 位长度**的整数，过去 long 通常表示 32 bit 整型(标准要求不小于 int,clang 13/gcc 11.2 在 64 位WSL2 中实际 long 是 64bit)
    * 可以使用后缀`LL`, 如 `long long x1 = 65536 << 16 ` 产生的 x1 为 0 而 
`long long x2 = 65536LL << 16` 为 `0x10000000`. 
    * 在C++中应该尽量少使用宏，用模板取而代之是明智的选择，取得最值也有特化的类模板`numeric_limits<long long>::`
* `char16_t`/`char32_t` 对应两种 Unicode 字符集。
  * UTF-32 是最简单的编码方法，该方法用一个 32 位的内存空间(太占用空间)
  * UTF-16 编码方法所需的内存空间从 32 位缩小到 16 位(对不常用的字符有特殊机制)
  * 引入了前缀`u8` for UTF-8(since C++17), `u` for UTF-16, `U` for UTF-32
  * 引入的原因是 `wchar_t` 在 windows(2bytes) 和 类Unix(4bytes) 中的大小不同
  * 过去 `char*` 表示多字节字符串， 它可以和 UTF-16/32 编码字符串互转，`<cuchar>`头文件中定义了相关函数
    ```C++
    size_t mbrtoc16( char16_t* pc16, const char* s, size_t n, mbstate_t* ps );
    size_t c16rtomb( char* s, char16_t c16, mbstate_t* ps );
    size_t mbrtoc32( char32_t* pc32, const char* s, size_t n, mbstate_t* ps );
    size_t c32rtomb( char* s, char32_t c32, mbstate_t* ps );
    ```
* `char8_t` 字符类型(since C++20): char类型来处理 UTF-8 字符可能有问题， 引入后 char8_t 和 char 无法相互转换， 需要使用库函数

## Chap 2: 内联和嵌套命名空间
* C++11标准增强了命名空间的特性， **内联命名空间**可以把空间内的函数和类型导出到父命名空间中：
    ```C++
    namespace Parent {
        inline namespace Child2{//since C++11
            void foo() { std::cout << "Child2::foo()" << std::endl; }
        }
    }
    int main()
    {
        Parent::foo();
    }
    ```
* C++17标准允许使用一种更简洁的形式描述嵌套命名空间,但是依旧没有办法简洁地定义内联命名空间，这个问题直到C++20标准才得以解决。:
    ```C++
    namespace A::B::C {// since C++17
        int foo() { return 5; }
    }
    namespace A::inline B::C {//since C++20
        int foo() { return 5; }
    }
    ```

## Chap 3: `auto` 占位符
C++98 中 `auto` 用来声明自动变量，这显然是多余的， C++11标准赋予了 `auto` 新的含义：声明变量时根据初始化表达式**自动推断该变量的类型**、**声明函数时函数返回值的占位符**。使用的时候需要注意4点：
1. 声明多个变量的时候，编译器遵从由左往右的推导规则
2. 当使用条件表达式初始化auto声明的变量时，编译器总是使用表达能力更强的类型： 
    ```C++
    auto i = true ? 5 : 8.0;    // i的数据类型为double
    ```
3. `auto` 无法在类中声明非静态成员变量， C++11 中静态成员变量的声明要可以使用, 但是必须使用 `const` 限定符, 在 C++17 标准中可以在没有 `const` 限定符的情况下声明静态成员变量：
    ```C++
    struct some_type {
        static const auto i = 5;     // C++ 11
        static inline auto j = 5;    // C++17
    };
    ```
4. C++20 之前无法使用 `auto` 声明形参, C++14 中可以使用 `auto` 为 _lambda_ 表达式声明形参
   ```C++
   void echo(auto str) {…} // C++20之前编译失败，C++20编译成功
   auto cmp = [](auto& a, auto& b) { return a < b; }; // since C++14
   ```

### 推导规则
1. 如果 `auto` 声明的变量是按值初始化，则推导出的类型会忽略 `cv` 限定符: 如果吗声明变量**没有使用引用和指针**， 编译器在推导的时候会忽略 const 和 volatile 限定符。
    ```C++
    const int i = 5;
    auto j = i;        // auto推导类型为int，而非const int
    auto &m = i;       // auto推导类型为const int，m推导类型为const int&
    auto *k = i;       // auto推导类型为const int，k推导类型为const int*
    ```
2. 使用 `auto` 声明变量初始化时，目标对象如果是引用，则引用属性会被忽略
3. 使用 `auto` 和万能引用变量时， 对于左值会将 `auto` 推导为引用类型：
    ```C++
    int i = 5;
    auto&& m = i;    // auto推导类型将左值推导为int& （这里涉及引用折叠的概念）
    auto&& j = 5;    // auto推导类型为int
    ```
4. 使用 `auto` 声明变量，如果目标对象是一个数组或者函数，则 `auto` 会被推导为对应的指针类型
5. 当 `auto` 关键字与列表初始化组合时, 
   1. 直接使用列表初始化， 列表中必须为单元素， 否则无法编译
   2. 用等号加列表初始化，列表中可以包含单个或者多个元素，`auto` 类型被推导为`std::initializer_list<T>`

**何时使用 auto**, 简单理解就是两个场景比较方便：
1. 当一眼就能看出声明变量的初始化类型的时候可以使用
2. 对于复杂的类型，例如 lambda 表达式、bind 等直接使用

## Chap 4: decltype 说明符
C++ 标准提供了 typeid 云算法获取目标操作数类型相关的信息， 返回一个 `std::type_info` 对象, 可以调用其 `name()` 成员函数获取类型名称：
```C++
int x1 = 0;
std::cout << typeid(x1).name() << std::endl;
```
1. 返回值是一个左值， 且其生命周期一直被扩展到程序生命周期结束
2. `std::type_info` 删除了复制构造函数，若想保存 `std::type_info`，只能获取其引用或者指针
3. `typeid` 的返回值总是忽略类型的 cv 限定符

### 使用 decltype 说明符
C++11标准引入了 `decltype` 说明符，使用 `decltype` 说明符可以获取对象或者表达式的类型，其语法与GCC 的 typeof 类似， 讨论其优势需要用到返回类型后置的例子：
```C++
template<class T1, class T2>
auto sum(T1 a1, T2 a2)->decltype(a1 + a2)
{
  return a1 + a2;
}
auto x4 = sum(5, 10.5);
```
**相对于 auto , decltype 会同步类型的 cv 限定符**：
```C++
template<class T>
auto return_ref(T& t)
{
  return t;
}

int x1 = 0;
static_assert(
    std::is_reference_v<decltype(return_ref(x1))>// 编译错误，返回值不为引用类型
    );

template<class T>
auto return_ref(T& t)->decltype(t)
{
  return t;
}

int x1 = 0;
static_assert(
    std::is_reference_v<decltype(return_ref(x1))>    // 编译成功
    );
```

### 推导规则
`decltype(e)`（其中e的类型为T）的推导规则有5条
1. 如果 e 是一个**未加括号**的标识符表达式（结构化绑定除外）或者**未加括号**的类成员访问，则 decltype(e) 推断出的类型是e的类型T。如果并不存在这样的类型，或者e是一组重载函数，则无法进行推导。
2. 如果 e 是一个函数调用或者仿函数调用，那么 decltype(e) 推断出的类型是其返回值的类型。

下面三点其实是**加了括号**情况下的类型推导：

3. 如果 e 是一个类型为 T 的左值，则 decltype(e) 是 T&
4. 如果 e 是一个类型为 T 的将亡值，则 decltype(e) 是 T&&
5. 除去以上情况，则 decltype(e) 是 T(cppreference 网站提供的推导规则， 如果类型是 pvalue, 推导类型是 T)
    ```C++
    const int&& foo();
    int i;
    struct A {
        double x;
    };
    const A* a = new A();
    decltype(foo());         // 函数调用： decltype(foo())推导类型为const int&&
    decltype(i);             // 未加括号的表达式： decltype(i)推导类型为int
    decltype(a->x);          // 未加括号的表达式：decltype(a->x)推导类型为double
    decltype((a->x));        // 加了括号， 括号内为左值： decltype((a->x))推导类型为const double&
    ```
### cv 限定符的推导
通常情况下，decltype(e)所推导的类型会同步e的cv限定符， 当e是**未加括号的成员变量**时，**父对象**表达式的cv限定符会被忽略，不能同步到推导结果
```C++
struct A {
    double x;
};
const A* a = new A();
decltype(a->x);    // decltype(a->x)推导类型为double, const属性被忽略
decltype((a->x));    // decltype((a->x))推导类型为const double&
```

### decltype(auto)
告诉编译器用decltype的推导表达式规则来推导auto。另外需要注意的是，decltype(auto)**必须单独声明，也就是它不能结合指针、引用以及cv限定符**。  
有了这个组合之后， 可以消除返回类型后置的语法：
```C++
template<class T>
decltype(auto) return_ref(T& t)
{
  return t;
}

int x1 = 0;
static_assert(
    std::is_reference_v<decltype(return_ref(x1))>    // 编译成功
    );
```

## Chap 5: 函数返回类型后置
C++11 引入了函数返回类型后置的方法，其中 auto 是一个占位符，函数名后 -> 紧跟的类型才是真正的返回类型。在返回类型比较复杂的时候，比如返回一个函数指针类型，返回类型后置可能会是一个不错的选择：
```C++
int bar_impl(int x)
{
  return x;
}
// 使用 typedef 给函数指针类型创建别名，然后用于函数的返回类型
typedef int(*bar)(int);
bar foo1()
{
  return bar_impl;
}
// 返回类型后置， 直接写出返回类型
auto foo2()->int(*)(int)
{
  return bar_impl;
}
```
### 推导函数模板返回类型
C++11标准中函数返回类型后置的作用之一是推导函数模板的返回类型，当然前提是需要用到decltype说明符,在这种情况下decltype(t1 + t2)不能写在函数声明前，**编译器在解析返回类型的时候还没解析到参数部分**:
```C++
template<class T1, class T2>
auto sum1(T1 t1, T2 t2)->decltype(t1 + t2)
{
  return t1 + t2;
}
```

## Chap 6: 右值引用
在C++中所谓的左值一般是指一个指向特定内存的具有名称的值， 它**有一个相对稳定的内存地址，并且有一段较长的生命周期**。而右值则是不指向稳定内存地址的匿名值（不具名对象），它的**生命周期很短，通常是暂时性的**。通常判断比较简单， 在自增操作中， 前缀的是左值， 因为它在原值上递增； 而后缀的是右值， 因为它会生成值得临时复制， 然后再递增， 最后返回的是临时值。  
```C++
int *p = &x++;  // 编译失败, 无法对右值取地址
int *q = &++x;  // 编译成功
```
通常字面量都是一个右值，除字符串字面量以外， 因为编译器会将字符串字面量存储到程序的数据段中，程序加载的时候也会为其开辟内存空间。  

### 左值引用 VS 右值引用
左值引用是编程过程中的常用特性之一，它的出现让C++编程在**一定程度上脱离了危险的指针**(参考 more effective C++ item 1)。  
通常的 Best Practice 会建议我们尽可能使用了常量左值引用，但这就表示我们无法在函数内修改该对象的内容（强制类型转换(const_cast)去除常量性除外）。    
右值引用是一种**引用右值且只能引用右值的方法**。 在语法方面右值引用可以对比左值引用，在左值引用声明中，需要在类型后添加`&`，而右值引用则是在类型后添加`&&`。  
右值引用的特点之一是可以延长右值的生命周期, 更重要的作用是**减少对象复制，提升程序性能**。
我们知道右值都存放在临时对象中， 当右值被使用后， 程序会马上销毁对象并且释放内存， 这样在关闭了返回值优化或者无法做返回值优化的时候， 函数在返回时， **使用临时对象调用拷贝构造函数， 然后析构临时对象**，从正确性看这么做毫无问题， 但是**从运行性能的角度上看却还有巨大的优化空间**。  

### 移动语义
C++11标准中引入了移动语义，它可以帮助我们**将临时对象的内存移动到对象中**，以避免内存数据的复制。
```C++
class BigMemoryPool{
    ...
    BigMemoryPool(BigMemoryPool&& other)
    {
        std::cout << "move big memory pool." << std::endl;
        pool_ = other.pool_;
        other.pool_ = nullptr;
    }

    BigMemoryPool(const BigMemoryPool& other) : pool_(new char[PoolSize])
    {
        std::cout << "copy big memory pool." << std::endl;
        memcpy(pool_, other.pool_, PoolSize);
    }
private:
    char *pool_;
}
```
上面这两个构造函数的实现就有明显的区别：
* 复制构造函数而言形参是一个左值引用，也就是说函数的实参**必须是一个具名的左值**，在复制构造函数中往往**进行深复制**，即在不能破坏实参对象的前提下复制目标对象。
* 而移动构造函数恰恰相反，它接受的是一个右值，其核心思想是**通过转移实参对象的数据以达成构造目标对象的目的**，也就是说实参对象是会被修改的。

除移动构造函数能实现移动语义以外，**移动赋值运算符函数也能完成移动操作**：
```C++
BigMemoryPool& operator=(BigMemoryPool&& other)
{
    std::cout << "move(operator=) big memory pool." << std::endl;
    if (pool_ != nullptr) {
        delete[] pool_;
    }
    pool_ = other.pool_;
    other.pool_ = nullptr;
    return *this;
}
```
需要注意虽然使用移动语义在性能上有很大收益，但是却也有一些风险，这些风险来自异常，在编写移动语义的函数时建议确保函数不会抛出异常。  
### 值类别
值类别在C++11标准中新引入，它是表达式的一种属性，该[属性](https://en.cppreference.com/w/cpp/language/value_category)将表达式分为3个类别：左值（lvalue）、纯右值（prvalue）和将亡值（xvalue）。 它们之间有很复杂的关系： <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220510200744.png" width="50%"/></div>   
理解这些概念的关键在于区别泛左值、纯右值和将亡值。

// todo: 移动语义的剩下部分有点困难， 白天清醒的时候看

## Chap 7: lambda表达式
C++ lambda表达式的语法非常简单：
```C++
[ captures ] ( params ) specifiers exception -> ret { body }
```
* `[ captures ]` —— 为**捕获列表**，它可以捕获当前函数作用域的零个或多个变量，变量之间用逗号分隔。
* `( params )` —— **可选参数列表**，语法和普通函数的参数列表一样，在不需要参数的时候可以忽略参数列表。
* _specifiers_ —— **可选限定符**，C++11中可以用 `mutable`，它允许我们在lambda表达式函数体内改变按值捕获的变量，或者调用非const的成员函数
* _exception_ —— **可选异常说明符**,可以使用 `noexcept` 来指明lambda是否会抛出异常。
* _ret_ —— **可选返回类型**，可以省略，省略的情况下由编译器推导

### 捕获列表
#### 作用域
捕获列表中的变量存在于两个作用域, 即 lambda 表达式定义的函数作用域（捕获变量处）以及 lambda 表达式函数体的作用域（使用变量）。能捕获的变量必须是一个自动存储类型， 即**非静态的局部变量**。对于全局变量和静态局部变量， 我们没有捕获但依然可以使用，相应的， 如果我们将一个lambda表达式定义在全局作用域，那么lambda表达式的捕获列表必须为空(没有可供捕获的变量)。
#### 值捕获 vs 引用捕获
捕获引用的语法与捕获值只有一个 `&` 的区别,
* 按值捕获的变量默认为常量，或者说lambda是一个常量函数（类似于常量成员函数），使用mutable说明符可以移除lambda表达式的常量性
    ```C++
    // inside some function
    int x = 5, y = 8;
    auto foo = [x, y] () mutable {
        x += 1;
        y += 2;
        return x * y;
    };
    std::cout << foo() << std::endl;
    ```
    * 当 lambda 表达式捕获值时，表达式内实际获得的是捕获变量的复制，我们可以任意地修改内部捕获变量，但不会影响外部变量。 
    * 捕获值的 lambda 表达式还有一点需要注意，捕获值的变量在 lambda 表达式定义的时候已经固定下来了，无论函数在 lambda 表达式**定义后如何修改外部变量的值，lambda 表达式捕获的值都不会变化**
* 捕获引用则不同，在 lambda 表达式内修改捕获引用的变量，对应的外部变量也会被修改

#### 特殊的捕获方法
lambda 表达式的捕获列表还有三种特殊的捕获方法：
1. `[this]` 捕获 this 指针， 可以使用 this 类型的成员变量和成员函数
2. `[=]` 捕获lambda表达式定义作用域的全部变量的值，包括this。
3. `[&]` 捕获lambda表达式定义作用域的全部变量的引用, 包括 this

### lambda 表达式的实现原理
lambda表达式与函数对象（仿函数）非常相似， 这两者比较的区别在于：
1. 使用 lambda 表达式不需要我们去显式定义一个类，这一点在快速实现功能上有较大的优势。
2. 使用函数对象可以在初始化的时候有更加丰富的操作

而实际上其**实现依赖于使用编译器自动生成函数对象类**： lambda 表达式在编译期会由编译器自动生成一个闭包类，在运行时由这个闭包类产生一个对象（闭包），即一个匿名且可以包含定义时作用域上下文的函数对象。lambda 表达式是 C++11 给我们提供的一块语法糖，其功能完全能够手动实现，而且如果实现合理，代码在运行效率上也不会有差距，只不过实用lambda表达式让代码编写更加轻松了。  
C++ 标准对于无状态的 lambda 表达式有着特殊的照顾，它可以**隐式转换为函数指针**。  

### 广义捕获
C++14 标准中定义了广义捕获，包括前文提到的**简单捕获** 和 **初始化捕获**， 后者解决了简单捕获的问题， 即只能捕获 lambda 表达式定义上下文的变量，而无法捕获表达式结果以及自定义捕获变量名。  
初始化捕获在很多场景下特别实用：
1. 使用移动操作减少代码运行的开销
    ```C++
    std::string x = "hello c++ ";
    auto foo = [x = std::move(x)]{ return x + "world"; };
    ```
2. 异步调用时复制 this 对象，防止 lambda 表达式被调用时因原始 this 对象被析构造成未定义的行为
    * C++17 标准对捕获 `*this` 做了增强， 具体来说就是在捕获列表中直接添加 `[*this]`，然后在 lambda 表达式函数体内直接使用 this 指向对象的成员,[*this]的语法让程序生成了一个*this对象的副本并存储在lambda表达式内。
    * C++20标准中引入了`[=, this]` 捕获 this 指针的语法，它实际上表达的意思和`[=]`相同，目的是让程序员们区分它与`[=,*this]`的不同：
    ```C++
    class Work{
    private:
        int value;

    public:
        Work() : value(42) {}
        std::future<int> spawn()
        {
            return std::async([=, tmp = *this]() -> int { return tmp.value; });
        }
        // since C++ 17 
        std::future<int> spawn()
        {
            return std::async([=, *this]() -> int { return value; });
        }
    };
    ```

C++14 标准让 lambda 表达式具备了模版函数的能力，我们称它为泛型 lambda 表达式。  

### 模板语法的泛型 lambda 表达式
C++14 标准 lambda 表达式通过支持 auto 来实现泛型， 这让我们难以与类型互动， 对类型的操作变得异常复杂。
```C++
template <typename T> struct is_std_vector : std::false_type { };
template <typename T> struct is_std_vector<std::vector<T>> : std::true_type { };
auto f = [](auto vector) {
    static_assert(is_std_vector<decltype(vector)>::value, "");
};
```
普通函数模板可以轻松通过形参匹配一个实参为 vector 的容器对象， 但是 auto 不具备这种能力， 只能先实现 `is_std_vector` 模板类， 然后通过断言来辅助实现。  
此外这样的语法获取 vector 存储对象的类型也变得十分复杂：
```C++
auto f = [](auto vector) {
    using T = typename decltype(vector)::value_type;
    // …
};
```
此外也可能出现推导出来的类型并不是 `vector` 而是 `const vector&`,使用 STL的 decay，这样就可以将类型的cv以及引用属性删除：
```C++
auto f = [](const auto& x) {
    using T = std::decay_t<decltype(x)>;
    T copy = x;
    using Iterator = typename T::iterator;
};
```
C++委员会决定在 **C++20中添加模板对lambda的支持**，语法非常简单：
```C++
[]<typename T>(T t) {}
```
这时候上面的例子就可以改写为：
```C++
auto f = []<typename T>(std::vector<T> vector) {
// …
};
auto f = []<typename T>(T const& x) {
    T copy = x;
    using Iterator = typename T::iterator;
};
```

### 可构造和可赋值的无状态 lambda 表达式
无状态 lambda 表达式可以转换为函数指针，但遗憾的是，在 C++20 标准之前无状态的 lambda 表达式类型既不能构造也无法赋值。这时候如果比较函数对象需要通过模板参数来实现， 如：
```C++
auto greater = [](auto x, auto y) { return x > y; };
std::map<std::string, int, decltype(greater)> mymap1, mymap2;
mymap1 = mymap2;
```
decltype(greater) 获取其类型作为模板实参传入模板。这个想法非常好，但是在C++17标准中是不可行的，因为lambda表达式类型无法构造, 那么久无法去做 decltype。 同样的无状态的 lambda 表达式无法赋值，上面第三行的也无法实现。 **C++20标准允许了无状态 lambda表达式类型的构造和赋值**，所以使用C++20标准的编译环境来编译上面的代码是可行的。 

## Chap 8: 非静态数据成员默认初始化
在C++11以前，对非静态数据成员初始化需要用到初始化列表，当类的数据成员和构造函数较多时，编写构造函数会是一个令人头痛的问题。C++11标准提出了新的初始化方法，即在**声明非静态数据成员的同时直接对其使用=或者{}初始化**， 使用的时候要注意两个问题：
1. 不要使用括号()对非静态数据成员进行初始化，因为这样会造成解析问题，所以会编译错误。
2. 不要用 auto 来声明和初始化非静态数据成员，虽然这一点看起来合理，但是 C++ 并不允许这么做。

在C++20中我们可以对数据成员的位域进行默认初始化。

## Chap 9: 列表初始化
我们称使用括号初始化的方式叫作直接初始化，而使用等号初始化的方式叫作拷贝初始化（复制初始化）。在其他时候，如new运算符和类构造函数的初始化列表就属于直接初始化，而函数传参和return返回则是拷贝初始化。 C++11 标准引入了列表初始化，它使用大括号{}对变量进行初始化，和传统变量初始化的规则一样，它也区分为直接初始化和拷贝初始化。
```C++
struct C {
    C(std::string a, int b) {}
    C(int a) {}
};
C bar()
{
  return {"world", 5};
}
// 在调用端
C x2 = {4};          // 拷贝初始化
C x3{2};             // 直接初始化
C x4 = bar();        // 拷贝初始化
C *x5 = new C{ "hi", 42 };  // 直接初始化
```
列表初始化在这里的奥妙所在，它支持**隐式调用多参数的构造函数**，有时候我们并不希望编译器进行隐式构造，这时候只需要在特定构造函数上声明 explicit 即可。  

### `initializer_list` 详解
标准容器之所以能够支持列表初始化， 它们自己也必须满足一个条件，支持 `std::initializer_list` 为形参的构造函数, 简单说初始化列表就是一个支持 begin、end 以及 size 成员函数的类模板。  
实际上编译器对于初始化列表的处理大概是用数组， [示例代码](code/9-1.cpp) 分析初始化列表中各个对象的地址， 其差值刚好是对象的大小：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220503001552.png" width="50%"/></div>

推断编译器做的工作大概如下， 生成中间代码也验证了推断：
```C++
const std::string __a[3] = 
  {std::string{"hello"}, std::string{"c++"}, std::string{"world"}};
C c(std::initializer_list<std::string>(__a, __a+3));
// GIMPLE
    struct initializer_list D.40094;
    const struct basic_string D.36430[3];
    …
    std::__cxx11::basic_string<char>::basic_string (&D.36430[0], "hello", &D.36424);
    …
    std::__cxx11::basic_string<char>::basic_string (&D.36430[1], "c++", &D.36426);
    …
    std::__cxx11::basic_string<char>::basic_string (&D.36430[2], "world", &D.36428);
    …
```

### 使用列表初始化注意事项
使用列表初始化要注意两个问题：
1. 隐式缩窄转换问题： C++ 中的隐式类型窄化经常出现， 而且不一定会引发错误，甚至有可能连警告都没有， C++ 标准中有四条规则：
   1. 从浮点类型转换整数类型
   2. 从long double转换到 double 或 float，或从 double 转换到 float，除非转换源是常量表达式以及转换后的实际值在目标可以表示的值范围内
   3. 从整数类型或非强枚举类型转换到浮点类型，除非转换源是常量表达式，转换后的实际值适合目标类型并且能够将生成目标类型的目标值转换回原始类型的原始值。
   4. 从整数类型或非强枚举类型转换到不能代表所有原始类型值的整数类型，除非源是一个常量表达式，其值在转换之后能够适合目标类型。
2. 列表初始化的优先级问题： 如果有一个类同时拥有满足列表初始化的构造函数，且其中一个是以 std::initializer_list 为参数，那么编译器将**优先以 std::initializer_list 为参数构造函数**。

### 指定初始化
C++20标准中引入了指定初始化的特性。该特性允许**指定初始化数据成员的名称**，从而使代码意图更加明确。  
1. 它要求对象必须是一个聚合类型，例如下面的结构体就无法使用指定初始化
2. 指定的数据成员必须是非静态数据成员。
3. 每个非静态数据成员最多只能初始化一次
4. 非静态数据成员的初始化必须按照声明的顺序进行
5. 针对**联合体**(union) 中的数据成员只能初始化一次，不能同时指定
6. 不能嵌套指定初始化数据成员
7. 在C++20中，一旦使用指定初始化，就不能混用其他方法对数据成员初始化了
8. 指定初始化在C语言中可以处理数组，当然在C++中这同样是被禁止的
    ```C++
    int arr[3] = { [1] = 5 };    // C++编译失败, C 编译成功， 显示它是 C99 extension
    ```

## Chap 10: 默认和删除函数
在没有自定义构造函数的情况下， 编译器会添加默认的构造函数， 这样的特殊成员函数有 6 个：
1. 默认构造函数
2. 析构函数
3. 拷贝构造函数
4. 拷贝赋值运算符
5. 移动构造函数(since C++11)
6.  移动赋值运算符(since C++11)

这些工作可以交给编译器完成， 但是也带来了一些麻烦
1. 声明任何构造函数都会抑制默认构造函数的添加
2. 一旦用自定义构造函数代替默认构造函数，类就将转变为非平凡类型
3. 没有明确的办法彻底禁止特殊成员函数的生成

### 显式默认和显式删除
C++11标准提供了一种方法能够简单有效又精确地控制默认特殊成员函数的添加和删除，只需要在声明函数的尾部添加`=default`和`=delete`, `=default`可以添加到类内部函数声明，也可以添加到类外部,`=delete`必须添加在类内部的函数声明。
* 使用 =default 可以解决上面提到的前两个问题  
* 相对于使用 private 限制函数访问，使用 `=delete` 更加彻底，它从编译层面上抑制了函数的生成
  * 编译器提供更好的报错能力
* 使用 `=delete` 更方便禁止重载函数的某些版本

### 显式删除的其他用法
显式删除**不仅适用于类的成员函数，对于普通函数同样有效**。
```C++
void foo() = delete;
static void bar() = delete;
int main(){
  bar();        // 编译失败，函数已经被显式删除
  foo();        // 编译失败，函数已经被显式删除
}
```
显式删除还可以用于类的new运算符和类析构函数。
* 显式删除特定类的new运算符可以[阻止该类在堆上动态创建对象](code/10-1.cpp)
* 显式删除类的析构函数在某种程度上和删除 new 运算符的目的正好相反，它阻止类通过自动变量、静态变量或者全局变量的方式创建对象，但是却可以[通过 new 运算符创建对象](code/10-2.cxx)

在类的构造函数上同时使用 explicit 和 =delete 是一个不明智的做法，它常常会造成代码行为混乱难以理解，应尽量避免这样做。

## Chap 11: 非受限联合类型
C++中的联合类型（union）是节约内存的一个典型代表，联合类型多种类型可以共享一片内存， 这篇内存也只能由一个对象使用，如[例子](code/11-1.cxx)中修改一个值， 另一个也会相应变化：
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220503191614.png" width="40%"/></div>  

过去的C++标准规定，联合类型的成员变量的类型不能是一个非平凡类型，也就是说它的成员类型不能有自定义构造函数， 而这是与 C++ 理念相背：
> 当面对一个可能被滥用的功能时，语言的设计者往往有两条路可走，一是为了语言的安全性禁止此功能，另外则是**为了语言的能力和灵活性允许这个功能**,C++的设计者一般会采用后者。但是联合类型的设计却与这一理念背道而驰。这种限制完全没有必要，去除它可以让联合类型更加实用。

在C++11标准中解除了大部分限制，联合类型的**成员可以是除了引用类型外的所有类型**。在这个时候， 让编译器去选择初始化本身也是不合适的，这个事情应该交给程序员来做，也就是说我们必须自己至少提供联合类型的构造和析构函数。  
如果我们不提供或者提供的构造和析构函数什么也没有做， [代码](code/11-2.cxx) 可以通过编译，但是行为是未定义的：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220503192701.png" width="60%"/></div>

我们当然[可以](code/11-3.cxx)在联合体的构造函数和析构函数中提供非平凡类型的构造和析构函数， 但是我们无法知道实际中激活的是哪个成员,我还是比较[推荐](code/11-4.cxx)让联合类型的构造和析构函数为空，也就是什么也不做，并且**将其成员的构造和析构函数放在需要使用联合类型的地方**, 推荐的实现中用了 placement new 的技巧来初始化构造x3和x4对象，在使用完对象后手动调用对象的析构函数。通过这样的方法保证了联合类型使用的灵活性和正确性。  
非受限联合类型对静态成员变量不属于任何对象， 所以不是对象构造时候定义， 不能在联合类型内部初始化，特性和初始化方法都类似于类的静态成员变量。

## Chap 12: 委托构造函数
一个类有多个不同的构造函数在C++中是很常见的， 过去有人会将初始化放到 [`commonInit` 函数](code/12-1.cxx)中从而减轻代码冗余问题, 对于基本类型的成员变量问题不大， 但是如果成员函数中包含复杂的对象，那么就可能引发不确定问题。
1. 影响类的构造效率，如[例](code/12-2.cxx) 中 string 类型并不是初始化而是赋值过程， 这样的话 string 成员做了两次操作， 一次为初始化， 一次为赋值， 带来额外的开销
2. 如果构造函数提供默认参数， 稍不注意就会引发[二义性问题](code/12-3.cxx)

这两个问题就出于无法让一个构造函数将初始化的一部分工作委托给同类型的另外一个构造函数。
### 委托构造函数
C++11标准支持了委托构造函数：**某个类型的一个构造函数可以委托同类型的另一个构造函数对对象进行初始化**， 其语法[十分简单](code/12-4.cxx), 但是要注意几个问题：
1. 每个构造函数(包括其他构造函数的目标构造函数）都可以委托另一个构造函数为代理
2. 不要递归循环委托：编译器可能不报错但是程序运行结构未定义
3. 如果一个构造函数为委托构造函数，那么其初始化列表里就[不能对数据成员和基类进行初始化](code/12-5.cxx)
4. 委托构造函数的执行顺序是先执行代理构造函数的初始化列表，然后执行代理构造函数的主体，最后执行委托构造函数的主体
5. 如果在代理构造函数执行完成后，委托构造函数主体抛出了异常，则自动调用该类型的析构函数。

代理构造函数可以是一个函数模板， 称为委托模板构造函数，指一个构造函数将控制权委托到同类型的一个模板构造函数。  

## Chap 13: 继承构造函数
[很多时候](code/13-1.cxx)， 派生类本身并没有需要初始化的数据成员，面对Base中大量的构造函数，我们不得不在Derived中定义同样多的构造函数，目的仅仅是转发构造参数。  
C++ 中可以使用 using 关键字将基类的函数引入派生类, C++11的**继承构造函数**正是利用了这一点，**将 using 关键字的能力进行了扩展，使其能够引入基类的[构造函数](code/13-2.cxx)**, 具体而言就是 Derived 使用 using Base::Base 让编译器为自己生成转发到基类的构造函数, 使用的时候有几点需要注意：
1. 派生类是隐式继承基类的构造函数，所以只有在程序中使用了这些构造函数，编译器才会为派生类生成继承构造函数的代码。//todo: 其他需要注意的点

## Chap 14： 强枚举类型
我们说C++是一门类型安全的强类型语言，但是枚举类型在一定程度上却是一个例外：枚举类型存在一定的安全检查功能,但却**可以隐式转换为整型**，这让不同的枚举类型[可以相互比较](code/14-1.cxx), 原因是枚举类型先被隐式转换为整型，然后才进行比较。  
然后是枚举类型的作用域问题，枚举类型会把其内部的枚举标识符导出到枚举被定义的作用域， 这样的话如果有同名的定义， 就会发生重复无法通过编译(可以使用命名空间解决)。 对上述问题的解决， 有个[不完美的方案](code/14-2.cxx)是采用类封装然后重载操作符。  
枚举类型本身是一个 POD(Plain Old Data) 类型，上面的实现破坏了这种特性， 并且要敲很多代码，更严重的问题就是**无法指定枚举类型的底层类型**。枚举类型缺乏类型检查的问题倒是成就了一种特殊用法，被称为[enum hack](code/14-3.cxx)。  

### 使用强枚举类型
因此 C++标准委员会在 C++11 标准中对其做出了重大升级，增加了强枚举类型。
1. 枚举标识符属于强枚举类型的作用域。
2. 枚举标识符不会隐式转换为整型。
3. 能指定强枚举类型的底层类型，底层类型默认为int类型

使用非常简单， 只要在 enum 关键字后添加 class 关键字。从[实例](code/14-4.cxx)中可以发现， 上面提到的问题得到了解决：
1. 在不使用命名空间的情况下，两个有着相同枚举标识符的强枚举类型**可以在一个作用域内共存**。
2. 枚举标识符属于强枚举类型的作用域，无法从外部直接访问它们，所以在**访问时必须加上枚举类型名**
3. 不同枚举类型就无法比较其枚举标识符了，因为它们**失去了隐式转换为整型的能力**
4. 我们可以在定义类型的时候[使用:符号来指明其底层类型](code/14-5.cxx)，利用它可以消除不同编译器带来的歧义(在C++11标准中，我们除了能指定强枚举类型的底层类型，还**可以指定普通枚举类型的底层类型**)。

### 后续标准对枚举的增强
从C++17标准开始，对有底层类型的枚举类型对象可以[直接使用列表初始化](code/14-6.cxx)。需要注意的是，`Color2 c2 = Color2{ 5 }`和 `Color c2 = { 5 }`含义是完全不同的:前者先通过列表初始化了一个临时对象， 然后赋值， 而后者没有这个过程。  
此外还要注意， 没有指定底层类型的枚举类型是无法使用列表初始化的， 上面的例子中的 Color 有默认的底层类型 int。  
C++20标准扩展了using功能，它[**可以打开强枚举类型的命名空间**](code/14-7.cxx)。

// todo: chap 15

## Chap 16: override 和 final 说明符
重写（override）、重载（overload）和隐藏（overwrite）在C++中是3个完全不同的概念:
1. 重写（override）的意思更接近覆盖, 在这里是指**派生类覆盖了基类的虚函数**， 两者必须要有相同的函数签名和兼容的返回类型
2. 重载（overload），它通常是指有两个或者两个以上函数，它们**函数名相同，但是函数签名不同**
3. 隐藏（overwrite）是指基类成员函数，无论它是否为虚函数，**当派生类出现同名函数时，如果派生类函数签名不同于基类函数，则基类函数会被隐藏**，如果派生类函数签名与基类函数相同，则需要确定基类函数是否为虚函数，如果是虚函数，则这里的概念就是重写；否则**基类函数也会被隐藏**。

### Override 说明符
C++语法对重写的要求很高，稍不注意就会无法重写基类虚函数，即使我们[写错了代码](code/16-1.cxx)，编译器也可能不会提示任何错误信息。光靠人力排查避免出错是很困难的,所以 C++11 标准提供了一个非常实用的 override 说明符, **明确告诉编译器这个虚函数需要覆盖基类的虚函数**，一旦编译器发现该虚函数[不符合重写规则](code/16-2.cxx)，就会给出错误提示。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220506161721.png" width="70%"/></div>  

### final 说明符
但是一直以来，C++标准并没有提供一种方法来阻止派生类去继承基类的虚函数。C++11标准引入 final 说明符解决了上述问题，它**告诉编译器该虚函数不能被派生类重写**。final说明符不仅能声明虚函数，还可以声明类。如果在类定义的时候声明了final，那么这个类将不能作为基类被其他类继承。  
需要注意的是， C++11 标准中这两个**没有作为保留关键字**（与过去代码兼容），只在合适的位置才有意思， 其他地方可以正常使用。
```C++
class X {
public:
    void override() {}
    void final() {}
};
```

## Chap 17: 基于范围的 for 循环
有时候为了遍历容器内容要写很多关于迭代器的代码， 过去可以使用标准库的 `std::for_each` 函数，相当于单纯的迭代器遍历操作交给编译器来完成， C++11标准引入了基于范围的 for 循环特性。
```C++
for ( range_declaration : range_expression ) loop_statement
```
范围表达式可以是数组或对象，对象必须满足以下2个条件之一：
1. 对象类型定义了 begin 和 end 成员函数
2. 定义了以对象类型为参数的 begin 和 end 普通函数

通常为了**避免数据复制， 在循环中使用引用， 如果不会修改引用对象， 加上 const 限定符以生成更高效的代码**。  
**begin 和 end 函数不必返回相同类型**:C++ 11 标准中基于范围的 for loop 基于下面的伪代码：
```C++
auto && __range = range_expression;
  for (auto __begin = begin_expr, __end = end_expr; __begin != __end; ++__begin) {
       range_declaration = *__begin;
       loop_statement
  }
```
这里 begin 和 end 使用同一个 auto , 要求两者返回同一个类型， **这种约束是完全没有必要的**， 只要 `__begin != __end` 能返回一个 bool 类型就行， C++17 标准进行了改进：
```C++
auto && __range = range_expression;
auto __begin = begin_expr;
auto __end = end_expr;
for (; __begin != __end; ++__begin) {
    range_declaration = *__begin;
    loop_statement
}
```
这将 begin 和 end 分离到不同的语句， 不再要求它们是相同类型。  
需要注意的是这两者都是从一个 `auto &&` 开始的， 如果 range_expression 是一个纯右值， 那么右值引用会拓展其生命周期， 如果它是一个泛左值， 结果就可能不确定。[例](code/17-1.cxx) 中使用一个 `std::vectorMint>&`,右值引用无法扩展其生命周期, 导致循环访问一个无效的对象， 一个解决的办法是将数据复制出来， 而 C++20 提供了对基于范围的 for 循环初始化语句的支持：
```C++
for (T thing = foo(); auto & x :thing.items()) {}
```

## Chap 18: 支持初始化语句的 if 和 switch
if 控制结构可以在执行条件语句之前先执行一个初始化语句:
```C++
if (init; condition) {}
```
* if 初始化语句中声明的变量拥有和整个 if 结构一样长的声明周期
* 在 else if 条件语句之前也可以使用初始化语句
  * 在 if 的变量生命周期会贯穿整个if结构（包括else if）
  * else if 中初始化的变量，生命周期只存在于 else if 以及后续存在的 else if 和 else 语句

switch 在通过条件判断确定执行的代码分支之前也可以接受一个初始化语句，switch初始化语句声明的变量的生命周期会贯穿整个switch结构。  
因为在 if 和 switch 中初始化的变量可以贯穿整个结构， 所以可以利用该特性对整个结构[加锁](code/18-1.cxx)。
## Chap 19: static_assert 声明
在静态断言出现以前，我们使用的是运行时断言，只有**程序运行起来之后才有可能触发它**。断言不能代替程序中的错误检查,只应该出现在需要表达式返回true的位置：
```C++
assert(buffer != nullptr);           // OK，用assert检查函数参数
bool get_user_input(char c){
assert(c == '\0x0d');                // 不合适，assert不应该用于检查外部输入
```
静态断言（static_assert 声明）是 C++11 标准引入的特性，用于在程序**编译阶段评估常量表达式并对返回false的表达式断言**：
1. 所有处理必须在编译期间执行，不允许有空间或时间上的运行时成本
2. 断言失败可以显示丰富的错误诊断信息
3. 它可以在命名空间、类或代码块内使用
4. 失败的断言会在编译阶段报错

这个语句要传入两个实参：**常量表达式**和诊断消息字符串。[只允许常量表达式](code/19-1.cxx)是因为编译器无法计算运行时才能确定结果的表达式。  
在支持C++17标准的环境中，我们可以[忽略第二个参数](code/19-2.cxx)。

## Chap 20: 结构化绑定
Python 的函数可以有多个返回值， 而 C++11 中[实现类似的功能](code/20-1.cxx)十分麻烦要先将函数返回值声明为 tuple, 然后用函数末班将引用绑定到 tuple 上。  
C++14 引入了返回值自动推导， C++17 中引入了结构化绑定：将一个或者多个名称绑定到初始化对象中的一个或者多个子对象（或者元素）上, 而且目标不必是函数返回的结果， [比如](code/20-2.cxx)可以用到 for 循环中。  
### 深入理解结构化绑定
在结构化绑定中编译器会根据限定符生成一个等号右边对象的匿名副本，而绑定的对象正**是这个副本而非原对象本身**。而且变量也是匿名对象成员的别名， 而不是对象的引用。[测试代码](code/20-3.cxx)执行结果表明了变量的类型和引用对象的地址：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220510121827.png" width="60%"/></div>

强 const auto 改为 auto&, 就可以实现绑定的数据成员和变量相互修改：
```C++
auto&[x, y] = bt;
// 相当于
BindTest bt;
auto &_anonymous = bt;
aliasname x = _anonymous.a
aliasname y = _anonymous.b
```

### 结构化绑定的三种类型
结构化绑定可以作用于3种类型： 原生数组、结构体和类对象、元组和类元组的对象。
1. 绑定到原生数组： 需要小心数组的退化，因为在绑定的过程中**编译器必须知道原生数组的元素个数**，退化将失去这个特性
2. 绑定到结构体和类对象, [限制条件](code/20-4.cxx)复杂得多：
   1. 类或者结构体中的非静态数据成员个数必须和标识符列表中的别名的个数相同
   2. 这些数据成员必须是公有的
      * C++20标准规定结构化绑定的限制不再强调必须为公开数据成员，编译器会根据当前操作的上下文来判断是否允许结构化绑定。
   3. 这些数据成员必须是在同一个类或者基类中
   4. 绑定的类和结构体中不能存在匿名联合体
3. 绑定元组合类元组的对象
   1. `std::tuple_size<T>::value` 符合语法并且结果和别名个数相同
   2. `std::tuple_element<i, T>::type` 符合语法并且代表 T 中第 i 个元素的类型
   3. 必须存在合法的成员函数模板 `get<i>()`或者函数模板`get<i>(t)`

## Chap 21: noexcept 关键字
过去使用 throw 声明是否抛出异常一直没什么问题， 直到 C++11 引入了移动构造函数：将一个容器的元素移动到另外一个新的容器中时，如果没有移动语义，我们只能将原始容器的数据复制到新容器中。如果在**数据复制的过程中复制构造函数发生了异常，那么我们可以丢弃新的容器，保留原始的容器**。有了移动语义，原始容器的**数据会逐一地移动到新容器中，如果数据移动的途中发生异常，那么原始容器也将无法继续使用**，我们无法保证恢复的过程中不会抛出异常。  
C++ 标准委员会提供了 `noexcept` 说明符，
* [作为说明符](code/21-1.cxx)，它能够用来说明函数是否会抛出异常
* noexcept 还能接受一个返回布尔的[常量表达式](code/21-2.cxx)，当表达式评估为true的时候，其行为和不带参数一样，表示函数不会抛出异常，反之则可能抛出异常
  * 由于 noexcept 对表达式的评估**是在编译阶段执行的，因此表达式必须是一个常量表达式**
* noexcept 还可以作为[运算符](code/21-3.cxx)，接受表达式参数并返回true或false， 表达式的结果取决于编译器是否在表达式中找到潜在异常

### 用 noexcept 来解决移动构造问题
这时候我们根据 noexcept 运算符的返回结果， 如果没有抛出异常的可能，那么函数可以选择进行移动操作；否则将使用传统的复制操作。  
[第一个版本](code/21-swap_1.cxx)的 swap 函数代码做两件事:
1. 检查类型 T 的移动构造和移动赋值函数会不会抛出异常
2. 通过移动构造和移动赋值函数来移动对象 a 和 b

[新版本](code/21-swap_2.cxx)加入静态断言， 如果移动构造和移动赋值会抛出异常， 那么函数就会编译失败。但是这个版本过于强势， 我们希望在不满足移动需求的时候可以选择赋值方法完成移动操作。  
[最终版本](code/21-swap_final.cxx)中有两个版本的swap_impl，两者只有第三个形参类型不同。swap函数会调用 swap_impl，并且以移动构造函数和移动赋值函数是否会抛出异常为模板实参来实例化 swap_impl 的第三个参数。其中的 `noexcept(T(std::move(a))) && noexcept(a.operator=(std:: move(b)))` 是判断移动构造和移动赋值过程中会不会发生异常， 可以使用 `std::is_nothrow_move_constructible <T>::value && std::is_nothrow_move_ assignable<T>::value` 来代替。

### 对比 throw() 和 noexcept
这两种指明不抛出异常的方法在外在行为上是一样的。如果用 noexcept 运算符去探测 noexcept 和throw() 声明的函数，会返回相同的结果。
* C++11 中， 声明了 `noexcept`的基础上抛出了异常，那么程序将不需要展开堆栈， 不会调用`std::unexpected`，而是调用`std::terminate` 终止程序。 而 `throw()` 则需要展开堆栈并且调用 `std::unexpected`。
* 在C++17标准中，`throw()` 成为 `noexcept` 的一个别名
* C++20中 `throw()` 被标准移除了

### 默认 noexcept 的函数
C++11标准规定下面几种函数会默认带有noexcept声明：
1. 默认构造函数、默认复制构造函数、默认赋值函数、默认移动构造函数和默认移动赋值函数。 有额外的要求：对应的函数在类型的基类和成员中也具有 noexcept 声明，自定义实现的函数默认也不会带有noexcept声明。 
2. 类型的析构函数以及 delete 运算符默认带有 noexcept 声明，请注意即使自定义实现的析构函数也会默认带有noexcept声明，除非类型本身或者其基类和成员明确使用 noexcept(false) 声明析构函数，以上也同样适用于 delete 运算符

**使用 noexcept 的时机**：总结了两种情况
1. 一定不会出现异常的函数。通常情况下，这种函数非常简短，如求整数的绝对值
2. 当我们的目标是提供不会失败或者不会抛出异常的函数时可以使用noexcept声明。 这种时候相比异常捕获， 终止程序是更好的选择

### 将异常规范作为类型的一部分
在C++17标准之前，异常规范没有作为类型系统的一部分，所以下面的代码在编译剪短不会有问题：
```C++
void(*fp)() noexcept = nullptr;
void foo() {}

int main()
{
  fp = &foo;
}
```
C++17标准将异常规范引入了类型系统。这样一来，`fp = &foo` 就无法通过编译了，因为 fp 和 &foo变成了不同的类型。虽然 noexcept 声明的函数指针无法接受没有 noexcept 声明的函数，但是反过来却是被允许的，[虚函数的重写](code/21-4.cxx)也遵守这个规则:这个设定可以保证现有代码的兼容性,在语义上也是可以接受的，因为函数指针既没有保证会抛出异常，也没有保证不会抛出异常，所以接受一个保证不会抛出异常的函数也合情合理。

## Chap 22: 类型别名和别名模板
在过去， 使用 typedef 和定义变量一样定义一个别名：
```C++
typedef std::map<int, std::string>::const_iterator map_const_iter;
map_const_iter iter;
```
C++11 中提供了使用 using 关键字的新的类型别名定义方法：`using identifier = type-id`。 这很像是一个赋值表达式并且更符合直觉。  
使用typedef定义函数类型别名和定义其他类型别名是有所区别的，而使用using则不存在这种区别：
```C++
typedef void(*func1)(int, int);
using func2 = void(*)(int, int);
```

### 别名模板
定义别名模板的语法和定义类型别名并没有太大差异，只是多了模板形参列表：
```C++
template < template-parameter-list >
using identifier = type-id;
```
很多时候使用 typedef 可以做到类似的事情， 但是在有带决定的类型， 还需要在变量声明前加上 typename 关键字：
```C++
template<class T>
struct int_map {
    typedef std::map<int, T> type;
};

template<class T>
struct X {
    typename int_map<T>::type int2other;  // 必须带有typename关键字，否则编译错误
};
```
类模板X没有确定模板形参T的类型, `int_map<T>::type` 是一个未决类型:
* 它可能是一个类型
* **也可能是静态成员变量**

名模板不会有`::type`的困扰，当然也不会有这样的问题:
```C++
template<class T>
using int_map = std::map<int, T>;

template<class T>
struct X {
    int_map<T> int2other;      // 编译成功，别名模板不会有任何问题
};
```
## Chap 23: 指针字面量 nullptr
C++标准中， 0既是一个整型常量，又是一个空指针常量。0作为空指针常量还能隐式地转换为各种指针类型。  
```C++
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif
```
也就是说， C++将NULL定义为0，而C语言将NULL定义为 `(void *)0`。这个时候， 隐式类型转换就可能带来[二义性](code/23-1.cxx)等未预期的问题。  
C++标准委员会在C++11中添加关键字 nullptr 表示空指针的字面量，它是一个 std::nullptr_t 类型的纯右值：
* 不允许运用在算术表达式中或者与非指针类型进行比较（除了空指针常量0）
* 可以隐式转换为各种指针类型，但是无法隐式转换到非指针类型

nullptr 的类型 std::nullptr_t，它并不是一个关键字，而是使用 decltype 将 nullptr 的类型定义在代码中：
```C++
namespace std
{
    using nullptr_t = decltype(nullptr);
    // 等价于
    typedef decltype(nullptr) nullptr_t;
}
static_assert(sizeof(std::nullptr_t) == sizeof(void *));
```

还可以使用 std::nullptr_t 去创建自己的 nullptr，并且有与 nullptr 相同的功能, 并且这些新的变量时左值，可以取地址， nullptr 是纯右值， 和对字面量常数取地址一样死没有意义的。  
有了 nullptr 之后还有一个好处，在函数模板中可以设计一些空指针类型的[特化版本](code/23-2.cxx)。

## Chap 24: 三向比较符
C++20 标准新引入了一个名为“太空飞船”（spaceship）的运算符`<=>`,两个比较的操作数lhs和rhs通过<=>比较可能产生3种结果，该结果可以和0比较，小于0、等于0或者大于0分别对应lhs < rhs、lhs == rhs 和 lhs > rhs。  
运算符`<=>`的返回值只能与0和自身类型来比较, 因为返回值不是普通类型而是三种之一，分别为`std::strong_ordering`、`std::weak_ordering`以及`std:: partial_ordering`。  
* strong_ordering 类型有三种比较结果：`std::strong_ ordering::less`、`std::strong_ordering::equal`以及`std::strong_ ordering::greater`, 结果强调的是strong的含义，表达的是一种**可替换性**，简单来说，若lhs == rhs，那么在任何情况下rhs和lhs都可以相互替换
* weak_ordering 同样有三种结果， 含义正好与 `std::strong_ordering`相对，表达的是**不可替换性**
* partial_ordering 类型有四种比较结果， 分别为`std::partial_ordering::less`、`std::partial_ordering::equivalent`、`std::partial_ordering::greater`以及`std::partial_ordering::unordered`, 它的约束力比 weak_ordering 更弱， 除了其他三个结果中的不能相互替换外， 第四个加过表示**两个操作数没有关系**： 如浮点类型， 除了正常的的浮点数， 还存在 `NaN`:
    ```C++
    std::cout << ((0.0 / 0.0 <=> 1.0) == std::partial_ordering::unordered);
    ```

//todo: 对类型的支持好麻烦， 跳过一下

## Chap 26: 扩展的 inline 说明符
C++17 之前定义非常量的静态类型成员变量十分麻烦， 因为声明和定义必须分开进行， 比较常见的错误是为了方便讲静态成员变量的定义放在[头文件](code/26-1.hpp)中, 如果将这部分代码包含到多个 CPP 文件中， 因为 include 是单纯的宏替换， 就会存在因为多份 X::text 的定义导致链接失败。  
对于字面量类型，如整型和浮点类型， 静态成员可以在声明的时候定义：
```C++
class X {
public:
    static const int num{ 5 };
};
```
C++ 17 标准中允许我们[内联定义静态变量](code/26-2.cxx)， 编译器会在类 X 的定义首次出现时对内联静态成员变量进行定义和初始化。