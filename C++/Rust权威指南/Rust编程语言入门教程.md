# [Rust编程语言入门教程](https://www.bilibili.com/video/BV1hp4y1k7SV)

## Intro
**Why Rust**:(性能/安全性/并发 but 难学)
* Rust 和 C/C++ 具有相同的性能, 但是很多常见的 Bug 可以在编译的时候被消灭
* 作为一种通用的编程语言, 擅长在下列场景中使用:
  * 需要运行时的速度
  * 需要内存安全
  * 更好地利用多处理器
* 比 C/C++ 等高性能的编程语言相比更加安全, 比拥有 GC 的 Java/C# 更加高效

Rust 有很多独有的概念, 和现在大多数主流的语言都不同. 

### 安装
从命令行运行[官网]`https://www.rust-lang.org/tools/install`中的相关代码:
* 更新 : `rustup update`
* 卸载 : `rustup self uninstall`
* 添加新的插件: `rustup component add [some component, 如 rust-analysis]`

安装成功后, 运行 `rustup doc` 可以得到英文的文档, 它也有社区翻译的[中文版本](https://kaisery.github.io/trpl-zh-cn/ch01-01-installation.html).  
Rust 语言中编译和运行是独立的两步:
* `rustc source.rs` 编译文件, 生成二进制文件和一个包含调试信息的 `.pdb` 文件
* 是预先编译的语言(ahead-of-time)
* 在复杂的程序中使用 Cargo

### Cargo
Cargo 是 Rust 的构建系统和包管理工具(帮助构建代码,下载依赖 etc)
* `cargo new project_name` 创建项目
* Cargo.toml: 配置文件的格式,位于项目的顶层目录下
  * package 段配置包
  * dependencies 列出以来项. 
* `cargo build` 创建二进制文件, 第一次运行会生成 cargo.lock(追踪以来的精确版本, 不要手动修改)
  * `--release` 选项用于发布时的构建(会做优化, 需要更长的编译时间)
* `cargo run`: 编译+运行
* `cargo check`: 检查代码是否合法, 不编译

## 猜数字