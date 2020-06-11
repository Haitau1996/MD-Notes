# Algotrithms

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

