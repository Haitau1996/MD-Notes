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

$\color{Green}Efeective\ Solution:$ 使用 union-fine 数据结构:
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
$\color{Green}复杂度分析:$ Kruskal 算法的计算一幅含有 V 个顶点和 E 条边的连通加权无向图的最小生成树所需的空间和 E 成正比，所需的时间和 $E\log E$ 成正比（最坏情况, 就是排序需要的）。<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210713143032.png"/></div>  

$\color{Olive}特殊情况$: If edges are already sorted, order of growth is $E \log* V$(通常$\log* V \leq 5$).
### Prim's algorithm
* 一开始这棵树只有一个顶点
* 然后向它添加V-1 条边，
* 每次总是将下一条 **连接树中的顶点与不在树中的顶点且权重最小的边** 加入树中

这显然是生成了 MST, 它自动将节点分成了两个切分, 而其中的权重最小的边就是横切边中的最小者.  
$\color{Olive}挑战:$ 找出只有1端在 T 中的最小边<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20210714141340.png"/></div>

Lazy solution. 维护一个由(至少)一个端点在 T 中的边组成的 PQ.
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

