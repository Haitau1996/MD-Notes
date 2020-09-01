# [Algotrithms(OCW)](https://cuvids.io/app/course/2/) 

## Part I

### cource Overview 

What is this cource?
**Algorithm** : method for solving a problem
**Data Structure** ： methods to store information 
![avatar](figure/1-1.PNG)

### 动态链接问题
for a given set of N-Objects,支持以下的操作<br>
* Union command
* find query

Connect 具备的性质<br>
* reflexive 
* transitive
* Symmetric

#### Quick Find
具体的数据结构： 
- [x] 使用一个size为n的array，id[]
- [x] 如果两者id一样的话，那么就认为是connected

优缺点： find query非常快，但是union需要消耗太多资源。<br>
具体实现过程中需要注意的地方：
* 初始构造函数需要给id[i]赋值为i
* union时候需要遍历整个array.

#### Quick Union
具体的数据结构：
- 使用一个size为n的array
- interpretation: id[i] 为 i 的parent, root of i is id[id[...]]

使用的方法：
- find：只要检测是否为同一个root
- union: 只要p和q之间merge即可

缺点： tree可能会变得非常长，则对大规模的问题无法使用。提升的方法：
- weighting : 把small 的 tree放在下面，因此没有item距离root比较远。
- Path Compression: 加上weighting, 将时间减到`O(n+lg*M)`

### Analysis of Algorithms

![status](figure/3-1.png)<br>
一般而言,有四个角度去理解程序,作为学生,我们必须全部了解他们.为何去分析算法:

* 预测performance
* 比较算法s
* 对程序运行提供一些保证

**cost of basic opeartion**  :<br>
![costs operation](figure/3-2.png)<br>
很多时候可以把离散的求和近似成连续的积分,去评估一个算法的复杂度:<br>
![descrete](figure/3-3.png)<br>
**常见算法的复杂度** :<br>
![growth](figure/3-4.png)<br>

**3-Sum 的 $N^2 \log N$ 解法**

* sort the N numbers ($N^2$ with insertion sort)
* for each pair of numbers a[i] and a[j] **binary search** for -(a[i] + a[j]) ($N^2 \log N$)

**内存使用的定量分析:**
![memory](figure/3-5.png)<br>

### Stacks and queues

面向对象编程的基本思想，分离interface和implementation.<br>
![seperate](figure/4-1.png)<br>

#### Linked Lists representation

![linklist stack](figure/4-2.png)<br>
不需要构造函数， 然后有一个指向node的reference,初始值为null.分析它的performance:

- 最坏的情况下每个operation都要const time
- N个Item需要约~40N的空间(不包括reference指向的string)

#### array implementation

用array实现，总是会遇到容量的问题，这个实现需要构造函数，同时给capacity(breaked the API),
![array stack](figure/4-3.png)<br>
这个实现需要注意几个小的问题:
- Overflow and Underflow: 使用resize解决这个问题
- Loitering(虚度\游荡): pop之后将最后元素设置为null,让Java回收它

##### Array resize
**repreted doubling** 如果array full, 创建new array of twice the size, 然后复制元素. 这样的话Insert N个元素的时间消耗为N+(2+4+8+...+N)~3N.<br>
在1/4的size时候,将容量变成现在的1/2(new新的再copy).性能分析:
- 内存的使用~8N到~32N之间(不包含string)
- worst下push/pop需要O(N)的时间,best和摊销下都是O(1)

#### Linkde-List Queue
这个时候我们需要Maintain两个pointer,分别对应first和Last Node.

#### 泛型
在实际的应用场景中, 我们通常需要装有不同对象类型的容器, 如有了StackOfStrings, 还想要StackOfURLs, StackOfInts,过去的做法, 使用面向对象的多态:
- [x] 需要在客户端的casting
- [x] 如果出现mismatch, 出现的是run-time error

新的解决方案就是**java generics**,可以避免client端的casting,同时在compile-time发现type dismatch errors.<br>
注: java 不允许Generic的array, 只能是用casting将Object的转为Item[ ]:<br>
```Java
S = (Item[]) new Object[capacity];
```

#### 迭代器
![Iterator](figure/4-4.png)<br>

### Sorting
用define的key将数据排成一个有序的,data可能是 _Double_, _String_ 或者 _java.io.File_ , 具体的做法是实现 _Callbacks_, 在不同的语言中具体的实现可能不同:
* C: function pointers
* C++ : class-type functions
* Java : interfaces<br>
![implementation](figure/5-1.png)
```Java
Public class Date implements Comparable<Data>{ //尖括号说明只允许和Data对比
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
* 在第i次迭代中, 找到剩余部分最小元素的下标 _min_
* Swap _a[i]_ 和 _a[min]_
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

# [Algorithms 4ed (book)](https://algs4.cs.princeton.edu/home/)