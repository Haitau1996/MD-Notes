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
  * VSCode <div align=center><img src="https://i.imgur.com/BPaAk8K.png" width="70%"/></div>
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
* 为了匹配特定位置的文本， 可以使用**定位符**：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220402220010.png" width="30%"/></div>

## Chap 10: 创建计算字段
很多时候，**存储在表中的数据都不是应用程序所需要的**，我们需要直接从数据库中检索出转换、计算或格式化过的数据， 而不是在客户机中重新格式化。
>但一般来说，在数据库服务器上完成这些操作比在客户机中完成要快得多，因为DBMS是设计来快速有效地完成这种处理的。

* **拼接字段**:将值联结到一起构成单个值,在 `SELECT` 语句中使用 `CONCAT` 函数拼接串，即把多个串连接起来形成一个较长的串。
  * **Trim 函数** MySQL支持`RTrim()` （去掉串右边的空格），`LTrim()`（去掉串左边的空格）以及`Trim()` （去掉串左右两边的空格）。
    ```sql
    SELECT CONCAT(prod_name, '->', prod_desc)
    FROM products
    ORDER BY prod_name;
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403112148.png" width="80%"/></div>
* `AS` 关键字赋予别名(alias),别名是一个字段或值的替换名。
* SQL 还支持对检索出的数据做算术运算， 支持 `+`,`-`,`*`,`/`
    ```sql
    SELECT prod_name, prod_price * 0.8 AS discount 
    FROM products
    ORDER BY prod_name;
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403113351.png" width="50%"/></div>

## Chap 11: 使用数据处理函数
>相对来说，多数SQL语句是可移植的,而函数的可移植性却不强,几乎每种主要的 DBMS 的实现都**支持其他实现不支持的函数，而且有时差异还很大**,因此使用函数，应该保证做好代码注释。

大多数 DBMS 支持以下几种数据处理函数：
1. 文本处理函数，需要特殊说明 SOUNDEX， 它是一个将任何文本串转换为描述其语音表示的字母数字模式的算法。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403193805.png" width="70%"/></div>其他的常用文本处理函数如下：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403193321.png" width="30%"/></div>
2. 日期和时间处理函数，在涉及日期的时候使用函数调用（如`Date(order_date) = '2005-09-01'`) 比直接判断(`order_date = '2005-09-01'`)要好,因为存储的项除了日期可能还包含时间值<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403194007.png" width="60%"/></div>
3. 数值处理函数(在主要的 DBMS 函数中最统一的部分)<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403194647.png" width="70%"/></div>

## Chap 12: 汇总数据
我们经常需要**汇总数据而不用把它们实际检索出来**，为此 MySQL 提供了专门的函数。常见的汇总检索有：
* 确定表中行数（或者满足某个条件或包含某个特定值的行数）。
* 获得表中行组的和。
* 找出表列（或所有行或某些特定的行）的最大值、最小值和平均值。

为此， MySQL 提供了以下几种函数：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220403200919.png" width="70%"/></div>

* `AVG()` 函数，计算表中列的平均值, 也可以限定特定的范围：
    ```sql
    SELECT AVG(prod_price) AS avg_price
    FROM products
    WHERE vend_id = 1003;
    ```
* `COUNT()` 函数进行计数,也可以设置条件。 它有两种使用的方式：
  * 使用 `COUNT(*)` 对表中行的数目进行计数，不管表列中包含的是空值（NULL）还是非空值。
  * 使用 `COUNT(column)` 对特定列中具有值的行进行计数，**忽略 NULL 值**。
    ```sql
    SELECT COUNT(*) AS num_cust
    FROM customers;
    ```
* `MAX()` 和 `MIN()` 函数，计算表中列的最大值和最小值, 它不但用于数值类型， 也可以用于文本类型， 并且忽略为 `NULL` 的行。
    ```sql
    SELECT MAX(prod_price) AS max_price
    FROM products
    WHERE vend_id = 1003;
    ```
* `SUM()` 函数，计算表中列的和，也可以用来处理计算值， 如 `SELECT SUM(item_price*quantity) AS total_price`, 它也会忽略 `NULL`。
* 聚集不同的值： 使用 `DISTINCT` 关键字，`DISTINCT` 必须使用列名，不能用于计算或表达式。
* 组合聚集函数：`SELECT` 语句可根据需要包含多个聚集函数。 
    ```sql
    SELECT COUNT(*) AS num_items,
       MIN(prod_price) AS price_min,
       MAX(prod_price) AS price_max,
       AVG(prod_price) AS price_avg
    FROM products;
    ```
## Chap 13: 分组数据
分组允许把数据分为多个逻辑组，以便能对每个组进行聚集计算。
* 使用 `GROUP BY` 子句建立分组
    ```sql
    SELECT vend_id, COUNT(*) AS num_prods
    FROM products
    GROUP BY vend_id;
    ```
    * `GROUP BY` 子句可以包含任意数目的列: 方便进行分组嵌套。
    * `GROUP BY` 子句必须出现在 `WHERE` 子句之后，`ORDER BY` 子句之前。
    * 列中有多行 `NULL` 值， 他们将分为一组。
    * 除聚集计算语句外，`SELECT` 语句中的每个列都必须在 `GROUP BY` 子句中给出, 如上面的 vend_id 
* 使用 `HAVING` 过滤分组：`WHERE` 过滤指定的是行而不是分组, 而 `HAVING` 过滤分组,并且**HAVING 支持所有WHERE 操作符**。区别在于**WHERE 在数据分组前进行过滤，HAVING 在数据分组后进行过滤**。
    ```sql
    SELECT cust_id, COUNT(*) AS orders
    FROM orders
    GROUP BY cust_id
    HAVING COUNT(*) >= 2;

    SELECT vend_id, COUNT(*) AS num_prods
    FROM products
    WHERE prod_price >= 10
    GROUP BY vend_id
    HAVING COUNT(*) >= 2;
    ```
* 分组 VS 排序<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220404112233.png" width="80%"/></div>
* `SELECT` 语句的顺序：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220404112338.png" width="50%"/></div>

## Chap 14: 使用子查询
**查询（query）** 任何SQL语句都是查询，但此术语一般指SELECT 语句。SQL还允许创建**子查询** （subquery），即嵌套在其他查询中的查询。
### 利用子查询进行过滤
如果我们需要列出订购物品 TNT2 的所有客户， 而客户信息和订购信息在不同的表中：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220404143145.png" width="30%"/></div>

这时候一个符合直觉的思路就是分成不同的步骤：
1. 检索包含物品TNT2的所有订单的编号
2. 检索具有前一步骤列出的订单编号的所有客户的ID
3. 检索前一步骤返回的所有客户ID的客户信息
    ```sql
    # step 1: 
    SELECT order_num
    FROM orderitems
    WHERE prod_id = 'TNT2';
    # step 2:
    SELECT cust_id
    FROM orders
    WHERE order_num IN (20005,20007);
    # step 3:
    SELECT cust_name, cust_contact
    FROM customers
    WHERE cust_id IN (10001,10004);
    ```

把把第一个查询变为子查询组合两个查询,在把前两个查询作为子查询组合进第三个查询：
```sql
SELECT cust_name, cust_contact
FROM customers
WHERE cust_id IN (SELECT cust_id
                  FROM orders
                  WHERE order_num IN (SELECT order_num
                                      FROM orderitems
                                      WHERE prod_id = 'TNT2'));
```
虽然子查询一般与IN 操作符结合使用，但也可以用于测试等于（`=`）、不等于（`<`）等。
### 作为计算字段使用的子查询
使用子查询的另一方法是创建计算字段， 如计算每个客户的订单总数，可以分成两步：
1. 从 customers 表中检索客户列表。
2. 对于检索出的每个客户，统计其在 orders 表中的订单数目。
    ```sql
    SELECT  cust_name,
            cust_state,
            (SELECT COUNT(*)
            FROM orders
            WHERE orders.cust_id = customers.cust_id) AS orders
    FROM customers
    ORDER BY cust_name;
    ```

上面这种涉及外部查询的子查询被称为**相关子查询**（correlatedsubquery），这时候必须注意限制有歧义性的列名。

## Chap 15: 联结表
关系表的设计就是要保证**把信息分解成多个表，一类数据一个表**，各表通过某些常用的值（即关系设计中的关系，relational）互相关联。
* 外键（foreignkey） 外键为某个表中的一列，它包含另一个表的主键值，定义了两个表之间的关系。
* 可伸缩性（scale） 能够适应不断增加的工作量而不失败

数据存储在多个表中， 联结允许用单条 `select` 语句检索出数据：
* **联结**是一种机制，用来在一条 `SELECT` 语句中关联表
* 联结不是物理实体, 但是简历联结后， SQL 会维护引用的完整性

### 创建联结
```sql
SELECT vend_name, prod_name, prod_price
FROM vendors, products
WHERE vendors.vend_id = products.vend_id
ORDER BY vend_name, prod_name;
```
* 最大的区别在于**指定的列并不完全在同一个表中**
* 在一条 `SELECT` 语句中联结几个表时，**相应的关系是在运行中构造的**, `WHERE` 子句就是配对的过滤条件， 使得 `SELECT` 只包含符合联结条件的行：
  * **应该保证所有联结都有 WHERE 子句**，否则MySQL将返回比想要的数据多得多的数据(笛卡尔积)
* 上面的联结称为等值联结(equal join， 也叫内部联结),可以使用稍微不同的语法指定(ANSI SQL规范首选INNER JOIN 语法)
    ```sql
    SELECT vend_name, prod_name, prod_price
    FROM vendors INNER JOIN products
    ON vendors.vend_id = products.vend_id;
    ```
* SQL对一条 `SELECT` 语句中可以联结的表的数目没有限制:
    ```sql
    SELECT prod_name, vend_name, prod_price, quantity
    FROM orderitems, products, vendors
    WHERE   products.vend_id = vendors.vend_id
            AND orderitems.prod_id = products.prod_id
            AND order_num = 20005;
    ```
## Chap 16: 创建高级联结
* **使用表别名**: 在之前我们可以为列输出定义别名， 而为了缩短 SQL 语句， 并且在单条 `SELECT` 语句中多次使用相同的表， **SQL 允许给表起别名**：
    ```sql
    SELECT cust_name, cust_contact
    FROM customers AS c, orders AS o, orderitems AS oi
    WHERE c.cust_id = o.cust_id
        AND oi.order_num = o.order_num
        AND prod_id = 'TNT2';
    ```
    * **表别名只在查询执行中使用, 并没有返回到客户端**

### 其他联结类型
在上章讨论了等值联结， 从两列的笛卡尔积中筛选出很小的部分， 这里讨论其他三种联结：**自联结，自然联结和外部联结**。
* **自联结**：单条 `SELECT` 语句中不止一次引用相同的表
    ```sql
    # 使用子查询找出 DTNTR 的供应商，列出其他物品
    SELECT prod_id, prod_name
    FROM products
    WHERE vend_id = (SELECT vend_id
                    FROM products
                    WHERE prod_id = 'DTNTR');
    # 自联结实现
    SELECT p1.prod_id, p1.prod_name
    FROM products AS p1, products AS p2
    WHERE p1.vend_id = p2.vend_id
        AND p2.prod_id = 'DTNTR';
    ```
    * 这时候在 自联结中使用别名是为了**避免 MySQL 不知道查询的是哪一列**
* **自然联结**：单条 `SELECT` 语句中引用相同的表，标准的联结返回所有数据，甚至相同的列多次出现。自然联结 **排除多次出现，使每个列只返回一次**。系统不会自动排除重复的列， 我们必须自己手动实现,下面就是一个非自然联结的例子:<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220407224352.png" width="30%"/></div>
    ```sql
    # 等值联结， 非自然
    SELECT c.cust_id, o.cust_id
    FROM customers AS c, orders as o
    WHERE c.cust_id = o.cust_id;
    # 自然联结
    SELECT c.cust_id
    FROM customers AS c, orders as o
    WHERE c.cust_id = o.cust_id;
    ```
* **外部联结**：许多联结将一个表中的行与另一个表中的行相关联，但有时候会需要包含没有关联行的那些行。
    ```sql
    # inner join: 检索所有客户以及其订单
    SELECT customers.cust_id, orders.order_num
    FROM customers INNER JOIN orders
        ON customers.cust_id = orders.cust_id;
    # outer join: 检索所有客户以及其订单，包括没有订单的客户
    SELECT customers.cust_id, orders.order_num
    FROM customers LEFT OUTER JOIN orders
        ON customers.cust_id = orders.cust_id;
    ```
    <div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220407231432.png" width="50%"/></div>

### 聚集函数与联结
实际上， 聚集函数也可以和联结配合使用， 如检索每个客户的订单数：
```sql
SELECT customers.cust_name,
       customers.cust_id,
       COUNT(orders.order_num) AS num_ord
FROM customers INNER JOIN orders
ON customers.cust_id = orders.cust_id
GROUP BY customers.cust_id;
```

## Chap 17: 组合查询
**组合查询**：MySQL也允许执行多个查询（多条SELECT 语句），并将结果作为单个查询结果集返回。基本有两种使用情况：
* 在单个查询中从不同的表返回类似结构的数据；
* 对单个表执行多个查询，按单个查询返回数据。

MySQL 中使用 `UNION` 操作符来组合数条 SQL 查询，例如我们可以使用两条 `SELECT` 分别检索两种条件,使用 `UNION` 可以组合符合两个条件的结果：
```sql 
# 普通的 SELECT 操作
SELECT vend_id, prod_id, prod_price
FROM products
WHERE prod_price <= 5
    OR vend_id IN (1001,1002);
# 使用 Union 操作符
SELECT vend_id, prod_id, prod_price
FROM products
WHERE prod_price <= 5
UNION
SELECT vend_id, prod_id, prod_price
FROM products
WHERE vend_id IN (1001,1002);
```

UNION 的规则：
* `UNION` 必须由两条或两条以上的SELECT 语句组成，语句之间用关键字 `UNION` 分隔
* `UNION` 中的每个查询必须包含相同的列、表达式或聚集函数
* 列数据类型必须兼容

**UNION 默认会去除重复行， 而更改默认行为可以使用 `ALL` 关键字**。

对组合查询的结果进行排序： 智能使用一条 `ORDER BY`  语句， 并且出现在最后一个 `SELECT` 语句中。因为**结果集，不存在用一种方式排序一部分，而又用另一种方式排序另一部分的情况**。

## Chap 18: 全文本搜索
MySQL 两个最常使用的引擎为 MyISAM 和 InnoDB，前者支持全文本搜索，而后者不支持。  
为了进行全文本搜索，必须索引被搜索的列，而且要随着数据的改变不断地重新索引。在索引之后， 在 `SELECT` 语句中可与 `Match()` 和 `Against()` 函数配合进行搜索。  
* 启用全文搜索支持： 一般是在创建表的时候启用,这里给 note_id 进行索引
    ```sql
    CREATE TABLE productnotes
    (
        note_id    int           NOT NULL AUTO_INCREMENT,
        prod_id    char(10)      NOT NULL,
        note_date datetime       NOT NULL,
        note_text  text          NULL ,
        PRIMARY KEY(note_id),
        FULLTEXT(note_text)
    ) ENGINE=MyISAM;
    ```
* 进行全文搜索：Match() 指定被搜索的列，Against() 指定要使用的搜索表达式。
    ```sql
    SELECT note_text
    FROM productnotes
    WHERE Match(note_text) Against('rabbit');
    ```
    * 如果单独列出他们返回的结果：发现 match 失败之后返回的是0， 成功时候返回的 rk 等级由MySQL根据行中词的数目、唯一词的数目、整个索引中词的总数以及包含该词的行的数目计算出来。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220409095708.png" width="90%"/></div>
* 使用查询扩展：第一行是包含 _anvils_ 的， 后面是包含第一行的词
    ```sql
    SELECT note_text
    FROM productnotes
    WHERE Match(note_text) Against('anvils' WITH QUERY EXPANSION);
    ```
* **使用布尔搜索**（即使没有 FULLTEXT 索引也可以使用）： 可以提供更多细节， 要匹配的词，要排斥的词（如果某行包含这个词，则不返回该行，即使它包含其他指定的词也是如此），排列提示，表达式分组等。<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220409100826.png" width="70%"/></div>
    ```sql
    SELECT note_text
    FROM productnotes
    WHERE Match(note_text) 
        Against('+safe +(<combination)' IN BOOLEANMODE);
    ```

## Chap 19: 插入数据
`INSERT` 是用来插入（或添加）行到数据库表的,包括插入完整的行，插入行的一部分，插入多行 和 插入某些查询的结果。  
* 插入完整的行， 可以使用默认的顺序和**指定列顺序**（优先使用， 默认的顺序不安全）：
    ```SQL
    INSERT INTO Customers
    VALUES(NULL,'Pep E. LaPew','100 Main Street','Los Angeles','CA','90046','USA',NULL,NULL);
    # 指定列顺序
    INSERT INTO customers(cust_name,cust_address,cust_city,cust_state,
                          cust_zip,cust_country,cust_contact,cust_email)
    VALUES('Pep E. LaPew','100 Main Street','Los Angeles','CA','90046','USA',NULL,NULL);
    ```
* 使用括号分割插入多行数据：
    ```sql
    INSERT INTO customers(cust_name,cust_address,cust_city,cust_state,cust_zip,cust_country)
    VALUES(
        'Pep E. LaPew',
        '100 Main Street',
        'Los Angeles',
        'CA',
        '90046',
        'USA'
    ),
    (
        'M. Martian',
        '42 Galaxy Way',
        'New York',
        'NY',
        '11213',
        'USA'
    );
    ```
* `INSERT` 可以和 `SELECT` 配合使用

## Chap 20: 更新和删除数据
使用 `UPDATE` 和 `DELETE` 更新和删除数据。
### 更新数据
`UPDATE` 包含三个部分：**需要更新的表， 列名和新值， 过滤语句**。需要主义， 更新的时候**不要省略WHERE 子句**， 否则会更新表中的所有行。
```sql
UPDATE customers
SET cust_email = 'elmer@fudd.com'
WHERE cust_id = 10005;
# 更新多个列， 只需要使用单个 set 指令
UPDATE customers
SET cust_name = 'The Fudds',
    cust_email = 'elmer@fudd.com'
WHERE cust_id = 10005;
```
使用 `IGNORE` 使得及时发生错误， 也会继续操作。 而默认的行为是， 一行或者多行出现作物， 整个更新操作都会被取消。

### 删除数据
`DELETE FROM` 要求指定从中删除数据的表名，`WHERE` 子句过滤要删除的行。
```sql
DELETE FROM customers
WHERE cust_id = 10006;
```
如果想从表中删除所有行，不要使用DELETE 。可使用TRUNCATE TABLE 语句，它完成相同的工作，但速度更快。

### 使用准则
省略了WHERE 子句，则UPDATE 或DELETE 将被应用到表中所有的行， 非常容易出错， 因此要遵循一些准则：
* 除非确实打算更新和删除每一行，否则绝对不要使用不带 `WHERE` 子句的 `UPDATE` 或 `DELETE` 语句。
* 保证每个表都有主键, 尽可能像 `WHERE` 子句那样使用它（可以指定各主键、多个值或值的范围）
* 在对 `UPDATE` 或 `DELETE` 语句使用 `WHERE` 子句前，应该先用 `SELECT` 进行测试
* 使用强制实施[引用完整性的数据库](http://www.nowamagic.net/librarys/veda/detail/907)，这样MySQL将不允许删除具有与其他表相关联的数据的行。

## Chap 21: 创建和操纵表
* 表创建：使用 `CREATE TABLE` 语句， 后面跟着新表的名字， 表列的名字和定义放在圆括号中并且使用逗号隔开。
    ```sql
    CREATE TABLE customers
    (
        cust_id      int       NOT NULL AUTO_INCREMENT,
        cust_name    char(50)  NOT NULL ,
        cust_address char(50)  NULL ,
        cust_city    char(50)  NULL ,
        cust_state   char(5)   NULL ,
        cust_zip     char(10)  NULL ,
        cust_country char(50)  NULL ,
        cust_contact char(50)  NULL ,
        cust_email   char(255) NULL ,
        PRIMARY KEY (cust_id)
    ) ENGINE=InnoDB;
    ```
    * 表的主键使用 `PRIMARY KEY` 指定
      * 主键可以由多个列一起组成: `PRIMARY KEY (order_num, order_item)`
    * 允许 NULL 值意味着允许插入行时不给出该列的值 
    * 用户 id 这种编号除了唯一外没有特殊的意义， 使用的最简单的编号是下一个编号， 而使用 `SELECT` 语句得出最大值并不可靠（SELECT 和 INSERT 两条语句之间可能有别的插入行）， 这时候就可以使用 `AUTO_INCREMENT`, 并且可以获取这个值：
        ```sql
        SELECT last_insert_id();
        ```
    * `DEFAULT` 关键字可以指定行的默认值
* 但MySQL与其他DBMS不一样，它具有多种引擎， 它们具有各自不同的功能和特性，为不同的任务选择正确的引擎能获得良好的功能和灵活性。引擎类型可以混用， 但是**外键不能跨引擎**。
* `ALTER TABLE` 语句可以更新表。但是，理想状态下，当表中存储数据以后，该表就不应该再被更新。
  * 添加列
    ```sql
    ALTER TABLE vendors
    ADD vend_phone CHAR(20);
    ```
  * 删除列
    ```sql
    ALTER TABLE Vendors
    DROP COLUMN vend_phone;
    ```
  * 定义外键
* `DROP TABLE` 语句删除列
* `RENAME TABLE old_name TO new_name` 重命名表

## Chap 22: 使用视图
**视图是虚拟的表**,在视图创建之后，可以用与表基本相同的方式利用它们。可以把整个查询包装成一个虚拟表。它有很多常见的使用场景：
* 重用SQL语句
* 简化复杂的SQL操作。在编写查询后，可以方便地重用它而不必知道它的基本查询细节。
* 使用表的组成部分而不是整个表。
* 保护数据：可以给用户授予表的特定部分的访问权限而不是整个表的访问权限。
* 更改数据格式和表示： 视图可返回与底层表的表示和格式不同的数据。

因为视图不包含数据，所以每次使用视图时，都必须处理查询执行时所需的任一个检索,添加或者更改数据之后， 视图**将返回改变过后的数据**。  
视图在使用的时候有一些限制：
* 视图可以嵌套，即可以利用从其他视图中检索数据的查询来构造一个视图。但是不能索引， 也不能触发关联的触发器或者默认值。
* 视图可以和表一起使用。

### 使用视图
* 视图使用 `CREATE VIEW` 创建
  * 更新可以先 `DROP` 再 `CREATE`, 也可以使用 `CREATE OR REPLACE VIEW`
* 使用 `SHOW CREATE VIEW view_name;` 查看视图的定义
* 使用 `DROP VIEW view_name;` 删除视图

#### Case 1: 简化联结
视图最常见的应用是隐藏复杂的 `SQL`, 这通常涉及联结：
```sql
# 创建视图
CREATE VIEW productcustomers AS
SELECT cust_name, cust_contact, prod_id
FROM customers, orders, orderitems
WHERE customers.cust_id = orders.cust_id
    AND orderitems.order_num = orders.order_num;
# 使用 SELECT 查询
SELECT cust_name, cust_contact
FROM productcustomers
WHERE prod_id = 'TNT2';
```
#### Case 2: 重新格式化检索出的数据
视图的另一常见用途是重新格式化检索出的数据， 如果我们经常要某个格式化的数据， 就可以创建一个视图， 然后在需要的时候使用它：
```sql
CREATE VIEW vendorlocations AS
SELECT Concat(RTrim(vend_name), ' (', RTrim(vend_country), ')')
       AS vend_title
FROM vendors
ORDER BY vend_name;
## 使用
SELECT *
FROM vendorlocations;
```
#### Case 3: 使用视图过滤数据
如果我们经常需要过滤没有电子邮件的客户， 这时候就可以创建一个视图，其中包含有电子邮件的客户的查询：
```sql
CREATE VIEW customeremaillist AS
SELECT cust_id, cust_name, cust_email
FROM customers
WHERE cust_email IS NOT NULL;
# 使用
SELECT *
FROM customeremaillist;
```
#### Case 4: 使用视图与计算字段
视图对于简化计算字段的使用特别有用：
```sql
CREATE VIEW orderitemsexpanded AS
SELECT order_num,
       prod_id,
       quantity,
       item_price,
       quantity*item_price AS expanded_price
FROM orderitems;
# 使用
SELECT *
FROM orderitemsexpanded
WHERE order_num = 20005;
```
### 更新视图
通常，视图是可更新的， 并非所有视图都是可更新的。基本上可以说，如果MySQL不能正确地确定被更新的基数据，则不允许更新（包括插入和删除）。如果视图中有以下操作， 则不能进行视图更新：
1. 分组(使用 GROUP BY 和 HAVING)
2. 联结
3. 子查询
4. 并
5. 聚集函数
6. DISTINCT
7. 导出(计算)列