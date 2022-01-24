<font size=10> Using The C++ Standard Template Libraries</font>  
<font size=6>**Ivor Hortan**</font>
***

- [Chapter 1: intruction to STL](#chapter-1-intruction-to-stl)
  - [模板](#模板)
  - [容器](#容器)
  - [迭代器](#迭代器)
    - [迭代器的类别](#迭代器的类别)
    - [流迭代器](#流迭代器)
    - [迭代器适配器](#迭代器适配器)
      - [反向迭代器](#反向迭代器)
      - [插入迭代器](#插入迭代器)
      - [移动迭代器](#移动迭代器)
  - [迭代器上的运算](#迭代器上的运算)
  - [智能指针](#智能指针)
    - [使用`unique_ptr<T>`](#使用unique_ptrt)
      - [重置 `unique_ptr` 对象](#重置-unique_ptr-对象)
      - [比较和检查 `unique_ptr` 对象](#比较和检查-unique_ptr-对象)
    - [使用 `shared_ptr<T>`](#使用-shared_ptrt)
      - [重置 `shared_ptr<T>`](#重置-shared_ptrt)
      - [比较和检查 `shared_ptr<T>` 对象](#比较和检查-shared_ptrt-对象)
    - [使用 `weak_ptr<T>` 指针](#使用-weak_ptrt-指针)
  - [算法](#算法)
  - [将函数作为实参传入](#将函数作为实参传入)
    - [函数对象](#函数对象)
    - [Lambda 表达式](#lambda-表达式)
      - [将 lambda 表达式传给函数](#将-lambda-表达式传给函数)
      - [捕获子句](#捕获子句)
- [Chapter 2: 使用序列容器](#chapter-2-使用序列容器)
  - [使用 `array<T,n>` 容器](#使用-arraytn-容器)
    - [访问元素](#访问元素)
    - [使用数组容器的迭代器](#使用数组容器的迭代器)
    - [比较数组容器](#比较数组容器)
  - [使用 `vector<T>` 容器](#使用-vectort-容器)
    - [创建 `vector<T>`容器](#创建-vectort容器)
    - [容量和大小](#容量和大小)
    - [访问元素](#访问元素-1)
    - [使用 vector 容器的迭代器](#使用-vector-容器的迭代器)
    - [向 vector 容器中添加元素](#向-vector-容器中添加元素)
      - [增加元素](#增加元素)
      - [插入元素](#插入元素)
    - [删除元素](#删除元素)
    - [`vector<bool>` 容器](#vectorbool-容器)
  - [使用 `deque<T>` 容器](#使用-dequet-容器)
    - [生成 deque 容器](#生成-deque-容器)
    - [访问元素](#访问元素-2)
    - [添加和移出元素](#添加和移出元素)
    - [替换 deque 中的内容](#替换-deque-中的内容)
  - [使用 `list<T>` 容器](#使用-listt-容器)
    - [生成 list 容器](#生成-list-容器)
    - [添加元素](#添加元素)
    - [删除元素](#删除元素-1)
    - [排序和合并元素](#排序和合并元素)
    - [访问元素](#访问元素-3)
  - [使用`forward_list<T>` 容器](#使用forward_listt-容器)
  - [自定义迭代器](#自定义迭代器)
- [Chapter 3: 容器适配器](#chapter-3-容器适配器)
  - [创建和使用 `stack<T>` 容器适配器](#创建和使用-stackt-容器适配器)
    - [stack 操作](#stack-操作)
  - [创建和使用 `queue<T>`容器适配器](#创建和使用-queuet容器适配器)
    - [queue 的操作](#queue-的操作)
  - [使用优先队列](#使用优先队列)
    - [创建优先队列](#创建优先队列)
    - [优先队列的操作](#优先队列的操作)
  - [堆](#堆)
    - [创建堆](#创建堆)
    - [堆操作](#堆操作)
  - [在容器中保存指针](#在容器中保存指针)
- [Chapter 4: map 容器](#chapter-4-map-容器)
  - [map 容器的用法](#map-容器的用法)
    - [创建 map 容器](#创建-map-容器)
    - [map 元素的插入](#map-元素的插入)
    - [访问 map 元素](#访问-map-元素)
    - [删除元素](#删除元素-2)
  - [`pair<>` & `tuple<>` 的用法](#pair--tuple-的用法)
    - [pair 的操作](#pair-的操作)
    - [tuple 的操作](#tuple-的操作)
  - [multimap 容器的用法](#multimap-容器的用法)
  - [改变比较函数](#改变比较函数)
    - [`greater<T>`](#greatert)
    - [使用函数对象](#使用函数对象)
  - [哈希](#哈希)
  - [`unordered_map` 容器](#unordered_map-容器)
    - [生成和管理`unordered_map`](#生成和管理unordered_map)
    - [调整格子个数](#调整格子个数)
    - [插入元素](#插入元素-1)
    - [访问元素](#访问元素-4)
    - [移除元素](#移除元素)
    - [访问格子](#访问格子)
    - [unordered_multimap](#unordered_multimap)
- [Chapter 5: set 容器](#chapter-5-set-容器)
  - [使用 `set<T>` 容器](#使用-sett-容器)
    - [添加和移除元素](#添加和移除元素)
    - [访问元素](#访问元素-5)
    - [set 迭代器](#set-迭代器)
    - [在 set 容器中保存指针](#在-set-容器中保存指针)
  - [使用 `multiset<T>` 容器](#使用-multisett-容器)
- [Chapter 6 : 排序、合并、搜索和分区](#chapter-6--排序合并搜索和分区)
  - [序列排序](#序列排序)
    - [排序以及相等的元素的排序](#排序以及相等的元素的排序)
    - [部分排序](#部分排序)
    - [测试排序序列](#测试排序序列)
  - [合并序列](#合并序列)
  - [搜索序列](#搜索序列)
    - [在序列中查找元素](#在序列中查找元素)
    - [在序列中查找任意范围的元素](#在序列中查找任意范围的元素)
    - [在序列中查找多个元素](#在序列中查找多个元素)
      - [`find_end()` 算法](#find_end-算法)
      - [`search()` 算法](#search-算法)
      - [`search_n()` 算法](#search_n-算法)
  - [分区序列](#分区序列)
    - [`partition_copy()` 算法](#partition_copy-算法)
    - [`partition_point()` 算法](#partition_point-算法)
  - [二分查找算法](#二分查找算法)
    - [`binary_search()` 算法](#binary_search-算法)
    - [`lower_bound()` 算法](#lower_bound-算法)
    - [`equal_range()` 算法](#equal_range-算法)
- [Chapter 7: 更多的算法](#chapter-7-更多的算法)
  - [检查元素的属性](#检查元素的属性)
  - [序列的比较](#序列的比较)
    - [查找序列的不同之处](#查找序列的不同之处)
    - [按字典查找序列](#按字典查找序列)
    - [序列的排列](#序列的排列)
  - [复制序列](#复制序列)
    - [复制一定数目的元素](#复制一定数目的元素)
    - [条件复制](#条件复制)
  - [复制和反向元素顺序](#复制和反向元素顺序)
  - [复制一个删除相邻重复元素的序列](#复制一个删除相邻重复元素的序列)
  - [从序列中移除相邻的重复元素](#从序列中移除相邻的重复元素)
  - [旋转序列](#旋转序列)
  - [移动序列](#移动序列)
  - [从序列中移除元素](#从序列中移除元素)
  - [设置和修改序列中的元素](#设置和修改序列中的元素)
    - [用函数生成元素的值](#用函数生成元素的值)
    - [转换序列](#转换序列)
    - [替换序列的元素](#替换序列的元素)
- [Chapter 8 : 生成随机数](#chapter-8--生成随机数)
  - [什么是随机数](#什么是随机数)
  - [概率、分布以及熵](#概率分布以及熵)
  - [用 STL 生成随机数](#用-stl-生成随机数)
    - [生成随机数的种子](#生成随机数的种子)
    - [获取随机种子](#获取随机种子)
# Chapter 1: intruction to STL
STL为一个功能强大且可扩展的工具集,用于组织和处理数据,STL可以划分为四个概念库:

1. 容器库
2. 迭代器库
3. 算法库
4. 数值库

## 模板
__模板并不是可执行的代码, 而是用于生成代码的蓝图或者配方__.在程序中, 如果一个模板从来没有被实例化过, 那么它将被编译器忽略.当模板的实例化类型存在歧义的时候, 可以显式指定消除多义性.(一般而言, 定义类模板的时候喝定义其他类一样, 需要拷贝构造,移动构造,拷贝赋值运算符,移动赋值运算符以及析构函数五个成员)  
类的模板类型参数需要一个实参,除非有默认实参.实例化一个模板的时候, 发生了三个事情:
1. 确定了参数类型
2. 生成了构造函数的定义
3. 析构函数也被生成用来销毁对象

## 容器
STL 提供了下面的三种容器:
1. 顺序容器: 用线性的方式组织对象, 但是可能不需要连续的存储空间
2. 关联容器: 可以用 key 去得到对应的 value
3. 容器适配器: 提供了替换机制的适配器模板, 可以用来访问基础的序列容器或者关联容器

__容器在堆上存放对象__,并且自动管理他们所占用的内存. 默认的内存分配器(allocator)性能还是不错的, 在这里不讨论自定义的内存分配器相关的话题.  

## 迭代器
是一种类似于指针的模板类对象, 使用STL算法的时候, 迭代器将容器和算法联系在了一起, 通常使用一对迭代器表示一段数据(from begin to end),通常使用 _c.begin() / c.end()_ 或者 _std::begin(c) / std::end(c)_ 得到对象的两个典型迭代器. 
### 迭代器的类别
所有类型的迭代器都必须要有一个拷贝构造函数, 一个析构函数以及一个拷贝赋值运算符. 而迭代器指向的对象必须是可交换的(swappable).
1. 输入迭代器:提供对象的可读访问
2. 输出迭代器: 提供对象的只写访问
3. 正向迭代器: 结合了输入迭代器输出迭代器的功能
4. 双向迭代器: 在前者基础上增加了自减运算
5. 随机访问迭代器: 可以支持对元素的随机访问. 这意味着它支持以下操作:
   * 通过一个整数进行自增/自减 `iter+=n`, `iter-=n`...
   * 通过一个整数进行索引访问 `iter[n]` 意思是 `*(iter+n)`
   * 两个迭代器之间做差得到迭代器之间元素个数
   * 迭代器可以进行大小比较

这些迭代器之间通过 `iterator_tag`(一种 traits)标识, 可以通过它来指定算法的参数, 表明他们满足算法需要的最小条件. 
### 流迭代器
可以使用流迭代器, **在流和可以通过迭代器访问的数据之间参数文本模式的数据**. STL 算法可以通过指定一对迭代器来输入数据, 所有对于任何可以通过输入流访问的数据源, 都可以将算法运用到他们的元素上, 例如算法, 可以像运用到容器中的对象上那样运用到流中的对象上. 可以创建流迭代器对象, 来处理指定类型的流对象数据. `istream_iterator<T>` 这个输入流迭代器, 可以从文件流或者标准输入流`cin`, 输出流迭代器为对象提供了一个向输出流输出的功能. 下面这段代码就通过 accumulate() 函数, 计算 `cin` 流迭代器输入的一些数据的和:
```C++
std::cout << "\nThe sum of the values you entered is "
    << std::accumulate(std::istream_iterator<double>(std::cin),
    std::istream_iterator<double>(), 0.0)
    << std::endl;
```
### 迭代器适配器
这是一个类模板, 为标准的迭代器提供了一些特殊的行为, 使得他们能够从迭代器模板得到派生, 下面分别介绍这三种适配器模板.
#### 反向迭代器
工作机制和标准迭代器相反, 可以创建双向或者随机访问迭代器的反向迭代器:  
<div align=center><img src="https://i.imgur.com/V691ucM.png"/></div>

容器元素的反向迭代器相对于标准迭代器, 向左偏移了一个 Object. 每个反向迭代器都有一个成员函数 `base()`, 返回一个基础迭代器, 指向 rIter 的下一个位置:  
<div align=center><img src="https://i.imgur.com/Py38b3A.png"/></div>

#### 插入迭代器
一般的迭代器只能访问或者改变序列中存在的元素, **插入迭代器通常用于在容器的任何位置添加新的元素, 不能作用在`array<T,n>`这种 fixed-size 容器上**:
* 向后插入迭代器: 通过调用成员函数`push_back()` 将一个新的元素添加到容器的尾部, 如果容器没有定义 `push_back()` 函数,向后插入迭代器将无法使用(`vector, list, deque` 都有这个函数)
* 向前插入迭代器: 通过调用成员函数`push_front()`, 将新元素添加到容器的头部, 同样的没有定义这种函数的话就无法使用(`list, forward_list, deque` 都有)
* 插入迭代器: 用于向任何有 `insert()` 函数的容器插入一个新的元素

#### 移动迭代器
移动迭代器是从普通迭代器创建的, 指向一定范围内的元素, 可用于将某个范围内类的对象移动到目标范围, 而不需要拷贝去移动. 

## 迭代器上的运算
迭代器头文件中定义了四个迭代器的运算模板
* `advance(iter, n)`, n 可以为负数
* `distance(iter1, iter2)`, 返回两个迭代器之间元素个数
* `next(iter,n)` `perv(iter, n)`, n 的默认值为1, iter 必须有双向迭代器功能

## 智能指针
智能指针是模仿原生指针的模板类, 大多情况下没有差别, 只有下面的两点:
* 只能保存堆上分配的内存的地址
* 不能做自增或者自减运算

### 使用`unique_ptr<T>`
指定一个对象并且独享所有权, 一般使用 `std::make_unique<T>()` 更高效地生成. 
```C++
size_t arrLen{10};
std::unique_ptr<int[]> ptrArr = std::make_unique<int[]>(arrLen);
// 这里可以使用 auto 做自动类型推导
```
使用时候需要注意, **不能以传值的方式将 `unique_ptr` 对象传入函数中**. 因为他们不支持拷贝, 必须用引用传递的方式, 同时, 作为函数的返回值, 必须以隐式移动运算的方式返回. 
#### 重置 `unique_ptr` 对象
当这种智能指针生命周期结束析构时, 所指向的对象也会被析构. 有一些方式可以改变这种行为
* `reset()` 可以析构它原来指向的对象, `unique_ptr<T>` 对象中的指针会被替换成为空指针或者指向新的对象, 不要将原来指向的一个对象的地址传给 reset,或者在已经包含`unique_ptr<T>`的地址创建一个新的智能指针`unique_ptr`, 这可以通过编译但是一定会让程序崩溃:
    ```C++
    auto pname = std::make_unique<std::string>("Algernon");
    pname.reset(new std::string{"Fred"});
    ```
* `release()`: 在不释放对象内存的情况下将 `unique_ptr<T>` 的内部原生指针设为空
* 可以使用 swap, 通过交换连个指针的方式交换对象:
    ```C++
    auto pn1 = std::make_unique<std::string>("Jack");
    auto pn2 = std::make_unique<std::string>("Jill");
    pn1.swap(pn2);
    ```

#### 比较和检查 `unique_ptr` 对象

STL 中的非成员模板函数定义了全套的比较运算符用于比较两个`unique_ptr<T>`对象或者 `unique_ptr<T>` 和空指针. 具体就是 `.get()` 得到的裸指针之间的比较. 此外, `unique_ptr<T>` 还能隐式类型转换成 boolean 类型.

### 使用 `shared_ptr<T>`
这种指针用引用计数的方式共享对象, 我们可以使用 `std::make_shared<T>` 更加高效地创建 `shared_ptr<T>` 对象, 也可以用一个定义好的指针来初始化另一个 `shared_ptr<T>` 指针(通过由其他指针 get() 的裸指针得到 `shared_ptr<T>` 可能导致程序的崩溃):
```C++
auto pdata = std::make_shared<double>(999.0); // Points to a double variable
std::shared_ptr<double> pdata2 {pdata};
```
**不能使用 `shared_ptr<T>` 保存一个默认生成的数组, 但是可以用于保存自己生成的 `array<T>` 或者 `vector<T>` 对象**:
```C++
auto shareArr = std::make_shared<int[]>(10);
//Process finished with exit code -1073740940 (0xC0000374)
```
#### 重置 `shared_ptr<T>`
将 `shared_ptr<T>` 对象设置为空指针, 地址设为空的同时, 引用计数也将 -1. 调用 `reset()` 也可以得到同样的效果, 同时可以让指针指向别的对象:
```C++
auto pname = std::make_shared<std::string>("Charles Dickens");
pname.reset(new std::string{"Jane Austen"});
```
#### 比较和检查 `shared_ptr<T>` 对象
同样的, 这个指针可以一作比较和转为 bool 指针, 此外还有两个新的成员函数:
* `unique()` , 如果指向对象的实例数目为 1, 返回 true
* `use_count()` 返回指向对象的实例的个数

### 使用 `weak_ptr<T>` 指针
`weak_ptr<T>` 对象只能从 `shared_ptr<T>` 或者已有的 `weak_ptr<T>` 对象创建,能做的事情优先, 不能用解引用的方式去访问其指向的对象, 但是可以有一些场景可以使用:
* 判断对象是否依然存在
    ```C++
    if(pwData.expired())
        std::cout << "Object no longer exists.\n";
    ```
* 可以使用`weak_ptr<T>` 对象创建一个 `shared_ptr<T>对象`: `std::shared_ptr<X> pNew {pwData.lock()};`

## 算法
提供一些可以用于计算和分析的函数, 运用于一些由一对迭代器指定的对象.**可以将算法运用到任何序列上, 只要能够提供特定的迭代器去访问**. 
* 非可变序列运算(Non-mutating sequence operations): 不改变值和顺序
* 可变序列运算(Mutating sequence operations): 改变序列元素中的值
* 排序/合并/关联运算: 使用时候改变序列中的顺序

## 将函数作为实参传入
有三种方式将一个函数作为实参传入另一个函数:
* 使用一个函数指针
* 传入一个函数对象
* 传入一个 lambda 表达式
  
### 函数对象
这被称为仿函数, 是一个重载了函数调用操作符`operator()(parameter list)` 的类对象, 提供一个比使用原生函数更高效将函数作为实参传入另一个函数的方式:
```C++
class Volume{
public:
    double operator()(double x, double y, double z) {return x*y*z; }
    double operator()(const Box& box){ 
        return box.getLength() * box.getWidth() * box.getHeight(); 
    }
};
Volume volume; // Create a functor
double room { volume(16, 12, 8.5) };
Box box{1.0, 2.0, 3.0};
double roomOfBox{ volume(box)};
```
### Lambda 表达式
用于定义匿名函数, 可以捕获他们作用域内的变量,然后使用他们, 函数的返回值为默认返回类型, 当然也可用在 return_type 中定义.   
<div align=center><img src="https://i.imgur.com/nnmOej2.jpg"/></div>

我们常常使用自动类型推导命名 lambda 表达式, 例如 `auto cube = [](double length)->double {return length* length * length; };`.
#### 将 lambda 表达式传给函数
对于编程人员,一个十分困难的问题就是**如何确定 lambda 表达式的类型**, 然后写入函数的参数列表, 简单的回答是定义一个函数模板, 让编译器特话后处理不同的 lambda 表达式:
```C++
template <typename ForwardIter, typename F>
void change(ForwardIter first, ForwardIter last, F fun){
    for(auto iter = first; iter != last; ++iter) // For each element in the range...
    *iter = fun(*iter); // ...apply the function to the object
}
int data[] {1, 2, 3, 4};
change(std::begin(data), std::end(data), [] (int value){ return value*value; });
```
在这种情况下, `fun` 就是一个函数模板,可以接受一个函数对象/一个函数指针或者一个 lambda 表达式.   
标准库中有一个模板类型`std::function<>` 对任意类型的函数指针的封装, 包含了参数类型和返回值, 这也就包含了 lambda 表达式, 可以写成一个:
```C++
std::function<double(double)> op { [] (double value) { return value*value*value; } };
std::function<double(double,double)> sizeOfRectangle 
        {[](double length, double width)->double {return length* width; }};
```
#### 捕获子句
lambda 的引入符不一定为空, 可以有捕获子句, 指定一些封闭范围内的变量, 使得他们可以在 lambda 表达式主体中使用, 如果这里没有任何参数的话, 那么只能去引用lambda 内部的变量, 这被称为无状态的 lambda 表达式.
* 方括号中加入 `=` , 表示用值的方式访问所有封闭范围内的自动变量, 只有加入 `mutable` 之后才能改变这些量副本的值
* 方括号中加入 `&`, 用引用的方式使用封闭范围中的变量, 都能改变且不需要 `mutable` 关键字
  

不能将 auto 用于任何使用了正在定义变量的变量的 lambda 表达式, auto 不能用于自引用. 为了捕获指定变量,只要在捕获子句中列出他们的名称, 用 `&` 的话是引用传递,如`[&, factor]` 表示其他所有变量都是按引用传递, factor 使用值, 而且要加 `mutable` 才能改变 lambda 表达式中 factor 的值.  

# Chapter 2: 使用序列容器
序列容器用线性的方式存取元素, 主要有五种标准的序列容器:
* `array<T,n>`: fixed-size array
* `vector<T>`: 可以动态扩容的容器, `push_back()`/ `pop_back()` 操作效率比较高
* `deque<T>`: 双端队列, 长度可变, 可以自动增长的序列, 两端添加/删除元素都比较搞笑
* `list<T>`: 双向链表
* `forward_list<T>`: 正向链表容器

## 使用 `array<T,n>` 容器
需要指定元素类型和个数, 和常规的数据使用没有太大区别, 相应的有两个优点:
* 可以使用 `at()`, 越界会抛出 `std::out_of_range` 异常
* 容器知道元素个数, 传入函数时候不需要指定 `size`

需要注意的是 n 必须为常量表达式(note: C++ 需要常量表达式,计算结果为常量的表达式, 以便声明数组边界/case 语句中的选择器/位域长度规范/枚举初始值设定项,常量表达式中唯一合法的操作有文本/枚举常量/声明为常量表达式初始化的常量的值 和 `sizeof`), 并且容器中的元素个数不能发生改变. 

### 访问元素
* 可以使用`opetator[]`访问, 这种实现没有做边界检查, 性能要好一些. 
* 使用 `at()` 访问, 当越界的时候抛出 `std::out_of_range` 异常
* 为了保持和其他容器的一致性操作, 也有成员函数 `empty()`(意义不大).  
* 函数模板 `std::get<n>()` 的实参必须是一个在编译期能确定的常量表达式, 提供了一个不需要在运行时检查, 但是能安全访问元素的方式(编译期检查, 别的容器无法编译期间检查大小, 故无法这么做)

### 使用数组容器的迭代器
模板定义了成员函数 `begin()` 和 `end()`, 分别返回首元素和最后元素后一个位置的迭代器. 我们一般使用 auto 做自动类型推导, 实际上迭代器被定义在容器类型种, 类型类似于这种: `std::array<unsigned int, 19>::iterator`. 最好不用成员函数, 而是用全局的 `std::begin()`, 因为它们是通用的,例如普通的数组无法使用成员函数, 但是可以使用全局的非成员函数获取迭代器.  
算法是独立于容器类型的, 对于任何具有指定类型迭代器的容器来说, 算法都可以运用到他们的元素上. 只想访问元素而不做修改时候, 最好使用函数`cbegin()` 和 `cend()`.  
### 比较数组容器
两个数组容器只要大小相同/保存的元素类型相同而且重载了比较云算符, 就可以支持比较操作:
```C++
std::array<double,4> these {1.0, 2.0, 3.0, 4.0};
std::array<double,4> those {1.0, 2.0, 3.0, 4.0};
std::array<double,4> them {1.0, 3.0, 3.0, 2.0};
if (these == those) std::cout << "these and those are equal." << std::endl;
if (those != them) std::cout << "those and them are not equal." << std::endl;
if (those < them) std::cout << "those are less than them." << std::endl;
if (them > those) std::cout << "them are greater than those." << std::endl;
```

## 使用 `vector<T>` 容器
`vectory<T>` 大小可以自动增长, 从而可以包含任意数量的元素, 因此只需要类型信息一个参数.
### 创建 `vector<T>`容器
一般情况下, 可以这样生成一个存放元素类型的vector容器: `std::vector<double> weightOfPeople;`,这时候没有元素也就没有分配空间, 我们可以使用成员函数 `reserve(n)`来指定内存分配保证至少可以容纳 n 个元素.  
当然也可以用初始化列表指定初值和元素个数:`std::vector<int> idOfStudents{ 1, 3, 4,5,6};`.   
还可以使用其构造函数来创建 vector 对象: `std::vector<double> vecOfZeros(10. 0.0);`. 这里需要注意的是, **使用小括号创建 vector 对象和使用大括号指定的初始化列表出来的结果是完全不同的**.  
使用元素类型相同的容器也可以初始化 vector 对象:
```C++
std::array<std::string, 5> words {"one", "two", "three", "four", "five"};
std::vector<std::string> words_copy {std::begin(words), std::end(words)};
// 如果使用移动的话, 原来的数组就会变成空的
std::vector<std::string> words_copy {std::make_move_iterator(std::begin(words)),
                                     std::make_move_iterator(std::end(words))};
```
### 容量和大小
vector 对象可以通过调用 `.size()`/`.capacity()` 来获取相应的大小和容量. 显然其大小不能超过容量, 当两者相等的时候, 向 vector 中插入元素就会导致更多的内存分配(例如 gcc 的实现是容量翻倍). `vector<T>` 对象的容量和大小的数据类型是`vector<T>::size_type`, 需要用变量保存的大小的时候最好使用自动类型推导.   
此外可以调用 `resize()` 来改变容量的大小,区别在于使用这个成员函数的话会导致元素个数的变化:
```C++
std::vector<int> values {1,2,3}; // 1 2 3 : size is 3
values.resize(5); // 1 2 3 0 0 : size is 5
values.resize(7, 99); // 1 2 3 0 0 99 99 : size is 7
values.resize(6); // 1 2 3 0 0 99 : size is 6
```

### 访问元素
* 使用方括号加索引为现有的元素设定值, 或者使用现有的值 :`vec[10] = 9u`;
* 上面的做法不能用于生成新的元素, 同时可能越界的时候通过 `.at()` 使用元素, 越界时候抛出`std::out_of_range` 异常
* `.front()`/`.back()` 返回一个引用, 可以出现在赋值运算符的左边
* `.data()` 返回一个指向数组的指针

### 使用 vector 容器的迭代器
vector 实现了所有可以返回迭代器的成员函数, 也可以调用全局的 `back_inserter()` 获取一个向后插入的迭代器, 因为没有实现 `push_front()`, 无法使用 front_insert_iterator:
```C++
std::vector<double> data {32.5, 30.1, 36.3, 40.0, 39.2};
// template< class InputIt, class OutputIt >
// OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
std::copy(std::istream_iterator<double>(std::cin), std::istream_iterator<double>(),
          std::back_inserter(data)); // 将流迭代器中的数据通过back_insert_iterator 写入 data
std::copy(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "));
```
这里我们做的就是两个事情, 第一个是从输入流中将数据写到 data, 然后再将 data 的数据写到 输出流中.  

### 向 vector 容器中添加元素
向容器中添加元素的唯一方式是使用它的成员函数.
#### 增加元素
可以使用`push_back()` 成员函数, 向末尾添加一个元素, 需要注意的是这个函数也可以使用右值引用参数, 通过转移语义来添加元素:
```C++
std::vector<std::string> words;
words.push_back(string("adiabatic")); // Move string("adiabatic") into the vector
```
还有一种更好的方式: `emplace_back()`, <font color=red>直接使用 _vector_ 中的元素构造函数所需要的参数直接在容器中构造对象</font>:
```C++
words.emplace_back("abstemious"); 
std::string str {"alleged"};
words.emplace_back(str, 2, 3); // Create string object corresponding to "leg" in place
```
#### 插入元素
* `emplace()` 这个成员函数接受的第一个参数是迭代器, 指定对象生成的位置, 新对象会插入到迭代器之后. 后面的参数都是作为插入元素构造函数的需要. 
    ```C++
    std::vector<std::string> words {"first", "second"};
    // Inserts string(5, 'A') as 2nd element
    auto iter = words.emplace(++std::begin(words), 5, 'A');
    ```
* `insert()` 可以在 vector 中插入一个或者多个元素, 元素会被插入到第一个参数所指定的迭代器的前面(如果是反向迭代器则是后面)
  * 插入第二个参数指定的单个元素, 新元素会在迭代器指定的位置插入, 相应的所有其他元素向后移动一位:
    ```C++
    std::vector<std::string> words {"one", "three", "eight"}; // Vector with 3 elements
    auto iter = words.insert(++std::begin(words), "two");
    ```
  * 插入第二个参数和第三个参数指定的序列:
    ```C++
    std::string more[] {"five", "six", "seven"}; // Array elements to be inserted
    iter = words.insert(--std::end(words), std::begin(more), std::end(more));
    ```
  * 在vector 的末尾插入元素, `iter = words.insert(std::end(words), "ten");`
  * 在插入点插入多个元素, 第二个参数指的是第三个参数插入的次数`iter = words.insert(std::cend(words)-1, 2, "nine");`
  * 在插入点插入初始化列表指定的元素:`iter = words.insert(std::end(words), { std::string {"twelve"},std::string {"thirteen"}});`

### 删除元素
同样的, 我们也只能通过容器的成员函数来删除元素.
* 通过 `clear()` 删除又有的元素
  ```C++
  std::vector<int> data(100, 99);
  data.clear();
  ```
* 通过成员函数`pop_back()` 删除最后一个元素
* 如果需要删除多余的容量, 使用 `shrink_to_fit()`, 如果生效之后, 现有的迭代器全部失效, 需要重新获取迭代器
* 使用`.erase()` 删除一个或者多个元素, 需要传入一个迭代器(删除一个)或者序列两端的迭代器(删除多个元素):`auto iter = data.erase(std::begin(data)+1, std::begin(data)+3);`意思就是删除 data+1 和 data+2 两个元素
* remove() 算法是由 algorithm 头文件中的模板生成, 可以删除匹配特定值得一段元素, **remove 是一个全局的函数, 因此严格说不能删除容器中的元素, 这里的做法是采用匹配元素的右边覆盖匹配元素的方式移除元素**:
  ```C++
  std::vector<std::string> words { "one", "none", "some", "all", "none", "most", "many"};
  auto iter = std::remove(std::begin(words), std::end(words), "none");
  ```
  <div align=center><img src="https://i.imgur.com/cwGd2Ka.png"/></div>

  如果需要删除剩下空的位置, 还是需要调用上面的.erase():
  ```C++
  words.erase(iter, std::end(words)); // Remove surplus elements, 两部合起来相当于下面一句
  words.erase(std::remove(std::begin(words), std::end(words), "none"), std::end(words));
  ```

### `vector<bool>` 容器
是模板的特例化, 通常一个 bool 只需要一个 Bit, 如果不特例化的化需要一个 Byte. 这个没有成员函数 `.data()`, 一些成员函数的行为也和一般模板的实例不同.在要用布尔值而且知道用多少的时候, 使用`bitset<N>` 相对是个好的选择.

## 使用 `deque<T>` 容器
当应用包含先入先出的事务处理时, 都应该使用 deque, 这种容器可以在头部和尾部高效地添加或者删除对象.   
<div align=center><img src="https://i.imgur.com/fZvEYpe.png"/></div>

### 生成 deque 容器
* 默认构造生成 `deque<T>` 容器没有任何元素, 添加第一个元素之后才会导致内存的分配, 这点和 vector 是一样的. 
* 也可以指定元素个数生成容器(`std::deque<int> my_deque(10);`,这种定义之下每个元素保存的都是默认值0)
* 之外可以用初始化列表来生成容器: `std::deque<std::string> words { "one", "none", "some", "all", "none", "most", "many" };`
* 或者从其他deque容器生成一个新的副本
  * 从已经有的容器中调用构造器:`std::deque<std::string> words_copy { words };` 
  * 接受两个迭代器生成新的 deque: `std::deque<std::string> words_part { std::begin(words), std::begin(words) + 5 };`

### 访问元素
* 可以使用下标运算符访问元素, 同时这种操作没有做边界检查
* 同样可以使用带有边界检查并且可以抛出异常的`.at()`成员函数
* 组织元素的方式导致大小和容量总是一样的, 因此没有 capacity 成员函数
* 元素并不是存放在一个数组中故没有返回 T* 的 `.data()` 成员函数
* resize() 三种重载版本基本和vector 相同
* front/back 用法也相同, 返回一个头/尾巴的引用
  
### 添加和移出元素

* push/pop/emplace 除了 back 之外, 还有 front 版本, 在序列头部执行类似的操作
* insert/emplace 同样也可以在 deque 中使用, erase/ clear 也一样
* 由于内部实现不同, 除了在头部的操作效率比vector 高, 其他过程都要相对慢一些

### 替换 deque 中的内容
`.assign()` 成员函数可以替换现有的所有函数, 有三个重载的版本:
* 可以由初始化列表指定的元素
* 可以是迭代器指定的一段元素
* 可以是特定对象的多个副本

```C++
//case 1:
std::deque<std::string> words {"one", "two", "three", "four"};
auto init_list = {std::string{"seven"}, std::string{"eight"}, std::string{"nine"}};
words.assign(init_list);
// case 2: 
std::vector<std::string> wordset {"this", "that", "these", "those"};
words.assign(std::begin(wordset)+1, --std::end(wordset)); // Assigns "that" and "these"
// case 3:
words.assign(8, "eight"); // Assign eight instances of string("eight")
```
值得注意的是 case 1 中需要用 `std::string` 生成 string 对象, init_list 被推导为 `initializer_list<const char*>` 后面assign 就会报错, 当然可以不单独定义 init_list 直接将初始化列表放入参数中(`words.assign({"seven", "eight", "nine"});`). 

## 使用 `list<T>` 容器
这是一个包含 T 类型对象的双向链表, 可以在常规时间内在序列已知的任何位置插入或者删除元素, 但是**不能索引元素**, 如果需要访问内部的一个元素, 必须从头/尾 开始遍历:  
<div align=center><img src="https://i.imgur.com/C0KRxix.png"/></div>

可以和其他容器一样获得list的迭代器, 但是**无法随机访问list中的元素,故获取的都是双向迭代器**.
### 生成 list 容器
list的构造函数用法类似于 vector 或者 deque. 
* `std::list<std::string> words;` 生成空的list
* `std::list<std::string> sayings (20,"hello");` 生成给定数量个默认对象的列表
* `std::list<double> save_values {values};` 复制 value
* `std::list<double> samples {++cbegin(values), --cend(values)};` 从两个迭代器指定的一段元素生成

当然这个是双向迭代器而不是随机访问迭代器, 只能自增/减而不能加减一个整数.

### 添加元素
同样可以使用成员函数 `push_front()`/`push_back()`, 这两个函数都有右值引用版本, 而相对而言成员函数`emplace_front()`/`emplace_back()`可以做得更好(它们是在容器中之间构造元素, 而不是构造完之后拷贝/搬移过去):
```C++
std::list<std::string> names {"Jane", "Jim", "Jules", "Janet"};
names.emplace_front("Ian"); // Create string("Ian") in place at the front of the list
```
同样的insert有三个版本:
* 在指定迭代器的位置插入一个元素
* 在给定的位置插入几个相同的元素
* 由给定的两个迭代器, 往指定位置插入一段元素

### 删除元素
* clear() 和 erase() 这两个成员函数的作用和前面两种容器相当.
* 有成员函数 `remove()` 移除和参数相匹配的元素
  ```C++
  std::list<int> numbers { 2, 5, 2, 3, 6, 7, 8, 2, 9};
  numbers.remove(2); // List is now 5 3 6 7 8 9
  ```
* `remove_if()` 引入一个一元断言为参数, 接受一个和元素同类型的参数或者引用, 返回一个布尔值
  ```C++
  numbers.remove_if([](int n){return n%2 == 0;}); // Remove even numbers. Result 5 3 7 9, 如果lambda 表达式返回 true 就需要删除
  // 全局的非成员 remove_if, example from cppreference.com
  std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";
  str2.erase(std::remove_if(str2.begin(),
                            str2.end(),
                            [](unsigned char x){return std::isspace(x);}),
             str2.end());
  ```
* 成员函数 `unique()` 移除连续的重复元素, 只留下其中的一个
  
### 排序和合并元素
全局的sort() 函数模板位于 algorithm 中, 要求使用随机访问迭代器, 但是 list 并不支持. **list模板定义了自己的 sort() 函数**, 它有两个版本:
* 无参版本按照升序排列所有元素
* 接受一个参数(函数对象或者 lambda 表达式) , 用参数定义的断言来比较两个元素
  
```C++
names.sort(std::greater<>()); // Function object uses perfect forwarding
```
`merge()`成员函数接受另一个相同类型的 list 作为参数, 两个容器中所有的元素都必须为升序, 参数 List 中所有元素会被合并到当前的 list 中. 这种操作之后 List 节点在内存中的位置不变, 但是操作他们的指针变了.   
list 容器的 `splice()` 有几个版本, 将参数 list 容器中的元素移动到当前容器中指定元素的前面.它可以移动单个元素/一段元素或者源容器的全部元素. 

### 访问元素

list 的成员函数 `front()`/`back()` 可以返回第一个元素和最后一个元素的引用, 同时可以通过第迭代器的自增/减来访问list 内部的元素. 获取迭代器的方式也是相同的, `begin()/end()/rbegin()/rend()`. 



## 使用`forward_list<T>` 容器

这种容器是以单向链表的形式存储元素, 和 `list` 的主要区别在于 **它不能反向遍历元素, 只能从头到尾遍历**. 这意味着有很多东西无法使用:

* 首先无法使用反向迭代器 

* 无法使用 `back()`, 只有成员函数 `begin()`

* `push/pop/emplace_back()` 也无法使用

* 没有成员函数 `size()`,无法用一个向前迭代器减去另一个, 但是可以通过定义在头文件 iterator 中的 `distance()` 得到元素的个数

  ```c++
  std::forward_list<std::string> my_words {"three", "six", "eight"};
  auto count = std::distance(std::begin(my_words), std::end(my_words));
  ```

* `forward_list<T>` 是正向链接元素, 故只能在元素后面插入, `splice_after()`/`insert_after()` 用来替代 list 容器中的 `splice()`/`insert()` 

* 但是需要在表头的时候还是会有问题, 使用成员函数 `cbefore_begin()`/`before_begin()` 来解决

* 和list 容器一样, 它有 sort()/merge()/remove()/remove_if()和 unique(), 用法也是一样的

## 自定义迭代器

STL 对定义了迭代器类的类型有一些特定的要求, **这是为了保证所有接受这种迭代器的算法都能正常工作**.  
使用迭代器模板的时候经常会出现一个问题, 就是我们通常不知道迭代器容器的数据类型, 于是我们就可以使用 `value_type` 的别名来指定类型:
```C++
template <typename Iter> void my_swap(Iter a, Iter b)
{
  typename Iter::value_type tmp = *a; // Better - but has limitations...
  *a = *b;
  *b = tmp;
}
```
但是, 问题又出现了:**算法既可以使用指针, 也可以使用迭代器, 而指针不是类, 无法包含定义的别名**. 就是说上面的代码 Iter 为迭代器的时候能用, 传入一个指针的时候, `int*::value_type` 是没有意义的.  STL 用模板优雅地解决了这个问题.  
`iterator_traits`模版的定义如下:
```C++
template<class Iterator>
struct iterator_traits
{
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};
``` 
上面的别名十分直观, 一个满足 STL 要求的迭代器必须全部定义这些别名, 但是对于输入迭代器, 除了 `iterator_category`,其他都可以定义为 void. 于是我们往其中传入一个迭代器, 如 `std::iterator_traits<MyIter>::value_type`, 就萃取出了元素的类型. 
```C++
template <typename Iter>
void my_swap(Iter a, Iter b)
{
    typename std::iterator_traits<Iter>::value_type tmp = *a;
    *a = *b;
    *b = tmp;
}
std::vector<std::string> words {"one", "two", "three"};
my_swap(std::begin(words), std::begin(words)+1); // Swap first two element
```
现在我们来看上面的代码发生了什么, 传入 word 的一个迭代器之后, 确定 tmp 的类型为 `iterator_traits<iterator<std::string>>::value_type`, 它实际上是 `iterator<std::string>::value_type` 的一个别名, `iterator<std::string>` 是从迭代器的模版产生的, 于是它包含了value_type 的定义:
```C++
typedef std::string value_type;
```
当传入的参数为指针 `T*` 的时候, 我们给出其特化的版本,这种专门针对指针类型的特化是符合语法的:
```C++
template<class T>
struct iterator_traits<T*>
{
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_access_iterator_tag iterator_category;
};
```
STL 定义了迭代器模版, 于是我们可以从中生成我们自己的迭代器:
```C++
template<class Category, class T, class Difference = ptrdiff_t,
        class Pointer = T*,class Reference = T&>
struct iterator
{
    typedef T value_type;
    typedef Difference difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Category iterator_category
};
class My_Iterator : public std::iterator<std::random_access_iterator_tag, int>
{
    // Members of the iterator class...
};
```
此外自定义的迭代器有了上面这一套别名后, 还需要成员函数.
* 根据经验,如果迭代器定义了一个Big-3 中的任意一个函数, 就需要显式定义一个析构函数
* 对于随机访问迭代器类, 需要一整套关系运算符,当然可以使用utility 中的标准库头文件中的模版来完成这些定义, 如果我们实现的同时还包含了下面的代码, 那么我们的实现优先级更高, 这些操作符对搜索和比较操作很重要 
  ```C++
  #include <utility>
  using namespace std::rel_ops;
  ```
* 迭代器的其他操作由它的类别决定


# Chapter 3: 容器适配器

容器适配器是一个封装了序列容器的类模板, 它 **在一般序列容器的基础上提供了一些别的功能**.适配器的含义就是**通过适配容器现有的接口来提供一些不同的功能**, 这里有三种容器适配器:

* `stack<T>` 封装了一个 `deque<T>` 容器的适配器类模板, 默认一个 LIFO
* `queue<T>` 封装了一个 `deque<T>` , 默认实现一个 FIFO 的队列
* `priority_queue<T>` 封装了`vector<T>` 容器的适配器类模板, 默认的实现是一个会对元素排序, 从而最大元素总是在队列最前面的队列

## 创建和使用 `stack<T>` 容器适配器

<div align=center><img src="https://i.imgur.com/nP86yM6.png"width="40%"/></div> 

`stack<T>` 容器适配器模板有两个参数, 第一个 T 存储元素的类型, 第二个是使用的底层容器的类型, 只要支持`back() push_back() pop_back() empty() size()` 这些操作,默认是 `deque<T>`.例如我们可以使用一个 list 作为底层的容器:

```C++
std::stack< std::string, std::list<std::string>> fruit;
```

值得注意的是, **[不能在初始化列表中用对象来初始化容器适配器, 但是可以用一个容器来初始化, 只要底层容器的类型和用于初始化容器类型一致](https://stackoverflow.com/questions/10293302/why-cant-i-construct-a-queue-stack-with-brace-enclosed-initializer-lists-c1)**.这个问题是因为适配器中没有对应与 initializer_list 匹配的构造函数, 并不是语言特性本身带来的问题, 如果自己定义一个适配器并且带有初始化列表的构造函数, 就可以使用. 

```C++
std::list<double> values {1.414, 3.14159265, 2.71828};
std::stack<double, std::list<double>> my_stack (values);
// 适配器的底层的容器 list<double> 和用于构造的对象容器类型一致
```

`stack<T>` 模板定义了拷贝构造函数, 使用的时候可以用圆括号和初始化列表.

```C++
std::stack<double, std::list<double>> copy_stack {my_stack};
std::stack<double, std::list<double>> copy_stack(my_stack);
```

### stack 操作

* `top()`: 返回栈顶元素的引用
* `push(const T& obj) / push(T&& obj)` 压入栈顶, 通过调用底层容器的 `push_back()` 实现
* `pop()` 弹出栈顶元素
* `size()/ empty()/ emplace()` 从字面就能理解其意义
* `swap(stack<T> & other_stack)` 当前栈中的元素和参数中的交换(两者的类型必须相同), 同时全局的 std::swap 也有对 stack 特化的版本

此外, stack 对象有一套比较运算符, 通过字典的方式来比较. 

## 创建和使用 `queue<T>`容器适配器

只能访问`queue<T>`容器适配器的第一个和最后一个元素. **只能在头部添加元素, 在尾部移除元素**.   
<div align=center><img src="https://i.imgur.com/wVEIcKd.png"/></div>

同样的, 我们也可以通过第二个参数指定底层的容器类型, 只要它能提供以下类型的操作:`front(), back(), push_back(), pop_front, empty() , size()`. 
### queue 的操作
* `front()`/`back()` 同样的返回第一个元素最后一个元素的引用, 如果 queue 是常量则返回常引用
* `push(const T&Obj)`/`push(T && Obj)` 在队尾添加一个元素(通过调用底层容器的 `push_back()` 实现)
* `pop()`/`size()`/`empty()` 这些操作就是按照字面理解
* `emplace()` 用传给的参数调用 T 的构造函数, 在尾部生成对象
* `swap(queue<T> &otherQueue)` 交换两者的元素, 同样的可以使用全局的 swap. 
* 和 stack 一样, 有一套比较运算符.

## 使用优先队列
`priority_queue<T>` 容器适配器定义了一个元素有序排列的队列, 默认**队列头部的元素优先级最高**. 这个处理过程有点类似于急救室抢救病人, 它并不总是先到先处理. 适配器模板有三个参数, 存储对象类型, 底层容器类型(默认是一个向量) 和 一个函数对象(用于决定元素的顺序). 
<div align=center><img src="https://i.imgur.com/w23Xiwf.png"/></div>

### 创建优先队列
**初始化列表中的序列可以来自任何容器，并且不需要有序**。 优先队列会对他们进行排序:
```C++
std::string wrds[] {"one", "two", "three", "four"};
std::priority_queue<std::string> words {std::begin(wrds),
                                        std::end(wrds)}; // "two" "three" "one "four"
//在使用中可以指定用于排序的函数对象和底层的容器类型
std::string wrds[] {"one", "two", "three", "four"};
std::priority_queue<std::string, std::deque<std::string>> words {std::begin(wrds), std::end(wrds)};
std::vector<int> values{21, 22, 12, 3, 24, 54, 56};
std::priority_queue<int> numbers {std::less<int>(), values};// 在列表初始化的第一个位置指定函数对象
std::priority_queue<int, std::vector<int>, std::greater<int>> // 还可以在模板参数中写出
                    numbers1 {std::greater<int>(), values};
```

### 优先队列的操作
* `push(const T& obj)/push(const T&& obj)`: 将元素副本/元素 放到适当的位置
* `emplace(T constructor args...)` 
* `top()` 返回优先队列中第一个元素的引用
* `pop()` 移除第一个元素
* `size()` 返回队列钟元素的个数
* `empty()` 判断队列是否为空
* `swap(priority_queue<T>& other)` 和参数进行元素互换, 所包含的元素类型必须相同

```C++
std::priority_queue<std::string> words;
std::string word;
std::cout << "Enter words separated by spaces, enter Ctrl+Z on a separate line to end:\n";
while (true)
{
  if ((std::cin >> word).eof())
  break;
  words.push(word);
}
std::priority_queue<std::string> words_copy {words}; // A copy for output
while (!words_copy.empty())
{
  std::cout << words_copy.top() << " ";
  words_copy.pop();
}
std::cout << std::endl;
```
## 堆
**堆不是容器, 而是一种特别的数据组织方式**.完全二叉树可以用数组的方式保存, 也可以用其他顺序表的方式储存.
### 创建堆
创建堆的函数在 `<algorithm>` 中, `make_heap` 对随机访问迭代器指定的一段元素重新排列, 生成一个堆. 
```C++
std::vector<double> numbers { 2.5, 10.0, 3.5, 6.5, 8.0, 12.0, 1.5, 6.0 };
std::make_heap(std::begin(numbers), std::end(numbers)); // Result: 12 10 3.5 6.5 8 2.5 1.5 6
```
优先队列是一个堆, 底层一个优先队列的实例创建了一个堆, 并且可以提供堆没有的优势: 
* 它可以自动保持元素的顺序
* 除了第一个元素, 我们无法访问到其他元素

因此, 相反在很多场合, 这的反面就成了堆的优势. 

### 堆操作
* `make_heap(iterBegin, iterEnd)`: 从哟个随机访问迭代器中生成堆
* `push_heap(iterBegin, iterEnd)` 用于 `push_back(someElement)` 之后, 会认为只有最后一个是新元素, 在此基础上重新排列元素使得其复合堆条件
* `pop_heap(iterBegin, iterEnd, std::greater<>())`: 将第一个元素移除之后, 并保证剩下的依旧是一个堆
* `is_heap()/is_heap_until()` 检查迭代器区间中的元素排列是否构成一个堆, 后者是检查第一个不构成堆的元素
* `sort_heap()` 将元素段作为堆来排序, 如果不是堆, 就会在运行时崩溃
> 我们可以直接使用 _algorithm_ 头文件中的函数模板 `sort` 排序, 但是 sort_heap 是充分利用堆的局部有序性可以使得排序变得更快
## 在容器中保存指针
在容器中存在智能指针比存放对象要好, 保存智能指针比保存原生指针要好. 我们在使用的时候常常是用临时对象调用 `push_back()` 的右值引用版本, 或者使用 `std::move()` 将智能指针移动到容器中. 
# Chapter 4: map 容器
`map` 容器是关联容器的一种, 在关联容器中**对象的位置取决于和它关联的键值**, 键可以是基本类型也可以是类类型. map 容器中的元素是 `pair<const Key, T>` 类型的对象, 每种map 模板都有自己的特性:  
|           | rb_tree | hash_map |
|:-----------:|:---------:| :---------:|
|允许重复键  | multimap|unodeared_multimap|
|不允许重复键|map|unordered_map|

## map 容器的用法
STL map 容器对于元素的组织方式并没有具体的要求, 但元素一般都会保存在一个平衡二叉树中(通常是rb_tree). 
### 创建 map 容器
map 模板有四个类型参数, 分别是 key 类型, value 类型, 用于比较 Key 大小的函数对象以及底层的内存分配器类型. 我们可以使用初始化列表来对比两个对象:
```C++
std::map<std::string, size_t> people{ std::make_pair("Ann", 25), std::make_pair("Bill", 46),
                                      std::make_pair("Jack", 32), std::make_pair("Jill", 32) }
```
当然, 不用 `make_pair` 也可以直接使用key-value, 如 `{"Ann",25}`.  
`map<K,T>` 中也定义了移动和复制构造函数, 用于从现有的容器/容器的迭代器构造.
### map 元素的插入
`map<K,T>` 容器的成员函数 `insert()` 有多个版本, 可以在容器中插入一个或者多个 `pair<const key, value>` 对象. 
```C++
std::map<std::string, size_t> people {std::make_pair("Ann", 25), std::make_pair("Bill", 46),
                                      std::make_pair("Jack", 32), std::make_pair("Jill", 32)};
auto pr = std::make_pair("Fred", 22); // Create a pair element...
auto ret_pr = people.insert(pr); // ..and insert it
std::cout << ret_pr.first->first << " " << ret_pr.first->second
<< " " << std::boolalpha << ret_pr.second << " \n"; // Fred 22 true
```
需要做一些解释:
* `make_pair` 没有带模板特化, 是相信编译器可以推导出来, 推导出来的 `<const char*,int>` 可以隐式类型转换成我们需要的类型
* map 的 `insert` 函数返回的是一个 pair 对象, 具体而言是 `pair<iterator, bool>`, 其成员 first 是一个迭代器指向插入元素(成功)或者阻止插入的元素(失败)

我们也可以提供一个提示符来指出元素插入的位置, 提示符是迭代器的形式, 通常是从提示符指示的位置开始查找新元素的插入位置, 好的提示符可以提高插入操作的速度.
```C++
auto ret_pr = people.insert(std::make_pair("Jim", 48));
people.insert(ret_pr.first, std::make_pair("Ian", 38));// 从上一个元素插入的位置开始查找新的插入位置
```
* `map` 中的成员函数 `count()` 返回map 中键对应元素的数目, 可以为我们的插入操作提供一些帮助
* `insert` 借助迭代器和初始化列表可以一次性插入多个元素
    ```C++
    std::map<std::string, size_t> crowd {{"May", 55}, {"Pat", 66}, {"Al", 22}, {"Ben", 44}};
    auto iter = std::begin(people);
    std::advance(iter, 4); // begin iterator+ 4
    crowd.insert(++std::begin(people), iter); // 使用迭代器, Insert 2nd, 3rd, and 4th elements from people
    crowd.insert({{"Bert", 44}, {"Ellen", 99}});// 使用初始化列表
    ```
    * 需要注意的是初始化列表生成的对象是 `initializer_list<const string,size_t>`类型, 并且该函数的参数必须是 const 类型, 因为 non-const 到 const 没有隐式类型转换

除了使用 insert 插入之外, 还可以使用 `emplace()`/`emplace_hint()` 构造并且插入元素, 生成元素的本质是一样的, 但我们必须为后者提供一个指示生成位置的迭代器作为第一个参数.此外他们有不同的返回值, 后者返回的不是一个 pair 而只是一个迭代器.
```C++
std::map<Name, size_t> people;
auto pr = people.emplace(Name{"Dan", "Druff"}, 77);
auto iter = people.emplace_hint(pr.first, Name{"Cal", "Cutta"}, 62);
```
### 访问 map 元素
* 可以获取 map 容器的开始/结束迭代器以及反向迭代器
* map 提供成员函数 `at(Key)` 返回参数对应的对象, Key 不存在则抛出 `out_of_range` 异常
* map 提供以键为参数的的下标运算符`some[key]`, 返回一个和键所关联对象的引用(如果键不存在, 则会插入一个 value 为默认值的新对象)

### 删除元素
* `erase()` 用于删除键和参数匹配的元素(可以接受一个 key 或者接受两个迭代器)
  ```C++
  std::map<std::string, size_t> people{{"eric",42}, {"Jhon",16}};
  std::string name{"eric"};
  people.erase(name);
  ```
* `clear()` 用于删除容器中的所有元素

## `pair<>` & `tuple<>` 的用法
一般来说, pair 可以封装一对不同的任意类型的对象, 例如可以封装两个序列容器或者两个序列容器的指针, 在 _\<utility>_ 头文件中定义. `tuple<>` 是 `pair` 的返化, 可以封装不同类型的任意数量的对象, 定义在 _\<tuple>_ 头文件中. 
### pair 的操作
pair 是比较简单的数据类型, 只有两个 public 数据成员 `first`/`second`. 和右值引用一样, 可以有各种版本的引用参数:
```C++
std::string s1 {"test"}, s2{"that"};
std::pair<std::string, std::string> my_pair{s1, s2};
std::pair<std::string, std::string> your_pair{std::string {"test"}, std::string {"that"}};
std::pair<std::string, std::string> his_pair{"test", std::string {"that"}};
std::pair<std::string, std::string> her_pair{"test", "that"};
```
上面四个版本涵盖了各种类型
* 第一个赋值参数
* 第二个移动参数
* 第三个第四个包含了从 `const char*` 到 `std::string` 的隐式类型转换

此外, 我们可以使用辅助函数 `std::make_pair<T1,T2>` 生成并且返回一个 _pair_ 对象, 函数模板的参数值可以手动指定也可以由编译器推断.   
pair 对象有全套的运算符 `==`,`!=`,`<`,`<=`,`>`,`>=`, 作为操作数的 pair 类型必须是相同的， 他们的成员变量比较的方式也必须相同。  

### tuple 的操作
使用 `<tuple>` 中定义的函数 `make_tuple()` 可以生成 tuple, tuple 是 pair 的泛化， 工作方式稍有不同：数目不固定， 因此用 `get<>()` 返回某个元素。
```C++
auto my_tuple = std::make_tuple(Name{"Peter", "Piper"}, 42, std::string{"914 626 7890"});
std::cout << std::get<0>(my_tuple)
          << " age = " << std::get<1>(my_tuple)
          << " tel: " << std::get<2>(my_tuple) << std::endl;
// get<> 的尖括号中也可以放类型， 如果该类型的元素只有一个
std::cout << std::get<Name>(my_tuple)
          << " age = " << std::get<int>(my_tuple)
          << " tel: " << std::get<std::string>(my_tuple) << std::endl;
```
全局的 `tie<>()` 函数模板提供了另一种访问 tuple 元素的方式（类似于结构化绑定）, 并且可以使用 `std::ignore` 忽略特定元素：
```C++
auto my_tuple = std::make_tuple(Name{"Peter", "Piper"}, 42, std::string{"914 626 7890"});
Name name{};
size_t age{};
std::string phone{};
std::tie(name, age, phone) = my_tuple;
std::tie(name, std::ignore, phone) = my_tuple;
```

## multimap 容器的用法
因为允许 Key 重复， multimap 使用和 map 有一些区别。 
* `.insert()` 可以插入一个或者多个元素， **插入总是成功**，并且返回指向插入元素的迭代器
* `.emplace()` 可以在适当位置构造元素
  * `.emplace_hint()` 可以提供一个迭代器形式的提示符， 控制元素的生成位置
* 不支持下标运算符（**因为 _key_ 不能唯一确定元素**）,可以使用 `find()`返回一个键和参数匹配的元素迭代器
  * `.equal_range(parameter)` 成员函数返回两个迭代器构成的 _pair_, 所确定的范围中键和参数的值相等
  * `.equal_range()` 的参数可以是不同类型**但是可以和键相互比较的对象**， 返回的 first 是指向第一个大于等于参数的元素（如果存在，需要检查是否等于尾后迭代器）， second 是键值大于参数的的第一个元素的迭代器
  * `.lower_bound()` 和 `.upper_bound()` 两个成员函数， 分别返回键值大于等于参数的第一个元素和键值大于参数的第一个元素 （不存在就返回尾后迭代器）
* `.count(some_key)` 成员函数可以知道有多少个元素的的键和指定的键值相等
* `.erase()` 成员函数有三个版本
  * 以单个迭代器作为参数(void)
  * 以键值作为参数，删除匹配的所有元素并且返回个数
  * 接受两个迭代器作为参数(返回指向被删除的最后一个元素的下一个位置的迭代器)

## 改变比较函数
map 容器的比较函数**在相等的时候不能返回 true**, 即不能使用 `>=`或者 `<=`, 因为 `key1< key2` 和 `key2 < key1` 都是 false 的时候便认为它们相等（`!(key1 <= key2)&&!(key2 <= key1) `）， 如果使用 `<=` 得到的结果就不符合预期。
### `greater<T>` 
如果某个累实现了 `operator>()`, 就可以传入 `greater<T>` 替代默认的比较函数（`less<T>`）:
```C++
// inside Name Class
bool operator>(const Name& name) const
{
 return second > name.second || (second == name.second && first > name.first);
}
// outside Name Class
std::map<Name, size_t, std::greater<Name>> people
 { {Name{"Al", "Bedo"}, 53}, {Name{"Woody", "Leave"}, 33}, {Name{"Noah", "Lot"}, 43} }
```
### 使用函数对象
如果键是指针， 我们不好在类内定义， 就可以使用**函数对象**(就是类型实现了 operator() 的对象)：
```C++
class Functor{
public:
    bool operator()(const std::unique_ptr<std::string>& first,
                    const std::unique_ptr<std::string>& second){
        return *first -> *second;
    }
}
std::map<std::unique_ptr<string>, std::string, Functor> phonebook;
```
实际上配合 auto 类型推导和 decltype， 理论上可以传入 lambda 表达式， 但是实际上使用时候编译器报错， 认为 lambda 表达式的构造函数默认是 delete的， 因此传入 `decltype(cmp)` 后无法构造出可调用对象：
```C++
int sz = 2;
auto a = [sz](){ return sz;};
decltype(a) b(10); //compiler error
decltype(a) b = a; //all good though
```
## 哈希
哈希是用给定范围的基本类型的数据项， 生成整数值的过程, 不仅可以在容器中保存对象， 也被用于密码和加密数据的安全系统中。哈希算法很多， 但却没有通用的。  
`<functional>` 头文件中定义了 `hash<K>` 模板，不同类型的专用算法取决于实现， C++14 标准需要满足下列具体条件：
* 不能抛出异常
* 对于相同的键值必须产生相等的哈希值
* 对于不相等的键产生碰撞的可能性必须最小接近 `numeric_limits<size_t>::max()` 的倒数
    ```C++
    std::hash<Box*> box_hash; // Box class is implemented elsewhere
    auto upbox = std::make_unique<Box>(1,2,3);
    std::cout << "hash val of box pointer is " << box_hash(upbox.get()) << std::endl;
    ```
## `unordered_map` 容器
`unordered_map` 包含的是有唯一键的键、值对元素， 元素不是有序的， 存放位置由哈希值确定。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220109165427.png" width="80%"/></div>  

* 容器的个子个数有默认值， 也可以手动指定
* 载入因子是每个格子平均保存的元素的个数，最大值默认是 1.0， 也可以修改

### 生成和管理`unordered_map`
生成 `unordered_map` 需要 key 可以实例化 `hash<K>`,并且必须能用 oprerator`==` 比较 key, 我们也可以指定格子个数：
* 可以用 initializer_list 生成
* 也可以使用两个指向 pair 的迭代器
    ```C++
    std::unordered_map<std::string, size_t> people 
        {{{"Jan", 44}, {"Jim", 33}, {"Joe", 99}}, 10};
    std::vector<std::pair<string, size_t>> folks {{"Jan", 44}, {"Jim", 33}, 
        {"Joe", 99}, {"Dan", 22}, {"Ann", 55}, {"Don", 77}}; 
    std::unordered_map<string, size_t> neighbors 
        {std::begin(folks), std::end(folks), 500};
    ```
如果是自定义类型， 我们需要实现 hash 和 operator == : 
```C++
class Name {
// Private and public members and friends as in Ex4_01...
public: 
    size_t hash() const { 
        return std::hash<std::string>()(first+second); 
    } 
    bool operator==(const Name& name) const { 
        return first == name.first && second== name.second; 
    } 
};
```
`unordered_map` 的哈希函数只能接受和键同类型的单个参数， 返回一个 size_t 值， 而上面实现的是一个成员函数版本的hash, 因此需要定义一个仿函数然后传入（如果我们定义了一个判断相等的函数对象， 也可以选择传入）：
```C++
class Hash_Name {
public: 
    size_t operator()(const Name& name) const { return name.hash(); } 
};
std::unordered_map<Name, size_t, Hash_Name> people 
    {{{{"Ann", "Ounce"}, 25}, {{"Bill", "Bao"}, 46}, {{"Jack", "Sprat"}, 77}}, 
    500,                    // Bucket count 
    Hash_Name()};           // Hash function for keys
```

### 调整格子个数
如果载入因子超过最大值， 容器将增加格子， 元素值会被再次哈希，这时候迭代器会失效， 我们也可以手动改变格子个数或者载入因子：
```C++
people.rehash(15);   // Make bucket count 15
people.max_load_factor(1.2*people.max_load_factor());      // Increse max load factor by 20%
```
也可以给出一个元素个数大小， 使得个数不超过的时候负载因子始终维持在最大数之内：
```C++
size_t max_element_count {100}; 
people.reserve(max_element_count);
```

### 插入元素
`unordered_map` 的成员函数 `.insert()` 提供的能力和 map 容器相同
* 通过复制或者移动来插入一个元素
* 使用或者不使用提示符来指明插入位置
* 插入多个元素的版本（如接受初始化列表、两个迭代器的版本）没有返回值
* `.emplace()`/`.emplace_hint()` 在适当的位置生成元素，后者的第一个参数是提示位置的迭代器

### 访问元素
* 通过下标运算符`[]` 获得对应 val 的引用
* `.at()`功能类似， 但是 key 不存在时候抛出 `std::out_of_range`
* `find()`/`euqal_range()` 工作方式和 map 一样
* 迭代器可以使用

### 移除元素
使用`.erase()` 成员函数来移除元素
* 接收一个 key 值： 返回一个整数（移除的元素个数）
* 接收一个迭代器： 返回删除元素后一个位置的迭代器
* 接收两个迭代器， 删除特定范围： 返回值同上

### 访问格子
使用容器的成员函数 `.begin()`/`.end()` 的重载版本获取指向格子的迭代器，如 `some_unordered_map.begin(1)` 返回第二个篮子的迭代器。 同样的，`.cbegin()` 重载版本返回 const 迭代器。  
* `.bucket_conut()` 返回格子的个数  
* `.bucket_size()` 返回参数指定的格子的元素个数
* `.bucket()` 返回的是格子的索引值  
    ```C++
    string key {"May"}; 
    if(people.find(key) != std::end(people)) 
        std:: cout << "The number of elements in the bucket containing " 
        << key << " is " << people.bucket_size(people.bucket(key)) << std::endl;
    ```

### unordered_multimap
是一个允许有重复键的无序map,和 unordered_map 的 `.insert()`/`.emplace()`/`.emplace_hint()` 很像， 区别在于使用前两个， 会返回一个 `pair` 对象说明插入是否成功。  
`.at()` 和 operator[] 不再可用， 只能选择
* `find()`: 容器中只有一个 key 
* `equal_range()`: 访问一段元素

# Chapter 5: set 容器
| 底层数据结构          | rb_tree | hash_table |
|:-----------:|:---------:| :---------:|
|允许重复键  | multiset|unodeared_multiset|
|不允许重复键|set|unordered_set|

set/multiset 默认使用的比较函数是 `less<T>`, unordered_set/unordered_multiset 默认使用 `equal_to<T>` 来判断元素是否相等。 
* 我们可以从有序 set 得到正向和反向迭代器
* 无序容器只能得到正向迭代器

## 使用 `set<T>` 容器
* 可以用初始化列表来初始化 set 容器
    ```C++
    std::set<int> numbers{3,7,9,6,5,1};
    ```
* 可以提供默认之外的比较函数
    ```C++
    std::set<std::string, std::greater<string>> words 
        {"one", "two", "three", "four", "five", "six", "seven", "eight"};
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220109230752.png" width="40%"/></div>
* 也可以使用两个迭代器来生成 set

### 添加和移除元素
set 中没有实现成员函数`.at()`, 也没有实现 `operator[]()`, 除此之外实现了大部分操作：
* `.insert()`:
  * 插入单个元素返回 `pair<iterator,bool>` 对象
  * 插入一段元素或者初始化列表不会有返回值
* `.emplace()`:
  * 返回一个 `pair<iterator, bool>`
* `.emplace_hint()`:第一个参数是迭代器，随后的参数会传入构造器
* `.clear()` 会删除所有元素
* `.erase()`: 删除迭代器指定位置的元素或者与对象匹配的元素

### 访问元素
* `.find()`: 返回第一个和参数匹配的迭代器
* `.count()`:返回元素在 set 中的个数， 0或者 1，（因为元素唯一）
* `.equal_range()`/`.lower_bound()`/`.upper_bound()` 和 multiset 容器很大程度一致

### set 迭代器
`set<T>` 容器的成员返回的迭代器都是双向迭代器， 有的类型包括 iterator, reverse_iterator, const_iterator 和 const_reverse_iterator。实际上 `set<T>` 返回的迭代器都是 const T 类型， 意味着我们**不能直接修改set元素**。如果想要修改元素， 必须要先删除它然后再插入修改后的版本。  

### 在 set 容器中保存指针
在 set 中存放 `shared_ptr<T>` 或者 `weak_ptr<T>` 时，并不关心指针的顺序， 但依旧要提供比较函数（推荐使用 memory 头文件中的 `owner_less<T>`）， 它允许 `weak_ptr<T>` 和 `shared_ptr<T>` 对象比较， 通过调用智能指针的成员函数 `owner_before()` 函数实现了一个 `owner_before<T>` 实例， 甚至可以将指向不同类型对象的智能指针相互比较, 即 `shared_ptr<T1>` 对象可以和 `weak_ptr<T2>` 相互比较。  

## 使用 `multiset<T>` 容器
multiset 和 set 类似， 但是可以保存重复的元素， 默认用 `less<T>`, 可以指定不同的比较函数， 但是需要在元素等价的时候返回 false。  
* `.insert()` 总是可以成功， 返回指向插入（最后一个）元素的迭代器
* `.emplace()`/`.emplace_hint()` 总是成功， 返回一个指向新创建元素的迭代器
* `.find()` 返回匹配的第一个元素的迭代器， 没有的话则返回 `end()`
* `.equal_range()` 返回一个 迭代器 pair， 没找到则 first 为 `end()`
* `.lower_bound()`/`.upper_bound()`: 前者返回的参数匹配`.equal_range()` 的 first, 后者为 second
* `.count()` 返回匹配的元素个数

// TODO: 保存指针的细节要白天看

# Chapter 6 : 排序、合并、搜索和分区
## 序列排序
函数模板`sort<Iter>()` 默认会将元素排列成升序，
* 对象类型要支持 `<` 操作符，
* 同时必须是可交换的， 
* Iter 必须为支持随机访问的迭代器， 而 list(提供双向迭代器)、forward_list(提供正向迭代器)用的是成员函数的 `.sort()`。
* 可以提供函数对象作为第三个参数

### 排序以及相等的元素的排序
`sort<Iter>()` 使用快排实现， **可能改变相等元素的位置**， 这时候 `stable_sort` 可以满足我们的要求。

### 部分排序
* 提供特殊的 `partial_sort()` 算法做部分排序， 需要三个随机访问迭代器，  first/second/last， 之后 `[first,second)` 会包含降序序列中最小的 $second - first$ 个元素。
  * 可以提供比较函数
    ```C++
    size_t count{ 5 }; // Number of elements to be sorted
    std::vector<int> numbers{ 22, 7, 93, 45, 19, 56, 88, 12, 8, 7, 15, 10 };
    std::partial_sort(std::begin(numbers), std::begin(numbers) + count, std::end(numbers)，std::greater<int>());
    // after : 93, 88, 56, 45, 22, 7, 19, 12, 8, 7, 15, 10,
    ```
* `partial_sort_copy()` 可以将排序元素复制到不同的元素段（另一个容器）中， 前两个参数指定范围， 第三个第四个指定结果存放位置， 同样可以指定比较函数。
* `nth_element()` 算法的应用范围由第一个第三个元素指定， 第二个参数是指向第 n 个元素的迭代器（结果有点像快排中的 partition）

### 测试排序序列
* `is_sorted(beg, end, cmp)` 判断某个区间是否有序
* `is_sorted_until()`: 返回一个元素中升序列上边界元素的迭代器
    ```C++
    std::vector<string> pets {"dog", "coati", "cat", "chicken", "pig", "llama", "goat"};
    std::cout << "The pets in descending sequence are:\n";
    std::copy(std::begin(pets),
              std::is_sorted_until(std::begin(pets), std::end(pets), std::greater<>()),
                                    std::ostream_iterator<string>{std::cout, " "});
    ```

## 合并序列
合并操作会合并两个有**相同顺序的序列**中的元素，结果是一个包含两个序列的元素副本的序列：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220115010009.png" width="80%"/></div>

* 没有关于合并后容器的信息， 不能创建元素，只能使用第五个参数提供的迭代器来保存元素 
* 返回的迭代器指向合并序列末尾的后一个位置
* 同样可以使用自定义比较函数

`inplace_merge()`算法可以合并**同一个序列中**两个连续有序的序列， 三个参数分别是 first/second/last, last 是双向迭代器，第一个输入序列`[first,second)`, 第二个`[second, last)`，结果 `[first,last)`：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220115010643.png" width="70%"/></div>

## 搜索序列
STL 提供了多种用来搜索序列的方法， 大多用于无序序列， 部分要求有序。
### 在序列中查找元素
有三种可以在输入迭代器所定义的范围中查找单个对象的算法：
* `find(first, second, elem)`: 在 `[first, second)` 中查找第一个和 elem 相等的对象
* `find_if(first, second, functor)`: 在范围内查找第一个使得第三个参数指定的谓词（谓词是指**条件表达式的求值返回真或假的过程**）为 true 的对象， 谓词不能修改传给它的对象
* `find_if_not(first, second, functor)`

### 在序列中查找任意范围的元素
`find_first_of()`: 在序列1 中搜索第二个序列第一次出现的任何元素,例如在一个字符串中查找第一个元音：
```C++
string text{ "The world of searching" };
string vowels{ "aeiou" };
auto iter = std::find_first_of(std::begin(text), std::end(text), 
                               std::begin(vowels),std::end(vowels));
```
* 另一个版本可以指定谓词：
    ```C++ 
    std::vector<long> numbers {64L, 46L, -65L, -128L, 121L, 17L, 35L, 9L, 91L, 5L};
    int factors[] {7, 11, 13};
    auto iter = std::find_first_of(std::begin(numbers),std::end(numbers), // The range to be searched
                        std::begin(factors), std::end(factors), // Elements sought
                        [](long v, long d) { return v % d == 0;}); // Predicate - true for a match
    ```

### 在序列中查找多个元素
`adjacent_find()` 可以用来搜索序列中两个连续相等的元素，用`==`运算符比较一对元素， 返回指向第一个元素的迭代器：
```C++
string saying {"Children should be seen and not heard."};
auto iter = std::adjacent_find(std::begin(saying), std::end(saying));
```
* 同样也可以自定义谓词
    ```C++
    std::vector<long> numbers {64L, 46L, -65L, -128L, 121L, 17L, 35L, 9L, 91L, 5L};
    auto iter = std::adjacent_find(std::begin(numbers), std::end(numbers),            
                                            [](long n1, long n2){ return n1 % 2 && n2 % 2; });
    ```

#### `find_end()` 算法
`find_end()` 会在一个序列中查找最后一个和另一个**元素段**的匹配项， 他会返回一个指向最后一个匹配的段第一个元素的迭代器， 或者返回指向 end 的迭代器。
```C++
string text {"Smith, where Jones had had \"had\", had had \"had had\"."
             " \"Had had\" had had the examiners\' approval."};
string phrase {"had had"};
auto iter = std::find_end(std::begin(text), std::end(text), 
                          std::begin(phrase), std::end(phrase));
```
* 可以自定义谓词

#### `search()` 算法
和 `find_end()` 类似， 只是返回第一个匹配项第一个元素的迭代器， 也可以自定义谓词。 

#### `search_n()` 算法
前两个参数是定义搜索范围的正向迭代器， 第三个参数是想要搜索第四个参数的**连续匹配次数**,同样可以自定义谓词：
```C++
std::vector<double> values {2.7, 2.7, 2.7, 3.14, 3.14, 3.14, 2.7, 2.7};
double value {3.14};
int times {3};
auto iter = std::search_n(std::begin(values), std::end(values), times, value);
// 返回的是指向第一个 3.14 的迭代器
```

## 分区序列
`partition()`算法在序列中分区（即对元素重新排列）， 谓词返回 true 的元素会放在返回 false 的元素前面：
```C++
std::vector<double> temperatures{ 65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50 };
auto average = std::accumulate(std::begin(temperatures),
        std::end(temperatures), 0.0) / temperatures.size();
std::partition(std::begin(temperatures), std::end(temperatures),
        [average](double t) { return t < average; });
std::cout << average << std::endl;
std::copy(std::begin(temperatures), std::end(temperatures), 
            std::ostream_iterator<double>{std::cout, "->"});
//44.9167
//44->41->40->29->31->28->32->48->56->75->65->50->
```
* `partition()` 算法并不保证这个序列原始元素的相对顺序， 想要保证稳定的话可以使用 `stable_partition()` 算法

### `partition_copy()` 算法
和 `stable_partition()` 相同的方式进行分区， 使谓词返回 true 的元素会**被复制到另一个单独的序列**， 返回 false 的元素会被复制到**第三个序列中**， 操作不改变原序列的顺序。
* 如果输入序列和输出序列重叠， 那么算法将无法工作
```C++
std::vector<double> temperatures {65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50};
std::vector<double> low_t; // Stores below average temperatures
std::vector<double> high_t; // Stores average or above temperatures
auto average = std::accumulate(std::begin(temperatures),std::end(temperatures), 0.0) 
              / temperatures.size();
std::partition_copy(std::begin(temperatures), std::end(temperatures),
                    std::back_inserter(low_t), std::back_inserter(high_t),
                    [average](double t) { return t < average; });
```

### `partition_point()` 算法
这个算法返回第一个分区的结束迭代器， 参数和 partition 类似, 在使用之前， 需要确保分区已经被 partition。
* 可以使用 `is_partitioned()` 先判断是否已经分区
```C++ 
std::vector<double> temperatures {65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50};
auto average = std::accumulate(std::begin(temperatures),std::end(temperatures), 0.0)
             / temperatures.size();
auto predicate = [average](double t) { return t < average; };
std::stable_partition(std::begin(temperatures), std::end(temperatures), predicate);
if(std::is_partitioned(std::begin(temperatures), std::end(temperatures),predicate))
    auto iter = std::partition_point(std::begin(temperatures), 
                                     std::end(temperatures), predicate);
```

## 二分查找算法
### `binary_search()` 算法
查找的序列必须是有序的或者至少相对于所查找的元素是有序的，在前两个参数确定的范围内查找是否存在等同于第三个参数的元素， 找到则返回 true。想要知道元素的位置需要使用 `lower_bound()`/`upper_bound()` 或者 `equal_range()`。
* 另一个重载版本接受额外的参数， 用于查找元素
    ```C++
    std::list<int> values {17, 11, 40, 36, 22, 54, 48, 70, 61, 82, 78, 89, 99, 92, 43};
    auto predicate = [](int a, int b){ return a > b; };
    values.sort(predicate); // Sort into descending sequence
    int wanted {22};
    if(std::binary_search(std::begin(values), std::end(values), wanted, predicate))
        std::cout << wanted << " is definitely in there - somewhere..." << std::endl;
    else
        std::cout << wanted << " cannot be found - maybe you got it wrong..." << std::endl;
    ```

### `lower_bound()` 算法
在前两个参数指定的范围内查找不小于第三个参数的第一个元素。 

### `equal_range()` 算法
查找有序序列中所有和给定元素相等的元素， 返回一个 pair 对象：
* first 是指向第一个不小于第三个参数的第一个元素
* second 指向第一个大于第三个参数的元素

# Chapter 7: 更多的算法
通常 STL 算法可以分成两类：
* 会改变所运用序列的算法
* 不改变所应用序列的算法

## 检查元素的属性
`<algorithm>` 头文件中定义了三种算法， 检查在算法应用到序列上时， 什么时候使得谓词返回 true:
* `all_of()`: 在所有元素都使得谓词返回 true 的时候返回 true 
* `any_of()`: 存在任一元素使得谓词返回 true 的时候返回 true 
* `none_of()`: 没有元素使得谓词返回 true 时候返回 true  
    ```C++
    std::cout << "There are "
        << (std::any_of(std::begin(ages), std::end(ages),
                        [min_age](int age) { return age < min_age; }) ? "some": "no")
        << " people under " << min_age << std::endl;
    ```

进一步的， 可以使用 `count()`/`count_if()` 可以找出和第三个参数等价/使得谓词返回 true 的元素个数。

## 序列的比较
如果两个序列的长度相同， 并且对应的元素相等， `equal()` 算法会返回 true, 有四个版本的 equal 算法：
* 前两个版本使用 `==` 操作符做比较
  * 第一个接受三个迭代器作为参数， 前两个指定序列1的范围， 第三个为第二个序列的开始迭代器， **如果第二个序列长度不够， 结果是未定义的**
  * 第二个版本接受四个迭代器作为参数
* 后面两个版本可以接受一个额外的谓词作为参数

### 查找序列的不同之处
`mismatch()` 算法和 equal 同样有四个接受不同参数的版本， 它可以告诉我们不匹配的位置。
* 返回的 pair 对象包含两个迭代器，指向两个序列中不匹配的元素位置，如第n 个不匹配， 则返回 `pair<iter1+n, iter2+n>`
* 如果是接受三个迭代器的版本， 即 `mismatch<iter1, end_iter1,iter2>`, 情况就有一些复杂：
  * 返回 `pair<end_iter1, (iter2+end_iter1-iter1)>`: 如果第一个序列比第二个序列长， 结果是未定义的

### 按字典查找序列
字典序是字母排序思想的泛化， 就是从两个序列的第一个元素开始依次比较各个元素， 这适用 `lexicographical_compare()`, 前两个参数定义第一个序列， 三四两个参数定于第二个序列， 默认使用 `<` 操作符来比较元素， 在需要的时候提供第五个参数作为比较的函数对象：
* 如果第一个更小返回 true, 否则返回 false
```C++
std::vector<string> phrase1 {"the", "tigers", "of", "wrath"};
std::vector<string> phrase2 {"the", "horses", "of", "instruction"};
auto less = std::lexicographical_compare(std::begin(phrase1), std::end(phrase1),
                                         std::begin(phrase2), std::end(phrase2),
                                         [](const string& s1, const string& s2){ return s1.length() < s2.length(); });
```

### 序列的排列
`next_permutation()` 生成一个序列的重排列， 他的结果是字典序的下一个排列， 默认使用 `<` 来做比较。
* 返回一个 bool 值， 新排列大于上一个排列时候返回true
* 就排列如果已经是最大的， 返回 false, 同时排列变为最小的
* 同样可以通过传入 lambda 表达式作为第三个参数
    ```C++
    std::vector<int> range{ 1, 2, 3, 4 };
	do {
		std::copy(std::begin(range), std::end(range), std::ostream_iterator<int>{std::cout, " "});
		std::cout << " -> ";
	} while (std::next_permutation(std::begin(range), std::end(range)));
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220117004246.png" width="90%"/></div>

`prev_permutation()` 参数和前者类似， 只不过返回值以及对序列的操作刚好相反。  
`is_permutation()` 检查一个序列是不是另一个序列的排列， 如果是就返回true:
* 同样允许使用一个开始迭代器指定第二个序列， 并不推荐使用

`shuffle()` 生成序列的随机排列。 

## 复制序列
### 复制一定数目的元素
`copy_n()` 从源容器复制指定个数的元素到目的容器中， 
* 第一个参数是指定源的输入迭代器
* 第二个参数是个数
* 第三个参数是指定目的地第一个位置的输出迭代器
    ```C++
    std::vector<string> names {"Al", "Beth", "Carol", "Dan", "Eve",
            "Fred", "George", "Harry", "Iain", "Joe"};
    std::unordered_set<string> more_names {"Janet", "John"};
    std::copy_n(std::begin(names) + 1, 3, std::inserter(more_names, std::begin(more_names)));
    ```

上面的 `inserter`是一个函数模板， 生成的 insert_iterator 对象调用成员函数 insert 向容器添加元素。目的地址可以是流迭代器：
```C++
std::copy_n(std::begin(more_names), more_names.size()-1,
            std::ostream_iterator<string> {std::cout, " "});
```

### 条件复制
`copy_if()` 复制使得谓词返回 true 的元素。前两个输入迭代器定义源， 第三个是指向目的序列第一个位置的输出迭代器， 第四个参数是一个谓词。  
**反向复制**： `copy_backward()` 并不是逆转元素顺序， 只是从后往前复制， 前两个参数定义序列， 第三个参数定义输出序列的结束迭代器， **这样是在序列重叠时候会有一定优势**。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220117010108.png" width="50%"/></div>

## 复制和反向元素顺序
`reverse_copy()` 可以将源序列复制到目的序列中， 并且目的序列中的元素是逆序的。
* 定义序列的前两个迭代器必须是**双向迭代器**
* 第三个参数是目的序列开始位置的输出迭代器
* 序列如果重叠， 输入结果是未定义的

`std::reverse()` 算法可以在原地逆序双向迭代器所指定的范围。

## 复制一个删除相邻重复元素的序列
`unique_copy()` 将一个序列复制到另一个序列中， 同时移除连续的重复元素， 默认使用 `==` 操作符决定元素是否相等，前两个参数定义源序列， 第三个是指向目的序列第一个位置的**输出迭代器**， 可选第四个判断相等的函数对象。例如去除字符串中多余的空格：
```C++
string text{ "Have    you    seen     eny duplicated spaces here" };
string result{};
std::unique_copy(std::begin(text), std::end(text), std::back_inserter(result),  
    [](const char& beg, const char& end) {
		return beg == ' ' && end == ' ';
	});
std::cout << result << std::endl;
```

## 从序列中移除相邻的重复元素
`unique()` 可以在序列中原地移除重复的元素， 返回一个正向迭代器作为新的结束迭代器， 无法保证它之后的元素状态，配合 `.erase()` 成员函数可以将后面的部分删除：
```C++
std::vector<std::string> words{ "A","A","BB","BB","CCC" };
auto new_end = std::unique(std::begin(words), std::end(words));
words.erase(new_end, std::end(words));
std::copy(std::begin(words), std::end(words), 
          std::ostream_iterator<std::string>{std::cout, " "});
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220117012448.png" width="30%"/></div>

## 旋转序列
`rotate()` 算法像左边旋转序列元素， 有点像转手镯上的珠子：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220117012708.png" width="70%"/></div>

`rotate_copy()` 则在新序列生成旋转之后的副本。

## 移动序列
`move()` 算法将前两个参数定义的序列移动到第三个参数指定的序列处，**这是一个移动操作， 无法保证操作之后输入序列不变**。  
`swap_ranges()` 交换两个序列， 需要三个正向迭代器作为参数。
```C++
using Name = std::pair<string, string>; // First and second name
std::vector<Name> people {Name{"Al", "Bedo"}, Name{"Ann", "Ounce"}, Name{"Jo", "King"}};
std::list<Name> folks {Name{"Stan", "Down"}, Name{"Dan", "Druff"}, Name{"Bea", "Gone"}};
std::swap_ranges(std::begin(people), std::begin(people) + 2, ++std::begin(folks));
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220117013706.png" width="50%"/></div>

## 从序列中移除元素 
很容易从函数名中直到其功能和参数：
* `remove()`
* `remove_cop()`
* `remove_if()`
* `remove_copy_if()`

## 设置和修改序列中的元素
`fill()`/`fill_n()` 提供了向序列填入给定值的简单方式。
### 用函数生成元素的值
`for_each()` 算法将一个函数对象应用到序列的每一个元素上， 函数对象的参数是序列元素的引用， 因此可以直接修改值。  
`generate()` 的第三个参数的函数对象形式为 `T func()`, 因此其返回值就直接保存到序列中了：
```C++
string chars (30, ' '); // 30 space characters
char ch {'a'};
int incr {};
std::generate(std::begin(chars), std::end(chars), [ch, &incr]
                            {
                            incr += 3;
                            return ch + (incr % 26);
                            });
std::cout << chars << std::endl; 
```
`generate_n()` 就是第二个参数从序列元素的迭代器变成了元素个数。 

### 转换序列
`transform()` 可以将函数应用到序列元素上， 并将这个函数的返回值保存到另一个序列中。和 for_each 的区别是， 前者的函数返回类型必须为 void, 而 transform 的一元函数版本必须返回一个值， 该值的类型可以保存到输出序列中。
* transform 还有二元版本， 前两个参数是第一个输入序列的输入迭代器， 第三个迭代器指向第二个输入序列的第一个元素， 第四个参数是指向一个输出序列的迭代器，第五个参数是一个函数对象， **它接受两个输入序列的元素作为参数并且返回一个和输出序列兼容（能写到输出序列）的值**。

### 替换序列的元素
* `replace()`前两个参数定义序列， 第三个参数为要替换的值， 第四个参数为新值。  
* `replace_if()` 第三个参数换为一个谓词。 
* 此外还有 `replace_copy()`/`replace_copy_if()`

# Chapter 8 : 生成随机数
## 什么是随机数
**数字计算机是确定性的，但是可以产生伪随机数序列**。假设一个数出现在一个序列中， 并且不能通过前面的元素来推测， 就认为这个数是随机的。
## 概率、分布以及熵
概率用于衡量事件发生的可能性， 概率密度函数是一个描述这个随机变量的输出值，在某个确定的取值点附近的可能性的函数。  
熵是对混乱程度的描述， 可以用于度量比特序列的随机性。 

## 用 STL 生成随机数
STL 中和随机数生成相关的有四个术语：
* **随机数生成引擎**是一个定义了生成随机数序列的无符号整数序列机制的类模板
* **随机数生成器**是随机数引擎类模板的一个预定义的实例
* 随机数引擎适配器是一个类模板， 通过修改另一个随机数引擎生成的序列来生成随机数序列
* 分布表示的是随机序列中出现在序列中的概率 

### 生成随机数的种子
种子决定了随机数生成器的初始状态和全部序列，对于给定的单个种子， 随机数生成器总是生成相同的序列， 为了每次能够产生不同的序列， 必须提供不同的种子。  

### 获取随机种子
`random_device` 定义的函数对象可以生成用作种子的随机的无符号整数值， 它采用非确定的数据源（通常由操作系统提供）。
```C++
std::random_device rd;
// 函数对象， 就可以调用， 构造函数函数 string& 类参数
auto my_1st_seed = rd();
```
这个类有三个成员函数：
* `.max()/.min()` 最大、最小的可能值
* `.entropy()` 是将数据看作是 double 或者 0 后熵的估计值

