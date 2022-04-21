# CS 453 Computer Networks
by Jim Kurose

# Introduction
- [x] 什么是因特网， 什么是协议
- [x] **网络边缘**， 接入互联网， 物理媒介
- [x] **网络核心**： 分组交换， 电路交换， 网络结构
- [x] 性能：丢失， 延迟， 吞吐量
- [ ] 协议分层，服务模型
- [ ] 安全，历史。。。

从组成的角度看， 
* 在网络边缘有 Host(end systems),它们在网络的边缘运行 apps.
* 在网络的核心， 有分组交换机(packet switches), 转发 packets(chunks of data)
  * 路由器， 交换机
* communication links 将它们联系在一起
* networks ：
  * 一系列由某个组织管理的设备，路由器，连接
* internet:"network of networks"
  * 将 ISP 连接起来
* 在网络的每个角落都有 **protocols**

从服务的角度看，
* 网络是给程序提供服务的基础设施
* 给分布式程序提供编程接口(programming interface)

协议(protocol)定义了在两个或多个通信实体之间交换的报文的格式和顺序，以及报文发送和／或接收一条报文或其他事件所采取的动作 。
* 报文
* 传输报文的动作

## 网络边缘
怎样连接到互联网？
* 家庭接入
* 机构接入
* 移动互联网接入（WIFI/4G/5G）

我们需要考虑它们的：
* 传输速率
* 用户间共享或者是单独使用

### 接入网络
Access networks: cable-based access(电缆因特网接入)   
<div align=center><img src="https://s2.loli.net/2022/01/21/7V4fE5LzirdBtUH.png" width="50%"/></div>

频分复用(FDM, frequency division multiplexing): 不同频道(channels) 在不同的频率带中传输。  
* 混合光纤同轴系统： 非对称的（上行比下载慢）
* 需要调制解调器
* 共享广播媒体

Access networks: digital subscriber line(DSL)(数字用户线)  
<div align=center><img src="https://s2.loli.net/2022/01/21/U9khmtBYXEbMDdq.png" width="50%"/></div>

* 使用**现有的**电话线连接到本地中心局的DSLAM(Digital Subscriber Line Access Multiplexer)
  * data 流向 internet
  * voice 流向 telephone net

Access networks: home networks  <div align=center><img src="https://s2.loli.net/2022/01/21/t8JnbdDrq1jGv7s.png" width="50%"/></div>

无线网络接入：  
在端系统和路由器之间共享无线的网络接入， via 基站（aka "接入点"）。
* wireless local aera networks(WLANs)
* Wide-aera cellular access networks(4G,5G)<div align=center><img src="https://s2.loli.net/2022/01/21/GFWk2Vu1nyLTU7g.png" width="70%"/></div>

Access networks: 企业网络  
* 有线、无线链路技术的组合，连接到交换机和路由器的组合 
  * 以太网
  * WIFI

Access networks: 数据中心网络
* 高带宽链路（10s 到 100s Gbps）将数百到数千台服务器连接在一起，并连接到 Internet 

### Host: sends packets of data
Host 的发送功能：
* take 应用的信息
* 将它分解成长度为 L bit的小块（packets）
* 通过接入网络以 R 的传输率传输 packet
  * 连接的传输速率，aka 连接容量，连接带宽<div align=center><img src="https://s2.loli.net/2022/01/21/6sfmLAvndGREeK2.png" width="40%"/></div>

### links: 物理媒介
* bit:在发送/接收 对之间传播
* 物理链路：发射器和接收器之间的连接
* 引导型媒体
  * 信号在固体介质中传播：铜、光纤、同轴电缆
* 非引导型媒体
  * 信号自由传播

常见的物理媒介：
* 双绞线： 两根绝缘的铜线组成
* 同轴电缆： 
  * 两个同心的铜导体组成
  * 双向的
* 光纤
  * 柔软的， 能够导引光脉冲的媒体。每个脉冲一个比特
  * 支持极高的比特率
* 无线电通道
  * 广播， 半双工
  * WIFI,4G cellular, 蓝牙

## 网络核心
* 网络的核心由网状的内部连接的 routers 构成
* 其核心的功能是**转发**(forwarding, 也被称为 switching)和**路由**(routing)
  * 端系统将信息分成小的 packet(分组)， 网络将分组从源到目的地链路的一个路由器**转发**(forwarding)到另一个路由器， 转发是一个 local 的行为<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220419144123.png" width="30%"/></div>
  * 路由是一个全局的行为， 决定分组从源到目的地的路径<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220419144049.png" width="70%"/></div>

### 分组交换
#### 存储转发传输
多数分组交换机在链路的输入端使用**存储转发传输**(store-and-forward transmission)机制。 在向输出链路传输某分组的第一个比特之前， **必须接受完整个分组**。如果传入的速度超过服务上限， 就需要排队等候(**Queueing**)<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220420195742.png" width="70%"/></div>

* 如果在某个时间段传入的速度都比输出链路的速度慢， 分组需要承受输出缓存的**排队时延**
* 如果一个到达的分组发现缓存已经被占满，将会出现**分组丢失**(packet loss)。

### 电路交换
电路交换是另一种交换方法， 端到端通过**独占资源**(no sharing)的方式， 保留了源到目的地的 call.   
在网络中有两种常见的电路交换方式： FDM 和 TDM.
* 在频分复用中， 电磁波按频率被分成很多的窄的频率带(frequency bands), 每个 call 持有一个频带并且可以以该频带的最大速度传输
* 在时分复用中， 时间被分成不同的片段， 每个 call 在分配给它的时间段可以以最大速度传输数据<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220420201627.png" width="50%"/></div>

#### 两种交换的对比
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220420202259.png" width="50%"/></div>

在这样的系统中， 分组交换支持 35 个用户， 只有小于 0.0004 的可能性会导致过载， 而电路交换只能支持 10 个用户。
* 分组交换适用于突发式的数据
* 但是分组交换可能出现过度拥堵

因此一个很自然的思路就是让分组交换实现电路交换类似的行为。

### Internet 结构： 网络的网络
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220420203010.png" width="50%"/></div>
今天的因特网是一个网络的网络， 其中包括 ISP(网络服务提供商)网络，IPX(因特网交换点)和内容提供商网络(CPN)。

## 性能： 分组交换中的时延，丢包和吞吐量
### 时延
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421100716.png" width="70%"/></div>

从上图看出， 时延有四个来源：
* **节点处理时延**：检查比特差错， 决定输出链路等， 通常是微秒量级或者更少
* **排队时延**：取决于路由器的拥挤程度
* **传输时延**：将所有的比特推向链路需要时间，链路的传输速度是有限的， 这个时延取决于套接字长度(L)和 链路的传输速度(R),$d_{trans} = L/R$
* **传播时延**： 取决于路程和链路长度，$d_{prop}=d/s$

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421111108.png" width="60%"/></div>

我们使用一个汽车过收费站的例子理解传输时延河传播时延的区别， 如果这个收费站处理车的速度是 12秒一辆， 那么整个车队的传输时延就是 12 * 10 = 120秒， 而传播时延就是就是距离除以时间， 差不多是 1 个小时。  
### 排队时延和丢包
我们定义一个流量强度 $\frac{L\cdot a}{R}$, 其中 a 是平均到达速率， L 为分组长度， R 为链路的传输速度。， 那么它和时延的有一个很符合直觉的关系：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421111850.png" width="30%"/></div>

当分组到达一个已经满的队列时，路由器就会丢弃（drop）这个分组， 丢失的分组可能会被前一个节点重新传输， 或者从源端系统重新发送， 也可能什么都不做。

### 吞吐量
吞吐量: 从发送者到接受者之间传输bit 的比特率(bit/time unit) 。
* 瞬时吞吐量: 某个时间， 从发送者到接受者之间传输的bit 的比特率。
* 平均吞吐量：某个时间段的平均值

这通常取决于链路的瓶颈，在这里， 就是 $min(R_c, R_s, R/N)$,通常网络的瓶颈在于网络边缘：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421113129.png" width="40%"/></div>

