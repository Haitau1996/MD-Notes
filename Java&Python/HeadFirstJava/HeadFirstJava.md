# Head First Java

## Chap 01 进入Java的世界
java将带你进入全新的领域, 同之前的语言不同, 它在设计时候就有下面的特性:
* 友好的语法
* 面向对象
* 内存管理
* 跨平台可移植

Java 程序的执行过程:<br>
![avator](figure/1.1.png)<br>
* source file: 拓展名为*.java,用来表示程序的一个组件, 类的内容在声明之后的花括号里
* 类: 带有一个或者多个方法, 方法必须在类的内部声明
* 方法: 花括号中编写的应该执行的指令, 可以理解为是一个函数或者过程(C++中的成员函数)
    - 不管程序多大(含有多少个类),都一定会有个 _Main_ 函数作为程序的起点

在main或者其他方法中, 我们可以
* do something: 声明\设定\调用方法等简单语句
* loop
  * while loop
  * do-while loop
  * for loop
    ```Java
    while(boolean expression){
        ... // do something
    }
    for(    ;   ;   ){
        ... // do some thing
    }
    ```
* branching : 在适当的条件下做某些事情
    ```Java
    if (x == 10) {
        System.out.print("x must be 10");
    } else {
        System.out.print("x isn’t 10");
    }
    if ((x < 3) & (name.equals("Dirk"))) {
        System.out.println("Gently");
    }
    ```

需要注意的是， ==Java 是面向对象的语言， **所有东西都必须包含在类中**==， 并且在条件判断中不支持别的类型到 boolean 的隐式类型转换， 可行的替代做法包括 三元运算法 `? :`/ 位运算/ if-else / 第三方库实现的静态方法。   

## Chap 02: 拜访对象村
<div align=center><img src="https://i.imgur.com/65Se4bz.png"/></div>

在继承中， subclass 会自动继承 superclass 的方法， 并且可以适用自定义的同名方法**覆盖（override）** 父类的方法。 在设计类时， 需要记住对象是靠类模型塑造出来的， 类不是对象， 却是对象的蓝图。我们适用圆点（`.`）来使用对象的变量与方法：
* 实例变量： 对象本身已知的事物
* 方法： 对象可以执行的动作

一般而言 mian 有两个用途， ==真正的 Java 程序会让对象和对象之间交互==：
* 测试真正的类
* 启动 Java 应用程序

在 Java 语言中不存在全局变量的概念， 我们适用 `public/static/final` 等修饰符就可以实现类似的 全局和常量等需求。 

## Chap 03: 认识变量
Java 语言中变量有两种： ==**primitive** 和 **reference**==.并且 Java 是静态类型语言， 注重变量的类型。 
* primitive 类型有八种， 四种整型两种浮点型还有 boolean 和 char, 需要注意的是它们都有指定的范围要避免溢出 <div align=center><img src="https://i.imgur.com/eBi4j59.png"/></div>
* 没有对象变量这样的东西， 有的只是==引用对象的变量==， 保存的是存取对象的方法，对象是放在可以回收的堆上， 而引用可以理解为实际对象的遥控器 <div align=center><img src="https://i.imgur.com/1Ffid7e.png"/></div>

实际上引用的大小取决于虚拟机的实现者， 只能保证任意一个虚拟机中所有的引用大小都一样。  
数组就像是一个杯架， 其中的每个元素是变量（可以位基础类型或者引用）， 数组本身也是一个对象， 一旦被声明出来只能装载所声明的类型的元素。 <div align=center><img src="https://i.imgur.com/aiIqLQv.png"/></div>

## Chap 04: 对象的行为
对象的状态影响行为，同时行为也会影响状态。例如我们可以给在函数调用的参数中使用实例变量
```Java
class Song{
    String title;
    String artist;
    void play(){
        soundPlayer.playSound(title);
    }
}
class Dog{
    int size;
    String name;
    void bark(){
        if(size >= 60){
            //...
        }
        else if{
            //...
        }
    }
}
```
* 方法中可能会运用形参， 调用时有时候需要传入实参， 实参传入方法后就成了形参， 跟局部变量是一样的。 
* 方法何以有返回值
* 可以向方法中传入多个参数， 但是个数和类型以及顺序要和声明的一致
* Java 是通过值传递， 就是拷贝传递

在处理数据的时候需要注意封装， 就是将实例变量设置为 private, 然后提供 public 的 getter 和 setter 提供对它的访问和修改。   
在 java 中注意局部变量和实例变量的区别：
* 实例变量声明在类内
* 局部变量声明在方法中
* ==局部变量没有默认值， 适用之前必须初始化==
* 实例变量的默认值， 根据类型不同可能为 flase/ 0/ null
