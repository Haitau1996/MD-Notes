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

