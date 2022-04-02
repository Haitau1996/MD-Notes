# MySQL 必知必会
## Chap 01: 了解 SQL
* **数据库（database）**: 保存有组织的数据的容器（通常是一个文件或一组文件）。
* **表（table）**： 某种特定类型数据的结构化清单。
  * 通常存储在表中的数据是一种类型的数据或一个清单， 对于不同清单应该创建多个表， 否则检索和访问将十分困难
  * **模式（schema）** 关于数据库和表的布局及特性的信息
* **列（column）** 表中的一个字段,所有表都是由一个或多个列组成的。
  * 正确地将数据分解为多个列极为重要,把它分解开，才有可能**利用特定的列对数据进行排序和过滤**
  * 每个表列都有相应的数据类型，它限制（或容许）该列中存储的数据。
* **行（row）**: 表中的一个记录。
* **主键（primary key）**: 一列（或一组列），其值能够唯一区分表中每个行。
  * 表中的任何列都可以作为主键, 但是需要满足一定条件：
    1. 任意两行都不具有相同的主键值
    2. 每个行都必须具有一个主键值

## Chap 02&3: MySQL 简介
* 数据的所有存储、检索、管理和处理实际上是由数据库软件——DBMS（数据库管理系统）完成的。
* DBMS 总体分成两类：
  * 基于共享文件系统的DBMS
  * 基于客户机 —— 服务器的DBMS， 该系统分成两个部分
    1. 服务器部分： 负责所有数据访问和处理的一个软件，数据、数据添加、删除和数据更新的所有请求都由服务器软件完成。
    2. 客户机是与用户打交道的软件
* MySQL工具
  1. mysql命令行实用程序
  2. MySQL Administrator 图形交互客户机
  3. MySQL Query Browser
* 使用 USE 关键词选择数据库
  * 如果不存的话就需要先使用 CREATE DATABASE 命令创建数据库
    ```sql
    CREATE DATABASE IF NOT EXISTS crash_course;
    USE crash_course;
    ```
* VSCode 中使用 <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220331211321.png" width="60%"/></div>
  * 还有[登陆的问题](https://github.com/formulahendry/vscode-mysql/issues/79)， 在 MySQL 命令行中运行`ALTER USER '<your_user>'@'localhost' IDENTIFIED WITH mysql_native_password BY '<your_password>';` 解决
## Chap 04: 检索数据
* `SELECT` 语句： 一定要说明从 **哪里** 选择 **什么数据**
  * 检索单个列： `SELECT column_name FROM table_name`
  * 检索多个列： 中间用逗号分隔， `SELECT column_name1, column_name2, ... FROM table_name`
  * 使用 `DISTINCT` 关键字，可以检索不重复的值， `SELECT DISTINCT column_name FROM table_name`
  * 使用 `*` 可以做通配符匹配，相当于检索所有列
* 限制结果： 使用 `LIMIT` 子句
  * `SELECT prod_name FROM products LIMIT 5` 只检索前 5条记录
  * `SELECT prod_name FROM products LIMIT 5，5` 检索从第 5条记录开始的 5条记录
* 完全限定的名字：同时使用表名和列名 `SELECT crash_course.prod_name FROM crash_course.products`

## Chap 05: 排序检索数据
**如果不明确控制的话，不能（也不应该）依赖该排序顺序**, 子句（clause） SQL语句由子句构成，有些子句是必需的，而有的是可选的, 我们可以使用 ORDER BY 子句对输出进行排序：
* 用非检索的列排序数据是完全合法的
* 按多个列排序，只要指定列名，列名之间用逗号分开即可
* `DESC` 指定降序排序， `ASC`（默认） 指定升序排序
* `ORDER BY` 配合 `LIMIT` 子句，可以找出指定列的最大/小值