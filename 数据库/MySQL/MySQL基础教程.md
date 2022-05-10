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
    * 在设置列的默认值时，需要给列加上 DEFAULT 关键字
        ```sql
        CREATE TABLE 表名 ( 列名 数据类型 DEFAULT 默认值…);
        ```
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

## Chap 6: 修改表
第四章中的 INSERT INTO 语句是修改表中的数据， 而这里使用的 `ALTER` 语句则是修改表的结构。
* 当修改列的定义时：`ALTER TABLE … MODIFY …`
  * 新旧类型必须具有兼容性
  * 也可用于修改列的顺序， 将某个列放到前面
    ```sql
    ALTER TABLE tb1C MODIFY birth DATETIME FIRST;
    ```
* 当添加列时：`ALTER TABLE … ADD …`
  * 可以使用 `FIRST` 或 `AFTER` 来指定添加列的位置
    ```sql
    ALTER TABLE tb1D ADD birth DATETIME FIRST;
    ALTER TABLE tb1E ADD birth DATETIME AFTER empid;
    ```
* 当修改列名和定义时：`ALTER TABLE … CHANGE …`
* 当删除列时：`ALTER TABLE … DROP …`

> 在英语中 modify 和 change 意思稍微有点区别， 前者的意思更多是 **调整，稍作修改使更加适合**， 后者含义更多是 **改变**。 所以前者只是更改列的数据类型， 而后者改变了名和定义

### 设置主键、自动编号
在创建了数据库之后， 需要从大量数据中**唯一确定**符合条件的记录， 这种“只会确定一个”的独一无二状态称为唯一（_unique_）。  
创建唯一记录， 会给列设置一个用于和其他列进行区分的特殊属性， 这种情况就要用到**主键**(PRIMARY KEY)。
* 没有重复的值
* 不允许输入空值 `NULL`

在创建表的时候设置主键:
```sql
CREATE TABLE 表名( 列名 数据类型 PRIMARY KEY …);
```

此外还可以设置具有“不允许重复”属性的唯一键(unique key),它不允许列中有重复值， 但是允许 NULL 值。  
给 INT 数据类型添加关键字 `AUTO_INCREMENT` 可以让数据库自动创建一个自增的列， 这个列的值会自动增加， 不需要手动指定，**自动编号设置方法随着 DBMS 的不同而变化**。
```sql
CREATE TABLE 表名( 列名 INT PRIMARY KEY AUTO_INCREMENT, 其他列 …);
```
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220428215833.png" width="40%"/></div>

同时连续编号可以初始化值， 如在上表中用下面的语句插入：
* `INSERT INTO t_series VALUES(100，' 卯 ')`列 a 中就会输入值“100”，然后从“101”开始分配连续的编号
* 在所有记录都删除掉之后， 可以以下面的方式进行初始化：`ALTER TABLE 表名 AUTO_INCREMENT=1;`
  * 当表中存在数据时，如果设置的编号值比已经存在的值大，也可以通过上面的语句重新设置编号的初始值 1

## Chap 7: 复制、删除表和记录
需要用到很多表，每次输入记录又很麻烦，所以为了能重复利用之前创建过的表，我们要掌握表的各种复制方法。
* 复制表的列结构和记录
* 仅复制表的列结构
* 仅复制记录

### 将表的列结构和记录整个复制
* 使用 `SELECT` 的结果复制列的结构和记录， 然后创建新的表。这种复制方法**不复制 `AUTO_INCREMENT` 等属性**， 它们需要复制之后进行设置。
  * 这种方法可能会改变列的属性。例如在某些 MySQL 的版本中，VARCHAR(100) 可能会变成 CHAR(100)
    ```sql
    CREATE TABLE 新表名 SELECT * FROM 元表名；
    ```
    执行`CREATE TABLE tb1_bk SELECT * FROM tb1;` 实际上就是执行下面的过程：<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220429204537.png" width="80%"/></div>

### 复制表的列结构&使用 select 复制表 记录
在 `CREATE TABLE` 命令的表名后面加上 `LIKE` 指定复制的元表。这种做法**不会复制表中的记录，但是会复制 `AUTO_INCREMENT` 和 `PRIMARY KEY` 等列的属性。**  
```sql
CREATE TABLE tb1_bkc LIKE tb1;
```
这样复制表的结构之后， 可以使用 `INSERT INTO` 命令来插入记录。
* 复制所有列
    ```sql
    INSERT INTO tb1_bkc SELECT * FROM tb1;
    ```
* 也可以复制列的一部分
    ```sql
    INSERT INTO tb1_bkc(name) SELECT empid FROM tb1;
    ```

### 删除表、数据库、记录
* 删除表
    ```sql
    DROP TABLE tb1;
    ```
  * 如不确定表一定存在，可以在表名前面加上 `IF EXISTS`
* 删除数据库
    ```sql
    DROP DATABASE db1;
    ```
* 也可以不删除表本身而直接删除其所有记录
    ```sql
    DELETE FROM table_name;
    ```

# PART III: 熟练使用 MySQL
## Chap 8: 使用各种条件进行提取
### 使用 SELECT 时设计列的显示
* 使用 `*` 通配符显示所有列
* 指定多个列的时候使用`,`分隔
* 同一个列可以显示多次
    ```sql
    SELECT * FROM tb;
    SELECT sales,empid FROM tb;
    SELECT sales,empid,sales,empid,sales, empid FROM tb;
    ```
* 选择的时候可以使用 `AS` 语句指定别名
    ```sql
    SELECT empid AS 员工号,sales AS 销售额 FROM tb;
    ```

### 计算列值或者处理字符串后显示列
<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220509211527.png" width="70%"/></div>

* 可以对列中的数据自由地使用上图所示的运算符，
* 也可以使用函数。 需要注意的是， 有的函数是[汇总某列的所有数据](MySQL必知必会.md#chap-12-汇总数据)，有的函数是[处理单个数据](MySQL必知必会.md#chap-11-使用数据处理函数)
* 还有一些和表无关的函数，如 `SELECT PI();`, `SELECT VERSION();`

*  字符串处理函数
   * `CONCAT()` 函数，用于拼接字符串<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220509213443.png" width="70%"/></div>
   * 从左/右取出：LEFT/RIGHT 函数,可以指定 列和字符数
   * 从第×个字符开始截取△个字符：SUBSTRING 函数
   * 重复显示：REPEAT 函数
       ```sql
       SELECT REPEAT('.',age) FROM tb1; # 简易的年龄直方图
       ```
   * 反转显示：REVERSE 函数

* 日期和时间函数
  * NOW 是用于返回当前日期和时间的函数

### 设置条件进行显示
* 可以使用 `LIMIT` 和 `OFFSET` 函数来设置起始位置和显示的记录数
* 使用 `WHERE` 子句提取记录的方法
  * 在 `WHERE` 子语句中可以适应比较运算符<div align=center><img src="https://raw.githubusercontent.com/Haitau1996/picgo-hosting/master/img/20220510102038.png" width="70%"/></div>