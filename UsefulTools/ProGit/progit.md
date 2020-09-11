# Pro Git , 2nd Edition

## Chap 1 : Intro

### 版本控制系统
分布式版本控制系统, 并非仅仅是检出文件的最新快照, 而是对代码仓库进行完整的镜像,不管是哪个服务器出现故障, 任何一个客户端都应该使用自己的本地镜像来恢复服务器.<br>
在Git 中, 文件可以处于以下三种状态: committed(已提交), modified(已修改), and staged(已暂存), 因此引入了Git项目中的三个主要区域, Git目录, 工作目录 和暂存区: <br>
![1.1](figure/1.1.png)

### Git 首次使用的配置

**设置编辑器** : `git config --global core.editor emacs`<br>
**用户身份**: 
* `git config --global user.name "John Doe"`
* `git config --global user.email johndoe@example.com`

**检查个人设置** :`git config --list ` <br>

## Chap 2: Git Basics

### 创建一个Git 仓库
* $ _git init_ 
* $ _git clone https://github.com/libgit2/libgit2 mylibgit_ 

### 记录变更
* 查看当前文件状态 $ `git status`, _-s_ 选项可以更简洁的方式显示
* 跟踪新文件 $ `git add someFile`
