# 现代 C++ 语言核心特性解析<!-- omit in toc -->
## Chap 1: 新基础类型
* `long long`(since C++11)表示**至少 64位长度**的整数，过去 long 通常表示 32 bit 整型(标准要求不小于 int,clang 13/gcc 11.2 在 64 位WSL2 中实际 long 是 64bit)
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
    struct sometype {
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