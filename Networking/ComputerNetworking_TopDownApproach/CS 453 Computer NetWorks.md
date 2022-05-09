# CS 453 Computer Networks
看 [Jim Kurose](https://gaia.cs.umass.edu/kurose_ross/index.php) 所开[课程](https://gaia.cs.umass.edu/kurose_ross/lectures.php)的笔记

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
| :----------------------: | :----------------------------------------------------: |
| 可靠的数据传输  | 不可靠的数据传输  |
| 流控制： 发送端不会压垮接收端| 不提供可靠性，流控制， 拥堵控制，timing/吞吐量 保证，安全和 连接setup |
|拥堵控制| |
|面向连接： 传输之前需要 set up|   | 
|不提供：timing/最小吞吐保证, 安全|  | 

## Web 和 HTTP
网页由一系列对象构成， 每个都存放在不同的服务器上：
* 对象可以死 html 文件， jpeg 图片， 音频等
* 多数网页由基础的 html 文件组成，它坑你包含多个指向的对象， 每个都由 URL 寻址<div align=center><img src="https://i.imgur.com/Lm824gE.png" width="60%"/></div>

### HTTP 概况
HTTP， 超文本传输协议，是应用层协议， 采用客户端-服务器模型：
* client: 提出请求、接收数据和显示网页对象的浏览器(也可能内置于系统中)
* server: 响应请求，发送网页对象

HTTP 使用 TCP 作为其支撑运输协议，是一个无状态协议（服务器不保存客户请求历史） ，通常有以下步骤：
* 客户初始化连接到服务器的 80 端口
* 服务器同意客户的 TCP 连接请求
* 客户端和服务器交换 HTTP 报文
* 连接结束

HTTP 连接有两种：**持续连接**和**非持续连接**
| 非持续连接 | 持续连接 |
| :------------------: | :------------------: |
| TCP connection opened| TCP connection opened to a server|
| **最多一个对象**可以通过TCP连接发送| 多个对象可以通过客户端和服务器的**同一个 TCP 连接**发送|
|TCP 连接关闭   | TCP 连接关闭   |

非持续连接流程示意：<div align=center><img src="https://i.imgur.com/KTLOvw7.png" width="70%"/></div>

* 如果知道了往返时间(RTT), 每个对象都需要 2 个 RTT<div align=center><img src="https://i.imgur.com/7SUBVK6.png" width="40%"/></div>
* 在持续连接中， 服务器在发送响应后保持该 TCP 连接打开，后续的请求和响应报文能够通过相同的连接进行传送

### HTTP 报文格式
* 请求报文
    ```http
    GET /somedir/page.html HTTP/1.1
    Host: www.someschool.edu
    Connection: close
    User-agent: Mozilla/5.0
    Accept-language: fr
    ```

//todo: 完善报文格式

### Web 缓存
Web 缓存器也叫代理服务器， 目标是在没有牵涉源服务器的情况下满足客户需求。
* 用户配置浏览器指向一个(可能在本地)的 web cache
* 浏览器的请求都发送给 Cache
  * 如果对象在 cache 中， 返回
  * 否则 cache 向源服务器发送请求，并且缓存源服务器返回对象， 然后返回给客户<div align=center><img src="https://i.imgur.com/yIZkK7n.png" width="40%"/></div>

在这里， Web 缓存器即作为源服务器的客户端， 也作为实际发出请求的客户端的服务端。 服务器可以在响应的头部中告知缓存器允许的缓存类型， 如<div align=center><img src="https://i.imgur.com/Je6JLJR.png" width="40%"/></div>

使用缓存器能带来很多好处：
1. 减小客户请求的响应时间
2. 减少机构接入链路的拥堵
3. 有了缓存器， 因特网更加稠密

### Conditional GET
缓存引入了新的问题： **存放在缓存器中的副本可能是陈旧的**。HTTP 有一种**条件GET**机制
* 客户端在使用 GET 的时候包含 IF-MODIFIED-SINCE 首部行
* 如果缓存已经是最新的， 服务器返回状态码 304 (Not Modified)并且不反悔对象, 否则返回对象<div align=center><img src="https://i.imgur.com/LzgrZI4.png" width="50%"/></div>

### HTTP/2 
核心目标是减少多对象 http 请求的延迟
* 请求对象的传输顺序可以基于客户端定义的优先级顺序(不一定要 FCFS)
* 可以将没有请求的对象(将来可能需要)发送给客户
* 可以将对象分成帧(Frames)， 并且调度帧以避免 HOL blocking

例如：
* HTTP 1.1:<div align=center><img src="https://i.imgur.com/0j5dmxu.png" width="60%"/></div>
* HTTP/2: <div align=center><img src="https://i.imgur.com/sjAcYjl.png" width="60%"/></div>

## 电子邮件
电子邮件系统有 3 个主要部分： 用户代理(user agent), 邮件服务器(mail server) 和简单邮件传输协议(SMTP)。  
SMTP 使用 TCP 的可靠数据传输通过端口 25 将用户消息从客户端发送给服务器, 传输的三个阶段
* SMTP 握手
* SMTP 传输数据
* SMTP 关闭

它的命令和响应行为有点像 HTTP:
* commands: ASCII 文本
* 响应： 状态码和短语

|  SMTP   | HTTP  |
|:---:|:---:|
| 推协议  | 拉协议  |
| 多个对象可以放在消息的不同部分 | 每个对象被封装在它自己的响应消息中|
|使用持续性连接| 持续性、非 持续性连接 |

## DNS(Domain Name System)
网络上的主机有两个标识： 域名 和 IP地址。DNS 服务的作用就是在两者之间提供翻译：
* 一个由分层的 DNS 服务器提供的分布式数据库
* 是一个应用层协议：
  * 反映了网络设计的哲学： 保持核心简单， 在边缘添加复杂性

除了域名到 IP地址的转换， 还提供以下服务：
* 主机别名
* 邮件服务器别名
* 负载分配

DNS 没有采用集中式设计， 因为它包含很多问题：**单点故障**， **通信容量**，**远距离的集中式数据库** 和维护问题。  
DNS 服务器大致有三种类型：<div align=center><img src="https://i.imgur.com/jxy6FsJ.png" width="70%"/></div>

在 engineering.nyu.edu 的主机想要知道 gaia.cs.umass.edu 的地址， 需要经历复杂的查询过程：
* 递归查询：<div align=center><img src="https://i.imgur.com/bNj0HLg.png" width="70%"/></div>
* **迭代查询**：<div align=center><img src="https://i.imgur.com/wueLJP5.png" width="70%"/></div>

为了改善时延性能并且减少 DNS报文数量，DNS缓存让本地DNS 服务器能将收到的一个 DNS回答缓存并在一段时间后丢弃信息。

## 视频流， 内容分发网络
预先录制的视频现在是住宅 ISP 流量的主体，这种服务面临两个重大问题：
* scale: 怎样触及 10亿级别的用户
* 多样性： 不同的用户有不同的能力，有线 vs 无线， 高带宽 vs 低带宽

解决的办法就是：分布式应用层的基础设施。视频的编码率有的是固定的(CBR)，有的是可变的(VBR)。而因特网视频的传输又面临两个问题： 用户-服务器之间的带宽随着时间变化， 并且拥堵会引发丢包和延迟问题。
* 使用缓存可以部分解决后一个问题：  
  * 在没有缓存的情况下， 获得帧的timing 和源不同， 时快时慢：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220427135903.png" width="70%"/></div>
  * 添加了视频缓存：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220427140050.png" width="70%"/></div>
* 经 HTTP 的动态适应流（DASH）是应对第一个问题的办法
  * 视频被分解成不同的块
  * 每个块以不同的比特率编码
  * 不同的编码块存放在不同的文件中，并且在每个 CDN 节点中都有文件的副本
  * 告示文件提供了不同块的 URL
  * 在客户端周期性检查到服务端的带宽， 向告示文件请求块的 URL, 根据带宽选择最大的比特率
    * 在不同的时间可以选择不同的比特率和不同的服务器

内容分发网络（CDN） 管理多个地理位置上的服务器， 在服务器上存储有视频的副本。
* 深入
* 邀请做客

## 套接字编程
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220429235130.png" width="70%"/></div>套接字是应用层和传输层之间的唯一 API。 

运输层提供两种 Socket 服务：
* TCP: 可靠的、面向字符串流的服务
* UDP: 不可靠的数据报

### 使用 UDP 套接字编程
UDP： 客户端和服务器之间没有"连接"
* 在发送数据之前没有握手
* 发送端明确地将目标IP地址

//todo UDP 套接字编程

# 运输层
## OverView
- [ ] 理解运输层服务背后的原理：
  - [ ] 多路复用与多路分解
  - [ ] 可靠的数据传输
  - [ ] 流控制
  - [ ] 拥堵控制
- [ ] 理解网络传输层协议

运输层协议为不同主机上的应用**进程之间**提供了**逻辑通信**，不同进程的主机好像直接连接在一起。
* 网络层： host 之间的逻辑通信
* 运输层： 进程之间的逻辑通信

* TCP : Transmission Control Protocol
  * 可靠的、按序地传输
  * 拥堵控制
  * 流控制
  * 需要初始化连接
* UDP : User Datagram Protocol
  * 不可靠的、无序地传输
  * 尽力而为的网络服务的基础拓展
* 它们都不提供下面的服务
  * 延迟保证
  * 带宽保证