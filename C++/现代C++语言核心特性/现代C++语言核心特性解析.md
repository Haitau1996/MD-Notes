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

