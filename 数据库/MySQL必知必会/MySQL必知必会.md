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
* Windows 系统中的使用
  * VSCode <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220331211321.png" width="60%"/></div>
    * 可能有[登陆的问题](https://github.com/formulahendry/vscode-mysql/issues/79)， 在 MySQL 命令行中运行`ALTER USER '<your_user>'@'localhost' IDENTIFIED WITH mysql_native_password BY '<your_password>';` 解决
  * 在 powershell 中可以使用命令行`mysql -u root -p<your_passwd> -h 127.0.0.1 -P <mysql_port_no>` 来连接数据库
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

## Chap 06&07: 过滤数据
只检索所需数据需要指定**搜索条件**（search criteria），搜索条件也称为**过滤条件**（filter condition），在 SELECT 语句中， 数据根据 WHERE 子狙中指定的搜索天剑进行过滤。
* 同时使用ORDER BY 和 WHERE 子句时，应该让 ORDER BY 位于 WHERE 之后
* WHERE 子句支持一系列的操作符， 可以支持单个值、范围和不匹配的检查：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220402143213.png" width="40%"/></div>
  * 一个列不包含值的时候被称为包含 空值（NULL）, 未知具有特殊的含义，数据库不知道它们是否匹配，所以在匹配过滤或不匹配过滤时不返回它们,**需要额外附加**。
    ```sql
    SELECT cust_id
    FROM customers
    WHERE cust_email IS NULL;
    ```

组合 WHERE 子句可以 创建复杂的搜索条件：
* `AND` 和 `OR` 子句，用于组合多个搜索条件
  * 不同的操作符有顺序的问题（SQL在处理 `OR` 操作符前，优先处理 `AND` 操作符）， 正确的做法是明确**使用括号来组合多个搜索条件**.
* `IN` 操作符用来指定条件范围, 取出由逗号分隔的清单，所有项目都括在圆括号中
  * `IN` 操作符一般比 `OR` 操作符清单执行更快。
  * `IN` 的最大优点是**可以包含其他 `SELECT` 语句**，使得能够更动态地建立 `WHERE` 子句
* `NOT` 操作符： `WHERE` 子句中的 `NOT` 操作符有且只有一个功能，那就是否定它之后所跟的任何条件。

## Chap 08: 使用通配符过滤
利用通配符可创建比较特定数据的搜索模式，几个相关的概念：
* **通配符**（wildcard） 用来匹配值的一部分的特殊字符。
* **搜索模式**（search pattern） 由字面值、通配符或两者组合构成的搜索条件。

使用 `LIKE` 子句实现通配符匹配：
1. `%` 匹配任何字符串，但是**不包括空字符串**：
    ```sql
    SELECT prod_id, prod_name
    FROM products
    WHERE prod_name LIKE 'jet%';
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220402161016.png" width="40%"/></div>
2. `_` 匹配任何**单个字符**，不包括空字符

使用通配符往往要比前两章讨论的搜索更慢，因此在使用时候需要注意：
1. 谨慎使用， 如果不使用通配符也能达到相同的目的， 优先使用其他操作
2. **除非绝对必要， 否则不要放到搜索模式的开始处**（放在后面对相对少的结果使用通配符总是效率高些）
3. 注意通配符的位置， 放错位置会导致搜索结果不正确

## Chap 09: 使用正则表达式进行搜索
正则表达式是用来匹配文本的特殊的串（字符集合），MySQL仅支持多数正则表达式实现的一个很小的子集。
* 基本字符匹配： LIKE 变成了 REGEXP, **LIKE 要求全部匹配**， 如 LIKE 1000 只匹配 1000， 而 **REGEXP 只要模式在字符串中出现就可以匹配**， 如 REGEXP 1000 可以匹配 JetPack 1000
    ```sql
    SELECT prod_name
    FROM products
    WHERE prod_name REGEXP '1000'
    ORDER BY prod_name;
    ```
* `.` 表示任何单个字符，如 `.000` 可以匹配 `JetPack 1000`/`JetPack 2000`
* `|` 用于正则表达式的或关系
    ```sql
    SELECT prod_name
    FROM products
    WHERE prod_name REGEXP '1000|2000'
    ORDER BY prod_name;
    ```
* `[]` 表示从几个字符之一进行匹配`WHERE prod_name REGEXP '[123] Ton'`
  * 可以使用 `-` 来简化范围：`[0-9]`
* 特殊字符应该使用 `\\` 作为前导符，如 `\\.` 可以匹配 `.`
* 有自定义的字符集，称为字符类：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220402215522.png" width="60%"/></div>
* 有时候需要对匹配的数目进行更强的控制：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220402215800.png" width="50%"/></div>
* 为了匹配特定位置的文本， 可以使用**定位符**：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220402220010.png" width="40%"/></div>