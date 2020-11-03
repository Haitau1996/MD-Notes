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