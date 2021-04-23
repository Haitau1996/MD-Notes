# Mastering Rust
## Chap 1: 入门
Rust 是一种用过去的知识来解决将来的问题的技术, 它的灵感来自于多种语言的知识, 其中比较重要的是 Cyclone 的基于区域的内存管理技术, C++ 的 RAII 原则, Haskell 的类型系统\异常处理类型和类型类. <br>
* **静态和强类型语言** , 静态意味着编译器将在编译时进行大量的类型检查, **强类型就意味着不允许发生诸如类型之间自动转换的事情** , 并且指向整数的变量不能再代码中改为指向字符串. 
* **编译期内存安全**: Rust 编译器可以在编译时跟踪程序中资源的变量, 并且在没有垃圾收集器的情况下完成这些操作. 
* **零成本抽象**: 编程的目的就是管理复杂性, 这是通过良好的抽象实现的, 它在抽象时候使用的包装器结构没有任何指针解引用, 不会产生堆分配的开销. 
* **支持高并发**: 语言具有应用程序接口和抽象能力, 使得编写正确和安全的并发代码变得十分容易. 

### Rust 简介
安装 Rust 工具链, 在 msys2/Unix 等环境中都可以使用下面这行安装工具链:
```bash
curl https://sh.rustup.rs -sSf | sh
```
如果网络环境不好可以设置环境变量`RUSTUP_DIST_SERVER`为`https://mirrors.tuna.tsinghua.edu.cn/rustup`, 使用清华的开源镜像站.此外[rust playground](https://play.rust-lang.org) 可以在线运行 Rust 程序, 这支持导入外部库, 并可在运行示例程序时候使用. 

#### 基元类型（Primitive types）
Rust 中的基元类型有下面几种：
* bool 类型
* char 字符
* 整型，区别在于位宽和是否有符号
* isize : 指针大小的有符号整型。 usize : 指针大小的无符号整型
* 浮点类型 f32, f64
* `[T； N]`: 固定大小的数组， T 表示元素类型， N 表示数目
* `[T]`: 动态大小的连续序列视图， T 表示任意类型
* str:字符串切片， 主要做引用
* (T,U,..)：有限序列， 其中 T,U可以是不同的类型
* fn(i32)-> i32 : 定义了接收类型和返回值的函数

#### 变量声明和不可变性
Rust 中使用 let 来声明变量，**并且在默认的情况下不可变**，就是说我们无法在初始化之后让给变量赋其他值， 如果我们希望变量在将来指向别的东西（同一类型）， 这需要在变量前加入 `mut` 关键字.
```rust
let mut greeting = "Hello";
```

#### 函数
函数将一系列指令抽象为具名实体, 稍后可以通过其他代码调用这些指令, 帮助用户管理复杂性, 需要注意的是 **Rust 不允许类似于 C++ 方式的函数重载**. 函数最后一个表达式会自动返回, 不过仍有关键字 return, 用于函数提前退出. 
```rust
fn add(a: u64, b: u64) -> u64 {
    a + b
}
```

#### 闭包
Rust 也支持闭包(closures), 类似于函数但是具有声明他们的环境或者作用域的更多信息(类似于 C++ 中的 lambda 表达式), `let my_closure = || ();` 就是一个简单得闭包.下面是更复杂的例子:
```Rust
fn main() {
    let doubler = |x| x * 2;
    let value = 5;
    let twice = doubler(value);
    println!("{} doubled is {}", value, twice);
    let big_closure = |b, c| {
        let z = b + c;
        z * twice
    };
    let some_number = big_closure(1, 2);
    println!("Result from closure: {}", some_number);
}
```
**闭包的主要作用是作为高阶函数的参数, 高阶函数是以一个闭包或者函数作为参数的函数**.

#### 字符串
字符串是常见的数据类型, Rust 中有两种形式,并且支持UTF-8
* `&str` 类型: 通常为指向现有字符串的指针, 可以在堆上和栈上, 也可以是已编译对象代码的数据段中的字符串
* `String`: 在堆上分配的
```Rust
fn main() {
    let question = "How are you ?"; // a &str type
    let person: String = "Bob".to_string();
    let namaste = String::from("नमते"); // unicodes yay!
    println!("{}! {} {}", namaste, question, person);
}
```

#### 条件和判断
Rust 和 C 有类似的 if - else 结构, 不过if 构造不是语句, 而是一个表达式(语句不返回任何值, 但是**Rust if-else 中的条件总是会返回一个值**, 并且 if 和 else 分支应该具有相同的返回类型). 下面的语句就直接把 `&str` 赋值给了 result:
```Rust
fn main() {
    let result = if 1 == 2 { 
        "Wait, what ?" 
    } else { 
        "Rust makes sense" 
    };
    println!("You know what ? {}.", result);
}
```
因此,如果删除 else 语句, if 返回的是一个 `&str`, 没有 else 返回的就是 `()`, 两者不是同种类型就会报错.

#### match 表达式
match 表达式可以认为是 C 语言中的 Switch 语句简化版, 允许用户根据变量的值,以及是否有高级过滤功能做出判断.
```Rust
fn req_status() -> u32 {
    200
}
fn main() {
    let status = req_status();
    match status {
        200 => println!("Success"),
        404 => println!("Not Found"),
        other => {
            println!("Request failed with code: {}", other);
            // get response from cache
        }
    }
}
```
Rust 允许我们通过 catch all变量(上面的 other) 或者下划线(`_`)来忽略其他可能性. 

#### 循环
Rust 中重复做某件事情可以通过 3 种构造完成, loop , while 和 for, 通常都包含 continue 或者 break 来跳出循环, 同时 loop 中可以使用循环标签中断:
```Rust
fn silly_sub(a: i32, b: i32) -> i32 {
    let mut result = 0;
    'increment: loop {
        if result == a {
            let mut dec = b;
            'decrement: loop {
                if dec == 0 {
                    // breaks directly out of 'increment loop
                    break 'increment;
                } else {
                    result -= 1;
                    dec -= 1;
                }
            }
        } else {
            result += 1;
        }
    }
    result
}

fn main() {
    let a = 10;
    let b = 4;
    let result = silly_sub(a, b);
    println!("{} minus {} is {}", a, b, result);
}
```
而 while 循环和 C 语言中十分类似, 补在赘述. for循环只适用于可以转换为迭代器的类型(一般为 range 类型):
```Rust
fn main() {
    // does not include 10
    print!("Normal ranges: ");
    for i in 0..10 {
        print!("{},", i);
    }

    println!();       // just a newline
    print!("Inclusive ranges: ");
    // counts till 10
    for i in 0..=10 {
        print!("{},", i);
    }
}
```
其中前者不包含 10 而后者包含 10.

#### 自定义数据类型
自定义类型可以是基元类型的包装器, 也可以是多个自定义类型的组合, 有三种形式: 结构体(struct), 枚举(enum) 及联合(union).
##### 结构体
* 单元结构体, 用于对于没有与之关联的数据或者状态进行实体建模, 也可用于表示错误类型
* 元祖结构体, 具有关联数据, 每个字段没有命名, 使用位置进行引用
    ```Rust
    struct Color(u8, u8, u8);
    fn main() {
        let white = Color(255, 255, 255);
        let red = white.0;
        let green = white.1;
        let blue = white.2;
        let orange = Color(255, 165, 0);
        // You can also destructure the fields directly
        let Color(r, g, b) = orange;
        println!("R: {}, G: {}, B: {} (orange)", r, g, b);
        // Can also ignore fields while destructuring
        let Color(r, _, b) = orange;
    }
    ```
* 类C 语言结构体: 最常见的形式, 可以按照任何的顺序初始化字段, 还可以提供有意义的名称
    ```Rust
    struct Player {
        name: String,
        iq: u8,
        friends: u8,
        score: u16
    }
    fn main() {
    let name = "Alice".to_string();
    let player = Player { name,
                          iq: 171,
                          friends: 134,
                          score: 1129 };
    }
    ```

##### 枚举
使用关键字 enum 创建, 后面跟着一对花括号, 在内部写所有可能的类型(变体):
```Rust
enum Direction { 
    N, 
    E, 
    S, 
    W
}

enum PlayerAction {
    Move {
        direction: Direction,
        speed: u8
    },
    Wait, 
    Attack(Direction)   
}

fn main() {
    let simulated_player_action = PlayerAction::Move {
        direction: Direction::N,
        speed: 2,
    };
    match simulated_player_action {
        PlayerAction::Wait => println!("Player wants to wait"),
        PlayerAction::Move { direction, speed } => {
          println!("Player wants to move in direction {:?} with speed {}",
                direction, speed)
        }
        PlayerAction::Attack(direction) => {
            println!("Player wants to attack direction {:?}", direction)
        }
    };
}
```

#### 类型上的函数和方法
通常我们希望类型具有函数或者方法.
##### 结构体上的 impl 块
我们可以使用两种定义向之前的 Player 中添加行为(函数)
1. 类似于够着函数的函数
2. 设置 Person 的 friend 字段的 getter 和 setter 方法
    ```Rust
    struct Player {
        name: String,
        iq: u8,
        friends: u8
    }
    impl Player {
        fn with_name(name: &str) -> Player {
            Player {
                name: name.to_string(),
                iq: 100,
                friends: 100
            }
        }

        fn get_friends(&self) -> u8 {
            self.friends
        }

        fn set_friends(&mut self, count: u8) {
            self.friends = count;
        }
    }
    fn main() {
        let mut player = Player::with_name("Dave");
        player.set_friends(23);
        println!("{}'s friends count: {}", player.name, player.get_friends());
        // another way to call instance methods.
        let _ = Player::get_friends(&player);
    }
    ```
实践中我们使用关键字 impl, 然后指定类型(如上面的 Player), 在紧跟的花括号中编写两种方法
1. 关联方法, 没有`&self`作为第一个参数, 可以在类型上调用并不需要通过实例调用(类似于静态成员函数)
2. 实例方法: 只能在结构体的实例中调用(类似于一般的成员函数, self 相当于 this 指针), self 有三种变体
   1. self: 不允许后续使用该类型
   2. &self: 提供对类型实例的读取访问权限(有点类似于 const 成员函数)
   3. &mut self : 提供对类型实例的可变访问权限

##### impl 块和枚举
Rust 中枚举也可以有 impl 块, 广泛应用于状态机, 当其与match 搭配使用的时候可以使得状态机的转换非常简洁:
```Rust
enum PaymentMode {
    Debit,
    Credit,
    Paypal
}
// Bunch of dummy payment handlers
fn pay_by_credit(amt: u64) {
    println!("Processing credit payment of {}", amt);
}
fn pay_by_debit(amt: u64) {
    println!("Processing debit payment of {}", amt);
}
fn paypal_redirect(amt: u64) {
    println!("Redirecting to paypal for amount: {}", amt);
}
impl PaymentMode {
    fn pay(&self, amount: u64) {
        match self {
            PaymentMode::Debit => pay_by_debit(amount),
            PaymentMode::Credit => pay_by_credit(amount),
            PaymentMode::Paypal => paypal_redirect(amount)
        }
    }
}
fn get_saved_payment_mode() -> PaymentMode {
    PaymentMode::Debit
}
fn main() {
    let payment_mode = get_saved_payment_mode();
    payment_mode.pay(512);
}
```
#### 集合
通常处理多个数据实例时候可以使用集合
##### 数组
具有**固定长度**, 可以存储相同类型的元素,`[T, N]` 表示,有点类似于 `std::array<T,N>`,如:
```Rust
let numbers: [u8; 10] = [1, 2, 3, 4, 5, 7, 8, 9, 10, 11];
let floats = [0.1f64, 0.2, 0.3];
```
##### 元组
是异构集合, 可以将不同类型的元素存储在一起, 从函数返回多个值的时候也可以用它:
```Rust
let num_and_str: (u8, &str) = (40, "Have a good day!");
```
##### 项目列表
和数组类似, 不过其长度不用事先制定,可以按照需求增加(在堆上分配的), 可以使构造函数`Vec::new` 或者宏`vec![]` 创建:
```Rust
let mut numbers_vec: Vec<u8> = Vec::new(); 
let mut vec_with_macro = vec![1]; 
```
##### 键/值 对
来自于 `std::collections` 模块, 名为 HashMap, 使用构造函数 `HashMap::new` 创建:
```Rust
let mut fruits = HashMap::new();
fruits.insert("apple", 3);
```
##### 切片
获取集合类型视图的常见做法, 基本是指针或者引用, 指向现有集合类型中其他变量所拥有的连续区间, 用`&[T]`表示:
```Rust
let mut numbers: [u8; 4] = [1, 2, 3, 4];
{
    let first_two: &mut [u8] = &mut numbers[0..2];
    first_two[0] = 100;
    first_two[1] = 99;
}
```
值得注意的是`&str`也属于切片类型.

#### 迭代器
提供了一种高效访问集合类型元素的方法, 并且尽在需要时对集合中的元素进行求职或者访问, 可以与多个转换操作链接. 

## Chap 2 : 使用 Cargo 管理项目
在大型项目中,编译所有的源文件并且将它们链接到一起变成了一个复杂的过程, 而软件包管理器就可以自动构建大型软件项目. <br>
实际的软件包代码通常被组织成多个文件, 并且有许多依赖项目, 需要用专门的工具来管理, 它们完成了诸如项目分析, 下载正确版本的依赖项, 检查版本冲突, 编译和链接文件等所有繁琐工作.<br>

### 模块
每个 Rust 程序都以 root 模块开头, 程序库文件root 模块是 lib.rs, 可执行文件通常是 main.rs, 代码越来越多时候允许拆分成模块, 并且有多种方式可以创建它:
#### 嵌套模块
在现有代码中用 mod 代码块创建嵌套模块, 在main前使用 use 语句添加到代码中,同时用到的要使用关键字 Pub 作为前缀使得其为共有:
```Rust
mod food {
    pub struct Cake;
    struct Smoothie;
    struct Pizza;
}
use food::Cake;
fn main() {
    let eatable = Cake;
}
```
#### 文件用作模块
我们有下面的文件结构:
<div align=center><img src="https://i.loli.net/2021/04/22/OQRYf5NP3zUq9oZ.png"/></div>
这时候如果要在 main 中使用 foo模块, 有三点要注意
* 在 main 中添加 `mod foo;` 引入文件
* 使用 use 语句添加使用的结构体
* 使用的函数/结构体 要有前缀 pub

导入的方式也有好几种:
* creat:绝对导入前缀
* self:相对导入前缀
* super: 同样是相对导入前缀, 可以从父模块导入元素

#### 将目录用作模块
可以创建一个目录来表示模块, 允许我们将模块中的子模块作为文件和目录的层次结构.如下面一个目录结构:
<div align=center><img src="https://i.loli.net/2021/04/22/dhyDWYj1Q4KPbpf.png"/></div>

```Rust
// my_program/foo/bar.rs
pub struct Bar;
impl Bar {
    pub fn hello() {
        println!("Hello from Bar !");
    }
}

// my_program/foo.rs
mod bar;
pub use self::bar::Bar;
pub fn do_foo() {
    println!("Hi from foo!");
}

// my_program/main.rs
mod foo;
use foo::Bar;
fn main() {
    foo::do_foo();
    Bar::hello();
}
```

### Cargo 和程序库
#### 新建一个 Cargo 项目
`cargo new [options] name` 新建一个项目, name 用于项目目录名(cargo help new 看帮助)
* 默认是创建一个可执行程序项目
* 创建库项目时候传入 `--lib` 作为参数
* 传入`--vcs` 更改默认的版本控制系统,可以为 hg, none 等
* 使用 TOML 配置文件格式

#### Cargo 与依赖项
软件包管理器不仅是帮用户解决依赖性的工具, 还应该确保项目的可预测和可重复地构建. Cargo 通过两个文件管理 Rust 项目:
* Cargo.toml 编写依赖管理及其所需版本
* Cargo.lock 锁文件, 包含所有直接依赖项和任何间接依赖项的绝对版本

程序库以 `major.minor.patch` 格式指定

#### 使用 cargo 执行测试
测试函数附有 `#[test]` 注释, 可以使用 cargo test 命令运行测试并且查看其结果. 

#### 使用 cargo 运行示例
使用 `cargo run [options] [--args]` 来运行代码

#### Cargo 工作区
Cargo 工作区可以帮我们将代码通用部分拆分成单独的程序库, 以便管理复杂性. 

### Cargo 工具扩展
Cargo 可以通过集成外部工具进行功能扩展, 从而改善开发体验.
#### 子命令和 Cargo 安装
cargo 自定义的子命令通常来自于 Github 或者本地项目目录下的二进制文件, 通过 `cargo install <binary crate name>` 或者在项目目录下执行 `cargo install` 安装. 
* cargo-watch: 用户代码发生变动后自动构建项目
* cargo-edit: 用户自动将依赖项添加到 Cargo.toml 文件中
* cargo-deb: 可以创建 .deb 包(Debian 系Linux中发布)
* cargo-outdated: 显示过时的依赖项

#### clippy 格式化代码
为我们代码检查一系列的问题, 从而确保生成高质量的 Rust 代码, 通过 `rustup component add clippy` 安装, 使用时直接 `cargo clippy`

#### Cargo.toml 文件介绍
略

### 搭建 Rust 开发环境
Rust 生态中有很多工具能够增强开发体验:
* rustfmt: 根据 Rust 代码样式指南中提及的约定格式化代码
* clippy: 对常见的错误和潜在的问题发出警告
* racer: 可以查找标准程序库, 并且提供代码自动化完成和实现工具的提醒功能

vscode 中有官方的插件可以下载.

## Chap 3: 测试/文档化和基准评估
### 单元测试的目的
软件系统中的各个组件的功能测试是保证代码高质量\有效并且实用的方法, 有助于建立开发人员将代码部署到生产环境中的信心, 并且在项目长期维护时保持代码的健壮性.<br>
单元测试是大规模重构的基础, 同时鼓励程序言编写主要依赖输入参数的模块化代码(无状态函数).

### 组织测试
我们通常有两种测试, 单元测试和集成测试.
* 单元测试: 编写在包含被测试代码的同一模块中
* 集成测试: 在程序库根目录下的`test/`目录中单独编写, 添加一个 use 声明来引入需要测试的公共API

几个相关的术语:
* **属性**: 指元素的注释, 通常是编译器内置的, 也可以由用户通过编译器插件创建
  * `#[name]`:适用于每个元素, 通常显示在他们定义的上方, 如 `#[test]` 注释表示函数被视为测试工具的一部分
  * `#![name]` 适用于每个软件包, 通常位于软件包根目录的最顶端部分
* 断言宏
  * `assert!`: 如果值为 false 就测试失败,可以添加格式化字符串提供自定义异常消息
  * `assert_eq!`: 接收两个值
  * `assert_ne!`: 同上, 刚好相反
  * `debug_assert!`: 仅仅在调试版本中有效, 帮助在调试模式下运行代码时捕获断言异常

### 单元测试
通常一个单元测试就是一个函数, 下面编写一个简单得单元测试:
```Rust
// first_unit_test.rs
#[test]
fn basic_test() {
    assert!(true);
}
```
可以使用 `rustc --test first_unit_test.r` 来编译, 而默认所有测试都是并行运行的, 除非设置环境变量`RUST_TEST_THREADS=1`. 现在 Cargo 可以支持运行测试, `cargo test` 用起来方便很多.
#### 隔离测试代码
辅助测试的方法智能在测试代码的上下文使用, 我们将所有测试相关的代码封装在模块中, 并在上面放置`#[cfg(test)]` 注释来实现这个目的. 
```Rust
// unit_test/src/lib.rs

// function we want to test
fn sum(a: i8, b: i8) -> i8 {
    a + b
}

#[cfg(test)]
mod tests {
    fn sum_inputs_outputs() -> Vec<((i8, i8), i8)> {
        vec![((1, 1), 2), ((0, 0), 0), ((2, -2), 0)]
    }

    #[test]
    fn test_sums() {
        for (input, output) in sum_inputs_outputs() {
            assert_eq!(crate::sum(input.0, input.1), output);
        }
    }
}
```
有两点说明:
* sum 是在 test 模块之外声明的, 会包含在正式发布的编译版本中
* 模块的单元测试还允许用户测试私有的函数和方法

#### 故障测试
`#[should_panic]` 和 `#[test]` 属性搭配使用, 表示函数将导致不可恢复的故障, 这种异常被称为 panic
#### 忽略测试
`#[ignore]` 表示在运行 `cargo test` 时候将忽略此类测试功能, 然后可以向命令传递 `--ignore` 参数来单独运行这些测试. 

### 集成测试
有点类似于黑盒测试, 从消费者的角度测试软件包公共接口的端到端使用, 和单元测试的区别在于目录结构和其中的项目需要公开. 例如我们给库`lib.rs` 进行测试, 其文件结构如下:
<div align=center><img src="https://i.loli.net/2021/04/22/Ej1H4KvtDPMoB3c.png"/></div>

```Rust
use integration_test::sum;
#[test]
fn sum_test() { 
    assert_eq!(sum(6, 8), 14); 
}
```
#### 共享通用代码
在测试之前我们可能需要一些 setup / teardown-related 代码(大概率由 test 目录下文件共享), 我们可以创建为共享通用代码的文件和目录模块, 或者使用模块 foo.rs 在集成测试中使用 mod 关键字来声明和引用它. 
```Rust
// integration_test/tests/common.rs
pub fn setup() {
    println!("Setting up fixtures");
}
pub fn teardown() {
    println!("Tearing down");
}
use integration_test::sum;
mod common;
use common::{setup, teardown};
#[test]
fn test_with_fixture() {
    setup();
    assert_eq!(sum(7, 14), 21);
    teardown();
}
```

### 文档
Rust 有名为 rustdoc 的专用工具将 markdown 的文档注释撰文 html. 文档解释分成两个层级:
* 元素级: 适用于模块中的元素, 如结构体, 单行以 `///` 开头,对于多行以 `/*`开头, 以`*/` 结尾
* 模块级: 出现在根层级的注释, 如 `main.rs`/`lib.rs`, `//!`表示单行注释的开始, 使用`/*!` 表示多行注释的开始, 并且将`*/`作为结尾标记

#### 文档属性
还有其他可以用于调整已经生成的文档界面,写成 `#[doc(key=value)]`这样的形式

#### 文档化测试
将代码示例放到软件包公共API 的所有文档通常是一种很好的做法, 维护时注意我们的代码可能发生变化而却忘记了去改这些示例. Rust 允许我们在文档中使用类似于 Markdown 的 "`"来嵌入代码, 并且将视为 但单元测试的一部分
```Rust
/// ```
/// assert_eq!(doctest_demo::sum(1, 1), 2);
/// ```
pub fn sum(a: i8, b: i8) -> i8 {
    a + b
}
```
### 基准
Rust 为我们提供了一个微观基准框架, 可以单独对代码各个部分进行基准测试, 而不受外界因素的影响. 
#### 内置微观基准工具
Rust 中的操作得益于两件事:
* 函数上方的 `#[bench]` 注释, 表示函数是一个基准测试
* 内部编译软件包 libtest 中包含一个 Bencher 类型, 基准函数通过它在多次迭代中运行相同的代码

#### 稳定版 Rust 上的基准测试
社区开发的基准测试软件包能够兼容稳定版本的 Rust, 以 _criterion-rs_ 为例, 使用的时候先需要在 Cargo.toml 中将包作为依赖项引入.

## Chap 4: 类型/泛型 和特征
类型是一组具名的可能值(named set of possible values),它提供了一种方法来弥合我们创建实体的底层表示与心理模型之间的差距, 为了我们提供了表示实体的意图/行为和约束的方法. Rust 的类型系统介于 C 和 Haskell 之间, 试图在这两种极端中找一个平衡:
* 类型系统是作为强类型系统存在的, 因此编译时候执行更多类型检查而不是在运行时抛出异常
* 类型系统是静态的, 绑定到整数值的变量无法又指向字符串

### 泛型
泛型是语言设计特性的一部分, 可以实现代码的复用并且遵循 DRY 原则, 在其支持下可以使用类型占位符来编写算法/函数/方法以及类型,当我们实例化任何泛型元素时, 他们会被替换成诸如 u32 这样的具体类型. 

#### 创建泛型
Rust 允许我们将多种元素声明为泛型(结构体/枚举/函数/特征/方法以及代码实现块)
1. 泛型函数: 我们需要将泛型参数放在函数名之后方括号之前:
    ```Rust
    fn give_me<T>(value: T) {
        let _ = value;
    }

    fn main() {
        let a = "generics";
        let b = 1024;
        give_me(a);
        give_me(b);
    }
    ```
    泛型其实是一种提供多态代码错觉的简易方法, 说它是错觉是因为其实编译之后是存在包含具体类型参数的重复代码.
2. 泛型结构体和枚举
    ```Rust
    enum Transmission<T> {
        Signal(T),
        NoSignal
    }

    fn main() {
        // stuff
    }
    ```

#### 泛型实现
我们可以为泛型编写impl代码块, 但是由于额外的参数, 它会变得冗长:
```Rust
// generic_struct_impl.rs
struct Container<T> {
    item: T
}
impl<T> Container<T> {
    fn new(item: T) -> Self {
        Container { item }
    }
}
```
如果是针对某个具体的类型特特化, 则 impl 后面的尖括号块可以不要:
```Rust
impl Container<u32> {
    fn sum(item: u32) -> Self {
        Container { item }
    }
}
```

#### 泛型应用
当我们进行实例化的时候, 编译器需要知道 T 的具体类型以进行替换, 有三种提示编译器的办法
1. 显式给编译器提供一种类型
2. 对泛型函数调用某些方法来接收具体类型
3. 使用 turbofish 运算符输入具体类型

```Rust
    // providing a type
    let v1: Vec<u8> = Vec::new();
    // or calling method
    let mut v2 = Vec::new();
    v2.push(2);    // v2 is now Vec<i32>
    // or using turbofish
    let v3 = Vec::<u8>::new();    // not so readable
```

### 用特征抽象行为
