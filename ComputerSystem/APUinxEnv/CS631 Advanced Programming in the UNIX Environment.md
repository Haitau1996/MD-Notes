# [CS631: Advanced Programming in the UNIX Environment](https://stevens.netmeister.org/631/)<!-- omit in toc -->
- [Week 1](#week-1)
  - [Introduction](#introduction)
    - [This class in a nutshell: the "what"](#this-class-in-a-nutshell-the-what)
    - [This class in a nutshell: the "why"](#this-class-in-a-nutshell-the-why)
    - [This class in a nutshell: the "how"](#this-class-in-a-nutshell-the-how)
  - [Unix 基础](#unix-基础)
    - [OS 设计](#os-设计)
    - [Unix Basics: 管道](#unix-basics-管道)
    - [文件和目录](#文件和目录)
    - [用户标识](#用户标识)
    - [Unix 时间值](#unix-时间值)
    - [标准 I/O](#标准-io)
    - [进程](#进程)
    - [信号](#信号)
- [Week 2](#week-2)
  - [文件描述符](#文件描述符)
  - [open(2) 和 close(3)](#open2-和-close3)
    - [`creat(2)`](#creat2)
    - [`open(2)`](#open2)
    - [`close(2)`](#close2)
  - [read,write,lseek](#readwritelseek)
    - [`read(2)`](#read2)
    - [`write(2)`](#write2)
    - [`lseek(2)`](#lseek2)
  - [文件共享&其他操作](#文件共享其他操作)
    - [原子操作](#原子操作)
    - [文件描述符复制](#文件描述符复制)
    - [缓存 `sync`/`fsync`/`fdatasync`](#缓存-syncfsyncfdatasync)
    - [File Descriptor Control](#file-descriptor-control)
- [Week 3](#week-3)
  - [`stat(2)`](#stat2)
  - [UID & GID](#uid--gid)
    - [`access(2)`](#access2)
  - [`st_mode`](#st_mode)
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

**shell**：[shell](code/cs631/week01/simple-shell.c) 做的事情其实很简单， 外面是一个死循环， 然后读取用户输入， 执行输入的命令(REPL,Read–Eval–Print Loop)。

### Unix Basics: 管道
通常我们从键盘读取输入， 将结果显示到屏幕上， 而管道（Pipeline）是一系列将标准输入输出链接起来的进程，其中每一个进程的输出被直接作为下一个进程的输入, 这时候标准输出是作为下一个软件的输入， 而标准错误依旧可以显示：
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220124230034.png" width="50%"/></div>

### 文件和目录
* UNIX 文件系统是一个**树形结构**，所有分区都安装在根(`/`) 目录下。文件名可以由除 `/` 和 `NUL` 之外的任何字符组成，因为路径名是由 `/ `分隔的零个或多个文件名的序列。
* 目录是包含目录项特殊的文件， 目录项中有索引和文件名的映射(mapping)
* 所有的进程都有一个当前工作目录， 它是相对路径的起点

### 用户标识
用户ID 和 组ID 是用于标识系统上不同用户并且赋予不同权限的数值。使用 id 命令可以查看。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220124232442.png" width="60%"/></div>

### Unix 时间值
* 日历时间。该值是自 UNIX epoch(1970年1月1日00:00:00)以来所经过的秒数累计值, 使用 `time_t` 保存
  * 底层如果是 int32, 那么就会出现 [2038年问题](https://zh.wikipedia.org/zh-hans/2038%E5%B9%B4%E9%97%AE%E9%A2%98)
* 进程时间。也被称为CPU时间, 使用 `clock_t` 保存， 有三个值
  * 时钟时间(clock time)
  * 用户 CPU 时间
  * 系统 CPU 时间<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220124233407.png" width="70%"/></div>

### 标准 I/O
* **文件描述符**： 一个小的非负整数， 标识进程正在访问的文件， shell 可以重定向任意的文件描述符
* 内核提供了两种 I/O
  * buffered I/O : e.g. `fopen(3)`/`getc(3)`/`putc(3)`
  * unbuffered I/O: e.g. `open(2)`/`lseek(2)`

### 进程
* 在内存中运行的程序被称为进程
* 通过使用 `exec(3)` 或者 `execve(2)` 函数将程序放入内存运行
* 每个进程都有唯一非负的标识符(进程ID)
* 只有使用 `fork(2)` 系统调用可以创建新的进程
* 使用 `fork(2)`/`exec(3)`/`waitpid(2)` 控制进程

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220124235255.png" width="50%"/></div>

### 信号
信号通知一个进程发生了某种情况，有三种处理方式：
* 允许它触发默认动作
* 显式忽略
* 捕获并且将控制转换到一个用户自定义函数

# Week 2
## 文件描述符
* 文件描述符(aka. 文件句柄)是内核用于标识文件的小的非负整数。
  * 取值范围为 $0 \sim OPEN\_MAX - 1$
* 通常 shell 把文件描述符 0 (STDIN_FILENO)关联到标准输入, 1 (STDOUT_FILEIO) 关联为标准输出， 2 (STDERR_FILENO)关联到标准错误(通常我们不使用 magic number, 而是使用常量)。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220202231806.png" width="80%"/></div>  

我们可以用[代码](code/cs631/week02/openmax.c)看有没有 OPEN_MAX 宏的定义或者从 `sysconf(_SC_OPEN_MAX)` 看其值的大小， 后者是可以在运行时修改的。
* 我们有时候无法依赖定义的值， 他们可能和进程实际应用的不同
* 标准中要求定义的常数在当下可能不适用
* 使用 `sysconf(3)`/`getrlimits(2)` 可能在运行时修改这些值， 下次调用值可能就变了
* 要习惯于写代码检查测试我们的理解
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202202022332897.png" width="70%"/></div>

## open(2) 和 close(3)
基础的文件 I/O 可以通过下面五个函数(都是系统调用)实现：
* open
* close
* read
* write
* lseek

### `creat(2)`
```C
#include<fcntl.h>
int creat(const char* pathname, mode_t mode);
// return file descriptor if ok, -1 on error
```
* `creat(2)` 返回一个 write-only 模式的文件句柄，在早期的 unix 系统中 open 无法直接创建文件， 我们就先要 `creat` 然后退出再 `open`。
* 这个接口后面就过时了， 现在相当于`open(path, O_CREAT|O_TRUNC|O_WRONLY,mode)`.

### `open(2)`
```C++
#include <fcntl.h>
int open(const char *pathname, int oflag, ... /* mode_t mode */);
// Returns: file descriptor if OK, -1 on error
```
oflag 应该是下面情况之一：
* O_RDONLY
* O_WRONLY
* O_WDWR

还有一些可选的选项：
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220210195229.png" width="70%"/></div>
除了 posix 外， 有的平台也支持一些额外的 ofalg。

* `openat(2)` 用于原子化处理其他工作目录中的相对路径名。 
* open 函数在失败的时候回返回相应的失败代码， 因此我们每次调用都要判断是否打开成功：
    ```C
    if(fd = open(path,O_RDWR)<0){
        /* error*/
    }
    ```

### `close(2)`
```C++
#include <unistd.h>
int close(int fd);//Returns: 0 if OK, -1 on error
```
* 用文件描述符关闭时会释放所有记录该文件的锁
* 如果没有显式调用， 内核会在进程终止的时候关闭它
* 为了避免文件描述符泄漏， 我们应该总是在同一个 scope 关闭文件

## read,write,lseek
### `read(2)`
```C
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t num);
//Returns: number of bytes read; 0 on EOF, -1 on error
```
函数会从当前的偏移量开始阅读， 并且将偏移量增加读取的 byte 数。在很多情况下返回的字节数可能比请求的少，例如：
* 提前遇到了 EOF， 如想要读取 200个字节，32个后就遇到了 EOF, 这时候返回 32, 下次 read 就返回 0
* 从网络读取， buffering 可能导致数据的延迟
* 被信号中断了

### `write(2)`
```C
#include <unistd.h>
ssize_t write(int fd, void *buf, size_t num);
//Returns: number of bytes written if OK; -1 on error
```
* write 返回的是已经写入的比特数，如果小于预期需要重写剩余部分
* 通常从某个 offset 开始写入的时候， 会覆盖原来的内容， 遇到 EOF 之后依旧会继续写入

### `lseek(2)`
```C
#include <sys/types.h>
#include <fcntl.h>
off_t lseek(int fd, off_t offset, int whence);
//Returns: new offset if OK; -1 on error
```
由 whence 决定 offset 如何使用：
* SEEK_SET 从开始
* SEEK_CUR 从当前位置开始
* SEEK_END 从文件结束开始

有时候使用起来有一点怪：
* seek to 一个负的偏移量： 相当于磁带倒带
* seek 0 就是当下的位置
* seek 越过文件结束符（可能会产生一个中间内容很多为空（`\0`）的文件， 不同的系统对它的支持不同，有的是直接将大量的 nul 写入磁盘，而有的文件系统支持[稀疏文件](https://zh.wikipedia.org/zh-cn/%E7%A8%80%E7%96%8F%E6%96%87%E4%BB%B6)）<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211195224.png" width="70%"/></div>上图中文件系统支持稀疏文件，所以生成的 file.hole 很小，`cp` 命令拷贝也支持，拷贝的副本空间占用也很小， 但是`cat`不支持 sparse file, 因此生成的却特别大。

## 文件共享&其他操作
因为 UNIX 是多用户、多任务的操作系统， 多个进程可以方便地同时操作同一个文件。为了方便，我们了解一些相关的数据结构：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211201811.png" width="80%"/></div>

* 每个进程的 table entry 都有一个文件描述符的表，包含
  * 文件描述符flag(如 FD_CLOEXEC)
  * 一个指向文件表项的指针
* 内核维护了一个文件表，每个文件表项目包含
  * 文件的状态标志（读、写、同步和非阻塞）
  * 当前的偏移量
  * 指向该文件 vNode 表项的指针
* vNode 中包含
  * vNode 的信息
  * inode 的信息（如当前文件大小）

这时候两个进程打开同一个文件就如下图所示：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211202441.png" width="60%"/></div>
这时候我们理解 lseek 的操作就更容易了， 它实际上只修改了文件表项中的偏移量， 并没有直接修改磁盘的文件。

### 原子操作
例如两个进程都想要向文件末尾写入字符串， 一个初始长度为 128 byte的字符串写完后就可能如下图所示：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211203051.png" width="80%"/></div>

pread 和 pwrite 允许原子性地定位并且执行 I/O:
> 原子操作指的是多个步骤组成的一个操作， 要么一步执行所有操作， 要么不执行， 不能只执行所有步骤的一个子集
```C
#include <unistd.h>
ssize_t pread(int fd, void *buf, size_t num, off_t offset);
ssize_t write(int fd, void *buf, size_t num, off_t offset);
//Returns: number of bytes read/written, -1 on erro
```
调用之后 current offset 并没有变化。

### 文件描述符复制
```C
#include <unistd.h>
int dup(int oldfd);
int dup2(int oldfd, int newfd);
//Returns: 新的文件描述符, -1 on error
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211205326.png" width="30%"/></div>

在上图中， 由于将 2(stderr) 重定向到了 /dev/null, 因此没有 err 信息输出，`dup2(STDOUT_FILENO, STDERR_FILENO)` 之后， stderr 就和 stdout 有了同样的文件描述符， 并且在终端输出。

### 缓存 `sync`/`fsync`/`fdatasync`
大多数 io 都通过缓存区进行， 为保证实际文件系统中与缓存区的内容一致， Unix 提供了三个函数
```C
#include<unistd.h> 
int fsync(int fd);
int fdatasync(int fd);
void sync(void);//返回值：若成功，返回0；若出错，返回−1
```
* sync : 将所有修改过的块缓存区排入队列， 然后返回（不等待实际磁盘操作结束）
* fsync 只对 fd 指定的文件起作用， 并且操作结束才返回
* fdatasync 类似于 fsync, 但只影响文件的数据部分， 前者会同步更新文件的属性
### File Descriptor Control
```C
#include <fcntl.h>
int fcntl(int fd, int cmd, ...);
//Returns: depends on cmd, -1 on error
```
这个函数可以改变已经打开的文件的属性， cmd 为不同值的时候有不同的效果。  
```C
#include <sys/ioctl.h>
int ioctl(int fd, unsigned long request, ...);
//Returns: depends on request, -1 on error
```
是一个工具箱， 不能用其他函数表示的 i/O 操作通常可以用 `ioctl` 表示。 

现在很多系统都将可以将标准输入、输出和错误作为文件，可以使用文件名 `/dev/std[in,out,err]`访问：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211210642.png" width="40%"/></div><div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220211210837.png" width="40%"/></div>

# Week 3
## `stat(2)`
```C
#include <sys/stat.h>
int stat(const char *path, struct stat *sb);
int lstat(const char *path, struct stat *sb);
// 和 stat 区别在于文件是符号链接时，写入符号链接有关的信息而不是链接引用的文件信息
int fstat(int fd, struct stat *sb);

#include <sys/stat.h>
#include <fcntl.h>
int fstatat(int fd, const char *path, struct stat *sb, int flag);
//Returns: 0 if OK, -1 on error
```
这4个 stat 函数都要传入一个结构体指针， 将信息写到指针指向的结构体中。下面是 stat 这个结构体 metadata 的最低要求， 不同的操作系统实现可能有更多的域：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220212224005.png" width="70%"/></div>  

实际上使用 stat 结构体最频繁的可能是 `ls -l` 命令(filename 并不在 stat 结构体中)：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220212224523.png" width="70%"/></div>  
系统可能提供 `stat(1)` 命令以显示文件的信息， 结果取决于不同的系统实现：
* reference system: <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220212225149.png" width="70%"/></div>
* ArchLinux in WSL2,没有 -r 选项，有类似的<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220212225238.png" width="70%"/></div>

事实上 `st_mode` 不仅包含文件的 permission, 还包含文件类型
* 普通文件：最常用， 其内容的解释由处理该文件的应用决定
* 目录： 包含其他文件的名字和指向相关信息的指针
* 字符特殊(character special)文件： 用于特定类型的设备， 如终端
* 块特殊(block special)文件: 通常用于磁盘
* FIFO: 有时候被称为命名管道，用于进程间通信
* 套接字： 用于网络通信
* 符号链接： pointer to another file

## UID & GID
与一个进程关联的 ID 至少有六个：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220213124420.png" width="70%"/></div>  
* 文件在 setuid 的时候， 是将 effective user ID 写入 `st_uid`, setgid 的时候同样是这样。

有效用户 id 背后的思想是 least priviledge ，即只给用户他们需要的权限， not more。euid 的存在使得用户在必要的时候可以提升自己的权限，例如我们想要 ping 一个网站，自己复制一个 ping 的副本就无法使用，因为 ping 的背后有 socket 操作需要 root 权限：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220213130550.png" width="70%"/></div><div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220213130953.png" width="70%"/></div>  
* 这个理念对于 gid 依旧适用

```C
#include <unistd.h>
int setuid(uid_t uid);
int seteuid(uid_t uid);
// Returns: 0 if OK, -1 on error
uid_t getuid(void);
uid_t geteuid(void);
// Returns: uid_t; never errors
```
* setuid 不但会设置 uid, 同时也会设置 euid(之前提升的 prililedge 也会消失)

### `access(2)`
```C
#include <unistd.h>
int access(const char *path, int mode);
int faccessat(int fd, const char *path, int mode, int flags);
//Returns: 0 if OK, -1 on error
```
* 按实际 uid 和 gid 测试访问能力

## `st_mode`
`st_mode` 同样编码了文件的访问权限位， 每个文件有 9 个权限位：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220213171447.png" width="60%"/></div>  
> 上表的 第一列为 sd_mode mask, mask 是一种字符模式,用来控制字符另一种模式某些部分的保留或删除  

`chmod(1)` 可以修改这九个权限位，这三类权限以各种方式给不同函数使用， 规则如下：
* 打开文件， 需要名字中包含的每个目录（包括隐藏的当前目录）的执行权限
* 用 `O_REONLY` or `O_RDWR` 模式打开文件， 需要 read permission
* 对于 `O_WRONLY` or `O_RDWR` 模式打开文件， 需要 write permission
* open 中指定 `O_TRUNC` 标志， 需要 write permission
* 创建一个新文件， 需要文件目录的 write + execute permission
* 删除一个文件， 同样需要目录的 w+e 权限， 但是**并不需要对该文件本身的读写权限**
* 如果需要执行文件(via `exec` family), 需要执行权限(**并不需要读权限**)

我的理解访问一个目录，相当于是执行它， 添加和修改文件就是写入+访问。  
内核的文件访问测试：
1. `effect-uid == 0`(超级用户): 允许访问
2. `e-uid == st_uid`(当前进程拥有此文件)
   1. 如果设置了合适的用户权限位， 允许访问
   2. 否则拒绝
3. `e-gid = st_gid`
   1. 如果设置了合适的组权限位， 允许访问
   2. 否则拒绝
4. 其他用户的访问结果取决于访问权限位的设置

理解了这个测试过程就能理解反直觉的结果， 如 用户所有的文件， 通过 chmod 设置权限为 0， 即使加上了组读取权限，该用户依旧不能读取（但是同组的其他用户可以）：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220213174220.png" width="70%"/></div>  
