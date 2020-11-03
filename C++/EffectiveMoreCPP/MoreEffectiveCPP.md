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
    Equipmentpiece bestPieces[10]；	// error! No way to call  EquipmentPiece ctors
    Equipmentpiece *bestPieces = new EquipmentPiece[10]；	// error! same problem
    ```
    可以有一些方法解决这个问题:
    * 使用Non-heap数组, 提供构造参数生成相应的数组
    * 使用指针数组而非对象数组:
        ```C++
        typedef Equipmentpiece* PEP；	// a PEP is a pointer to // an EquipmentPiece
        PEP bestPieces[10]；	// fine, no ctors called
        PEP *bestPieces = new PEP[10]；	// also fine
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
        operator double() const；	// converts Rational to double
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
class UPInt {	//	"unlimited precision int"
public：		
    UPInt& operator++()；	//	prefix ++
    const UPInt operator++(int)；	//	postfix ++
    UPInt& operator--()；	//	prefix --
    const UPInt operator--(int);	//	postfix --
    UPInt& operator+=(int);	//	a += operator for UPInts and ints
    ... 
};
```
前置的意思是 increment and fetch(累加然后取出),后置的意义是 fetch and increment(取出之后再累加). 
```C++
// prefix form： increment and fetch 
UPInt& UPInt::operator++() { 
    *this += 1；	//	increment
    return *this;	//	fetch
}
// postfix form： fetch and increment
const UPInt UPInt::operator++(int){
    const UPInt oldValue = *this； //	fetch
    ++(*this)；	        // increment
    return oldValue；	// return what was fetched
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
