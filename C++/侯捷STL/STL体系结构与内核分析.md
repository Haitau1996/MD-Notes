# STL 体系结构与内核分析
## Lecture 1
标准库以头文件的形式出现:
* C++ 标准库的头文件不带后缀`.h`
* 新式的 C 头文件不带后缀 `.h`, 但是加了一个c, 如`cstdio`
* 旧式的 C 头文件依旧可以使用`stdio.h`

旧式的头文件内组件没有封装在命名空间 std 中, 但是新式的在.
几个重要的资源
* cplusplus.com
* cppreference.com (可以通过 cppman 在命令行访问)
* https://gcc.gnu.org/onlinedocs/

### STL 体系结构基础
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

### 容器的分类与测试 
#### 容器 - 结构与分类
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

#### 几个容器的测试
* 使用到几个[辅助的函数](source/test_helper.cpp), 运行 <font size= 5>[array 的测试](source/array_test.cpp)</font>结果如下:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210622191032.png"/></div>

* 使用容器 <font size=5>vector [测试](source/vector_test.cpp)</font>一个查找的过程:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623160636.png"/></div>
 
  发现 `std::find()` 相对还是比较快的, 而 sort 之后再 bsearch , sort 过程就要花费很多时间.   
* 接下来测试 <font size=5>[list](source/list_test.cpp)</font>:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623161557.png"/></div>

_list / forward_list / deque_ 都有最大的个数, 值和具体运行环境有关. 可以发现, 对 _list_ 排序的时间比 _vector_ 明显要长.  
* 接下来测试<font size=5>[forward_list](source/forward_list_test.cpp)</font>
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623162448.png"/></div>
  
需要注意的是, 仔细查看 `forward_list` 的 Modifiers, 发现它只有 `push_front()` 成员函数而没有 `push_back`, 直觉上也是, 如果没有表尾指针, 向尾部插入的速度是十分不合理的. 
* 测试一个 gun c 的非标准单向链表 <font size=5>[slist](source/slist_test.cpp)</font>
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623163804.png"/></div>

* 测试一个 <font size=5>[deque](source/deque_test.cpp)</font>.
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623164705.png"/></div>

  先看 deque 的结构, 可以发现它是 **用分段连续的方法提供一个元素放在连续空间中的假象**. 
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623170249.png"/></div>

* [_stack_](source/stack_test.cpp) 和 [_queue_](source/queue_test.cpp) 实际上是一个容器适配器, 其内部是一个 _deque_:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623171139.png"/></div> <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210623171251.png"/></div>
  因为他们只有一个方向 压入/弹出, 故对应的成员函数是 `push()`/`pop()`. 此外, _deque / queue / stack_ 不提供迭代器, 因为**通过迭代器修改元素会改变其独特的性质**.

* [_multi_set_](source/multiset_test.cpp) 测试中可以看到, 容器自带的 sort 比 `std::sort` 块:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624093336.png"/></div>
* [_multi_map_](source/multimap_test.cpp): 需要注意的是, 这里使用了 pair, map 中的一个元素是`<int, string>`, 并且 multimap 不能用 `[]` 做 insersiton<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624094048.png"/></div>
* [_unordered_multiset_](source/unordered_multiset_test.cpp):可以看到篮子数量比元素数量还要多, 这是设计上的内部考量, 元素个数大于篮子个数的时候, 篮子数量会大致翻倍然后重新打散元素.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624094948.png"/></div><div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624095257.png"/></div>
* [_unordered_multimap_](source/unordered_multimap_test.cpp):<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624095628.png"/></div>
* [_set_](source/set_test.cpp): 和 multiset 不同,其中的元素不可重复, 重复的元素就被忽略了, 故 size 刚好为 random 能产生的数字数量(32768)<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624105639.png"/></div>
* [_map_](source/map_test.cpp): 值得注意的是,这里可以使用`[]` 插入元素<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624110336.png"/></div>

### 使用分配器 _allocator_
[这里](source/allocator_test.cpp) 我们使用了集中 gunc "方言" 类型的内存分配器, 它们都在 `ext\*_allocator.hpp` 中定义. 一般而言, 分配器是配合容器使用的, 我们没有必要去直接使用分配器.实在是要直接使用分配去的话, 类似于这样写, allocate 中的数字是需要的元素个数:
```C++
int* p;
a1locator<int> alloc1 ;
p = alloc1.allocate( 1 ) ;
alloc1.deallocate(p, 1);
__gnu_cxx::malloc_allocator<int> alloc2 ;
p = alloc2.allocate( 1 );
alloc2.deallocate( p, 1);
```
从使用者的角度看, 我们使用 `new`/`delete`, `new[]`/`delete[]`, `malloc/free`应对小量的内存需求, **不需要记住到底要删除多少个元素**, 因此用起来比分配器要方便得多. 这时候, 如果我们要了 5 个而只还了 3个, 其结果就是未定义的. 

## Lecture 2
### 源代码的分布
* GUN C++ 以4.9.2 版本为例, 其目录表面上在 `\4.9.2\include\C++` 中, 实际上在
  *  `\4.9.2\include\C++\bit` 各种以 STL 开头的文件中
  *  `\4.9.2\include\C++\ext` 这里有 GNU 自己的一些方言(extention)

### _OOP_ vs. _GP_
* 面向对象编程企图将数据结构和算法结合在一起, 可以看到 list 这时候就只能用 `someList.sort()`, 无法用 `::sort`, 因为**其内部用到了随机访问迭代器** <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624125219.png"/></div>
* 而 GP 就是将数据结构和算法分开, 通过迭代器提供接口, 而在算法那边, 可以根据不同的迭代器类型特化不同版本的 `::sort`:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624130457.png"/></div><div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624130006.png"/></div>

  * 容器和算法的团队可以各自闭门造车, 期间以迭代器联通即可
  * 算法通过迭代器确定操作范围, 并且通过迭代器取用容器的元素

所谓的算法, 其内部最终**涉及元素本身**的操作, 无非**比大小**而已.   
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210624131015.png"/></div>

### 操作符重载与模版(泛化/特化/偏特化)