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
* 使用`std::move()` 强制编译器执行 move 操作
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
* 我们可以使用 clang-modernize 可以更新旧的 for loop 到 range-based for loop

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

### 变量作用域和初始化
这个可以简化为一句话: 直到不得不使用该变量时, 才声明变量.
* never create a variable until you can initialize it
* 尽可能地去限制变量的作用域
* 尽可能使用 const
* 有一些静态分析工具可以帮助我们缩小变量的 scope

### 正确初始化成员变量
* 使用成员初始化列表
  * 它在构造器之前/基类构造器 之后被调用
    ```C++
    ClassName(const int i, std::string t_s)
    : BaseClass(i),
      m_s(std::move(t_s))
    {}
    ```
  * 数据成员的初始化顺序和它们的声明顺序一样(不是初始化列表中的顺序)
  * 成员初始化列表是初始化 const 数据成员的唯一方法
  * clang/gcc 在初始化顺序和声明不同的时候会报错

### 使用 lambda 表达式做复杂的初始化
例如我们根据需要给一个 const string 赋值的时候, 如果先声明之后赋值, 就无法保持 constness. 我们可以将选择的语句放在函数中, 根据参数返回不同的 string, 也可以使用 `? :` 构成的三元表达式, 但是在分支很多的时候语句会非常复杂, 而一个比较好的实践是使用 lambda:
```C++
const std::string s1 = [&](){
  switch(argc){
    case 0: return "0 args";
    case 1: return "1 arg";
    default: return "many args";
  }
}();
```
总结:
* lambda 表达式可以用于简化复杂的初始化过程
* 这有助于帮助我们创建更多的 const-correct 代码
* 这有助于减少对象的创建/赋值, 帮助提升代码性能

## 资源管理
### 在接口中, 只使用指向单个(optional,可选)对象的裸指针
在 core guideline 中指出, 接口中的指针应该只用于指向单个对象, 下面就出现了buffer overflow:
```C++
void print_ints(const int *s, int count){
  for(int i{0}; i < count; ++i){
    std::cout<< s[i] << "\n";
  }
}
...
const int ints[]{1,2,3,4,5};
print_ints(ints,6);
```
在这种情况下, 我们可以使用一个容器,如 vector.  
进一步, 我们要求裸指针只能指向单个并且 optional 对象(可能为 NULL), 因为**非 optional 的情况我们可以使用引用**:
```C++
void use_string(const std::string *pts){
  if(pts){
    // do something
  }
}
int main(){
  std::string s("hello");
  use_string(&s);
}
```
总结:
* 要求接口中的裸指针只能指向单个并且 optional 对象
* Guideline Library 中有一个 span 类, 提供开始/结尾指针可以和过去的接口兼容
* 对于 optional 参数考虑使用 optional
* 使用 C++ Core Checker 帮助检查代码

### 使用 RAII 自动管理对象
RAII: Resource Acquisition Is Initialization. 在这种思想下, 我们创建一个对象相当于获取某种资源, 在对象失效后它会自我清理, 这充分利用的 C++ 中的对象生命周期/析构函数 和 stack 管理机制. 
* 使用构造器-析构器 对, 确保任何分配/初始化 过的对象会被释放
* 消除了 unexpected 代码路径下对象没有没 cleanup 的可能
* 事实上可以做得非常高效

### 避免默认生成的操作, 或者全部定义他们
<div align=center><img src="https://i.imgur.com/qxOdgsm.png"/></div>

在 C++ 11 之前, 人们常常把类的特殊的函数称为 Big-3, C++ 11 引入了移动操作之后, 所有的特殊函数如上图, 合理地利用移动操作可以帮助我们写出效率更高的代码.
```C++
struct S{
  ~S(){};
  std::string s;
};
int main(){
  std::vector<S> ss;
  for(int i{}; i< 100; ++i){
    ss.emplace_back();
  }
}
```
上面这段代码中, 我们定义了析构函数, 于是编译器没有生成搬移的特殊函数, 在每个操作中都需要做一系列的复制, 效率特别低.  
<font color=blue>rules of zero</font> 可以帮助我们实现更高效地代码: 
* 不要自己定义任何的特殊函数
* 类初始化甚至可以包含默认构造函数做的事情

特例:
* 如果我们要管理资源,我们应该有构造函数/析构函数并且实施 RAII
* 这以为着我们要对所有的特殊函数做 定义/`=default`/`=delete`
* 在管理资源的时候, 只管理一份

### 尽量使用 stack-based 对象
Stack based 对象有下面这些优点:
* 没有动态分配, 一般要更加高效
* 可以帮助编译器优化代码
* less code/ safer / utilizes RAII

可以使用 heap profilers 帮助实现.

### 不要使用 c-style 内存管理
我们自定义一个包含一个字符串和定义了所有特殊函数的类, 因为在 OOP 中, 自定义类中使用 malloc/free 在生命周期没有调用构造函数/析构函数, **需要手动调用**, 就算如此,面对构造/析构函数抛出异常的时候也难以处理:
```C++
struct MyString{
  std::string s = "hello world";
};
int main(){
  void* p = malloc(sizeof(MyString));
  MyString *o = new(p) MyString();
  o->~MyString();
  free(p);
}
```

### 避免使用 `new`/`delete`
new/delete 比 malloc/free 要更好一点, 因为它们可以正确地初始化/析构对象, 但是和其他手动资源管理一样, 我们要尽力避免:
```C++
struct MyClass{
...
};
void do_something(){
  MyClass *o = new MyClass();
  // do the work
  delete o;
}
```
在分配和释放中间可能由于种种原因控制流离开了函数(如提前 return/ 异常), 这时候就会有资源泄漏.  
```C++
void delete_ptr(std::string *p, int){
  delete p;
}
void do_something(){
  delete_ptr(new std::string("hello"), do_something_else());
}
```
上面的例子中, 因为函数参数的求值顺序是未定义的, 因此很可能 `new` 了对象之后, 在 `do_something_else()` 抛出异常, 这时候控制流离开 `delete_ptr()`, 而手动分配的内存就没有释放.这时候我们应该使用 `unique_ptr<T>` 和 `make_unique<T>()`
```C++
void use_ptr(const unique_ptr<string>& p, int){}
void do_something(){
  use_ptr(make_unique<string>("hello"), do_something_else());
}
```
总结:
* 避免使用手动的资源管理
* 更多使用 RAII/ scoped pointers
* 使用`std::make_sharde<T>()`(C++11) 和 `std::make_unique<>()`(C++14), 这比手动构造然后传得智能指针要更加高效
* 可以使用的工具有 ReSharper C++

### 正确使用智能指针
`unique_ptr<>`:
* 用于表示 single ownership
* 相对于手动的堆内存管理没有任何额外的开销
* 在必要的时候可以轻易转化为后者

`shared_ptr<>`:
* 表示 multiple ownership
* 内部使用 reference counting 实现
* 相对于 `unique_ptr<>` 有额外开销(有时候很显著)

此外我们可以使用 lamnda 闭包来自定义内存管理的函数:
```C++
const auto d = [](FILE *f){fclose(f);};
const auto fileptr = unique_ptr<FILE, decltype(d)>(fopen("my/file","r"),d);
```

* `std::unique_ptr` 支持 array type 的构造和析构, 但是目前 `shared_ptr` 不支持.
* unique_ptr 是无法复制的
  ```C++
  std::vector<std::unique_ptr<int>> v;
  auto i = std::make_uinque<int>(5);
  v.push_back(i); // 编译错误
  v.push_back(std::move(i)); // ok
  v.emplace_back(std::make_unique<int>(5)); // better
  ```
* 不要使用 `auto_ptr`
  * 在没有移动语义之前设计
  * 在复制/拷贝赋值 时候 steal 所有权
  * 该特性已被弃用
* 我们可以使用的工具有 clang-tidy

### 使用 `weak_ptr` 打破环引用
```C++
#include<memory>
#include<vector>
struct Node: std::enable_shared_from_this<Node>{
  std::shared_ptr<Node> parent;
  std::vector<std::shared_ptr<Node>> childern;
  void addChild(const std::shared_ptr<Node> &chd){
    children.push_back(chd);
    chd->parent = this->shared_from_this();
  }
};
int main(){
  auto some_node = std::make_shared<Node>();
  some_node->addChild(std::make_shared<Node>());
}
```
address sanitizer 可以检测出这段代码中的环引用, 我们应该将 parent 设置为 weak_ptr, 并且提供一个函数可以访问:
```C++
struct Node: std::enable_shared_from_this<Node> {
  ...
private:
  std::weak_ptr<Node> m_parent;
public:
  std::shared_ptr<Node> parent(){
    auto p = m_parent.lock()
    if(p) return p;
    else throw std::runtime_error("no parents");
  }
}
```

### 除非转移控制权, 否则不要向函数传递智能指针
传递参数有五种使用场景, 可以使用 C++ Core checker 检查智能指针的滥用
* use: 可以传递指针或者引用
  * 如果参数不能为 null, 偏向使用引用类型
  * 尽可能使用 const
* may share
  * 传递一个 `const shared_ptr<T>&`, 这意味着指针是 const 而不是指针指向的对象, 我们可以修改指向的对象
  * 如果不修改对象, 用 `const shared_ptr<const T>&`
* share
  * 传递 `shared_ptr<T>`, 在函数需要增加指针的 reference counting 时候
* reseat
  * 传递 `shared_ptr<T>&/unique_ptr<T>&`: 函数调用后指针指向另一个对象
* take
  * 将要转移智能指针的控制权, 因为它无法复制, 因此只有被搬移的时候代码才能编译成功
  * `void take(unique_ptr<T>)`

### 避免拷贝 `shared_ptr`
在拷贝 `shared_ptr` 时, 产生一个新的`shared_ptr` 对象, 分享所有权
* 引用计数的增加/减小 都是原子操作, 有很大的开销
* 在回收时, 需要检查引用计数是否为 0 
* 考虑传递过程中可能的临时对象
* 我们可以考虑使用 `T&`/`reference_wrapper<T>`/`T*`

### 不要传递从有别名的智能指针中获取的指针或引用
