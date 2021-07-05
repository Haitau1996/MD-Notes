# [Algotrithms OCW](https://cuvids.io/app/course/2/) 
- [Algotrithms OCW](#algotrithms-ocw)
- [Part I](#part-i)
  - [cource Overview](#cource-overview)
  - [动态链接问题](#动态链接问题)
    - [Quick Find [eager approach]](#quick-find-eager-approach)
    - [Quick Union[lazy approach]](#quick-unionlazy-approach)
    - [improvment](#improvment)
  - [Analysis of Algorithms](#analysis-of-algorithms)
    - [Obersvation](#obersvation)
    - [mathematical models](#mathematical-models)
    - [order-of-growth classifications](#order-of-growth-classifications)
    - [theory of algorithms](#theory-of-algorithms)
    - [内存使用的定量分析](#内存使用的定量分析)
  - [Stacks and queues](#stacks-and-queues)
    - [Stacks](#stacks)
      - [链表实现](#链表实现)
      - [array implementation](#array-implementation)
    - [Array resize](#array-resize)
    - [Linkde-List Queue](#linkde-list-queue)
    - [泛型](#泛型)
    - [迭代器](#迭代器)
    - [applications](#applications)
  - [Sorting](#sorting)
    - [Selection Sort](#selection-sort)
    - [Insertion Sort](#insertion-sort)
    - [Shell Sort](#shell-sort)
    - [Shuffle](#shuffle)
    - [Application:Convex Hull](#applicationconvex-hull)
  - [Merge Sort](#merge-sort)
    - [Intro](#intro)
    - [bottom-up mergesort](#bottom-up-mergesort)
    - [sorting complexity](#sorting-complexity)
    - [comparators](#comparators)
    - [稳定性](#稳定性)
  - [Quick Sort](#quick-sort)
    - [selection](#selection)
    - [duplicate keys](#duplicate-keys)
  - [优先队列](#优先队列)
    - [_API and elementary implementations_](#api-and-elementary-implementations)
    - [_Binary Heap_](#binary-heap)
    - [Heap Sort](#heap-sort)
    - [event-driven simulation](#event-driven-simulation)
  - [符号表](#符号表)
    - [API](#api)
    - [elementary implementations](#elementary-implementations)
      - [简单链表实现](#简单链表实现)
      - [ordered array](#ordered-array)
      - [ordered operations](#ordered-operations)
    - [BSTs](#bsts)
      - [ordered operations](#ordered-operations-1)
      - [迭代](#迭代)
      - [deletion](#deletion)
        - [Deleting the minimum](#deleting-the-minimum)
        - [Hibbard deletion](#hibbard-deletion)
# Part I

## cource Overview 

What is this cource?  
**Algorithm** : method for solving a problem  
**Data Structure** ： methods to store information 
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210630184506.png"/></div>

## 动态链接问题
for a given set of N-Objects,支持以下的操作  
* Union command
* find query

Connect 具备的性质  
* Reflexive(**自反性**): p is connected to p.
* Symmetric(**对称性**):if p is connected to q, then q is connected to p
* Transitive(**传递性**): if p is connected to q and q is connected to r,then p is connected to r

我们设计的类需要支持下

面的接口:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701164312.png"/></div>

### Quick Find [eager approach]
具体实现的示意图如下： 
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701141351.png"/></div>  

:arrow_forward: 使用一个size为n的array，id[]  
:arrow_forward: 如果两者id一样的话，那么就认为是connected

优缺点： find query非常快，但是 ==union需要消耗太多资源==, 如果对N个对象做 N 次 Union, 那么就需要 $N^2$ 次操作<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701161948.png"/></div>

具体实现过程中需要注意的地方：
* 初始构造函数需要给id[i]赋值为i
* union时候需要遍历整个array.

### Quick Union[lazy approach]
具体的数据结构：
- 使用一个size为n的array
- interpretation: `id[i]` 为 i 的parent, root of i is `id[id[...]]`
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701162342.png"/></div>

使用的方法：
- find：只要检测是否为同一个 root
- union: 只要p和q之间 merge 即可

缺点： tree可能会变得非常长，则对大规模的问题无法使用。提升的方法：
- weighting : 把small 的 tree放在下面，因此没有item距离root比较远。
- Path Compression: 加上weighting, 将时间减到`O(n+lg*M)`

### improvment
* weighting
  * 记录每棵树的 size(元素数量), 这需要一个新的 array, `sz[]`,用于记录
  * 每次 union 时将小的树挂在大树的树根下
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701163049.png"/></div>

* path compression
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701163710.png"/></div>

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701163833.png"/></div>

`lg* N` 意味着要做多少次 lg 才能到 1, 如 lg* 2 = 1, lg* 4 = 2, lg* 16 = 3... 它一般是小于 5 的($N = 2^{65536}$), 因此实际上可以理解线性算法.
## Analysis of Algorithms

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701214743.png"/></div>
一般而言,有四个角度去理解程序,作为学生,我们必须全部了解他们.为何去分析算法:

* 预测 performance
* 比较算法
* 对程序运行提供一些保证

在算法理论中可能需要 Understand theoretical basis. 我们用这样的方法研究计算机程序的运行时间:
* 细致地观察真实世界的特点，通常还要有精确的测量
* 根据观察结果提出假设模型
* 根据模型预测未来的事件
* 继续观察并核实预测的准确性
* 如此反复直到确认预测和观察一致

### Obersvation
在计算机中有一个 3-Sum 问题, 给定 N 个整数, 数出和为 0 的 triples 个数,最直观的解法就是暴力枚举:
```C++
public static int count(int[] a)
{
    int N = a.length;
    int count = 0;
    for (int i = 0; i < N; i++)
        for (int j = i+1; j < N; j++)
            for (int k = j+1; k < N; k++)
                if (a[i] + a[j] + a[k] == 0)
                    count++;
    return count;
}
```
运行时间在 log-log plot 中是一条斜线, 斜率大概为 3.  
### mathematical models
**cost of basic opeartion**: 大多数的 ==primitive operations== 只需要常量时间.
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701221332.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701221526.png"/></div>

很多时候可以把离散的求和近似成连续的积分,去评估一个算法的复杂度:  
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701222039.png"/></div>  

### order-of-growth classifications
**常见算法的复杂度**:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701222407.png"/></div>

**3-Sum 的 $N^2 \log N$ 解法**

* sort the N numbers ($N^2$ with insertion sort)
* for each pair of numbers a[i] and a[j] **binary search** for -(a[i] + a[j]) ($N^2 \log N$)
    ```Java
    public static int binarySearch(int[] a, int key)
    {
        int lo = 0, hi = a.length-1;
        while (lo <= hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (key < a[mid]) hi = mid - 1;
            else if (key > a[mid]) lo = mid + 1;
            else return mid;
        }
        return -1;
    }
    ```
### theory of algorithms
在涉及对算法的评估时, 我们往往会采取下面三个角度:
* Best case: Lower bound on cost.
* Worst case: Upper bound on cost.
* Average case: Expected cost for random input <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701223900.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701224409.png"/></div>
  
在实际上, 人们常常先给出一个实现, 确定算法的上届. 然后尝试给出一个证明, 确定算法的下界. 不断减小其中的 Gap, 即便如此, 很多问题都无法得到一个 Optimal algorithm(上下界相同).  
### 内存使用的定量分析
过去的 32 位系统指针大小是 4 Byte, 64 位系统的指针是 8 Byte, 在过渡期间这个问题比较痛苦.
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701224809.png"/></div>

此外, 还需要注意三点:
* Object overhead. 16 bytes.
* Reference: 8 Bytes
* 内存对齐: Each object uses a multiple of 8 bytes<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210701225057.png"/></div>


## Stacks and queues

面向对象编程的基本思想，分离interface和implementation.  
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702091457.png"/></div>

### Stacks
Stack 需要实现下面的接口:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702091631.png"/></div>

#### 链表实现
```Java
public class LinkedStackOfStrings
{
    private Node first = null;
    private class Node
    {
        String item;
        Node next;
    }
    public boolean isEmpty()
    { return first == null; }

    public void push(String item)
    {
        Node oldfirst = first;
        first = new Node();
        first.item = item;
        first.next = oldfirst;
    }

    public String pop()
    {
        String item = first.item;
        first = first.next;
        return item;
    }
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702092340.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702092255.png"/></div>

不需要构造函数， 然后有一个指向node的reference,初始值为null.分析它的performance:

- 最坏的情况下每个operation都要const time
- N个Item需要约 ~40N 的空间(不包括reference指向的string)

#### array implementation

用array实现，我们使用一个容量为 N 的 array, `s[]`, 存放整个 stack. 添加元素的时候在`s[N]`处, 删除时在 `s[N-1]` 处. 总是会遇到容量的问题，这个实现需要构造函数，同时给capacity(breaked the API),
```Java
public class FixedCapacityStackOfStrings
{
    private String[] s;
    private int N = 0;
    public FixedCapacityStackOfStrings(int capacity)
    { s = new String[capacity]; }

    public boolean isEmpty()
    { return N == 0; }

    public void push(String item)
    { s[N++] = item; }

    public String pop()
    { return s[--N]; }
}
```
这个实现需要注意几个小的问题:
- Overflow and Underflow: 使用resize解决这个问题
- Loitering(虚度\游荡): pop之后将最后元素设置为null,让Java回收它
    ```Java
    public String pop()
    {
        String item = s[--N];
        s[N] = null;
        return item;
    }
    ```

### Array resize
**repreted doubling** 如果array full, 创建new array of twice the size, 然后复制元素. 这样的话Insert N个元素的时间消耗为N+(2+4+8+...+N)~3N.  
```Java
public ResizingArrayStackOfStrings()
{ s = new String[1]; }

public void push(String item)
{
    if (N == s.length) resize(2 * s.length);
    s[N++] = item;
}
private void resize(int capacity)
{
    String[] copy = new String[capacity];
    for (int i = 0; i < N; i++)
    copy[i] = s[i];
    s = copy;
}
```
在1/4的size时候,将容量变成现在的1/2(new新的再copy).性能分析:
- 内存的使用 ~8N 到 ~32N 之间(不包含string)
- worst 下 push/pop 需要O(N)的时间,best 和摊销下都是 O(1)
    ```Java
    public String pop()
    {
        String item = s[--N];
        s[N] = null;
        if (N > 0 && N == s.length/4) resize(s.length/2);
        return item;
    }
    ```

### Linkde-List Queue
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702095106.png"/></div>
这个时候我们需要 Maintain 两个 pointer,分别对应 first 和 Last Node.
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702095205.png"/></div>

```Java
public class LinkedQueueOfStrings
{
    private Node first, last;
    private class Node
    { /* same as in LinkedStackOfStrings */ }
    public boolean isEmpty()
    { return first == null; }

    public void enqueue(String item)
    {
        Node oldlast = last;
        last = new Node();
        last.item = item;
        last.next = null;
        if (isEmpty()) first = last;
        else oldlast.next = last;
    }

    public String dequeue()
    {
        String item = first.item;
        first = first.next;
        if (isEmpty()) last = null;
        return item;
    }
}
```
### 泛型
在实际的应用场景中, 我们通常需要装有不同对象类型的容器, 如有了 StackOfStrings, 还想要 StackOfURLs, StackOfInts,过去的做法, 使用面向对象的多态:
- [x] 需要在客户端的 casting
- [x] 如果出现 mismatch, 出现的是 run-time error
```Java
StackOfObjects s = new StackOfObjects();
Apple a = new Apple();
Orange b = new Orange();
s.push(a);
s.push(b);
a = (Apple) (s.pop());
```

新的解决方案就是 **java generics**,可以避免 client 端的 casting,同时在 compile-time 发现 type dismatch errors.  
```Java
Stack<Apple> s = new Stack<Apple>();
Apple a = new Apple();
Orange b = new Orange();
s.push(a);
s.push(b); // Compile-time Errors: type dismatch
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702095947.png"/></div>

注: java 不允许Generic的array, 只能是用casting将Object的转为Item[ ]: <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702100106.png"/></div>

```Java
S = (Item[]) new Object[capacity];
```

### 迭代器
在 Java 中实现 iteration over some container, 只需要让这个 container 实现 `java.lang.Iterable` 接口. 实现之后就可以有非常 elegant 的客户端代码:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702105313.png"/></div> 

```Java
import java.util.Iterator;
public class Stack<Item> implements Iterable<Item>
{
    ...
    public Iterator<Item> iterator() { return new ListIterator(); }
    private class ListIterator implements Iterator<Item>
    {
        private Node current = first;
        public boolean hasNext() { return current != null; }
        public void remove() { /* not supported */ }
        public Item next()
        {
            Item item = current.item;
            current = current.next;
            return item;
        }
    }
}
```
背包是一种不支持从中删除元素的集合数据类型 —— 它的目的就是帮助用例收集元素并迭代遍历所有收集到的元素（用例也可以检查背包是否为空或者获取背包中元素的数量）。迭代的顺序不确定且与用例无关。
### applications
编译器就是使用 stack 来支持函数调用:
* Function call: push local environment and return address
* Return: pop return address and local environment

## Sorting
用define的key将数据排成一个有序的,data可能是 _Double_, _String_ 或者 _java.io.File_ , 具体的做法是实现 _Callbacks_(**reference to executable code**), 在不同的语言中具体的实现可能不同:
* C: function pointers
* C++ : class-type functions
* Java : interfaces  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702152246.png"/></div>

对于传入的数据类型, 它必须通过 `compareTo()` 方法实现一个全序关系:
* 自反性，对于所有的 v，v=v；
* 反对称性，对于所有的 `v<w` 都有`v>w`，且v=w 时w=v；
* 传递性，对于所有的v、w和x，如果 `v<=w` 且 `w<=x` ，则 `v<=x`。

```Java
public class Date implements Comparable<Data>{ //尖括号说明只允许和Data对比
    private final int month, day, year;
    Public Data(int m, int d, int y){
        month = m;
        day = d;
        year = y;
    }
    // 实现 compareTo method
    public int compareTo(Data that){
        if(this.year < that.year) return -1;
        if(this.year > that.year) return +1;
        if(this.month < that.month) return -1;
        if(this.month > that.month) return +1;
        if(this.day < that.day) return -1;
        if(this.day > that.day) return +1;
        return 0;
    }
}
```

### Selection Sort
选择排序的算法描述:
* 在第i次迭代( i 从 0 开始)中, 找到剩余部分(从 _a[i]_ 到 _a[n-1]_ )最小元素的下标 _min_
* Swap _a[i]_ 和 _a[min]_

它有两个不变量:
* index i(包括 i) 的左边已经排好序
* index i 右边的元素没有排序, 但是可以确定左边的值不大于右边任何一个<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702154842.png"/></div>
    ```Java
    ...
    private static void exch(Comparable[] a; int i,int j){
        Comparable swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }
    public static void selection_sort(Comparable[] a){
        int N = a.length;
        for(int i =0 ; i < N; i++){
            int min  = i;
            for(int j = i+1; j< N; j++){ 
                if(less(a[j],a[min]))
                    min = j;
            }
            exch(a,i,min);
        }
    }
    ...
    ```

### Insertion Sort
前面部分是有序的, 每次迭代中将后面的第一个元素(下标为i)插入前面有序的部分, 和交换排序略微有点区别, 它也有两个不变量:
* 下标 i 的左边部分(包括 i) 是已经排好的
* 下标 i 的右边部分没有被看见<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702154328.png"/></div> 
```Java
public class Insertion
{
    public static void sort(Comparable[] a)
    {
        int N = a.length;
        for (int i = 0; i < N; i++)
            for (int j = i; j > 0; j--)
                if (less(a[j], a[j-1]))
                    exch(a, j, j-1);
                else break;
    }
    ...
}
```
这种算法需要 $\sim \frac{1}{4} N^2$ 次比较和交换on average,在最坏的情况下是$\frac{1}{2} N^2$. 它在 partially-sorted arrays 上有比较好的性能.

### Shell Sort
insertion sort 每次只移动一个位置, 翻转一个逆序对. 一个提升思路是每次都 move 多个位置, 这样的话一次可能就不止改变一个逆序对:
* 间隔非常大的时候, 是处理一个小的subarray
* 间隔不大的时候, 实际上array基本已经有排好序了
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702155528.png"/></div>

间隔的序列
* Powers of two: 这个效率非常差, 因为奇数和偶数的之间的交换只有在最后一个1-sort的时候才会发生
* Powers if two minus one : 可能可以工作
* $3x+1$: 简单实现, 效果还行
* merging of $(9\times 4^i) - (9 \times 2^i) + 1$ 和 $4^i - (3 \times 2^i)+1$
    ```Java
    public class Shell
    {
        public static void sort(Comparable[] a)
        {
            int N = a.length;
            int h = 1;
            while (h < N/3) h = 3*h + 1; // 1, 4, 13, 40, 121, 364, ...
            while (h >= 1)
            { // h-sort the array.
                for (int i = h; i < N; i++)
                {
                    for (int j = i; j >= h && less(a[j], a[j-h]); j -= h)
                        exch(a, j, j-h);
                }
                h = h/3;
            }
        }
        ...
    }
    ```
实际上如果使用  3x+1 的序列, 最坏情况下的对比次数是  $N^{\frac{3}{2}}$.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702160642.png"/></div>

### Shuffle
Knuth Shuffle:
* 在第 i 次迭代中, 从 0 到 i 中选取随机数 r
* Swap _a[i]_ 和 _a[r]_

### Application:Convex Hull
* 可以通过逆时针的方式完成一个 凸包的遍历, 
* 而该凸包上的点和 p(y 坐标最小) 的连线和 y 轴的夹角逐渐变大. 

这样的话, 我们就可以从简单的 fact 将一个问题转化为排序问题.
* 找到 Y 坐标最小的值/然后定义夹角再排序
* 怎样定义 counter clock wise turn: 使用向量的叉积

## Merge Sort
### Intro
* 将 array 分成两半
* **递归地** 将每个 half 排序
* 将两个 halves 合并

先看更简单的 merge 过程, 只需要先将原来的数组拷贝到 auxiliary 数组 `aux[]` 中, 然后维护三个 index, 根据比较结果赋值和挪动指针:
```Java
private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi)
{
    assert isSorted(a, lo, mid);    // Pre-Condition: a[lo .. mid] sorted
    assert isSorted(a, mid+1, hi);  // as above

    for (int k = lo; k <= hi; k++)
        aux[k] = a[k];
    int i = lo, j = mid+1;
    for (int k = lo; k <= hi; k++)
    {
        if (i > mid) a[k] = aux[j++];    // 小的那个 half 已经挪完了
        else if (j > hi) a[k] = aux[i++];// 大的那个 Half 已经挪完了
        else if (less(aux[j], aux[i])) a[k] = aux[j++];
        else a[k] = aux[i++];
    }

    assert isSorted(a, lo, hi);
}
```
在Java中可以增加很多断言, 可以**帮助检测程序的逻辑错误, 同时更有利于文档的编写**.有了 merge 之后, 它可以应付最基本的情况(只有两个元素), 于是 sort 的实现就变得十分简单:
```Java
private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi)
{
    if (hi <= lo) return;
    int mid = lo + (hi - lo) / 2;
    sort(a, aux, lo, mid);
    sort(a, aux, mid+1, hi);
    merge(a, aux, lo, mid, hi);
}
public static void sort(Comparable[] a)
{
    Comparable[] aux = new Comparable[a.length];// 在此写出辅助 array, 可以减少很多内存使用
    sort(a, aux, 0, a.length - 1);
}
```
If $D (N)$ satisfies $D (N) = 2 D (N / 2) + N$ for $N > 1$, with $D (1) = 0$,then $D (N) = N \log N$.  
Merge sort最多使用 $N \log N$ 次 compare 和 $6 N \log N$次 array acesses 去给size为N的array排序.  
此外,还有几个小的提升方法:
* Merge sort对于小的subarray来说效率并不高,可以 **在 ~7 个item左右的时候换成insertion sort**.
* 在两个 subarray 已经排好序的时候, 跳过 merge 过程
    ```Java
    ...
    if (!less(a[mid+1], a[mid])) return;
    merge(a, aux, lo, mid, hi);
    ...
    ```
* 在 `aux[]` 和 `a[]` 之间来回倒腾, 省去元素拷贝的时间

### bottom-up mergesort
自底向上的 mergesort 丢弃了递归, 而是使用每次对指数递增的 subarray 做 merge:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702211821.png"/></div>

```Java
public static void sort(Comparable[] a)
{
    int N = a.length;
    Comparable[] aux = new Comparable[N];
    for (int sz = 1; sz < N; sz = sz+sz)
        for (int lo = 0; lo < N-sz; lo += sz+sz)
            merge(a, aux, lo, lo+sz-1, Math.min(lo+sz+sz-1, N-1));
}
```

### sorting complexity
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702212217.png"/></div>

Proposition: Any compare-based sorting algorithm must use at least $\log( N ! ) \sim N \log N$ compares in the worst-case. 证明如下:
* 假设有一个包含 $a_1$ 到 $a_N$ N个不同值的 array
* 最坏的情况下会有 高度为 h 的决策树
* 高度为 h 的二叉树最多有 $2^h$ 个树叶
* 一共有 $N!$ 种排列 $\Rightarrow$ 至少要有 $N!$ 个树叶

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702212947.png"/></div>

因此, Optimal algorithm = mergesort.

### comparators
Comparator interface: sort using an **alternate order**.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702213337.png"/></div>

我们在之前的实现中, 使用的是默认的 `compareTo()` 方法, 实际上, `sort` 可以定义第二个参数:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210702213433.png"/></div>

```Java
public static void sort(Object[] a, Comparator comparator)
{
    int N = a.length;
    for (int i = 0; i < N; i++)
        for (int j = i; j > 0 && less(comparator, a[j], a[j-1]); j--)
            exch(a, j, j-1);
}
private static boolean less(Comparator c, Object v, Object w)
{ return c.compare(v, w) < 0; }
private static void exch(Object[] a, int i, int j)
{ Object swap = a[i]; a[i] = a[j]; a[j] = swap; }
```
除了上述对于 sort 的修改, 我们还需要实现一个 Comparator:
```Java
public class Student
{
    private final String name;
    private final int section;
    ...
    public static class ByName implements Comparator<Student>
    {
        public int compare(Student v, Student w)
        { return v.name.compareTo(w.name); }
    }
    public static class BySection implements Comparator<Student>
    {
        public int compare(Student v, Student w)
        { return v.section - w.section; }
    }
}
```

### 稳定性
A stable sort **preserves the relative order of items with equal keys**.
* Insertion sort is stable: 我们没有在插入的时候让新的 item 插入到相等的 item 之前
* Selection sort is not stable: 可能在某个 long distance exchange 中将某个不大的值放到很后面
* Shellsort sort is not stable: 同样的有Long distance exchange
* Mergesort is stable: 取决于 merge 过程是否稳定, 良好的 merge 实现是稳定的(equal key 时候从左边的 array 选择)

## Quick Sort
Quick Sort 也是一种递归的算法, 它和 merge sort 的一个很大的区别在于, Quick 是在确定完位置之后再递归, Merge 是递归之后再 Merge(确定位置):
1. Shuffle the array 
2. pertition the array, 对于一个某个 index j:
   1. Entry `a[j]` is in place
   2. No larger entry to the left of j & No smaller entry to the right of j
3. Sort each subarray recursively <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704194944.png"/></div>

置换位置的过程, 我们首先设置头和尾指针, 然后向中间挪动, 两个指针发现合适的元素 i 和 j 的时候, 交换 `a[i]` 和 `a[j]`, 如果两个指针越界的, 就交换 `a[lo]` 和 `a[j]`:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704195459.png"/></div>

```Java
private static int partition(Comparable[] a, int lo, int hi)
{
    int i = lo, j = hi+1;
    while (true)
    {
        while (less(a[++i], a[lo])) // 找到左边需要 swap 的元素
            if (i == hi) break;
        while (less(a[lo], a[--j])) // 找到右边需要交换的元素
            if (j == lo) break;
        if (i >= j) break;          // 看指针是否越界
        exch(a, i, j);
    }
    exch(a, lo, j);                 // 把标兵和交界处的元素互换
    return j;
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704195825.png"/></div>

_partition_ 这个最重要的过程实现之后, 我们就可以使用递归的方式去实现整个 快排:
```Java
public class Quick
{
    private static int partition(Comparable[] a, int lo, int hi)
    { /* as above */ }
    public static void sort(Comparable[] a)
    {
        StdRandom.shuffle(a);
        sort(a, 0, a.length - 1);
    }
    private static void sort(Comparable[] a, int lo, int hi)
    {
        if (hi <= lo) return;
        int j = partition(a, lo, hi);
        sort(a, lo, j-1);
        sort(a, j+1, hi);
    }
}
```
实现的细节上, 有下面几点需要注意:
1. 这个分割的过程是 in-place 的
2. 循环的终止条件需要我们仔细考虑
3. 处理重复的元素有点麻烦
4. 我们要使用随机洗牌提供性能保证
   * 在 subarray 中使用随机方式选择标兵可以实现相同的效果
   
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704200703.png"/></div>

从概率的分析看, 比较的期望是 $\sim 2n\log n$, 交换的期望是$\sim \frac{1}{3}n\log n$:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704201119.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704201146.png"/></div>

性能总结:
1. Quick Sort 谁一种随机化的算法
2. 平均情况下比 Merge Sort 多 39% 的比较, 但是只需要更少的 data movement, 因此比 merge sort 要快一些
3. 其他的情况
   1. 最好的情况下需要 $n\log n$ 次比较
   2. 最坏的情况下需要 $\sim \frac{1}{2}n^2$ 次比较
4. Quicksort is not stable

小的性能提升方法, 和 Merge sort 类似
1. 在 $\sim 10$ 个元素的时候使用插入排序
2. 取 3 个随机元素的中位数当标兵

### selection
目标: Given an array of n items, find the $k^{th}$ smallest item.  
我们思考后可以得到下面的结论:
1. 上界就是$n\log n$: 最坏的情况下排序后就能找到第 n 个
2. 对于 k = 1, 2, 3, 扫描一遍就能得到
3. 对于其他情况, 也是 n 为下界: 必须每个元素都看到

Quick-select:  
* 我们每次用快排中的分割方法, 确定 a[j];
* j 比 k 小就在右边接着分,  否则就在左边分, 相等的话就找到了
    ```Java
    public static Comparable select(Comparable[] a, int k)
    {
        StdRandom.shuffle(a);
        int lo = 0, hi = a.length - 1;
        while (hi > lo)
        {
            int j = partition(a, lo, hi);
            if (j < k) lo = j + 1;
            else if (j > k) hi = j - 1;
            else return a[k];
        }
        return a[k];
    }
    ```

这个算法的复杂度是显性的, $n + n / 2 + n / 4 + … + 1 \sim 2n$.更复杂的分析如下:
\[\begin{aligned}
    C_{n}   & \sim 2 n+2 k \ln (n / k)+2(n-k) \ln (n /(n-k)) \\
            & \leq(2+2 \ln 2) n\\
            & \approx 3.38n
\end{aligned}
\]

### duplicate keys
对于重复元素非常多的情况下, 如果每次都将重复的元素放在标兵的一侧, 那么使用的时候就会发现很容易出现复杂度 $\sim \frac{1}{2}n^2$ 的情况. 解决的一个思路被称为 **3-way partitioning**: <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704211726.png"/></div>

其具体的做法如下:
* Let v be partitioning item `a[lo]`
* 将 i 从左到右扫描
  * (`a[i]` < v): exchange `a[lt]` with `a[i]`; increment both `lt` and `i`
  * (`a[i]` > v): exchange `a[gt]` with `a[i]`; decrement `gt`
  * (`a[i]` == v): increment i
    ```Java
    private static void sort(Comparable[] a, int lo, int hi)
    {
        if (hi <= lo) return;
        int lt = lo, gt = hi;
        Comparable v = a[lo];
        int i = lo;
        while (i <= gt)
        {
            int cmp = a[i].compareTo(v);
            if (cmp < 0) exch(a, lt++, i++);
            else if (cmp > 0) exch(a, i, gt--);
            else i++;
        }
        sort(a, lo, lt - 1);
        sort(a, gt + 1, hi);
    }
    ```
    <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704212402.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704212707.png"/></div>

## 优先队列
### _API and elementary implementations_
对于 Java 中的不同 collection, 其删除操作所删除的对象各有不同:
* _Stack_: Remove the item ==most recently added==
* _Queue_: Remove the item ==least recently added==
* _Randomized queue_: Remove a random item
* _Priority queue_: Remove the ==largest (or smallest) item==

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704214710.png"/></div>

其使用接口的方式如下:
```Java
MinPQ<Transaction> pq = new MinPQ<Transaction>();
while (StdIn.hasNextLine())
{
    String line = StdIn.readLine();
    Transaction transaction = new Transaction(line);
    pq.insert(transaction);
    if (pq.size() > m)
        pq.delMin();
}
```
我们可以用array(unordered) 和 linked-list(ordered)实现:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704215343.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210704215506.png"/></div>

### _Binary Heap_
Binary heap 是一个 heap-ordered 完全二叉树的数组实现, 它要求:
* parent's key 不小于 children's key
* 下标从 1 开始
* 按照 level 顺序接受 nodes
* 不需要显式的链接

这时候, 数组元素的位置就可以确定互相间的关系
* k元素的父节点就是 [k/2]
* k的子节点就是 2k 和 2k+1

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705091955.png"/></div>

接下来看各种相关的操作是如何实现:
1. swim: 当一个 node 的 key 比它的 parent's key 大的时候, 只需要将 child 和 parent 对换, 直到依旧满足 heap order
    ```Java
    private void swim(int k)
    {
        while (k > 1 && less(k/2, k))
        {
            exch(k, k/2);
            k = k/2;
        }
    }
    ```
2. 添加新元素的时候, 只需要将 node 放在最后, 然后 swip it up, 最多需要 $1 + \lg n$ 次对比 
    ```Java
    public void insert(Key x)
    {
        pq[++n] = x;
        swim(n);
    }
    ```
2. sink:一个元素的 key 比它的 children 小, 需要做的就是比较两个 children 的 key, 再和大大那个交换, 直到满足条件
    ```Java
    private void sink(int k)
    {
        while (2*k <= n)
        {
            int j = 2*k;
            if (j < n && less(j, j+1)) j++;
            if (!less(k, j)) break;
            exch(k, j);
            k = j;
        }
    }
    ```
4. 删除最大值: 将 root 和 end 的节点交换, 删除最后一个节点(原来的 root), 然后将新 root 下沉
    ```Java
    public Key delMax()
    {
        Key max = pq[1];
        exch(1, n--);
        sink(1);
        pq[n+1] = null;
        return max;
    }
    ```
```Java
public class MaxPQ<Key extends Comparable<Key>>
{
    private Key[] pq;
    private int n;

    public MaxPQ(int capacity)
    { pq = (Key[]) new Comparable[capacity+1]; }

    public boolean isEmpty()
    { return n == 0; }

    public void insert(Key key) // see previous code
    public Key delMax() // see previous code
    private void swim(int k) // see previous code
    private void sink(int k) // see previous code

    private boolean less(int i, int j)
    { return pq[i].compareTo(pq[j]) < 0; }
    private void exch(int i, int j)
    { Key t = pq[i]; pq[i] = pq[j]; pq[j] = t; }
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705093713.png"/></div>

此外我们需要考虑一些东西:
* 下溢和上溢
  * Underflow: throw exception if deleting from empty PQ.
  * Overflow: add no-arg constructor and use resizing array.
* Minimum-oriented priority queue: 将 less 改成 greater,并且实现 greater
* key 的不变性: 假设客户端无法在使用 PQ 的时候修改key, 最佳实践就是使用 immutable keys, immutable 数据有下面的好处
  * Safe to use as key in priority queue or symbol table
  * Simplifies debugging
  * Simplifies concurrent programming.
  * More secure in presence of hostile code

### Heap Sort
Heap Sort 的基本思路如下:
* 首先将 input array 看成是一个完全的二叉树
* 然后使用 n 个key 构建一个 max-heap(using **bottom-up method**)<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705095259.png"/></div>
    ```Java
    for (int k = n/2; k >= 1; k--)
        sink(a, k, n);
    ```
* 最后重复地选出(并删除)最大元素
    ```Java
    while (n > 1)
    {
        exch(a, 1, n--);
        sink(a, 1, n);
    }
    ```

Heap construction makes ≤ n exchanges and ≤ 2n compares, uses ≤ $2n\lg n$ compares and exchanges.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705095710.png"/></div>

### event-driven simulation
Change state only when something interesting happens:
* Between collisions, particles move in straight-line trajectories.
* Focus only on times when collisions occur.
* Maintain PQ of collision events, prioritized by time.
* Delete min = get next collision.

```Java
public class Particle
{
    private double rx, ry; // position
    private double vx, vy; // velocity
    private final double radius; // radius
    private final double mass; // mass
    private int count; // number of collisions

    public Particle( ... ) { ... }
    public void move(double dt) { ... }
    public void draw() { ... }
    // 下面就是预测是否和 particle / wall 碰撞
    public double timeToHit(Particle that) { }
    public double timeToHitVerticalWall() { }
    public double timeToHitHorizontalWall() { }
    // 碰撞之后的行为
    public void bounceOff(Particle that) { }
    public void bounceOffVerticalWall() { }
    public void bounceOffHorizontalWall() { }
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705100852.png"/></div>

## 符号表
### API
符号表是对 key-value 对的抽象.
* Insert a value with specified key.
* Given a key, search for the corresponding value.

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705101954.png"/></div>

下面是实现中的几个约定:
* Values are not `null`.
* Method `get()` returns null if key not present.
* Method `put()` overwrites old value with new value.

Key 和 value 的要求:  
* Value 类型: 任意的 generic 类型
* Key 类型则需要满足下面的假设<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705104400.png"/></div>
* 一个最佳实践是使用 immutable 类型作为符号表的 key

在Java 中的 Equality test(继承 method `equals()`), 对于自定义的类型, 要注意它的实现:
* Reflexive: `x.equals(x)` is true.
* Symmetric: `x.equals(y)` 当且仅当 `y.equals(x)`.
* Transitive: if `x.equals(y)` and `y.equals(z)`, then `x.equals(z)`.
* Non-null: `x.equals(null)` is false

在实现的时候往往会加上下面几行作为优化:
```Java
if (y == this) return true;
if (y == null) return false;
if (y.getClass() != this.getClass())
    return false;
```
* 对于 primitive type, 使用 `==`
* 对于对象, 使用 `equals()`
* 对于数组, 需要比较其中的每个元素 `Array.deepEquals(a,b)` 而不是 `q.equals(b)`

### elementary implementations
#### 简单链表实现
使用一个链表, 每个节点维护一个 key-value 对, 查找只需要对整个链表做扫描, 插入时同样, 如果发现了 key 的话就更新值, 没发现就插入到头节点. <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705105926.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705110100.png"/></div>

#### ordered array
使用一个有序的 array 存储 key-value 对.这时候我们可以使用二分法查找, 问题在于插入的时候需要挪动一大堆的元素. <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705110333.png"/></div>

#### ordered operations
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705110750.png"/></div>

### BSTs
A BST is a binary tree in **symmetric order**.
* 二叉树: 一个二叉树要么是空的, 要么只有两个不相交的子树
* Symmetric order: Each node has a key; a node’s key is both 
  * Larger than all keys in its left subtree.
  * Smaller than all keys in its right subtree<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705111332.png"/></div>

在 Java 中, 一个 BST 就是**对根节点的引用**, 每个节点都有四个 field:
* A key and a Value
* A reference to the left and the right subtree

```Java
private class Node
{
    private Key key;
    private Value val;
    private Node left, right;
    public Node(Key key, Value val)
    {
        this.key = key;
        this.val = val;
    }
}
public class BST<Key extends Comparable<Key>, Value>
{
    private Node root;
    private class Node
    { /* see previous slide */ }
    public void put(Key key, Value val)
    { /* see slide in this section */ }
    public Value get(Key key)
    { /* see next slide */ }
    public Iterable<Key> keys()
    { /* see slides in next section */ }
    public void delete(Key key)
    { /* see textbook */ }
}
```
Search/Insert 的实现十分简单
```Java
public Value get(Key key)
{
    Node x = root;
    while (x != null)
    {
        int cmp = key.compareTo(x.key);
        if (cmp < 0) x = x.left;
        else if (cmp > 0) x = x.right;
        else return x.val;
    }
    return null;
}

public void put(Key key, Value val)
{ root = put(root, key, val); }
private Node put(Node x, Key key, Value val)
{
    if (x == null) return new Node(key, val);
    int cmp = key.compareTo(x.key);
    if (cmp < 0) x.left = put(x.left, key, val);
    else if (cmp > 0) x.right = put(x.right, key, val);
    else /* if(cmp ==0)*/  x.val = val;
    return x;
}
```
二叉树的很多操作的复杂度都和其深度有关, 因此它的不同形状对性能的影响很大:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705112700.png"/></div>

如果 N 个不同 key 随机插入 BST, 预期 search/insert 操作的对比次 $\sim 2\ln N$(和 [quick sort](#selection) 很像).

#### ordered operations
* min/max 操作十分 trivial
* floor(Largest key ≤ a given key) 需要详细考虑不同的情况
  * Case 1. [k equals the key in the node]:The floor of k is k
  * Case 2. [k is less than the key in the node]: The floor of k is in the left subtree.
  * Case 3. [k is greater than the key in the node]: The floor of k is in the right subtree(if there is any key ≤ k in right subtree);
    ```Java
    public Key floor(Key key)
    {
        Node x = floor(root, key);
        if (x == null) return null;
        return x.key;
    }
    private Node floor(Node x, Key key)
    {
        if (x == null) return null;
        int cmp = key.compareTo(x.key);

        if (cmp == 0) return x;

        if (cmp < 0) return floor(x.left, key);
        Node t = floor(x.right, key);
        if (t != null) return t;
        else           return x;
    }
    ```
* rank(How many keys < k), 和 floor 类似, 需要考虑三种情况
    ```Java
    public int rank(Key key)
    { return rank(key, root); }
    private int rank(Key key, Node x)
    {
        if (x == null) return 0;
        int cmp = key.compareTo(x.key);
        if (cmp < 0) return rank(key, x.left);
        else if (cmp > 0) return 1 + size(x.left) + rank(key, x.right);
        else if (cmp == 0) return size(x.left);
    }
    ```

#### 迭代
迭代很多时候可以使用递归实现, 如果不用的话就可以使用一个队列作为辅助, 如对于 in-order travaesal:
1. Traverse left subtree
2. Enqueue key
3. Traverse right subtree
    ```Java
    public Iterable<Key> keys()
    {
        Queue<Key> q = new Queue<Key>();
        inorder(root, q);
        return q;
    }
    private void inorder(Node x, Queue<Key> q)
    {
        if (x == null) return;
        inorder(x.left, q);
        q.enqueue(x.key);
        inorder(x.right, q);
    }
    ```
#### deletion
##### Deleting the minimum
* Go left until finding a node with a null left link.
* Replace that node by its right link.
* Update subtree counts.

```Java
public void deleteMin()
{ root = deleteMin(root); }
private Node deleteMin(Node x)
{
    if (x.left == null) return x.right;
    x.left = deleteMin(x.left);
    x.count = 1 + size(x.left) + size(x.right);
    return x;
}
```
##### Hibbard deletion
首先我们找到 key 为 k 的元素
* case 0: 没有 child, 直接删除
* case 1: 只有一个 child, 直接用 child 替代
* case 2: 有两个 childern, 用右边的最小值替代当前位置, 并且删除最小值(同样的可以用左边最大值替代)
```Java
public void delete(Key key)
{ root = delete(root, key); }
private Node delete(Node x, Key key) {
    if (x == null) return null;
    int cmp = key.compareTo(x.key);
    // search for the key
    if (cmp < 0) x.left = delete(x.left, key);
    else if (cmp > 0) x.right = delete(x.right, key);
    else {
        if (x.right == null) return x.left; // no right child
        if (x.left == null) return x.right; // no left child 
        // replate with successor
        Node t = x;
        x = min(t.right);
        x.right = deleteMin(t.right);
        x.left = t.left;
    }
    //update subtree counts
    x.count = size(x.left) + size(x.right) + 1;
    return x;
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210705124139.png"/></div>

