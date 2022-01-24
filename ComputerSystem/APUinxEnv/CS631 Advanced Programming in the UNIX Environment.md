# [CS631: Advanced Programming in the UNIX Environment](https://stevens.netmeister.org/631/)
# Week 1
## Introduction
### This class in a nutshell: the "what"
* 获得对 Unix 操作系统的理解
* 获得（系统）编程经验
* 理解基础的操作系统概念(with focus on Unix family)
  * 多用户概念
  * 基础&高级 I/O
  * 进程的关系
  * 进程间的通讯
  * 使用客户端-服务器模型做基础的网络编程

### This class in a nutshell: the "why"
* 理解 UNIX 系统工作方式帮助我们领悟其他 OS 的概念
* 系统级编程经验十分珍贵， 它构成其他编程的基础
* C 语言的系统级编程帮助我们理解其他编程概念
* 大多数的高级语言(最终)调用(或者自行实现) C 标准库的函数

### This class in a nutshell: the "how"
* 参考平台是 [NetBSD](https://stevens.netmeister.org/631/virtualbox/), 使用 NetBSD 9.0 上的 gcc 7.4.0 编译自己的代码。
* 习惯在 Unix 阅读[源代码](https://stevens.netmeister.org/631/#source-code)
* 在本课程中要编写一些代码， 编写代码是一种交流方式， 保证自己的代码
  * 有清楚的结构
  * 良好的版式（格式）
  * 使用一致的编程 [style](https://stevens.netmeister.org/631/style)(缩进， 括号的方式, etc.)
  * 变量和函数应该谨慎地命名
  * 注释应该仅限于必要时使用， **更多解释为什么那样做， 而不是说明怎样实现**

## Unix 基础
### OS 设计
<div align=center><img src="https://s2.loli.net/2022/01/24/MzmHdgYceF8REh6.png" width="30%"/></div>

系统调用和库函数(手册的第一部分是用户命令， 看其他部分的实现需要安装 `man-pages`)
* 系统调用是内核中功能实现的入口，它们在手册的第二部分(eg. `write(2)`)
* 库调用转移到执行所需功能的用户代码， 通常在手册的第三部分(eg. `printf(3)`)<div align=center><img src="https://s2.loli.net/2022/01/24/QFdB9V8o7O3KU4L.png" width="70%"/></div>

我们所关心的标准化工作包括：
* IEEE [POSIX (1003.1-2008)](https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/) / SUSv4,系统可能提供了超过标准接口描述的功能， 例如 ls
  * POSIX 中的 Option:<div align=center><img src="https://s2.loli.net/2022/01/24/nKAmhFsfztJ4SI5.png" width="70%"/></div>
  * ArchLinux 上的 Option: <div align=center><img src="https://s2.loli.net/2022/01/24/IRZrobdXatnLFpG.png" width="70%"/></div>

* C 标准：ANSI C ; C99 (ISO/IEC 9899); C11 (ISO/IEC 9899:2011); C18 (ISO/IEC 9899:2018)
  * ASCI C 中重要的功能：
    * 函数原型
    * 通用指针（`void *`）
    * 几种抽象的数据类型(e.g. pid_t, size_t),背后是使用 typedef 语句实现。例如使用 32bit 去定义时间可能有 2038问题（溢出）， 使用抽象的数据类型， 很多 linux 系统内部使用 64bit 实现而无需修改其他代码
* 错误处理：
  * 有意义的函数返回， 通常表现为 `errno` 错误对照表
  * 我们可以使用 `strerror(3)` & `perror(3)` 函数实现值和字符串的相互查找

在这个课程中， 要求所有的代码都通过`-Wall -Werror -Wextra` 的检测， 为了方便我们可以添加环境变量：
* sh:<div align=center><img src="https://s2.loli.net/2022/01/24/XOU96h1KspegaIZ.png" width="50%"/></div>
* zsh，在`~/.zshrc`添加下面两行: <div align=center><img src="https://s2.loli.net/2022/01/24/9oi536HAOgMpNTE.png" width="50%"/></div>

**shell**：shell 做的事情其实很简单， 外面是一个死循环， 然后读取用户输入， 执行输入的命令。