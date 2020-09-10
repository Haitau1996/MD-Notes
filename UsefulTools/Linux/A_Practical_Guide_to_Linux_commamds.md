## Notes On *A Prtctical Guide to Linux Commands, Editors, and Shell Programmings,Forth Edition*

## Chapter 2 入门

**ssh** 代表Secure Shell,常用的选项 **-p** 指定端口（port）<br>
**Control-U** 默认的行删除，如果不作用可换用-X<br>  
**man** 查找文档(manual)<br>
**help** 查找帮助，默认的调用方式如 ls --help <br>
**Control-D** 退出终端<br>

## Chapter 3 实用程序

**ls** 显示文件名，常用的选项 -a (all的简写) -l (long,以长格式列出) -R（recursive,递归）<br>
**cd** 更改目录(change directory)<br>
**cp** 复制(copy)<br>
**mkdir** 创建目录（make directory) <br>
**rm** 删除目录（remove），常用选项 -r(recursivs,递归删除)<br>
**pwd** 显示当前目录(print working directory)<br>
**cat** 显示文件，可以与管道联合<br>
**grep** 查找字符串，支持正则表达式<br>
**|** 管道（tunnel）,将一个命令的输出作为另一个的输入<br>
**locate/whereis/which** 定位文件/程序、代码/可执行文件的完整路径名<br>

## Chapter 4 Linux 文件系统
**chmod** 改变文件访问权限 1（执行）2（写入）4（读取）<br>

## Chapter 5 Shell
**标准用法** command [argv1] [argv2] ... [argvn] RETURN<br>
**输出重定向** 命令 > 文件，命令 >> 文件 （向文件末尾添加）<br>
**输入重定向** 命令 < 文件<br> 
**管道** 命令A 参数 | 命令B  参数，将A输出到temp文件，再以temp为输入运行B<br>
**后台运行** 命令 &

## Chapter 8 bash & Chap 10 bash 程序设计
**if - else statement** <br>
if test-command<br>
    then commands<br>     
    elif test-commend<br>
    then commands<br>               
fi<br>
**for statement** <br>
for loop-index<br>
do<br>
    commends<br>
endo<br>

# 附加功能

**windows pot player设置**
cat ~/.ssh/id_rsa.pub | ssh chenht@cluster02.swip.ac.cn "cat >> ~/.ssh/authorized_keys"