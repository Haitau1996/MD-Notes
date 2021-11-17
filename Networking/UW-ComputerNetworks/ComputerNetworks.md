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
Network-Application 接口定义了应用应该如何使用网络， 让 apps 通过 hosts 访问对象， 而**隐藏了 network 的细节**：<div align=center><img src="https://i.imgur.com/MZephjF.png"/></div>

对于一个简单的 client-server setup, 
* 客户端向服务端发送一个请求
* 服务端 returns a (longer) reply
