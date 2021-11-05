# C++ 新经典: 对象模型
## Chap 1: Visual Studio 常见快捷方式
* `F9`: add/remove a break point in current line
* `F5`: 开始调试
* `F10`: 逐过程调试
* `F11`: 逐语句调试
* `Alt + 6`: 调试 -> 窗口 -> 内存 -> 内存1 `&some_var` （查看内存内容）
* `Shift + F9`: 调试 -> 快速监视

## Chap 2: 对象
### 空间占用
* 即使不包含任何成员的类， 在 sizeof operator 操作的结果依然是 1 而不是 0. **对象是有地址的， 在内存中对应的位置保存了 1 个字节的内容**。 
* **类中的成员函数是不占用类对象内存空间的**，（实际上是跟着类走， 和对象关系不大）
* 成员变量是在每个对象中，占用对象的空间

### 对象结构的发展和演化
当下主要的 C++ 对象模型如下：
1. 非静态的成员变量跟着对象走
2. 静态成员变量跟对象没有关系
3. 静态、非静态的成员函数都保存在类对象实例外
4. 虚函数是通过在类对象中添加一个指向虚函数表的指针实现

如果类之间有多重的继承关系， 并且每个父类都有虚函数， 那么子类的情况可能有所不同。  

### `this` 指针
<div align=center><img src="https://i.imgur.com/7LTXVcd.png"  width="40%" height="40%"/></div>

我们使用一个 C 类的对象调用三个函数， 发现 A 和 C 的 this 指针指向同一个位置而 B 则和他们相差一个 sizeof(int):<div align=center><img src="https://i.imgur.com/Zn6l0PX.png"/></div>

这是因为 C 对象有三个部分， 从 A B继承的和本身， 而先从 A 类继承因此 AC 都是从那个地方开始， 而 B 对象则是在中间部分。如果 C 中 override 了 funcB[^overrideFuncB]， 那么调用 c.funcB 结果会是 004FF8F4:
[^overrideFuncB]: override non-virtual 实际上是一个非常不好的做法，因为override 一般是要实现动态绑定， 而 non-virtual 是静态绑定
```C++
c_obj.funcB();// 004FF8E4
c_obj.B::funcB();//004FF8E8
```

### 构造函数
程序员如果没有定义任何构造函数， 编译器会隐式地生成一个默认的构造函数，但是实际上==只有在必要的时候编译器才会合成默认构造函数， 而不是必然或者必须合成==：
1. 该类没有任何构造函数， 但包含一个类类型的成员变量