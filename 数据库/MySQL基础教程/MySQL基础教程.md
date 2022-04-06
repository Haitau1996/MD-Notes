# MySQL 基础教程
# Part II: MySQL 基础知识
## Chap 3: MySQL 监视器
MySQL 监视器这个客户端程序以用于操作 MySQL 的 CUI 为基础：
* 启动监视器：
    ```shell
    mysql -u user_name -ppassword -h host_name -P port_number
    ```
    * `-p` 和 password 之间没有空格
    * 如果在 windows 下使用 cmd, 可以使用 `chcp 65001` 来设置编码为 utf-8
    * mysql 监视器中使用 `status` 查看服务端&客户端的编码状态

## Chap 4: 创建数据库
* `CREATE DATABASE`创建数据库：
    ```console
    mysql> CREATE DATABASE db_name;
    ```
    * 在 windows/macos 下数据库名和表名是不区分大小写的， 但是**在 Linux 下是区分的**
    * 执行成功显示 Query OK
* `SHOW`命令用于确认数据库信息
   ```console
   mysql> SHOW DATABASES;
   mysql> SHOW TABLES;
   ``` 
   * mysql 被默认创建， 存储 MySQL 各种的信息
* `USE db_name` 用于切换数据库
  * 成功显示 Database changed
* `CREATE TABLE table_name` 创建表
    ```console
    CREATE TABLE 表名( 列名1 数据类型1，列名2 数据类型2…);
    ```
    * 列名和数据类型之间用空格分开
    * 创建表的时候加入 `CHARSET=utf8` 可以指定字符串编码
    * 数据库名、表名和列名可以使用 ``(反引号)括起来使用， 而**输入表中的字符串值需要使用单引号或者双引号括起来**