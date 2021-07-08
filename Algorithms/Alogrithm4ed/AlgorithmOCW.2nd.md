<font size =10>[Algotrithms OCW Part II](https://cuvids.io/app/course/2/)</font>

- [无向图](#无向图)
  - [introduction](#introduction)
  - [graph API](#graph-api)
    - [Graph representation](#graph-representation)
    - [Set-of-edges graph representation](#set-of-edges-graph-representation)
    - [Adjacency-matrix graph representation](#adjacency-matrix-graph-representation)
    - [Adjacency-list graph representation](#adjacency-list-graph-representation)
  - [depth-first search](#depth-first-search)
## 无向图
### introduction
Graph. Set of <font color=blue>vertices</font>(顶点) connected pairwise by <font color=blue>edges</font>(边).  
* <font color=blue>Path</font>: Sequence of vertices connected by edges.
* <font color=blue>Cycle</font>: Path whose first and last vertices are the same<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210707093508.png"/></div>

### graph API
#### Graph representation
我们可以将图画出来, 直观地表示图的结构, 但是它**提供的 intuition 可能 misleading**, 同一个图可以画得差别很大.  
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210707094211.png"/></div>

#### Set-of-edges graph representation
这种表示形式, 维护一个由边构成的 list(可能是链表或者数组):<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210707094708.png"/></div>

#### Adjacency-matrix graph representation
**邻接矩阵**维护一个 V-By-V 二维 boolean 矩阵, 其中的每个元素, `adj[v][w] == adj[w][v]`.

#### Adjacency-list graph representation
**邻接表** Maintain vertex-indexed array of lists.
```Java
public class Graph
{
  private final int V;
  private Bag<Integer>[] adj;
  public Graph(int V)
  {
    this.V = V;
    adj = (Bag<Integer>[]) new Bag[V];
    for (int v = 0; v < V; v++)
    adj[v] = new Bag<Integer>();
  }
  public void addEdge(int v, int w)
  {
    adj[v].add(w);
    adj[w].add(v);
  }
  public Iterable<Integer> adj(int v)
  { return adj[v]; }
}
```
In practice. Use adjacency-lists representation.
* Algorithms based on iterating over vertices adjacent to v.
* Real-world graphs tend to be sparse.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210707095410.png"/></div>

### depth-first search
