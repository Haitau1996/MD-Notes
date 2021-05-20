# [Operating System : three easy pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)
My note of the book written by [Remzi H. Arpaci-Dusseau](http://pages.cs.wisc.edu/~remzi/) and [Andrea C. Arpaci-Dusseau](http://pages.cs.wisc.edu/~dusseau/)

### Chap 1: About the Book
**虚拟化(virtualization)\并发(concurrency)和持久性(presitence)** 是操作系统中的三个关键概念, 理解后有助于帮助我们理解操作系统是如何工作的.学习这个课程,需要借助笔记和 __项目__ 理解巩固操作系统中的概念.

### Chap 2: Introduction to Operating System
有一类软件让程序运行变得容易, 允许程序共享内存,让程序能够和设备交互, 以及其他类似有趣的工作,要做到这点,操作系统需要用一种被称为虚拟化(virtualization)的通用技术: __将物理资源(内存/处理器/磁盘等)转化为更为通用\更强大且更利于使用的虚拟形式__. 每个CPU\内存和磁盘都是系统的资源,操作系统扮演的主要角色就是管理这些资源,做到高效或公平,或者实际上考虑其他许多可能的目标.

#### 虚拟化 CPU
从结果上看, 在硬件的帮助下, 操作系统负责提供一种假象, **系统拥有非常多的虚拟CPU**. <br>
一次运行多个程序可能会带来各种新的问题, 如两个程序要在特定的时间运行, 应该先运行哪一个. 这个问题需要由操作系统的 策略(policy) 来回答. 

#### 虚拟化内存
每个进程访问自己的私有虚拟空间, 操作系统以某种方式映射到机器的物理内存上.物理内存是有操作系统共享的资源, 这是如何完成的属于虚拟化的主题.

#### 并发
concurrency 指代一系列的问题, 他们在同时处理很多事情时出现并且必须解决. 并发问题不再局限于操作系统本身, 现代多线程程序也存在相同的问题. 例如两个线程共享一个计数器, 需要三步:
1. 将计数器值从内存加载到寄存器
2. 将该值递增
3. 将其保存回内存

而这三条指令并不是以原子方式执行(所有的指令一次执行), 因此可能造成混乱使得结果和我们希望的不同.

#### 持久性
在系统内存中, 数据容易丢失(DRAM 以 volatile 方式存储数值),因此我们需要硬件和软件来持久地存储数据. 操作系统中管理磁盘的软件通常称为文件系统(file system), 负责高效和可靠地方式将用户的任何文件存在在系统的磁盘上. 操作系统并不会为每个应用程序创建专用的虚拟磁盘, 而是经常需要共享文件中的信息. 

#### 设计目标
操作系统取得 CPU/内存 或磁盘等物理资源, 并对他们进行虚拟化. 它处理与并发相关的码放且棘手的问题, 它持久地存储文件从而使他们长期安全. 相关的目标如下:
1. 基本目标是建立一些抽象, 让系统方便和易于使用
2. 另外一个目标是提供高性能
3. 还有就是在应用程序之间以及 OS和应用程序之间提供保护
4. 操作系统也必须不间断运行

## 第一部分: 虚拟化
### Chap 4: 抽象 -- 进程
进程就是运行中的程序, 是一个正在运行的程序实例. 操作系统通过虚拟化 CPU 来提供这些假象, 具体而言就是 **让一个进程只运行一个时间片, 然后切换到其他进程, 提供存在多个虚拟CPU 的假象**, 这就是 **time sharing** of the CPU.潜在的开销就是性能损失, 想要实现得好就需要一些低级机制(**mechanisms**, 低级方法或协议,实现所需要的功能) 以及一些高级智能(其中一些以策略, policy, 的形式存在).

#### 抽象: 进程
操作系统为正在运行的程序提供的抽象, 就是进程, 一个进程就是一个正在运行的程序. 理解进程就需要理解其 **机器状态: 程序在运行中可以读取或者更新的内容**.
* 内存是机器状态的一个明显组成部分, 可以访问的内存被称为地址空间(address space)
* 另一个部分是**寄存器**: 其中包含一些特殊的寄存器,如 %rsp(stack pointer) 和相关的帧指针(frame pointer)用于管理函数参数栈/局部变量和返回地址, 程序计数器(Program Counter) 告诉我们即将执行哪个指令
* 最后包含经常访问持久存储设备

#### 进程 API
现代操作系统都以某种形式提供这些 API:
* 创建
* 销毁
* 等待
* 其他控制
* 状态

#### 进程创建: 更多细节
我们需要理解操作系统是如何启动并运行一个程序的:
1. 第一件事情就是将代码和所有的静态数据加载到内存
2. 此后操作系统在进行次进程之前还需要执行一些其他的操作, 例如为程序的运行时栈(run-time stack) 分配一些内存,也可能为程序的堆分配一些内存
3. 操作系统还将执行一些其他的初始化任务, 特别的 IO 相关的

#### 进程状态
简单来说, 进程可以出于下面三种状态之一:
* 运行: 在运行状态下, 处理器正在执行它的指令
* 就绪: 进程已经准备好运行, 但是操作系统选择不在此时运行
* 阻塞: 一个进程执行了某种操作(例如IO), 直到发生其他事件的时候才会准备运行

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210506165134.png"/></div>

#### 数据结构
操作系统也有一些关键的数据结构来跟踪各种相关的信息. 例如 xv6 使用一个 context 的结构体来保存寄存器的内容, 用于上下文切换. 

### Chap 5: 进程 API
UNIX 提供了创建新进程的方式, 通过系统调用 `fork()` 和 `exec()`, 还有一个系统调用 `wait()` 等待其创建的子进程执行完成. 
#### `fork()` 系统调用
下面的代码展示了 fork 是如何使用的:
```C++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    printf("hello world form pid %d \n", (int)getpid());
    int rc = fork();// 从这里开始有两个进程
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(0);
    }else if(rc == 0){ // 子进程则进入这个代码块
        printf("hello I am a child %d\n", (int)getpid());
    }else {    // 父进程进入这个代码块
        printf("hello I am parent of %d with pid %d\n",rc, (int)getpid());
    }
    return 0;
}
```
```Shell
hello world form pid 992 
hello I am parent of 993 with pid 992
hello I am a child 993
```
在 UNIX 系统中, 如果要操作某个进程, 就需要使用 `PID` 来指明. 使用 `fork()` 调用新创建的进程几乎与调用进程完全一样, 它拥有自己的地址空间,寄存器,程序计数器, 但是从 `fork()` 返回的值是不同的, 父进程获得的是子进程的 PID, 而子进程获得的是 0. 可以容易地理解, 两者的输出顺序是不确定的. 

#### `wait()` 系统调用
我们可以对上面的代码稍加修改, 加入 `<sys/wait.h>` 头文件, 在父进程执行的块中加入一行:
```C++
else{
    int wc = wait(NULL);
    printf("hello from parent of %d with %d \n",rc, (int)getpid());
}
```
这是父进程就会等到子进程运行完成之后再运行, 这时候 `wait()` 返回, 接着父进程才输出自己的信息.

#### `exec()` 系统调用
`exec()` 系统调用是创建进程 API 的一个重要部分, 可以让父进程与子进程执行执行不同的程序. 例如我们修改子进程的代码:
```C++
    char* myArgs[3];
    myArgs[0] = strdup("wc");
    myArgs[1] = strdup("wait.c");
    myArgs[2] = NULL;
    execvp(myArgs[0],myArgs);//调用成功就不再返回
    printf("this line will never come out\n");
```
结果就是在子进程中调用字符计数器 wc, 对文件 wait.c 做计数. `exec()` 从可执行程序中加载代码和静态数据, 并且覆盖自己的代码段, 堆/栈 以及其他内存空间也会被重新初始化, 操作系统会执行唉程序, 对 `exec` 的调用永远不会返回.

#### 为何如此设计 API
事实证明, 分离 fork 和 exec 的做法在构建 UNIX Shell 的时候非常有用, 给了 shell 在执行 fork 之后 exec 之前的运行代码的机会, 这些代码可以在运行新程序之前改变环境, 从而实现一系列有趣的功能. 例如 UNIX 中管道的实现, 用的是 `pipe()` 系统调用, 一个进程的输出被链接到一个内核管道上(队列), 另一个进程的输入也被链接到同一个管道上. 

### Chap 6: 受限直接执行(Limited Direct Execution)
虚拟化 CPU 的思想很简单: 运行一个进程一段时间, 然后运行另一个进程, 如此轮换. 构建如此虚拟化机制存在两个挑战:
* 性能
* 控制权

构建操作系统的一个主要挑战就是**在保持控制权的同时获得高性能**.
#### 基本技巧: 受限直接执行
直接执行的概念很好理解: 只需直接在 CPU 上运行程序即可.
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210519092134.png"/></div>

但是这在我们虚拟化 CPU 时产生了一些问题:
* 如果我们只运行一个程序, 操作系统怎么确保程序不做任何我们不希望它做的事同时依然高效运行
* 当我们运行一个进程时, 操作系统如何让它停下来并切换到另一个进程, 从而实现虚拟化 CPU 所需的时分共享

#### 受限制的操作
在 CUP 上运行会带来一个问题, 如果进程希望执行某种受限操作(如 IO/申请内存), 应该如何做?<br>
**硬件通过提供不同的执行模式来协助操作系统**, 在用户模式下, 应用程序不能完全访问硬件资源, 在内核模式下, 操作系统可以访问机器的全部资源. 还提供了陷入(trap)内核和从陷进返回(return-from-trap)到用户模式程序的说明, 以及一些指令, 让操作系统告诉硬件陷阱表(trap table) 在内存中的位置.<br>
现代操作系统提供了用户程序执行系统调用的能力, 允许内核小心地向用户程序暴露某些关键功能(文件访问, 创建销毁进程, 进程间通信, 分配内存 etc.). 执行陷阱时, 硬件要小心, 必须保存足够的调用者寄存器, 以便在操作系统发出从陷阱返回指令时能正确返回. <br>
系统调用看起来像是过程调用, 是因为它是一个过程调用, 但是隐藏在过程调用的内部是著名的陷阱指令. 剩下的问题就是陷进如何知道在 OS 内运行哪些代码, **内核通过在启动时设置陷阱表来实现**. 机器启动时, 在特权模式下执行, 告诉硬件在发生某些异常时间时需要运行哪些代码, 发生异常时通过特殊的指令通知硬件这些处理程序的位置. 一旦硬件被通知, 他就会记住这些处理程序的位置, 直到下次一重新启动机器.<br>
能够执行特权指令告诉硬件缺陷表位置是一个非常强大的功能, 这也是一项特权操作, 硬件会阻止用户模式下执行. 
<div align=center><img src="https://i.loli.net/2021/05/19/oz39teKy61Ans52.png"/></div>

#### 进程之间的切换
有一个关键的问题是 : 操作系统如何重新获取 CPU 的控制权, 以便它可以在进程之间切换.

##### 协作方式: 等待系统调用
协作风格下, 操作系统相信系统的进程会合理运行, 运行过长时间的进程会被假定暂时放弃 CPU, 以便操作系统可以运行其他任务. 而进程通过进行系统调用, 将 CPU 控制权转移给操作系统. 如果应用执行了某些非法操作, 也会将控制权转移给操作系统. 

##### 非协作方式: 操作系统进行控制
在协作方式中, 进程陷入无限循环时, 唯一的解决办法就是重启计算机. 在没有协作的方式下, 获得控制权的方式很简单: **时钟中断**. 和前面讨论系统调用一样, 操作系统必须通知硬件哪些代码在发生时钟中断时运行. 

##### 保存和恢复上下文
无论通过系统调用协作还是时钟中断强制执行, 都必须决定是继续运行当前程序还是切换到另一个进程, 决定由调度程序(scheduler)作出. 决定进行切换, OS 就会执行一些低层代码(上下文切换, context switch): 系统为当前正在执行的进程保存一些寄存器值, 并且为即将执行的进程恢复一些寄存器的值, 并且切换内核栈. 
<div align=center><img src="https://i.loli.net/2021/05/19/n4GxYWIbdCLaHOZ.png"/></div>

需要注意的是由两种类型的寄存器保存/恢复:
* 发生时钟中段的时候, 运行几次呢很难过的用户寄存器由硬件隐式保存, 使用该今晨的内核栈
* 操作系统决定从 A 切换到 B, 内核寄存器被软件明确保存, 但是这种刚好被存储在该进程结构的内存中

```x86asm
# void swt ch ( struct context ** old, st ruct context *new)
#
# S av e c urre n t regi s ter co ntext in old
# and then load register context from new.
.globl swtch
swtch:
# Sav e old regi s ter s
movl 4(%esp), %eax # put old ptr into eax
popl 0 (%eax)   # save the old IP
movl %esp, 4 (:%eax) # and stack
movl %ebx, 8 (:%eax) # and other registers
movl %ecx, 12 (%eax)
movl %edx, 16( %eax)
movl %esi, 20(%eax)
movl %edi, 24(%eax)
movl %ebp, 28(%eax)

# Loa d new regi s ter s
movl 4 (%esp), %eax # put new ptr into eax
movl 28 ( %eax) , :%ebp # restore other registers
movl 24(%eax), %edi
movl 20(%eax), %esi
movl 16(%eax), %edx
movl 12(%eax), %ecx
movl 8(%eax), %ebx
movl 4 (%eax), %esp # stack is switched here
pushl 0 (%e = z)   # return addr put in place
ret   # finally return into new ctxt
```

#### 担心并发吗
操作系统可能简单决定, 在中断处理时禁用中断, 但是这可能丢失中断, 在技术上是不好的. 操作系统开发了很多复杂的家锁方案, 以保护内部数据结构的并发访问, 使得多个活动可以同时在内核中进行. 这可能导致各种有趣并且难以发现的错误. 

#### 小结
实现 CPU 虚拟化的关键底层机制, 受限直接执行, 基本思路很简单: <font color=red>就让你想运行的程序在 CPU 上运行, 但首先确保设置好硬件, 以便没有操作系统帮助的情况下限制进程可以执行的操作</font>. 下一个问题就是: 在特定的时间, 我们应该运行哪个进程.

### Chap 7: 进程调度 - intro
我们应该如何考虑调度策略的基本框架?
* 什么是关键假设
* 哪些指标非常重要
* 哪些基本方法已经在早起系统中使用
#### 调度指标
我们在这里考虑任务周转时间: 任务完成的时间减去任务到达的时间.即
\[
    T_{turnaround} = T_{completion} - T_{arrival}
\]
另一个有趣的指标是公平(fairness), <font color=pink>性能和公平在调用系统中往往是矛盾的</font>. 

#### FIFO
最基本的算法被称为先进先出调度,有时候也称为先到先服务. 这容易引起护航效应, 即**一些耗时较少的潜在资源消费者被排在重量级的资源消费者后**.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210519113514.png"/></div>

#### 最短任务优先(Shortest Job First)
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210519132857.png"/></div>

SJF 策略很简单: 先运行最短的任务, 然后是次短的, 如此下去. 如果假设所有工作同时到达, 那么可以证明 SJF 是一个最优的调度算法. 但是我们的放宽这条假设, 那么就得到下图的调度:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210519132932.png"/></div>

在过去的批处理计算中, 开发了一些非抢占式(non-preemptive)的调度程序, 这样的系统会等每项工作做完再考虑是否运行新工作. 现代化的调度可以进行上下文切换, 几乎都是抢占式的(preemptive).

#### 最短完成时间优先(STCF)
基于时钟中断和上下文切换的讨论, 当 B/C 到达时, 调度程序可以抢占(preempt) 工作A, 这被称为 最短完成时间优先/抢占式最短作业优先 调度. 每当新工作进入系统时, 它会先确定剩余工作和新工作中, 谁的剩余时间最少, 然后调度该工作. SJF 是最优的, 那么 STCF 的最优性也是复合直觉的. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210519142501.png"/></div>

#### 新的度量指标: 响应时间
在批处理中 STCF 是一个很好的策略, 引入分时系统后, 用户将会坐到终端面前, 同时也要求系统的交互性要好. 响应时间为从任务到达系统到首次运行的时间:
\[
    T_{response} = T_{firstrun} - T_{arrival}
\]
下面的问题就是如何构建对响应时间敏感的调度程序.

#### Round Robin(轮转)
Round Robin 的基本思路非常简单, RR 在一个时间片内运行一个工作, 然后切换到队列中的下一个任务, 而不是运行一个任务直接到结束.时间切片必须是时钟中断周期的倍数, 我们看一个例子:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210519150900.png"/></div>

时间片太短是有问题的: 突然上下文切换的成本将影响整体性能(不仅来自于保存和恢复少量寄存器的操作, **在 CPU 高速缓存/TLB/分支预测和其他片上硬件中建立的大量状态刷新和引入**), 系统设计者需要权衡时间片的长度, 使其足够长以便摊销(amortize)上下文切换的成本.<br>
更一般地说, 任何公平的政策(如 RR), 即在小规模时间将 CPU 均匀分配到各个活动进程之间的策略, 在周转时间这类指标上表现不佳. 接下来我们要放宽两个假设: 作业没有 IO 和 每个作业的运行时间是已知的.

#### 结合 IO
当前正在运行的作业在 I/O 期间不会使用 CPU, 它将被阻塞等待 I/O 完成. 调度程序在 I/O 完成时做出决定, 会产生中断, 并发出 I/O 进程从阻塞状态移回就绪状态. 我们通过将每个 CPU 突发作为一项工作, 调度程序确保交互的进程经常运行, 当这些交互式作业在执行 I/O 时, 其他 CPU密集型作业将进行, 这种重叠可以更好地使用资源.
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210519153059.png"/></div>

#### 无法预知
事实上, 在一个通用的操作系统中, 他们对每个作业的长度知之甚少, 我们将在后面介绍没有先验知识的 SJF/STCF, 进一步与 RR 调度程序结合起来.

### Chap 8: 多级反馈队列
多级反馈队列(Multi-level Feedback Queue, MLFQ) 需要解决两个问题:
1. 首先要优化周转时间, 操作系统不知道工作要进行多久, 而这是 SJF 等算法所必须的
2. 其次为了给交互用户更好的体验, 需要降低响应时间

在这里我们了解调度程序如何在过程中学习进程的特征, 从而做出更好的调度决策. 当然, 这可能出错, 作出比一无所知时候更糟糕的决定.

#### MLFQ 基本规则
MLFQ 中有许多独立的队列(queue), 每个队列有不同的优先级(priority level), 任何时刻, 一个工作只能存在于一个队列中:
* MLFQ 总是执行优先级高的工作 (规则 1)
* 每个队列中可能会有多个工作, 相同的优先级采用轮转调度(规则 2)

于是, MLFQ 的调度关键在于如何设置优先级, 它并不是固定的, 而是根据观察到的行为调整它的优先级.   

#### 尝试 1: 如何改变优先级
我们在这给出一种一个工作的生命周期中, MLFQ 如何改变优先级的算法(需要注意工作负载中有时间很短, 频繁放弃CPU的交互型工作, 也有需要很多时间, 响应不那么重要的计算密集型工作)
* 规则3: 工作进入系统时, 放在最高优先级
* 规则 4a: 工作用完整个时间片后, 降低其优先级
* 工作 4b: 如果在其时间片内释放 CPU, 则优先级不变

##### case 1: 单个长工作
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520101517.png"/></div>

##### case 2: 来了一个短工作
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520101700.png"/></div>

这时候我们就发现这个算法的一个主要目标: **如果不知道工作是短工作还是长工作, 就在开始的时候假定其是短工作, 并赋予最高优先级, 如果确实是短工作就会很快执行完毕, 否则将被移入低优先级的队列, 这时候该工作就被认为是长工作了**. 通过这种方式, MLFQ 近似于 SJF.  
##### case 3: 如果有 IO
IO 操作会使得任务在时间片完成之前放弃 CPU, 这时候不处罚它, 只是保证其优先级不变.

##### 当前算法的问题
目前的算法有几个问题:
1. 首先会有饥饿问题: 系统有太多交互型工作, 就会不断占用 CPU, 导致长工作饥饿
2. 聪明的用户能够愚弄调度程序
3. 一个程序在不同的时间可能有不同的表现

#### 尝试 2: 提升优先级
上面提到的问题, 我们能想到的一个思路是周期性地提升所有工作的优先级. 
* 规则 5: 经过一段时间 S, 就将系统中所有工作重新加入最高优先级队列
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520104157.png"/></div>  

这带来一个明显的问题, S 的值应该如何设置, 这似需要一点黑魔法, 如果 S 设置得太高, 长工作会饥饿, 太低则交互型工作得不到合适的 CPU 时间比例.  

#### 尝试 3: 更好的计时方式
如何阻止调度程序被愚弄, 元凶是 4a/4b, 我们提供一个更好的计时方式, 记录一个进程在某一层中消耗的总时间, 而不是在调度时重新计时. 
* 规则 4: 一旦工作用完了其在某一层中的时间配额(无论中途主动放弃了多少次CPU), 就降低其优先级
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520104648.png"/></div>

这样的话, 就无法通过在时间片结束前调用 IO 从而垄断 CPU 时间. 

#### MLFQ 调优及其他问题
很多细节的问题, 如配置多少队列, 每个队列时间片多大等, 只有利用对工作负载的经验,以及后续对调度程序的调优, 才会导致令人满意的平衡.  
大多数 MLFQ 都支持不同队列可变的时间长度, 一般高优先队列只有较短的时间片, 可以快速切换, 低优先队列配置较长的时间片. 

### Chap 9: 调度 -- 比例份额
比例份额调度程序又是被称为公平份额调度程序, 基于一个简单得想法: **调度程序的最终目标, 是确保每个工作获得一定比例的 CPU 时间,而不是优化周转时间和响应时间**.其中一个例子就是彩票调度: 每隔一段时间就会举行一次彩票抽奖决定接下来运行哪个进程, 越是应该频繁运行的进程, 越要有更多赢得彩票的机会.  
#### 基本概念: 彩票数表示份额
彩票数(ticket) 代表了进程占有某个资源的份额, 随机方法相对于传统方法有几个优势:
1. 随机方法常常可以避免奇怪的边角情况
2. 随机方法轻量, 几乎不需要记录任何状态
3. 随机方法很快

#### 彩票机制
彩票调度提供了一些机制, 用不同且有效的方式来调度彩票.
1. 利用彩票货币(ticket currency), 允许拥有一组彩票的用户以他们喜欢的某种货币将彩票分配给自己的不同工作, 操作系统自动将这种货币兑换为正确的全局彩票
   <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520112309.png"/></div>
2. 彩票转让: 一个进程可以临时将自己的彩票交给另一个进程
3. 彩票通胀: 进程之间相互不信任就没有意义, 在信任的环境中, 如果一个进程知道它需要更多的 CPU 时间, 就可以增加自己的彩票, 将此告知操作系统而无需与其他进程通信

#### 实现
最简单的实现需要一个随机数生成器和一个记录所有进程的数据结构:
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520112732.png"/></div>

```C++
 // counter: used to track if we’ve found the winner yet
 int counter = 0;
 // winner: use some call to a random number generator to
 // get a value, between 0 and the total # of tickets
 int winner = getrandom(0, totaltickets);
 // current: use this to walk through the list of jobs
 node_t *current = head;
 while (current) {
    counter = counter + current->tickets;
    if (counter > winner)
    break; // found the winner
    current = current->next;
 }
 // ’current’ is the winner: schedule it...
 ```

#### 实例
我们引入两个互相竞争的工作, 有相同的彩票和运行时间, 而公平指标定义为两个工作的完成时刻:

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520113247.png"/></div>

轻易发现只有当工作执行非常多的时间片时, 彩票调度算法才能得到预期的结果.此外对于给定的一组工作, 彩票分配的问题依然没有最佳答案.

#### 为什么不是确定的

前面看到了, 随机方式可以使得调度程序实现简单且大致正确, 但偶尔不能产生正确的比例, 尤其是工作的运行时间很短的情况下.   
步长调度的实现很简单, 每个工作都有自己的步长, 这个值和票数成反比. 我们得到进程的步长(stride)后, 每次运行都会让它的计数器(pass value, 行程值) 增加步长值, 记录整体进展.  
调度器使用进程步长以及行程值确定调用哪个进程: **需要进行调度的时候, 选择目前拥有最小行程值的进程, 并在结束后增加一个步长**.  
```C++
curr = remove_min(queue);   // pick client with min pass
s chedule(curr);   // run for quantum
curr->pass += curr->stride;   // update pass using stride
insert(queue, curr);   // return curr to queue
 ```

#### 小结
总之, 两种调度都没作为 CPU 调度广泛使用, 他们都能很好地适应 IO, 同时无法知道具体应该拥有多少ticket. 只有在相对容易解决这两个问题的领域更好用, 如在虚拟化的数据中心中.

### Chap 10: 多处理器调度
随着多处理器系统的普及, 操作系统遇到的一个新问题就是多处理器调度. 我们接下来考虑如何将前面的调度原则的想法应用到多处理器上, 同时会出现什么新的问题, 应该如何解决.

#### Background: 多处理器架构
多处理器引入的区别核心在于**对于硬件缓存的使用**, 以及**多处理器之间共享数据的方式**.  
在单 CPU 系统中, 存在多级的硬件缓存, 程序第一次读取数据时, 数据在内存中, 需要花费较长的时间, **处理器判断该数据很可能被再次使用, 因此将其放入 CPU 缓存中**. 缓存系统基于局部性(locality)的概念:
* 时间的局部性: 一个数据被访问后, 很可能在不久的将来被再次访问
* 空间的局部性: 程序访问地址为 x 的数据后, 很可能会紧接着访问 x 周围的数据

如果多个处理器并且共享一个内存, 结果如何?
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520145558.png"/></div>

最普遍的问题就是缓存一致性: 某位置的内存数据D, A 将其修改后没有写回数据D', 而B 去访问的时候没有更新数据得到了旧值 D. 硬件提供了这个问题的基本解决方案: 通过监控内存访问, 硬件可以保证获得正确的数据, 并且保证共享内存的唯一性. 在基于总线的系统中, 一种方式是使用总线窥探.

#### 别忘了同步
跨 CPU 访问(尤其是写入)共享数据或者数据结构时, 需要使用互斥原语(mutual exclusion primitives) 才能保证正确性. 例子下面的代码中:
```C++
typedef struct ____Node_t {
    int    value;
    struct ___Node_t *next;
} Node_t;
int List_Pop() {
    Node_t *tmp =    head;    //    remember old head ...
    int value   =    head->value;    //    ... and its value
    head        =    head->next;    //    advance head to next pointer
    free(tmp);    //    free old head
    return value;    //    return value at head
}
```
两个 CPU 同时进入函数, 想要各删除一个节点, 如果不加锁可能就是对 head 做 double free, 这是非常危险的. 这就需要在函数开始前调用 `lock(&m)` 结束后调用 `unlock(&m)`, 确保代码按照预期执行. 但是加锁**会有性能问题**, 随着 CPU 数量的增加, 访问同步共享的数据结构就会变得很慢.   
#### 缓存亲和度
一个进程在某个 CPU 上运行, 会在缓存中维护很多状态, 该进程在相同 CPU 运行的时候, 由于缓存数据而执行的更快. 因此 **尽可能将进程保持在同一个 CPU 上**.

#### 单对列调度
这是对单处理器调度基本架构的简单复用, 将所有需要调度的工作放在一个单独的队列中, 被称为单队列多处理器调度(**single-queue multiprocessor scheduling**, SQMS).它有两个尖锐的问题:
1. 缺乏可扩展性: 调度程序开发者需要通过加锁来保证原子性, 当 CPU 增加时, 系统花费了越来越多的时间在锁的开销上
2. 缓存亲和性问题: 大多数 SQMS 引入了一些亲和机制
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520152829.png"/></div>
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520152853.png"/></div>
    引入后只有工作E 在不同的 CPU 上来回迁移. 为了公平还可以选择不同的工作来迁移, 只是其实现要复杂一些.

#### 多队列调度
SQMS 基本调度框架**包括了多个调度队列, 每个队列可以使用不同的调度规则**, 比如轮转或者其他任何可能的算法. 工作进入系统后, 会依照一些启发性的规则将其放入某个调度队列, 这样每个 CPU 的调度之间相互独立, 避免了单队列方式中由于数据共享以及同步带来的问题.   
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520153309.png"/></div>

但是这就带来一个新的问题: **负载不均衡**. 
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210520153357.png"/></div>

最明显的解决办法是让工作移动, 这被称为迁移. 而不同的迁移模式确实最棘手的部分. 一种基本的方法被成为 工作窃取(**work stealing**): 工作量较少的(源)队列不定期偷看其他队列是不是比自己的工作多, 如果显著地工作更慢, 就从目标队列窃取一个或者多个工作, 实现负载均衡. 这种方法频率的确定又是一种黑魔法.

### Chap 12 : 内存虚拟化 Intro
我们在这里将从简单开始, 使用诸如基址/界限等非常基本的技术, 慢慢增加复杂性以应对新的挑战, 包括 TLB 和多级叶表, 最终能描述一个全功能的现代虚拟内存管理程序的原理. 操作系统提供虚拟地址这个假象有很多原因:
* 主要为了方便使用
* 隔离和保护

