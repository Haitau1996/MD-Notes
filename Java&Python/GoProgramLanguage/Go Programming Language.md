---
tags: []
created: 2020-12-27T09:58:27.546Z
modified: 2020-12-27T11:16:51.216Z
---
# Go 程序设计语言
## Chap 1: 入门

### Hello World
Go 是编译型的语言: 工具链将程序的源文件转为机器相关的二进制指令:
```zsh
//一次性实验
$ go run helloworld.go
// 输出成可以复用的程序
$ go build helloworld.go
$ ./helloworld
Hello, 世界
```
**Go 原生支持 Unicode**, 故可以处理所有国家的语言, 需要