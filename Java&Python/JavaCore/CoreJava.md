# Java in a NetShell 
by [Benjamin J. Evans & David Flanagan]()
## Part I : Introducing Java
### Chap 1: Introduction to the Java Environment

***
# Thinking in Java
author [Bruce Eckel](https://www.bruceeckel.com)
## Chap 1: 对象导论
### 抽象过程
**所有的语言都提供抽象机制**, Assembly language is a small abstraction of the underlying machine. Many so-called “imperative” languages that followed (such as FORTRAN, BASIC, and C) were abstractions of assembly language.**程序员必须建立机器模型和待解决问题之间的关联**,面向对象编程通过向程序员提供表示问题空间中元素的工具而更进了一步, 这种表示方式非常通用,Java基于的语言之一smalltalk有五个特性:
1. 万物都为对象
2. 程序是对象的集合, 他们通过发送消息来告诉彼此所要做的(调用请求)
3. 每个对象都有自己的由其他对象所构成的存储,因此可以在程序中构建复杂的体系
4. 每个对象都有其类型
5. **某一特定类型的所有对象都可以接收同样的消息**: 可替代性是OOP中最有力的概念

对象具有**状态(内部数据)\行为(方法)和标识(内存中的唯一地址).**
### 每个对象都有一个接口
抽象数据类型的运行方式与内置类型几乎完全一致: __你可以创建某一个类型的变量, 然后操作这些变量__, 类描述了具有相同特性(数据元素) 和 行为(功能)的对象集合.某个对象都只能满足某些请求, 这些请求由对象的接口(interface)定义,决定接口的就是类型,此外,程序中**必须要满足这些请求的代码**,这些代码与隐藏的数据一起构成了实现.

### 每个对象都提供服务
当试图开发/理解OO程序设计时, __最好的办法是将对象想象为"服务的提供者"__ .将对象看成服务提供者有助于提高对象的 __内聚性(cohesiveness)__ , 在良好的OO设计中, 每个对象都恰好完成一个任务,并且并不视图做更多事情.
### 被隐藏的具体实现
程序员的perspective分为类创建者和客户端程序员, 通常类创建者隐藏对象内部脆弱的部分,减少程序的Bug.同时类设计者可以改变内部的工作方式而不影响客户端程序员. Java用`public`(任何人都可以访问的)\ `private`(除了类型创建者和类型内部的方法之外任何人都不能访问的元素) 和`protected`(继承的类可以访问这种成员, 但不能访问private) 设定边界.
### 复用具体实现 
类被创建和测试过后, 理想条件下代表一个有用的代码单元, 可以复用. 最简单的复用方式就是直接使用该类的一个对象,如用组合(composition)的方式,代表一种 _has-a_ 的关系, 在建立新的类时, 应该**优先考虑组合,它更加灵活方便(松耦合)**.
### 继承

***
# Java 核心技术 vol 1 : fundamentals
By [Cay Horstmann](https://horstmann.com/corejava/)