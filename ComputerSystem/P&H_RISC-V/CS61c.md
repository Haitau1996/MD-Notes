# CS61C: Great Ideas in Computer Archicture

## Lecture 1: Cource Introduction
**计算机体系结构的六大思想**
1. 抽象
2. 摩尔定律
3. 局部性原则/内存层次结构
4. 并行(Amdahl's Law)
5. 性能测量&提升
6. 通过冗余提高可靠性

为什么现在体系结构还是 Exciting? 
* 部署的设备数量一直在增加, 但是没有单个 killer App, 这意味着有不同的需求和体系结构
* Clock Cycle 和 Thermal Design 增长变平了

## Lecture 2: 数字表示
#### Data Input : Analog `->` Digital
为了导入模拟信号, 我们必须做两个事情:
* 采样
* 量化

#### Big Idea: Bit 可以表示任何事情
* 字符?
  * 26个字母需要 5 Bit
  * 7 Bit(in 8) 的 ASCII, 16 32 的 Unicode
* 逻辑值 1 Bit
* **N Bit 可以表示最多 $2^N$ 个东西**

几种常见的进制 2,10, 16.从十进制转为其他进制的时候, 可以理解成用不同大小的盒子装东西, 先用最大的盒子, 然后依次递减. 

负数表示:
1. One's Complement(正数保持不变, 负数翻转):
    ![](CS61c/2.1.png)
2. Two's Complement(翻转后再加一)
    ![](CS61c/2.2.png)
3. Bias 编码 : 数字代表的值 = Signed + bias(例如-15)
 
## Lecture 3: Introduction to C 
#### Overview
C 编译器将 C代码转化为 依赖体系结构的 机器码
* Java 是转为不依赖体系结构的字节码
* Scheme 环境直接翻译代码
* 这在程序变成机器指令的时候会有区别
* 对C语言来说, 编译是四个过程, 预处理,编译,链接,汇编转机器码.

编译的优势:
* 极大提高了运行时性能
* 编译时间能接受(增强的编译过程,如使用 Makefile, 允许我们只需要重新编译修改过的文件)

编译的缺点:
* 所有的compiled files 都是体系结构相关的, 依赖于 CPU 类型和 操作系统
* 可执行文件在新系统中一定要重新编译
* 修改 -> 编译 -> 运行 的迭代过程有点慢

#### C 语法
main 的两个参数, argc 是参数的个数, 每个参数都放在 一个 pointer to array , `char *argv[]`中,其中 `argv[0]` 就是程序名(方便**在错误信息中显示**).<br>
变量声明但是没有初始化, 那么**其中放的数据就是垃圾**. C 中的内存乐意理解成一个非常大的 array, 其中每个 Cell 都关联有一个地址, cell 中同样可以存放数据. <br>
Java 和 C 都是使用 value 来传递参数, 过程/函数/方法 得到参数的一个副本, 所有改变这个副本并不影响 the original.