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
