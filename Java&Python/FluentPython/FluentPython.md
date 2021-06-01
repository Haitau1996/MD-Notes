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
len 之所以不是一个普通方法，是为了让Python 自带的数据结构可以走后门，abs 也是同理.这种处理方式在**保持内置类型的效率和保证语言的一致性之间找到了一个平衡点**.

## Chap 2: 序列构成的数组
Python 从 ABC 语言继承了很多 Idea: 序列的泛型操作、内置的元组和 mapping 类型、用缩进来架构的源码、无需变量声明的强类型...

### 内置的序列类型简介
Python 标准库用 C 实现了丰富的序列类型:
* 容器序列: list、tuple 和collections.deque 这些序列能存放不同类型的数据
* 扁平序列: str、bytes、bytearray、memoryview 和array.array，这类序列只能容纳一种类型

同时还可以按照元素是否可修改分类:
* 可变序列:list、bytearray、array.array、collections.deque 和 memoryview
* 不可变序列: tuple、str 和 bytes。

<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601144206.png"/></div>

上图显示可变序列和不可变序列的差异,最重要也最基础的序列类型应该就是列表（list）, 它是可变的, 同时可以存放不同类型的元素.  

### 列表推导 和 生成器表达式

列表推导是构建列表（list）的快捷方式，而生成器表达式则可以用来创建其他任何类型的序列。

#### 列表推导和可读性

```Shell
>>> symbols = '$¢£¥€¤'
>>> codes = []
>>> for symbol in symbols:
...  codes.append(ord(symbol))
# 另一种写法
>>> symbols = '$¢£¥€¤'
>>> codes = [ord(symbol) for symbol in symbols]
```
列表推导可以**帮助我们把一个序列或是其他可迭代类型中的元素过滤或是加工，然后再新建一个列表**。例如在上面的例子中, for 语句被用于生成新建列表, 通常的原则是，只用列表推导来创建新的列表，并且尽量保持简短.  

#### 列表推导同filter和map的比较
filter 和map 合起来能做的事情，列表推导也可以做，而且还不需要借助难以理解和阅读的 lambda 表达式。
```Shell
>>> symbols = '$¢£¥€¤'
>>> beyond_ascii = [ord(s) for s in symbols if ord(s) > 127]
>>> beyond_ascii
[162, 163, 165, 8364, 164]
>>> beyond_ascii = list(filter(lambda c: c > 127, map(ord, symbols)))
```

#### 笛卡尔积
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601150534.png"/></div>

上面展示的就是四种花色和三种牌面的笛卡尔积, 结果是一个包含 12 个元素的列表, 可以这样实现:
```shell
>>> colors = ['black', 'white']
>>> sizes = ['S', 'M', 'L']
>>> tshirts = [(color, size) for color in colors for size in sizes]
```
这个列表的两个循环嵌套的顺序和 for 从句的先后顺序是一致的.  

#### 生成器表达式
使用生成器是比使用列表来初始化元组/数组或其他序列类型的更好的选择, 生成器表达式背后遵守了迭代器协议，可以逐个地产出元素，而不是先建立一个完整的列表，然后再把这个列表传递到某个构造函数里。  
生成器表达式的**语法跟列表推导差不多，只不过把方括号换成圆括号而已**。
```Shell
>>> symbols = '$¢£¥€¤'
>>> tuple(ord(symbol) for symbol in symbols)
(36, 162, 163, 165, 8364, 164)
>>> import array
>>> array.array('I', (ord(symbol) for symbol in symbols))
```

### 元组不仅仅是不可变的列表
除了用作不可变的列表，它还可以用于没有字段名的记录。
#### 元组和记录
将元组中的元素视为记录中的一个字段数据, 这个位置信息给数据赋予了意义, 位置信息也变得非常重要. 
#### 元组拆包
元组拆包可以应用到任何可迭代对象上，唯一的硬性要求是，被可迭代对象中的元素数量必须要跟接受这些元素的元组的空档数一致。除非我们用* 来表示忽略多余的元素.其中**最好辨认的元组拆包形式就是平行赋值**:
```Shell
>>> lax_coordinates = (33.9425, -118.408056)
>>> latitude, longitude = lax_coordinates # 元组拆
```
##### 用*来处理剩下的元素
在Python 中，函数用*args 来获取不确定数量的参数算是一种经典写法. 
```Shell
>>> a, *body, c, d = range(5)
>>> a, body, c, d
(0, [1, 2], 3, 4)
```

#### 嵌套元组拆包
接受表达式的元组可以是嵌套式的, 只要这个接受元组的嵌套结构符合表达式本身的嵌套结构，Python 就可以作出正确的对应。

#### 具名元组
`collections.namedtuple` 是一个工厂函数，它可以用来构建一个带字段名的元组和一个有名字的类——这个带名字的类对调试程序有很大帮助。
```Shell
>>> from collections import namedtuple
# 元组的四个元素分别为 名字/国家/人口/坐标, 其中坐标依旧是一个元组
>>> City = namedtuple('City', 'name country population coordinates') 
>>> tokyo = City('Tokyo', 'JP', 36.933, (35.689722, 139.691667)) 
>>> tokyo
City(name='Tokyo', country='JP', population=36.933, coordinates=(35.689722,139.691667))
```
此外, 还有 `_field` 属性, `_make()` 方法 和 `_asdict()` 方法. 

#### 作为不可变列表的元组
除了跟增减元素相关的方法之外，元组支持列表的其他所有方法, 此外, 元组没有`__reversed__` 方法，但是这个方法只是个针对 list 的优化而已，reversed(my_tuple) 这个用法在没有`__reversed__` 的情况下也是合法的。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601153905.png"/></div>
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601153936.png"/></div>

### 切片
