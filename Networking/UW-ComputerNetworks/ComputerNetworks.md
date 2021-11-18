# Computer Networks
[David Wetherall](https://www.youtube.com/playlist?list=PLVEo1P9gAninM7KwP1KKolfMQdNs6P6Am)
## Intro to Protocals and Laying
在网络中有三个 topics, 分别讨论 信号、分组(packets) 和 apps 在网络中是如何工作的，这门课程主要讨论的是中间的部分：<div align=center><img src="https://i.imgur.com/x257gJ0.png"/></div>
* internet 是如何工作的
* 计算机网络的基础
  * 我们必须解决怎样复杂的问题
  * 哪些策略被证明是珍贵的

### the Use of Network
* For User communication
  * 在这个场景下低延迟十分重要
* For resource sharing: 比给每个用户一份专用的资源更加高效
  * 网络链接本身通过 **Statistical Multiplexing** 共享（通过用户需求的 statistics 共享，multiplexing 意思就是共享），例如使用 100Mb 带宽给平均使用率 50% 的30个用户提供服务<div align=center><img src="https://i.imgur.com/Wd7Nhf4.png"/></div>
* For content delivery:可以使用 replicas（复制品）<div align=center><img src="https://i.imgur.com/D6pbafK.png"/></div>
* For Computer Communication
* To Connect Computers to the Physical World: 如 WebCams, door-locks

连接的价值（Metcalfe’s Law）：网络的价值和节点数的平方（$N^2$）成正比<div align=center><img src="https://i.imgur.com/8irPpkE.png"/></div>

### 网络的组件（components）
<div align=center><img src="https://i.imgur.com/hCq2iuN.png"/></div>
上图是网络的主要主城部分和功能， 下图是功能的表示：
<div align=center><img src="https://i.imgur.com/hlVJ96n.png"/></div>

Links 的种类：
* Full-duplex: 全双工， bidirectional, 可以同时双向使用
* Half-duplex: 半双工， bidirectional, 但是不能同时使用， 无线网就是这样一个例子
* Simplex:单工，不常见

无线连接是一个半双工的例子，里面消息是通过广播传播，范围内的每个Node 都会接受到。<div align=center><img src="https://i.imgur.com/3UkCaUc.png"/></div>
根据不同规模， 可以将网络分类：<div align=center><img src="https://i.imgur.com/J2kO6rm.png"/></div>
<div align=center><img src="https://i.imgur.com/nOKch9W.png"/></div>

其中有两个重要的接口：
1. app 和 host 之间的(**sockets are widely used**)
2. 网络（ app 以下的部分）的组件之间(可以使用 traceroute 查看)
    <div align=center><img src="https://i.imgur.com/mLEC8Vk.png"/></div>

### the Socket API
Network-Application 接口定义了应用如何使用网络， 让 apps 通过 hosts 访问对方， 而**隐藏了 network 的细节**：<div align=center><img src="https://i.imgur.com/MZephjF.png"/></div>

对于一个简单的 client-server setup,这是很多应用得基础, 如 文件传输/网页浏览 
* 客户端向服务端发送一个请求
* 服务端 returns a (longer) reply

Socket API 是使用网络的抽象, 它是主流操作系统和编程语言的一部分,提供流和数据报两种服务,它有下面的 API:<div align=center><img src="https://i.imgur.com/TOnAHM5.png"/></div>

它们的执行顺序如下, 首先要两边都要有一个 socket,然后服务端需要更多的准备,接下来就是 sent-reply, accept 比 sent 还要早, recvive 比 send 也要更早, 这是一个 call block, 相当于是对方要先做好接收准备, 这边才开始接收:
<div align=center><img src="https://i.imgur.com/zDJt8sY.png"/></div>

traceroute 是广泛使用的命令行工具, 使用 network-network 接口(IP) 窥探网络内部.<div align=center><img src="https://i.imgur.com/F4Dtwrh.png"/></div>

### 协议与分层
网络的功能非常多, 我们需要某种模块化的方式管理其中的复杂度. 协议和分层是分割网络功能地主要结构化方法, 每一个协议地实例都通过协议和其同位体(peer)virtually 对话, 并且只使用来自下一层的服务.<div align=center><img src="https://i.imgur.com/nrnq5ia.png"/></div>
使用的一系列协议被称为协议栈(protocol stack):<div align=center><img src="https://i.imgur.com/GSkxf0q.png"/></div>

Encapsulation 是实现高效协议分层的机制, 
* 较低层次包装来自上层的内容, 并且添加自己的信息形成一条用于传输的新的消息
* 有点像用信封送信, 邮政系统不会查看内部的信息<div align=center><img src="https://i.imgur.com/u1eCaOg.png"/></div>

实际上 header 中有 demultiplexing key, 帮助我们做信号分解:<div align=center><img src="https://i.imgur.com/OGqWefB.png"/></div>

有了分层之后, 就可以方便地做信息隐藏和组件复用, 甚至用于连接不同地系统:<div align=center><img src="https://i.imgur.com/QX5hQUV.png"/></div>

同时, overhead 和 hide information 也是 Laying 的主要缺点.

### 参考模型
分层之后, 有一个关键的设计问题就是**在各个 Layer 中要实现哪些功能**,参考模型就给我们提供了一个指导框架(这些只是参考模型, 并不严格要求, 经常有多个协议同时在一个 Layer 上, 有时候也难以将一个特定的协议严格限制在一个 Layer) 
**OSI 七层模型**:<div align=center><img src="https://i.imgur.com/64yvDmu.png"/></div>

**common practice**:<div align=center><img src="https://i.imgur.com/0TSqrqf.png"/></div><div align=center><img src="https://i.imgur.com/k1xoHBf.png"/></div>

不同层的数据单元有不同的名称:<div align=center><img src="https://i.imgur.com/9Y4FSuD.png"/></div>
同样地,它们地设备名称也是不同, 但是它们长得都差不多:<div align=center><img src="https://i.imgur.com/eu0QluG.png"/></div>