# STL 体系结构与内核分析
## Intro
标准库以头文件的形式出现:
* C++ 标准库的头文件不带后缀`.h`
* 新式的 C 头文件不带后缀 `.h`, 但是加了一个c, 如`cstdio`
* 旧式的 C 头文件依旧可以使用`stdio.h`

旧式的头文件内组件没有封装在命名空间 std 中, 但是新式的在.
几个重要的资源
* cplusplus.com
* cppreference.com (可以通过 cppman 在命令行访问)
* https://gcc.gnu.org/onlinedocs/

## STL 体系结构基础
STL 有六大基本组件: **容器, 分配器, 迭代器, 算法, 仿函数, 适配器**.其关系如下:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622143801.png"/></div>

* 迭代器有点像泛化的指针, 是算法和容器之间的桥梁
* 仿函数作用像是一个函数, 实际上是重载了操作符`()`的累的实例
* 适配器

```C++
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;
int main(){
    int ia[6] {27, 210, 12, 47, 109, 83};
    vector<int, allocator<int>> vi(ia,ia+6);
    std::cout << count_if(vi.begin(),vi.end(),
                          not1(bind2nd(less<int>(),40)));
}
```
* `count_if` 是 STL 中的算法, 计算给的条件之下符合的元素有几个
* `not1`/`bind2nd` 都是**函数适配器**, 作用分别是对 bool 求反 和绑定第二个参数
* `less` 是标准库中的仿函数

<font size=5> **前闭后开区间** `[)` </font> 无法对 `c.end()`解引用. 
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622164029.png"/></div>

```C++
Container<T> C;
...
Container<T>::iterator ite = std::begin(C);
for( ; ite != c.end(); ++ite){
    // do something
}
// range based for loop
// since C++ 11
for(auto& elem : c){
    // do some thing
}
```

## 容器的分类与测试 
### 容器 - 结构与分类
* 顺序容器
  * <font color=pink>array</font>: 语言的一部分, since C++11 也变成了一个 class(`std::array`)
  * vector: 空间可以自动增长, 分配器自动做这个事情
  * deque:两端都可以增长
  * list: 双向环状列表
     <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622183340.png"/></div>
  * <font color=pink>forward_list</font>
    <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622183611.png"/></div>
* 关联容器: 对于大量需要查找的元素, 更方便, 一般使用是红黑树实现(高度平衡), **multi 表示 key 可以重复**
  * Set/Multiset: set 的 key 和 value 是同一个
    <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622182751.png"/></div>
  * Map/MultiMap
    <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622182839.png"/></div>
* <font color=pink>Unordered(不定序) Container</font> 也可以认为是一种关联式容器, 实际上**最好的实现是 separate chaining**, 如果碰撞率过高, 需要 rehashing. <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622184025.png"/></div>
  * Unordered Set/MultiSet
  * Unordered Map/MultiMap

使用到几个[辅助的函数](src/test_helper.cpp), 运行 [array 的测试](source/array_test.cpp)结果如下:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622191032.png"/></div>

### 使用容器 vector
