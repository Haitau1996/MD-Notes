# [Operating System : three easy pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)
inspired by the book written by [Remzi H. Arpaci-Dusseau](http://pages.cs.wisc.edu/~remzi/) and [Andrea C. Arpaci-Dusseau](http://pages.cs.wisc.edu/~dusseau/)

### Chap 1: About the Book
虚拟化(virtualization)\并发(concurrency)和持久性(presitence)是操作系统中的三个关键概念, 理解后有助于帮助我们理解操作系统是如何工作的.学习这个课程,需要借助笔记和 __项目__ 理解巩固操作系统中的概念.

### Chap 2: Introduction to Operating System
有一类软件让程序运行变得容易, 允许程序共享内存,让程序能够和设备交互, 以及其他类似有趣的工作,要做到这点,操作系统需要用一种被称为虚拟化(virtualization)的通用技术: __将物理资源(内存/处理器/磁盘等)转化为更为通用\更强大且更利于使用的虚拟形式__. 每个CPU\内存和磁盘都是系统的资源,