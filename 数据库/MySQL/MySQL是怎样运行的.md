# MySQL 是怎样运行的 - 从根上理解 MySQL
读书笔记
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
3. 同一台类 Unix 系统中可以使用 **Unix域套接字**, 服务器默认监听 `/tmp/mysql.sock`， 也可自行指定

### 服务器处理客户请求
不管连接方式， 最后效果都是客户端进程向服务器发送一段文本（SQL语句）， 服务器处理后再向客户端返回一段文本（处理结果）。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220628152414.png" width="60%"/></div>  
#### 连接管理
当一个客户端连接到服务器进程时， 服务器会**创建一个进程专门处理与这个客户端的交互， 在客户端退出时候不会立即销毁**， 而是缓存起来分配给新的连接进来的客户端。  
客户端发起连接时会对主机信息、用户名、密码做验证， 认证失败会拒绝连接， 如果两者不在同一台计算机上， 还可以通过传输层安全协议(Transport Layer Security,TLS)进行加密。
#### 解析与优化
1. 查询缓存： MySQL 会把处理过的请求和查询结果缓存起来， **这个查询缓存可以在不同客户端之间共享**，缓存系统会监测涉及的每张表， 表的结构改变或者数据被修改则缓存失效。（因为开销问题， MySQL 8.0 将其删除）
2. 语法解析： 本质上是个编译问题， 不过多讨论
3. 查询优化： 如外连接转为内连接、表达式简化、子查询转为连接等， 优化的结果就是生成一个执行计划

#### 存储引擎
MySQL 服务器把数据存储和提取操作都封装到了存储引擎模块中， 表只是一个逻辑概念， 在物理上如何记录、怎么读取数据、怎么把数据写入到具体的存储器上都是存储引擎负责。不同的存储引擎管理的表可能有不同的数据结构， 采用的存取算法也不同。最常用的是 InnoDB(当前默认) 和 MyISAM(5.5.5 之前的默认)。  
* 创建时指定： 
    ```MySQL
    CREATE TABLE 表名(
        建表语句
    ) ENGINE=存储引擎名称;
    ```
* 修改表的引擎`ALTER TABLE 表名 ENGINE=存储引擎名`

## Chap 2: MySQL 控制按钮 - 启动项和系统变量
MySQL 的设置选项一般都有默认值， 可以在启动的时候修改这些默认值， 这些设置项被称为启动选项（start option）。
* 在命令行中指定： `--启动选项1[=值1] --启动选项2[=值2] ...`
* 在配置文件中使用选项： 配置文件启动选项被划分为若干个组， 每个组有一个组名<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220629145930.png" width="70%"/></div>
  * MySQL 按照一定的顺序依次读取各个配置文件(如果存在)，重复的选项以最后一个配置文件中的为准
  * 同一个配置文件的多个组中以最后一个出现的组的选项为准
  * 如果不想使用默认的配置文件， 可以使用 --defaults-file=/temp/myconfig.txt 手动指定

MySQL 服务程序在运行中会有很多影响程序行为的变量， 被称为系统变量。使用`SHOW VARIABLES [LIKE pattern]` 查看：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220629151818.png" width="60%"/></div>
设置系统变量：
1. 通过启动选项设置： 命令行、配置文件
2. 大部分系统变量， 值可以在服务器运行过程中动态修改
   1. 系统变量有不同的作用范围： 有的系统变量不是针对单个用户的，因此作用范围分两种
   2. GLOBAL(全局范围) 和 SESSION(对话范围)
   3. 设置和查看的时候也是 `SET [GLOBAL|SESSION] 系统变量名=值/ SHOW [GLOBAL|SESSION] VARIABLES LIKE`
3. 注意事项： 并不是所有的变量都具有 GLOBAL 和 SESSION 的作用范围， 有的只有前者、有的只有后者、有的兼而有之； 有的系统变量是只读的，无法修改

状态变量： 帮助我们更好地了解服务器的运行状态。通过 `SHOW [GLOBAL|SESSION] STATUS [LIKE 模式]` 查看<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220629153936.png" width="50%"/></div>

## Chap 3: 字符集和比较规则
**字符集**：字符与二进制数据的映射关系。简单的比较可以是二进制数据的直接比较， 如果有不区分大小写之类的规则， 可以转为大写或小写之后再比较。  
### MySQL 中支持的字符集和比较关系
UTF-8 是可边长的编码， 表示一个字符时候需要 1-4 个字节， 但是最常用的字符是 1-3 个字节就可以表示。MySQL 中的 utf8 是阉割到三个字节的字符集， 完整的名为 utf8mb4。
MySQL 比较规则的命名十分规律：
* 比较规则的名称都是以字符集开头
* 紧跟着就是规则应用的语言
* 然后后缀表示区分重音、大小写等

<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220705151453.png" width="70%"/></div>

