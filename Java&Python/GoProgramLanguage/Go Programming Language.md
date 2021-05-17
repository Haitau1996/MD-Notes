---
tags: []
created: 2020-12-27T09:58:27.546Z
modified: 2020-12-27T11:16:51.216Z
---
# Go 程序设计语言
## Chap 1: 入门

### Hello World
Go 是编译型的语言: 工具链将程序的源文件转为机器相关的二进制指令:
```shell
# 一次性实验
$ go run helloworld.go
# 输出成可以复用的程序
$ go build helloworld.go
$ ./helloworld
Hello, 世界
```
**Go 原生支持 Unicode**, 故可以处理所有国家的语言. Go 代码是用包来组织的, 包类似于其他语言中的库和模块. 其中有个特殊的包名 `main`, 用于定义一个独立的可执行程序.<br>
我们需要告诉编译器源文件需要哪些包, 在导入缺失或者存在不需要的包编译就会失败. Go 语言不需要在语句或者声明后面使用分号结尾, 除非有多个语句或声明在同一行. 同时, Go 对于代码的格式化要求非常严格. 

### 命令行参数
大部分程序处理输入产生输出, `os` 包提供一些函数和变量, 以和平台无关的方式和操作系统打交道, 例如命令行参数以 `os.Args` 给一个字符串 _slice_ 供程序访问:
```go
package main

import (
	"fmt"
	"os"
)

func main() {
	var s, sep string;
	for i := 1; i < len(os.Args); i++ {
		s += sep + os.Args[i]
		sep = " "
	}
	fmt.Println(s)
}
```
var 关键字声明了两个 string 类型的变量, s/sep, 此外这里展示了一种 for 循环:
```go
for initialization; condition; post {
    // zero or more statements
}
```
另一种 for 循环的形式, go 中不允许存在无用的临时变量, 解决方法是使用空白标识符(`_`):
```go
for index, val := range os.Args[1:]{
    // zero or more statements here
}
```
声明和初始化 string 有多种方式:
```go
s := ""
var s string
var s = ""
var s string = ""
```
对于 string 的 + 操作, 每次都会生成一个新的字符串然后赋值, 旧的不使用则会被例行垃圾回收, 考虑效率的话可以使用 string 包中的 `Join` 函数.

### 找出重复行
`bufio` 包提供带缓存的 io, 读取后我们可以在 map 中存储一个 key-value 对, 于是可以用 `map[string]int` 给出一个 字符串-计数 对:
```go
input := bufio.NewScanner(os.Stdin)
for input.Scan() {
	counts[input.Text()]++
}
for line, n := range counts {
    if n > 1 {
        fmt.Printf("%d\t%s\n", n, line)
    }
}
```
和 C 语言或者其他语言一样, `fmt.Printf` 按照表达式列表生成格式化的输出. 此外我们可以打开文件:
```go
func countLines(f *os.File, counts map[string]int) {
	input := bufio.NewScanner(f)
	for input.Scan() {
		counts[input.Text()]++
	}
	// NOTE: ignoring potential errors from input.Err()
}
```

### 获取 URL
go 提供了一系列包, 在 net 下分组管理, 使用它们可以方便地通过互联网发送和接收信息, 使用底层的网络链接, 创建服务器. <br>
此外, go 还支持并发编程, 在这里我们了解一下 go 的主要并发机制, goroutine 和通道.<br>
下面的例子中, 我们将相关的函数写在 fetch 中, 然后启动多个例程:
```go
func main() {
	start := time.Now()
	ch := make(chan string)
	for _, url := range os.Args[1:] {
		go fetch(url, ch) // start a goroutine
	}
	for range os.Args[1:] {
		fmt.Println(<-ch) // receive from channel ch
	}
	fmt.Printf("%.2fs elapsed\n", time.Since(start).Seconds())
}

func fetch(url string, ch chan<- string) {
	start := time.Now()
	resp, err := http.Get(url)
	if err != nil {
		ch <- fmt.Sprint(err) // send to channel ch
		return
	}

	nbytes, err := io.Copy(ioutil.Discard, resp.Body)
	resp.Body.Close() // don't leak resources
	if err != nil {
		ch <- fmt.Sprintf("while reading %s: %v", url, err)
		return
	}
	secs := time.Since(start).Seconds()
	ch <- fmt.Sprintf("%.2fs  %7d  %s", secs, nbytes, url)
}
```
main 函数使用 make 串讲一个字符串通道, 对每个命令行参数, go 在第一轮循环中启动一个新的 goroutine, 异步调用 fetch 来使用 `http.Get` 来获取 URL 内容. 

