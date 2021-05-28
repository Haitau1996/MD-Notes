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
