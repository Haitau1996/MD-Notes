# Java语言程序设计与数据结构
**author: 梁勇**
***
## Chap 1: 计算机、程序和java概述
计算机包含软件和硬件两个部分，硬件的组件通过一个总线(bus)子系统连接。<br>
![bus](figure/1.1.png)<br>
编程语言:机器语言\汇编语言和高级语言.
![prog lang](figure/1.2.png)<br>
**Java语言规范\ API\ JDK\ JRE** : <br>
- API也称为库, 包含开发Java程序而预定义的类和接口, 在[网站](download.java.net/jdk8/docs/api/)上可以看到最新版的API.
- Java有三个版本, Java标准版(Java SE) Java企业版(Java EE) 和 Java微型版(Java ME), 各个版本都带有开发工具包(Java Development Toolkit, JDK), 它由一组独立程序构成, 都是由命令行调用,用于编译\运行和测试Java程序.Java源程序是区分大小写的.

### 创建\编译\运行 java程序
**Java源程序由.java程序文件, 编译生成.class文件, 该文件由java虚拟机(JVM)执行**.拓展名必须为.java,并且文件名和公共类名字相同,具体的执行过程如下:<br>
![run java code](figure/1.3.png)<br>
.class字节码是低级语言, 类似于机器指令, 但是在体系结构中是中立的, 可以在任何带有JVM的机器中运行, JVM不是物理机器,而是一种解释JAVA字节码的程序.他们的行为大致可以用下图表示:<br>
![java bite](figure/1.4.png)<br>
在运行一个JAVA程序的时候, JVM 首先会用一个类加载器(class loader)的程序将类的字节码加载到内存中, 如果程序中还使用到了其他的类, 类加载器会在使用他们之前**动态**加载他们, JVM使用一个称为字节码验证器的程序检查字节码的合法性,确保字节码没有违反Java的安全规范.

## Chap 2: 基本程序设计

* 每一个Java程序都应该有一个main方法, 作为程序运行的入口, 变量名应该使用描述性文字,而不是x和y这种名字. 
* java的基本数据类型: **byte**, short, int, long, float 和 double

## Chap 3: 选择

### boolean 数据类型
保存布尔值的变量称为boolen variable, 只能是true和flase, **并且无法和int类型相互转换**.

### if 分支结构常见问题
* 在if行出现分号, 相当于一个空语句, 后面的语句块无论如何都会运行
* 悬空的else出现错位: 同一个块中, else总是和它最近的If子句匹配
* 浮点值的相等测试

常见的陷阱:
* 简化bool变量赋值
* 避免相同情形下的重复代码

条件操作`boolen-expression ? expression1: expression2;`
java产生随机数的方法`(int)(Math.random()*10)` .逻辑操作符和c++中的情形一直, 也是有!/&&/||/^用于产生复合布尔表达式.

### switch 语句
```Java
switch(status){
    case 0 : // do something
            break;
    case 1 : // do something
            break;
    default: // do something
}
```

* status必须计算出一个char, byte,short, int 或者String 类型, 必须用括号括住
* value必须是和status相同的类型, 而且是**常量表达式**

### 逻辑操作符
`&&` 和 `||` 都有短路计算的特点, 和C++中一致.

## Chap 4: 数学函数\字符和字符串

### 常见的数学函数
Java在Math类中提供了很多实用的方法,用来计算常用的数学函数, 例如`Math.PI`和`Math.toDegrees()`,他们主要可以划分成三个类型,
1. 三角函数方法
2. 指数函数方法
3. 服务方法

### 字符数据操作
1. Java的字符数据类型用于表示单个字符,如 `char letter = 'A'`;
2. Java支持Unicode(如 `char letter= '\u6B22'`) 和ASCII的字符编码
3. 对于特殊的字符, 需要有转义序列
4. char数据类型可以转换为任何一种数值类型,反之亦然
    ```java
    char ch = (char)0xAB0041; // 只保留了最后符合utf-8的那些位,显示为A
    char ch2 = (char)65.25; // float cast 成了 int 然后cast成char 'A'
    ```
    0 ~ FFFF中的任何一个十六进制正整数都可以隐式转为字符型数据, 而不在此范围内的任何其他数值都 __必须显示转为`char`__
5. java 的character类提供了很多方法用于字符测试

### String类型
String和System以及Scanner类一样,都是java库中一个预定义的类, 它是一个 _reference type_ ,同样的String对象有很多简单的方法, 如 length(), chatAt(index),concat(s1)等,他们都是 **实例方法**, 和Math类中的方法不同, Math类中的方法都是静态方法.调用方法分别为 `s1.length()`和 `Math.pow(2,2.5)`.<br>
* 从某个名为inp的Scanner读取String的方法有两种, `inp.next()`读取以空白结束的字符串,`inp.nextLine()`可以读取一整行.<br>
* 两个String对象s1,s2都是reference type,用 ==只能判断他们是否是指向同一个对象, 而字符串的比较需要使用String的方法, 如`s1.equals(s2)`,`s1.compareTo(s2)`.<br>
* `s1.substring(beginIndex,endIndex)`可以得到一个子串.<br>
* 使用 `Integer.paraseInt(s1)` 可以实现string向 int 的转换,同样的可以有 _Double.paraseDouble(s1)_,而数值类型转为String就只需要一个连接符, `String s = value + ""`

## Chap 5: 循环
我们经常使用一个循环变量count来执行对循环贷额计数, 但是我们需要注意两个问题:
1. 要保证循环的继续条件最终可以变成false
2. 程序员经常犯的错误就是循环多执行一次或者少执行一次(off-by-one error)

一次增加一个步骤的渐进编码是一个很好的习惯, 在我们不知道如何编写循环的时候, __可以编写只执行一次的代码, 然后再规划如何重复执行这些代码__, 从这里我们得到了循环设计的策略:
* stage 1: 确定需要重复的语句
* stage 2: 将这个语句写在一个循环中
    ```java
    while(true){
        // chunk of codes
    }
    ```
* stage 3: 为循环的条件编码, 并为控制循环添加合适的语句
    ```java
    while(循环继续条件){
        // chunk of codes
        用于控制循环的语句
    }
    // example: 用户确认或者标记控制循环
    Scanner input = new Scanner(System.in);
    char continueLoop = 'Y';
    while(continueLoop == 'Y'){
        // execute the loop once
        ...
        System.out.print("Enter Y to continue or N to quit:");
        continueLoop = input.nextLine().charAt(0);
    }
    ```
警告 : 不要使用浮点值相等来进行循环控制, 因为他们都是近似值,如 2.0 可能在实际存储是2.000...3.<br>
`do{...}while( );` 这种循环和while loop的唯一区别在于前者可以保证至少循环一次, 而 for loop 具有编写循环的简明语法, 和 C++ 一样, 如果循环控制变量只在循环内使用, 那么在初始操作中声明它是一个比较好的编程习惯:<br>
```java
for(初始操作 ; 循环继续条件; 每次迭代之后的操作){
    // 循环体
}
```
上面的初始操作和迭代后的操作都可以有多个使用逗号隔开的语句, 但是这不是好的编程习惯, 那会使得程序的可读性变差.<br>
三种loop在表达上是等价的,可以根据习惯在语境中选择自己觉得最直观/舒服的一种循环语句, __各种循环语句中多写一个分号是错误的,往往会造成严重的问题__:
```java
for(int i = 0; i < 10; i++);
{
    System.out.println("i is " + i);
    // 实际上这句话只打印一次 
}
int i = 0;
while(i < 10); //这里进入一个死循环
{
    System.out.println("i is " + i);
    i++;
}
```
但是在do-while loop中是需要"_;_"的, 一个循环可以嵌套在另一个循环之中, 而嵌套循环往往可能需要很长的运行时间.和C++一样, 在循环中也可以使用`continue`跳过当前这轮迭代,或者使用`break`跳过整个循环语句.<br>

## Chap 6 : 方法
方法可以用于定义可重用的代码以及组织和简化编码, 方法的定义由 方法名称\参数\返回值类型以及方法体组成:
```Java
修饰符  返回值类型 方法名(参数列表){
    // 方法体
}
```
![method](figure/6.1.png)<br>
方法名和参数列表一起构成方法的签名(method signature), 需要注意的是,在方法头中需要对每一个参数进行单独的数据类型声明,如 `public static int max(int num1, num2)` 就是错误的,在Java语言中,带返回值的方法也可以当语句调用, 只需要忽略返回值即可.方法可以带来代码的共享和重用, 在新类中,可以使用 `ClassName.MethodName()` 调用方法(c++中是 `ClassName::functionName()`).<br>
调用方法和别的语言一样,在系统中是用一种被称为call stack的内存结构实现,void方法不返回值,但是return可以用于提前终止程序,因此只能作为语句而不能出现在表达式中.<br>
在java中, **所有的方法调用都是通过传值的方式将实参传给形参**, 因此一般意义上对int之类值的swap函数, 调用完毕之后两个参数的值本身并没有发生交换.此外,实参必须与方法签名中定义的形参在次序和数量上相匹配, 并且在类型上兼容(隐式类型转换可以发生).<br>
![swap error](figure/6.2.png)<br>
使用方法封装的优点:
1. 将不同的问题分开, 可以使程序的逻辑更加清晰, 同时提高代码的可读性
2. 将实现某个功能的代码和错误都限制在方法中,缩小了调试范围
3. 其他程序可以复用这部分代码

和C++一样, 我们也可以重载方法, 实现使用同一个名字来定义不同的方法, __只要他们的参数列表是不同的__, <font color = red> 不能基于不同的修饰符或者返回值类型重载方法 . </font> <br>
变量的作用域指的是变量 __可以在程序中被引用的范围__,可以在一个方法的不同块中声明同名的局部变量, 但是不能在嵌套块或者通一个块中两次声明同一个局部变量(在C++中嵌套块内声明的同名变量会将外部的同名变量隐藏起来).<br>
软件开发的关键在于应用抽象的概念, 而抽象有多种层次, 方法抽象(method abstraction)是通过将方法的使用和它的实现分离实现的,这就被称为信息隐藏(information hiding) 或者 封装, 在编写一个大型程序的时候,可以使用分治的策略, 逐步求精,将大问题分解成子问题. <br>
