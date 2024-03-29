# Computer Organization and Design: the Hardware/Software Interface
<font size=4> [David A. Patterson](https://www2.eecs.berkeley.edu/Faculty/Homepages/patterson.html) and [John L. Hennessy ](https://web.stanford.edu/~hennessy/) </font>

# Chap 1: 计算机抽象和技术
## Introduction
### 传统的计算机分类和应用
PCs: 对单用户在低价区间提供良好的性能, 通常运行第三方软件.  
服务器: 执行大负载任务, 一般要求更强的**可靠性**.  
嵌入式计算机:目标是运行单一应用程序或者一组相关的应用程序, 一般对于故障的容忍度低.  

## 计算机体系结构中的八大伟大思想
### Design for Moore's Law
计算机设计者面临的一个永恒的问题就是摩尔定律驱动的快速变化, 他们必须预测其完成时候的工艺水平,而不是用设计开始时候的.

### 使用抽象简化设计
提高软件硬件生产能力的一个major technique 就是使用抽象来表示不同的设计层次, 在高的层次上只能看到底层低的简化模型, 而细节被隐藏起来了.

### 加速common 的事件
加速大概率事件意味着你需要知道大概率事件是什么, 这需要仔细的实验和测量评估.

### 通过并行提高性能
从计算机诞生开始, 计算机的设计者就通过并行执行操作来提高性能. 

### 通过流水线提高性能
在计算机体系结构中一个特别的并行场景就是流水线.

### 通过预测来提高性能
执行错误的恢复代价不高而且预测相对准确的时候, 可以用猜测的方式提前开始某些操作.

### 存储器层次
程序员想要更快/更大/更便宜的内存, 设计师们发现通过存储器的层次结构来解决这些相互矛盾的需求.

### 通过冗余提高可靠性
任何一个人物理器件都可能失效, 因此可以使用冗余部件的方式来提高系统的可靠性.**冗余部分可以替代失效部分并且帮助检测错误.**

## 程序的背后
系统软接位于应用软件和硬件之间, 两种系统软件对于现代计算机来说是必须的: 
* **操作系统** 是用户程序和硬件之间的接口, 为用户提供各种服务和监控功能.
    1. 处理基本的输入输出操作
    2. 分配storage and memory
    3. 为多个应用程序同时提供受保护的共享的计算机资源服务
* **编译器** 将高级语言翻译成计算机所能识别的机器语言程序.

### 从高级语言到硬件语言
计算机服从于我们的指令(instruction,能被计算机识别并执行的位串，可以将其视为数).  
后来程序员发明了一种被称为汇编器(assembler)的软件, 将symbolic版本的程序(汇编语言写成)翻译成binary版本, 如`add A,B` 被翻译成`1000110010100000`,而机器可以理解的二进制语言是机器语言. 汇编语言的程序效率依旧比较低下, 后来人们创造了高级程序设计语言.  
<div align=center><img src="https://i.imgur.com/UMIKie2.png" width="50%"/></div>
高级程序设计语言的使用带来了很多好处, 包括
* 用更像自然语言的方式思考, 同时可以为特定的应用设计语言
* 提高了程序生产效率

## Under the Covers
任何一个计算机的基础硬件目标都是完成相同的基本功能: **输入数据、输出数据 处理数据 和 存储数据**.于是计算机的主要部分分别为input, output, memory,datapath(也叫运算器), and control.  
<div align=center><img src="https://i.imgur.com/jU22lqS.png" width="50%"/></div>
### 显示器
图像由像素矩阵组成，可以表示成二进制位的矩阵，称为位图 (bit map).计算机硬件采用光栅刷新缓冲区（又称为帧缓冲区）来保存位图以支持图像, 要显示的图像保存在帧缓冲区中，每个像素的二进制值以刷新频率读出到显示设备。

### Opening the Box
处理器从逻辑上包括两个主要部件：**数据通路和控制器**．数据通路 (datapath)负责完 成算术运算 ，控制器 (control) 负责指导数据通路、存储器和 IO 设备按照程序的指令正确执行.  
内存(memo­ry,由动态随机访问存储器(DRAM)组成)是程序运行时的存储空间，它同时也用于保存程序运行时所使用的数据.  
在处理器内部有另一种存储器, 被称为**缓存(chche)**,它采用的是另一种被称为SRAM(静态随机访问存储器)的存储技术，速度更快而且不那么密集,SRAM 和 DRAM 是存储器层次中的两层。  
抽象层次中的一个关键接口是指令集体系结构(ISA) 一 硬件和底层软件之间的接口。

### 数据安全
计算机中的内存是易失性存储器(volatile memory),而在存储器层次中它的下一层是非易失性存储器(nonvolatile memory, 也叫secondary memory), 在过去的计算机中它可能是磁盘,而在ipad等设备中使用非易失性半导体存储器,闪存 (flash memory).

### 与其他计算机通信
* 局域网(local area network, LAN)：一种在一定地理区域（例如在同一栋大楼内）使用的传输数据的网络。
* 广域网(wide area network, WAN)：一种可将区域扩展到几百千米范围的网络。

## 处理器和存储器制造技术
晶体管(transistor) 仅仅是一种受电流控制的开关。 集成电路 (IC) 是由成千上万个晶体管组成的芯片, 它的制造是从硅锭(silicon crystal ingot),经切片机切成厚度不超过 0.1 英寸的晶圆 (wafer)。圆晶经过一系列化学加工过程最终产生之前所讨论的晶体管、导体和绝缘体。晶圆中或是在图样化的几十个步骤中出现一个细微的瑕疵就会使其附近的电路损坏，这些瑕疵 (defect) 使得制成一个完美的晶圆几乎是不可能的。 通过切分，可以只淘汰那些有瑕疵的芯片(chip).合格芯片要连接到 1/0 引脚上，这一过程称为封装。在封装之后，必须进行最后一次测试，因为封装过程也可能出错。最后芯片被交付给用户。

## 性能
* 响应时间：也叫执行时间 (execution time) , 是计算机完成某任务所需的总时间，包括硬盘访问 、内存访问 、 I/O 活动、操作系统开销和 CPU 执行时间等。
* 吞吐率：也叫带宽 (bandwidth), 性能的另一种度量参数，表示单位时间内完成的任务数量。

### 性能的度量
时间来度量计算机的性能，那么完成同样的计算任务，需要时间最少的计算机是最快的。 
* 墙上时钟时间: 完成任务所需的总时间，包括了硬盘访问、内存访问、 I/O 操作和操作系统开销等一切时间。
* CPU 执行时间 (CPU executiontime) , 简称 CPU 时间，它只表示在 CPU 上花费的时间，而不包括等待 I/O 或运行其他程序的时间。

**要改进一个程序的性能，必须明确性能的定义，然后通过测量程序执行时间来寻找可能的性能瓶颈**。

### CPU 性能及其因素
$$
\begin{aligned}
一个程序的CPU 执行时间 &=& 程序的CPU时钟周期数 \times 时钟周期时间 \\
一个程序的CPU 执行时间 &=& 程序的CPU时钟周期数 / 时钟频率
\end{aligned}
$$

### 指令的性能
$$
CPU时钟周期数 = 程序的指令数 \times 每条指令的平均时钟周期(CPI)
$$

### 经典性能公式
在前面两者的基础上, 我们可以得到经典CPU性能公式,它特别有用，因为它们把性能分解为三个关键因素。
$$
\begin{aligned}
CPU时间 &=& 指令数 \times &CPI \times 时钟周期时间\\
CPU时间 &=& 指令数 \times &CPI / 时钟频率
\end{aligned}
$$
一般而言, CPI和总的指令数相对于CPU时间和clock rate是难以获得的, 而指令数量取决于指令集架构(ISA)而不是其实现(微结构),我们可以在不知道计算机全部实现细节的情况下对指令数进行测量。 

## 功耗墙
CMOS (互补型金属氧化半导体)，其主要的能耗来源是动态能耗(静态能耗也是存在的)，即在晶体管开关过程中产生的能耗，即晶体管的状态从 0 翻转到 1 或从 1 翻转到 0 消耗的能量。
$$
能耗 \propto 负载电容 \times 电压^2
$$
这表示的是一个010 和 101 的逻辑转换过程中消耗的能量, 一个晶体管消耗的能量是这个的一半, 而其功耗是能耗乘以开关频率:
$$
功耗 \propto 负载电容 \times 电压^2 \times 开关频率
$$
开关频率是时钟频率的函数，负载电容是连接到输出上的晶体管数量和工艺的函数.

## 从单处理器向多处理器转变
程序员要想显著改进响应时间，必须重写他们的程序以充分利用多处理器的优势。过去的流水线技术是指令级并行的一个例子, 在抽取了硬件的并行本质之后，程序员或编译程序可认为在硬件中指令是串行执行的, 但是将来程序员最终将成功跃进到显式并行编程.   
并行编程带来了很大的挑战, 包括：调度、负载平衡、通信以及同步等开销。 

## 谬误与陷阱
陷阱：在改进计算机的某个方面时期望总性能的提高与改进大小成正比。  
实际上这是由Amdahl 定律计算得到的.
$$
改进后的执行时间＝受改进影响的执行时间／改进量＋不受影响的执行时间
$$
谬误：利用率低的计算机功耗低。  
谬误：面向性能的设计和面向能量效率的设计具有不相关的目标。  
由于能耗是功耗和时间的乘积，在通常情况下，对千软硬件的优化而言，即使在优化的部分起作用时能耗可能高了一些，但是这些优化缩短了系统运行时间 ， 因此整体上还是节约了能量。  
陷阱：用性能公式的一个子集去度量性能。  
有一种用 MIPS(每秒百万条指令)取代时间以度量性能的方法,但是它是有问题的.MIPS 规定了指令执行的速率，但没有考虑指令的能力。一计算机上，不同的程序会有不同的 MIPS, 因而一台计算机不会只有一MIPS值, **如果一个新程序执行的指令数更多，但每条指令的执行速度更快，则 MIPS 的变化是与性能无关的**。

# Chap2: 指令--计算机的语言
计算机语言中的基本单词称为指令，一台计算机的全部指令称为该计算机的指令集(IS).  
不同的指令集有很大的相似性, 因为**计算机都是基于基本原理相似的硬件技术所构建的,此外所有计算机都必须提供一些基本操作**.

理解如何表达指令, 读者也将发现计算的秘密: 存储程序概念(stored-program concept).

## 计算机硬件的操作
每个RISC-V 算术指令只执行一个操作, 并且有且仅有3个变量,这一点符合硬件的简单性设计原则(操作数个数可变将给硬件设计带来更大的复杂性).
```assembly
add a, b, c // 将 b, c 变量的值加起来存在 a 中
```
**设计原则 1: 简单源于规整**

## 计算机硬件的操作数
**算术指令的操作数十分严格, 他们必须来自寄存器**(由硬件直接构建并且数量有限).可以认为它们是构成计算机"建筑"的"砖块", RISC-V 体系结构的集群起是 64 bit, 经常被称为是 double-word(相对的, 32 bit 被称为 word).典型的 RISC-V 芯片上一共有 32个寄存器, 算术操作的三个操作数一定要从这些寄存器中选择. 

**设计原则 2: 越小越快** 设计者必须在程序期望更多寄存器和加快时钟周期之间进行权衡, 在RISC-V中用`x0` ~ `x31`代表32个寄存器.
```C
f = (g + h) − (i + j);
// 下面是在 x86-64 平台使用 riscv64-elf-gcc 编译得到对应的汇编
addw    a0,a0,a1
addw    a2,a2,a3
subw    a0,a0,a2
```
### 存储器操作数
处理器在寄存器中只能保存少量数据, 因此数据结构(数组和结构体)是保存在内存中. 
RISC-V的算术运算指令只对寄存器进行操作,所以**RISC-V 必须包含在存储器和寄存器之间传输数据的指令**(data transfer instruction),指令通过存储器地址访问其中的字,将数据从存储器复制到寄存器的数据传送指令通常叫取数(load)指令, 真实的RISC-V的取数指令名字为 `ld`(load doubleword缩写).  
```c
g = h + A[8]; // C code
ld x9, 8(x22) // Temporary reg x9去存到A[8]
add x20, x21, x9 // 将 h和 temp reg值加起来放到 x20
// 使用 riscv64-elf-gcc 输出
lw  a5,32(a1)
addw    a0,a5,a0
```
汇编中使用偏移来处理数组, x22(存放A的首元素地址)被称为基础寄存器(base register), 指令中的常量(本例中为8) 被称为偏移量(offset).

对齐限制: 数据地址与存储器的自然边界对齐的要求, 在 RISC-V 和 X86 体系结构中没有 alignment restriction, 但是 MIPS 有.  

risc-v 中将数据写到内存中的指令为 `sd`(store doubleword):
```C
A[12] = h + A[8];
ld x9, 64(x22) // 让 x9 寄存器存入数据 A[8]
add x9, x21, x9 // 将 A[8] 和 h 之和写入 x9
sd x9, 96(x22)  // 将 x9 中数据写入 A[12] 中
```
很多程序的变量比寄存器的个数多, 因此编译器会将最常用的变量保存在寄存器中, 而用 load 和 store 相应的指令在寄存器和内存之间移动数据, 将不常使用的变量（或稍后才使用的变量）存回到存储导中的过程叫作寄存器溢出(register spilling). 相对而言, **寄存器的数据访问远远快于内存, 而且更容易利用**(可以在一条指令中读取两个寄存器的值/对他们进行运算/写回寄存器, 但是对一条指令只能加载或者写入内存中的数据, 而且不能对他们进行运算). 因此指令集的体系结构必须拥有足够多的寄存器, 并且编译器必须高效地利用这些寄存器.

### 常数或立即数操作数
程序中经常会在某个操作中使用到常数, 为了避免从存储器中取出常数, 指令集提供其中一个操作数是常数的算术运算指令, 这种有一个常数操作数的快速加法指令叫作加立即数(add immediate), `addi`. 常数 0 还有另外的作用, 例如使用 0 减去一个寄存器值取其相反数, RISC-V 将 `x0` 恒置为0.(根据使用频率加速大概率事件)

## 有符号数和无符号数
在计算机硬件中，数是以一串或高或低的电信号来体现的，这恰好可以被认为是基为2的数.对于一个 64 bit 的数, 我们用**最低有效位**(least significant bit) 表示最右边的一位,**最高有效位**(most significant bit)表示最左边的一位.

最直观的表示方法就是用一个64位的 bit vector ($x_{63} x_{62}\cdots x_1 x_0$) 表示下面的整数, 这种记法被称为无符号数:
$$
(x_{63}\times 2^{63}) + (x_{62} \times 2 ^{62}) + \cdots + (x_1 \times 2^1) + (x_0 \times 2^0)
$$
如果操作的正确结果不能被最右端的硬件位所表示，那么就发生了溢出(overflow), 如何处理溢出是由编程语言/操作系统和程序来决定的. 

计算机程序还需要对负数进行计算, 区分正负的最直观方法是增加一个独立的符号位, 这种方法称为符号和幅值表示法(sign and magnitude representation). 这种便是法的缺点是显而易见的, 首先符号位放在哪里是一个问题, 无法再就是爱你时提前知道结果的符号, 需要额外的操作来设置符号, 另外这意味着数中有两个零(正零和负零). **最终的一个解决方法是一种被称为二进制补码的实现**, 这样的话一个bit vector ($x_{63} x_{62}\cdots x_1 x_0$) 就表示这样的整数:
$$
-(x_{63}\times 2^{63}) + (x_{62} \times 2 ^{62}) + \cdots + (x_1 \times 2^1) + (x_0 \times 2^0)
$$
这时候溢出发生在**有限二进制数最左边的符号位与采用无穷多位表示该数时左边位的值不同的情况**下,我们用一个 4 bit 简单例子说明, -9 在这无穷位的情况下表示为 $111\cdots10111$, 但是 4 bit 只能是 $0111$, 符号位变了,从一个负数溢出为正.

需要注意的是, **取数的指令对于有符号和无符号的数是有区别的**, 因为我们取有符号数时候需要使用符号位填充剩余的高位(符号扩展), 取无符号数的时候只需要往高位填充 0. RISC-V 提供 `lbu`(load byte unsigned) 和 `lb`(load byte) 分别对应这两种情况. 处理二进制补码快速取反的方法,**简单对每一位取反(0 变成1 , 1 变成0) 然后对结果加1** 。 如我们想要得到 -7, 7 的二进制表示是  `0111`, 取反后 `1000`, 再加1 得到补码表示 `1001`, 这个结果的原理是一个数和取反结果相加为 $111\cdots111$, 即补码表示的 -1. 

## 计算机中指令的表示
指令的各部分都可看成一个独立的数，将这些数拼接在一起就形成了指令, RISC-V 中的32个寄存器就是它们的数字(从 0 到 31). 机器指令分成若干字段(field), 例如 `add x9, x20, x21`用decimal 表示就是 0 21 20 0 9 51, 第 1/4/6 三个字段组合起来表示要执行的是加法操作, 剩下几个字段表示的是源寄存器和目标寄存器.
<div align=center><img src="https://i.imgur.com/s0pdekf.png" width="70%"/></div>

指令的布局形式叫作指令格式(instruction format), 从上面可以看到, RISC-V 的指令占有 32 bit(1 word), 简单源于规整, 所有 RISC-V 的指令都是 32 bit. 如果指令要比上述字段更长的字段时, 就会发生问题, 例如取自指令指定了两个集群起和一个常数, 对于常数来说 5 位的字段常常太小而用处不大, **优秀的设计需要适宜的折中方案**, 解决的办法就是保持指令的长度相同, 但是不同的指令采用不同的格式, 上面的方案是用于寄存器(R-Type), 对于立即数还有新的格式(I-Type):
<div align=center><img src="https://i.imgur.com/uwgXzAA.png" width="70%"/></div>

当我们在使用 load 指令的时候, `ld x9 64(x22)`, 我们需要两个 source 寄存器, 一个存放 base 地址 一个存放 data, 这时候我们使用的是一种 S-Type 的格式:
<div align=center><img src="https://i.imgur.com/3wqtsKs.png" width="70%"/></div>

RISC-V 可以推断是用的 `addi/add`, 故不强制要求, 只是为了方便理解吸管上加立即数还是会使用 `addi`, 另外没有 `subi`, `addi` 中的立即数是使用补码实现的, 因此这月可以用于减法. 

这部分有两个关键的思想, 引出了存储程序的概念释放了计算机的巨大潜力:
1. 指令用数的形式表示
2. 和数据一样, 程序存在存储器中, 并且可以读写

## 逻辑操作
人们发现对字中由若干位组成的字段甚至对单个位进行操作是很有用的, 编程语言和体系结构中增加了一些指令, 用于简化对字中若干位进行打包或者拆包的操作, 这些指令被称为逻辑操作。
<div align=center><img src="https://i.imgur.com/PmmTE1S.png" width="70%"/></div>

上面的这些操作都是可以按照字面上去理解其含义, 有几点需要注意的地方在于:
1. 注意区分逻辑右移(移动产生高空位使用0填充)和算术右移(移动产生高空位使用 sign bit 填充)
2. 为了保证全是用三操作数的格式, RISC-V 的设计者用异或操作去实现取反(NOT 相当于和 $11 \cdots 1$做异或)

## 决策指令
计算机与简单计算器的区别在于决策能力, RISC-V 中有两个和 if/goto 功能类似的指令, 分别是
```C++
beq register1, register2, L2  // branch if equal
bne register1, register2, L2  // branch if not equal
```
这个语句被称为是条件分支, 分别代表如果两个寄存器中值相等(beq)/不相等(bnq) 则跳转到标签为 L1 的语句执行. 在if 语句的结尾部分，需要引入另一种分支指令，通常叫作无条件分支指令,一个表示无条件分支的指令是设置恒为 true 的条件分支:
```C
if(i == j) f = g + h;
else f = g - h;
// assembly code 
bne x22, x23, Else // 如果不想等的话跳转到 Else
add x19, x20, x21  // f = g + h
beq x0, x0, Exit   // 无条件分支, 跳至退出处
Else: sub x19, x20, x21 // f = g - h
Exit: ...
```

### 循环
决策指令在两个场景很重要(分支和循环), 在 这两种苍井下汇编指令是相同的. 我们举一个简单的循环程序例子:
```C
while(save[i] == k)
    i = i + 1;
__asm{
    Loop: slli x10, x22, 3 // 临时寄存器 x 10 = i * 8
          add x10, x10, x25 // x25为save[0] 地址, x10 为 save[i] 地址
          ld x9, 0(x10)
          neq x9, x24, Exit // 如果不想等的话直接退出
          addi x22, x22, 1 // i= i +1
          beq x0, x0, Loop // while 语句
    Exit:
        ...
}
```
基本块: 没有分支(可能出现在末尾的除外)并且没有分支目标/分支标签(可能出现在开始的除外) 的指令序列. 以分支指令结束的这类指令序列对于编译非常重要, 因此有这个专用的术语进行描述. 

除了相等的判别, 有时候判断其他的关系(例如大于`>`小于`<`不小于`>=`不大于`<=`...)也非常重要,而这种 Bit pattern 的大小对比分为 signed 和 unsigned 两种情况:
* `blt reg1, reg2, Label`: branch less than, 如果 reg1 更小的话则跳转(补码)
* `bge reg1, reg2, Label`: branch greater or equal (补码) 
* `bltu/bgeu` 对应 Unsigned 数字的比较

MIPS 指令集和 ARM 指令集用了其他的方式来处理:
* 使用一个临时的寄存器,其中的结果基于比较, 然后用 `beq/bnq` 来跳转: 使得 datapath 更加简单, 但是需要更多指令来执行`slt` (set on less than) 和`slti`(set on less than immediate) 指令用于处理有符号整数，而`sltu` (set on less than unsigned)和 `sltiu` (set on less than immediate unsigned) 指令则用于处理无符号整数)
* ARM 使用 condition code, 记录算术操作的结果是负数/0/溢出
  * negative (N) – the result that set the condition code had a 1 in the mostsignificant bit;
  * zero (Z) – the result that set the condition code was 0;
  * overflow (V) – the result that set the condition code overflowed;
  * carry (C) – the result that set the condition code had a carry out of the most significant bit or a borrow into the most significant bit.

### 边界检查捷径
对于一个经常用于 array 边界检查的语句`0 <= x < y`, 而负数的补数是一个很大的数, unsigned 检查 `x<y` 就可以cover 上面两个检查.
```C
bgeu x20, x11, IndexOutOfBounds // if x20 >= x11 or x20 < 0, goto IndexOutOfBounds
```
### Case/Switch 语句
实现switch 语句的最简单方法是借助一系列的条件判断，将switch 语句转化为 if-then-else 语句嵌套。有时候另一种更有效的方法是将多个指令序列分支的地址编码为一张表，即**转移地址表**(或者转移表 Jump Table), 转移地址表是一个由代码中标签所对应地址构成的数组。程序需要跳转的时候首先将转移地址表中适当的项加载到寄存器中，然后使用寄存器中的地址值进行跳转。这需要使用寄存器中的地址做branch, RISC-V 中使用一个 Inirect Jump 指令, 执行一个到寄存器地址处的 无条件分支, jump-and-link 寄存器指令(`jalr`) 就是用于这个目的. 

## 计算机硬件对过程的支持
过程(procedure)/函数 是程序员进行结构化编程的工具, 有助于提高代码的可复性性和程序的可读性. 在过程运行中, 程序要遵循以下步骤:
1. 将参数放在一个过程能够访问的位置
2. 将控制权交给过程
3. 获得过程需要的存储资源
4. 执行需要的任务
5. 将返回值放在调用程序可以访问的位置
6. 将控制权返回初始点

计算机中存放数据最快的位置是寄存器, 我们希望尽可能更多地使用寄存器, RISC-V 遵循下面的一些寄存器使用约定:
1. `x10 ~ x17` 用于传递参数或者返回值
2. `x1` 用千返回起始点的返回地址寄存器

除了分配这些寄存器之外， RISC-V 汇编语言还包括一条过程调用指令：跳转到某个地址的同时将下一条指令的地址保存在寄存器(jump-and-link instruction)`jal`:
```C
jal x1, ProcedureAddress // jump to ProcedureAddress and write return address to x1
```
几个名词解释:
* 返回地址: 一个指向调用处的link, 使得procedure 调用完毕之后可以回到正确地地方, RISC-V 中用它存在寄存器 _x1_ 中
* 调用者(caller): 调用 procedure 并且提供参数的程序