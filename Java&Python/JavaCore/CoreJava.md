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
* C 中的右移运算都是用 `>>`(实际上是为 unsigned 定义的), 但是在 Java 中 `>>>` 用0 填充高位, `>>` 用sign bit 填充高位
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
需要设置实例域的值, 应该提供 私有的数据域, 公有的域访问器方法, 公有的域更改器方法. 这样做比直接公开数据域复杂, 但是可以有很多好处:
1. 可以改变内部实现, 除了改变类的方法外不会影响其他代码
2. 更改器方法可以执行错误检查, 而直接对域进行赋值不能做这个处理

#### 基于类的访问权限
如比较两个雇员是否为同一人:
```Java
Class Employee{
    ...
    boolean equals(Employee other){
        return name.equals(other.name);
    }
    ...
}
if( peter.equals(boss)) ...
```
equals 方法不仅可以访问 peter 的私有域, 还能访问 boss 的私有域, 这都是合法的, <font color=blue> 因为类的方法可以访问同类的任何一个实例的私有域</font>.<br>

#### 私有方法
很多时候可以将方法设为私有, 如一个计算代码划分为多个辅助的方法, 通常这些方法不应该成为公有接口的一部分, 因为他们往往与当前的实现机制关系非常紧密, 或者需要一个特别的协议以及一个特别的调用顺序.如果数据的表达方式发生变化, 这些私有方法不再需要的时候可以删去它, 因为类外的其他方法不可能直接调用他们. 

#### Final 实例域
将实例域设为final, 构建对象时必须初始化这样的域, 构造器执行后, 其值被设定, 在后面的操作中不能对它进行修改. **一般这种用于基本数据类型 或者不可变类(如 sring)的域, 修饰可变类可能给阅读程序带来混乱.**

### 静态域和静态方法
#### 静态域
如果将域定义为 `static`, 每个类中只有一个这样的域. 普通的域则是类的每一个对象都有一根自己的拷贝. <br>
静态变量用的比较少, 一般是用一个静态的常量, 如 $\pi$ 在 Math 类中可能是这样表示的:
```Java
public class Math {
   ...
   public static final double PI = 3.14159265358979323846;   
   ...
}
double myAera = 3 * 3 * Math.PI;
```

#### 静态方法
可以认为静态方法是不带 this 参数的方法, 静态方法不能操作对象, 所以不能再静态方法中访问实例域, 一般是在下面两种情况下使用静态方法:
* 一个方法不需要访问对象状态, 需要的参数都是通过显式参数的方式提供('Math.pow()')
* 一个方法只需要访问类的静态域

<font size = 4> Factory 方法</font>: 静态方法的另一种常见用途, 产生不同风格的格式对象, 如下面的代码, 使用的原因有两个: 

```Java
NumberFormat currencyFormatter = NumberFormat.getCurrencylnstance();
NumberFormat percentFormatter = NumberFormat.getPercentInstance();
double x = 0.1;
System.out.println(currencyFormatter.format(x)); // prints $0.10
System.out.println(percentFormatter.format(x)); // prints 10 %
```
* 无法命名构造器
* 使用构造器的时候无法改变构造的对象类型

#### Main 方法
不需要使用对象调用静态方法, 而 main 也是静态的, 在启动程序前没有任何一个对象. 值得注意的是, 每一个类可以有一个 Main 方法, 这常常用于进行单元测试.如一个 StaticTest.java 中有两个类, Employee 和public的StaticTest, 那么就可以分别执行
```Shell
java Emplpyee
java StaticTest
```
分别执行两个 main 方法. 

### 方法参数

Java 程序设计语言总是采用值调用的方式, 方法得到的是参数值得一个拷贝, 不能修改传递给它任何参数变量的内容, 方法参数工有两种类型, 基本数据类型和对象引用, 他们作用的方式如下图:
* 基本数据类型<br>
    ![](figure/Core4.1.png)
* 引用类型(对象的引用 和 它的拷贝引用同一个对象)<br>
    ![](figure/Core4.2.png)

Java 中对对象采用的不是引用调用, **而是引用进行的值传递, 因此 一个方法不能实现让对象参数引用一个新的对象**.

### 对象构造
对象的构造非常重要, Java 提供了多种编写构造器的方式. 如果过个方法有相同的名字同时参数类型不同, 便产生了重载. 如果编译器找不到匹配的参数, 或者找出多个可能的匹配, 就会产生编译时错误. 

#### 默认初始化
如果在构造器中没有显式地给赋予初值, 那么被自动地赋为默认值: 数值为0, 布尔值为 false, 对象引用为 null. 但一般不建议这么做, 不明确地初始化, 会影响代码的可读性.

#### 默认构造器
指的是没有参数的构造器,  和 C++ 一样, 如果在编写一个类的时候没有编写构造器, 系统就会提供一个默认构造器, 该构造器将所有的实例域设置为默认值. 但是类中提供了至少一个构造器, 但是没有提供默认构造器时, 构造对象不提供参数就会被视为不合法. 

#### 显式初始化
可以在类的定义中, 将一个值赋给任何域, 执行构造器之前, 先执行赋值动作, 初始值不一定是常量:
```Java
class Employee {
   private String name = ""; 
   ...
}
```

#### 调用另一个构造器

关键词 this 还有另一个含义, 可以用来调用同一个类的另一个构造器, 例如:
```Java
public Employee(double s)
{        
    // calls Emiployee (String, double)    
    this("Employee #" + nextId, s);
    nextId++;    
}
```
此外还有一种是初始化语句块, 在类的声明中可以包含多个代码块, 只要构造对象, 这些块就会被执行. 对类的静态域进行初始化的代码比较复杂, 可以采用 静态的初始化块. 

```Java
public Employee(double s)
{        
    ...
    static{
        Random generator = new Random();
        nextId = generator.nextInt(1000);
    }
}
```
#### 对象析构与 _finalize_ 方法
由于Java 有自动的垃圾回收器, 不需要人工回收内存, 所以 Java 不支持析构器. 某些对象使用了内存之外的资源, 当资源不需要的时候将它回收和再利用就比较重要. **可以为一个类添加 finalize() 方法, 它将在垃圾回收器清除之前调用**.

### 包
Java 允许使用包将类组织起来, 使用包的主要原因是确保类名的唯一性. 

#### 类的导入
可以使用 import 语句导入一个特定的类或者整个包:
```Java
import java.util.*;
import java.util.Data;
// 发生命名冲突时,如 sql 和 util 都有date 类
import java.util.*;
import java.sql.* ;
import java.util.Data; // 只使用 Java.util中的 Data
// 两者都要使用的时候, 在每个类前面加入完整的包名
java.util.Data deadline = new java.util.Date();
java.sql.Date today = new java.sql.Date(...);
```

#### 静态导入
import语句还可以导入静态方法和静态域, 常见的两个实际运用在于
* 算术函数, 对math 类进行静态导入, 更自然的方式使用算术函数
    ```Java
    // import
    Math.sqrt(Math.pow(x,2)+Math.pow(y,2));
    // Static Import
    sqrt(pow(x,2)+pow(y,2));
    ```
* 笨重的常量(大量名字冗长的常量, 用静态导入比较方便)

#### 将类放入包中
![](figure/Core4.3.png)<br>
在 Employee.java 中可以这样写:
```Java
package com.horstmann.corejava;
public class Employee {
    ...
}
```
编译器在编译文件的时候不检查目录结构, 但是最终 虚拟机找不到类包 可能会导致程序无法运行. <br>
如果没有指定 private 或者 public, 这个部分的方法或者变量可以被同一个包中所有方法访问.<br>

### 类路径
为了使类可以被多个程序共享, 需要做到下面几点
1. 把类放在同一个目录中
2. 将Jar放在一个目录中(如 /home/user/archives)
3. 设置类路径(class path)

Javac 编译器总是能在当前的目录中查找文件, 但是 java 虚拟机仅在classpath 有 "." 才查看当前目录, 默认没有设置classpath 是没有问题的, 但是如果设置的 classpath 而 classpath 不包含 . 目录时, 程序依旧可以通过编译, 但是无法正常运行.
``` bash
// unix
java -classpath /home/user/classdir:.:/home/user/archives/archive.jar MyProg
// windows
java -classpath c:\classdir;.;c:\archives\archive.jar MyProg
```

### 文档注释
JDK 中包含 javadoc, 可以从源文件中生成一个 Html的文档, 它从下面的特性中抽取信息
* 包
* 公有的类与接口
* 公有的和受保护的方法
* 公有的和受保护的域

类注释: 放在 import 语句后, 类定义之前<br>
方法注释: 通用标记外还能有三种标记(@param variable description,@return description, @throws class description )<br>
通用注释: @author name, @see tag ...
### 类的设计技巧

1. 一定要将数据设计为私有
2. 一定要对数据进行初始化: 最好不要依赖于系统的默认值
3. 不要在类中使用过多的基本数据类型
4. 不是所有的域都需要独立的访问器或者域修改器
5. 采用标准格式进行类的定义
6. 将职责过多的类进行分解
7. 类名和方法名要体现其职责

## Chap 05: Inheritance

继承已经存在的类就是附中这些类方法和域, 在此基础上还可以添加一些新的方法和域, 以满足程序设计的要求.<br>
反射指的是在程序运行期间发现更多的类及其属性的能力, 可以先浏览
//TODO:日后返回来学习.

### 类 超类 和 子类
经理和雇员之间明显存在着 `is-a` 关系(继承的主要特征), 于是我们可以有以下代码:
```Java
public class Manager extends Employee{
    // added methods and fields
}
```
<font color=red> java 中的所有继承都是公有继承, 不存在 C++ 中私有继承,保护继承的概念</font>. 已经存在的类被称为 超类/父类/基类, 新类型被称为 子类/派生类. **子类比超类拥有的功能更加丰富**, 超和子(super-/sub-) 来源于集合论中的术语, 不表示功能多少. <br>
通过拓展超类的方式定义子类的时候, 只需要指出两者的不同之处. 超类的某些方法对于子类不一定使用, 这时候需要用覆盖的方式替代(区别于重载).例如一个 Manager 的工资可能还有 bonus, 需要重载 getSalary 方法:

```Java
public double getSalary() {
   return salary + bonus; // won't work 
}
```
而 **Manager 类无法直接访问超类的私有域**, 就必须从公有方法的接口访问, 如果 getSalary() 调用 getSalary(), 就会造成无限调用自己, 最终的解决方案是 <font color=red>引入了 _super_ 关键字 </font>.

```Java
public double getSalary(){
    double baseSalary = super.getSalary();
    return baseSalary + bonus;
}
```

super 和 this 不同, 它不能理解为一个对象的引用, 只是提示编译器调用超类的方法. 在 C++ 中是使用 `superClassName::method()` 的方式调用. 此外, 可以在构造器中使用 super. 

```Java
public Manager(String name, double salary, int year, int month, int day)
{
    super (name, salary, year, month,);// 调用 super 的构造函数
    bonus = 0; 
}
```
如果子类没有显式调用超类的构造器, 那么将调用超类默认的构造器, 如果超类没有带默认构造器(没有自定义构造器时会自动生成), 那么 Java 编译器会报告错误.运用场景 this 相似, 有两个用途, 其一是隐式类型参数, 其二是调用类的其他构造器.在 C++ 中可以用列表初始化调用父类的构造器, <br>
```Java
Manager::Manager(String name, double salary, int year, int month)          
:Employee(name, salary, year,      month,     day)
{
    bonus = 0; 
}
```
使用的时候, 可以让 Employee 类型的引用变量引用 Manager 的对象(多态), 在运行时自动选择调用哪种方法(动态绑定).<font color=blue> 在 Java 中, 不需要将方法声明为虚拟方法, 动态绑定是默认的处理方式, 如果不希望一个方法具有虚拟特性, 可以将它标志为 _final_ .</font> <br>

#### 继承层次与多态
继承并不一定仅限于一个层次, 层次继承中从某个特定的类型到其祖先的路径被称为该类的 **继承链**. 但是 <font color=red> Java 不支持多继承, 只能通过继承超类 + 实现接口 实现事实上的多继承</font>.<br>
继承体系中有 "is-a" 关系, 即每个对象都是其超类的对象, 但是反过来不成立, 另一种表述就是 "置换原则", 在程序中使用超类对象的任何一个地方都可以用子类对象替换. 对象变量是多态的, 一个 Employee 变量既可以引用 Employee 对象, 也可以引用 Manager 对象. 

```C++
Manager boss = new Manager("Carl Cracker", 80000, 1987, 12, 15); 
boss.setBonus(5000);
Employee staff = new Employee[3]; //OK
staff[0] = boss; 
staff[0].setBonus(5000); // ERROR
```

staff 存的是一个 Employee 类型的对象, **不能直接调用 setBonus 的方法, 需要做强制类型转换之后才能使用**.

#### 动态绑定

首先回顾一下调用过程的详细描述:
1. 