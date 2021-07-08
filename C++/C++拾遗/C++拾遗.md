# C++ Primer
## Basics
* C++ Primer中的iterator运算, 两个迭代器做差之后就是它们的距离(例如用于二分查找),结果是`iterator<T>::difference_type`这个带符号类型(这个类型实际上是从迭代器中**萃取**出来的).
* 理解复杂的数组声明:允许定义数组的指针以及数组的引用,**最好的办法是,假定它为一个表达式并且对它求值, 从数组的名字开始按照由内向外的顺序阅读**, 从优先级上看下边引用`[]` 是高于间接访问的,因此加不加括号还是有很大的区别:
    ```C++
    int *ptrs[10]; // 包含整形指针的array
    int (*Parray)[10]; //指向一个包含十个整型数字数组的指针
    int (&arrRef)[10] = arr; // arrRef引用一个含有10个int的数组
    int *(&array)[10] = ptrs; //array是一个数组的引用, 该数组包含10个pointer
    ```

* vector和string一样, 数组的下标是否在合理范围内是由程序员检查的
* since c++11, 出现了`begin`和`end`两个non-menber函数, 可以作用在array等上,得到首元素和**尾元素下一个位置**的指针, 两个指针运算的结果是一种`ptrdiff_t`的带符号标准库类型(数组使用时下标可以为负, 但是string和vector都不允许)
* c-Style string 会带来一些问题,可以用它初始化 `std::string`:
    1. 不以'\0'结尾的数组, 在 `strlen` 等函数中, 计算的结果是会随着编译器的变化而变化(未定义行为)
    2. c-style 字符串的比较实际上是指针的比较, 和c++ `std::string` 的结果完全不同
    3. 目标字符串大小由调用者指定
* 处理复合表达式:
    1. 拿不准的时候使用括号强制表达式的组合关系复合逻辑需求
    2. 如果改变了某个运算对象的值, 在表达是的其他地方不要再使用这个运算对象
* `&&`和`||`有**短路求值**的特点
* `sizeof(arr)`处理array的时候, arr不会被当成指针
* 局部静态对象: 在第一次经过对象定义语句的时候初始化, 直到**程序终止时候才被销毁**.
* 局部变量会隐藏同名的全局变量, 要使用`::`得到该全局变量的scope
* 数组作为形参时候, 数组/指针的形参是等价的,要规定传入数组的长度有以下做法
    ```C++
    void print(const int*);
    void print(const int[]);
    void print(const int [10]); // 这里的十不会起到作用
    ```
    1. 使用标记指定数组长度, 如c-style string:
        ```C++
        void print(const char* cp){
            if(cp){
                while(*cp!='\0')// 也可写成 while(*cp)
                    std::cout << *cp++;
            }
        }
        ```
    2. 使用标准库规范(beg和end)界定开始点和结束点
    3. 显式传入一个指定数组大小的形参
* 函数重载时,顶层const 不影响传入函数的对象, 但是底层的const可以(在指针和引用中):
    ```C++
    Record lookup(Phone);
    Record lookup(const Phone); // 重复声明
    Record lookup(Account&);
    Record lookup(const Account&); // 新函数, 作用于常量引用
    Record lookup(Account*);
    Record lookup(const Account*); // 新函数,作用于指向const的指针
    ```
    1. 编译器找到一个与实参最佳匹配的函数,生成调用函数的代码
    2. 找不到一个版本与实参匹配, 发出无匹配的错误信息
    3. 多于一个函数可以匹配,但都不是最佳选择,出现**二义调用**
    4. 在不同的作用域中**无法重载函数名**
* 多次声明同一个函数也是合法的, 但是**给定的作用域中一个形参只能被赋予一次默认实参**
* `constexpr`,相当于一个编译期的const, 但是功能更强, 令所修饰的表达式或者函数具有编译期的常量性
    * constexpr函数(被隐式指定为内联)要求返回值和形参类型都是字面值类型,c++11 只允许一条return 语句, 但是14中有所放宽
    ```C++
    constexpr int new_sz(){return 42;}
    constexpr size_t scale(size_t cnt){return new_sz() * cnt;} //允许返回值并非一个常量
    int arr[scale(2)]; // 正确, 参数是一个常量表达式
    int i = 2;
    int arr[scale(i)]; // error: scale(i) 不是一个常量表达式
    ```
* assert 和 NDEBUG配合, 定义了后者, assert什么也不做
* 函数指针形参: 形参可以是指向函数的指针, 形参是函数类型, 实际上依旧当做指针使用:
    ```C++
    void useBigger(const std::string &s1, const std::string &s2, 
                   bool pf(const std::string &, const std::string&));
    //等价的声明:
    void useBigger(const std::string &s1, const std::string &s2, 
                   bool (*pf)(const std::string &, const std::string&));
    //同时, 也可以用尾置声明返回指向函数的指针
    auto f1(int) -> int (*)(int*, int);
    ```
## 面向对象编程
* const成员函数: 不能改变调用该函数的对象的内容<font color=red> 常量对象,以及常量对象的指针或引用都只能调用常量成员函数.</font> const可以作为成员函数的签名重载, 而调用该函数的对象隐式地当做一个整体(`this`指针)传入成员函数.
* 类没有声明默认构造函数时, 编译器会自动生成默认构造函数,类包含内置类型或者复合类型时(有成员变量),**只有这些成员全被赋予类内初始值,才适用默认构造函数**.
* 编译器能够帮我们完成Big 3, 但是**当类需要分配对象之外的资源(如含有指针)时, 合成的版本常常是错误的**
* 使用mutable声明的data member, 可以被const成员函数改变.
* 提供类内初值时候, 必须使用`=`或者`{}`的直接初数化方式:
    ```C++
    class Window_mgr{
    private:
        //标准情况下window_mgr包含一个标准尺寸的空白Screen
        std::vector<Screen> screens{Screen(24,80,'')};
    }
    ```
* 和函数一样, 类也可以有**前向声明**(forward declaration), 声明后定义之前是一个不完全类型,声明过后允许类包含指向自身类型的引用或者指针.
* 类可以把其他的类指定为友元, 但是这种关系没有传递性.**每个类负责控制自己的friend class 和 friend function**.
    ```C++
    class Screen{
        //只将Window_mgr中的clear设置为friend
        friend void Window_mgr::clear(ScreenIndex);
    };
    ```
* 类的作用域: 在类外, 成员的名字被隐藏起来,因此实现成员函数的时候需要提供`ClassName::functionName`.类的名字查找有一些需要注意的:
    1. 首先编译成员的声明
    2. 类全部可见之后再编译函数体(处理完全部的声明后才会处理函数的定义)
* 构造函数的初始值有时候必不可少: 成员是const 或者引用的话,必须要初始化
    ```C++
    class ConstRef{
    public:
        ConstRef(int ii);
    private:
        int i;
        const int ci;
        const &ri;
    };
    ConstRef::ConstRef(int ii):i(ii),ci(ii),ri(ii){};//这时候只能用初始化,不能用初值
    ```
* 将构造函数声明为`explicit`阻止隐式类型转换(不用于拷贝形式的初始化中`=`),在这种情况下我们可以显式地去做强制类型转换:
    ```C++
    item.combine(Sals_data(null_book));//OK:实参是一个显式构造的Sals_data
    item.combine(static_cast<Sals_data>(cin)); // 强制cast, 可行
    ```
* 字面常量值,除了算术类型,引用和指针外, 某些类也可以是(类可能含有`constexpr`函数成员),数据成员都是字面值的聚合类,或者满足以下要求:
    1. 数据成员都是字面值类型
    2. 类必须要有一个`constexpr`构造函数
    3. 数据成员含有类内初始值, 初始值必须一条常量表达式,如果成员属于某个种类类型, 那么初始值必须使用成员自己的`constexpr`构造函数
    4. 类必须使用自己析构函数的默认定义, 该成员负责销毁类的对象
* 静态成员 类的静态成员 __不与任何对象绑在一起, 也不含this指针__,在外部定义类的静态成员时,**不能重复`static`关键字**,一般而言, 我们常常要在类的外部定义和初始化每个静态成员,一个静态成员只能定义一次.此外, 我们可以在类内为类的静态成员提供`const`整型的类内初始值,要求必须是**字面值常量类型的的`constexpr`**,即使在类内部被初始化了, 常常要在类的外部定义一下该成员.

## Intro to 顺序容器
* 一般而言,应用中占主导地位的操作决定了容器类型的选择(访问操作多还是插入/删除操作多)
* 迭代器范围(iterator range)概念是标准库的基础, 由两个迭代器构成, 分别指向同一个容器中的元素或者**尾元素之后的位置**,一般称为左闭合区间`[beg, end )`.
* 迭代器`begin, end`有多个版本, 带r的返回反向迭代器, 带c的返回常量迭代器
* 当一个容器初始化另一个容器时, _两个容器类型和元素类型都必须相同_, 也可以接受两个迭代器表示想要拷贝一个元素范围.顺序容器还可以用一个容器大小和初始值构造出来.
* 和内置数组一样, std::array的大小也是类型的一部分,定义时除了指定元素类型之外 __还要指定容器大小__(隐式/显式),不能对内置数组进行拷贝和赋值操作(浅拷贝),但是std::array可以,同时允许赋值:
    ```C++
    array<int,10> digit = {1,2,3};
    array<int,10> cpy =digit; //正确
    digit = {1,2,3,5,5,6,7,8}; // 允许
    ```
* **顺序容器使用assign**, 用参数所指定的元素(拷贝)替换左边容器中的所有元素:
    ```C++
    list<string> names;
    vector<const char*> oldstyle;
    names=oldstyle; // ERROR: 容器类型不匹配
    names.assign(oldstyle.begin(),oldstyle.end());
    ```
* `swap`操作相同类型容器的内容(除了array,其他都在常数时间内可以完成),统一使用**非成员**的swap是一个好习惯.
* 容器大小的操作,`size()`,`empty()`,`max_size()`, 有一个例外是forward_list不支持`size()`
* 除了std::array和forward_list,每个顺序容器都支持`push_back`
* list,forward_list和deque支持`push_front`的类似操作
* vector,deque,list,string都支持`insert`成员, forward_list提供特殊版本的insert.
* sice C++11,引入emplace操作:**将参数的类型传递给容器中元素的构造函数**,参数必须与元素类型的构造函数相匹配.
* 包括array在内的顺序容器都要front成员函数, 除了forward_list之外的顺序容器都有back成员函数,返回首尾元素的 __引用__(直接方法,间接方法是用迭代器)
* at成员函数类似于下标运算符, 但是如果下标越界, 会抛出`out_of_range`异常
* pop_front和pop_back成员函数返回的是void, 接受迭代器版本的erase允许我们删除两个迭代器之间的元素
* forward_list删除一个element会改变前一个element的next指针的值,因此插入\删除的实现是特殊的`insert_after`和`erase_after`
* 使用resize增大/缩小容器, 但是array不支持
* 向容器添加和删除元素的操作可能**使得指向容器的指针/引用/迭代器失效**,这可能导致严重的运行时错误, 每次改变容器的操作做完都应该重新定位迭代器.
* 容器的size()指的是当前已经保存的元素数目, 而capacity是在不重新分配内存的情况下它最多可以保存多少元素.此外, vector/string/deque支持`shrink_to_fit`
* string 还支持append和replace函数,并且有多个重载版本.
* string 提供了六种不同的搜索函数, 每个函数都有4个重载版本. 在标准库中还可以从后向前找(`rfind ...`)
    1. `rfind` 查找子串
    2. `find_first_of(numbers)` 查找一个与number中任何一个字符匹配的位置
    3. `find_first_not_of(numbers)`
    4. `compare` 返回0/正数或者负数(根据两个字符串大小)
* **容器适配器** : 本身是用其他容器实现的, 但是使用适配器的机制使得它看起来像是另一种东西. 标准库中定义了三种容器适配器 `stack` `queue` 和 `priority_queue`, 他们都支持两种构造方法
    1. 默认构造函数构造一个空的对象
    2. 接受一个容器构造函数拷贝该容器内容来初始化适配器

## 泛型算法
* **泛型算法永远不会执行容器操作,它们只会运行在迭代器之上**, 结果是迭代器可以完成向容器添加元素的操作,但是算法本身永远不会改变底层容器的大小.
* 除了少数例外, 标准库算法都对一个范围内的元素进行操作.
* 只读算法, 不修改范围内的元素,如 `std::accumulate(c.cbegin(),c.cend(),iniValue)`和`std::equal(a.cbegin(), a.cend(), b.cbegin())`等
* 一些算法将新的值赋予序列中的元素, 我们在使用时应该 __确保序列原大小不小于我们算法要求写入的元素数目__, 算法不会执行容器操作, 因此它们自身不可能改变容器的大小.


## 关联容器
## 动态内存与智能指针
* C++中使用一对运算符`new` / `delete`做动态内存管理, 在正确的时间释放内存十分困难, 新的标准库提供了两种智能指针
    1. `shared_ptr` 允许多个指针指向同一个对象
    2. `unique_ptr` 独占所指向的对象<br>
    同时定义有一个`weak_ptr`的弱引用,指向shared_ptr所管理的对象.
    ```C++
    shared_ptr<std::string> p1; // A shared_ptr to std::string
    ```
    传统的指针操作智能指针都支持, 同时,提供了成员函数`get()`返回保存的原始指针, 成员`p.swap(q)`和非成员版本`swap(p,q)`的交换方法,对于share_ptr<T> `sp`,还提供成员函数`sp.unique()`和`sp.use_count()`.

# C++ 高级编程

# Pointers On C
## 快速上手
* 在 C 语言中, 看上去数组参数是以引用方式传递, 实际上就是传入的首元素的地址. 总之, 在 C 中**所有传递给函数的参数都是按值传递的**.

## 数据
* `typedef` 写法 **和普通的声明基本相同**
    ```C
    // 声明时变量时
    char *ptrToChar;
    // 声明和使用 typedef 时
    typedef char *ptrToChar;
    ptrToChar somePtr;
    struct Node{
        Node* next;
        int val;
    };
    // 声明指向节点的指针
    Node *ptrToNode;
    // 使用 typedef
    typedef Node *head;
    ```
* 链接属性一共有 3 种, external/internal/none
  * `external` 链接属性的标识符**无论声明多少次/位于几个源文件都表示同一实体**, 所有源文件中的所有函数都可以访问
  * `internal` 链接属性的标识符**在同一源文件内的所有声明都指向同一实体, 位于不同源文件的多个声明分属不同的实体**
  * 某个关键字具有 `external` 属性, 前面加入 `static` 可以使其链接属性变为 `internal`, 把**函数声明的 `static` 可以防止其被其他源文件调用**

* C 中`static`的总结:
  * 用于函数定义或者用于代码块之外的变量声明的时候, 作用是修改标识符的链接属性, 存储类型和作用域不受影响
  * 用于代码块内部的变量声明, 作用是修改变量的储存类型, 变量的链接属性和作用域不受影响

## 语句
* `break` 和 `continue` 的区别<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210708135318.png"/></div>
* `switch` 语句中的**标签必须是常量表达式**(可以在编译期间求职的表达式), 不可以是任何变量, 同时要注意每个标签的 `break` 和最后的 `default` 语句
  
## 操作符和表达式
* 下标引用和间接访问表达式是等价的
    ```C
    array[index];
    *(array + index);
    ```
* 如果函数的执行具有副作用, 那么函数的执行顺序的不同可能会产生不同的结果, 因此要避免写出 `f()+ g() + h()` 这样的语句, 因为加法运算符要求左边的加法必须在右边的加法之前执行, 但是**三个函数的调用顺序是未定义的**
  
## 指针
* 在对指针执行直接访问前, 指针必须初始化, 否则结果是未定义的
* 对于任何**非指向数组元素的指针**执行算术运算是非法的
* 两个指针并不是指向同一数组元素, 它们之间的相减是错误的

## 函数
* C 函数所有参数均以 **传值调用** 方式传递, 传递数组时候实际是 **传址调用**
* 标准库中的`va_list`类型和`va_start`/`va_arg`/`va_end` 三个宏配合使用可以实现可变参数的函数

## 数组
* 数组 vs 指针
  * 可以将地址赋值給指针, 但是地址赋值給数组的操作是非法的(数组名是一个指针常量)
  * 指针定义时只需要一个指针大小,是一个标量值, 而数组在定义时候就确定数量元素,`sizeof` 操作返回的是整个数组的长度
  * 数组的下标索引其实是指针操作的别名
