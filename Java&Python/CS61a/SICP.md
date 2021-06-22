# [计算机程序的构造和解释](https://mitpress.mit.edu/sites/default/files/sicp/full-text/book/book-Z-H-4.html#%_toc_start)
# Chap 1: 抽象构造过程
## 程序设计的基本元素
语言都提供**能够将简单的认识组合起来形成更复杂认识的方法**, 为此提供了三种机制:
* 基本表达式(primitive expressions)，用于表示语言所关心的**最简单的个体**。
* 组合的方法(means of combination)，通过它们可以从较简单的东西出发**构造出复合的元素**。
* 抽象的方法(means of abstraction)，通过它们可以**为复合对象命名，并将它们当作单元去操作**。

我们需要处理两类要素：过程和数据（以后将会发现，它们实际上**并不是这样严格分离**）.
### Expressions
