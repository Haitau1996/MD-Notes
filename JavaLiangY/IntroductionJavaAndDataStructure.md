# Java语言程序设计与数据结构
**author: 梁勇**
***
## Chap 1 计算机、程序和java概述
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

## Chap 4: 数学函数\字符和字符串

### 常见的数学函数
Java在Math类中提供了很多实用的方法,用来计算常用的数学函数, 例如`Math.PI`和`Math.toDegrees()`,他们主要可以划分成三个类型,
1. 三角函数方法
2. 指数函数方法
3. 服务方法

### 字符数据操作
1. Java的字符数据类型用于表示单个字符,如 `char letter = 'A'`;
2. Java支持Unicode(如 `char letter= '\u6B22'`) 和ASCII的字符编码