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
所有的序列类型都支持切片操作，但是实际上切片操作比人们所想象的要强大很多.  
切片不包含区间范围中左右一个元素是 Python 风格, 符合以 [0 为起始下标的传统](https://www.cs.utexas.edu/users/EWD/transcriptions/EWD08xx/EWD831.html).    

#### 对对象进行切片
我们可以用`s[a:b:c]` 的形式对 s 在 a 和 b 之间以 c 为间隔取值。c 的值还可以为负，负值意味着反向取值。  
这种用法作为索引或者下标在 `[]` 中返回一个切片对象:`slice(a,b,c)`. Python 会调用 `someSeq.__getitem__(slice(start, stop, step))`. 

#### 多维切片和省略
二维的numpy.ndarray 就可以用`a[i, j]` 这种形式来获取，抑或是用 `a[m:n, k:l]` 的方式来得到二维切片。这时候对象的特殊方法 `__{get/set}item__` 要以 tuple 的方式接收索引, 即调用的是 `a.__getitem__((i, j))`.  
省略（ellipsis）的正确书写方法是三个英语句号（...）, 在Python 解析器眼里是一个符号，而实际上它是 `Ellipsis` 对象的别名，而 Ellipsis 对象又是 `ellipsis` 类的单一实例。在NumPy 中，... 用作多维数组切片的快捷方式. 

#### 给切片赋值
如果把切片放在赋值语句的左边，或把它作为 del(递归定义的删除语句) 操作的对象，我们就可以对序列进行嫁接、切除或就地修改操作。
```Python
>>> l = list(range(10))
>>> l
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> l[2:5] = [20, 30]
>>> del l[5:7]
>>> l
[0, 1, 20, 30, 5, 8, 9]
```

### 对序列使用 + 和 *
Python 程序员会默认序列是支持 + 和 * 操作的,Python 会新建一个包含同样类型数据的序列来作为拼接的结果.
```Python
>>> 5 * 'abcd'
'abcdabcdabcdabcdabcd'
>>> 'hah' + 'gbfjk'
'hahgbfjk'
```
#### 建立由列表组成的列表
有时我们会需要初始化一个嵌套着几个列表的列表, 最好的选择是使用列表推导:
```Python
board = [['_'] * 3 for i in range(3)]
board[1][2] = 'X' # [['_', '_', '_'], ['_', '_', 'X'], ['_', '_', '_']]
```
另一种做法是错误的, 生成的外面的列表其实是对同一个列表的引用:
```Python
weird_board = [['_'] * 3] * 3
weird_board[1][2] = 'O' # [['_', '_', 'O'], ['_', '_', 'O'], ['_', '_', 'O']]
```

### 序列的增量赋值
+= 背后的特殊方法是`__iadd__`(in-place addition), 如果一个类没有实现 `__iadd__`, 就会退一步调用 `__add__`: 如果 a 实现了 `__iadd__`, 对可变序列, a 会就地改动，就像调用了`a.extend(b)` 一样; 如果没有实现, 就和 `a = a+b`(首先计算a + b，**得到一个新的对象**，然后赋值给a).  
因此, 对于一个 Immutable 的对象, 重复拼接操作效率会很低. 
#### 一个关于 += 的谜题
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210601163931.png"/></div>

这给我们三个教训:
* 不要把可变对象放在元组里面
* 增量赋值不是一个原子操作。我们刚才也看到了，它虽然抛出了异常，但还是完成了操作
* 查看Python 的字节码并不难，而且它对我们了解代码背后的运行机制很有帮助

### list.sort方法和内置函数sorted
list.sort 方法会就地排序列表, 这是该方法的返回值是 None 的原因，提醒你本方法不会新建一个列表。(在Python 中是惯例)  
内置函数sorted，它会新建一个列表作为返回值。不管 sorted 接受的是怎样的参数, 最后都返回一个列表. 这两个函数都可选两个关键字参数:
* reverse=
* key=

### 用bisect来管理已排序的序列
bisect(haystack, needle) 在haystack（干草垛）里搜索needle（针）的位置，该位置满足的条件是，**把 needle 插入这个位置之后，haystack 还能保持升序**.其具体表现可以从两方面调教:
* 两个可选参数: `lo` 和 `hi` 缩小范围
* 还有个姊妹函数`bisect_left`

#### 用`bisect.insort` 插入新元素
`insort(seq, item)` 把变量 item 插入到序列 seq 中，并能保持 seq 的升序顺序。insort 跟 bisect 一样，有 lo 和hi 两个可选参数用来控制查找的范围。它也有个变体叫insort_left，这个变体在背后用的是 bisect_left。

### 当列表不是首选时
面对各类需求时，我们可能会有更好的选择。
#### 数组
如果我们需要一个只包含数字的列表，那么array.array 比list 更高效。数组支持所有跟可变序列有关的操作，包括`.pop`、`.insert` 和`.extend`, 并且支持`array.tofile` 和`array.fromfile`, 它们用起来很简单速度也块。   
创建数组需要一个类型码，这个类型码用来表示在底层的 C 语言应该存放怎样的数据类型.
```Shell
>>> from array import array
>>> from random import random
>>> floats = array('d', (random() for i in range(10**7)))
# 创建列表用 listcom, 这是一个array, 要用 genexp
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210602hfhahf.png"/></div>

#### 内存视图
memoryview 是一个内置类，它能让用户在不复制内容的情况下操作同一个数组的不同切片。memoryview.cast 的概念跟数组模块类似，能用不同的方式读写同一块内存数据(有点类似于C的类型转换, 用不同的编码,如 unsigned/signed 去读同样的数据)，而且内容字节不会随意移动。  

#### NumPy和SciPy
NumPy 和SciPy 提供的高阶数组和矩阵操作,都是异常强大的库，也是其他一些很有用的工具的基石.

#### 双向队列和其他形式的队列
`collections.deque` 类（双向队列）是一个线程安全、可以快速从两端添加或者删除元素的数据类型。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210602jkfljkijfoiahf.png"/></div>

除了 deque, Python标准库还有下面的队列实现：
* queue：提供了同步（线程安全）类Queue、LifoQueue 和PriorityQueue，不同的线程可以利用这些数据类型来交换信息。
* multiprocessing: 这个包实现了自己的Queue，它跟queue.Queue 类似，是设计给进程间通信用的。
* asyncio: 里面有Queue、LifoQueue、PriorityQueue 和JoinableQueue, 为异步编程里的任务管理提供了专门的便利。
* heapq: 没有队列类，而是提供了heappush 和heappop 方法，让用户可以把可变序列当作堆队列或者优先队列来使用。

## Chap 3: 字典和集合
dict 类型不但在各种程序里广泛使用，它也是Python 语言的基石。
### 泛映射类型
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210602100251.png"/></div>

`collections.abc` 模块中有 `Mapping` 和 `MutableMapping` 这两个抽象基类，它们的作用是为dict 和其他类似的类型定义形式接口.  
所有映射类型都是利用dict 来实现的，因此它们有个共同的限制，即只有可散列的数据类型才能用作这些映射里的键(value 则不受此限制).  
如果一个对象是可散列的，那么在这个对象的生命周期中，它的**散列值是不变的**，而且这个对象需要实现 `__hash__()` 方法。另外可散列对象还要有 `__eq__()` 方法， 这样才能跟其他键做比较。如果**两个可散列对象是相等的，那么它们的散列值一定是一样的**.  
### 字典推导
字典推导（dictcomp）可以从任何以键值对作为元素的可迭代对象中构建出字典。
```Python
>>> DIAL_CODES = [ 
... (86, 'China'),
... (81, 'Japan'),
... ]
>>> {code: country.upper() for country, code in country_code.items() 
... if code > 66}
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210602102238.png"/></div>

#### 用setdefault处理找不到的键
当字典 `dict[key]` 不能找到正确的键的时候，Python 会抛出异常, 可以用 `d.get(k, default)` 来代替d[k]，给找不到的键一个默认的返回值,[使用 setdefault 是一个更好的做法](https://github.com/fluentpython/example-code/blob/master/03-dict-set/index.py), 其中的:
```Python
index.setdefault(word, []).append(location)
```
就相当于:
```Python
if key not in my_dict:
    my_dict[key] = []
my_dict[key].append(new_value)
```
### 映射的弹性键查询
如果我们希望键在映射中不存在的时候依旧能读取到默认值, 可以有两种做法:
* 使用 `defaultdict` 
* 定义一个 dict 的子类, 同时实现 `__missing__`

#### defaultdict
实例化一个 `defaultdict` 的时候，需要给构造方法提供一个**可调用对象**，这个可调用对象会在 `__getitem__` 碰到找不到的键的时候[被调用](https://github.com/fluentpython/example-code/blob/master/03-dict-set/index_default.py)，让 `__getitem__` 返回某种默认值。如 `dd = defaultdict(list)`, 如果键 key 不存在, 则:
1. 调用list() 来建立一个新列表
2. 把这个新列表作为值，'key' 作为它的键，放到 dd 中
3. 返回这个列表的引用。

#### 特殊方法`__missing__`
基类 dict 并没有定义这个方法，但是 dict 是知道有这么个东西存在的。如果继承类提供了`__missing__` 方法，那么在`__getitem__` 碰到找不到的键的时候，Python 就会自动调用它，而不是抛出一个 KeyError 异常。  
```Python
class StrKeyDict0(dict): 
    def __missing__(self, key):
        if isinstance(key, str): 
            raise KeyError(key)
        return self[str(key)]
```
上面的例子是, 如果我们用一个 str 去查找可编程电路板的针脚, 但是很多时候我们输入针脚号为 int 的时候, 需要能查找成功对应的针脚, 查找失败有两种情况:
1. 如果输入的为 str 并且没有对应的接口, 那么就失败
2. 如果输入的为 int , 我们将它转化称为 str 之后再查找

### 字典的变种
collections 模块中，除了defaultdict 之外还有不同映射类型:
* collections.OrderedDict: 在添加键的时候会保持顺序，因此键的迭代次序总是一致的
* collections.ChainMap: 可以容纳数个不同的映射对象，然后在进行键查找操作的时候，这些对象会被当作一个整体被逐个查找，直到键被找到为止。
* collections.Counter: 这个映射类型会给键准备一个整数计数器。每次更新一个键的时候都会增加这个计数器。

### 子类化UserDict
创造自定义映射类型来说，以 UserDict 为基类，比以普通的 dict 为基类要方便, 后者有时会**在某些方法的实现上走一些捷径**，导致我们不得不在它的子类中重写这些方法，但是UserDict 就不会带来这些问题. UserDict 并不是 dict 的子类，但是 UserDict 有一个叫作data 的属性，是dict 的实例，这个属性实际上是UserDict 最终存储数据的地方(背后的思想在 C++ 中很常见, **用组合替代继承**)。  
此外, UserDict 继承的是 `MutableMapping`，所以 StrKeyDict 里剩下的那些映射类型的方法都是从UserDict、MutableMapping 和Mapping 这些超类继承而来的, 下面两个方法非常有用:
* MutableMapping.update
* Mapping.get

### 不可变映射类型
types 模块中引入了一个封装类名叫MappingProxyType。如果给这个类一个映射，它会返回一个只读的映射视图。虽然是个只读视图，但是它是动态的。这意味着如果对原映射做出了改动，我们**通过这个视图可以观察到，但是无法通过这个视图对原映射做出修改**。  
```Python
>>> from types import MappingProxyType
>>> d = {1:'A'}
>>> d_proxy = MappingProxyType(d)
>>> d_proxy[2] = 'x'
TypeError: 'mappingproxy' object does not support item assignment
```

### 集合论
集合的本质是许多**唯一对象**的聚集, 集合中的元素必须是可散列的，set 类型本身是不可散列的，但是frozenset 可以. 集合还实现了很多基础的中缀运算符, 如交集/并集/差集. 其极快的查找功能来源于背后的散列表, 内置的set 和frozenset 提供了丰富的功能和操作.
#### 集合字面量
set 可以有字面量的形式(`{'a','b'}`), 但是空集要用`set()`, 因为 `{}` 表示的是空字典. 但是没有针对 frozenset 的特殊字面量句法，我们只能采用构造方法。 
```Python
>>> frozenset(range(10))
frozenset({0, 1, 2, 3, 4, 5, 6, 7, 8, 9})
```

#### 集合推导
同样的, 集合也可以有类似于字典的推导:
```Python
>>> from unicodedata import name 
>>> {chr(i) for i in range(32, 256) if 'SIGN' in name(chr(i),'')}
{'§', '=', '¢', '#', '¤', '<', '¥', 'μ', '×', '$', '¶', '£', '©',
'°', '+', '÷', '±', '>', '¬', '®', '%'}
```

#### 集合的操作
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210602161905.png"/></div>

* 集合的数学运算：这些方法或者会生成新集合，或者会在条件允许的情况下就地修改集合
* 集合的比较运算符，返回值是布尔类型
* 集合的其他方法包含添加/清除元素等

### dict和set的背后
#### 散列表
dict/set 背后的散列表其实是一个**稀疏数组** (总是有空白元素的数组称为稀疏数组,sparse array).Python 会设法保证大概还有三分之一的表元是空的，所以在快要达到这个阈值的时候，原有的散列表会被复制到一个更大的空间里面。
#### 散列值和相等性
自定义对象调用hash() 的话，实际上运行的是自定义的 `__hash__`。如果两个对象在比较的时候是相等的，那它们的散列值必须相等，否则散列表就不能正常运行. 在最理想的状况下，越是相似但不相等的对象，它们散列值的差别应该越大。
#### 散列表算法
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210602163143.png"/></div>

`my_dict[search_key]` 的背后 Python 首先会调用hash(search_key) 来计算search_key 的散列值，把这个值最低的几位数字当作偏移量，在散列表里查找表元.  
若不是空的，则表元里会有一对`found_key:found_value`。这时候Python 会检验`search_key ==found_key` 是否为真，如果它们相等的话，就会返回 found_value。 
不匹配的话，这种情况称为散列冲突, 为了解决散列冲突，算法会在散列值中另外再取几位，然后用特殊的方法处理一下，把新得到的数字再当作索引来寻找表元. 

#### dict的实现及其导致的结果
1. 键必须是可散列的
   1. 支持hash() 函数，并且通过__hash__() 方法所得到的散列值是不变的。
   2. 支持通过__eq__() 方法来检测相等性。
   3. 若a == b 为真，则hash(a) == hash(b) 也为真。
2. 字典在内存上的开销巨大
3. 键查询很快
4. 键的次序取决于添加顺序

set 的实现和结果和上面十分类似.

