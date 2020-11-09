# More Effective C++ by Scott Meyers

## Basics

### Item 1 仔细区别pointers 和 references

Pointer和reference的接口看起来差别巨大,(pointer使用*和-> operator,reference使用.),而他们具体在什么时候使用?<br>
1. 首先,reference必须代表某个对象,没有null reference, 这样使用起来比pointer更有效率,因为使用reference之前不需要检验其有效性:
    ```C++
    void printDouble(const double& rd){
        cout << rd; // no need to test rd; it must refer to a double
    }
    void printDouble(const double *pd){
    if (pd) { // check for null pointer
        cout << *pd;
        }
    }
    ```
2. Pointer可以被重新赋值指向另一个对象,但是reference总是指向它最初获得的那个对象.
    ```C++
    string s1("Nancy");
    string s2("Clancy");
    string& rs = s1; // rs refers to s1
    string *ps = &s1; // ps points to s1
    rs = s2; // rs still refers to s1, but s1’s value is now "Clancy"
    ps = &s2; // ps now points to s2; s1 is unchanged
    ```
因此,当你需要指向某个东西,而且绝不会指向其他东西或者当你实现一个操作符而其语法需求无法由pointer完成时,应该使用reference,其他时候使用pointer.

### Item 2 最好使用C++的转型操作符

新的cast形式十分容易被辨识出来,static_cast基本与C旧式转型有相同的威力和限制, 比如我们不能够用static_cast将一个struct转型为一个int,或者将一个double转型为一个指针, 不能移除表达式的constness.<br>
其他转型操作符更适用于范围狭窄的目的, 如const_cast最常见的目的就是将常量性移除,dynamic_cast用来执行继承体系下的安全向下转型或者是跨系转型动作.<br>
我们可以用dynamic_cast将指向base class对象的pointer或者reference转型为指向derived或者sibling base的pointer和reference.它无法应用于缺乏虚函数的类型上,也不能改变类型的常量性.<br>
reinterpret_cast最常见的用途是 **转换函数指针类型,函数指针的转型动作,并不具备可移植性**.<br>

### Item 3 绝对不要以多态的方式处理数组

C++允许以base class的pointer和reference来操作"derived class object 所形成的数组"。如果有继承至BST的BalancedBST, 用BST的指针去便利BalancedBST, 就会出问题:
```C++
class BST { ... };
class BalancedBST: public BST { ... };
void printBSTArray(ostream& s,
const BST array[],
int numElements){
    for (int i = 0; i < numElements; ++i) {
    s << array[i]; // this assumes an
    }              // operator<< is defined
}                  // for BST objects
BalancedBST bBSTArray[10];
...
printBSTArray(cout, bBSTArray, 10);
```
bBSTArray[i]实际是一个指针运算表达式的缩写, 代表*(bBSTArray+i),相距$i \times sizeof(\text{数组中的对象})$, 编译器将它视为$i \times sizeof(BST)$, 但是derived class 通常比base class 对象大, 这样就会出现问题.<br>
此外, 如用base 指针删除derived class object组成的数组, 编译器看到`delete[] array`的句子, 通常理解为:
```C++
for (int i = the number of elements in the array - 1;i >= 0;--i){
    array[i].BST::~BST(); // call array[i]’s destructor
} 
```
而**通过base pointer删除一个derived classes object构成的数组, 其结果是未定义的** .

### Item 4 非必要不提供default constructor

默认构造函数(i.e, 一个可以无参数调用的构造器)使用时有下面的要求: **必须要有某些外来信息才能生成对象的 classes, 则不必要拥有默认的构造函数**.<br>
```C++
class EquipmentPiece { 
public: 
    EquipmentPiece(int IDNumber);
    ...
};
```
这意味着, 如果类缺乏default constructor, 使用这个class的时候会有一些限制:
1. 在产生数组的时候,没有默认构造函数就会出问题:
    ```C++
    Equipmentpiece bestPieces[10]； // error! No way to call  EquipmentPiece ctors
    Equipmentpiece *bestPieces = new EquipmentPiece[10]； // error! same problem
    ```
    可以有一些方法解决这个问题:
    * 使用Non-heap数组, 提供构造参数生成相应的数组
    * 使用指针数组而非对象数组:
        ```C++
        typedef Equipmentpiece* PEP； // a PEP is a pointer to // an EquipmentPiece
        PEP bestPieces[10]； // fine, no ctors called
        PEP *bestPieces = new PEP[10]； // also fine
        for (int i = 0； i < 10； ++i)
            bestPieces[i] = new EquipmentPiece( ID_Number);
        ```
    使用指针数组依旧是有问题的, <font color=red> 第一是必须记得将指针所指的所有对象删除, 第二是如果内存总量很大, 需要一些空间来放置指针,还需要一些空间来放 _EquipmentPiece_ 对象 </font> 过度使用内存的问题可以避免, 方法是先为此数组分配 raw memory, 然后使用 _placement new_ 在这块内存上构造 _EquipmentPiece_ 对象.这种做法要求你在数组内对象生命周期结束时, 以手动的方式调用其析构函数, 再调用 _operator delete[]_ (因为rawMemory不是用 _new_ 操作符得到的, 一般的数组删除操作作用在它上的结果是未定义的).

2. 它将不适用于很多 template-based container classes, 因为他们的实例化目标类型必须要有一个默认构造器. **因为在这些template内部几乎总是产生一个以 template类型参数 作为类型而架构起来的数组.**<br>
    ```C++
    template<class T> 
    class Array { 
    public: 
        Array(int size);
        ...
    private： 
        T *data； 
    };  
    template<class T> 
    Array<T>::Array(int size) { 
        data = new T[size]；//calls T::T() for each element of the array
    }
    ```
    如果谨慎设计template, 可以消除对默认构造器的需求.
3. 最后 一点考虑和虚基类有关, 如果虚基类缺乏默认构造函数, 与之合作十分困难, 因为**虚基类的构造函数的参数必须由想要产生的对象的派生层次最深的class提供, 缺乏默认构造器的虚基类, 要求所有派生的类都必须了解其意义, 并且提供虚基类构造器的参数**.

如果类构造函数可以确保对象的所有fields都会被正确地初始化, 添加无意义的默认构造函数的效率/复杂性成本可以免除, 如果无法保证, 最好避免让 _default constructor_ 出现.

## 操作符
### Item 5 : 对定制的"类型转换函数"保持警觉
对于user define type, 我们可以选择是否提供某些函数, 供编译器拿来实现隐式类型转换.两种函数允许编译器执行这样的转换, **单参数构造器和隐式类型转换操作符.**<br>
1. 单参数构造器:
    ```C++
    class Name { 
        public: 
        Name(const std::string& s); // 将一个 string 转成了name类型
    };
    ```
2. 隐式类型转换操作符, 是一个拥有奇怪名称的成员函数: operator 之后加上一个类型名称:
    ```C++
    class Rational {
    public: 
        ...
        operator double() const； // converts Rational to double
    }；
    Rational r(1,2); // r的值为 1/2
    double d = 0.5 * r; // r隐式转换成为double, 然后做乘法
    ```

而提供这种函数的问题在于, 它将在你从未打算也从未预期的情况下被调用,得到的结果可能是不正确不直观的程序行为并且难以调试. 如我们在打印 r 的时候, 如果忘记给 _Rational_ 写一个 _operator <<_ , r将隐式转换成一个double并且被打印出来: **隐式类型转换的出现可能导致错误(非预期)的函数被调用**. 解决的办法也很简单, <font color=red>使用一个对等的函数取代类型转换操作符</font>. <br>
而通过单参数构造器完成的隐式类型转换, 则比较难以消除. 下面就是一个错误使用的例子:
```C++
template<typename T> 
class Array { 
public: 
    Array(int lowBound, int highBound)； 
    Array(int size);  // 可能被使用成单参数构造器
    T& operator[](int index); 
    ... 
};
bool operator==( const Array<int>& Ihs, const Array<int>& rhs)；
Array<int> a(10) ;
Array<int> b(10) ;
for (int i = 0；i < 10； ++i) { 
    if (a == b[i]) { // oops! "a" should be "a[i]"
        // do something when a[i] and b[i] are equal
    } else { 
        // do somethingwhen they're not；
    }
}
```
这个错误带来的结果是, 写错代码后, 原来的 `==` 无法调用, 但是只要调用 _Array(int size)_ 这个构造器,那么每次就是 a和一个大小为b[i]的临时对象做比较. <br>
简单的解决办法是, **使用关键词 _explicit_**, 只要将构造函数声明为 _explicit_, 就无法因为隐式类型转换的需要而调用他们, 不过**显式类型转换依旧是允许的**.<br>
一条判断隐式类型转换是否合法的规则是, **没有任何一个转换程序可以内含一个以上的"用户定制转换行为(包含上面两种转换)"**, 可以利用这种规则, 将你希望的对象构造行为合法化, 将不希望的隐式构造非法化.<br>
考虑之前的Array Template, 我们可以产生一个新的class, ArraySize, 单参数的构造器接收一个ArraySize对象, 而非一个 int. <br>
`Array<int> a(10)` : 10 可以隐式类型转换成为一个临时的ArraySize对象, 该对象正是 Array<int> 构造函数需要的, 可以做. <br>
`a == b[i]` : 编译器不能考虑将int转为一个临时性的ArraySize对象, 然后根据这个临时对象产生Array<int> 对象, **那将调用两个用户定制的转换行为**.<br> 
这种做法其实是proxy技术的一个特殊实例.

### Item 6: 区别 increment/decrement 操作符的前置和后置形式

前置和后置操作符increment/decrement, 都没有参数, 这个语言学的漏洞只好让后置式有一个int作为参数.
```C++
class UPInt { // "unlimited precision int"
public：  
    UPInt& operator++()； // prefix ++
    const UPInt operator++(int)； // postfix ++
    UPInt& operator--()； // prefix --
    const UPInt operator--(int); // postfix --
    UPInt& operator+=(int); // a += operator for UPInts and ints
    ... 
};
```
前置的意思是 increment and fetch(累加然后取出),后置的意义是 fetch and increment(取出之后再累加). 
```C++
// prefix form： increment and fetch 
UPInt& UPInt::operator++() { 
    *this += 1； // increment
    return *this; // fetch
}
// postfix form： fetch and increment
const UPInt UPInt::operator++(int){
    const UPInt oldValue = *this； // fetch
    ++(*this)；         // increment
    return oldValue； // return what was fetched
}
```
后置操作符并未调动其参数, 参数的唯一目的是为了区别前置和后置形式.<br>
两者都是会改变 _UPInt_ 的值, 所以不能声明为const成员函数, 而将后置形式的函数的返回值声明为const UPInt, 那么这个动作 `i++++` 就是非法的(返回的一个oldValue无法再被修改), 这是因为, 内置的int就不允许后置的两个 `++`, 因为第二个 `operator++` 所改变的对象是第一个 `++` 操作符所返回的对象,而不是原对象. 因此设计为const, 返回的对象是一个const对象, 不能调用 `operator++` 这种 non-const的成员函数.<br>
单从效率的角度, 用户也更应该喜欢前置式的increment, 除非需要后置increment的行为. 如何保证两者的行为一致, 具体的做法是: **后置的操作应该以其前置的兄弟为基础, 这样的话我们只需要维护前置式的版本**, 后置的版本会自动调整为一致的行为.

### Item 7: 千万不要重载 &&, || 和 , 操作符
C++对布尔表达式采用骤死式评估方式, 但是C++允许用户为自定义的类型定制 `&&` 和 `||` 操作符, 这样的话函数调用语义将取代骤死式语义, 从根本层面改变规则. <br>
如果重载 operator&&, 那么下面的式子将会被理解为后面两者之一:
```C++
if (expressionl && expression?) ... 
// when && is a member function
if (expression1.operator&&(expression2)) ... 
// when && is a global function
if (operator&&(expression1, expression2)) ... 
```
函数调用和骤死式语义有两个重要的区别:
* 函数调用动作被执行, 所有参数的值都必须评估完成
* C++语言的规范并未明确规定调用函数动作中各个参数的评估顺序, 而骤死式总是从左往右评估

同样的, 如果表达式内含 `,`, 那么逗号左侧将会先被评估(evaluating), 然后是右侧, 而整个包含逗号的表达式求值的结果取得是逗号右边的值.<br>
**把操作符写成一个non-menber function, 绝对无法保证左侧表达式一定比右侧更先被evaluating**, 这样的话非成员函数的做法就不可行. 唯一剩下的可能是写成一个成员函数,但是依旧不能保证能像之前逗号的那种行为.<br>
这是不能被重载的操作符:<br>
![figure](figure/7.1.png)<br>
这些操作符则可以重载:<br>
![figure](figure/7.2.png)<br>
即便如此, 如果没有好的理由要去重载某个操作符, 那就不要这样做.

### Item 8: 了解不同意义的new和delete
new opetator是语言内建的, 不能改变其意义只能做相同的事情,无论如何都无法改变其行为 它的动作分成两个方面:
1. 分配足够多的内存,用来放置某种类型的对象
2. 调用一个构造器, 为刚分配的内存中的那个对象设定初值

我们可以改变的是其中的1, 可以重写或者重载那个函数,函数名为 operator new, 通常的声明是`void * operator new(size_t size)`, 此外我们可以重载这个`operator new`, 加上额外的参数, 但是**第一参数的类型必须总是 size_t**.<br>
operator new 也可以直接调用, 像malloc一样, 唯一的任务就是分配内存. `string *ps = new string("memory management")` 实际上反映的是以下的行为:
```C++
void *memory = operator new(sizeof(string));
call string::string("memory management") on memory;
string *ps = static_cast<string*>(memory); 
```

#### Placement new
偶尔会有一些分配好的内存, 我们需要在上面构建对象, 有个特殊版本的 operator new, 称为placement new,允许我们这么做. 
```C++
class Widget {
public:
    Widget(int widgetSize)；
    ...
};
Widget * constructWidgetInBuffer(void *buffer,
                                 int widgetSize){
    return new (buffer) Widget(widgetSize)；
}
void * operator new(size_t, void *location){
    return location；
}
```
在这个构建Widget的过程中, 对象必须置于特定的地址, 或者特殊函数分配出来的内存上. 于是我们写出几种new 的区别:
* 希望对象产生于堆中, 使用 new operator, 分配内存并且为该对象调用一个构造器
* 只是打算分配内存, 调用operator new, 没有构造函数被调用
* 打算在heap对象产生时候自己决定内存分配方式, 使用 new operator并且调用自己写的operator new.
* 打算在已经分配内存(拥有指针)上构造对象, 使用placement new

#### 删除与内存释放
内存释放是由函数opearator delete执行,通常的声明类似于 `void operator delete(void *memoryToBeDeallocated)`, 于是 `delete ps` 会造成编译器产生类似于下面的代码:
```C++
ps->~string(); // 调用析构函数
operator delete(ps); // 释放对象所占用的内存
```
于是打算处理原始的未设置初值的内存, 应该避免使用 new operator和 delete operator, 改调用operator new 获得内存 和 operator delete归还系统.(相当于在C++中调用`malloc`和`free`).

#### Arrays
1. 如果面对尚未支持 `new[]` 的编译器, 定制数组内存管理行为往往不是一个理想的决定
2. 数组版本与单一对象版的区别是, 他所调用的构造器的数量, array new必须针对数组中的每一个对象调用一个构造器
    ```C++
    string *ps = new string[10]; // 调用operator new[] 分配可以放下十个string的内存
                                 // 然后对每个element 调用构造器
    delete [] ps;                // 对每个string 调用析构函数
                                 // 然后再调用opereator delete[] 删除array的内存
    ```

我们也可以重载 operator new[] 不过两者的重载有着相同的限制.<br>
总的来说, **_new / delete operator_ 的任务已经被语言规范限制死了, 但是我们可以修改它完成任务的方式**.

## 异常
C++增加了异常特性之后, 深远而根本地改变了很多事情, 原始指针如今成了一种高风险的事情,我们必须更加小心, 防止程序在执行时候突然中止. 而程序之所以在异常出现的时候仍然有良好的行为, 不是因为碰巧如此, 而他们加入的异常的考虑. <br>
使用返回错误码的方式发出异常信号, **无法保证此函数的调用者会检查那个变量或者那个错误码**,  于是程序可能会一直运行下去, 远离错误发生地点. 如果是以抛出exception的方式发出异常信号, 如果异常未被捕捉, 程序便会立刻终止.<br>

### Item 9: 利用destructors避免资源泄漏
使用指针可能会有很大的问题, 如果我们在下图所示的类结构中, 一次次处理不同的类的实例:<br>
![class heri](figure/9.1.png)<br>
```C++
void processAdoptions(istream& dataSource) {
    while (dataSource){                 // while there’s data
        ALA *pa = readALA(dataSource) ; // get next animal
        pa->processAdoption();          // process adoption
        delete pa;                      // delete object that readALA returned
    }                                   
}
```
这里需要注意的是, 我们在每次得带的最后, 需要删除 _pa_, 否则很快就会出现资源泄漏的问题. 如果 `pa->processAdoption()` 这行抛出异常, 而函数`processAdoptions`无法捕捉它, 那么这个异常就会传播到函数的调用端, `pa->processAdoption()` **之后的语句都会被跳过, 无法执行** . 要避免这个也十分简单, 在那句上加入 `try{...} catch(...){delete pa; throw;}`, 这个简单的处理让我们**被迫重复撰写可被正常路线和异常路线共享的清理代码**, 这里指的是 delete这个动作. <br>
解决的办法很简单, 只要将 "一定得执行的清理代码" **转移到 _processAdoptions()_ 函数的某个局部对象的析构函数中就可以, 用一种行为类似于指针的对象(但是动作更多)取代pa, 这种对象被我们称为智能指针**.<br>
智能指针背后的实现十分复杂, 但是背后的观念十分清楚: <font color=red> 用智能指针取代原始指针, 就不用担心heap objects 没有被阐述, 即使是在异常被抛出的情况下</font>.
```C++
void processAdoptions(istream& dataSource) {
    while (dataSource) { 
        auto_ptr<ALA> pa(readALA(dataSource)); 
        pa->processAdoption(); 
    }
}
```
只要坚持这个原则, 将资源封装在对象中, 通常便可以在 exceptions 出现的时候避免资源泄漏, 但是如果异常是在你正在取得资源的过程中抛出的, 需要其他的特殊设计.<br>

### Item 10 : 在constructor内阻止资源泄露
```C++
BookEntry::BookEntry(const string& name, 
                     const string& address, 
                     const string& imageFileName, 
                     const string& audioClipFileName)
: theName(name), theAddress(address), 
  theImage(0), theAudioClip(0) {
    if (imageFileName != "") { theImage = new Image(imageFileName); }
    if (audioClipFileName != "") { theAudioClip = new AudioClip(audioClipFileName); }
}
BookEntry::~BookEntry() {
    delete theImage; 
    delete theAudioClip; 
}
```
这样一个通讯簿系统, 看起来是安全的,如果 _imageFileName_ 是空的, 那么被初始化为null指针的在被删除时候, C++ 可以保证删除 null 指针是安全的.<br>
但是, 如果执行BookEntry的构造函数的时候有异常被抛出:
```C++
if (audioClipFileName != "") { 
    theAudioClip = new AudioClip(audioClipFileName); 
}
```
可能是new operator 在发生 operator new的时候无法分配足够的内存, 可能是 `AudioClip` 构造器本身抛出一个 exception, 最终异常都会传播到到 BookEntry 对象的那一端没这时候谁来删除 theImage 已经指向的那个对象? 控制权已经转移到了 `BookEntry` 上, 但是其析构函数绝对不会调用, **因为C++只会析构已经完成的对象, 构造函数执行完毕之后才能算是完成的构造.**<br>
```C++
void testBookEntryClass(){
    BookEntry *pb = 0；
    try {
        pb = new BookEntry( "Addison-Wesley Publishing Company", "One Jacob Way, Reading, MA 01867");
        ...
    }
    catch (...){ // catch all exceptions
        delete pb; // delete pb when an exception is thrown
        throw; // propagate exception to caller
    }
    delete pb; // delete pb normally
}
```
上面用 try-catch的方法, 看似是在异常发生的时候可以删除pb, 但是实际上 Image Object依旧是泄漏了, assignmentd的动作其实也不会施加于 pb 身上, 使用 smart pointer `auto_ptr<BookEntry>` 取代原来的裸指针也不会让情况有所好转. <br>
要用让未完全构造好的对象调用析构函数, 必须在对象内部哪些数据身上附带某种指令, 指示构造器进行到什么程度, 如此繁重的工作**会降低构造器的速度, 使得每一个C++对象变得庞大**, C++为了效率不这么做, 那么我们就必须自己设计 constructor, 使得在异常发生的情况下依旧能够自我清理, 通常 **只需要将所有可能的异常捕捉起来, 执行某种清理工作, 然后重新抛出异常使得异常可以传播出去即可.**<br>
```C++
BookEntry::BookEntry (  const string& name,
                        const string& address,
                        const string& imageFileName,
                        const string& audioClipFileName)
:theName(name), theAddress(address),
 theImage(0), theAudioClip(0){
    try { // this try block is new
        if (imageFileName != "") {
            theImage = new Image(imageFileName);
        }
        if (audioClipFileName != "") {
            theAudioClip = new AudioClip(audioClipFileName);
        }
    }
    catch (...) {        // catch any exception
        delete theImage;     // perform necessary
        delete theAudioClip; // cleanup actions
        throw;           // propagate the exception
    }
}
```
可以看到, 如果 BookEntry的构造函数本体开始执行的时候, 四个对象已经完全构造好了, 所以 BookEntry对象被销毁, data member 的销毁无需我们介入. 此外可以看到处理异常时候的代码和析构函数中的代码可能有所重合, 那么我们可以将他们写成一个 private的成员函数, 然后在构造和析构的过程中调用它.<br>
现在剩下一个最后的问题, theImage 和 theAudioClip 变成一个常量指针(只能在成员初始化列表中初始化), 带来的问题就是 <font color=red> 在初始化过程中发生异常, _theImage_ 所指对象不会被销毁, 我们也无法再构造函数本体中加入try/catch 语句解决问题, 他们是语句而初始化列表只接受表达式 </font>. <br>
* 一种**解决的具体做法就是将 Try/catch 语句放在一个Private的成员函数中, 然后初始化列表中调用该函数**.带来的问题就是将构造函数完成的动作分布到多个函数中, 造成维护上的困难.
* 另一种做法就是将 theImage 和 theAudioClip 所指的对象视为资源, 使用局部的对象(智能指针)
    ```C++
    class BookEntry {
    public:
    ... // as above
    private：
    ...
        const auto_ptr<Image> theImage;         // these are now
        const auto_ptr<AudioClip> theAudioClip; // auto_ptr objects 
    };
    ```

### Item 11: 禁止异常( _exceptions_)流出析构函数(_destructprs_)之外
两种情况下析构函数会被调用,我们无法在析构函数内区分这些状态:
* 对象正常情况下被销毁(离开自己的生命周期或者被明确地销毁)
* 对象被 exceptions的处理机制(传播过程中的 stack-unwinding,栈展开机制) 销毁

如果**控制权基于exception的因素离开析构函数, 而此时正有一个exception处以作用状态, C++便会调用 terminate 函数, 将程序结束, 甚至不等局部对象被销毁**. <br>
```C++
class Session {
public:
    Session();
    ~Session();
    ...
private:
    static void logCreation(Session *objAddr);
    static void logDestruction(Session *objAddr);
};
Session::~Session(){
    logDestruction(this); // 这里可能抛出异常
}
```
在log函数中抛出一个异常, 这个异常并没有被对象的析构函数捕获, 所以会传到析构函数的调用端, 万一异构函数本身也是因为某个异常被调用的, terminate函数便会被自动调用. <br>
阻止异常传出构造器之外的唯一办法, 是使用 try-catch 语句, 如果在catch语句中依然有代码, 其中依然可能导致异常的产生:
```C++
Session::~Session(){
    try {
        logDestruction(this);
    }
    catch (...) {}
}
```
这样取代的办法就是, 如果logDestruction函数抛出一个异常, 我们便把忘记记录析构这个任务.<br>
如果exception从析构函数中抛出, 而且没有在当地捕捉, 那么析构函数便是执行不全, 没有完成析构函数应该做完的所有事情,可能造成资源泄漏, 因此我们应该 **全力阻止exception传出析构函数外**. 

### Item 12 : 了解抛出一个异常 与 传递一个参数 或者调用一个虚函数之间的差异

函数参数声明的语法与catch 子句声明的语法相同(可以 by-value, by-ref, by-pointer),但是 **从抛出端传递一个exception到catch语句 和 从函数调用端传递一个自变量到函数参数** 两者之间有重大的不同点.<br>
当我们调用一个函数, 除非函数失败以至于无法返回, 控制权最终还是会回到调用端, 但是当我们抛出一个exception, 控制权不会再回到抛出端. 
```C++
istream operator>>(istream& s, Widget& w);
void passAndThrowWidget(){
    Widget localWidget;
    cin >> localWidget; // pass localWidget to operator>>
    throw localWidget; // throw localWidget as an exception
}
```
`operator>>` 的reference w绑定到 localWidget 上, **而无论是by-value 还是 reference 传递异常, 都会发生 localWidget 对象的复制, 而交到catch手上的正是那个副本**. 即使是抛出的异常没有被瓦解的风险, 如一个静态变量, 抛出 exception还是会产生一个副本, 复制的过程是由异常对象的 cony constructor执行的, 相应于对象的"静态类型" 而不是 "动态类型":
```C++
class Widget { ... };
class SpecialWidget: public Widget { ... };
void passAndThrowWidget(){
    SpecialWidget localSpecialWidget;
    ...
    Widget& rw = localSpecialWidget; // rw refers to a SpecialWidget
    throw rw; // this throws an exception of type  Widget
}
```
这和其他所有的 C++复制对象情况一致, **复制动作永远是以对象的静态类型为本**.因此下面的两个catch语句看起来相同， 但是依旧是有区别的：
```C++
catch (Widget& w) {
    ...     // handle the exception
    throw； // rethrow the exception so it
}           // continues to propagate
catch (Widget& w) {
    ...       // handle the exception
    throw w； // propagate a copy of the
}             // caught exception
```
一般而言， 我们只有使用`throw;`才能重新抛出当前的exception, 同时也比较有效率. 如果最初的异常类型是 _SpecialWidget_, 第二个语句块抛出一个新的异常, 类型是 _Widget_, 因为那是`w`的静态类型. 下面三种catch语句, 都可以捕捉 _passAndThrowWidget_ 抛出的 _Widget exception_, 于是抛出异常和传参之间的另一个区别, 一个被抛出的对象(必定为临时对象) 可以简单用 by-reference捕捉, 但是 **函数调用过程中将一个临时变量传递一个 `non-const reference` 参数是不允许的, 但是对 exception是合法的**.<br>
抛出exceptions和函数传递参数相比, 会多构造一个 "被抛出物" 的副本, 如果是 `by-value` 的方式传递异常, 还要将临时对象再拷贝到 `w` 上.<br>
throw exception `by-pointer` 有一个很大的问题, 是传出一个局部对象的指针, 在异常的的时候离开局部对象的 scope, 对象被销毁, 因此 catch 子句会获得一个指向"已被销毁的对象"的指针, 这是义务性复制规则设计下要避免的情况.<br>
#### exception 与 catch 子句相匹配
这个过程中只有两种类型转换是可以允许的, 不同于函数传参中可以发生各种类型转换:
1. **继承构架中的类转换**: 一个针对 base class exception 而编写的 catch子句, 是可以处理类型为 derived class 的 exceptions. 
    ![exception inheritance](figure/12.1.png)<br>
2. **有型指针转为无型指针**: 一个针对 `const void*` 设计的catch语句可以捕捉任何指针类型的 exception.

此外, catch语句总是依出现的顺序做匹配尝试, try语句中同时有针对 base class 和 derived class 设计的catch语句时候, derived class exception依旧可能被 针对 base class 设计的catch 子句处理掉,就是**相当于采用first fit的策略. 而我们调用虚函数的时候, 采用的是 best fit 的策略**,因此我们绝对不要将base 的 catch放在derived之前.