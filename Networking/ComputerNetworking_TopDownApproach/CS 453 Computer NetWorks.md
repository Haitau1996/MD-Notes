# CS 453 Computer Networks
by Jim Kurose

# Introduction
- [ ] 什么是因特网， 什么是协议
- [ ] **网络边缘**， 接入互联网， 物理媒介
- [ ] **网络核心**： 分组交换， 电路交换， 网络结构
- [ ] 性能：丢失， 延迟， 吞吐量
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
