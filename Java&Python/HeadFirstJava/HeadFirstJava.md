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
    - 不管程序多大(含有多少个类),都一定会有个Main函数作为程序的起点

在main或者其他方法中, 我们可以
* do something: 声明\设定\调用方法等简单语句
* loop
    ```Java
    while(boolean expression){
        ... // do something
    }
    for(    ;   ;   ){
        ... // do some thing
    }
    ```
* branching 
    ```