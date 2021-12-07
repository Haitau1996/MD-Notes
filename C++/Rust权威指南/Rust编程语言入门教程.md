# [Rust编程语言入门教程](https://www.bilibili.com/video/BV1hp4y1k7SV)

# Intro
**Why Rust**:(性能/安全性/并发 but 难学)
* Rust 和 C/C++ 具有相同的性能, 但是很多常见的 Bug 可以在编译的时候被消灭
* 作为一种通用的编程语言, 擅长在下列场景中使用:
  * 需要运行时的速度
  * 需要内存安全
  * 更好地利用多处理器8
* 比 C/C++ 等高性能的编程语言相比更加安全, 比拥有 GC 的 Java/C# 更加高效

Rust 有很多独有的概念, 和现在大多数主流的语言都不同. 

## 安装
从命令行运行[官网](https://www.rust-lang.org/tools/install)中的相关代码:
* 更新 : `rustup update`
* 卸载 : `rustup self uninstall`
* 添加新的插件: `rustup component add [some component]`, 如 `rustup component add rust-analysis`

安装成功后, 运行 `rustup doc` 可以得到英文的文档, 它也有社区翻译的[中文版本](https://kaisery.github.io/trpl-zh-cn/ch01-01-installation.html).  
Rust 语言中编译和运行是独立的两步:
* `rustc source.rs` 编译文件, 生成二进制文件和一个包含调试信息的 `.pdb` 文件
* 是预先编译的语言(ahead-of-time)
* 在复杂的程序中使用 Cargo

## Cargo
Cargo 是 Rust 的构建系统和包管理工具(帮助构建代码,下载依赖 etc)
* `cargo new project_name` 创建项目
* Cargo.toml: 配置文件的格式,位于项目的顶层目录下
  * package 段配置包
  * dependencies 列出以来项. 
* `cargo build` 创建二进制文件, 第一次运行会生成 cargo.lock(追踪以来的精确版本, 不要手动修改)
  * `--release` 选项用于发布时的构建(会做优化, 需要更长的编译时间)
* `cargo run`: 编译+运行
* `cargo check`: 检查代码是否合法, 不编译, 在编写程序的时候周期性调用检查语法

# 猜数字
## 完成一次猜测
```Rust
use std::io;
fn main() {
    let mut guess = String::new();// 返回一个 String 的新实例
    io::stdin().read_line(&mut guess)
        .expect("Failed to read line");
    //io::Result 枚举类型， 可以是 Ok 和 Err, Err 中有失败的原因， 并且其中有 expect 方法
}
```
上面是完成一次猜测的主体内容，其中的 let 用于声明变量， 而 _Rust_ 中变量默认不可变， 需要将内容写入 _guess_, 因此加上了 `mut` 关键字声明变量可变。   
默认情况下，Rust 会将少量标准库中定义的模块引入到每个程序的作用域中。这些模块称作 _prelude_, 如果需要的类型不在其中， 则要使用 use 语句引入， 如果不引入的话可以使用 `std::io::stdin()...`。而 `.read_line` 将读取的内容写入 guess, IO 很可能发生错误， 因此 `.expect` 后面跟着错误处理。
## 生成神秘数字
crate 是 Rust 的代码包， 有二进制 crate(可以运行) 和库 crate, 可以在 Cargo.toml 中引入 crate 依赖,并且在编译过程中自动下载：
```
[dependencies]
rand = "0.8.3"
```
_0.8.3_ 事实上是 `^0.8.3` 的简写，它表示与 _0.8.3_ 版本公有 API 相兼容的版本,实际就是任何不低于 _0.8.3_， 但是低于 _0.9.0_ 的版本。 cargo 会使用 _Cargo.lock_ 文件确保构建是可重现的。  
引入之后， 我们可以使用 use 语句引入 `rand::Rng`, 然后生成一个随机数：
```Rust
let secret_number = rand::thread_rng().gen_range(1..101);
```

## 比较数字
这里我们适用 match 匹配做比较， 得到的结果是一个枚举类型 `std::cmp::Ordering`:
```Rust
let guess:u32 = guess.trim().phase().expect("please enter an integer");
match guess.cmp(&secret_number) {
        Ordering::Less => println!("Too small!"), //arm
        Ordering::Greater => println!("Too big!"),
        Ordering::Equal => println!("You win!"),
}
```
在 rust 中可以适用同名变量来隐藏原来的变量，而 guess 在 0 到 100 之间， 需要使用类型说明帮助编译器推导。 

## 多次猜测
使用 loop 做无限循环， 在猜对的时候使用 break 退出， 同时猜测的时候也可以使用通配符,在输入不符合预期的时候重新循环：
```Rust
loop{  
    io::stdin().read_line(&mut guess)
        .expect("Failed to read line");

    let guess: u32 = match guess.trim().parse() {
        Ok(num) => num,
        Err(_) => continue,
    };
    match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }
    //-- snip --
}
```
# 通用编程概念
## 变量与可变性
* 使用 `let` 声明变量
* 变量默认是不可变的， 要修改变量的值使用 `mut` 关键字
    ```Rust
    let x = 5;
    x = 10; //error[E0384]: cannot assign twice to immutable variable `x`
    ```

### 变量与常量
即便如此， rust 依旧提供了常量：
* 声明常量使用关键字 `const`, 它的类型必须被标注
* 不可以使用 `mut` 修饰， 因为它永远不可变
* 可以在任何作用域中声明， 包括全局作用域
* 常量只能绑定到常量表达式， 无法绑定给函数调用结果或者只有在运行时才能计算出来的值
* 明明是所有字母大写 + snakeCase, 如 `MAX_POINTS`

### shadowing
rust 中可以定义一个与之前变量同名的新变量， 即可以用相同变量名称来隐藏一个变量，以及重复使用 `let` 关键字来多次隐藏。
```Rust
let x = 5;
let x = x + 1;
```
* 使用 `let` 声明的新变量可以与原来变量的类型不同
  ```Rust
  let spaces = "   ";
  let spaces = spaces.len();
  ```

## 数据类型
Rust 是静态类型语言， 在编译时候必须知道所有变量的类型
* 基于使用时的值， 编译器能够自动推断具体的类型
* 但是如果可能的类型比较多的话， **就必须手动添加类型的标注**， 否则编译器会报错

### 标量类型
标量类型代表一个值， rust 中有四种标量类型： 整数类型， 浮点类型， 字符类型， 布尔类型。 
#### 整数类型
* 使用 u 开头表示 unsigned, 使用 i 开头表示 signed
* 后面跟的数字是占用的空间大小(多少个 bits)
* `isize`/`usize` 类型由计算机的架构所决定<div align=center><img src="https://i.imgur.com/9UV2F67.png"/></div>

除了 byte 之外， 所有数值字面值都允许使用字符后缀， 如 `57u8`。<div align=center><img src="https://i.imgur.com/mOA6x18.png"/></div>

整数类型可能会溢出
* 在 debug 模式下的溢出， 引发程序的 _painc_
* 在发布模式下， 不会检查整数溢出， 直接执行环绕(round) 操作

#### 其他标量类型
rust 中有单精度(f32) 和双精度(f64,默认)两种浮点类型， rust 中所有的数字类型都支持基本数学运算：加法、减法、乘法、除法和取余。  
rust 中布尔类型有两个可能的值 `true`和 `false`, 并且使用一个字节的内存空间。  
`char` 类型是语言中最原生的字母类型， 大小为四个字节(four bytes)，并代表了一个 Unicode 标量值。
```Rust
    let z = 'ℤ';
    let heart_eyed_cat = '😻';
``` 

### 复合类型
复合类型可以将多个值组合成一个类型。Rust 有两个原生的复合类型：元组（tuple）和数组（array）。
#### tuple
创建 tuple
* 在小括号里， 将值用逗号分开
* 各个元素的类型可以不同
    ```Rust
    let tup:(i32,f64, u8) = (4, 5.0,1);
    let (x, y, z) = tup; //destructure
    ```
在使用的时候可以使用**模式匹配**（pattern matching）来**解构**（destructure）元组值,或者使用 `.` 标记法来访问对应位置的元素， 如 `tup.0`。  

#### array
* 在中括号中， 将值用逗号分开用于创建数组，如果元素相同可以使用 `[elem_val; size]`
* 数组中每个元素的类型必须相同
* 数组的长度也是固定的

使用数组是将数据存放在 **_stack_** 而非 _heap_ 中， 或者想要固定数量的元素， Vector 更加灵活， 可以改变长度。  
数组的类型是 `[element_type, size]`, 如 `let a:[i32;4] = [0, 1, 2, 3];`, 使用索引访问数组的元素， 索引超出范围能够通过编译(并不绝对)， 但是运行时会引发 _panic_。  

## 函数
* rust 中使用关键字 `fn` 声明函数
* 函数和变量名命名都是适用 snake case
* rust 不关心函数声明的先后

### 参数
函数有形参(parameter) 和实参(argument) 的区别， 在声明参数的时候， 必须声明每个参数的类型， 多个参数之间适用逗号(`,`) 分开。 
```Rust
fn main() {
    print_labeled_measurement(5, 'h');
}

fn print_labeled_measurement(value: i32, unit_label: char) {
    println!("The measurement is: {}{}", value, unit_label);
}
```

### 语句和表达式
Rust 中区分语句和表达式，语句（Statements）是执行一些操作但不返回值的指令。**表达式（Expressions）计算并产生一个值**。
* 函数由一系列语句组成， 可选的由一个表达式结束
* 函数的定义也是语句
* 语句不返回值， 所以**不能使用 `let` 将语句赋值给一个变量**
    ```Rust
    let x = (let y =6); // expected expression, found statement
    let y = {
        let x = 1;
        x + 3    // Ok: 块表达式的值值相当于最后一个表达式求值
        // 如果最后一行加了逗号， 则是一个表达式， 求值后是一个空的 tuple, ()
    }
    ```

### 函数的返回值
* 使用 `->` 后面跟返回值的类型， 但是不能为返回值命名
* Rust 中返回值就是最后一个表达式求值
* 想要提前返回的话使用 `return` 关键字并且指定一个值

## 控制流
### `if` 表达式
`if` 表达式允许我们根据条件来执行不同的代码分支
* 条件必须是 bool 类型
* 与表达式相关联的代码就叫分支(arm)
* 可以有 else 块
* 连续使用多个 `else if`, 最好使用 `match` 重构
* if 代码块也是一个表达式， 可以使用 `let` 赋值, 但是所有可能执行的 arm 中表达式求值后应该是相同的类型

#### 循环
rust 中提供了三种循环 `loop`,`while` 和 `for`。
* `loop` 关键字告诉 Rust 一遍又一遍地执行一段代码直到你明确要求停止(使用 `breck` 后面可选一个表达式作为 loop 表达式的值)
* `while condition{...}`当条件为真，执行循环。
* `for` 用于紧凑地遍历容器
    ```Rust
    let a = [10, 20, 30, 40, 50];
    for element in a.iter() {
        println!("the value is: {}", element);
    }
    ```

Range 搭配 for 循环可以更好地遍历：
* `Range` 由标准库提供
* 指定开始数字和结束数字， 左闭右开地生成它们之间的数字
* `rev` 方法可以反转 `Range`
    ```Rust
    for number in (1..4).rev() {
        println!("{}!", number);
    }
    ```

# 所有权
所有权是 Rust 语言最独特的特性， 它让 Rust 无需 GC（garbage collector）就可以保证内存安全。  
## 什么是所有权
所有权是 rust 的核心特性
* 所有程序在运行时都必须管理它使用计算机内存的方式
  * 有的语言有垃圾收集机制， 它会不断寻找不再使用的内存
  * 有的语言需要程序员手动管理内存
* Rust 采取的是第三种方式

在系统级编程语言中， 一个值是在 stack 上还是 heap 上对语言的行为和程序员要做的某些决定有重要影响。 
* stack 按照值的接收顺序存储， 然后按相反的顺序将它们移除(LIFO)
* stack 上的数据必须是已知的固定大小
  * 编译的时候未知或者运行时大小可能发生变化的数据必须存放在 heap 上
* heap 数据的组织性要更差一些
  * 数据放入 heap 时候， 会请求一定数量的空间
  * 操作系统在 heap 中找到足够大的空间， 标记为已用并返回一个指针
* 数据压到 stack 中比 heap上分配要快得多， 并且访问也更快（heap 中要通过指针跳转， 由于缓存的缘故， 跳转次数更多速度越慢）

**管理 heap 数据正是所有权存在的原因**， 所有权能解决下列问题：
* 跟踪代码的哪些部分在使用 heap 的哪些数据
* 最小化 heap 中重复数据使用量
* 清理 heap 上未使用的数据以避免空间不足

### 所有权规则
1. Rust 中的每一个值都有一个被称为其 **所有者**（owner）的变量
2. 值在任一时刻有且只有一个所有者。
3. 当所有者（变量）离开作用域，这个值将被丢弃。

Scope 就是程序中一个项目的有效范围。 
String 类型比基础数据类型更加复杂， 它是 heap based, 可以帮助我们理解所有权。
* 字符串字面值： 程序中写死的那些字符串值， 它们是不可变的
* Rust 中提供了 `String`: 可以在heap 中存放那些在编译时未知的字符串
  * 使用 `from` 函数从字符串字面量创建 `String` 类型 let s = `String::from("hello")`
  * 这些字符串是可以修改的， 如调用 `push_str`
* Rust 中， 对于某值而言， 当拥有它的变量走出作用范围时， 内存就会立即自动交还给操作系统
* 这实际上是将这个过程卸载 drop 函数中