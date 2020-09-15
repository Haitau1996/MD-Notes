# [COMPUTER SYSTEM: A Programmer's Perspective](http://www.csapp.cs.cmu.edu/)
<font size=5> **Randal Bryant & David Hallaron** </font>

### Chap 1 A Tour to Computer System
In a sense, the goal of this book is to help you understand what happens and why when you run hello on your system.
#### Information Is Bits + Contexts
The source program is a sequence of bits, each with a value of 0 or 1, organized in 8-bit chunks called bytes. Each byte represents some text character in the program. __系统中的所有信息(disk file, programs stored in memory, data transferd across a network)都是用 a bunch of bits 表示的__, 在不同的context中, 一串的bytes可能表示string,double, int 或者 机器指令.

#### Programs Are Translated by Other Programs into Different Forms
C语言是一种高级语言,它的语句将被翻译成 low-level machine-language 指令, 然后打包成Binary Disk file 的可执行程序,在Unix系统中的过程如下图:<br>
![compile](figure/Book1.1.png)<br>
1. __预处理阶段__: 根据字符 _#_ 开头的命令,修改原始的C程序
2. __编译阶段__: 将文本文件.c翻译成包 __含汇编语言程序的文本文件__ `.s`
3. __汇编阶段__: 汇编器将 `.s` 翻译成可重定位目标程序格式(relocatable object
program) `.o`
4. __链接阶段__: 负责处理相互依赖的 .o 文件之间的merging.如 The printf function resides in a separate precompiled object file called printf.o, which must somehow be merged with our hello.o program.

#### It Pays to Understand How Compilation Systems Work
程序使用的mechine code是编译系统生成的,但是我们依旧需要知道编译系统是如何工作的:
1. 优化程序的性能
2. 理解linking-time errors
3. 避免安全漏洞

#### Processors Read and Interpret Instructions Stored in Memory
##### Hardware Organization of a System
![orgination](figure/Book1.2.png)<br>
__Bus__ 总线是用于不同组件沟通的electrical conduits, 一般用于传输fixed-size chunk of bytes(words,一般是4个字节(32位)或者8个(64位)).<br>
__Main memory__ 临时存储系统, 一般是由一系列动态随机存储器构成,逻辑上它是Linear array of bytes, 每个都有自己的地址.<br>
__Processor__ A processor appears to operate according to a very simple instruction execution model, deﬁned by its _instruction set architecture_, 比如下面四种操作: **Load Store Operate** and **Jump**.

#### Caches Matter
A major goal for system designers is __to make these copy operations run as fast as possible__.To deal with the processor–memory gap, system designers include smaller, faster storage devices called cache memories.One of the most important lessons in this book is that application programmers who are aware of cache memories can exploit them to improve the performance of their programs by an order of magnitude. 一般而言, 每个设备中的存储组织成了一个存储层次结构,__The main idea of a memory hierarchy is that storage at one level serves as a cache for storage at the next lower level__:<br>
![memory hierarchy](figure/Book1.3.png)<br>

#### 操作系统管理硬件
![OS](figure/Book1.4.png)<br>
操作系统有两个作用:
1. to protect the hardware from misuse by runaway applications
2. to provide applications with simple and uniform mechanisms for manipulating complicated and often wildly different low-level hardware devices. 
##### 进程
一个进程是操作系统给运行程序的的抽象, 多个进程可以同时在系统中运行, 进程似乎是独占处理器\主存和IO设备.<br>
在任何一个时刻，单处理器系统都只能执行一个进程的代码。当操作系统决定要把控制权从当前进程转移到某个新进程时，就会进行 _上下文(context)_ 切换，即保存当前进程的上下文、恢复新进程的上下文，然后将控制权传递到新进程。新进程就会从它上次停止的地方开始。<br>
![swith](figure/Book1.5.png)<br>

##### 线程
尽管通常我们认为一个进程只有单一的控制流，但是在现代系统中，一个进程实际上可以由多个称为线程的执行单元组成，每个线程都 __运行在进程的上下文中，并共享同样的代码和全局数据__。由于网络服务器中对并行处理的需求，线程成为越来越重要的编程模型，因为 __多线程之间比多进程之间更容易共享数据__，也因为线程一般来说都比 __进程更高效__。<br>
##### 虚拟内存
![virtual memory](figure/Book1.6.png)<br>
虚拟内存是一个抽象概念，它为每个进程提供了一个假象，即每个进程都在独占地使用主存。每个进程看到的内存都是一致的，称为虚拟地址空间。
##### Files
A ﬁle is a sequence of bytes, nothing more and nothing less. 

#### 重要主题
##### Amdahl 定律 
$T_{new} = (1-\alpha) T_{old} + \frac{\alpha T_{old}}{k}$ <br>
$S= T_{old} / T_{new} = \frac{1}{(1-\alpha) + \alpha /k}$ <br>
要想显著加速整个系统，必须提升全系统中占用资源大的部分的速度.<br>
$S_{\inf} = \frac{1}{1 - \alpha}$
##### 并发和并行
并发(concurrency)是一个通用的概念，指一个同时具有多个活动的系统;并行(parallelism)指的是 __用并发来使一个系统运行得更快__, 并发可以在计算机系统的多个抽象层次上运用, 从高到低重点强调三个层次:
###### 线程级并发
传统意义上，这种并发执行只是模拟出来的，是通过使一台计算机在它正在执行的进程间快速切换来实现的. 多核处理器和超线程 (hyperthreading) 的出现，这种系统才变得常见。

###### 指令级并行
在较低的抽象层次上，现代处理器可以同时执行多条指令的属性称为指令级并行。如果处理器可以达到比一个周期一条指令更快的执行速率，就称之为超标量(super­ scalar)处理器。大多数现代处理器都支持超标最操作。

###### 单指令\多数据并行
在最低层次上，许多现代处理器拥有特殊的硬件，允许一条指令产生多个可以并行执行的操作，这种方式称为单指令、多数据，即 SIMD 并行。

##### 计算机系统中抽象的重要性
![abstraction](figure/Book1.7.png)<br>
抽象的使用是计算机科学中最为重要的概念之一,在处理器里，指令集架构(ISA)提供了对实际处理器硬件(微结构)的抽象。在学习操作系统时，我们介绍了三个抽象：__文件是对 I/0 设备的抽象，虚拟内存是对程序存储器的抽象，而进程是对一个正在运行的程序的抽象__。