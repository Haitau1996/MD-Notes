# [Rust编程语言入门教程](https://www.bilibili.com/video/BV1hp4y1k7SV)

# Intro
**Why Rust**:(性能/安全性/并发 but 难学)
* Rust 和 C/C++ 具有相同的性能, 但是很多常见的 Bug 可以在编译的时候被消灭
* 作为一种通用的编程语言, 擅长在下列场景中使用:
  * 需要运行时的速度
  * 需要内存安全
  * 更好地利用多处理器
* 比 C/C++ 等高性能的编程语言相比更加安全, 比拥有 GC 的 Java/C# 更加高效

Rust 有很多独有的概念, 和现在大多数主流的语言都不同. 

## 安装
从命令行运行[官网]`https://www.rust-lang.org/tools/install`中的相关代码:
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