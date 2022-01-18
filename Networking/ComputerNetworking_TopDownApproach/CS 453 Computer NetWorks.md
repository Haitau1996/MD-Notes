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