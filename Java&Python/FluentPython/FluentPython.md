# Fluent Python 
***
# Part I : Prologue
## Chap 1: The Python Data Model
我们可以将 python 的 data model 看成是python 的一种框架描述, 它规范了这门语言自身构建模块的接口。Python 解释器碰到特殊的句法时，会使用特殊方法去激活一些基本的对象操作，这些特殊方法的名字以两个下划线开头，以两个下划线结尾.例如, `obj[key]` 的背后就是 `__getitem__` 方法, 这些特殊的方法让我们的对象实现支持语言构架, 并与之交互.  
### Python 风格的纸牌
一个简单的 [例子](https://github.com/fluentpython/example-code/blob/master/01-data-model/frenchdeck.py) 展示如何实现 `__getitem__` 和 `__len__` 这两个特殊方法:  

稍微解释一下, `namedtuple` 是用于建立一个只有少数属性没有没有方法的对象，比如
数据库条目. 这里实现了 FrenchDeck 类后, 就可以使用对应的两个方法:
```shell
>>> deck = FrenchDeck()
>>> len(deck)
52
>>> deck[13]
Card(rank='2', suit='diamonds')
>>> from random import choice
>>> choice(deck)
Card(rank='9', suit='clubs')
```
* 作为类的用户可以不用记住标准操作的各类名称, 直接使用 `len(someObj)` 而不是用`someObj.size()` 或者 `someObj.length()`
* 实现了 `__getitem__` 就可以更加方便地使用 Python 的标准库

**迭代通常是隐式的**，譬如说一个集合类型没有实现`__contains__` 方法，那么 in 运算符就会按顺序做一次迭代搜索。而排序, 我们可以自己实现并且传入`sorted`作为参数:
```Python
suit_values = dict(spades=3, hearts=2, diamonds=1, clubs=0)
def spades_high(card):
    rank_value = FrenchDeck.ranks.index(card.rank)
    return rank_value * len(suit_values) + suit_values[card.suit]
for card in sorted(deck, key=spades_high): # doctest: +ELLIPSIS
... print(card)
```
可以说, 我们通过实现 `__len__` 和 `__getitem__` 两个特殊的方法, 使得 FrenchDeck 和 Python 自有的序列数据类型一样, 可以体现出 Python 的核心语言特性, 同时将它可以用于标准库的函数(如`random.choice`/`sorted`).  
### 如何使用特殊方法
特殊方法的存在是为了**被 Python 解释器调用的**，我们并不需要调用它们, 应该使用 `len(my_object)` 而不是 `my_object.__len__()`.而对于 Python 内置类型, CPython 可能抄近路, 如直接读取内存对象的 C 语言结构体.   
很多时候**调用特殊方法是隐式的**, 如 `for i in someList:` 背后调用的是`iter(someList)`, 这个方法背后则调用`someList.__iter__()` 方法.  
通常你的代码无需直接使用特殊方法, 通过内置的函数（例如`len`、`iter`、`str`，等等）来使用特殊方法是最好的选择。
#### 模拟数值类型
利用特殊方法, 我们可以让自定义对象通过运算符进行运算, 一个[简单 Vector 的实现](https://github.com/fluentpython/example-code/blob/master/01-data-model/vector2d.py) 就包含了 `__init__`/`__add__`/`__mul__`/`__repr__`/`__bool__` 等特殊方法的实现, 其支持的操作也可以轻易从特殊方法的名字中看出.  
#### 字符串表示形式
Python 内置函数 `repr` 能把一个对象用字符串的形式表达出来以便辨认, 交互式控制台和调试程序（debugger）用 `repr` 函数来获取字符串表示形式, 此外还有一个`__str__`,在调用 `str()` 函数被使用，或是在用`print` 函数打印一个对象的时候才被调用的，并且它返回的字符串对终端用户更友好.  
只想实现这两个特殊方法中的一个，`__repr__` 是更好的选择.  
下表是 Python 的特殊方法总览:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601110750.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601110814.png"/></div>

### len 不是普通方法
len 之所以不是一个普通方法，是为了让Python 自带的数据结构可以走后门，abs 也是同理.这种处理方式在保持内置类型的效率和保证语言的一致性之间找到了一个平衡点.