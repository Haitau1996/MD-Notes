# The Rust Programming Language
## Chap 1 : Getting Started
在 Linux/MacOS/msys2 中都可以使用命令安装 _rustup_ 工具, 如果网络不好的话可以参考 TUNA 上的[使用帮助](https://mirrors.tuna.tsinghua.edu.cn/help/rustup/)用国内源加速. 为了编译执行程序还需要一个链接器, 一般安装一个 C 语言编译器就能解决. <br>
安装工具会在本地生成离线文档, 使用 `rustup doc ` 可以在浏览器中使用它,里面甚至有本书的英文版.<br>
Rust 中的函数体被花括号包裹起来, 有一个 `rustfmt` 的工具可以格式化代码风格(`rustfmt someFile.rs`).
```rust
#![allow(unused)]
fn main() {
    println!("Hello, world!");
}
```
其中的一个 `println!` 并不是一个普通函数而是宏, **rust 中所有以 感叹号 `!` 结尾的调用都是宏**.最后需要注意的是 Rust 代码大部分都会用分号来结尾.<br>
Rust 是一种预编译语言, 随着代码量变多团队变大, 管理项目依赖和代码构建就会变得十分复杂和琐碎, Rust 使用一个构建工具 _Cargo_ 来简化问题.

### Cargo
Cargo 是工具链中内置的构建系统以及包管理工具, 可以处理代码构建, 下载编译依赖库等琐碎但重要的任务:
```shell
$ cargo new hello_cargo
$ cd hello_cargo
```
这时候就自动生成了一个 `Cargo.toml` 的配置文件, 包含 `main.rs` 的代码目录 `src` 以及初始化一个新的 Git 仓库(--vcs 参数可以选择不使用版本控制系统或者使用某个特定的版本控制系统).

#### 使用 cargo 构建和运行项目
```shell
$ cargo build
# 在 tatget 目录下完成构建任务
# --release 选项在优化模式下构建并且执行可执行程序
$ cargo run
# 如果没有更改的话直接运行, 
# 如果有的话重新编译并且运行新的修改项目
$ cargo check 
# 快速检查当前的额代码可否通过编译
# 并且不需要花费额外的时间去真正执行可执行程序
```

## Chap 3: 通用编程概念
### 变量与可变性
Rust 中的变量**默认是不可变的, 提供这一概念是为了能够让我们写出复杂甚至是并行的代码**.可以通过在变量名称前面加上 `mut` 关键字来使其可以改变, 该关键字还暗示其他代码可能会改变这个变量的值. 设计一个变量的可变性有多种考虑, 如果使用某些重型数据结构时, 适当的使用可变性去修改一个实例, 可能比赋值和重新分配一个新的实例更加有效率, 而使用相对轻量的数据结构时, 采用函数式的风格, 通过吃Aug年间新的变量来赋值, 可能使得代码更容易理解, 为了可读性而损失些许性能也许是值得的.
#### 变量与常量的不同
变量的不可变性和常量会弄混淆, 常量不仅仅是默认不可变, 它总是不可变, 用 const 而不是 let 关键字来声明, 在声明的同时需要显式说明值的类型, 只能将常量绑定到一个常量表达式上, 无法将一个函数的返回值, 或者需要在运行时计算的值绑定到常量上.
```Rust
const MAX_POINTS: u32 = 100_000;
```

#### 隐藏
我们可以重复使用 `let` 关键字并配以相同的名称来不断隐藏变量, 并且允许这个变量在操作完成后保持自身的不变性. 这个和 `mut` 变量还有一个区别就是, **`mut` 变量无法修改变量的类型, 只能修改其值**
```Rust
let spaces = " ";
let spaces = spaces.len();

let mut spaces = " ";
spaces = spaces.len(); /// Error: mismatched types
```

### 数据类型
我们将讨论两种不同的数据类型子集: 标量类型和复合类型. Rust 是静态类型语言, 这意味着需要在编译程序的过程中知道所有变量的具体类型.在这句`let guess: u32 = "42".parse().expect("Not a number!");` 中, 为了避免混淆, 需要手动添加类型标注.
#### 标量类型
Rust 中内建4种基本的标量类型, 整数/浮点数/布尔值 和 字符. 
##### 整数类型
会明确自身是否存在符号(描述负数的能力), 并且拥有明确的大小. 另外还有取决于体系结构的两种特殊的整数类型`isize`/`usize`, 在 64 位架构上就是 64 位, 在 32 位架构上就是 32 位. <br>
需要注意的是, 整数是会溢出的, Rust 对这种行为规则十分有趣, **在 debug 模式下在整数发生溢出的时候会触发 panic(某种异常)**, 在发布模式下, 就会自动 [rounding](../../ComputerSystem/CSAPP/CSAPP.Book.md).

##### 浮点数类型
Rust 提供了两种浮点数类型, `f32`/`f64`, 使用了 IEEE 的标准表示, 在现代计算机体系结构中两者的效率相差无几, 因此默认是使用`f64`.

##### 数值运算
对于所有的数值类型, Rust 都支持常见的数值运算, 并且将值绑定到左边的变量中.

##### 布尔类型
Rust 中的布尔类型只有两个可能的值, true 和 false, 只会占据单个字节的空间大小. 

##### 字符类型
char 类型被用于描述语言中最基本的单个字符, 占 4个字节, 是一个 Unicode 标量值, **用单引号指定**, 而不同于字符串使用的双引号.

#### 复合类型
复合类型是将多个不同类型的值组合为一个类型, Rust 提供了两种内置的复合类型, 元组(tuple) 和数组(array).
##### 元组类型
可以将其他不同类型的多个值组合到一个复合类型中, 还拥有一个固定的长度(**无法在声明结束后增加或者减少其中的元素数量**).使用的方法如下:
```Rust
let tup: (i32, f64, u8) = (500, 6.4, 1);
let tup = (500, 6.4, 1);
let (x, y, z) = tup;
let five_hundred = x.0;
```
从上面看, 可以通过解构(类似于 C++17 中的结构化绑定)和索引(`.`)来访问元组中的值.

##### 数组类型
数组中的每个元素都应该是相同的类型, 同样**拥有固定的长度**, 当然在 Rust 中有更加方便的动态数组(Vector)类型(不在这里讨论), 数组的使用方式如下:
```Rust
let a = [1, 2, 3, 4, 5];
let a: [i32; 5] = [1, 2, 3, 4, 5];
let a = [3; 5]; //  a concise way let a = [3, 3, 3, 3, 3];
```
访问数组的元素和C等语言一样, 使用 `name[index]` 访问, 出现越界的情况时, 发生 panic.

### 函数

Rust 使用蛇形命名法(小写字母命名, 下划线分割单词)作为规范来命名函数和变量名称, 定义函数的时候用关键字 `fn` 并且紧随一对小括号(中间为参数), 然后函数主体写在一个代码块中. 可以发现和 C/C++ 不同的是这里的函数不需要先声明后使用.**rust 不关心你在哪里定义函数, 只需要在使用的地方该函数是可见的.**  
函数参数一种特殊的变量, 它被视为函数签名的一部分, 并且需要在调用函数时为这些参数提供具体的值:
```Rust
fn main() {
    println!("Hello, world!");
    another_function();
}
fn another_function(x: i32) {
    println!("The value of x is: {}", x);
}
```
#### 函数体中的语句和表达式

Rust 是一门基于表达式的语言, 它将语句( _statement_ )和表达式( _expression_ )区别为两个不同的概念.语句（Statements）是**执行一些操作但不返回值的指令**, 表达式（Expressions）**计算并产生一个值**, 但是表达式可以作为语句的一部分.  
```Rust
    let y = {
        let x = 3;
        x + 1
    };
```
这里面, 方括号内的这个代码块也是要给表达式, 产生的值为 4. 

#### 函数的返回值
我们需要在函数后面加入箭头符号(`->`)声明其返回值类型, 在函数体内可以用 return 语句提前指定一个值从函数中返回, 而大多时候是**隐式返回最后的表达式**, 如果我们在表达式的最后加了一个分号, 就会导致该表达式变为语句, 没有返回值引发编译错误.  
Rust 的注释一般是两道斜杠开始到行尾, 在 14 章会介绍其他的注释格式.

### 控制流
#### if 表达式
```Rust
    let number = 3;
    if number < 5 {
        println!("condition was true");
    } else {
        println!("condition was false");
    }
```
if 表达式中的条件表达式必须产生一个 `bool` 类型的值, 否则无法通过编译, 并且不支持 C++ 中非零值到 true 这种隐式类型转换. 同样也支持 `else/else if` 这种多重条件判断.此外, if 语句后面的代码块**也可以认为是一个表达式, 输出的值就是最后一个表达式的值**并且可以用在 let 语句中给变量赋值. 
```Rust
let number = if condition {
        5
    } else {
        6
    };
```

#### 使用循环重复执行
Rust 中提供了三循环, `loop`,`while` 和 `for`.  
loop 反复执行代码块, 直到遇到了`break` 循环终止:
```Rust
    let mut counter = 0;
    let result = loop {
        counter += 1;
        if counter == 10 {
            break counter * 2;
        }
    };// 代码结束执行侯, result 为 20
```
while 是才每次执行循环体前做一次判断, 在条件为假或者执行中碰到 `break` 的时候就退出当前循环. 
```Rust
    let mut number = 3;
    while number != 0 {
        println!("{}!", number);
        number = number - 1;
    }
```
for 循环是遍历集合中每一个元素的简明方法:
```Rust
    let a = [10, 20, 30, 40, 50];
    for element in a.iter() {
        println!("the value is: {}", element);
    }
    for number in (1..4).rev() {
        println!("{}!", number);
    }
```

## Chap 4: 认识所有权
正是所有权概念和相关工具的引入, Rust 才能在没有垃圾回收机制的前提下保证内存安全. 
### 所有权规则
* Java: 在运行时定期检查并且回收那些没有被继续使用的内存
* C: 程序员需要手动分配和释放内存
* Rust: 使用包含特定规则的所有权系统来管理内存, 这套规则**允许编译器在编译过程中执行检查工作, 而不产生任何运行时开销**
  * Rust 中的每个值都有一个对应的变量作为它的所有者
  * **在同一时间内, 值有且仅有一个所有者**
  * 当所有者离开自己的作用域, 它持有的值就会被释放掉

作用域是一个项在程序中的有效范围, 一般是一个代码块
#### String
```rust
let s = "hello";
```
我们借助字符串类型理解这部分内容,上面的变量 s 指向了一个字符串字面量, 它的值被硬编码到了当前的程序中. 但**不是所有的字符串值都能在编译时确定**, 第二种 String 类型会在堆上分配自己需要的存储空间, 能处理在编译时未知大小的文本.   
```Rust
let mut s = String::from("hello");
s.push_str(", world");
```
而 String 类型可变是因为它采用了不同的内存处理方式. 

#### 内存与分配
对于字符串字面量, 它会被硬编码到二进制文件中, 访问异常高效. 而 String 类型为了支持一个可变的/可增长文本的类型, 需要在 heap 中分配一块在编译时不知道大小的内存来存放数据:
* 使用的内存是由操作系统在运行时动态分配出来的
* 用完后需要用某种方式归还给操作系统

Rust 提供了一套解决方案, 内存自动地在拥有它的变量离开作用域后释放, 变量离开作用域时, 会调用一个叫做 `drop` 的特殊函数. 但是在复杂的环境中,**如果同时拥有多个指向同一块堆内存的变量, 就会出现二次释放的问题**.
##### 移动
```Rust
let s1 = String::from("hello");
let s2 = s1;
```
<div align=center><img src="https://i.imgur.com/SKiKlPK.png"/></div>

Rust 不会在复制值的时候深度复制堆上的数据, 因为右上图在数据足够大的时候有相当的运行时开销. 但是如果按照左边的方式, s1 s2 离开作用域会调用两次 drop, 因此Rust 在做浅拷贝的同时, 会让第一个变量无效, 这种行为被称为 **移动**(move).<div align=center><img src="https://i.imgur.com/yALiO4H.png"/></div>

如果我们需要图 4-3 这样深度拷贝堆上的数据时, 可以使用一种名为 `clone` 的方法:
```Rust
let s1 = String::from("hello");
let s2 = s1.clone();
```
##### 栈上数据的复制
现在还有一个问题, 在栈上的数据并不需要调用 `clone()` 方法依旧是合法的. 这是因为这些类型的数据**可以在编译时确定自己的大小,并且完整地保存在栈中**, Rust 为他们提供了一个名为 ==Copy== 的 trait, 一但某个类型拥有了这种 trait, 其变量在赋值给其他变量之后依旧可用(==Copy 和 Drop 不兼容==).  
一般来说, **任何简单的标量类型和它们的组合类型都是可以 Copy 的, 任何需要分配内存或者某种资源的类型都不会是 Copy 的.**  
#### 所有权与函数
将值传递给函数在语义上与给变量赋值相似。**向函数传递值可能会移动或者复制，就像赋值语句一样**。
```Rust
let s:String = String::from("hello world");
take_ownership(s);
```
在调用自定义函数 `take_ownership` 之后, s 指向的对象就被借用走了, 再使用 s 就会触发编译错误.  
#### 返回值与作用域
**函数在返回值的过程中也会发生所有权转移**:变量所有权的转移总是遵循相同的模式, 将一个值赋值给另一个变量的时候就会转移所有权, 当一个持有堆数据的变量离开作用域时, 就会调用 `drop` 清理回收数据, **除非将这些数据的所有权转到另一个变量上**.  
当我们希望调用函数时保留参数的所有权, 就不得不将传入值作为结果返回, 我们可以利用 tuple 来返回多个值. 
```Rust
fn main() {
    let s = String::from("hello");
    let (_, s) = take_ownership(s, 2);
    println!("the s now is {}", s);
}
fn take_ownership(mut some_str: String, mut i: i32) -> (i32, String) {
    println!("the ownership of {} is token", some_str);
    some_str.push_str(", world");
    i += 2;
    (i, some_str)
}
```
但是使用 tuple 返回太笨拙, Rust 为这种场景提供了一个名为引用的功能. 

### 引用与借用
```Rust
fn main() {
    let s1 = String::from("hello");
    let len = calculate_length(&s1);
    println!("The length of '{}' is {}.", s1, len);
}

fn calculate_length(s: &String) -> usize {
    s.len()
}
```
`&` 符号就是引用，它们**允许你使用值但不获取其所有权**。在上面的例子中,`s.len()`之后 s 离开作用域, 但是因为它不持有指向值的所有权, 所以不调用 `drop()`.<div align=center><img src="https://i.imgur.com/R4swICi.png"/></div>

这种通过引用将参数传递给函数的方法被称为**借用**(borrowing), 和变量一样, 引用默认是不可变的. 
#### 可变引用
实现可变引用需要先将变量声明为可变, 然后修改函数签名:
```Rust
fn main() {
    let mut s = String::from("hello");
    change(&mut s);
}

fn change(some_string: &mut String) {
    some_string.push_str(", world");
}
```
Rust 可变引用有一个很大的限制：**在特定作用域中的特定数据只能有一个可变引用**, 这个规则帮助我们避免数据竞争.
* 两个或以上的指针同时访问同一个空间
* 其中至少一个指针会向空间中写入数据
* 没有同步数据访问的机制

数据竞争引发的未定义行为往往难以在运行时进行跟踪, 使得出现的 Bug 难以诊断/修复. Rust 从规则上避免了这种情形的出现.我们可以通过花括号创建一个新的作用域, 实现不同时存在的多个引用. 
```Rust
    let mut s = String::from("hello");
    {
        let ref_of_s = &mut s;
        ref_of_s.push_str(" World");
    }
    let another_ref = &mut s;
```
此外, 我们不能同时拥有不可变引用和可变引用.

#### 悬垂引用
在 Rust 语言中, 编译器会确保永远不会进入这种状态. 
```Rust
fn main() {
    let reference_to_nothing = dangle();
}

fn dangle() -> &String {//error:expected named lifetime parameter
    let s = String::from("hello");
    &s
}
```
在这个例子中, 我们在 `dangle()` 中要求返回一个对 String 对象的引用, 而 s 离开作用域后就失效了, 因此编译器就会报错:**Rust 能够保证引用总是有效的**.

### 切片
除了引用, Rust 还有一种不持所有权的数据类型: 切片(slice).slice 允许你引用集合中一段连续的元素序列，而不用引用整个集合。