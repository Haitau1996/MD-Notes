# Learning C++ Best Practive Training
## Introduction
C++ 使用有不同的风格, 如传统的
```C++
size_t count_a(void *buff){
    size_t count;
    char *typed_buff;
    count = 0;
    typed_buff = (char* )buff
    for(int i =0; typed_buff[u]!= '\0'; ++i){
        if(typed_buff[i] == 'a'){
            ++count;
        }
    }
    return count;
}
```
相对现代的:
```C++
size_t count_a(const std::vector<char> &t_vec){
    size_t count{};
    for(const auto& c: t_vec){
        if(c == 'a')
            ++count;
    }
    return count;
}
```
和基于 STL 的:
```C++
size_t count_a(const std::vector<char> &t_vec){
    return std::count(t_vec.begin(), t_vec.end(),'a');
}
```
当我们从传统的风格转向 Best Practice 的时候, 往往会有下面的提升:
* 减少常见的 mistakes
* 更块地找到 errors
* 并没有性能损失, 常常能提升性能

### 使用正确的类型 & 尽可能使用 const
<div align=center><img src="https://i.imgur.com/IDZzJwd.png"/></div>

* 在上面的代码中, 实际上 size 的类型是 `vector<int>::size_type`, 这背后是 `size_t`或者 unsigned 64-bit 整型, 在内存足够大的时候, 就会限制 range 的大小或者出现溢出的问题.
* 我们在面的代码中没有修改 v 因此**应该将其声明为 `const`**
  * 使用后代码更加清晰易读
  * 编译器会强制做 const 检查

### Use the Tools
使用源代码管理系统:
* 方便 track down bugs
* 让我们可以放心重构

现代编译器可以帮助我们发现问题, 我们应该尽量将警告级别提升到最高:
* msvc: /W4(不推荐/Wall)
* clang/gcc:
  * -Wall Wextra -Wshadow -Wnon-virtual-dtor
  * -Wold-stale-cast -Wcast-align -Wunused
  * -Woverloaded-virtual-pedantic
* clang
  * 考虑 -Weverything -Wno-c++98-compat

静态检测工具:
* cppcheck
* clang
  * -format
  * -check
  * -tidy
* metrix++

运行时分析工具:
* sanitizers:
* valgrind

自动测试工具可以帮我们确定代码如预期般运行, 并且在重构时候给我们信心
* Catch2 - popular and well supported testing framework
* Google Test
* Boost.Test

持续集成工具:
* Travis CI
* Appveyor

模糊测试

### 不要做不成熟的优化
举一个简单的例子:
* 使用`std::move()` 强制百年一起执行 move 操作
* 编译器很多时候会做返回值优化 RVO, 使用 `std::move()` 之后就无法做了

## 表达式和语句
### 不要使用 C 风格的可变长参数函数
```C++
#include <stdio.h>
#include <stdarg.h>

void print_strings(int n, ...){
    va_list vl;
    va_start(vl,n);
    for(int i = 0; i < n; ++i){
        const char* val = va_arg(vl,const char* );
        printf(" %s",val);
    }
    va_end(vl);
    printf("\n");
}
int main(){
    print_strings(4, "a","b","c");
}
```
C 语言中的可变长参数函数是使用宏实现的, 在上面会引发 segmentation fault 代码中, 没有任何代码检测工具可以帮我们发现非法内存引用的错误. 替代的做法是 C++11 中引入的新语法, [Variadic Templates](../侯捷C++11/HouJieC11.md#variadic-templates-intro):
* 新特性更安全
* 可以在编译期做优化得到更好地 runtime 效率
* 可以使用 runtime sanitizers 
```C++
void print(){}
template<typename First, typename... Left>
void print(First&& f, Left&&... t)
{
  std::cout << f << "->";
  print(t...)
}
```
### 优先使用 range-based for loop
range base for-loop 只能用于遍历整个容器, 同时使用的时候应该注意在 oop 传值有对象 slicing 的问题(最好传常引用).使用传统的 for loop 可能有下面的问题:
* 可能效率更低, 如 `for(auto itr= vec.begin(), itr!= vec.end(),++itr)` 如果编译器不做优化, 每次迭代过程中都要检查 itr 是否到了尾部
* 容易写错, 如`for(auto itr= vec1.begin(), itr!= vec2.end(),++itr)`
* range-based for loop 更加灵活
    ```C++
    for(const auto& c: "hello world"){
      std::cout<< int(c) << '\n';
    }
    ```
* 我们可以使用 clnag-modernize 可以更新旧的 for loop 到 range-based for loop

### 了解标准库
* 容器
  * C++ 标准库提供了不同场景下需要的容器类型
  * 每种容器都有自己的时间和空间保证
  * 标准库经过了很多测试, 而自己实现 100% 正确的容器十分困难

* 算法
  * 对一对迭代器之间的元素做高度优化过的复合的操作
  * 理解他们对高效操作容器十分重要<div align=center><img src="https://i.imgur.com/Rxlk7q7.png"/></div>
  
### 不要依赖函数参数的求值顺序
```C++
struct Foo{
  expilcit Foo(string t_s): m_s(move(t_s)){}
  string m_s;
};
void do_something(const size_t len, Foo f){
  cout<< "Got :" << len << " "<< f.m_s <<'\n';
}
int main(){
    string s{"test"};
    do_something(s.size(),Foo(std::move(s)));
}
```
```shell
Got :0 test #GCC
Got :4 test #clang
```
从左到右求值和从右到左得到的结果完全不同, 而这个顺序在 C++ 语言标准中是未定义的.

### 尽量避免类型转换
* `dynamic_cast<T>()`
  * 可以用于在在多态中向父类/子类/兄弟类 做类型转换
  * 这种转换在编译和运行时检查
  * 使用它经常被认为是设计缺陷(可以转化为), 最好是避免使用
* `static_cast<T>()`
  * 可以被用在 explicit 和 implicit 的类型转换中
  * 可能会导致对象的 silcing
  * 没有运行时开销
  * 有时候不得不用, 但是最好还是避免使用
* `const_cast<T>()` 
  * 可用于移除 const 或者 volatile
  * 可能在调用 non-const correct API 的时候使用(如调用系统调用)
  * 除了必要情况, 应该避免使用
* `reinterpret_cast<T>()`
  * 可以将任意类型转换为几乎其他任意类型
  * 应该总是避免使用

总结:
* 我们应该尽力避免所有转换
* 可以使用 `-Wold-style-cast` 让编译器发出警告