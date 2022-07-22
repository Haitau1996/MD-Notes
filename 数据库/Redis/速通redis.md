## 数据结构
Redis 是内存中的键值对的数据存储系统， 因此这里的数据类型指的是 value 的类型， key 默认都是字符串。
### 字符串
Redis 使用 C 语言编写， 但是 C 的字符串有很多缺点：
1. O(n) 时间复杂度获取长度
2. 没有好的扩容机制
3. 特殊的字符无法处理

Redis 3.0 之后的简单动态字符串， 根据单个字符的长度分成 sdshdr8/16/32/64：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212258475.png" width="70%"/></div> 
* 获取长度的时间复杂度为 O(1)
* 减少因为扩容引起的数据搬运次数
* 可以存储更加复杂的二进制数据

### 链表
Redis 链表底层实现是双向链表：
```C
typedef struct listNode{
    struct listNode *prev;
    struct listNode *next;
    void* value;
} listNode;
typedef struct list{
    listNode* head;
    listNode* tail;
    unsigned long len;
    void *(*dup)(void *ptr); // 节点值复制函数
    void (*free)(void *ptr); // 节点值释放函数
    void (*match)(void *ptr, void *key); // 节点值对比函数
};
```
常见的操作包括链表左/右增加、删除元素， 获取链表长度， 按索引范围获得值。  

### 哈希表
哈希表中 key-value 是一一对应关系， 可以在 O(1) 时间复杂度下获取 key 对应的 value, C 语言没有内置哈希表， redis 用拉链法作为哈希表的实现。 
1. dictht: 单纯表示一个哈希表
2. dictEntry: 哈希表的一项， 可以看做就是一个键值对
3. dict: 给外层调用的哈希表结构， 包含两个 dictht

其中 dictht 的数据结构如下<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212315912.png" width="70%"/></div>
* 使用一个 sizemask 是让大于 size 的数字和他做 & 操作之后再取位置就不再会溢出
* size 通常为 2的 n 次方
* used 指的是元素的个数， 可以用于计算负载因子

最基本的单元：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212318485.png" width="40%"/></div>
dict 在外界提供的接口， 里面放了ht两个 dictht<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212323236.png" width="40%"/></div>
* 通常 ht[1] 是没有数据的， 在负载因子过大的时候做 rehash 的时候， ht[1] 放新表， 结束后再让 ht[0] 指向它。
* rehashidx 表示已经完成了第几个元素的搬运
* 如果 redis 不做后台备份， 负载因子大于等于 1 就做 rehash, 否则大于等于 5 才执行

### 集合
* 普通集合： 就是对于哈希表的封装
* 整数集合： <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212326655.png" width="70%"/></div> 里面的数据是有序的， 查找是二分查找

### 有序集合
跳表， 参考二叉树， 提高索引的效率：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212329661.png" width="70%"/></div><div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207212337933.png" width="98%"/></div>

## 持久化
Redis 是内存数据库， 有较快的读取速度， 但是数据容易丢失， 断电后内存数据就会小时。持久化就是将内存的数据写入硬盘中, 保证断电后的非易失性。  
Redis 中有两种常见做法： RDB(将所有数据写入硬盘)和 AOF(将数据修改命令写入硬盘)
### RDB(Redis Data Base)
把目前内存中的数据生成一个快照（RDB 文件）保存在硬盘中，是一个全量备份。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207221550129.png" width="80%"/></div>  
各种数据结构在 RDB 文件中的结构如下：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207221552656.png" width="70%"/></div>  
RDB 触发的条件：
* 手动触发
  * save: 主先测定执行 rdbSave 函数， 服务器进程阻塞
  * bgsave: fork 一个子进程， 由子进程去执行 rdbSave 函数
* 自动触发： 配置文件写入 save m n, 代表 m 秒内发生 n 次变化的时候会自动执行 bgsave

### AOF
AOF 记录之后所有对 Redis 数据进行修改的操作， 如果发生事故， Redis 可以通过 AOF 文件将数据修改命令全部执行一遍以恢复数据， 是一种增量备份。  
随着时间的进程， AOF 文件会越来越大恢复起来就会变慢， 可以根据数据库状态生成新的 AOF， 再删掉老的 AOF 文件。  
AOF 的触发条件：
* 手动触发： bgrewriteaof
* 自动触发： 配置文件设置 appendonly yes 开启
    * 还要配置写汇策略： Always/Everysec(默认)/No
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/202207221602121.png" width="70%"/></div>

