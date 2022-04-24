# CS 453 Computer Networks
by Jim Kurose

# Introduction
- [x] 什么是因特网， 什么是协议
- [x] **网络边缘**， 接入互联网， 物理媒介
- [x] **网络核心**： 分组交换， 电路交换， 网络结构
- [x] 性能：丢失， 延迟， 吞吐量
- [ ] 协议分层，服务模型
- [x] 安全，历史。。。

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

## 分层，封装和服务模型
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421131032.png" width="70%"/></div>

分层：每个层级实现一个服务
* 在这个层级执行了某些动作
* 直接使用下层的服务

分层是设计复杂系统的一种方式， 它有两个明显的优点：
1. 清晰的结构让我们可以很好地定位系统的组成部分， 理解它们之间的关系
2. 模块化帮助简化系统的维护和更新

### 协议分层
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421132219.png" width="80%"/></div>
//todo: 补充协议的细节

### 安全
在不同的 layer 中都需要考虑安全问题， 其中两点是最需要关注的：
1. 有怎样的安全问题
2. 应该如何应对

分组嗅探：<div align=center><img src="https://i.imgur.com/XtkECX1.png" width="60%"/></div>
IP 哄骗： <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421195646.png" width="60%"/></div>
拒绝服务攻击（Denial of Service attack):<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220421195803.png" width="40%"/></div>

常见的保护手段：
1. 认证： proving you are who you say you are
2. 保密： 通过加密实现
3. 完整性验证： 使用数字签名验证数据的完整性
4. 访问限制： 密码保护的虚拟私有网络
5. 防火墙

# 应用层
## Overview
* 理解应用层协议的概念和实现：
   - [ ] 传输层的服务模型
   - [ ] 客户端-服务器模型
   - [ ] P2P 模型
* 通过主流应用层协议和基础设施的学习进一步理解网络协议
   - [ ] HTTP
   - [ ] SMTP, IMAP, POP3
   - [ ] DNS
   - [ ] 视频流系统， CDNs
* 编写网络应用程序
   - [ ] Socket API

客户端-服务器模型（如 http,imap, FTP）：
* 在服务器端：
  * 是一个一直在线(always-on)的主机
  * 有永久的 IP 地址
  * 通常为了方便扩容， 放在数据中心中
* 客户端：
  * 和服务器通信
  * 可能是间歇性在线
  * 可能只有动态 ip 地址
  * 客户端之间**不相互通信**

P2P 模型（如p2p 文件共享）：
* 不存在一直在线的服务器
* 任意的端系统都可以直接通信
* 主机向其他主机申请服务， 同时为其他主机提供服务
* 主机只是间歇性在线， 同时会改变 IP 地址

进程间通信：
* 进程： 主机上运行的程序实例
  * 在同一个主机上， 两个进程通过进程间通信(inter-process communication) 进行通信
  * 在不同的端系统上， 进程通过交换消息通信

进程通过**套接字**接口发送/接收消息， 套接字就像是一扇门， 而一个消息传输过程涉及到发送端和接收端的两个套接字，此外需要提供**IP地址**和**端口号**进行进程寻址。  
此外，通信需使用具体的协议(包括开源和闭源的协议)， 在协议中定义了很多东西：
* 交换信息的类型：如请求， 响应
* 消息的语法：消息中有哪些域
* 消息的语义： 域中信息的含义
* 进程如何发送/响应 消息的规则

应用使用的传输端服务包括很多方面：
1. 可靠的数据传输(data integrity)
2. Timing(定时， 时机的掌握)
3. 吞吐量
4. 安全<div align=center><img src="https://i.imgur.com/yK1gNVH.png" width="80%"/></div>


传输端有两种常见的服务协议：
| TCP 服务  | UDP 服务  |
| :----: | :----: |
| 可靠的数据传输  | 不可靠的数据传输  |
| 流控制： 发送端不会压垮接收端| 不提供可靠性，流控制， 拥堵控制，timing/吞吐量 保证，安全和 连接setup |
|拥堵控制| |
|面向连接： 传输之前需要 set up|   | 
|不提供：timing/最小吞吐保证, 安全|  | 

