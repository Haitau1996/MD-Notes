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