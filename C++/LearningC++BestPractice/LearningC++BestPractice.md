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