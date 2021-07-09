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

