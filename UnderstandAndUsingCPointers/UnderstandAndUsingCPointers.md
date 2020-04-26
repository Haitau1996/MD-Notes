# Unstanding and Using C Pointers

## Introduction

一个重要的问题是程序运行过程中变量的**scope**
![scope of variable](figure/1-1.png)<br>
因此，使用指针的时候常常出现下列问题：<br>
* Accessing arrays and other data structures beyond their bounds.
* Referencing automatic variables after they have gone out of existence.
* Referencing heap allocated memory after it has been released.
* Dereferencing a pointer before memory has been allocated to it.
<font color=red> 使用dynamic 分配内存，意味着这些部分将由自己管理分配，使用完没有释放的情况下就会出现内存泄漏。</font>

总之，问题分成两类，释放了内存之后依然用指针引用，还没分配内存就使用。<br>
**如何阅读声明** 从后往前读，如`const int *pci` pci is a pointer to const integer,`const int* const pci` cpci is a const pointer to const integer.<br>
**virtual memory** The addresses return by a program running in a virtual operation system, the operation system maps the virtual address to a real physical memory address when needed.<br>
**concent of Null** null在使用的时候有多种意思，具体要结合语境，因此特别容易搞混淆。NULL分配给了一个指针，就是说明该指针不含任何东西。NULL concept是对null指针的抽象，两个指向不同类型的空指针应该是相等的。在语言中是使用宏来实现
```c
#define NULL (void*(0))

```
同时我们要知道，空指针和未初始化的指针不是一回事，前者不指向任何地方，后者有可能指向任何地方。<br>
<font color=red> 有一个非常不安全的事情，就是空指针可以作为int被强制类型转化为0</font>,如作为函数的输入,在c++中有一个类型安全的空指针nullptr。<br>
**Pointer to void** 其中的两个特性：<br>
* 使用pointer to char来实现，在内存分配之类的上
* 无法==其他的pointer，特例是两个NULL的void pointer是相等的

使用pointer to void时候需要注意，如一个int point 被 cast 成 void，再可以cast成其他类型，这是非常危险的。<br>
**全局和静态pointer** 
```c
int *globalpi;
void foo() {
 static int *staticpi;
 ...
}
int main() {
 ...
}
```
其中，可以发现他们的存储结构可以认为是没有从堆栈中要，是全局的。
![global pointer](./figure/1-2.png)<br>

