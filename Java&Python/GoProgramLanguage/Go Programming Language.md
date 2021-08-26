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
**Go 原生支持 Unicode**, 故可以处理所有国家的语言. Go 代码是用包来组织的, 包类似于其他语言中的库和模块. 其中==有个特殊的包名 main==, 用于定义一个独立的可执行程序.  
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
另一种 for 循环的形式, ==go 中不允许存在无用的临时变量, 解决方法是使用空白标识符==(`_`):
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
go 提供了一系列包, 在 net 下分组管理, 使用它们可以方便地通过互联网发送和接收信息, 使用底层的网络链接, 创建服务器.   
此外, go 还支持并发编程, 在这里我们了解一下 go 的主要并发机制, goroutine 和通道.  
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

## Chap 2: 程序结构
Go 语言中的大程序都从小的基本组件构建而来:
* 变量存储值
* 简单表达式通过加和减等操作合并成大的
* 基本类型通过数组和结构体聚合
* 表达式通过 if 和 for 等控制语句来决定执行顺序
* 语句被组织成函数用于隔离和复用
* 函数被组织成源文件和包

### 名称
基本继承 C 语言的命名规则: 字母/下划线开头, 后面跟任意数量的字符/数字和下划线, 区分大小写, 同时要避开关键字.  

但是需要注意的是作用域, 一个实体在函数中声明就在函数局部有效, 函数外声明对包中的所有源文件可见,**实体第一个字母的大小写决定其可见性是否跨包**: ==大写字母开头是导出的==.  
### 声明
声明给一个程序实体命名, 并且设定其部分或者全部属性, 有四种主要的声明: **变量(_var_)/ 常量(_const_)/ 类型(_type_) 和函数(_func_)**.  
需要注意的是函数的声明包含一个名字,一个参数列表和一个**可选的**返回值列表(如果不返回任何内容可以省略):
```go
// 华氏度转摄氏度
func fToC(f float64) float64 {
	return (f - 32) * 5 / 9
}
```

### 变量
`var` 声明一个具体类型的变量, 然后附加一个名字设置它的初始值, 有通用的形式:
```go
var name type = expression
```
类型和表达式可以省略一个, 但是不能都省略.省略类型, 其具体类型将由初始化表达式决定; 省略表达式, 初始值对象类型的零值. ==**零值机制保障了所有变量都是良好定义的, Go 中不存在未初始化变量**==. 同时声明列表并且用表达式列表对其做初始化, 或者调用返回多个值的函数做初始化:
```go
var i, j, k int // int, int, int
var b, f, s = true, 2.3, "four" // bool, float64, string
var f, err = os.Open(name) // os.Open returns a file and an error
```

#### 短变量声明
在函数中, 一种被称为短变量声明的可选形式可以用来声明和初始化**局部变量**:
```go
name:= expression
i := 100 // an int
var boiling float64 = 100 // a float64
```
需要注意的两点:
* 如果一些变量在同一个语法块中声明, 那么短声明的行为等同于赋值
* 短变量声明至少声明一个新变量(否则应该使用普通赋值语句)

#### 指针
指针的值是一个变量的地址, 不是所有值都有地址, 但是所有变量都有. 表达式 `&x` 获取该地址, 如果 x 为 int, 其指针类型为 `*int`. **在 Go 中函数返回局部变量地址是非常安全的**:
```go
var p = f();
func f() *int{
    v:=1
    return &v
}
```

#### new 函数
表达式 `new(T)` 创建一个未命名的 T 类型变量, 初始化为 T 类型的零值, 并且返回其地址(类型为 `*T`), 这种创建的变量和取其地址和普通的局部变量没有什么不同, 只是不需要引入和声明一个虚拟的名字. new 是一个预先声明的函数, 并不是一个关键字, 它可以重新定义为另外的其他类型: 在 delta 函数中, new函数就不可用(名称被覆盖)
```go
func delta(old, new int) int{
	return new-old
}
```

#### 变量的生命周期
包级别变量的生命周期是整个程序的执行时间, 局部变量有一个动态生命周期, 执行声明语句时创建一个新的实体, 变量一直生存直到**它不可访问**, 这时候占用的存储空间被回收. 一定使用堆空间的对象如下声明:
```go
var global *int
func f() {
    var x int
    x = 1
    global = &x
}
```
因为 f 返回之后还可以从 global 变量访问, 这种情况被称为 **x 从 f 中逃逸**, ==每一次变量逃逸都需要一次额外的内存分配过程==.

### 赋值
赋值语句更新变量所指的值, 最简单的形式是使用赋值符 `=`, 数字变量也可以通过 `++` 和 `--` 进行自增/自减.
#### 多重赋值
另一种形式的赋值是多重赋值, 它允许几个变量一次性被赋值, **实际更新变量之前, 右边的所有表达式先被推演**, 因此在交换两个变量时特别有用:
```go
x, y = y, x
a[i], a[j] = a[j], a[i]
func fib(n int) int {
    x, y := 0, 1
    for i := 0; i < n; i++ {
        x, y = y, x+y
    }
    return x
}
```
风格上如果表达式比较复杂, 则避免多重赋值.

#### 可赋值性
赋值发生在显式和隐式(函数调用和返回) 中, 如果左边的变量类型和右边的值类型相同, 它就是合法的. 对于前面讨论过的类型, 可赋值性规则十分简单:**类型必须精确匹配**.

### 类型声明
type 声明定义一个新的命名类型, 它和某个已有类型使用同样的低层类型, 这提供了一种方式来区分低层类型的不同或者不兼容使用. 
```go
type Celsius float64
type Fahrenheit float64

const (
	AbsoluteZeroC Celsius = -273.15
	FreezingC     Celsius = 0
	BoilingC      Celsius = 100
)

func CToF(c Celsius) Fahrenheit { return Fahrenheit(c*9/5 + 32) }

func FToC(f Fahrenheit) Celsius { return Celsius((f - 32) * 5 / 9) }
```
需要注意的是, ruturn 语句中的 `Fahrenheit` 是从 `float64` 到华氏度需要的显式类型转换, 不是函数调用. 而底层类型决定了其结构/表达方式 以及它支持的内部操作集合, 这些都是相同的. 
```go
var c Celsius
var f Fahrenheit
fmt.Println(c == 0) // "true"
fmt.Println(f >= 0) // "true"
fmt.Println(c == f) // compile error: type mismatch
fmt.Println(c == Celsius(f)) // "true"!
```
很多类型都声明一个 String 方法, 控制类型值的显示方式:
```go
func (c Celsius) String() string { return fmt.Sprintf("%g°C", c) }
```

### 包和文件
包的作用和其他语言中的库/模块相似, 用于支持模块化/封装/编译隔离和重用. 一个包源代码保存在一个或者多个以 .go 结尾的文件中, 所在目录名的尾部就是包的导入路径. **每个包给它的生命提供独立的命名空间**, 从包外部引用一个函数必须明确指定, 如 `image.Decode` 和 `utf16.Decode`. 我们通过控制变量在包外面的可见性(第一个字母大小写)或导出情况来隐藏信息. 
#### 导入
在 go 程序中, 每个包通过被称为导入路径的唯一字符串标识.导入之后就一定要使用(如果导入一个没有被引用的包, 就会触发一个编译错误).
```go
package main

import (
	"fmt"
	"os"
	"strconv"
	"gopl.io/ch2/tempconv"
)

func main() {
	for _, arg := range os.Args[1:] {
		t, err := strconv.ParseFloat(arg, 64)
		if err != nil {
			fmt.Fprintf(os.Stderr, "cf: %v\n", err)
			os.Exit(1)
		}
		f := tempconv.Fahrenheit(t)
		c := tempconv.Celsius(t)
		fmt.Printf("%s = %s, %s = %s\n",
			f, tempconv.FToC(f), c, tempconv.CToF(c))
	}
}
```

#### 包的初始化
包的初始化从初始化包级别的变量开始, 这些变量按照声明的顺序初始化, 在依赖已解析完毕的情况下, 根据聚类的顺序进行.   
对于其他变量, 初始化表达式不是简单地设置它的初始化, 这时候使用 init 函数的机制会比较简单, 在每个文件中, 程序启动的时候, init 函数(不能被调用和被引用)按照他们声明的顺序自动执行. 

### 作用域
声明的作用域是指用到所声明的名字的源代码片段, 区别于生命周期. 前者值得是声明在程序文本中出现的区域, 是一个编译时属性; 后者是变量在执行期间能被程序的其他部分所引用的起止时间, 是一个运行时属性.  
语法块是一由大括号包围起来的语句序列, 如一个循环体或者函数体, 其内部声明的变量对块外不可见.当编译器遇到一个名字或者引用时, 将从内层封闭语法块到全局块寻找其声明, 如果在内外都有声明, 那么内层声明将覆盖外层声明.   
包级别的声明可以被同一个包中的任何文件引用, 导入的包则是文件级别的, 控制流便签的作用域是整个外层的函数. 

## Chap 3: Basic Data Types
Go 语言的数据类型分成四类: 基础类型, 复合类型(包括数组和结构体), 引用类型 和 接口类型. 
### 整数
Go 中的整数包括有符号和无符号,并有 4 种大小 , 8 位/ 16位/ 32 位 和 64位.`int8`/`uint8`..., rune 类型是 `int32` 的同义词, 用于指明一个值是 _Unicode_ 码点, `byte` 是 `int8` 的同义词, 表明一个值是原始数据不是量值.   
`uintptr` 用于完整存放指针, 仅用于低层编程. (这种设计和 RUST 很像, 不过 RUST 缩写更多).  
同样的, 它支持算术\逻辑和比较等运算, 优先级降序排列如下:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210517164555.png"/></div>

此外也具备位运算:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210517164740.png"/></div>

和 C 语言一样, 做右移的时候, 对于有符号数是用 sign bit 填充高位, 对于无符号数是以 0 填充高位(CSAPP 中的算术右移和逻辑右移), 因此相关处理的时候须使用无符号整型.   
还有需要注意的是, 不同的整数类型之间的操作需要使用强制类型转换, 而文字符号(Rune literals) 形式的字符是写在一对单引号内, 可以是 ASCII 字符或者直接使用 Unicode 码点或者码值转义. 
```go
ascii := 'a'
unicode := 'D'
newline := '\n'
fmt.Printf("%d %[1]c %[1]q\n", ascii) // "97 a 'a'"
fmt.Printf("%d %[1]c %[1]q\n", unicode) // "22269 D 'D'"
fmt.Printf("%d %[1]q\n", newline) // "10 '\n'"
```

### 浮点数
Go 中有两种大小的浮点数 `float32` 和 `float64`, 前者有效数字大概是 6 位, 后者是 15 位, 在现代计算机体系结构中使用两者的效率接近, 而前者往往容易累积误差, 故不建议使用. 

### 复数
Go 中具备两种大小的复数, `complex64` 和 `complex128`, 分别有两种浮点数构成, 内置的 complex 函数根据给定的十步和虚部创建复数, 而 内置的 real 和 imag 函数非别提取其实部和虚部. 可以使用 `==` 或者`!=` 判断复数是否相等. `math/cmplx` 提供了复数运算所需要的库函数. 

### 布尔值
bool 型的值只有两种可能 `true` 和 `false`, 它可以由运算符 `&&` 和 `||` 组合得到, 但是**这可能引起短路行为**.

### 字符串
字符串是不可变的字节序列, 可以包含任意数据, 包括 0 值字节. 