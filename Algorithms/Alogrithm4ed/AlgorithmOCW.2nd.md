<font size =10>[Algotrithms OCW Part II](https://cuvids.io/app/course/2/)</font>

- [无向图](#无向图)
  - [UG:Intro](#ugintro)
  - [graph API](#graph-api)
    - [Graph representation](#graph-representation)
    - [Set-of-edges graph representation](#set-of-edges-graph-representation)
    - [Adjacency-matrix graph representation](#adjacency-matrix-graph-representation)
    - [Adjacency-list graph representation](#adjacency-list-graph-representation)
  - [depth-first search](#depth-first-search)
    - [Trémaux maze exploration](#trémaux-maze-exploration)
    - [深度优先查找](#深度优先查找)
    - [graph 处理的设计模式](#graph-处理的设计模式)
    - [DFS: 数据结构](#dfs-数据结构)
    - [DFS: 特性](#dfs-特性)
  - [breadth-first search](#breadth-first-search)
    - [BFS:特性](#bfs特性)
  - [connected components](#connected-components)
- [有向图](#有向图)
  - [DG:Intro](#dgintro)
  - [DG:API](#dgapi)
  - [DG:Search](#dgsearch)
    - [Depth-first search in digraphs](#depth-first-search-in-digraphs)
    - [Breadth-first search in digraphs](#breadth-first-search-in-digraphs)
  - [topological sort](#topological-sort)
    - [Precedence scheduling](#precedence-scheduling)
    - [Topological sort](#topological-sort-1)
  - [strong components](#strong-components)
    - [Kosaraju-Sharir algorithm](#kosaraju-sharir-algorithm)
- [最小生成树](#最小生成树)
  - [MST: Intro](#mst-intro)
  - [MST: 贪婪算法](#mst-贪婪算法)
  - [Weighted edge API](#weighted-edge-api)
  - [Kruskal's algorithm](#kruskals-algorithm)
  - [Prim's algorithm](#prims-algorithm)
    - [Indexed priority queue](#indexed-priority-queue)
- [最小路径(shortest paths)](#最小路径shortest-paths)
  - [SP:APIs](#spapis)
  - [最小路径的特性](#最小路径的特性)
    - [边的松弛](#边的松弛)
    - [通用算法](#通用算法)
  - [SP:Dijkstra's algorithm](#spdijkstras-algorithm)
  - [edge-weighted DAGs](#edge-weighted-dags)
  - [negative weights](#negative-weights)
    - [Bellman-Ford algorithm](#bellman-ford-algorithm)
    - [检查 negative cycle](#检查-negative-cycle)
- [最大流问题](#最大流问题)
  - [MF:Intro](#mfintro)
    - [Mincut Problem](#mincut-problem)
    - [Maxflow problem](#maxflow-problem)
  - [Ford-Fulkerson 算法](#ford-fulkerson-算法)
  - [maxflow-mincut theorem](#maxflow-mincut-theorem)
- [STRING SORTS](#string-sorts)
  - [String in Java](#string-in-java)
  - [Key-Index Sorting](#key-index-sorting)
  - [LSD(Least-significant-digit-first) radix sort](#lsdleast-significant-digit-first-radix-sort)
  - [MSD radix sort](#msd-radix-sort)
  - [3-way radix quicksort](#3-way-radix-quicksort)
  - [suffix arrays](#suffix-arrays)
- [单词查找树](#单词查找树)
  - [R-way tries](#r-way-tries)
    - [performance](#performance)
  - [ternary(三元的) search tries](#ternary三元的-search-tries)
  - [character-based operations](#character-based-operations)
    - [Patricia trie](#patricia-trie)
    - [Suffix tree](#suffix-tree)
- [子串查找](#子串查找)
  - [brute force](#brute-force)
    - [回退的问题](#回退的问题)
  - [Knuth-Morris-Pratt](#knuth-morris-pratt)
## 无向图
### UG:Intro
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
#### Trémaux maze exploration
* 展开一个 ball 串
* 记录每个已经访问过的叉路口和道路
* 如果没有路了就原路返回

这个算法最重要的是**不要重复路经同一个点**.

#### 深度优先查找
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709101217.png"/></div>

#### graph 处理的设计模式
**<font color=blue> Goal</font>**: 在 graph 数据和 graph processing 之间解耦:
* 创建一个 Graph 对象
* 将它传给一个 graph-processing 例程
* 向例程查询想要的数据<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709101557.png"/></div>

#### DFS: 数据结构
* Boolean array `marked[]` to mark visited vertices.
* Integer array `edgeTo[]` to keep track of paths.(`edgeTo[w] == v`) means that edge v-w taken to visit w for first time
* Function-call stack for recursion

```Java
public class DepthFirstPaths
{
  private boolean[] marked;
  private int[] edgeTo;
  private int s;
  public DepthFirstPaths(Graph G, int s)
  {
    //... 在这里初始数据结构
    dfs(G, s);
  }
  private void dfs(Graph G, int v)
  {
    marked[v] = true;
    for (int w : G.adj(v))
    if (!marked[w])
    {
      dfs(G, w);
      edgeTo[w] = v;
    }
  }
}
```
#### DFS: 特性
* DFS marks all vertices connected to s in time proportional to the sum of their degrees
* After DFS, can check if vertex v is connected to s in constant time and can find v–s path (if one exists) in time proportional to its length
    ```Java
    public boolean hasPathTo(int v)
    { return marked[v]; }
    public Iterable<Integer> pathTo(int v)
    {
      if (!hasPathTo(v)) return null;
      Stack<Integer> path = new Stack<Integer>();
      for (int x = v; x != s; x = edgeTo[x])
        path.push(x);
      path.push(s);
      return path;
    }
    ```
### breadth-first search
Repeat until queue is empty:
* Remove vertex v from queue
* Add to queue all unmarked vertices adjacent to v and mark them.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709103405.png"/></div>

#### BFS:特性
In any connected graph G, BFS computes shortest paths from s to all other vertices in time proportional to E + V.
```Java
public class BreadthFirstPaths
{
    private boolean[] marked;
    private int[] edgeTo;
    private int[] distTo;
    …
    private void bfs(Graph G, int s) {
        Queue<Integer> q = new Queue<Integer>();
        q.enqueue(s);
        marked[s] = true;
        distTo[s] = 0;
        while (!q.isEmpty()) {
            int v = q.dequeue();
            for (int w : G.adj(v)) {
                if (!marked[w]) {
                    q.enqueue(w);
                    marked[w] = true;
                    edgeTo[w] = v;
                    distTo[w] = distTo[v] + 1;
                }
            }
        }
    }
}
```
### connected components
定义: Vertices v and w are connected if there is a path between them.  
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709104159.png"/></div>

定义: A connected component is a maximal set of connected vertices.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709104539.png"/></div>

实现方式很简单. To visit a vertex v :
* Mark vertex v as visited.
* Recursively visit all unmarked vertices adjacent to v.

```Java
public class CC
{
    private boolean[] marked;
    private int[] id;
    private int count;
    public CC(Graph G)
    {
        marked = new boolean[G.V()];
        id = new int[G.V()];
        for (int v = 0; v < G.V(); v++)
        {
            if (!marked[v])
            {
                dfs(G, v);
                count++;
            }
        }
    }
    public int count()
    { return count; }
    public int id(int v)
    { return id[v]; }
    public boolean connected(int v, int w)
    { return id[v] == id[w]; }
    private void dfs(Graph G, int v)
    {
        marked[v] = true;
        id[v] = count;
        for (int w : G.adj(v))
            if (!marked[w])
                dfs(G, w);
    }
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709110507.png"/></div>

## 有向图
### DG:Intro
<font color=blue>Digraph</font>: Set of vertices connected pairwise by directed edges.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709111609.png"/></div>

### DG:API
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709142448.png"/></div>

```Java
public class DiGraph
{
    private final int V;
    private final Bag<Integer>[] adj;
    public DiGraph(int V)
    {
        this.V = V;
        adj = (Bag<Integer>[]) new Bag[V];
        for (int v = 0; v < V; v++)
            adj[v] = new Bag<Integer>();
    }
    public void addEdge(int v, int w)
    {
        adj[v].add(w);
        // 和Graph 的唯一区别是这里不需要添加 adj[w].add(v);
    }
    public Iterable<Integer> adj(int v)
    { return adj[v]; }
}
```

### DG:Search
#### Depth-first search in digraphs
可以看到, 这个 DFS 算法和无向图是完全一致的, Every undirected graph is a digraph (with edges in both directions).
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709143051.png"/></div>

#### Breadth-first search in digraphs
广度优先算法在有向图和无向图中也是相同的.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709143919.png"/></div>

### topological sort
#### Precedence scheduling
<font color=green>Goal</font>:   Given a set of tasks to be completed with **precedence constraints**,in which order should we schedule the tasks?  
抽象:
vertex = task; edge = precedence constraint.

#### Topological sort
DAG: Directed acyclic(没有环) graph.  
Topological sort: Redraw DAG so all edges point upwards.  
具体的实现:
1. 首先运行 DFS
2. 然后用 post-order 返回节点<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709145051.png"/></div>

```Java
public class DepthFirstOrder
{
    private boolean[] marked;
    private Stack<Integer> reversePostorder;
    public DepthFirstOrder(Digraph G)
    {
        reversePostorder = new Stack<Integer>();
        marked = new boolean[G.V()];
        for (int v = 0; v < G.V(); v++)
            if (!marked[v]) dfs(G, v);
    }
    private void dfs(Digraph G, int v)
    {
        marked[v] = true;
        for (int w : G.adj(v))
        if (!marked[w]) 
            dfs(G, w);    
        reversePostorder.push(v);
    }
    public Iterable<Integer> reversePostorder()
    { return reversePostorder; }
}
```

### strong components
定义: 
* Vertices v and w are **strongly connected** if there is both a directed pathfrom v to w and a directed path from w to v.  
* A strong component is a **maximal subset** of strongly-connected vertices<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709145835.png"/></div>

#### Kosaraju-Sharir algorithm

Reverse graph: Strong components in G are same as in $G^R$.  
Kernel DAG: Contract each strong component into a single vertex.  
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709150159.png"/></div>

该算法的核心思想:
* 计算 $G^R$ kernal DAG 中的拓扑序<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709150820.png"/></div>
* Run DFS, considering vertices in reverse topological order.<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709150852.png"/></div>

```Java
public class KosarajuSharirSCC
{
    private boolean marked[];
    private int[] id;
    private int count;
    public KosarajuSharirSCC(Digraph G)
    {
        marked = new boolean[G.V()];
        id = new int[G.V()];
        DepthFirstOrder dfs = new DepthFirstOrder(G.reverse());
        for (int v : dfs.reversePostorder())
        {
            if (!marked[v])
            {
                dfs(G, v);
                count++;
            }
        }
    }
    private void dfs(Digraph G, int v)
    {
        marked[v] = true;
        id[v] = count;
        for (int w : G.adj(v))
            if (!marked[w])
                dfs(G, w);
    }
    public boolean stronglyConnected(int v, int w)
    { return id[v] == id[w]; }
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210709151134.png"/></div>

## 最小生成树
### MST: Intro
$\color{blue}{定义}$: **生成树**是图 G 的子图 T, 它满足以下性质:
* connected
* Acyclic
* 包含所有的节点

$\color{olive}{最小生成树}$: 给出一个包含Positive edge weights 的无向图(connected), 找出最小 weight 的生成树.

### MST: 贪婪算法
给出几个简化的假设: Edges weights 不相等, 并且图是 connected, 那么 MST 是存在且唯一的.  
图的一种**切分**(cut) 是将图的所有顶点分为两个非空且不重叠的两个集合。**横切边**是一条连接两个属于不同集合的顶点的边.
> **切分定理**:在一幅加权图中，给定任意的切分，它的横切边中的权重最小者必然属于图的最小生成树。

MST 的贪心算法: 
* 初始状态下所有边均为灰色
* 找到一种切分，它产生的横切边均不为黑色,将它权重最小的横切边标记为黑色
* 反复直到标记了 V-1 条黑色边为止

Efficient implementations: Choose cut? Find min-weight edge?
* Kruskal's algorithm.
* Prim's algorithm

### Weighted edge API
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713094808.png"/></div>

```Java
public class Edge implements Comparable<Edge>
{
  private final int v, w;
  private final double weight;
  public Edge(int v, int w, double weight)
  {
    this.v = v;
    this.w = w;
    this.weight = weight;
  }
  public int either()
  { return v; }
  public int other(int vertex)
  {
    if (vertex == v) return w;
    else return v;
  }
  public int compareTo(Edge that)
  {
    if (this.weight < that.weight) return -1;
    else if (this.weight > that.weight) return +1;
    else return 0;
  }
}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713095123.png"/></div>

在此基础上, 其实现的示意图如下:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713095239.png"/></div>

```Java
public class EdgeWeightedGraph
{
  private final int V;
  private final Bag<Edge>[] adj;
  public EdgeWeightedGraph(int V)
  {
    this.V = V;
    adj = (Bag<Edge>[]) new Bag[V];
    for (int v = 0; v < V; v++)
      adj[v] = new Bag<Edge>();
  }
  public void addEdge(Edge e)
  {
    int v = e.either(), w = e.other(v);
    adj[v].add(e);
    adj[w].add(e);
  }
  public Iterable<Edge> adj(int v)
  { return adj[v]; }
}
```
MST 的 API 如下:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713095442.png"/></div>

使用方式:
```Java
public static void main(String[] args)
{
    In in = new In(args[0]);
    EdgeWeightedGraph G = new EdgeWeightedGraph(in);
    MST mst = new MST(G);
    for (Edge e : mst.edges())
        StdOut.println(e);
    StdOut.printf("%.2f\n", mst.weight());
}
```
### Kruskal's algorithm
按照边的权重顺序（从小到大）处理, 将它加入最小生成树中, 加入的边不会与已经加入的构成环, 直到树中包含 V-1 条边为止.  
$\color{Olive}Challenge:$ 怎样检验加入的 v-w 边产生一个环, 其实现复杂度如何:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713141840.png"/></div>

$\color{Green}{Efeective\ Solution:}$ 使用 union-fine 数据结构:
* 对每个 connected component 维护一个 set
* 如果 w 和 v 在同一个 set, 那么加入边 v-w 会产生一个环
* 否则, 添加 v-w 边, 同时 merge 两个 set

```Java
public class KruskalMST
{
    private Queue<Edge> mst = new Queue<Edge>();
    public KruskalMST(EdgeWeightedGraph G)
    {
        MinPQ<Edge> pq = new MinPQ<Edge>(G.edges()); // 从边生成 pq
        
        UF uf = new UF(G.V());
        while (!pq.isEmpty() && mst.size() < G.V()-1)
        {
            Edge e = pq.delMin();
            int v = e.either(), w = e.other(v);
            if (!uf.connected(v, w))
            {
                uf.union(v, w);
                mst.enqueue(e);
            }
        }
    }
    public Iterable<Edge> edges()
    { return mst; }
}
```
$\color{Green}{复杂度分析:}$ Kruskal 算法的计算一幅含有 V 个顶点和 E 条边的连通加权无向图的最小生成树所需的空间和 E 成正比，所需的时间和 $E\log E$ 成正比（最坏情况, 就是排序需要的）。<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713143032.png"/></div>  

$\color{Olive}{特殊情况}$: If edges are already sorted, order of growth is $E \log* V$(通常$\log* V \leq 5$).
### Prim's algorithm
* 一开始这棵树只有一个顶点
* 然后向它添加V-1 条边，
* 每次总是将下一条 **连接树中的顶点与不在树中的顶点且权重最小的边** 加入树中

这显然是生成了 MST, 它自动将节点分成了两个切分, 而其中的权重最小的边就是横切边中的最小者.  
$\color{Olive}挑战:$ 找出只有1端在 T 中的最小边<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210714141340.png"/></div>

$\color{Olive}{Lazy\ solution}$: 维护一个由(至少)一个端点在 T 中的 **边组成的 PQ**.
* Delete-min to determine next edge e = v–w to add to T
* Disregard if both endpoints v and w are marked (both in T).
* Otherwise, let w be the unmarked vertex
  * add to PQ any edge incident to w
  * add e to T and mark w
    ```Java
    public class LazyPrimMST
    {
        private boolean[] marked; // MST vertices
        private Queue<Edge> mst;  // MST edges
        private MinPQ<Edge> pq;   // PQ of edges
        private void visit(WeightedGraph G, int v)
        {
            marked[v] = true;
            for (Edge e : G.adj(v))
                if (!marked[e.other(v)])
                    pq.insert(e);
        }

        public Iterable<Edge> mst()
        { return mst; }

        public LazyPrimMST(WeightedGraph G)
        {
            pq = new MinPQ<Edge>();
            mst = new Queue<Edge>();
            marked = new boolean[G.V()];
            visit(G, 0);
            while (!pq.isEmpty() && mst.size() < G.V() - 1)
            {
                Edge e = pq.delMin();
                int v = e.either(), w = e.other(v);
                if (marked[v] && marked[w]) continue;
                    mst.enqueue(e);
                if (!marked[v]) visit(G, v);
                if (!marked[w]) visit(G, w);
            }
        }
    }
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210714142521.png"/></div>

$\color{Olive}{Eager\ solution}$:  维护一个一条边连接到 T 的**顶点构成的 PQ**, 其优先级就是该点连接到 T 最短的边.  
* Delete min vertex v and add its associated edge e = v–w to T.
* Update PQ by considering all edges e = v–x incident to v
  * ignore if x is already in T
  * add x to PQ if not already on it
  * decrease priority of x if v–x becomes shortest edge connecting x to T

#### Indexed priority queue
Associate an index between 0 and N - 1 with each key in a priority queue:
* Supports insert and delete-the-minimum
* Supports decrease-key given the index of the key.

具体的实现是对之前 PQ 的改进:
* 首先有一个和之前的 MinPQ 一样的实现
* 维护平行的 array `keys[]`/`pq[]` 和`qp[]`:
  * key[i] 为 i 的优先级
  * pq[i] 是 index of the key in heap position i
  * qp[i] is the heap position of the key with index i
* Use `swim(qp[i])` to implement `decreaseKey(i, key)`.

<div align=center><img src="https://i.imgur.com/DQwK9rf.png"/></div>

## 最小路径(shortest paths)
Given an edge-weighted digraph, find the shortest path from s to t.
### SP:APIs
首先我们需要定义边和图, 在边和图的基础上才能实现最短路径:
<div align=center><img src="https://i.imgur.com/miaDg9G.png"/></div><div align=center><img src="https://i.imgur.com/8APFRhW.png"/></div>

```Java
public class DirectedEdge
{
  private final int v, w;
  private final double weight;
  public DirectedEdge(int v, int w, double weight)
  {
    this.v = v;
    this.w = w;
    this.weight = weight;
  }
  public int from()
  { return v; }
  public int to()
  { return w; }
  public int weight()
  { return weight; }
}
public class EdgeWeightedDigraph
{
  private final int V;
  private final Bag<DirectedEdge>[] adj;
  public EdgeWeightedDigraph(int V)
  {
    this.V = V;
    adj = (Bag<DirectedEdge>[]) new Bag[V];
    for (int v = 0; v < V; v++)
      adj[v] = new Bag<DirectedEdge>();
  }
  public void addEdge(DirectedEdge e)
  {
    int v = e.from();
    adj[v].add(e);
  }
  public Iterable<DirectedEdge> adj(int v)
  { return adj[v]; }
}
```
<div align=center><img src="https://i.imgur.com/COQCv9h.png"/></div>

### 最小路径的特性
我们的目标是找到从 s 到其他所有节点的最短路径, 观察发现存在一个 最短路径树(如果到某个顶点有两条路径, 可以删除其中一条的最后一条边). 于是我们可以用两个vertex-indexed arrays 表示这个 SPT:
* `distTo[v]` is length of shortest path from s to v.
* `edgeTo[v]` is **last edge** on shortest path from s to v.
    ```Java
    public double distTo(int v)
    { return distTo[v]; }
    public Iterable<DirectedEdge> pathTo(int v)
    {
        Stack<DirectedEdge> path = new Stack<DirectedEdge>();
        for (DirectedEdge e = edgeTo[v]; e != null; e = edgeTo[e.from()])
            path.push(e);
        return path;
    }
    ```

#### 边的松弛
在遇到新的边时, 更新信息就可以得到新的最短路径, 我们在其中会遇到边的松弛技术: 如果 e = v->w 给出了一条经由 v 到 w 的最短路径, 就要更新 `distTo[w]` 和 `edgeTo[w]`:
```Java
private void relax(DirectedEdge e)
{
    int v = e.from(), w = e.to();
    if (distTo[w] > distTo[v] + e.weight())
    {
        distTo[w] = distTo[v] + e.weight();
        edgeTo[w] = e;
    }
}
```
如果 G 是一个 edge-weighted digraph, 如果满足下面条件, `distTo[]` 就是从 s 出发的最短路径:
* distTo[s] = 0
* 对于每个顶点v, distTo[v] 就是从 s 到 v 的某个路径
* 对于边 e = v->w, distTo[w] <= distTo[v] + e.weight

#### 通用算法
首先将distTo[s]置0, 其他元素设置为无穷大, 然后做下面的动作:
> 放松 G 中的任意边, 直到不存在有效边为止.

### SP:Dijkstra's algorithm
首先将最小的非树顶点放松并加入树中, 直到所有的顶点都在树中或者所有的非树顶点的 `distTo[]` 均为无穷大.   
改算法计算任意给出无负边的 edge-weighted digraph 的 SPT,因为:
* 每条边 e = v->w 只松弛了一次, 保证 `distTo[w] <= distTo[v] + e.weight()`
* Inequality holds until algorithm terminates:
  * `distTo[w]` 不会增加
  * `distTo[v]` 不会改变

```Java
public class DijkstraSP
{
    private DirectedEdge[] edgeTo;
    private double[] distTo;
    private IndexMinPQ<Double> pq;
    public DijkstraSP(EdgeWeightedDigraph G, int s)
    {
        edgeTo = new DirectedEdge[G.V()];
        distTo = new double[G.V()];
        pq = new IndexMinPQ<Double>(G.V());
        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        pq.insert(s, 0.0);
        while (!pq.isEmpty())
        {
            int v = pq.delMin();
            for (DirectedEdge e : G.adj(v))
                relax(e);
        }
    }
}
private void relax(DirectedEdge e)
{
    int v = e.from(), w = e.to();
    if (distTo[w] > distTo[v] + e.weight())
    {
        distTo[w] = distTo[v] + e.weight();
        edgeTo[w] = e;
        if (pq.contains(w)) pq.decreaseKey(w, distTo[w]);
        else pq.insert (w, distTo[w]);
    }
}
```
这个算法看起来和 prim 算法很像, 实际是同一族算法(DFS and BFS are also in this family of algorithms), 主要的区别在于:
* Prim 算法选择距离<font color=red> tree</font> 最近的顶点(无向图中)
* Dijkstra 选择距离<font color=red> source</font> 最近的顶点(有向图中)

一般而言, 它的复杂度取决于内部PQ 的实现:<div align=center><img src="https://i.imgur.com/3wulAnv.png"/></div>

### edge-weighted DAGs
如果是无环的图, 找最短的路径更加简单, 可以使用拓扑排序:
* Consider vertices in topological order
* Relax all edges pointing from that vertex

同样我们得到的也是一个 SPT:
* 每条边 e = v->w 只松弛了一次, 保证 `distTo[w] <= distTo[v] + e.weight()`
* Inequality holds until algorithm terminates:
  * `distTo[w]` 不会增加: 它是单调降的
  * `distTo[v]` 不会改变: 因为是按照拓扑排序走的, v 松弛侯不会有边再指向 v(即使是有负边)

```Java
public class AcyclicSP
{
    private DirectedEdge[] edgeTo;
    private double[] distTo;

    public AcyclicSP(EdgeWeightedDigraph G, int s)
    {
        edgeTo = new DirectedEdge[G.V()];
        distTo = new double[G.V()];

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        Topological topological = new Topological(G);
        for (int v : topological.order())
            for (DirectedEdge e : G.adj(v))
                relax(e);
    }
}
```

### negative weights
a SPT exists iff **no negative cycles**.
#### Bellman-Ford algorithm
对下列操作重复 V 次: 松弛所有的边(E条).有个改进:FIFO implementation. Maintain queue of vertices whose distTo[] changed.
<div align=center><img src="https://i.imgur.com/zza8pnp.png"/></div>

#### 检查 negative cycle
If any vertex v is updated in pass V, there exists a negative cycle.

## 最大流问题
### MF:Intro
输入都是一个 edge-weighted digraph(其中的权重不是欧几里得距离而是容量), 有一个源 s 和目标 t.
#### Mincut Problem
定义 st-cut: 是将顶点分成两个不相连的集合, 其中 s 在一个 set A 而 t 在另一个 set B. 它的容量就是从 A 到 B 的边的容量之和.<div align=center><img src="https://i.imgur.com/lL2nezY.png"/></div>

Minimum st-cut (mincut) problem:<font color=olive> Find a cut of minimum capacity.</font>  

#### Maxflow problem
定义 An st-flow (flow) is an assignment of values to the edges such that:
* Capacity constraint: 0 ≤ edge's flow ≤ edge's capacit
* Local equilibrium: inflow = outflow at every vertex (except s and t)

Maximum st-flow (maxflow) problem:<font color=olive> Find a flow of maximum value.</font>

### Ford-Fulkerson 算法
* 首先将所有的 flow 初始化为0
* 增长通路(augmenting path)找到一条从 s 的 t 的 undirected path
  * 可以在 forward 方向上增加 flow(非满)
  * 可以在 backward 方向上降低 flow(非空)
* 根据瓶颈增加flow

### maxflow-mincut theorem
Def. The <font color=olive>net flow across</font>: a cut (A, B) is the sum of the flows on its edges from A to B minus the sum of the flows on its edges from B to A.  
<font color=blue>Flow-value lemma</font>: Let f be any flow and let (A, B) be any cut. Then, the net flow across (A, B) equals the value of f.  
它的一个推论就是, 从 s 出来的outflow 和 到达 t 的inflow 相等, 都等于 flow 的值.  
>弱二元性: 如果 f 是任意的 flow 而 (A,B) 是任意的 cut, flow value <= cut 的容量.  
Value of flow f = net flow across cut (A, B) ≤ capacity of cut (A, B).

<font color=olive>Augmenting path theorem</font>: A flow f is a maxflow iff no augmenting paths.  
<font color=olive>Maxflow-mincut theorem</font>. Value of the maxflow = capacity of mincut.

Pf. The following three conditions are equivalent for any flow f :  
i. There exists a cut whose capacity equals the value of the flow f.  
ii. f is a maxflow.  
iii. There is no augmenting path with respect to f.  
//TODO: 复杂度分析

## STRING SORTS
### String in Java
String 是字符的序列,而字符在不同的变成语言中是不同的.
* C char data type: 通常是一个 8-bit 整数
  * 支持 7-bit 的 ASCII
  * 最多支持 256 个字符
* Java char data type: A 16-bit unsigned integer
  * 支持原生的 16-bit Unicode
  * 支持 21-bit 的 Unicode 3.0

<font color=olive>String data type in Java</font>:Immutable sequence of characters.
* Length
* Indexing
* Substring 
* Concatenation(连接)
    <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210723101319.png"/></div>

除此以外, Java 中还有一个 `StringBuilder` 数据类型做客可变的字符序列, 内部是使用变长的 `char[]` 数组和长度实现的:<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210723111104.png"/></div>

此外还有和它类似的 `StringBuffer`, 它更慢但是是线程安全的.

### Key-Index Sorting
如果排序使用的 key 是 0 到 R-1 之间的整数, 我们可以将 key 作为 array index, 从而超过 $N\log N$ 的 optimal 算法.
* 使用 key 作为 Index 计算每个 letter 的频率
* 根据上面得到的频率计算累积的频率
* 根据累积频率移动元素
* 将元素复制回原来的 array

```Java
int N = a.length;
int[] count = new int[R+1];

for (int i = 0; i < N; i++)
    count[a[i]+1]++;

for (int r = 0; r < R; r++)
    count[r+1] += count[r];

for (int i = 0; i < N; i++)
    aux[count[a[i]]++] = a[i];

for (int i = 0; i < N; i++)
    a[i] = aux[i];
```
上面的复杂度很好分析, 就是 $\color{Olive}{N+R}$

### LSD(Least-significant-digit-first) radix sort
这个算法的思路很简单, 就是从右到左依次使用每个元素做 key-index sort:
* Consider characters from right to left
* Stably sort using $d^{ th}$ character as the key
    ```Java
    public class LSD
    {
        public static void sort(String[] a, int W) //fixed Length W String
        {
            int R = 256;
            int N = a.length;
            String[] aux = new String[N];
            for (int d = W-1; d >= 0; d--)
            {
                int[] count = new int[R+1];
                for (int i = 0; i < N; i++)
                    count[a[i].charAt(d) + 1]++;
                for (int r = 0; r < R; r++)
                    count[r+1] += count[r];
                for (int i = 0; i < N; i++)
                    aux[count[a[i].charAt(d)]++] = a[i];
                for (int i = 0; i < N; i++)
                    a[i] = aux[i];
            }
        }
    }
    ```
<div align=center><img src="https://i.imgur.com/V6nEcbX.png"/></div>

### MSD radix sort
* Partition array into R pieces according to first character
* Recursively sort all strings that start with each character

<div align=center><img src="https://i.imgur.com/2pSIXfa.png"/></div>

如果遇到了已经到达末尾的 string, 就当他们在尾部有一个比其他 Char 都小的 extra char.
```Java
private static int charAt(String s, int d)
{
    if (d < s.length()) return s.charAt(d);
    else return -1;
}
```
```Java
public static void sort(String[] a)
{
    aux = new String[a.length];
    sort(a, aux, 0, a.length - 1, 0);
}
private static void sort(String[] a, String[] aux, int lo, int hi, int d)
{
    if (hi <= lo) return;
        int[] count = new int[R+2];
    for (int i = lo; i <= hi; i++)
        count[charAt(a[i], d) + 2]++;
    for (int r = 0; r < R+1; r++)
        count[r+1] += count[r];
    for (int i = lo; i <= hi; i++)
        aux[count[charAt(a[i], d) + 1]++] = a[i];
    for (int i = lo; i <= hi; i++)
        a[i] = aux[i - lo];

    for (int r = 0; r < R; r++)
    sort(a, aux, lo + count[r], lo + count[r+1] - 1, d+1);
}
```
但是这种算法可能会有很严重的性能问题:
* 对于小的 subarray, 运行的速度非常慢
* 由于递归的存在, 会有大量的小 subarray

解决的办法就是低于某个临界值的时候, 使用 insertion sort:
* Insertion sort, but start at $d^{th}$ character.
    ```Java
    private static void sort(String[] a, int lo, int hi, int d)
    {
    for (int i = lo; i <= hi; i++)
        for (int j = i; j > lo && less(a[j], a[j-1], d); j--)
            exch(a, j, j-1);
    }
    ```
* 实现 less
    ```Java
    private static boolean less(String v, String w, int d)
    {
        for (int i = d; i < Math.min(v.length(), w.length()); i++)
        {
            if (v.charAt(i) < w.charAt(i)) return true;
            if (v.charAt(i) > w.charAt(i)) return false;
        }
        return v.length() < w.length();
    }
    ```

### 3-way radix quicksort
这个算法的思路就是对第 d 个元素做 3-way partitioning(大于/小于/等于)
* Less overhead than R-way partitioning in MSD string sort.
* Does not re-examine characters equal to the partitioning char

```Java
private static void sort(String[] a)
{ sort(a, 0, a.length - 1, 0); }
private static void sort(String[] a, int lo, int hi, int d)
{
    if (hi <= lo) return;
    int lt = lo, gt = hi;
    int v = charAt(a[lo], d);
    int i = lo + 1;
    while (i <= gt)
    {
        int t = charAt(a[i], d);
        if (t < v) exch(a, lt++, i++);
        else if (t > v) exch(a, i, gt--);
        else i++;
    }

    sort(a, lo, lt-1, d);
    if (v >= 0) sort(a, lt, gt, d+1);
    sort(a, gt+1, hi, d);
}
```
<div align=center><img src="https://i.imgur.com/NtlVvf6.png"/></div>
<div align=center><img src="https://i.imgur.com/PCmreMk.png"/></div>

### suffix arrays
在 java 中, substring 只是需要移动一个 offset, 因此我们可以在 $O(N)$ 的时间内生成一个 suffix. 然后可以根据这个做很多事情, 例如查找最长的重复子串, 就可以先做 suffix array, 然后对生成的 array 排序:
<div align=center><img src="https://i.imgur.com/IPK7bzL.png"/></div>

## 单词查找树
我们希望借助 String 的性质, 找到一种比 hashing 更快/比 BSTs 更灵活的数据结构和算法.<div align=center><img src="https://i.imgur.com/iX5D97y.png"/></div>

### R-way tries
trie 来自于一个文字游戏, 其主要作用是取出(re<font color=blue>trie</font>val)数据. 
* 将字符(而不是 keys) 存放到节点中
* 每个 node 有 R 个 children, 每个存放可能的字符(我们没有将 null 画出)
* 将 value 存放在相关的 key 的最后一个字符中<div align=center><img src="https://i.imgur.com/difuWL7.png"/></div>

在 trie 中的搜索十分简单, 沿着节点向下查找
* search hit: node where search ends has a non-null value
* Search miss: reach **null link** or node where search **ends has null value**.

Tries 中的插入:
* Encounter a null link: create new node.
* Encounter the last character of the key: set value in that node

Tries 中的删除:
* Find the node corresponding to key and set value to null.
* If node has null value and all null links, remove that node (and recur).

实现的时候, 我们将子节点放在一个 array 中, 其 index 对应的就是 char 的顺序
```Java
private static class Node
{
  private Object value;
  private Node[] next = new Node[R];
}
```

其他部分的代码如下:
```Java
public class TrieST<Value>
{
    private static final int R = 256;
    private Node root = new Node();
    private static class Node
    { /* see previous slide */ }

    public void put(String key, Value val)
    { root = put(root, key, val, 0); }
    private Node put(Node x, String key, Value val, int d)
    {
        if (x == null) x = new Node();
        if (d == key.length()) { x.val = val; return x; }
        char c = key.charAt(d);
        x.next[c] = put(x.next[c], key, val, d+1);
        return x;
    }
    public boolean contains(String key)
    { return get(key) != null; }
    public Value get(String key)
    {
        Node x = get(root, key, 0);
        if (x == null) return null;
        return (Value) x.val;
    }
    private Node get(Node x, String key, int d)
    {
        if (x == null) return null;
        if (d == key.length()) return x;
        char c = key.charAt(d);
        return get(x.next[c], key, d+1);
    }
}
```
#### performance
<font color=olive>Search hit</font>: Need to examine all L characters for equality.  
<font color=olive>Search miss</font>:
* Could have mismatch on first character
* Typical case: examine only a few characters (sublinear).

### ternary(三元的) search tries
这个思路是将 characters 和 values 放在 node 中, **each node has 3 children**: smaller (left), equal (middle), larger (right).<div align=center><img src="https://i.imgur.com/wVuzeYQ.png"/></div>    
<font color=olive>Search hit</font>: Node where search ends has a non-null value.  
<font color=olive>Search miss</font>: Reach a null link or node where search ends has null value.  

Node 中放了五个数据域:
```Java
private class Node
{
    private Value val;
    private char c;
    private Node left, mid, right;
}
```
其他部分代码如下:
```Java
public class TST<Value>
{
    private Node root;
    private class Node
    { /* see previous slide */ }

    public void put(String key, Value val)
    { root = put(root, key, val, 0); }
    private Node put(Node x, String key, Value val, int d)
    {
        char c = key.charAt(d);
        if (x == null) { x = new Node(); x.c = c; }
        if (c < x.c) x.left = put(x.left, key, val, d);
        else if (c > x.c) x.right = put(x.right, key, val, d);
        else if (d < key.length() - 1) x.mid = put(x.mid, key, val, d+1);
        else x.val = val;
        return x;
    }
    public boolean contains(String key)
    { return get(key) != null; }
    public Value get(String key)
    {
        Node x = get(root, key, 0);
        if (x == null) return null;
        return x.val;
    }
    private Node get(Node x, String key, int d)
    {
        if (x == null) return null;
        char c = key.charAt(d);
        if (c < x.c) return get(x.left, key, d);
        else if (c > x.c) return get(x.right, key, d);
        else if (d < key.length() - 1) return get(x.mid, key, d+1);
        else return x;
    }
}
```

可以使用 $R^2$ TST, 作为 R-way tries 和 TST 的混合, 最早两个字母用一个 $26^2$ 的矩阵表示, 后面的用 TST, 它可以作为 TST 的一种提升:
<div align=center><img src="https://i.imgur.com/xadOzZu.png"/></div>
<div align=center><img src="https://i.imgur.com/hpx0Q7G.png"/></div>

### character-based operations
符号表可以支持若干常用的 character-based 操作:
* Prefix match
* Wildcard match
* Longest prefix

```Java
private void collect(Node x, String prefix, Queue<String> q)
{
    if (x == null) return;
    if (x.val != null) q.enqueue(prefix);
    for (char c = 0; c < R; c++)
        collect(x.next[c], prefix + c, q);
}
public Iterable<String> keysWithPrefix(String prefix)
{
    Queue<String> queue = new Queue<String>();
    Node x = get(root, prefix, 0);
    collect(x, prefix, queue);
    return queue;
}
```
在 Trie 中左最长公共字符串十分简单:
* search for query string
* keep track of longest key encountered

```Java
public String longestPrefixOf(String query)
{
    int length = search(root, query, 0, 0);
    return query.substring(0, length);
}
private int search(Node x, String query, int d, int length)
{
    if (x == null) return length;
    if (x.val != null) length = d;
    if (d == query.length()) return length;
    char c = query.charAt(d);
    return search(x.next[c], query, d+1, length);
}
```
#### Patricia trie
* Remove one-way branching
* Each node represents a sequence of characters<div align=center><img src="https://i.imgur.com/pxYtVr8.png"/></div>

#### Suffix tree
Patricia trie of suffixes of a string.<div align=center><img src="https://i.imgur.com/czUML4Z.png"/></div>

## 子串查找
<font color=olive>Goal</font>: Find pattern of length M in a text of length N.(通常 `M<<N`)  

### brute force
对于每个 text 作为位置, 检查 pattern是否匹配.
```Java
public static int search(String pat, String txt)
{
    int M = pat.length();
    int N = txt.length();
    for (int i = 0; i <= N - M; i++)
    {
        int j;
        for (j = 0; j < M; j++)
            if (txt.charAt(i+j) != pat.charAt(j))
                break;
        if (j == M) return i;
    }
    return N;
}
```
在最坏的情况下的, 需要 $\sim MN$ 次比较.
#### 回退的问题
在很多应用场景下, 我们希望避免 text stream 中的回退, 但是上面的暴力算法包含了回退的过程:<div align=center><img src="https://i.imgur.com/dvYVrM5.png"/></div>

一个常见的改进是在增加 j 的同时增加 i, 但依旧改变不了需要回退的结果.
```Java
public static int search(String pat, String txt)
{
    int i, N = txt.length();
    int j, M = pat.length();
    for (i = 0, j = 0; i < N && j < M; i++)
    {
        if (txt.charAt(i) == pat.charAt(j)) j++;
        else { i -= j; j = 0; }
    }
    if (j == M) return i - M;
    else return N;
}
```
### Knuth-Morris-Pratt
前面的暴力算法没有利用已经扫描过的子串信息而常常需要回退, 而KMP 算法总是能够避免回退, 它是基于**确定性有限状态自动机**(DFA).
* 有限数量的状态(包括开始和停止)
* 对于字母表中的每个 char 只有一种转变
* 如果转变到了 halt 状态则代表已经找到子串<div align=center><img src="https://i.imgur.com/PaTuQL7.png"/></div>

该有限状态机使用一个 2-D array 表示, 在该状态下, 接收到对应的数字则转到对应的状态:
```Java
public int search(String txt)
{
    int i, j, N = txt.length();
    for (i = 0, j = 0; i < N && j < M; i++)
        j = dfa[txt.charAt(i)][j];// 从矩阵中, 查找接收到数字之后的下一个状态
    if (j == M) return i - M;
    else return N;
}
```
于是最重要的问题就是**如何建立 DFA**:
1. 对于成功匹配的转变, 只需要 j+1 即可
2. 在不匹配的情况下, 在状态 j 时并且下一个字符是 `c!= pattern.charAt(j)`, 只需要在还在建立中的 DFA 中模拟 `pattern[1..j-1]`(忽略首字母是因为要右移一位, 忽略最后的字符是因为匹配失败), 然后做转换 c, 在代码中就是 Copy `dfa[][X]` to `dfa[][j]`:<div align=center><img src="https://i.imgur.com/aRdUJrU.png"/></div>
    ```Java
    public KMP(String pat)
    {
        this.pat = pat;
        M = pat.length();
        dfa = new int[R][M];
        dfa[pat.charAt(0)][0] = 1;
        for (int X = 0, j = 1; j < M; j++)
        {
            for (int c = 0; c < R; c++)
                dfa[c][j] = dfa[c][X];
            dfa[pat.charAt(j)][j] = j+1;
            X = dfa[pat.charAt(j)][X];
        }
    }
    ```

KMP 算法需要不多于 M+N 次的 char access 和 $R\times M$ 的空间. 