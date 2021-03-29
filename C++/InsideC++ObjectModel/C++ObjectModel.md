# 深度探索 C++ 对象模型

## Chap 1: 关于对象
C 语言中, 数据和处理数据的操作(函数)是分开声明的, 语言没有支持数据和函数之间的关联性. C++ 加上封装之后, 成本增加了多少? <br>
实际上, C++ 在布局以及存取之间的额外负担主要是由 _virtual_ 引起的:
* 虚函数机制: 用以支持一个有效率的执行期绑定
* 虚基类: 用以实现多次出现在继承体系中的base class, 有一个单一而被共享的实例.

### C++ 对象模式
C++ 中有两种class data members: _static_ 和 _nonstatic_ . 同时有三种 class member functions: _static_, _nonstatic_ 和 _virtual_. 