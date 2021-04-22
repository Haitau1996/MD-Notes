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
* （T,U,..）：有限序列， 其中 T,U可以是不同的类型
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