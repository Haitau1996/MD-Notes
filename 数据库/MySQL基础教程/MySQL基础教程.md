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
* `CREATE DATABASE`**创建数据库**：
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
* `USE db_name` 用于**切换数据库**
  * 成功显示 Database changed
  * 在使用其他数据库的时候， 依旧可以使用 `db_name.table_name` 来操作
* `CREATE TABLE table_name` **创建表**
    ```sql
    CREATE TABLE 表名( 列名1 数据类型1，列名2 数据类型2…);
    ```
    * 列名和数据类型之间用空格分开
    * 创建表的时候加入 `CHARSET=utf8` 可以指定字符串编码
    * 数据库名、表名和列名可以使用 ``(反引号)括起来使用， 而**输入表中的字符串值需要使用单引号或者双引号括起来**
* 确认表的结构， 使用 `DESCRIBE table_name` or 简写`DESC` 
* 向表中插入数据 
    ```sql
    # 按照设置好的列顺序
    INSERT INTO 表名 VALUES( 数据1, 数据2…);
    # 指定列名来插入记录
    INSERT INTO 表名(列名1, 列名2, …) VALUES(值1, 值2, …);
    # 一次性输入多行数据
    INSERT INTO 表名 ( 列名1, 列名2…) VALUES 
    ->( 数据1, 数据2…),
    ->( 数据1, 数据2…),
    ->( 数据1, 数据2…)…;
    ```
* `SELECT` 可以显示选中列的数据， 还可以用于显示与数据库无关的值

## Chap 5:  数据类型和数据输入
数据库中的表中只能输入各个列指定格式的数据， 数据类型可以降低错误数据输入的可能性, MySQL 数据输入检查相对宽松。
* 数值类型：<div align=center><img src="https://i.imgur.com/xlwLUJ9.png" width="70%"/></div>
  * 数值类型可以加正负号以及科学输入法
* 字符串类型：<div align=center><img src="https://i.imgur.com/QSKVUFh.png" width="70%"/></div>
  * 在 4.0 以前 VARCHAR/ CHAR 的位数单位为字节， 4.1 之后变成了字符数
  * 字符串数据在输入的时候要使用单括号或者双括号括起来， 实际的 `'` 则需要转义
* 日期与时间：<div align=center><img src="https://i.imgur.com/owQa6wk.png" width="70%"/></div>