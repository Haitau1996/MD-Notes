# MySQL 是怎样运行的 - 从根上理解 MySQL
## Chap 1: 初识 MySQL
MySQL 的运行分为客户端和服务端
* 服务器程序直接和要存储的数据打交道， 多个客户端程序恶意连接到服务端程序
* 客户端程序向服务器发送增删查改请求， 服务端据此对存储的数据进行处理

启动 MySQL 服务器程序：
* 在类 unix 系统安装目录中的几个常用可执行文件/脚本
    * 执行 mysqld 可执行文件直接启动一个服务器进程
    * mysqld_safe 启动脚本间接调用 mysqld, 并且**持续监控服务器的运行状态**(进程出现错误时候帮助重启服务器程序， 另外将出错信息和其他诊断日志输出到错误日志)
    * mysql.server 脚本间接调用 mysqld_safe, 通过 start/stop 参数启动/关闭服务器程序
    * mysqld_multi 启动多个服务器实例， 本书不讨论
* 在 windows 中启动：手动启动 或者将它注册为服务， 然后通过 `net start/stop MySQL` 启动

启动客户端程序可以使用命令行， `mysql -hHostName -uUserName -pPassWord`， 断开可以使用 `quit/exit/\q`,可以启动多个客户端程序， 且**每个客户端程序都是互不影响的**。  
**客户端与服务器的连接**：
1. TCP/IP 数据库服务器进程和客户端进程运行在不同的主机中， 它们之间必须通过网络通信
2. 同一台 windows 主机中可以使用进程间通信，包括**命名通道**(服务器启动 --enable-named-pipe,客户端启动 --pipe)和**共享内存**(服务端 --shared-memory, 客户端 --protocol=memory)
3. 同一台类 Unix 系统中可以使用 **Unix域套接字**, 服务器默认监听 `/tmp/mysql.sock`， 也可执行指定

