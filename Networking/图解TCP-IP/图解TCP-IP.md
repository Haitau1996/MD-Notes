# 图解 TCP/IP
## Chap 1: 网络基础知识

起初计算机以单机模式被广泛应用, 随着计算机的发展, 人们将一个个计算机连在一起形成一个计算机网络, 根据其规模可以氛围 WAN 和 LAN (Wide/Local Area Network).
![](https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210416110228.png)<br>
![](https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210416110314.png)<br>

计算机网络发展的 7 个阶段,互联网是由许多独立发展的网络通信技术融合而成, 能够使他们不断融合并实现统一的正是 TCP/IP 技术
![](https://gitee.com/Haitau1996/picture-hosting/raw/master/img/计算机与网络的发展阶段.png)<br>

简单而言, 协议就是**计算机与计算机之间通过网络实现通信时事先达成的一种约定**,计算机从物理连接层面到应用程序软件层面, 各个组件都必须严格遵循者事先达成的约定才能实现真正的通信.分组交换就是将大的数据块分割成为一个个叫做包的较小单位进行传输的办法. <br>
![](https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210416135341.png)<br>

### 协议分层与OSI参考模型
OSI 模型将通信协议中必要功能分成了7层, 通过这些分层, 那些比较复杂的网络协议更加简单化. **其中每一层都接收来自下一层提供的特定服务, 并且负责为自己的上一层提供特定的服务**, 使得比较复杂的网络协议更加简单化.
