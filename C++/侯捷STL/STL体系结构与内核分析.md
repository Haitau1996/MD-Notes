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
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622143801.png"/></div>

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
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622164029.png"/></div>

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
  * <font color=pink>array</font>: 语言的一部分, since C++11 也变成了一个模板类(`std::array`)
    ```C++
    template<class T, std::size_t N>
    struct array;
    ```
  * vector: 空间可以自动增长, 分配器自动做这个事情
  * deque:两端都可以增长
  * list: 双向环状列表
     <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622183340.png"/></div>
  * <font color=pink>forward_list</font>
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622183611.png"/></div>
* 关联容器: 对于大量需要查找的元素, 更方便, 一般使用是红黑树实现(高度平衡), **multi 表示 key 可以重复**
  * Set/Multiset: set 的 key 和 value 是同一个
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622182751.png"/></div>
  * Map/MultiMap
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622182839.png"/></div>
* <font color=pink>Unordered(不定序) Container</font> 也可以认为是一种关联式容器, 实际上**最好的实现是 separate chaining**, 如果碰撞率过高, 需要 rehashing. <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622184025.png"/></div>
  * Unordered Set/MultiSet
  * Unordered Map/MultiMap

#### 几个容器的测试
* 使用到几个[辅助的函数](source/test_helper.cpp), 运行 <font size= 5>[array 的测试](source/array_test.cpp)</font>结果如下:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210622191032.png"/></div>

* 使用容器 <font size=5>vector [测试](source/vector_test.cpp)</font>一个查找的过程:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623160636.png"/></div>
 
  发现 `std::find()` 相对还是比较快的, 而 sort 之后再 bsearch , sort 过程就要花费很多时间.   
* 接下来测试 <font size=5>[list](source/list_test.cpp)</font>:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623161557.png"/></div>

_list / forward_list / deque_ 都有最大的个数, 值和具体运行环境有关. 可以发现, 对 _list_ 排序的时间比 _vector_ 明显要长.  
* 接下来测试<font size=5>[forward_list](source/forward_list_test.cpp)</font>
  <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623162448.png"/></div>
  
需要注意的是, 仔细查看 `forward_list` 的 Modifiers, 发现它只有 `push_front()` 成员函数而没有 `push_back`, 直觉上也是, 如果没有表尾指针, 向尾部插入的速度是十分不合理的. 
* 测试一个 gun c 的非标准单向链表 <font size=5>[slist](source/slist_test.cpp)</font>
  <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623163804.png"/></div>

* 测试一个 <font size=5>[deque](source/deque_test.cpp)</font>.
  <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623164705.png"/></div>

  先看 deque 的结构, 可以发现它是 **用分段连续的方法提供一个元素放在连续空间中的假象**. 
  <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623170249.png"/></div>

* [_stack_](source/stack_test.cpp) 和 [_queue_](source/queue_test.cpp) 实际上是一个容器适配器, 其内部是一个 _deque_:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623171139.png"/></div> <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210623171251.png"/></div>
  因为他们只有一个方向 压入/弹出, 故对应的成员函数是 `push()`/`pop()`. 此外, _deque / queue / stack_ 不提供迭代器, 因为**通过迭代器修改元素会改变其独特的性质**.

* [_multi_set_](source/multiset_test.cpp) 测试中可以看到, 容器自带的 sort 比 `std::sort` 块:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624093336.png"/></div>
* [_multi_map_](source/multimap_test.cpp): 需要注意的是, 这里使用了 pair, map 中的一个元素是`<int, string>`, 并且 multimap 不能用 `[]` 做 insersiton<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624094048.png"/></div>
* [_unordered_multiset_](source/unordered_multiset_test.cpp):可以看到篮子数量比元素数量还要多, 这是设计上的内部考量, 元素个数大于篮子个数的时候, 篮子数量会大致翻倍然后重新打散元素.<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624094948.png"/></div><div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624095257.png"/></div>
* [_unordered_multimap_](source/unordered_multimap_test.cpp):<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624095628.png"/></div>
* [_set_](source/set_test.cpp): 和 multiset 不同,其中的元素不可重复, 重复的元素就被忽略了, 故 size 刚好为 random 能产生的数字数量(32768)<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624105639.png"/></div>
* [_map_](source/map_test.cpp): 值得注意的是,这里可以使用`[]` 插入元素<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624110336.png"/></div>

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
* 面向对象编程企图将数据结构和算法结合在一起, 可以看到 list 这时候就只能用 `someList.sort()`, 无法用 `::sort`, 因为**其内部用到了随机访问迭代器** <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624125219.png"/></div>
* 而 GP 就是将数据结构和算法分开, 通过迭代器提供接口, 而在算法那边, 可以根据不同的迭代器类型特化不同版本的 `::sort`:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624130457.png"/></div><div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624130006.png"/></div>

  * 容器和算法的团队可以各自闭门造车, 期间以迭代器联通即可
  * 算法通过迭代器确定操作范围, 并且通过迭代器取用容器的元素

所谓的算法, 其内部最终**涉及元素本身**的操作, 无非**比大小**而已.   
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624131015.png"/></div>

### 操作符重载与模版(泛化/特化/偏特化)
[操作符重载](https://en.cppreference.com/w/cpp/language/operators),其具体的形式如下, 我们一般要注意区分前置和后置的情形.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624151549.png"/></div>

具体我们可以看一个 list 的迭代器, 我们说它是泛型指针, 那么所有能对指针进行的操作, 我们也可以在这种迭代器做:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624152520.png"/></div>

[class template](https://en.cppreference.com/w/cpp/language/class_template)在使用的时候需要**明白地告诉编译器具体参数的类型是什么**:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624152758.png"/></div>

[function template](https://en.cppreference.com/w/cpp/language/function_template) 是泛化的函数, 编译器会对 function Template 进行**实参推导**.  
这里对 [member template](https://en.cppreference.com/w/cpp/language/member_template) 不做深入的讨论.  

#### 模板的泛化与特化
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624153709.png"/></div>

* 特化的版本, `template<>` **尖括号中是空的**, 不需要写`typename etc`
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624154010.png"/></div>

#### 模板的偏特化(partial specialization)
出现在模板参数有多个或者专门针对指针的情况, 例如我们的向量对于bool可以有更节约空间的方式实现:
```C++
// 泛化
template<class T,
         class Alloc= alloc>
clase vector{
  ...
};
// 偏特化
template<class alloc>
class vector<bool, alloc>{
  ...
};
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624154545.png"/></div>

### 分配器
在所有的 C++ 平台上, new 都是调用 `operator new`, 背后调用 `malloc` 得到的块比我们想象的要大一些, 附加了很多额外的开销, 面对小块内存, overhead 占的比例就大:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624161946.png"/></div>

接下来查看 VC6 中的 allocator 实现:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624162310.png"/></div>

Borland C++ 5的实现如下:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624162943.png"/></div>

GUN C 2.9.1 也差不多( `<defalloc.h>`):
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624163213.png"/></div>

这会引发一个问题, 如果我们容器中放很多个元素, 单个元素的大小不大, 那么 overhead 的影响可能就无法忽略. 实际上, 各个容器的默认分配器在 GUNC 2.9.1 中是`alloc` ( `<stl_alloc.h>` )而不是`allocator`.其诉求是**尽量减小 malloc 的次数**, 它维护一条链表, 每个位置对应一个大小并且指向一个链表, 要的时候过去查看对应链表是否为空, 是的话才调用 _malloc_. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624164009.png"/></div>

GNU C++ 4.9 之后使用的分配器变成了 `std::allocator`, 它有一个别名为`__allocator_base<_Tp>` 的父类`new_allocator`
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624164450.png"/></div>

G4.9 的标准库还有很多 extention allocators, 其中`__pool_alloc` 就是上面提到的 `alloc`:
```C++
vector<string, __gnu_cxx::__pool_alloc<string>> vec;
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624164802.png"/></div>

C++2.9中的容器的结构和分类如下:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624171803.png"/></div>

### 深度探索 _list_
在 2.9 版本中, 我们发现, _list_ 本质上里面只有一个数据成员, `node`, 就是一个指针, 在 32 位的系统上取 sizeof 自然就是 4. 它是一个指针, 指向一个空白节点:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624172338.png"/></div>

为什么需要向 `__list_iterator` 传入三个参数(实际上后续的版本已经改了)?  
其中的 iterator 是一个 class, 才能足够 smart. `list<T>::iterator` 就是 `__list_iterator<T, T&, T*>`. 它有 5 个 _typedef_ 和一系列的操作符重载. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624183046.png"/></div>

后置的 `++`用了很多已经被重载的操作符, 但是具体的调用细节还是要仔细琢磨:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624183431.png"/></div>

前置的返回的是一个 reference, 后置的返回的是一个对象.   
GCC 4.9 中的 List 变得十分复杂:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624191522.png"/></div>
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210624191658.png"/></div>

### 迭代器的设计原则
_traits_ 是人为制造的萃取机, iterator traits 能够萃取出迭代器的特性, 我们就需要知道它有哪些特性. <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625103323.png"/></div>

例如 rotate 中就需要一个参数`std::__iterator_category(__first)`, 而它是萃取机`iterator_traits<_iter>::iterator_category()`, 在 rotate 的原型中, 还想要知道 iterator 的 `difference_type` 和 `value_type`.此外迭代器的相关类型还有 `reference` 和 `pointer`.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625105134.png"/></div>

不是 class, 没有办法做 typedef, 如往算法中传递的不是通用的迭代器而只是 raw pointer.  萃取机作为一个中介层就能解决问题: 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625111429.png"/></div>

traits 可以接收任意的 type, 那么根据传入的类型做偏特化. 为什么传入 const pointer 依旧得到 type, 去了 const, **因为声明一个无法被赋值的变量没什么用**. 标准库中有各种 traits:
* type traits `<…/C++/type_traits>`
* iterator traits `<.../C++/bits/stl_iterator.h>`
* char traits `<…/C++/bits/char_traits.h>`
* allocator traits `<…/C++/bits/alloc_traits.h>`
* pointer traits `<…/C++/bits/ptr_traits.h>`
* array traits `<…/C++/array>`

### _vector_
主要的标准库实现容量到了之后都是两倍的增加. 它是靠三根指针控制整个vector, 故它的 size 在 32 位系统上是 12.每次成长会大量调用拷贝函数和析构函数, 故需要注意这部分开销. <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625125137.png"/></div>

在 vector 中含有指针, 实际上就可以当作迭代器. 当它被丢给萃取机时, 就会走入 `T*` 的路径.在 C++ 2.9 中vector 是一个单一的类, 但是在 4.9 中就有了复杂的继承关系:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625131006.png"/></div>

这个 impl 为了用 allocator 的公共, 但是没有 is-a 关系, 设计时候使用 public 继承其实不是好的习惯. _vector_ 中的 iterator 实现非常曲折, 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625131923.png"/></div>

### _array_
没有包装的话, 就无法享受STL 中便利的算法/仿函数. TR1 中的 array 也用 typedef 实现了 `value_type`/`pointer`/`iterator`, 同样它的迭代器在萃取机中, 走的也是针对指针特化的版本. 在 4.9 中也变得十分复杂:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625132619.png"/></div>

forward_list 对比 list, 没有更多的东西, 故不多介绍.

### _deque_
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625133020.png"/></div>

deque 是分段连续的, 为了维持这种假象, 迭代器走到边界的时候需要有能力找到上/下一个缓冲区. <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625144829.png"/></div>

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625152518.png"/></div>

```C++
inline size_t __deque_buf_size(size_t n, size_t sz){
    return n!=0? n:(size<512? size_t(512/sz):size_t(1));
}
```
2.9 版本的是允许指定缓存区大小的, 我们从 `insert(iterator position, const value_type&x)` 看deque 动态变化的过程, 它首先会判断是否是在头部和尾部, 都不在的话调用 `insert_aux`.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625153426.png"/></div>

### _deque_ 如何模拟连续空间
deque 就是依靠迭代器完成了对连续空间的模拟, 该迭代器重载了很多操作符. 
* 两个迭代器的距离相当于 $ L_{buffer}\times N_{buffer difference} + itr到末尾长度 + x 到开头的长度$
    ```C++
    difference_type operator-(const self& x) const{
        return difference_type(buffer_size())* (node - x.node -1)+
        (this->cur - this->first) + (x.last - x.cur);
    }
    ```
* 重载 `++`/`--` 的时候需要考虑到边界的问题<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625155753.png"/></div>
* 此外, 作为随机访问迭代器, 提供了 `+`/`+=` 等操作符重载, 需要先判断目标点是否落在同一个 Buffer:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625160315.png"/></div>

4.9 不再允许调整 Buffer size:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625160747.png"/></div>

控制中心是一个 vector, 增长的时候它是拷贝到中间那段, 这样向左右都可以扩充缓冲区. 

### _queue_ 和 _stack_
他们是容器适配器, 其内部含有一个 容器, 然后将无法使用的功能封起来.
```C++
template <class T, class Sequence=deque<T>>
class queue {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c; //底層容器
public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size() ; }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front() ; }
}
```
_stack_ 或 _queue_ 都 **<font color=red>不允许</font> 遍历**, 也 **不提供迭代器**. 他们都可以选择 _list_ 或者 _deque_(默认) 作为底层容器.   
_stack_ 可以选择 vector作为底层容器, 但是 _queue_ 不能(vec 没有 `pop_front()`).  
他们都不能用 `set/map` 作为底层结构.

### 红黑树深度探索
红黑树是高度平衡的二叉搜索树, 它的排序规则有利于 search 和 insert, 并且保持适度平衡(没有任何节点过深).  
rb_tree 提供遍历以及迭代器, 按正常规则遍历, 便能获得排序状态.   
我们不应该(没有绝对禁止)使用红黑树的迭代器改变元素值, 修改会破坏其内部结构. 在它的支撑下, map 允许元素的 data 被改变, 但是元素的 key 是不可以被改变的.   
提供两种 insertion 的操作, `insert_unique()` 和 `insert_equal()`.  

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625164259.png"/></div>
在双向链表中有一个刻意制造的不含data的节点, 在红黑树中也是(为了方便实现).我们可以尝试自己调用一下红黑树:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625165135.png"/></div>

```C++
//GCC 2.9
rb_tree<int, int, identity<int>, less<int>> itree;
// GCC 4.9  _Rb_tree<int, int, _Identity<int>, less<int>> itree;
cout << itree.empty() << endl; //1
cout << itree.size () << endl; //0
itree.insert_unique(3); // GCC 4.9 itree._M_insert_unique(3);
itree.insert_unique(8);
itree.insert_unique(5);
itree.insert_unique(9);
itree.insert_unique(13);
itree.insert_unique (5) ; // no effect, since using insert_unique(()) .
cout<< itree.empty() << endl; //0
cout << itree.size() << e ndl; //5
cout<<itree.count( 5 ) << endl; //1
itree.insert_equal(5); // itree._M_insert_equal(5);
itree.insert_equal(5);
cout << itree.size() << endl; //7
cou t<< itree.count(5) << endl; //3, since using insert_equal ().
```

### _set / multi_set_
他们排序的依据就是 key, 它的 key 和 value 合二为一, 我们 <font color=red> 无法 </font> 使用他们的迭代器改变元素的值. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625171153.png"/></div>

VC6 中没有提供 `identity()`, 但是他们有类似的实现:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625171610.png"/></div>

### _map / multi_map_
他们排序的依据是 key, 同时提供的迭代器也无法修改元素的 key, 但是**修改 data 的操作是合理的**. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625172438.png"/></div>

它的迭代器就是红黑树的迭代器, 打包成 pair 的时候指定的 **Key 是 _const_**. 在 VC6 中则是自己写一个仿函数, 实现了类似于 _select2st_ 的功能. <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625172738.png"/></div>

map 重载了操作符`[]`: 传回和 key 相关联的 data, key 不存在的话就会创建一个元素,data 为默认值.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625180528.png"/></div>

### 容器 hashtable
hashtable 实现没有太多数学上的东西, 更多的是经验的总结.<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625180840.png"/></div>

发生碰撞的时候, 需要设计将它们拉开. 更好的做法是发生碰撞后将它们放到链表中(separate chaining):
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625181333.png"/></div>

链表太长(元素的个数比篮子的个数多)的话需要将它们打散, 篮子增加两倍(不严格, 因为用的质数), 然后重新 hashing. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210625182008.png"/></div>

```C++
// 标准库中的定义
template<class Value,
         class Key,
         class HashFcn,
         class ExtractKey,
         class EqualKey,
         class Alloc = alloc>
class hashtable{
public:
    typedef HashFun hasher;
    typedef EqualKey key_equal;
    typedef size_t size_type;
private:
    hasher hash;
    key_equal equals;
    ExtractKey get_key;
    ...
};
// 写一个包装器
struct eqstr{
  bool operator()(const char* s1, const char* s2) cosnt{
    return strcmp(s1, s2) == 0; // strcmp 返回值是 -1, 0, 1
  }
};
// 在 APP 端这样使用
hashtable<const char*,
          const char*,
          hash<const char*>,
          identity<const char*>,// GCC 特有的, VC6 有类似的实现
          eqstr,// 这是一个函数包装器, 因为 c-string 的strcmp接口和我们想要的不同
          alloc>
ht(50, hash<const char*>(), eqstr());
ht.insert_unique("kiwi");
```
在 GNU C++ 2.9 中使用模板偏特化的方法, 对特种类型的数据实现了对应的 hash:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628101129.png"/></div>

但是并不是所有类型都有 哈希函数, 如在 GCC 2.9 中只针对 C-Style String 有 hash 函数.得到了 哈希值后, 决定在哪个篮子里一般就是取模运算, 一般篮子数的大小为质数.

## Lecture 3: Algorithm
STL的六大部件, 除了算法是函数模板, 其他的都是类模板.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628102557.png"/></div>

算法看不到容器, 但是可以通过向迭代器问答的方式(使用萃取机, 如`typename iterator_traits<I>::iterator_category`)得到容器的信息(如迭代器怎么走).

### 迭代器的分类
```C++
//五種iterator category
struct input_iterator_tag {} ;
struct output_iterator_tag {} ;
struct forward_iterator_tag : public input_iterator_tag {} ;
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_itcrator_tag{};
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628102806.png"/></div>

迭代器有五种, 是通过类继承的方式形成了层次结构,这种分类比 `enum` 要好:
```C++
void _display_category(random_access_iterator){
    std::cout << "random access"<< std::endl;
}
void _display_category(bidirectional_iterator){
    ...
}
...
template<typename Iter>
void display_category(Iter iter){
    typename iterator_traits<Iter>::iterator_category cagy;
    _display_category(cagy);
}
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628105453.png"/></div>

考察一下 `ostream_iterator` 的上线, 发现它继承了 一系列的 `typedef`, 可以少写不少东西. 

### 迭代器分类对算法的影响
我们以 `distance` 函数为例, 说明迭代器分类对于算法效率的影响:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628110003.png"/></div>

这是STL 中算法实现的基本方法: **先有一个总的函数, 在里面萃取出迭代器的类型, 然后根据迭代器类型调用不同的重载版本**.接着看 `advance`:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628124505.png"/></div>

从这里也可以看出,使用类的层次结构可以不用对每种迭代器都实现特定的重载版本, 可以沿着继承链向上查找. 下面看 迭代器和类型的萃取机是如何影响 copy 的实现:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628130539.png"/></div>

这里的类型萃取机就是在问类型有没有实现 non-trival(非编译器默认给出的) 的操作符 `=`.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628131306.png"/></div>

算法是模版函数, 要可以针对各种各样的类型, 语法没办法让主函数只能接收某种类型的迭代器, 但是会有"暗示".<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628132012.png"/></div>

### 算法源码剖析
```C++
template <class InputIerator ,
          class T>
T accumulate (InputIterator first ,
              InputIterator last ,
              T init ){
  for ( ; first != last ; ++first)
    //將元素累加至初值init 身上
    init = init + *first;
  return init;
}
template <class InputIerator ,
          class T,
          class BiniryOperation>
T accumulate (InputIterator first ,
              InputIterator last ,
              T init ,
              BinaryOperation binary_op){
  for ( ; first != last ; ++first)
    //將元素累 **计算** 至初值init 身上
    init = binary_op(init,*first);
  return init;
}
```
这样的话, [测试函数](source/accumulate_test.cpp)就覆盖了不同版本的累积函数.传入的函数只要和小括号中的参数相同即可, 可以是一般的函数(函数指针)/函数对象/lambda 表达式.
```C++
template <class InputIteraror ,
          class Function>
Function for_each (InputIterator first ,
                  InputIterator last ,
                  Function f ){
  for ( ; first ! = last; ++first)
    f(*first);
  return f;
}
```
`for_each` [实例](source/for_each_test.cpp)就是对每个元素, 都传入的可调用对象(参数只有一个元素)执行.  
C++ 标准库中很多算法都有`algo`/`algo_if`/`algo_copy` 对:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628134957.png"/></div>

其中的 _Predicate_ 可以理解为一种断言, 根据迭代器指向对象返回一个 boolean 值.

有的算法除了有全局的版本`std::algo()`/ , 部分容器还有成员函数实现`c.algo()`:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628142338.png"/></div>

`find/find_if` 也是这样, 因为下面八个关联容器有自己特有的数据组织方式, 因此类内实现起这些操作来比较快.  
`sort` 要求的是随机访问迭代器, 所以 `list`/`forward_list` 只有成员函数版本的 `c.sort()` 实现. 使用的时候还要注意可以有反向迭代器(实际上是迭代器适配器):
```C++
reverse_iterator
rbegin()
{ return reverse_iterator(end());}
reverse_iterator
rend()
{ return reverse_iterator(begin());}
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628143651.png"/></div>

在使用二分查找的时候, 内部用了`lower_bound`/`upper_bound`:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628144220.png"/></div>

```C++
template <class Forwardlterator, class T>
ForwardIterator
lower_bound(ForwardIterator first,
            ForwardIterator last,
            const T& val){
  ForwardIterator it;
  iterator_traits<Forwardlterator>::differencc_type count, step;
  count = distance(first, last);
  while (count>O)
  {
    it = first; step=count/2; advance(it,step);
    if (*it < val) {
      first= ++it;
      count -= step +1;
    }
    else count = step;
  }
  return first;
}
template <class Forwardlterator, class T>
bool binary_scarch (Forward iterator first,
                    Forwardlterator last,
                    const T & val)
{ 
    first = std::lower_bound(first, last, val);
    return (first!=last && !(val < *first));
}
```

### 仿函数(Functor)
标准库提供的仿函数有下面几个主要的类型(继承 _binary_function_ 是融入 STL 的体系结构, 没有额外开销得到了很多个 `typedef`, 并且可以被适配(adaptable)):
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628152042.png"/></div>

1. 算术类
    ```C++
    template<class T>
    struct minus:public binary_function<T, T, T>{
      T operator()(const T& x, const T& y) const
      { return x-y;}
    };
    ```
2. 逻辑运算类
    ```C++
    template <class T>
    struct logical_and :public binary_funttion<T, T, bool> {
      bool operator()(const T& x, const T& y) const
      { return x&&y;}
    };
3. 相对关系类(比大小)
    ```C++
    template <class T> 
    struct equal_to : public binary_funrtion<T, T, bool> {
      bool operator()( T & x, const T & y) const
      { returr(x == y;)}
    };
    ```

这些动作要传给算法, 故需要用函数/仿函数的形式, GNU C 中有一些有用但是非标准的仿函数:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628151341.png"/></div>

### 函数适配器
这个适配器A改造了B, 那么使用A的时候, 内部的事情实际上就是 B 在做. A 是使用者和 B 之间的桥梁. 在我们讨论的这么多适配器中, 都是用内含(而非继承)的方式取实现.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628153148.png"/></div>

#### 函数适配器
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628154124.png"/></div>

我们先看 `bind2nd`, 它的一个大的原则是先记住其参数(`less<int>()`产生的一个临时对象和 40, 需要注意的是, 很多时候类名+小括号**是在产生临时对象**而不是调用仿函数), 然后真正用的时候才会绑定.  
使用辅助函数 `bind2nd` 一个很重要的原因是我们无法写出 `Operation` 的具体类型, 因此需要使用这个辅助函数, 让编译器帮忙推导, 然后传入`binder2nd` 的构造函数中, 这时候内部的 protected 数据就记住了两个参数.  
`count_if` 的第三参数, _pred_, 不断地调用 它的 operator(), 到此故事就结束了.  
函数适配器能回答 `Operation::second_argument_type` 这个问题, 因此会在 `binder2nd<...>(...)` 的时候检查类型, 这样的话类型不匹配在编译的时候就会报错, 而不是把问题留在运行时.而且返回值类型和适配后传入的第一个值的类型都可以用类似的萃取方法得到.因此**一个函数对象必须能够回答这三个问题才能和 `bind2nd` 这个函数对象完美搭配**, 故要继承 _binary_function_ 得到相应的 typedef.  
`typename`在这里的作用是帮助编译, 因为在编译到哪个地方的时候, `Operation` 是什么还不知道, 编译器到`A::B`会有犹豫, 它是类的成员 还是一个 typename.  
`binder2nd`适配完一个函数后, 它也可能被其他适配器适配, 因此需要继承 `unary_function`以回答相应的问题.  
`bind2st/bind2nd` 现在有点过时了, 被复杂的 `bind` 取代.

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210628160739.png"/></div>

有了上面的基础, 看 `not1` 就相对容易, 它也是用一个辅助函数, 在里面实例化一个临时的 `unary_negate` 对象, 这个对象中有一个 protected 成员记住了 `pred`, 真正调用的时候才把记住的`pred`拿出来用.

#### `std::bind`
C++ 11 中引入了 `std::bind`, 它和placeholder(占位符)[配合使用](source/bind_test.cpp) 有很好的效果, 它可以绑定:
1. 函数
2. 函数对象
3. 成员函数, 占位符`_1`必须是某个对象的地址
4. 数据成员, 占位符`_1`必须是某个对象的地址

### 迭代器适配器
#### reverse_iterator
之前提到的`rbegin`就是一个迭代器适配器:
```C++
template<class Iterator>
class reverse_iterator{
protected:
  Iterator current;
public:
  typedef typename iterator_traits<Itertaor>::iterator_category iterator_category;
  ...
  // 最重要的是 dereference, 对反向迭代器的取值就是对正向的前一个位置的取值
  reference operator*() const{
    Iterator temp = current;
    return *--temp;
  }
  ...
}
```
#### inserter
我们在调用 copy 的时候, 实际上对于每个元素已经写死了是调用赋值操作.如果我们想要在其中插入, 可以使用 _inserter_ 这个辅助函数(接收容器和迭代器), 在里面使用了一个迭代器适配器 _insert_iterator_, 它在里面重载了操作符`=`:`copy(bar.begin(), bar.end(), inserter(foo, it))`.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629095135.png"/></div>
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629095208.png"/></div>

使用之后, 将迭代器适配成为了 _insert_iterator_, 因此使用 copy 的时候调用的是适配器重载的操作符.

#### ostream_iterator
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629101324.png"/></div>

`std::cout` 是一个对象, 可以取地址, 用于初始化 `ostream_iterator` 适配器之后, 适配器里面重载了 操作符`operator=`/`operator++`, 就符合 copy 的操作.

#### istream_iterator
当我们在创建一个输入流迭代器的时候已经在读数据了.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629102228.png"/></div>
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629102254.png"/></div>

同一个 copy 搭配不同的适配器就会有不同的行为.

## Lecture 4
### 一个万用的哈希函数
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629103653.png"/></div>

类的 hash function 一个 naive 的思路就是将各个元素的哈希值加起来.TR1 就有一个一个不错的哈希函数
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629104302.png"/></div>

上面的 1,2,3 三个同名函数, 使用了可变参数模板, 因为其类型不同, 所以是不同的东西.  
1️ 所做的事情就是把事情传给 2 做, 2 会先调用 4, 然后做递归, 最后一步就会转给 3做,它同样需要调用 4. 于是核心就在 4 的修改种子的动作中.

4做的事情没有任何数学支持, 就是要越乱越好. 比单纯的将元素的 hash code 相加要好不少. 其中的魔数就是黄金分割在 16进制中的表示.

```C++
//from boost <funcfional/hash>
template <typename T>
inline void hash_combine(size_t& seed , const T& val){
  seed ^= hash<T>()(val) + 
          0x9e3779b6 +
          (seed << 6) +
          (seed >> 2);
}
template <typename T>
inline void hash_val(size_t& seed, const T& val){
  hash_combine(seed,val);
}
template <typename T, typename... Types>
inline void hash_val(size_t& seed,
                     const T& val,
                     const Types&... arsg)
{
  hash_combine(seed,val);
  hash_val(seed,args...);
}
template <typename... Types>
inline size_t hash_val(const Types&... arsg){
  size_t seed = 0l
  hash_val(seed, args...);
  return seed;
}
// Functor
class CustomerHash{
  public:
    size_t operator()(const Customer&s)const{
      return hash_val(c.fname, c.lname,c.no);
    }
};
```
### Tuple
在 G4.8 中, 我们可以看到使用可变参数模板和继承实现的 tuple:
```C++
template<typename... Values> class tuple;
template<> class tuple<>{};

template<typename Head, typename... Tail>
class tuple<Head, Tail...>
:private tuple<Tail...>{
  typedef tuple<Tail...> inherited;
public:
  tuple(){}
  tuple(Head v, Tail.. vtail):
    m_head(v),inherited(vtail...){}//inherited(vatil) 是调用父类构造器, 不是生成临时对象
  typename Head::type head(){return m_head;}
  inherited& tail(){ return *this;}
protected:
  Head m_head;
}
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629114920.png"/></div>

使用 tuple 也很方便, 可以用 `get<n>(someTuple)`从中得到某个元素, 也有类似于萃取的机制 `tuple_size<tuple_type>::value`.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629115457.png"/></div>

### Type Traits
G2.9 中实现了一些简单的 type_traits, 有一个泛化的版本和若干个针对不同类型的特化版本:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629125338.png"/></div>

这个实用性不强, 因为需要用户为自己的类写出 traits. [C++ 11 之后](http://www.cplusplus.com/reference/type_traits/)我们不需要为自己的类写特化的 traits, 就可以直接使用, 如 `is_void<T>::value`/`has_virtual_destructor<T>::value`.

### type_traits 的实现
实现 `is_void`, 我们先看使用模板的方式是如何去掉常量性质和易变(volatile)性质, 具体是使用两个接收不同参数的 type traits, 非常量/易失 就走泛化版本, 否走走特殊路径, 两者都返回相同的值:
```C++
template<typename _Tp>
struct remove_const{ 
  typedef _Tp type;
};
template<typename _Tp>
struct remove_const<_Tp const>{// 对 const 的特化版本
  typedef _Tp type; 
};
template<typename _Tp>
struct remove_volatile{ 
  typedef _Tp type;
};
template<typename _Tp>
struct remove_volatile<_Tp volatile>{// 对 volatile 的特化版本
  typedef _Tp type; 
};

/// remove cv
template<typename _Tp>
struct remvoe_cv{
  typedef remove_const<typename remove_volatile<_Tp>::type>::type type;
};
/// add const
template<typename _Tp>
struct add_const{
  typedef _Tp const  type;
};

/// 同样的, 对于void, 我们有泛化的版本返回 false/ 特化的版本返回 true
template<typename >
struct __is_void_helper:public false_type{};
template<>
struct __is_void_helper<void>:public true_type{};
template<typename _Tp>
struct is_void
:public __is_void_helper<typename remove_cv<_Tp>::type>::type
{};
```
从上面的实现来看, 实际上就是先把 常量性和易失性 这种令人迷惑的东西, 然后丢给 helper, 然后在 helper 中使用泛化版本回答是假, 然后实现一堆特化版本实现为 true.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629145315.png"/></div>

`is_class, is_union, is_enum, is_pod` 这些在源代码中无法找到具体的实现, 可能要依赖编译器:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629145600.png"/></div>

### cout
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/backups/20210629131552.png"/></div>

把很多类型的对象往 cout 扔, 就是因为他们实现了重载的版本. 如果标准库中没有的话, 我们需要为自己的类型重载操作符 `<<`.
