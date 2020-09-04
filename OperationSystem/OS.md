# [Operating System : three easy pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)
inspired by the book written by [Remzi H. Arpaci-Dusseau](http://pages.cs.wisc.edu/~remzi/) and [Andrea C. Arpaci-Dusseau](http://pages.cs.wisc.edu/~dusseau/)

### Chap 1: About the Book
虚拟化(virtualization)\并发(concurrency)和持久性(presitence)是操作系统中的三个关键概念, 理解后有助于帮助我们理解操作系统是如何工作的.学习这个课程,需要借助笔记和 __项目__ 理解巩固操作系统中的概念.

### Chap 2: Introduction to Operating System
有一类软件让程序运行变得容易, 允许程序共享内存,让程序能够和设备交互, 以及其他类似有趣的工作,要做到这点,操作系统需要用一种被称为虚拟化(virtualization)的通用技术: __将物理资源(内存/处理器/磁盘等)转化为更为通用\更强大且更利于使用的虚拟形式__. 每个CPU\内存和磁盘都是系统的资源,操作系统扮演的主要角色就是管理这些资源,做到高效或公平,或者实际上考虑其他许多可能的目标.<br>
从结果上看, 在硬件的帮助下, 操作系统负责提供一种假象, 系统拥有非常多的虚拟CPU. 而每个进程访问自己的私有虚拟空间, 操作系统以某种方式映射到机器的物理内存上. 

// TODO: 看完CSAPP再来