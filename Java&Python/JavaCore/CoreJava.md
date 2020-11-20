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
| Micro Edition| Java EE | 用于微型手机和其他小型设备的Java平台| 

安装 Java 开发工具箱
* 下载 JDK
* 设置执行路径如在 Linux 的 shell rum command中加入 `export PATH=jdk/bin:$PATH`
* 安装 源代码和库文件 `jdk-doc`, `wget https://horstmann.com/corejava/corejava.zip`
* 命令行界面/ IDE 编写程序