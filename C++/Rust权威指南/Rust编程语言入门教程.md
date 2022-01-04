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
* 字符串字面值： 程序中写死的那些字符串值， 它们是不可变的,并不是所有字符串值都能在编写代码时候确定，所以使用场景有限
* Rust 中提供了 `String`: 可以在 heap 中存放那些在编译时未知的字符串
  * 使用 `from` 函数从字符串字面量创建 `String` 类型 `let s = String::from("hello")`
  * 这些字符串是可以修改的， 如调用 `push_str`
* Rust 中， 对于某值而言， 当拥有它的变量走出作用域时， 内存就会立即自动交还给操作系统
* 这实际上是将这个过程卸载 drop 函数中

#### 变量和数据的交互方法
多个变量可以与同一个数据使用一种独特的方式进行交互。
```Rust
let s1 = String::from("hello");
let s2 = s1;
```
<div style="text-align:center"><img src="https://s2.loli.net/2021/12/15/hLZQ3yEoF6Se1WM.png"width="30%" hight="30%" /></div>

一个 String 有三个部分组成，指针， 长度和容量， 它们是放在 stack 上的， 而字符串的内容部分放在 heap 上。当 s1 赋值给 s2 时， stack 上的部分被复制了一份， 但是 heap 上的数据只有一份。因此可能有 double free 问题， Rust 的解决办法是让 s1 失效。<div style="text-align:center"><img src="https://s2.loli.net/2021/12/15/bAoOSWMYZDK6UVn.png" width="30%" hight="30%" /></div>

* 浅拷贝
* 深拷贝
* Rust 让 s1 失效了， 我们使用移动（move） 描述这种行为

因为不会自动创建数据的深拷贝， 就运行时性能而言， 任何自动赋值的操作都是廉价的。  
如果真的想要对 heap 上的数据进行深度拷贝， 可以使用 `clone` 方法。 
* rust 提供来了 copy 这个 trait, 就可以像整数这样在 stack 上复制。
* **copy 和 drop 这两个 trait 不能共存**， 如果类型或者类型的一部分实现了 drop, 就不能实现 copy trait
    * 简单标量和他们的组合都是可以 copy 的(如 i8 构成的元组)
    * 任何需要分配内存或者某种资源的都无法实现 copy trait

### 所有权与函数、返回值
* 将值传递给函数在**语义上类似于对变量进行赋值**。将变量传递给函数将会触发移动或复制，就像是赋值语句一样。
* 函数在返回值的过程中也会发生所有权的转移。  

一个变量的所有权总是遵循下面的模式：
* 把一个值赋给其他变量的时候就会发生移动
* 当一个包含 heap 数据的变量离开作用域的时候， 其值就会被 drop 函数清理， 除非已经转移的所有权

## 引用与借用
Rust 有一个被称为引用(reference)的特性， 在调用函数的时候传入的参数有 `&` 符号，就是引用语义，它们允许你在**不获取所有权的前提下使用值**。
```Rust
// 函数声明中
fn calculate_length(s: &String) -> usize { 
    s.len() 
} 
// 在调用端
let len = calculate_length(&s1);
``` 
<div style="text-align:center"><img src="https://i.imgur.com/xCrGUAb.png" /></div>

从上图可以看出， 变量 s 并没有堆上数据的所有权，所以在离开作用域之后不会调用 `drop` 函数。把引用作为参数的行为叫做**借用**（borrowing）。
### 可变引用
```Rust
fn main() { 
    let mut s = String::from("hello");  
    change(&mut s); 
} 
 
fn change(some_string: &mut String) { 
    some_string.push_str(", world"); 
}
```
我们可以在引用中修改数据（可变引用）， 但是它的使用有特定的限制：
* 在特定作用域中， 一次只能声明一个可变引用。
* 不能同时拥有一个可变引用和不可变引用

这是为了避免数据竞争，数据竞争在满足下面的条件下会发生：
1. 有两个或者多个指针同时方位一个数据
2. 至少有一个指针用于写入数据
3. 没有任何机制来同步数据的访问

### 悬空引用
悬垂指针是说指向曾经存在的某处内存地址，但该内存已经被释放掉甚至是被重新分配另作他用了。而在Rust语言中，编译器会**确保引用永远不会进入这种悬垂状态**。
```Rust
fn dangle()->&String{
    let s = String::from("hello");
    &s //error[E0106]: missing lifetime specifier
}
```

## 切片(slice)
除了引用，Rust还有另外一种不持有所有权的数据类型：切片 （slice）。切片允许我们**引用集合中某一段连续的元素序列**，而不是整个集合。  
字符串切片是指向 String 对象中某个连续部分的引用:
```Rust
let s = String::from("hello world");
let hello = &s[0..5];
let world = &s[6..11];
```
* 形式是 `&some_str[start..end]`, 左闭右开
* 如果开头是0 或者结束在末尾则可以省略
* 字符串切片的边界必须位于有效的UTF-8字符边界内。尝试从一个多字节字符的中间位置创建字符串切片会导致运行时错误。
* 字符串字面值也是一个切片,将字符串切片作为参数使我们的API更加通用，且不会损失任何功能
```Rust
fn first_word(s: &str) -> &str { 
    let bytes = s.as_bytes(); 
 
    for (i, &item) in bytes.iter().enumerate() { 
        if item == b' ' { 
            return &s[0..i]; 
        } 
    }  
    &s[..] 
}
// in the caller
let word = first_word(&my_string[..]);
```
Rust还有其他更加通用的切片类型。  

# 结构体
## 定义 _struct_
* 使用 `struct` 关键字， 并为整个 struct 命名
* 在花括号内， 为所有字段(field) 定义名称和类型，不同字段之间用逗号隔开
* 创建 struct 实例时， 要为每个字段指定具体值
  * 无需按照声明的顺序进行指定
* 使用点标记法(`.`) 访问字段
* 一旦 struct 的实例是可变的， 那么实例的所有字段都是可变的
* struct 可以作为函数的返回值
* 如果字段名与值对应的变量名相同时候可以简写
    ```Rust
    fn build_user(email: String, username: String) -> User {
        User {
            email,
            username,
            active: true,
            sign_in_count: 1,
        }
    }
    ```
* 基于现有实例创建时候可以使用更新语法
   ```Rust
    let user2 = User {
        email: String::from("another@example.com"),
        username: String::from("anotherusername567"),
        ..user1
    };
    ```
* rust 可以定义 tuple struct(整体有名字但是字段没有名字)
    ```Rust
    struct Color(i32, i32, i32);
    let black = Color(0, 0, 0);
    ```
    * 行为像元组， 可以使用模式匹配和`.`索引元素
* 可以定义没有字段的结构体
    * 通常用于在某个类型上实现一个 trait

### struct 数据的所有权
* 如果字段中使用的是类似于 `String` 这种非引用的数据， 结构体的实例拥有自身全部数据的所有权。**只要结构体是有效的，那么它携带的全部数据也就是有效的**。
* 可以在结构体中存储指向其他数据的引用，不过这需要用到Rust中独有的生命周期功能

## Struct 使用实例
我们可以通过 直接使用变量，包装为元组和包装为 struct 等方式去求一个长方形的[面积](code/rectangle.rs),封装之后代码清晰便于维护。为了方便查看， 我们用到了编译器提供的格式化方法
* `std::fmt::Debug`
* `#[derive(Debug)]`
* `{:?}` 和 `{:#?}`

## 方法
* 和函数类似的地方： fn关键字， 名称， 参数， 返回值
* 不同的地方
    * 方法是在 `struct/enum/trait` 对象的上下文中定义（**使用 `impl` 块**）
    * 第一个参数可以是 `&self`（也可以获取其所有权或可变借用）， 表示调用方法的实例(不再需要显式传递参数)
        ```Rust
        impl Rectangle {
            fn area(&self) -> u32 {
                self.width * self.height
            }
        }
        // 在调用的地方
        println!("{}",rect.area());
        ```
    * Rust 没有指针的`->`运算符， 会自动引用或者解引用
        * 就是根据情况给调用方法的对象自动添加 `&/&mut/*`, 以便匹配方法的签名
        ```Rust
        //这两行调用效果相同
        p1.distance(&p2);
        (&p1).distance(&p2);
        ```
* 每个 struct 可以拥有多个 impl 块
### 关联函数
* 在 impl 块中定义的不将 `self` 作为第一个参数的函数（不是方法）， 类似于其他语言的静态方法
    * 如 `String::from()` 
    * 通常用于构造器
* `::` 用于关联函数或者命名空间

# 枚举与模式匹配
## 定义枚举
枚举允许我们列举所有可能的值来定义一个类型， 声明枚举需要列出所有可能的种类（**枚举变体**， variant）：
```Rust
enum IpAddrKind {
    V4,
    V6,
}
let ipv4 = IpAddrKind::V4;
```
### 嵌套
枚举类型是自定义的数据类型， 可以嵌套到 struct 中：
```Rust
struct IpAddr{
    kind:IpAddrKind,
    address: String,
}
// 在使用端
let localhost = IpAddr{
    kind:IpAddrKind::V4;
    address: String::from("127.0.0.1");
};
```
**Rust 允许数据直接附加到枚举的变体中**,其中可以嵌套任意类型的数据， 甚至是另一种枚举类型：
* 不需要使用额外的 struct 存储数据
* 每个变体可以拥有不同的类型以及关联的数据量
    ```Rust
    enum IpAddrKind{
        V4(u8,u8,u8,u8),
        V6(String),
    }
    let localhost = IpAddrKind::V4(127.0.0.1);
    let loopback = IpAddrKind::V6(String::from("::1"));
    ```

可以使用 `impl` 块为枚举定义方法。

## `Option` 枚举
* 位于标准库的预导入模块(Preclude)中
* 描述某个值可能存在(某种类型)或者不存在的情况

在其他的编程语言中， 有 Null 这个概念：
* 它是一个值， 表示没有值
* 一个变量可能处于两种状态， 空值(`null`)、非空

**但是 Rust 语言没有 `Null`**:
* 当我们想要像使用非空值那样使用 Null(如向某个空字符串调用 push_str 类似的方法)， 就会引发某种错误
* 但是 Null 的概念是有用的，因此 Rust 引入的类似的枚举类型 `Option<T>`.
    ```Rust
    enum Option<T> {
        Some(T),
        None,
    }
    let some_string = Some("a string");
    let absent_number: Option<i32> = None;//编译器无法推断， 要显式声明
    ```
    * `Option<T>`和 `T` 是不同的类型， 不可以把 `Option<T>` 当 T 使用
    * 想要使用其中的 T, 必须先将 `Option<T>` 转换为 T

## `match` : 控制流运算符
* match 作为一种强大的**控制流运算符**，允许将一个值与一系列的模式相比较，并根据匹配的模式执行相应代码。  
* 模式可由字面量、变量名、通配符和许多其他东西组成
    ```Rust
    enum Coin {
        Penny,
        Nickel,
        Dime,
        Quarter,
    }

    fn value_in_cents(coin: Coin) -> u32 {
        match coin {
            Coin::Penny => {
                println!("Lucky penny!");
                1
            },
            Coin::Nickel => 5,
            Coin::Dime => 10,
            Coin::Quarter => 25,
        }
    }
    ```
### 绑定值的模式
匹配分支另外一个有趣的地方在于它们可以**绑定被匹配对象的部分值**，而这也正是我们用于从枚举变体中提取值的方法。
```Rust
enum UsState {
    Alabama,
    Alaska,
    // --略
}
enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}
fn value_in_cents(coin: Coin) -> u32 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => { // 在这里将 State 取出来了
            println!("State quarter from {:?}!", state);
            25
        },
    }
}
```
我们可以用类似的方式去匹配 `Option<T>` 这个枚举：
```Rust
fn plus_one(x: Option<i32>) -> Option<i32> {
    match x {
        None => None,
        Some(i) => Some(i + 1),
    }
}
```
<font color=red>match 匹配的时候必须穷尽所有的可能</font>, 这样才能确保代码合法有效， 如果可能情况比较多可以使用 `_` 通配符代替其他的情况：

```Rust
let v = 0u8;
match v{
    1 => println!("one"),
    2 => println!("two"),
    _ => (),
}
```

## `if let`
处理那些只用关心某一种匹配而忽略其他匹配的情况，也可以搭配 else 使用：
```Rust
// 使用 match 
    let some_val = Some(1);
    match some_val{
        Some(1) => println!("hello from one"),
        _ => println!("hello from other val"),
    }
// 使用 if let 
    if let Some(1) = some_val{
        println!("hello from one");
    } else{
        println!("hello from other val");
    }
```

# 包、单元包及模块
* 代码组织包括下面的部分
  * 哪些细节可以暴露， 哪些细节是私有的
  * 作用域中什么名称是有效的
* Rust 的模块系统
  * Package: Cargo 的特性， 让我们构建、测试和功效 crate
  * Crate(单元包)： 一个模块树，可以产生一个库或者可执行文件
  * Module: 让我们控制代码的组织、作用域和私有路径
  * Path: 为 Struct、function 或者 module 等命名的方式
## 包与单元包
* Crate 要么是 Binary, 要么是 Library。
* Crate 的根是源代码文件， 编译器从这里开始
* 一个Package 包含一个 Cargo.toml, 描述了如何构建这些 Crates
  * 只能报验 0-1 个 library crate
  * 可以包含任意数量的 binary crate
  * 至少要有一个 crate
### Cargo 的惯例
* `src/main.rs`:
  * binary crate 的 crate root
  * crate 名与 package 名相同
* `src/lib.rs`
  * package 包含一个 library crate
  * 这是 library crate 的 root
  * crate 名和包名相同他
* 一个 Package 可以同时有 `src/main.rs` 和 `src/lib.rs`
* 一个 Package 可以有多个 binary crates:
  * 文件放在 `src/bin` 下
  * 每个文件都是单独的 binary crate

Crate 的作用是将相关的功能放到一个作用域中， 方便在项目之间共享，防止冲突。

### 定义 module 控制作用域和私有性
* Module:
  * 在一个 crate 中对代码进行分组
  * 增加可读性， 易于复用
  * 控制项目的私有性， public\private
* 建立 module
  * 使用 mod 关键字
  * 可以嵌套，可以包含其他项(struct等)的定义
  <div style="text-align:center"><img src="https://i.imgur.com/mAsomaM.png" /></div>
  
## 路径 Path
在 rust 中的路径相当于是命名空间，和文件系统中的路径差不多：
* 绝对路径： 从 crate 的 root 开始，使用 crate 名或者字面值 crate
* 相对路径： 从当前模块开始， 使用 self/super 等关键字

使用绝对路径和相对路径取决于定义代码和使用代码的模块会不会一起移动， 单独移动最好使用绝对路径。  
### 私有边界
模块不但可以组织代码， 还可以定义私有边界：
* 如果想把函数/struct 设为私有， 可以将它放在某个模块中
* Rust 中所有条目默认是私有的
* 父级模块无法访问子模块中的私有模块
* 子模块可以访问父模块中的私有模块， 同一级也可以
  
pub 关键字声明为共有，可以作用于模块、函数等：
* 放在 struct 之前， struct 为公有的， 但是下面的字段是私有的
* 放在 enum 之前， enum 为公共的， 并且**里面的所有变体也是公共的**

## use 关键字
我们可以借助use关键字来将路径引入作用域，并像使用本地条目一样来调用路径中的条目，使用use将路径引入作用域时也需要遵守私有性规则。
* 对于函数通常引入到上一级
* 对于 enum/struct 则引入到条目本身
* 同名条目： 引入到父级
* `as` 关键字可以为类型指定一个新的本地名称，也就是别名。

### pub use 重新导出名称
使用 use 将路径名称导入到作用域中， 该名称在此作用域中是<font color=red>私有</font>的（外部代码看不见）。
```Rust
mod front_of_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
    }
}
pub use crate::front_of_house::hosting;
```

使用外部包
1. 在 Cargo.toml 中添加依赖的包(自动从 crates.io 中下载)
2. use 将特定条目引入作用域

使用嵌套的路径来清理众多use语句:
* 可以在同一行内使用嵌套路径来将上述条目引入作用域
  * 路径的相同部分::{路径的差异部分}， 如 `use std::{cmp::Ordering, io}`
  * 可以在嵌套路径中使用self

通配符 `*` 将路径中所有公共条目引入，通常不这么用， 一般用于测试时候将需要的东西引入 test 模块， 或者用于预导入模块的引入。

## 拆分模块为不同的文件
* 在 mod 定义后使用分号而不是代码块会让Rust前往与当前模块同名的文件中加载模块内容。
* 目录的层级结构和模块的要相匹配

# 通用集合类型
Rust 标准库包含了一系列的集合， 它们将自己持有的数据存储在堆上， 因此无需在编译时确定数据的大小， 并且可以动态扩容。
## 动态数组
`Vec<T>` 被称为动态数组， 允许在其中存储多个相同类型的值，这些值会彼此相邻地排布在内存中。
* 调用关联函数`Vec::new()` 创建空的动态数组
* `vec!` 宏从指定的初始值创建新的动态数组
* `push` 方法在尾部添加元素(编译器可以根据上下文推断)

有两种方法可以得到动态数组的值：
* 使用索引（越界时候会 panic）
* 使用 get 方法， 返回一个 `Option<T>`,可以使用 match 匹配
    ```Rust
    let v = vec![1,2,3];
    match v.get(2){
        Some(third)=> println!("the third element is {}", third),
        None => println!("element out of range"),
    }
    ```
所有权和借用规则在引用 Vec 元素的时候保持不变。例如我们声明一个不可变的引用 `let first_ref = &some_vec[0]`, 如果要向 some_vec 添加元素， 和 C++ 中的 vector 一样， 如果空间不够， 这时候会向操作系统要更多内存， 将数组复制过去， 而原来的引用就失效了， 在 Rust 中这种操作是不合法的。  
枚举的变体可以附加不同类型的数据， 并且这些变体本身的类型是相同的， 因此可以配合动态数组使用：
```Rust
enum SpreadsheetCell {
    Int(i32),
    Float(f64),
    Text(String),
}

let row = vec![
    SpreadsheetCell::Int(3),
    SpreadsheetCell::Text(String::from("blue")),
    SpreadsheetCell::Float(10.12),
];
```

## String 类型
在 rust 中， 字符串 Byte 的集合，提供一些方法， 可以将字节解析为文本。
* 在核心语言层面提供字符串切片`str`(或`&str`)
* 标准库中提供了 String 类型
  * 可以增长， 可以修改， 可以获得所有权
  * 同样采用 UTF-8 编码

Rust 标准库还提供了很气其他的字符串类型
* `String` vs `str` 后缀： 前者可获得所有权， 后者可借用
* 可以使用不同的编码或者内存布局

创建一个新的字符串(String)
* 许多对于 `Vec<T>` 可用的操作也同样可用于 `String`
  * `let mut s = String::new(); `
  * 对那些实现了 Display trait 的类型调用 `to_string`方法
  * `String::from()` 从字面值得到 `String`

更新 String: 
* `push_str()` 方法添加字符串切片，并且不获得所有权
* `push()` 单个字符添加到 String 后
* `+` 运算符可以拼接字符串(第一个要是 String, 第二个要是切片)
  * 使用了类似于 `fn add(self, s:&str)->String` 的签名，第一个传入后丢失了所有权
  * 使用了解引用强制转换（s2的所有权保留了）
    ```Rust
    let s1 = String::from("Hello, ");
    let s2 = String::from("world!");
    let s3 = s1 + &s2; // 注意这里的s1已经被移动且再也不能被使用了
    ```
* `format!` 宏可以更灵活地拼接字符串， 和 `println!` 很像,返回一个 String 并且不会取得任何参数的所有权
    ```Rust
    let s1 = String::from("tic");
    let s2 = String::from("tac");

    let s = format!("{}-{}", s1, s2);
    ```

按照索引的语法访问 String 的元素会报错，String 是对 `Vec<u8>` 的包装：
```Rust
let len = String::from("Здравствуйте").len();
```
上面的字符串中每个 unicode 标量值占用了两个字节， 因此字节索引并不总是有效地对应一个标量值。  
Rust 有三种看待字符串的方式**字节，标量值，字形簇**，第三个更接近所谓的字母，索引操作因该消耗常量时间， 但是 String 无法保证， 它需要遍历所有内容来确定合法字符。  
使用 `&some_str[begin..end]` 获得一个切片， 切割时候必须沿着 `char` 的边界， 否则程序就会 panic。  
遍历String 的方法：
* 对于标量值， 使用 `some_string.chars()` 方法
* 对于字节， 使用 `some_string.bytes()` 方法
* 对于字形簇， 标准库没有提供

## 哈希映射
### `HashMap<K,V>` 创建和使用
* 用 key-val 对的形式存储数据， 一个 key 对应一个 value 
* Hash 函数决定了如何在内存中存放 K 和 V

使用方式(同样可以基于上下文推断类型)：
* 通过 `HashMap::new()` 创建
* 通过 `insert()` 方法插入新的元素

不在预导入模块中， 标准库对它的支持比较少， 没有内置的宏来创建
* 同构的：所有的 K 必须为同一种类型，所有的 V 必须为同一种类型
  
另外一个构建哈希映射的方法是，在一个由键值对组成的元组动态数组上使用 collect 方法， collect 方法可以把数据整合成很多种不同的集合， 所以要指明其类型：
```Rust
let teams  = vec![String::from("Blue"), String::from("Yellow")];
let initial_scores = vec![10, 50];

let scores: HashMap<_, _> = teams.iter().zip(initial_scores.iter()).collect();
```

### HashMap 和所有权
* 对于实现了 copy trait 的类型， 值会复制到 HashMap 中 
* 对于拥有所有权的值， 所有权会移动转移给 HashMap
    ```Rust
    let field_name = String::from("Favorite color");
    let field_value = String::from("Blue");

    let mut map = HashMap::new();
    map.insert(field_name, field_value);
    // filed_name和field_value从这一刻开始失效，
    ```
* 将值的引用插入到 HashMap 中，值本身不会移动(在 HashMap 的生命周期内引用的值要保持有效)

### 访问和修改元素
* get 方法， 参数为 K 类型， 返回值为 `Option<V>`  
* 使用 for 循环可以遍历值， 用模式匹配可以祛除 K,V
* HashMap 大小可以变化， 但是每个 K 只能对应一个 V 
  * K 已经存在， 更新 V
  * K 不存在， 添加新的键值对
* entry 方法检查指定的 K 是否对应的一个 v 
  * K 为参数
  * 返回值是一个枚举类型， Entry
* Entry 有 `or_insert()` 方法, K 存在就返回对应 V 的可变引用， 不存在则插入新值再返回这个值的可变引用
    ```Rust
    let text = "hello world wonderful world";
    let mut map = HashMap::new();

    for word in text.split_whitespace() {
        let count = map.entry(word).or_insert(0);
        *count += 1;
    }
    ```
### Hash 函数
默认可以抵抗 DoS 攻击， 可以中不同的 hasher 来切换到另一个函数, hasher 是实现了 BuildHasher Trait 的类型。

# 错误处理
Rust 的可靠性延伸到了错误处理领域， 在大多数情况下在编译时候就能提示错误并且处理。错误分类
* 可恢复错误，可以再次尝试
* 不可恢复错误（bug, 如访问索引超出范围）

很多语言不刻意区分这两种错误统一使用异常处理， Rust 没有异常处理机制
* 可恢复错误： `Result<T,E>`
* 不可恢复错误： `panic!` 宏

## 不可恢复错误
Rust提供了一个特殊的 `panic!` 宏, 它执行时:
* 程序会答应一个错误信息
* 展开(unwind) 、清理调用栈(stack)
* 退出程序

当panic发生时，程序会默认开始栈展开，清理遇到函数的数据， 可以选择不进行清理直接停止程序， 内存需要由操作系统进行清理。
* 在 `cargo.toml` 中加入
    ```toml
    [profile.release]
    panic = 'abort'
    ```

`panic!` 可能出现在我们写的代码或者我们依赖的代码中， 通过 `panic!` 调用函数的回溯信息来定位代码出现问题的地方。
* 为了获取回溯信息,必须在 debug 模式下(build 时候不带 `--release`)
* 设置环境变量`RUST_BACKTRACE`后再执行 `cargo run`， 如 powershell 中 <div align=center><img src="https://i.imgur.com/eMoOz4Z.png" /></div>

## 可恢复的错误和 `Result` 枚举
```Rust
enum Result<T, E> {
    Ok(T),
    Err(E),
}
```
* T 代表了 Ok 变体中包含的值类型，该变体中的值会在执行成功时返回；
* E 则代表了 Err 变体中包含的错误类型，该变体中的值会在执行失败时返回。<div align=center><img src="https://i.imgur.com/PZC2c14.png" /></div>

可以使用 match 表达式处理 Result：
```Rust
let f = File::open("hello.txt");
    let f = match f{
        Ok(file) => file,
        Err(err) => {
            panic!("Error opening file {:?}", err)
        },
    };
```
### 匹配不同的错误
但我们想要的其实是根据不同的失败原因做出不同的反应， 这时候可以根据不同的错误再做 match:
```Rust
//--- snip
 Err(error) => match error.kind() {
            ErrorKind::NotFound => match File::create("hello.txt") {
                Ok(fc) => fc,
                Err(e) => panic!("Tried to create file but there was a problem: {:?}", e),
            },
            other_error => panic!("There was a problem opening the file: {:?}", other_error),
        },
```
* match 很有用但是十分原始
* `Result<T,E>`实现（使用 match 表达式实现的）了许多接收闭包的方法, 我们使用这些方法可以让代码更加简洁

### unwrap 和 expect
`unwrap`就是 match 表达式的一个快捷方法：
* Result 返回值是Ok变体时，unwrap就会返回Ok内部的值
* Result 的返回值是Err变体时，unwrap则会替我们调用panic! 宏。
```Rust
let f = File::open("hello.txt").unwrap();
```

`expect` 在 `unwarp` 基础上附加错误信息:
```Rust
let f = File::open("hello.txt").expect("Failed to open hello.txt");
```

### 传播错误
可以将这个错误返回给调用者，将结果包裹到 Result 这个枚举中然后返回:
```Rust
use std::io;
use std::io::Read;
use std::fs::File;

fn read_username_from_file() -> Result<String, io::Error>{}
// 在调用端
let result = read_username_from_file();
match result{
    //code snip---
}
```
#### `?` 运算符
可以通过将操作符 `?` 放在 Result 值之后来实现简化（只在返回值为 Result 枚举的函数中使用）
* 如果表达式的 Result 为 Ok, 那么 Ok 变体中包含的值将作为表达式的结果，接着执行后续代码，
* 如果是 Err， 那么 Err 就是<font color=red>整个函数</font>的返回值，相当于直接 return
```Rust
let mut f = File::open("hello.txt")?;
//相当于
let mut =  match File::open("hello.txt"){
    Ok(file) => file,
    Err(e) => return Err(e),
}
// 可以链式调用
File::open("hello.txt")?.read_to_string(&mut s)?;
```
被`？`运算符所接收的错误值会隐式地被 `from` 函数处理, from 是 Trait `std::convert::From` 上的函数， 用于错误之间的转换
* 当？运算符调用from函数时，它就开始尝试将传入的错误类型转换为当前函数的返回错误类型。
* 用于针对不同错误原因返回同一种错误类型（需要实现相应的 from 函数）

## 何时使用 `panic!`
* 在定义个可能失败的函数时， 优先返回 Result,将选择权交给调用者
* 认为Err是不可恢复的，使用`panic!`

可以使用 panic!:
* 演示某些概念 ： unwarp
* 原型代码 unwarp, expect
* 测试 unwarp, expect

比编译器拥有更多信息时, 调用 unwarp 十分合理（它一定不会出现 Err 变体）

### 指导性建议
* 当某个错误可能会导致代码处于损坏状态时，最好使用 `panic!` 来处理错误
  * bad state: 某些假设， 保证或者不变性被打破

场景建议：
* 调用你的代码传入无意义的参数值: panic
* 调用外部不可控代码， 返回非法状态， 无法修噶 : panic
* 失败是可以预期的： Result
* 你的代码对值进行操作， 应该首先验证这些值: panic

### 创建自定义类型验证有效性
我们可以创建新的类型， 把验证逻辑放到构造实例的函数中。
```Rust
pub struct Guess {
    value: i32,
}
impl Guess {
    pub fn new(value:i32)->Guess {
        if value < 1 || value > 100 {
            panic!("Guess value must be between 1 and 100, got {}.", value);
        }
        Guess {
            value
        }
    }
    pub fn value(&self)->i32{
        self.value
    }
}
```

# 泛型 trait 与生命周期
重复的代码有几个危害：
* 容易出错
* 需求变更的时候需要在多处修复

使用函数消除重复有下面的步骤：
1. 定位到重复的代码
2. 将重复的代码提取至函数体中，并在函数签名中指定代码的输入和返回值
3. 将两段重复代码实例改为调用函数

## 泛型数据类型
泛型是将具体类型或者其他属性的抽象代替：
* 编写的不是最终的代码， 而是一种模板， 里面有一些占位符（`<T>`）
* 编译的时候将占位符替换为具体的类型
* 函数定义中：
  * 参数类型和返回值类型为 T 
* struct 结构体中定义，一般用于字段,可以使用多个泛型类型参数
* enum 中使用泛型， 一般用于变体中,某个变体也可以不使用泛型
* 方法定义中, 可以针对具体类型做特化,泛型的类型参数可以和对应的结构体不同
    ```Rust
    // 函数中定义类型
    fn largest<T>(list: &[T]) -> T {
        // code snip ---
    }
    // struct 
    struct Point<T,U>{
        x: T, 
        y: U,
    }
    // enum
    enum Option<T>{
        Some(T),
        None,
    }
    // method
    impl<T,U> Point<T,U>{
        fn mixup<V,W>(self, other:Point<V,W>) ->Point<T,W>{
            Point{
                x:self.x,
                y:other.y,
            }
        }
    }
    impl Point<i32, i32>{
        fn x(&self)->&i32{
            &self.x
        }
    }
    ```

Rust 语言会在编译的时候做单态化， 将泛型类型替换为具体的类型， 因此使用泛型不影响性能。

## Trait
trait（特征）被用来向Rust编译器描述某些特定类型拥有的且能够被其他类型共享的功能：
* trait 抽象的定义共享行为
* trait bounds: 泛型类型参数指定为实现了特定行为的类型(实现了某些 trait)
* 和其他语言的接口类似(不完全相同)

### 定义和实现
trait 提供了一种将特定方法签名组合起来的途径，它定义了为达成某种目的所必需的行为集合。
* 使用关键字 trait
* 只有方法的签名， 没有具体的实现
* 可以有多个方法， 每个方法签名各占一行， 以 `;` 结尾
* 实现该 trait 的类型必须提供方法的具体实现
    ```Rust
    pub trait Summary{
        fn summarize(&self)-> String;        
    }

接下来要做的就是**在类型上实现 _trait_**:
* 与为类型实现方法（`impl SomeType{}` ）类似
* 不同在于要写入 tarit 的信息
  * impl <font color=red> SomtTrait for</font> SomeType{...}
  * impl 块中要对函数有具体的实现 
    ```Rust
    pub struct Tweet {
        pub username: String,
        pub content: String,
        pub reply: bool,
        pub retweet: bool,
    }

    impl Summary for Tweet {
        fn summarize(&self) -> String {
            format!("{}: {}", self.username, self.content)
        }
    }
    ```

实现 trait 的约束条件：
* 这个类型 或者 这个 trait 是在本地 crate 中定义的
* 无法为外部类型来实现外部的 trait
  * 孤儿原则，如果没有这个规则， 两个 crate 可以为同一个类型实现同一个 trait

默认实现：有时为 trait 中的某些或所有方法都提供默认行为非常有用，它使我们无须为每一个类型的实现都提供自定义行为。
* 默认实现中可以调用其他方法， 及时它们没有默认实现(这时候有点像 Template Method 设计模式)
```Rust
pub trait Summary {
    fn summarize(&self) -> String {
        String::from("(Read more...)")
    }
}
impl Summary for Tweet {
    // no implementation of summarize, use default
}
```

### trait 作为参数和返回值
如果想让函数的参数为实现指定 Trait 的类型， 可以这样写:
* impl trait 语法： 适用于简单情况
    ```Rust
    pub fn notify(item: impl Summary) {
        println!("Breaking news! {}", item.summarize());
    }
    ```
* trait bound 语法,在复杂情况下更简洁， 如有多个参数
    ```Rust
    pub fn notify<T:Summary>(item: T， next_item:T) {
        println!("Breaking news! {}", item.summarize());
    }
    ```
* `+` 指定多个 Trait Bound:
    ```Rust
    pub fn notify(item: impl Summary + Display) {}
    pub fn notify<T: Summary + Display>(item: T) {}
    ```
* 可以在函数签名后使用 where 子句：
    ```Rust
    fn some_function<T: Display + Clone, U: Clone + Debug>(t: T, u: U) -> i32 {}
    // 使用 where 子句
    fn some_function<T, U>(t: T, u: U) -> i32
        where 
            T: Display + Clone,
            U: Clone + Debug
    {}
    ```

Trait 也可以作为返回类型：
* impl trait 语法：`fn returns_summarizable() -> impl Summary{}`
  * 只能返回确定的通一种类型， 如果是两种不同但实现了 trait 的类型， 代码会报错
* trait bound 
    ```Rust
    fn largest<T: PartialOrd + Copy>(list: &[T]) -> T {
        let mut largest = list[0];
        for &item in list.iter() {
            if item > largest {
                largest = item;
            }
        }
        largest
    }
    ```

使用 trait bound **有条件地实现方法**：
* 通过在带有泛型参数的 impl 代码块中使用 trait约束，我们可以单独为实现了指定trait的类型编写方法。
    ```Rust
    use std::fmt::Display;
    struct Pair<T> {
        x: T,
        y: T,
    }
    impl<T> Pair<T> {
        fn new(x: T, y: T) -> Self {
            Self {
                x,
                y,
            }
        }
    }

    impl<T: Display + PartialOrd> Pair<T> {
        fn cmp_display(&self) {
            if self.x >= self.y {
                println!("The largest member is x = {}", self.x);
            } else {
                println!("The largest member is y = {}", self.y);
            }
        }
    }
    ```
* 同样可以为实现了某个trait的类型有条件地实现另一个trait(覆盖实现),如下面这段来自标准库的代码， 作用就是为所有实现 Display 和 Sized Trait 的类型实现 `ToString` Trait
    ```Rust
    #[cfg(not(no_global_oom_handling))]
    #[stable(feature = "rust1", since = "1.0.0")]
    impl<T: fmt::Display + ?Sized> ToString for T {
        #[inline]
        default fn to_string(&self) -> String {
            let mut buf = String::new();
            let mut formatter = core::fmt::Formatter::new(&mut buf);
            // Bypass format_args!() to avoid write_str with zero-length strs
            fmt::Display::fmt(self, &mut formatter)
                .expect("a Display implementation returned an error unexpectedly");
            buf
        }
    }
    // 因此如 2 作为整型实现了 display ， 就可以调用 toString 方法
    let s:String = 2.to_string();
    ```

## 生命周期
Rust 的每个引用都有自己的生命周期，对应着引用保持有效性的作用域。
* 在大多数时候，生命周期都是隐式且可以被推导出来的
* 当引用的生命周期可能以不同的方式相互关联时，我们就必须手动标注生命周期。

### 避免悬垂引用
在[实例](code/live_long_enough.rs)中， 被引用的变量已经离开作用域依旧使用引用， 就会报错(error[E0597]: `x` does not live long enough)。  
Rust 编译器拥有一个借用检查器 （borrow checker），它被用于比较不同的作用域并确定所有借用的合法性。如下图所示， <div align=center><img src="https://i.imgur.com/HpR5cwN.png" /></div>

因此， 只需要让变量的生命周期范围覆盖其引用的生命周期，就能确保其合法。

### 函数中的泛型生命周期
```Rust
fn longest(x: &str, y: &str) -> &str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}
```
上面这段代码中， 因为无法确定返回值的生命周期和参数的生命周期的关系， 故在调用的时候会出错。 解决的办法就是在函数签名中用类似于泛型的语法标注生命周期：
```Rust
fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}
```
生命周期标注的特点：
* 不会改变任何引用的生命周期长度。
* 使用了泛型生命周期的函数也可以接收带有任何生命周期的引用
* 标注本身会被用于**描述多个引用生命周期之间的关系，但不影响生命周期**

语法：
* 生命周期的参数名
  * 以`'`开头
  * 且通常使用很短全小写字符
* 生命周期的标注位置
  * 放在引用的 `&` 符号后面
  * 使用空格将标注和引用类型分开<div align=center><img src="https://i.imgur.com/TONPf9Y.png" /></div>

函数签名中的生命周期标注：我们同样在函数名与参数列表之间的尖括号内声明泛型生命周期参数。当我们将具体的引用传入 longest 时，被用于替代 `'a` 的具体生命周期就是作用域 x 与作用域 y 重叠的那一部分, 返回的引用在这个重叠区域中都是有效的。
```Rust
    let string1 = String::from("long string is long");
    let result;
    {
        let string2 = String::from("xyz");
        result = longest(string1.as_str(), string2.as_str());
        //borrowed value does not live long enough
    }
    println!("The longest string is {}", result);
```

### 深入理解生命周期
指定生命周期的方式往往取决于函数的具体功能。
* 当函数返回一个引用时，返回类型的生命周期参数必须要与其中一个参数的生命周期参数相匹配。
* 否则只可能指向一个创建于函数内部的值， 离开函数后就成了悬垂引用
  * 这时候可以直接返回值， 让后传递所有权
    ```Rust
    fn longest<'a>(x: &str, y: &str) -> &'a str {
        let result = String::from("really long string");
        result.as_str()//error[E0597]: `result` does not live long enough
    }
    ```

### 结构体定义中的生命周期标注
结构体中可包括：
* 自持有的类型
* 引用：需要在每个引用中添加生命周期标注
    ```Rust
    struct ImportantExcerpt<'a> {
        part: &'a str, // 结构体持有的引用生命周期至少要长于结构体
    }
    ```

### 生命周期的省略
在某些特定情况下 Rust 程序员总是在一遍又一遍地编写同样的生命周期标注，这样的场景是可预测的，而且有一些明确的模式。于是，Rust 团队决定将这些模式直接写入编译器代码中，使借用检查器在这些情况下可以自动对生命周期进行推导而无须显式标注。
* 省略规则并不能提供完整的推断
  * 应用规则后生命周期依旧模糊 -> 编译错误
  * 解决办法就是手动添加标注

函数参数或方法参数中的生命周期被称为输入生命周期，而返回值的生命周期则被称为输出生命周期。省略的具体三个规则：
* 每一个引用参数都会拥有自己的生命周期参数。（输入规则）
* 当只存在一个输入生命周期参数时，这个生命周期会被赋予给所有输出生命周期参数（输出规则）
* 当拥有多个输入生命周期参数，而其中一个是`&self`或`&mut self`时，self 的生命周期会被赋予给所有的输出生命周期参数。
    ```Rust
    fn first_word(s: &str) -> &str {...}
    // 应用第一条规则
    fn first_word<'a>(s: &'a str) -> &str {...}
    // 再应用第二条规则
    fn first_word<'a>(s: &'a str) -> &'a str {...}
    ```

### 方法定义中的生命周期标注
* 声明和使用生命周期参数的位置取决于它们是与结构体字段相关，还是与方法参数、返回值相关。
* struct 字段的生命周期名
  * 在 impl 后声明
  * 在 struct 名后使用
  * 生命周期是 struct 类型的一部分
* 在impl代码块的方法签名中
  * 引用可能是独立的，也可能会与结构体字段中的引用的生命周期相关联。
  * 由于规则三， 标注并不是必须的

### 静态生命周期
`'static`:整个程序的持续时间
* 所有字符串字面值都是这个生命周期

为引用指定 `'static` 时候需要注意：
* 是否需要引用在整个生命周期内都存活

# 编写自动化测试
## 如何编写测试函数
Rust 语言中的测试是一个函数， 包含三个部分
1. 准备所需的数据或状态
2. 调用需要测试的代码
3. 断言运行结果与我们所期望的一致

解剖测试函数
* 测试函数需要使用 test 属性（attribute） 进行标注
  * Attribute 是代码的元数据
  * 加上 `[#test]` 到 fn 前就可以将函数转为测试函数
* `cargo test` 运行所有的测试函数
* 当使用 cargo 创建 library 项目的时候， 会生成一个 test module， 里面有一个测试函数
  * 可以添加任意数量的 test module 和 test 函数
* 测试函数 panic 就表示测试失败
* 每个测试函数运行在一个独立的新线程
* 主线程会监视这些测试线程， 看见测试线程挂掉了， 就将它标记为失败

### 测试相关的宏
`assert!` 宏来自标准库， 用来确定某个状态是否为 true
* true 测试通过
* false 调用 `panic!`, 测试失败

`assert_eq!` 和 `assert_ne!` 做相等性测试：
* 来自标准库
* 判断两个参数相等或者不相等
* 断言失败： 自动打印两个参数的值
  * 调用 debug 格式打印
  * 要求参数实现了 `PartialEq` 和 `Debug` 两个 trait

**自定义错误信息**：  
可以向上面三个宏添加**可选的**自定义消息
* 失败时候自定义消息和失败消息都会打印
* `assert!`：第一个参数必选， 自定义消息为第二个参数
* 剩下两个断言前两个参数必选， 第三个参数作为自定义消息
* 自定义消息参数会传给 `format!` 宏，可以使用 `{}` 占位符

### 使用 should_panic 检查 panic
检查代码是否返回了正确的结果，确认代码能否按照预期处理错误状况同样重要。
* 编写测试在某些特定情况下是否发生 painc
* `should_panic`属性，使代码 panic 时候测试通过， 不发生 panic 时候执行失败
    ```Rust
    #[cfg(test)]
    mod tests {
        use super::*;
        #[test]
        #[should_panic]
        fn greater_than_100() {
            Guess::new(200);
        }
    }
    ```
* 在其中添加一个可选的 expected 参数
  * 检查失败的错误信息中是否包含有指定的文字
    ```Rust
    #[test]
    #[should_panic(expected = "Guess value must be less than or equal to 100")]
    fn greater_than_100() {
        Guess::new(200);
    }
    ```
### 测试中使用 `Result<T,E>`
我们也可以不使用上面三个 assert 宏，无需 painc， 可以使用 `Result<T,E>` 作为返回类型编写测试
  * 返回 Ok: 测试通过
  * 返回 Err: 测试失败
    ```Rust
    #[test]
    fn it_works() -> Result<(), String> {
        if 2 + 2 == 4 {
            Ok(())
        } else {
            Err(String::from("two plus two does not equal four"))
        }
    }
    ```
* 不要在 `Result<T,E>`编写测试时候添加 `should_panic` 属性， 因为它们返回 Result,不会发生恐慌

## 控制测试的运行方式
可以添加命令行参数改变 cargo test 的行为：
* 默认行为
  * 并行运行
  * 所有测试
  * 不显示所有输出（指测试通过的情况下），读取相关的输出更容易
* 命令行参数
  * 针对 cargo test的参数紧跟 cargo test之后`cargo test --help`
  * 针对可执行程序， 放在 `-- ` 之后（有空格）`cargo test -- --help`。

**串行或者并行地测试**:
* 默认使用多个线程运行：
  * 测试之间不会相互依赖
  * 不会依赖于某个共享的状态（工作目录、环境变量等）
* `--test-thread` 参数跟着线程的数量

**显式函数输出**：
* 默认情况下，Rust的测试库会在测试通过时捕获所有被打印至标准输出中的消息（不显示）
  * 只要测试顺利通过，它所打印的内容就无法显示在终端上
  * 在测试失败时，我们才能在错误提示信息的上方观察到打印至标准输出中的内容
* `cargo test -- --nocapture` 

**按测试名称运行测试**：
* 将测试的名称（一个或者多个）作为 cargo test 的参数
  * `cargo test one_handrad`:单个测试， 指定名称
  * `cargo test add`: 多个测试，指定测试名的一部分或者模块名称
    ```Rust
    #[cfg(test)]
    mod tests {
        use super::*;

        #[test]
        fn add_two_and_two() {
            assert_eq!(4, add_two(2));
        }
        #[test]
        fn add_three_and_two() {
            assert_eq!(5, add_two(3));
        }
        #[test]
        fn one_hundred() {
            assert_eq!(102, add_two(100));
        }
    }
    ```

**忽略测试**：
`[ignore]` 属性用于某些非常耗时的测试，排除在正常的测试运行之外：
* `cargo test -- --ignored` 添加针对二进制程序的参数覆盖默认行为

## 测试的组织和结构
Rust 对测试分为单元测试和集成测试
* 单元测试： 小， 专注， 针对某个模块进行隔离的测试，可以测试 private 函数
  * test 模块上的 `#[cfg(test)]` 标注（cfg 为 configuration缩写）
    * 只有在运行 cargo test 才编译和运行代码
    * 运行 cargo build 则不会
* 集成测试： 在库外部， **只能访问公共接口**， 在测试中可能使用多个模块
  * 目的是验证被测试的多个部分是否能够**正确地一起工作**
  * 覆盖率非常重要
  * 使用的时候要创建和 src 并列的 tests 目录
  * 目录下的每个文件都是一个单独的 crate
    * 需要将被测试库导入
    * 无需 `cfg`
  * 运行指定的集成测试
    * 运行单个测试: `cargo test [test_func_name]`
    * 对测试文件的所有函数测试: `cargo test --test [file_name]`
  * 可以建立文件夹， 将我们需要的公共部分（非测试文件）放入其中， Rust就不会再将文件夹的模块视作一个集成测试文件了
* 针对 binary crate 的集成测试（将主要内容放到 lib.rs 中）
  * 不能在 tests 目录下建立集成测试
  * 也无法把 main.rs 的函数导入作用域
  * 因为只有 library crate 才能暴露给其他 crate 使用 

# 编写一个命令行程序
**读取命令行**：  
使用Rust标准库提供的`std::env::args`函数读取命令行的参数值， 它返回一个迭代器， 调用迭代器上的 `.collect()`方法得到参数值的集合。  
**读取文件**：  
引入标准库中的`std::fs`模块读取文件,`fs::read_to_string()` 接受文件名作为参数， 返回一个 `Result<String>`, 因此可以加上 `expect()` 做错误处理。

## 重构： 增强模块化程度和错误处理能力
[前面的实现](code/minigrep_v1.rs)有很大的问题，需要拆分 main 函数， 需要将 query 和 filename 封装到一个结构体中， 需要更好地处理不合法的输入信息，二进制程序关注点分离有下面的指导原则：
1. 将程序拆分为 main.rs 和 lib.rs ，并将实际的业务逻辑放入 lib.rs 
2. 当命令行解析逻辑相对简单时，将它留在 main.rs 中也无妨
3. 当命令行解析逻辑开始变得复杂时，同样需要将它从 main.rs 提取至 lib.rs 中

我们首先做的就是将读取命令行的部分[分离](code/minigrep_v2.rs),改进了模块化。然后可以使用 Result 枚举来[实现](code/minigrep_v3.rs)错误的处理，接下来将剩余的部分也提取到[函数](code/minigrep_v4.rs)中,接下来要将代码迁移到 [lib.rs](code/minigrep_src_v1/lib.rs) 中, 同时修改 [main.rs](code/minigrep_src_v1/main.rs)。
## TDD 开发库功能
1. 编写一个会失败的测试， 运功该测试， 确保它是按照预期的原因失败
2. 编写或修改刚足够的代码， 让新测试通过
3. 重构刚刚添加/修改的代码， 确保测试始终能够通过
4. 返回步骤1，继续

使用 TDD 实现一个 [search](code/tdd.rs).
## 使用环境变量
使用环境变量可以允许用户只配置一次环境变量，就能让配置的选项在整个终端会话中一直有效。
```Rust
// in lib.rs , implements both senstive case and insenstive case
// then add a boolean in Config...
use std::env;
let case_sensitive = env::var('CASE_SENSITIVE').is_err();
// code snip
```
## 将错误信息写入标准错误
* 标准输入 `stdout`
* 标准错误 `stderr`
    * `eprintln!` 将信息输出到 stderr

# 函数式语言特性： 迭代器和闭包
## 闭包：能够捕获环境的匿名函数
* 是匿名函数
* 可以保存为变量， 作为参数或者函数的返回值
* 可以在一个地方创建闭包， 然后再另一个上下文中调用来完成运算
* 可以从其定义的作用域中捕获值

如我们模拟一个耗时比较多的操作， 希望在必要的时候才调用， 并且只调用一次， 就可以将它封装在一个闭包中：
```Rust
let expensive_closure = |num| {
    println!("calculating slowly...");
    thread::sleep(Duration::from_secs(2));
    num
};
```
### 闭包的类型推断和标注
* **不强制要求标准参数和返回值类型**，类型信息是暴露给用户的显式接口的一部分，闭包被存在变量中，不会暴露给用户。
* 通常很短小， 在狭小上下文中工作， 编译器通常能推断出来
* 闭包的参数和返回值会被推倒为具体类型， 因此不能用不同参数调用同一个闭包或者同一个闭包返回不同的类型

### 使用泛型参数和Fn trait来存储闭包
我们还可以将闭包的结果存储到一个结构体中， 这种模式被称为记忆化或惰性求值
* 需要结果的时候才执行该闭包
* 将运行的结果缓存下来

Struct 需要直到字段的类型（因此要指明闭包类型）， 每个闭包实例都有自己唯一的匿名类型， 即使签名完全一样， 所以需要使用泛型和 trait bound.  
标准库定义了 fn trait， 所有的闭包至少实现了下面其中一个：
* Fn
* FnMut
* FnOnce

```Rust
struct Cacher<T>
    where T: Fn(u32) -> u32 // 闭包接受一个 u32 返回一个 u32s
{
    calculation: T,//闭包
    value: Option<u32>,//缓存
}
// 缓存的逻辑不是自动生成， 需要我们自己实现
impl<T> Cacher<T>
where T: Fn(u32) -> u32
{
    fn new(calculation: T) -> Cacher<T> {
        Cacher {
            calculation,
            value: None,
        }
    }

  fn value(&mut self, arg: u32) -> u32 {
        match self.value {
            Some(v) => v,
            None => {
                let v = (self.calculation)(arg);
                self.value = Some(v);
                v
            },
        }
    }
}
```
上面的 Cacher 有局限性：
* 假设 value 方法为不同的 arg 返回相同的值
  * value 存一个 HashMap， key 为 arg, val 为执行结果
* 只能接收一个获取u32类型参数并返回u32类型的值的闭包。

### 捕获上下文环境
闭包**可以捕获自己所在的环境并访问自己被定义时的作用域中的变量**。
* 会产生内存开销
```Rust
let x = 4;
let equal_to_x = |z| z == x; //Ok
fn equal_to_x(z: i32) -> bool { z == x }//can't capture dynamic environment in a fn item
let y = 4;
assert!(equal_to_x(y));
```
捕获值的方式有三种：
1. 取得所有权: FnOnce
2. 可变借用： FnMut
3. 不可变借用： Fn 

Rust 会自动推导出它需要使用的trait
* 所有闭包都实现了 FnOnce
* 没有移动捕获变量的实现了 FnMut
* 无需可变访问捕获变量的闭包实现了 Fn

move 关键字可以强制闭包取得它所使用环境值的所有权。
* 闭包传入新线程时相当有用，它可以将捕获的变量一并移动到新线程中去

## 迭代器
迭代器模式： 为序列中的每一个元素执行某些任务。
* 迭代器负责两个事情
  * 遍历每个项
  * 确定序列的遍历何时完成
* Rust 中的迭代器
  * 惰性（layzy）：除非你主动调用方法来消耗并使用迭代器，否则它们不会产生任何的实际效果。
    ```Rust
    let v1 = vec![1, 2, 3];
    let v1_iter = v1.iter();
    for val in v1_iter {
        println!("Got: {}", val);
    }
    ```

### Iterator Trait
* 所有迭代器都实现了 Iterator trait
    ```Rust
    pub trait Iterator {
        type Item;
        fn next(&mut self) -> Option<Self::Item>;
        // 这里省略了由Rust给出的默认实现方法
    }
    ```
* type item 和 Self::Item 定义和 trait 的关联类型
  * 必须要定义一个具体的 Item 类型，而这个Item类型会被用作 next 方法的返回值类型。
  * 只要求实现者手动定义一个方法：next方法。
    * 每次返回一项， 包裹在 `Some` 中，直到返回 `None`

几个迭代的方法：
* `iter` 方法： 在不可变引用上创建迭代器
* `into_iter` 方法： 创建的迭代器会获得所有权
* `iter_mut` 方法： 迭代可变引用

### 消耗迭代器的方法
标准库为 Iterator trait提供了多种包含默认实现的方法，其中的一部分会在定义中调用 next 方法：
* 需要在实现Iterator trait时手动定义 `next` 方法
* 调用 `next` 的方法叫做消耗型适配器

### 产生其他迭代器的方法
Iterator trait 还定义了另外一些被称为迭代器适配器（iterator adaptor）的方法
* 可以使你将已有的迭代器转换成其他不同类型的迭代器
* 可以链式地调用多个迭代器适配器完成一些复杂的操作，同时保持代码易于阅读。

### 使用闭包捕获环境
* `filter` 方法
  * 接受一个闭包
  * 在遍历迭代器的每个元素的时候返回 bool 类型
    * 如果结果为 true， 则包含在新的适配器中
    * 否则不包含
    ```Rust
    #[derive(PartialEq, Debug)]
    struct Shoe {
        size: u32,
        style: String,
    }
    fn shoes_in_my_size(shoes: Vec<Shoe>, shoe_size: u32) -> Vec<Shoe> {
        shoes.into_iter()
        .filter(|s| s.size == shoe_size)
        .collect()
    }
    ```

### 创建自定义迭代器
* 要实现 next 方法
  * 先定义一个结构体（实现相应的构造器...）
  * 然后实现 next 方法
    ```Rust
    impl Iterator for Counter {
        type Item = u32;
        fn next(&mut self) -> Option<Self::Item> {
            self.count += 1;
            if self.count < 6 {
                Some(self.count)
            } else {
                None
            }
        }
    }
    ```

# _cargo, cartes.io_
## 通过 release profile 发布构建
Rust中的发布配置 （release profile）是一系列**预定义**好的配置方案
* 可以自定义：使用不同的配置， 对代码编译有更多控制
* 每个 profile 配置都独立于其他的 profile
* 两个主要的 profile
  * dev profile: 适用于开发， `cargo build`
  * release profile: 默认在正式发布时使用 `cargo build --release`

### 自定义 profile
可以在 cargo.toml 中添加 `[profile.xxxx]`, 覆盖我们想要修改的子集,配置的默认值和完整选项参考官网的[文档](https://doc.rust-lang.org/cargo/reference/profiles.html).
```toml
[profile.dev]
opt-level = 0

[profile.release]
opt-level = 3
```
## 将包发布到 _crates.io_ 上
_crates.io_ 可以通过发布包来共享你的代码：
* crate的注册表在 [crates.io](https://crates.io) 上
* 托管的包大部分都是开源的

### 文档注释
* 文档注释： 用于生成文档
  * 文档为 html 格式
  * 显示公共 API 的文档注释： 如何使用 API
  * 使用三斜线，并且支持 markdown 语法
* `cargo doc` 生成 HTML 文档
  * 运行 rustdoc 工具
  * 生成文档在 target/doc 中
  * `cargo doc --open` 可以直接打开
* 比较常用的区域有 
  * Examples
  * Panics
  * Errors
  * Safety 
* 文档注释可以作为测试
  * `cargo test` 会在执行时将文档注释中的代码示例作为测试去运行
* `//!`
  * 它可以为包裹当前注释的外层条目（而不是紧随注释之后的条目）添加文档
  * 这种文档注释通常被用在包的根文件

### `pub use` 导出方便使用的公共 API
在开发过程中建立起来的 cartes 组织结构对于使用者并不是特别友好
* 代码模块可能是一个包含多个层次的树状结构, 使用者难以找到
* 引用也困难，如 `use my_crate::some_module::another_module::UsefulType;`

解决的办法就是使用 `pub use`:
* 不需要重新组织内部代码结构
* 重新导出操作会取得某个位置的公共条目并将其公开到另一个位置

### 创建账号并且发布
* 发布之前需要创建账号并且获得 API token
* 运行命令： `cargo login [API token]`
  * API令牌存入 _～/.cargo/credentials_ 文件
* 在发布之前，我们需要在Cargo.toml 文件的[package]区域中为这个包添加一些元数据（metadata）。 
  * 独一无二的名称
  * description：会出现在搜索结果中
  * license
  * version
  * auth
* 发布： 使用 `cargo publish`命令
* crate 一旦发布， 就是永久性的：该版本无法覆盖， 对应代码无法删除
  * 保证所有依赖于 _crates.io_ 的包都能一直被正常构建
* 修改 crate 之后，修改 toml 文件中的 version
* 不能移除某一个老版本的包，但我们仍然可以阻止未来的新项目将它们引用为依赖
  * `cargo yank --vers 1.0.1` 
  * `cargo yank --vers 1.0.1 --undo` 取消撤回

## cargo 工作空间
但随着项目规模逐渐增长，你也许会发现自己的代码包越来越臃肿，并想要将它进一步拆分为多个代码包。
* cargo 工作空间：帮助开发者管理多个相互关联且需要协同开发的包。
  * 其实就是一套共享 cargo.lock 和输出文件夹的包

在工作中间下的 `Cargo.toml` 中写入：
```toml
[workspace]

members = [
    "adder",
    "add-one",
    "add-two",
]
```
Cargo 不会默认工作空间中的包互相依赖，我们需要在 adder `Cargo.toml` 中写入依赖：
* `cargo run -p adder` 就可以运行指定包
```toml
[dependencies]
add-one = { path = "../add-one" }
```
工作空间中只有一个 `Cargo.lock` 文件， 在顶层目录下
* 保证 所有 crate 使用的依赖版本都相同
* 工作空间内所有的 crate 都互相兼容

## 安装二进制的 crates
* cargo install 安装来自 `crates.io` 的 crate
  * 只能安装具有二进制目标的 crate 
  * 拥有 `src/main.rs` 或者其他被指定为二进制入口的文件
  * 安装在 `$HOME/.cargo/bin` 中

# 智能指针
指针（pointer）是一个通用概念，它指代那些包含内存地址的变量。智能指针是这样的数据结构：
* 行为和指针类似
* 有额外的元数据和功能

引用计数智能指针类型：
* 通过记录所有者的数量来使一份数据被多个所有者同时持有
* 在没有任何所有者时自动清理数据

之前介绍的引用和智能指针不同， 引用只是借用数据， 而智能指针很多时候都拥有它所指向的数据。  
**智能指针的实现**：
* 智能指针通常使用 struct 实现，并且实现了 
  * Deref(拥有引用一致的行为) 和 Drop(自定义智能指针离开作用域时候运行的代码) 这两个 trait

## 使用 `Box<T>` 在堆上分配数据
`Box<T>` 是最简单的智能指针：
* 允许我们在 heap 上存储数据
* stack 上存放指向 heap 数据的指针
* 适用于需要“间接存储”的场景
* 没有性能开销
* 没有额外的功能<div align=center><img src="https://i.imgur.com/pk8t3HT.png" width="30%"/></div>

它在下面这些场景中常用：
* 当你拥有一个无法在编译时确定大小的类型，但又想要在一个要求固定尺寸的上下文环境中使用这个类型的值时。
* 当你需要传递大量数据的所有权，但又不希望产生大量数据的复制行为时
* 当你希望拥有一个实现了指定trait的类型值，但又不关心具体的类型时

使用`Box<T>`在堆上存储数据：
```Rust
fn main() {
    let b = Box::new(5);
    println!("b = {}", b);
}
```
### 使用装箱赋能递归类型
* 在编译的时候， Rust 需要直到一个类型所占用的空间大小
* 而递归类型的大小无法在编译的时候确定<div align=center><img src="https://i.imgur.com/3ySJ9BC.png" width="30%"/></div>
* Box<T> 类型的大小是确定的

关于 Cons List
* 链接列表是一种来自Lisp编程语言与其方言的数据结构
* Cons List 的每个成员由两个元素组成
  * 当前项的值
  * 下一个元素
* Cons List 最后一个成员只包含一个 Nil 值，没有下一个元素

我们接下来想要实现这样的链表,最符合直觉的实现是不对的（error info: recursive type List has infinite size）：
```Rust
enum List {
    Cons(i32, List),
    Nil,
}
```
* 对于非递归类型， Rust 对于枚举使用的是占用空间最大的那个变体的尺寸
* `Box<T>` 是一个指针， Rust 知道它所占用的空间大小<div align=center><img src="https://i.imgur.com/cWEVQua.png" width="20%"/></div>
    ```Rust
    enum List{
        Cons(i32, Box<List>),
        Nil,
    }
    fn main() {
        let list = Cons(1,
            Box::new(Cons(2,
                Box::new(Cons(3,
                    Box::new(Nil))))));
    }
    ```

## Deref Trait
* 实现 Deref 这个trait 让我们可以==自定义解引用运算符`*`的行为==
* 智能指针可以像常规引用一样行为

常规的引用也是一种指针：
```Rust
let x = 5;
let y = &x;
let z = Box::nex(x);
assert_eq!(5,x);
assert_eq!(5,*y);
assert_eq!(5,*z);
```
### 定义自己的智能指针
* `Box<T>` 实际上被定义成一个拥有一个元素的 tuple strut，我们在一个 tuple 上实现 Deref 这个 Trait 就可以实现解引用运算符
    ```Rust
    use std::ops::Deref;
    struct MyBox<T>(T);
    impl<T> MyBox<T>{
        fn new(x:T)->MyBox<T>{
            MyBox(x)
        }
    }
    impl<T> Deref for MyBox<T>{
        type Target = T;
        fn deref(&self)->&T{
            &self.0
        }
    }
    ```

### 函数和方法的隐式解引用转换
deref coercion(kəʊˈɜːʃn, 强制) 是Rust为函数和方法的参数提供的一种便捷特性， 其原理如下：  
* 假设 T 实现了 Deref trait:
  * Deref coercion 可以把 T 的引用转化为 T 经过 Deref 操作后生成的引用
* 当某个类型的引用传递给函数或者方法， 但是类型和定义的参数类型不匹配：
  * Deref coercion 就会自动发生
  * 编译器会对 deref 进行一系列调用， 来把它转为所需要的参数类型
    * 编译时完成， 没有性能开销
    ```Rust
    // 省略上面已经有的 myBox 实现的代码
    fn hello(name:&str){
        println!("hello {}",name);
    }
    fn main(){
        let m = MyBox::new(String::from("Rust"));
        hello(&m);
        // &m 实际为 &MyBox<String>
        // deref &String
        // deref &str ， 这样类型就匹配了, 上面一行相当于下面这个复杂且难以理解的调用
        hello(&(*m)[..]);
    }
    ```
### 解引用转换与可变性
使用 DerefMut trait 能够重载可变引用的 `*` 运算符:
* 当T: Deref<Target=U> 时，允许 &T 转换为 &U。
* 当T: DerefMut<Target=U> 时，允许 &mut T 转换为 &mut U。
* **当 T: Deref<Target=U> 时，允许 &mut T 转换为 &U**(反过来不行)

## Drop trait
实现 Drop trait，允许我们在**变量离开作用域时执行某些自定义操作**
* 如 文件、网络资源的释放
* 任何类型都可以实现 Drop trait(它在预导入模块)
* `drop()` **方法**不能手动调用(Drop trait 的目的就是自动释放)，想提前清理使用`drop()`**函数**
    ```Rust
    struct CustomSmartPointer {
        data: String,
    }

    impl Drop for CustomSmartPointer {
        fn drop(&mut self) {
           println!("Dropping CustomSmartPointer with data `{}`!", self.data);
        }
    }
    ```

## `Rc<T>`
有时候， 单个值会被多个数据所有， 如 Graph 中， 需要保存邻节点。
* `Tc<T>` 支持了多重所有权
* 可以追踪所有到值的引用
* 引用为0 ： 可以安全地清理
* 不在预导入模块中

使用场景：
* 希望将堆上的一些数据分享给程序的多个部分同时使用，而又无法在编译期确定哪个部分会最后释放这些数据时
* 只能用于单线程场景

使用实例：
* `Rc::clone(&a)` 增加引用计数(没有深度拷贝操作)
* `Rc::strong_count(&a)` 获得引用计数
  * 对应的有 `Rc::weak_count` 函数<div align=center><img src="https://i.imgur.com/tqjk4av.png" width="80%"/></div>
* 实际上是通过**不可变引用**共享只读的数据
    ```Rust
    enum List {
        Cons(i32, Rc<List>),
        Nil,
    }

    use crate::List::{Cons, Nil};
    use std::rc::Rc;

    fn main() {
        let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
        let b = Cons(3, Rc::clone(&a));
        let c = Cons(4, Rc::clone(&a));
    }
    ```

## `RefCell<T>` 和内部可变性模式
内部可变性允许你在只持有不可变引用的前提下对数据进行修改， 在它的数据结构中使用了unsafe（不安全）代码来绕过Rust正常的可变性和借用规则。
* `RefCell<T>`类型代表了其持有数据的唯一所有权。<div align=center><img src="https://i.imgur.com/a2oLKVb.png" width="70%"/></div><div align=center><img src="https://i.imgur.com/8qGlmrN.png" width="70%"/></div>
* 只能用于单线程的场景，相当于是支持一个可变或者不可变的借用
* 内部可变性就是可变借用一个不可变的值（只是将检查延后到运行时）

使用的时候有两个方法
* borrow, 返回智能指针 `Ref<T>`， 实现了 Deref
* borrow_mut, 返回智能指针 `RefMut<T>`, 也实现了 Deref

而 `RefCell<T>` 会在运行时记录借用信息
* 调用 borrow： 不可变借用数 +1，`Ref<T>`离开作用域被释放时候，可变借用计数 -1
* 调用 borrow_mut： 可变借用数 +1，`RefMut<T>`离开作用域被释放时候，可变借用计数 -1
* 在任意时间内， 只能有一个可变借用和多个不可变借用， 否则会触发 panic

`Rc<T>` 和 `RefCell<T>` 结合使用可以实现一个拥有多重所有权的可变数据：
```Rust
#[derive(Debug)]
enum List {
    Cons(Rc<RefCell<i32>>, Rc<List>),
    Nil,
}
```
标准库还提供了其他一些类型来实现内部可变性:
* `Cell<T>` 选择了通过复制来访问数据。
* `Mutex<T>` 被用于实现跨线程情形下的内部可变性模式。

## 循环引用会造成内存泄漏
