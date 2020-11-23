# Java 核心技术 vol 1 : fundamentals
By [Cay Horstmann](https://horstmann.com/corejava/)

## Chap 01 : Intro to java

java 并不是一种程序设计语言, 而是一个完整的平台, 有一个庞大的库,包含了很多可以重用的代码和一个完整的提供诸如安全性\跨操作系统的可移植性以及自动垃圾收集等服务的执行环境. <br>
面向对象是一种程序设计技术, 它将重点放在数据(对象) 和对象的接口上.Java 和 C++的主要不同点在于多继承, 在java中取而代之的是简单的接口(interface)概念. <br>
**体系结构中立**: 编译器生成的是一种体系结构中立的目标文件格式, 一种编译过的代码, 只要有java的运行时系统, 就可以在许多处理器上运行. 解释字节码比全速运行机器指令慢很多, 但是可以将最频繁的字节码序列翻译成为机器码, 这个过程称为 **即时编译**(Just-In-Time).<br>
Java 规范中没有依赖具体实现的地方, 基本数据类型的大小以及相关的算法都有明确地说明. 此外, Java 把多线程的实现交给了底层操作系统或多线程库来完成.<br>
**动态性**: java 与 C/C++ 相比更加具有动态性, 库中可以自由地添加新方法或者实例变量,对客户端却没有影响, 在Java中得到运行时类型信息十分简单.<br>

## Chap 02 : Java 编程环境

| Name | 简称 |   解释      | 
| ---- | ---- |  --------- |
| Java Development Kit| JDK | 编写 Java 的程序员使用的软件| 
| Java Runtime Environment | JRE | 运行 Java 程序的用户使用的软件|
| Standard Edition | Java SE | The Java platform for use on desktops and simple server applications |
| Enterprise Edition| Java EE | The Java platform for complex server applications |
| Micro Edition| Java ME | 用于微型手机和其他小型设备的Java平台| 

安装 Java 开发工具箱
* 下载 JDK
* 设置执行路径如在 Linux 的 shell rum command中加入 `export PATH=jdk/bin:$PATH`
* 安装 源代码和库文件 `jdk-doc`, `wget https://horstmann.com/corejava/corejava.zip`
* 命令行界面/ IDE 编写程序

## Chap 03 : java 基本的程序设计结构

对于一个 C++ 程序员员而言, 更需要关注的是两者之间的区别.<br>
* java 语言规范, main 方法必须设置为 public
* java 中所有的函数(方法) 都属于某个类, 因此 main 方法必须有一个外壳类, main 方法设为 void 没有给操作系统返回退出代码, 正常的情况下退出代码为0, 如果要在终止程序时候 **返回其他代码, 调用 `System.exit()` 方法**
* java 为强类型语言, 一共有 8 种基本数据类型, 4种整形. 2种浮点类型 1种用于表示unicode 的 char 还有一种 Boolean, 如果需要在数值计算中不含任何舍入误差, 就应该用 `BigDecimal` 类
* C++ 中指针或者数值可以替代 boolean 值, 但是 java 不行
* C++ 中区别声明与定义一个变量, 但是Java 不做这种区分
    ```C++
    int i = 10 ; // C++ 定义一个变量
    extern int i ; // C++ 声明一个变量
    ```
* Java 中用关键词 `final` 声明一个常量, 表示该变量只能被赋值一次, 如果希望某个常量可以在一个类中的多个方法使用, 可以设置为 `static final`, const 是 Java 保留字, 但是没有使用 
* C 中的右移运算都是用 `>>`(实际上是为 unsigned 定义的), 但是在 Java 中 `>>>` 用0 填充高位, '>>' 用sign bit 填充高位
* Java Math 类中的方法(全都是静态方法) 都是使用 计算机浮点单元中的例程, 如果可预测结果比速度重要的话应该用 `StrictMath` 类
* 数值类型之间的相互转换<br>
    ![](figure/Core3.1.png)<br>
    注意的是不要在 boolean 类型和任何数值类型之间做强制类型转换
* 自定义枚举类型的使用方法如下<br>
    ```Java
    enum Size {Small, Medimum, Large, ExtraLarge};
    Size s = Size.Small;
    ```
* 在 Java 中, 字符串是不可修改的, 只能用 `substring()` 这个方法将老字符串的部分取出来拼接, 这样 <font color=red> 编译器可以让字符串共享 </font>. C++ 重载了操作符 `==` , 但是 Java 中判断两个string 是否相等用的是 `str1.equals(str2)`.java 字符串使用 char 序列构成, 大部分常用的 Unicode 字符使用一个代码单元就可以表示, 辅助用的则是采用 一对. 
* 有时候需要使用较短的字符串构建一个新的字符串, 采用连接的方式效率比较低, 每次都要创建一个新的 string 对象, 使用 StringBuilder 就可以, 添加的时候调用类的 .append(strTail) 方法. 
* 控制台输入是 创建一个 `Scanner` 对象, 并且与 `System.in` 关联,  因为输入可见, 不适合从控制台读取密码, Java SE 6 引入了 `Console` 对象, 格式化输出则沿用了 C 语言库函数 `printf` 的方法
* 文件输入与输出, 就要用 File 对象构造一个 Scanner 对象:
    ```Java 
    Scanner in = new Scanner(Path.of("myfile.txt"), StandardCharsets.UTF_8);
    PrintWriter out = new PrintWriter("myfile.txt", StandardCharsets.UTF_8);
    ```
    如果Scanner 文件找不到的话 或者 PrintWriter 无法创建就会抛出异常 `IOException`.
* 块作用域, 在 C++ 中可以在嵌套的块中重新定义同名变量, 内层会覆盖外层定义的变量, 使用 `::` 可以访问全局定义的该变量, 但是在 Java 中不允许怎么做. 
* Switch 语言的标签 必须是 **整数或者枚举变量**, 不能测试字符串
* 若 基本的整数和浮点数不符合要求, 可以使用 `BigInteger` 和 `BigDecimal` 类, 使用时不能用我们常见的运算符, 因为语言没有重载该运算 Java 程序员没有重载运算符的权力.
* Java 习惯使用 类型与变量名分开的数组声明方法(`int[] a = new int[100]`), 同时支持 range-based for loop.
* Java数组的拷贝, 直接使用 = 将导致两个变量引用同一个数组, 想要实现深拷贝的效果 使用 `Arrays.copyOf(src, src.length())` 静态方法
* Java 的多维数组不能用 for each 自动处理二维数组中每个元素, 而是按行处理, 同时可以用于不规则数组

## Chap 04: Objects and Classes
### 概述
#### 类
Java 语言是完全面向对象的, 每个对象包含对用户公开的特定功能部分以及隐藏的实现部分. 过去说Algorithms + Data Structures = Programs, 而 **OOP 将数据放在第一位, 确定需要操作的数据, 然后再决定操作数据的算法**. <br>
* **类** 是构造对象的模板或者蓝图, 由类构造对象的过程成为创建类的实例(instance)
* **封装**: 将数据和行为组合在一个包内, 并且对对象的使用者隐藏了数据的实现方式.  数据被称为 实例域(instance field), 操纵数据的过程称为方法(method), 封装的关键在于 **绝对不能让类的方法可以直接地访问其他类的实例域**, 程序仅通过对象的方法与对象数据进行交互. 

#### 对象
对象的三个主要特性:
* 对象的行为: 可以对对象施加哪些操作
* 对象的状态: 施加方法时, 对象如何响应
* 对象的标识: 如何辨别具有相同行为与状态的不同对象

对象的状态必须通过方法的调用实现, 否则说明封装性出现了问题. <br>

面向对象的系统没有所谓的顶部, 我们首先从设计类开始, 然后再往每个类中添加方法. 一般而言 方法对应着动词, 实例域对应着名词. 

#### 类之间的关系

类之间常见的关系有:
* 依赖("use a"): 常见的是一个类的方法操纵另一个类的数据
* 聚合("has a"): 类 A 的对象中 包含类 B 的对象
* 继承("is a"): 表示特殊与一般的关系

### 使用现有类
#### 对象与对象变量
想要使用对象, 首先需要构造对象并且指定其初始状态, 然后对 对象施加方法. <br>
Java 中使用构造器(constructor) 构造对象的实例, 它是一种特殊的方法, 用于构造并且初始化对象. 一个对象变量 如 `Data deadline;` 中的deadline 并没有实际包含一个对象, 而仅仅是 初始化之后引用一个对象. 可以显式将对象变量设置为 null, 表明它目前没有引用任何对象. 这种引用和 C++ 中的引用有很大的区别, 比如 C++ 中没有空引用, 而且引用不能被赋值引用其他对象, 行为上, **Java 中的引用变量更像是 C++ 中的对象指针**. Java 中所有对象都是存储在堆中, 不用担心内存管理问题, 垃圾回收器会处理相关的事宜. <br>

#### 更改器方法和访问器方法
需要查询类的信息应该使用类的 get 方法(访问器方法), set/add 方法是对对象的状态进行修改(更改器方法), 在 C++ 中往往会给访问器方法加 const 修饰符(不改变对象的状态), 但是 **Java 并无明显的区分**. 

### 使用用户自定义类
#### 一个简单的雇员类
文件名必须与public类的名字相匹配。在~个 源文件中，只能有一个公有类，但可以有任意数目的非公有类。
```Java
class ClassName
{
    constructor1;
    constructor2;
    ...
    method1; 
    method2;
    ...
    field1;
    field2;
    ...
}
```
#### 多个源文件的使用
如果我们使用的是多个类如 Employee.java + EmployeeTest.java , 那么在编译的时候可以是用两种方式:
* 使用通配符调用 Java 编译器
    ```Shell
    javac Employee*.java
    ```
* 直觉编译 EmployeeTest, 可以理解为 Java 内置了 make 功能, 可以自动查找依赖的 class , 如果依赖的 .java 有新版本, 则会重新编译输出 .class 文件

#### 从构造器开始

我们看雇员类的构造器:
```Java
public Employee(String n, double s, int year, int month, int day) 
{
    name = n;
    salary = s;
    hireDay = LocalDate.of(year, month, day);
}
new Employee("James Bond", 100000, 1950, 1, 1);
```
构造器和类同名,总是伴随 new 操作符的执行被调用, 不能对一个已存在的对象调用构造器实现 实例域 的设置.<font color=red> C++ 程序员容易犯的错误就是忘记 _new_ 操作符 </font>. 

#### 隐式参数与显式参数
* 隐式参数值得是出现在方法名前的 Employee 对象, 相当于 C++ 中默认传入的 this 指针
* 显式参数就是明确在方法声明中列出的参数

```Java
public void raiseSalary(double byPercent){
    double raise = this.salary * byPercent / 100;
    this.salary += raise; // this 其实可以省略
}
```
C++　和 Java 的类其实还有一个重要的区别, C++ 中通常在类的外面实现成员函数, 类内部的实现自动称为 inline 函数, <font color=red>但是在Java 编程中, 所有的方法都不许在类的内部定义</font>, 是否将方法设置内联是 Java 虚拟机的任务. 

#### 封装的优点
