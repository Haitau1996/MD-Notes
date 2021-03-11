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
