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

## Chap 4: 文本和字节序列
Python3 明确区分了人类可读的文本字符串和原始的字节序列, 我们需要理解字符串和字节序列的区别. 
### 字符问题
一个字符串是一个字符序列, 重要的是**如何定义"字符"**, 目前“字符”的最佳定义是 Unicode 字符。  
* 字符的标识，即码位, 是前缀 `U+` 的4~6个十六位数字
* 字符的具体表述取决于所用的编码

码位转化为字节序列的过程被称为编码, 反之就是解码:
```Python
>>> s = 'café'
>>> len(s) 
4
>>> b = s.encode('utf8') 
>>> b
b'caf\xc3\xa9' 
>>> len(b)
5
>>> b.decode('utf8') 
'café'
```
Python 3 的bytes 类型却不是把str 类型换个名称那么简单，而且还有关系紧密的bytearray 类型。

### 字节概要
Python 3 引入了不可变bytes 类型,Python 2.6 添加了可变 bytearray 类型, bytes 或bytearray 对象的各个元素是介于0~255（含）之间的整数，虽然字面量表示法表明其中有 ASCII 文本, 但是可能使用三种不同的方式显示:
* 可打印的 ASCII 范围内的字节, 使用 ASCII 字符本身显示
* 制表符/换行符/回车符 等, 使用转义序列
* 其他字节的值, 使用十六进制的转义序列

除了格式化方法和几个处理 Unicode 的方法, str 类的其他方法都支持 bytes 和 bytearray 类型.构建bytes 或bytearray 实例还可以调用各自的构造方法:
* 一个str 对象和一个encoding 关键字参数
* 一个可迭代对象，提供 0~255 之间的数值。
* 一个实现了缓冲协议的对象(使用缓冲类对象构建二进制序列是一种低层操作，可能涉及类型转换)

#### 结构体和内存视图
struct 模块提供了一些函数，把打包的字节序列转换成不同类型字段组成的元组，还有一些函数用于执行反向转换，把元组转换成打包的字节序列.  
memoryview 类不是用于创建或存储字节序列的，而是共享内存，让你访问其他二进制序列、打包的数组和缓冲中的数据切片，而无需复制字节序列.  
### 基本的编解码器
Python 自带了超过100 种编解码器, 用于在文本和字节之间相互转换. 只需要将编码/解码器名称传给 `open`/`str.encode()`,`bytes.decode()` 等函数的 encoding 参数.

### 了解编解码问题
导致编码问题的是 可能是UnicodeEncodeError、UnicodeDecodeError 或者 SyntaxError 等其他错误.
#### 处理UnicodeEncodeError
把文本转换成字节序列时，如果**目标编码中没有定义某个字符，那就会抛出UnicodeEncodeError 异常**，除非把errors参数传给编码方法或函数，对错误进行特殊处理。
```Python
>>> city = 'São Paulo'
>>> city.encode('cp437', errors='ignore') 
b'So Paulo'
>>> city.encode('cp437', errors='replace') 
b'S?o Paulo'
>>> city.encode('cp437', errors='xmlcharrefreplace') 
b'S&#227;o Paulo'
```
#### 处理UnicodeDecodeError
不是每一个字节都包含有效的ASCII 字符，也不是每一个字符序列都是有效的UTF-8 或UTF-16。但是很多陈旧的编码能解码任何的字节序列流而不抛出错误, 如随机噪声. 
```Python
>>> octets = b'Montr\xe9al'
>>> octets.decode('utf_8', errors='replace')
'Montr􀓠al'
```

#### 使用预期之外的编码加载模块时抛出的SyntaxError
Python 3 默认使用UTF-8 编码源码，如果加载的.py 模块中包含 UTF-8 之外的数据，而且没有声明编码，会得到 SyntaxError...  
为了修正这个问题，可以在文件顶部添加一个神奇的coding 注释.
```Python
# coding: cp1252
print('Olá, Mundo!')
```
#### 如何找出字节序列的编码
简单来说，不能。必须有人告诉你。只要假定字节流是人类可读的纯文本，就可能通过试探和分析找出编码, 如统一字符编码侦测包Chardet能识别所支持的30 种编码.

#### BOM：有用的鬼符
BOM，即字节序标记（byte-order mark），指明编码时使用Intel CPU 的小字节序。  
在小字节序设备中，各个码位的最低有效字节在前面, 在大字节序CPU 中，编码顺序是相反的, 为了避免混淆，UTF-16 编码在要编码的文本前面加上特殊的不可见字符ZERO WIDTH NOBREAK SPACE（U+FEFF）。

### 处理文本文件
处理文本的最佳实践是“Unicode 三明治”, 要尽早把输入（例如读取文件时）的字节序列解码成字符串。对输出来说，则要尽量晚地把字符串编码成字节序列。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210603145234.png"/></div>

处理文本很简单, 但是如果依赖默认编码, 就会遇到麻烦:`locale.getpreferredencoding()` 返回的编码是最重要的, 这是打开文件的默认编码，也是重定向到文件的sys.stdout/stdin/stderr 的默认编码。
* 没有指定 encoding 参数，默认值由 `locale.getpreferredencoding()` 提供
* 如果设定了PYTHONIOENCODING 环境变量, sys.stdout/stdin/stderr 的编码使用设定的值

**关于编码默认值的最佳建议是：别依赖默认值**。
### 为了正确比较而规范化Unicode字符串
```Python
>>> s1 = 'café'
>>> s2 = 'cafe\u0301'
>>> s1,s2
('café', 'café')
>>> s1 == s2
False
```
在Unicode 标准中，'é' 和'e\u0301' 这样的序列叫“标准等价物”（canonical equivalent），应用程序应该把它们视作相同的字符, **但是Python看到的是不同的码位序列, 因此判定两者不相等**. 我们用 `unicodedata.normalize` 函数提供的Unicode 规范化, 第一个参数是 'NFC'(Normalization Form C)、'NFD'、'NFKC' 和'NFKD'其中之一
#### 大小写折叠
大小写折叠其实就是把所有文本变成小写，再做些其他转换(由`str.casefold()`方法支持)。  
#### 规范化文本匹配实用函数
NFC 和 NFD 可以放心使用，而且能合理比较 Unicode 字符串。如果要处理多语言文本，工具箱中应该有 nfc_equal 和fold_equal 函数。
#### 极端“规范化”：去掉变音符号
[这个例子](https://github.com/fluentpython/example-code/blob/master/04-text-byte/sanitize.py)会对文本做进一步处理，很有可能会改变原意。只有知道目标语言、目标用户群和转换后的用途，才能确定要不要做这么深入的规范化。

### 支持字符串和字节序列的双模式API
#### 正则表达式中的字符串和字节序列
如果使用字节序列构建正则表达式，\d 和\w 等模式**只能匹配 ASCII 字符**；相比之下，如果是字符串模式，就能匹配 ASCII 之外的 Unicode 数字或字母.
#### os函数中的字符串和字节序列
os 模块中的所有函数、文件名或路径名参数既能使用字符串，也能使用字节序列。如果这样的函数使用字符串参数调用，该**参数会使用sys.getfilesystemencoding() 得到的编解码器自动编码，然后操作系统会使用相同的编解码器解码**。

# Part III: Functions as Objects
## Chap 5: First-Class Function
在Python 中，函数是一等对象:
* 在运行时创建
* 能赋值给变量或数据结构中的元素
* 能作为参数传给函数
* 能作为函数返回的结果

### 把函数视作对象
Python 函数是对象, 我们可以将它赋值给变量, 然后通过变量名调用, 还可以作为参数传给map 函数.
```Python
>>> fac = factorial
>>> fac
<function factorial at 0x0000017e6be85160>
>>> m2fac = map(factorial, range(11)) 
>>> list(m2fac)
```
### 高阶函数
接受函数为参数，或者把函数作为结果返回的函数是高阶函数.在函数式编程范式中，最为人熟知的高阶函数有map、filter、reduce 和apply(Python 3中已移除)。
#### map、filter和reduce的现代替代品
函数式语言通常会提供map、filter 和reduce 三个高阶函数, Python 3 中，map 和filter 还是内置函数，但是由于引入了列表推导和生成器表达式，它们变得没那么重要了。列表推导或生成器表达式具有map 和filter 两个函数的功能，而且更易于阅读:
```Python
>>> list(map(fact, range(6))) 
[1, 1, 2, 6, 24, 120]
>>> [fact(n) for n in range(6)] 
[1, 1, 2, 6, 24, 120]
>>> list(map(factorial, filter(lambda n: n % 2, range(6)))) 
[1, 6, 120]
>>> [factorial(n) for n in range(6) if n % 2] 
[1, 6, 120]
```
reduce 在 Python 3 中放到functools 模块里, sum 和reduce 的通用思想是把某个操作连续应用到序列的元素上，累计之前的结果，把一系列值归约成一个值。

### 匿名函数
Python 简单的句法限制了lambda 函数的定义体**只能使用纯表达式**。换句话说，lambda 函数的定义体中不能赋值，也不能使用while 和try 等Python 语句。
```Python
>>> fruits = ['strawberry', 'fig', 'apple', 'cherry', 'raspberry', 'banana']
>>> sorted(fruits, key=lambda word: word[::-1])
['banana', 'apple', 'fig', 'raspberry', 'strawberry', 'cherry']
```  
除了作为参数传给高阶函数之外，Python 很少使用匿名函数。由于句法上的限制，非平凡的lambda 表达式要么难以阅读，要么无法写出。
### 可调用对象
除了用户定义的函数，调用运算符（即`()`, 类似于C++中的lambda|functor|function pointer）还可以应用到其他对象上,使用内置的callable() 函数判断:
* 用户定义的函数
* 内置函数
* 内置方法
* 方法
* 类: 调用类时会运行类的`__new__` 方法创建一个实例，然后运行`__init__` 方法，初始化实例，最后把实例返回给调用方。因为Python 没有new 运算符，所以调用类相当于调用函数。
* 类的实例: 如果类定义了`__call__` 方法，那么它的实例可以作为函数调用。
* 生成器函数:使用yield 关键字的函数或方法。调用生成器函数返回的是生成器对象。(函数使用`return` 来进行返回值，每调用一次，返回一个新加工好的数据返回给你；yield不同，它会在调用生成器的时候，把数据生成object，然后**当你需要用的时候，要用next()方法来取，同时不可逆**)

### 用户定义的可调用类型
不仅Python 函数是真正的对象，任何Python 对象都可以表现得像函数。为此，只需实现实例方法`__call__`。实现`__call__` 方法的类是创建函数类对象的简便方式，此时必须在内部维护一个状态，让它在调用之间可用，例如 BingoCage 中的剩余元素。

### 函数内省
与用户定义的常规类一样，函数使用 `__dict__` 属性存储赋予它的用户属性。计算两个属性集合的差集便能得到函数专有属性列表:
```Python
>>> class C: pass 
>>> obj = C() 
>>> def func(): pass 
>>> sorted(set(dir(func)) - set(dir(obj))) 
```
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210607095236.png"/></div>

### 从定位参数到仅限关键字参数
Python 3 进一步提供了仅限关键字参数, 调用函数时使用 `*` 和 `**` “展开” 可迭代对象，映射到单个参数。一个用于生成关HTML标签的[函数](https://github.com/fluentpython/example-code/blob/master/05-1class-func/tagger.py)使用起来就有很多方式:
```Python
>>> tag('br')  # <1>
'<br />'
>>> tag('p', 'hello')  # <2>
'<p>hello</p>'
```
稍微解释一下:
* 第一个参数后面的任意个参数会被 `*content` 捕获，存入一个元组。
* tag 函数签名中没有明确指定名称的关键字参数会被 `**attrs` 捕获，存入一个字典。
* 仅限关键字参数是Python 3 新增的特性。cls 参数只能通过关键字参数指定，它一定不会捕获未命名的定位参数。

### 获取关于参数的信息
函数对象有个`__defaults__` 属性，它的值是一个元组，里面保存着定位参数和关键字参数的默认值。仅限关键字参数的默认值在`__kwdefaults__` 属性中。参数的名称在`__code__` 属性中，它的值是一个code 对象引用，自身也有很多属性。我们从[这个](https://github.com/fluentpython/example-code/blob/master/05-1class-func/clip_introspection.rst)例子中看出, 默认的组织信息的方式并不是最便利的, 我们可以使用inspect 模块。  
inspect.Signature 对象有个bind 方法，它可以把任意个参数绑定到签名中的形参上，所用的规则与实参到形参的匹配方式一样。框架可以使用这个方法在真正调用函数前验证参数.

### 函数注解
Python 3 提供了一种句法，用于为函数声明中的参数和返回值附加元数据:
```Python
def clip(text:str, max_len:'int > 0'=80) -> str:
    ...
```
函数声明中的各个参数可以在 `:` 之后增加注解表达式,如果参数有默认值，注解放在**参数
名和 `=` 号之间**。如果想注解返回值，在`) `和函数声明末尾的: 之间添加`->` 和一个表达式。  
Python **不做检查、不做强制、不做验证，什么操作都不做**。换句话说注解对 Python 解释器没有任何意义。注解只是元数据，可以供IDE、框架和装饰器等工具使用。 

### 支持函数式编程的包
得益于operator 和functools 等包的支持，函数式编程风格也可以信手拈来。
#### operator模块
在函数式编程中，经常需要把算术运算符当作函数使用。
```Python
from functools import reduce
def fact(n):
    return reduce(lambda a, b: a*b, range(1, n+1))
```
operator 模块为多个算术运算符提供了对应的函数，从而避免编写`lambda a, b: a*b` 这种平凡的匿名函数。
```Python
from functools import reduce
from operator import mul
def fact(n):
    return reduce(mul, range(1, n+1))
```
operator 模块中还有一类函数，能替代从序列中取出元素或读取对象属性的lambda 表达式：因此，`itemgetter` 和`attrgetter` 其实会自行构建函数。
* itemgetter 使用[] 运算符，因此它不仅支持序列，还支持映射和任何实现`__getitem__` 方法的类。
* attrgetter 与 itemgetter 作用类似，它创建的函数根据名称提取对象的属性。(如果参数名中包含`.`（点号），attrgetter 会深入嵌套对象，获取指定的属性)

#### functools.partial 冻结参数
functools 除了 reduce 之外, 最有用的是partial 及其变体，partialmethod. functools.partial 这个高阶函数用于部分应用一个函数。部分应用是指，**基于一个函数创建一个新的可调用对象，把原函数的某些参数固定**。
```Python
>>> from operator import mul
>>> from functools import partial
>>> triple = partial(mul, 3) 
>>> triple(7) 
136 
21
>>> list(map(triple, range(1, 10))) 
[3, 6, 9, 12, 15, 18, 21, 24, 27]
```
这其实有点像 适配器模式. `functools.partialmethod` 函数（Python 3.4 新增）的作用与partial 一样，不过是**用于处理方法**.

## Chap 6: 使用一等函数实现设计模式
Norvig 建议在有一等函数的语言中**重新审视“策略”“命令”“模板方法”和“访问者”模式**。通常，我们可以把这些模式中涉及的某些类的实例替换成简单的函数，从而减少样板代码。
### 案例分析：重构“策略”模式
如果合理利用作为一等对象的函数，某些设计模式可以简化，“策略”模式就是其中一个很好的例子。
#### 经典的策略模式
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210607110800.png"/></div>

“策略”模式的定义如下:
> 定义一系列算法，把它们一一封装起来，并且使它们可以相互替换。本模式使得算法可以独立于使用它的客户而变化。

* 上下文: 把一些计算委托给实现不同算法的可互换组件，它提供服务。
* 策略: 实现不同算法的组件共同的接口。
* 具体策略: “策略”的具体子类。fidelityPromo、BulkPromo 和 LargeOrderPromo 是这里实现的三个具体策略。

#### Function-Oriented Strategy
在[经典的实现](https://github.com/fluentpython/example-code/blob/master/06-dp-1class-func/classic_strategy.py)中, 每个具体策略都是一个类，而且都只定义了一个方法，即discount。此外，策略实例没有状态(没有实例属性), 我们可以**把具体策略换成了简单的函数，而且去掉了[Promo抽象类](https://github.com/fluentpython/example-code/blob/master/06-dp-1class-func/strategy.py)**。

#### 选择最佳策略：简单的方式
```Python
promos = [fidelity_promo, bulk_item_promo, large_order_promo] 
def best_promo(order): 
    """选择可用的最佳折扣
    """
    return max(promo(order) for promo in promos) 
```
这个实现把所有的策略卸在一个列表中, 十分简单易读, 但是**若想添加新的促销策略，要定义相应的函数，还要记得把它添加到 promos 列表中**.

#### 找出模块中的全部策略
在Python 中，模块也是一等对象，而且标准库提供了几个处理模块的函数。Python 文档是这样说明内置函数 `globals` 的。
> 返回一个字典，表示当前的**全局符号表**。这个符号表始终针对当前模块（对函数或方法来说，是指定义它们的模块，而不是调用它们的模块）。

```Python
promos = [globals()[name] for name in globals() 
            if name.endswith('_promo') 
            and name != 'best_promo']
```
收集所有可用促销的另一种方法是，在一个单独的模块中保存所有策略函数，把`best_promo` 排除在外。
```Python
promos = [func for name, func in
                   inspect.getmembers(promotions, inspect.isfunction)]
```
动态收集促销折扣函数更为显式的一种方案是使用简单的装饰器。

### 命令模式
“命令”设计模式也可以通过把函数作为参数传递而简化。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210607113111.png"/></div>

> “命令”模式的目的是 **解耦调用操作的对象（调用者）和提供实现的对象（接收者）**

这个模式的做法是，在二者之间放一个Command 对象，让它实现只有一个方法（execute）的接口，调用接收者中的方法执行所需的操作。这样，调用者无需了解接收者的接口，而且不同的接收者可以适应不同的Command 子类。**MacroCommand 可能保存一系列命令**，它的execute()方法会在各个命令上调用相同的方法。  
我们可以不为调用者提供一个Command 实例，而是给它一个函数, MacroCommand 可以实现成定义了`__call__`方法的类。这样，MacroCommand 的实例就是可调用对象，各自维护着一个函数列表，供以后调用:
```Python
class MacroCommand:
    """一个执行一组命令的命令"""
    def __init__(self, commands):
        self.commands = list(commands) 
    def __call__(self):
        for command in self.commands: 
            command()
```
这两个模式之间的区别在于:
* 命令模式多了一个接收者（Receiver）角色
* 策略模式的意图是封装算法，它认为“算法”已经是一个**完整的、不可拆分的原子业务**（注意这里是原子业务，而不是原子对象），即其意图是让这些**算法独立，并且可以相互替换，让行为的变化独立于拥有行为的客户**
* 命令模式则是对动作的解耦，把一个动作的执行分为**执行对象（接收者角色）、执行行为（命令角色），让两者相互独立而不相互影响**。

**“命令”和“策略”模式（以及“模板方法”和“访问者”模式）可以使用一等函数实现，这样更简单，甚至“不见了”**(至少对于某些用途而言是这样).

## Chap 7: 函数装饰器和闭包
函数装饰器用于**在源码中“标记”函数，以某种方式增强函数的行为**。除了在装饰器中有用处之外，闭包还是回调式异步编程和函数式编程风格的基础.  
### 装饰器基础
装饰器是可调用的对象(一般是高阶函数)，其参数是另一个函数（被装饰的函数）, 一般而言处理被装饰的函数, 然后将其返回, 当然也可以将它替换成另一个函数或者可调用对象. 
```Python
@decorate
def target():
    print('running target()')
# 相当于下面
def target():
    print('running target()')
target = decorate(target) # 从这里可以清楚看到, decorate 不一定要返回 target
```
装饰器只是语法糖, 装饰器可以像常规的可调用对象那样调用，其参数是**另一个函数**。很多时候，这样做更方便，尤其是做元编程（在运行时改变程序的行为）时。

### Python何时执行装饰器
装饰器的一个关键特性是，它们在**被装饰的函数定义之后立即运行**,这通常是在导入时(即Python 加载模块时).
```shell
$ python3 registration.py
running register(<function f1 at 0x100631bf8>)
running register(<function f2 at 0x100631c80>)
running main()
registry -> [<function f1 at 0x100631bf8>, <function f2 at 0x100631c80>]
running f1()
running f2()
running f3()
```
从[上面例子](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/registration.py)的输出可以发现, register 在模块中其他函数之前运行（两次）。**函数装饰器在导入模块时立即执行，而被装饰的函数只在明确调用时运行**。这突出了Python 程序员所说的导入时和运行时之间的区别。  
### 使用装饰器改进“策略”模式
之前的示例有个很重要的问题就是**新增策略后可能会忘记将它放到 `promos` 列表中**, 使用[注册装饰器](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/strategy_best4.py)解决了这个问题.和之前的相比有几个优点:
* 促销策略函数无需使用特殊的名称（即不用以_promo 结尾）
* @promotion 装饰器突出了被装饰的函数的作用，还便于临时禁用某个促销策略：只需把装饰器注释掉。
* 促销折扣策略可以在其他模块中定义，在系统中的任何地方都行，只要使用`@promotion`装饰即可。

需要注意的是, **多数装饰器会修改被装饰的函数**。通常，它们会定义一个内部函数，然后将其返
回，替换被装饰的函数。**使用内部函数的代码几乎都要靠闭包才能正确运作**。

### 变量作用域规则
Python 不要求声明变量，但是假定**在函数定义体中赋值的变量是局部变量**。如果在函数中赋值时想让解释器**把它当成全局变量，要使用global 声明**：
```Python
>>> b = 6
>>> def f3(a):
...     global b
...     print(a)
...     print(b)
...     b = 9
...
>>> f3(3)
3
6
>>> b
9
```

### 闭包
人们有时会把闭包和匿名函数弄混, 因为**在函数内部定义函数不常见，直到开始使用匿名函数才会这样做**。闭包指延伸了作用域的函数，其中包含函数定义体中引用、不在定义体中定义的非全局变量。  
对比一个 avg 的 [OO实现](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/average_oo.py) 和 [高阶函数实现](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/average.py), 前者的历史值存在 `self.series` 属性中, 而高阶函数中, 看上去调用avg(10) 时，`make_averager` 函数已经返回了，而它的本地作用域也一去不复返。实际上, averager 函数中，series 是自由变量（free variable）。这是一个技术术语，指未在本地作用域中绑定的变量:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210607143835.png"/></div>

闭包是一种函数，它会保留**定义函数时存在的自由变量的绑定**，这样调用函数时，虽然定义作用域不可用了，但是仍能使用那些绑定。只有嵌套在其他函数中的函数才可能需要处理不在全局作用域中的外部变量。
### nonlocal声明
在另一种实现中, 我们在averager 的定义体中为count 赋值了，这会把count 变成局部变量。total 变量也受这个问题影响。前面的没有报错是因为列表是可变的, 但是对数字、字符串、元组等不可变类型来说，只能读取，不能更新。如果尝试重新绑定，例如`count = count + 1`，其实会**隐式创建局部变量count**。这样，count 就不是自由变量了，因此不会保存在闭包中。
```Python
def make_averager():
    count = 0
    total = 0
    def averager(new_value):
        count += 1
        total += new_value
        return total / count
        return averager
```
Python 3 引入了 `nonlocal` 声明。它的作用是**把变量标记为自由变量，即使在函数中为变量赋予新值了，也会变成自由变量**。如果为 `nonlocal` 声明的变量赋予新值，闭包中保存的绑定会更新。
```Python
def make_averager():
    count = 0
    total = 0
    def averager(new_value):
        nonlocal count, total
        count += 1
        total += new_value
        return total / count
        return averager
```
### 实现一个简单的装饰器
[这里](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/clockdeco.py)是实现装饰器的典型行为：把被装饰的函数**替换成新函数**，二者接受相同的参数，而且（通常）**返回被装饰的函数本该返回的值，同时还会做些额外操作**。  

### 标准库中的装饰器
标准库中最值得关注的两个装饰器是lru_cache 和全新的singledispatch.
#### 使用`functools.lru_cache`做备忘
它实现了备忘（memoization）功能。这是一项优化技术，它**把耗时的函数的结果保存起来，避免传入相同的参数时重复计算**。
```Python
import functools
from clockdeco import clock
@functools.lru_cache() 
@clock 
def fibonacci(n):
    if n < 2:
        return n
    return fibonacci(n-2) + fibonacci(n-1)
if __name__=='__main__':
    print(fibonacci(30))
```
使用这个装饰器后, fib 的调用次数从2 692 537降低到了 31.这个装饰器可以使用两个可选的参数来配置。它的签名是：
```Python
functools.lru_cache(maxsize=128, typed=False)
```
lru_cache 使用字典存储结果，而且键根据调用时传入的定位参数和关键字参数创建，所以被lru_cache 装饰的函数，它的**所有参数都必须是可散列的**。

#### 单分派泛函数
因为 Python **不支持重载方法或函数**，所以我们不能使用不同的签名定义htmlize 的变体，也无法使用不同的方式处理不同的数据类型。`functools.singledispatch` 装饰器可以把整体方案拆分成多个模块，甚至可以为你无法修改的类提供专门的函数。使用后的普通函数根据第一个参数的类型，以不同方式执行相同操作的一组函数。这个[例子](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/generic.py)中有几点需要特别强调的:
* 我们先是要用装饰器标记基函数
* 对于专门的类型使用 `@<base>.register(<type>)` 注册专用的函数
* 只要可能，注册的专门函数应该处理抽象基类,代码支持的兼容类型更广泛。

### 叠放装饰器
装饰器是函数，因此可以组合起来使用, 把@d1 和@d2 两个装饰器按顺序应用到f 函数上，作用相当于`f = d1(d2(f))`。
### 参数化装饰器
我们可以让装饰器接收其他参数, **创建一个装饰器工厂函数，把参数传给它，返回一个装饰器，然后再把它应用到要装饰的函数上**。
#### 一个参数化的注册装饰器
为了便于启用或禁用register 执行的函数注册功能，我们为[它](https://github.com/fluentpython/example-code/blob/master/07-closure-deco/registration_param.py)提供一个可选的active 参数，设为False 时，不注册被装饰的函数. 其中的 register 函数**不是装饰器，而是装饰器工厂函数**, 它的返回值是一个装饰器。  

# Part IV: 面向对象惯用法
## Chap 8: 对象引用、可变性和垃圾回收
本章的主题是对象与对象名称之间的区别。名称不是对象，而是单独的东西。**变量是标注，而不是盒子**。  
### 变量不是盒子
人们经常使用“变量是盒子”这样的比喻，但是这有碍于理解面向对象语言中的引用式变量。**最好把它们理解为附加在对象上的标注**:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210607160328.png"/></div>

如上图所示, 如果理解为盒子, 就无法解释 Python 中的赋值. 对引用式变量来说，说把变量分配给对象更合理，反过来说就有问题(或者说是**变量绑定到对象上,就像为对象贴上标注**).

### 标识、相等性和别名
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210607160929.png"/></div>

```Python
>>> alex == charles 
True
>>> alex is not charles 
True
```
在上图显示的结果中, lewis 和charles 是别名，即两个变量绑定同一个对象。而alex 不是charles 的别名，因为二者绑定的是不同的对象。alex 和charles 绑定的对象具有相同的值（`==` 比较的就是值），但是它们的标识不同。  
> 每个变量都有标识、类型和值。对象一旦创建，它的标识绝不会变；你可以把标识理解为对象在内存中的地址。is 运算符比较两个对象的标识；id() 函数返回对象标识的整数表示。

#### 在==和is之间选择
== 运算符比较两个对象的值（对象中保存的数据），而is 比较对象的标识。通常我们关注的是值，而不是标识，因此Python 代码中 `==` 出现的频率比 is 高.  
a == b 是语法糖，等同于`a.__eq__(b)`。继承自object 的`__eq__`方法比较两个对象的ID，结果与is 一样。但是多数内置类型使用更有意义的方式覆盖了`__eq__` 方法，会考虑对象属性的值。  
#### 元组的相对不可变性
**元组与多数 Python 集合一样, 保存的是对象的引用**.元组的不可变性其实是指 tuple 数据结构的物理内容（即保存的引用）不可变，与引用的对象无关。  
元组的值会随着引用的可变对象的变化而变,元组中不可变的是元素的标识。
```Python
>>> t1 = (1, 2, [30, 40])
>>> id(t1[-1]) 
4302515784
>>> t1[-1].append(99)
>>> id(t1[-1]) 
4302515784
```

### 默认做浅复制
复制列表（或多数内置的可变集合）最简单的方式是使用内置的类型构造方法, 构造方法或`[:]` 做的是浅复制（即**复制了最外层容器，副本中的元素是源容器中元素的引用**,例如, 如果是一个 list of list, 那么复制时候是复制的最外层list, 新旧两个list id 依旧不同）。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210608094936.png"/></div>

#### 为任意对象做深复制和浅复制
copy模块提供的`deepcopy` 和`copy` 函数能为任意对象做深复制和浅复制。一般来说，深复制不是件简单的事。如果对象有循环引用，那么这个朴素的算法会进入无限循环。deepcopy 函数会记住已经复制的对象，因此能优雅地处理循环引用.当然, 我们可以实现特殊方法`__copy__()` 和`__deepcopy__()`，控制copy 和deepcopy 的行为.  

### 函数的参数作为引用时
Python 唯一支持的参数传递模式是共享传参(Java 的引用类型是这样，基本类型按值传参),指函数的各个形式参数获得实参中各个引用的副本。也就是说，**函数内部的形参是实参的别名**。

#### 不要使用可变类型作为参数的默认值
可选参数可以有默认值, 然而，我们应该避免使用可变的对象作为参数的默认值. 在一个[简单类](https://github.com/fluentpython/example-code/blob/master/08-obj-ref/bus.py)的实验中, 不为HauntedBus 指定乘客的话，奇怪的事就发生了，这是因为`self.passengers` 变成了`passengers` 参数默认值的别名。出现这个问题的根源是，默认值在定义函数时计算（通常在加载模块时），因此默认值**变成了函数对象的属性**。所以我们通常使用`None` 作为接收可变值的参数的默认值,passengers 不是None，正确的实现会把passengers 的副本赋值给self.passengers.  
#### 防御可变参数
```Python
>>> basketball_team = ['Sue', 'Tina', 'Maya', 'Diana', 'Pat'] 
>>> bus = TwilightBus(basketball_team) 
>>> bus.drop('Tina') 
>>> bus.drop('Pat')
>>> basketball_team 
['Sue', 'Maya', 'Diana'] # 下车的人从篮球队消失了
```
校车为传给构造方法的列表创建了别名。正确的做法是，校车自己维护乘客列表:
```Python
def __init__(self, passengers=None):
    if passengers is None:
        self.passengers = []
    else:
        self.passengers = list(passengers)
```
### del和垃圾回收
del 语句删除名称，而不是对象,仅当删除的变量保存的是对象的最后一个引用，或者无法得到对象时才会被当作垃圾回收. 在CPython 中，垃圾回收使用的主要算法是引用计数。CPython 2.0 增加了分代垃圾回收算法，用于检测引用循环中涉及的对象组 —— 如果一组对象之间全是相互引用，即使再出色的引用方式也会导致组中的对象不可获取。

### 弱引用
弱引用不会增加对象的引用数量。引用的目标对象称为所指对象（referent）。因此我们说，弱引用不会妨碍所指对象被当作垃圾回收。控制台中的`_` 变量是一例, 调用跟踪对象也常导致意料之外的引用。
```Python
>>> import weakref
>>> a_set = {0, 1}
>>> wref = weakref.ref(a_set)
>>> a_set = {2, 3, 4}
>>> wref() 
{0, 1}
>>> wref() is None 
False
>>> wref() is None 
True
```
这个结果很有意思, 一开始 `wref` 弱引用指向的是 `a_set`, 后来在控制台中绑定到了 `_`, 故还在, 运行`wref() is None` 之后`_` 绑定给了 False, 故对象已经不在了.  

### Python对不可变类型施加的把戏
对元组 t 来说，t[:] 不创建副本，而是返回同一个对象的引用。此外，tuple(t) 获得的也是同一个元组的引用. str、bytes 和frozenset 实例也有这种行为。共享字符串字面量是一种优化措施，称为驻留（interning）。CPython 还会在小的整数上使用这个优化措施，防止重复创建“热门”数字.

## Chap 9: 复合 Python 风格的对象
Python 中依靠鸭子类型（duck typing）：我们只需按照预定行为实现对象所需的方法即可。
> 鸭子类型（英语：duck typing）在程序设计中是动态类型的一种风格。在这种风格中，一个对象有效的语义，不是由继承自特定的类或实现特定的接口，而是由 "**当前方法和属性的集合**" 决定。

### 对象表示形式
Python 提供了两种方式:
* `repr()` 以便于开发者理解的方式返回对象的字符串表示形式。
* `str()` 以便于用户理解的方式返回对象的字符串表示形式。

为了给对象提供其他的表示形式，还会用到另外两个特殊方法：`__bytes__` 和`__format__`。

### 再谈向量类
我们定义一些特殊方法后得到基本的[向量类](https://github.com/fluentpython/example-code/blob/master/09-pythonic-obj/vector2d_v0.py), 有几点需要强调:
* typecode 是类属性，在Vector2d 实例和字节序列之间转换时使用。
* 定义`__iter__` 方法，把Vector2d 实例变成可迭代的对象，这样才能拆包

我们已经定义了很多基本方法，但是显然少了一个操作：使用bytes() 函数生成的二进制表示形式重建Vector2d 实例。
### 备选构造方法
我们希望能从字节序列转化为 vector2d 实例, 于是想要[实现 frombytes 方法](https://github.com/fluentpython/example-code/blob/master/09-pythonic-obj/vector2d_v1.py)
* 类方法使用classmethod 装饰器修饰
* 不用传入self 参数；相反，要通过cls 传入类本身

### classmethod与staticmethod
* classmethod 定义操作类，而不是操作实例的方法
* staticmethod 普通的函数，只是碰巧在类的定义体中，而不是在模块层定义
```Python
>>> Demo.klassmeth() 
(<class '__main__.Demo'>,)
>>> Demo.klassmeth('spam')
(<class '__main__.Demo'>, 'spam')
>>> Demo.statmeth() 
()
>>> Demo.statmeth('spam')
('spam',)
```
classmethod 装饰器非常有用，我们可以看到其第一个参数是class名.同时, staticmethod 不是特别有用.  
### 格式化显示
// TODO: 格式化一直看不大懂

### 可散列的Vector2d
为了把Vector2d 实例变成可散列的，必须实现`__hash__` 方法（`__eq__` 方法已经实现过了), 此外还要让向量不可变.  
我们要把x 和y 分量设为[只读特性](https://github.com/fluentpython/example-code/blob/master/09-pythonic-obj/vector2d_v3.py)
* 使用两个前导下划线（尾部没有下划线，或者有一个下划线），把属性标记为私有的
* @property 装饰器把读值方法标记为特性, 读值方法与公开属性同名，都是x
  * 修饰方法，是方法可以像属性一样访问。
  * 与所定义的属性配合使用，这样可以防止属性被修改。

需要注意的是, 想要创建可散列的类型, 不一定要实现特定, 也不一定要保护实例属性, 只需要正确实现 `__hash__` 和`__eq__` 方法即可, 但是**实例的散列值绝不应该变化，因此我们借机提到了只读特性**.  

### Python的私有属性和“受保护的”属性
Python 不能像Java 那样使用 private 修饰符创建私有属性，但是 Python 中可以有名称改写(name mangling): ，如果以`__mood` 的形式（两个前导下划线，尾部没有或最多有一个下划线）命名实例属性，Python 会把属性名存入实例的`__dict__` 属性中，而且会在前面加上一个下划线和类名。只要编写`v1._Vector__x = 7` 这样的代码，就能轻松地为Vector2d 实例的私有分量直接赋值。  
有的人约定使用一个下划线前缀编写“受保护”的属性（如self._x）, 很多Python 程序员严格遵守的约定，他们不会在类外部访问这种属性.

### 使用`__slots__`类属性节省空间
通过`__slots__` 类[属性](https://github.com/fluentpython/example-code/blob/master/09-pythonic-obj/vector2d_v3_slots.py)，能节省大量内存，方法是让解释器在元组中存储实例属性，而不用字典。
```Python
class Vector2d:
    __slots__ = ('__x', '__y')
    typecode = 'd'
    ...
```
目的是告诉解释器：**这个类中的所有实例属性都在这**,这样就会避免使用消耗内存的`__dict__` 属性. 在类中定义`__slots__` 属性之后，实例不能再有`__slots__` 中所列名称之外的其他属性。如果类中定义了`__slots__` 属性，而且想把实例作为弱引用的目标，那么要把`__weakref__`添加到`__slots__` 中。

### 覆盖类属性
Python 有个很独特的特性：类属性可用于为实例属性提供默认值。Vector2d.typecode 属性的默认值是'd', 如果在转换之前把 Vector2d **实例的typecode 属性**设为'f'，那么使用4 字节单精度浮点数表示各个分量. 
```Python
>>> from vector2d_v3 import Vector2d
>>> v1 = Vector2d(1.1, 2.2)
>>> dumpd = bytes(v1) # b'd\x9a\x99\x99\x99\x99\x99\xf1?\x9a\x99\x99\x99\x99\x99\x01@'
>>> v1.typecode = 'f'
>>> dumpf = bytes(v1) # b'f\xcd\xcc\x8c?\xcd\xcc\x0c@'
```
有种修改方法更符合 Python 风格，类属性是公开的, 我们经常可以创建一个子类, 用于定制数据属性.
```Python
>>> from vector2d_v3 import Vector2d
>>> class ShortVector2d(Vector2d): 
...     typecode = 'f'
```

## Chap 10: 序列的修改、散列和切片
不要检查它是不是鸭子：检查它的叫声像不像鸭子、它的走路姿势像不像鸭子，等等。**具体检查什么取决于你想使用语言的哪些行为**。
### Vector类第1版：与Vector2d类兼容
[第一版的实现](https://github.com/fluentpython/example-code/blob/master/10-seq-hacking/vector_v1.py)有几点需要注意的:
* 迭代，我们使用self._components 构建一个迭代器
* 我们只需在Vector2d.frombytes 方法的基础上改动最后一行：直接把memoryview 传给构造方法，不用像前面那样使用* 拆包。

这里没有让Vector 继承Vector2d有两个原因:
* 两个构造方法不兼容，因此不建议继承(可以通过适当处理 `__init__`参数解决)
* 想要让这个类实现序列协议
### 协议和鸭子类型
在面向对象编程中，协议是非正式的接口，只在文档中定义，在代码中不定义。Python 的序列协议只需要`__len__` 和`__getitem__` 两个方法。例如我们在第一章中的[FrenchDeck 类](https://github.com/fluentpython/example-code/blob/master/01-data-model/frenchdeck.py),协议是非正式的，没有强制力，因此如果你**知道类的具体使用场景，通常只需要实现一个协议的部分**。例如，为了支持迭代，只需实现`__getitem__` 方法，没必要提供`__len__` 方法。

### Vector类第2版：可切片的序列
我先先简单实现了这两个方法:
```Python
class Vector:
    ...
    def __len__(self):
        return len(self._components)
    def __getitem__(self, index):
        return self._components[index]
```
但是现在支持切片, 但是有个问题, **内置的序列类型，切片得到的都是各自类型的新实例，而不是其他类型**。这里的Vector 切片得到的确是 array.
#### 切片原理
```Python
>>> class MySeq:
...     def __getitem__(self, index):
...             return index
...
>>> s=MySeq()
>>> s[1:4]
slice(1, 4, None)
>>> s[1:4:2]
slice(1, 4, 2)
```
Slice 是内置的类型, 发现它有start、stop 和step 数据属性，以及indices 方法. 
> `S.indices(len) -> (start, stop, stride)` 给定长度为len 的序列，计算S 表示的扩展切片的起始（start）和结尾（stop）索引，以及步幅（stride）。超出边界的索引会被截掉，这与常规切片的处理方式一样。

indices 方法开放了内置序列实现的棘手逻辑，用于优雅地处理缺失索引和负数索引，以及长度超过目标序列的切片。

#### 能处理切片的`__getitem__`方法
于是, 我们更新了部分[代码](https://github.com/fluentpython/example-code/blob/master/10-seq-hacking/vector_v2.py), 重点在于 `__getitem__` 的实现, 具体方法也很简单:
* 如果index 是一个 clice, 那么就会调用 class 的构造方法生成一个新的对象
* 如果是一个整数, 那么就返回第 N 个元素

### Vector类第3版：动态存取属性
在Vector2d 中，我们使用`@property` 装饰器把x 和y 标记为只读特性.属性查找失败后，解释器会调用`__getattr__` 方法, 于是我们可以实现[自定义的 `__getattr__` 方法](https://github.com/fluentpython/example-code/blob/master/10-seq-hacking/vector_v3.py):
```Python
shortcut_names = 'xyzt'

    def __getattr__(self, name):
        cls = type(self)  # <1>
        if len(name) == 1:  # <2>
            pos = cls.shortcut_names.find(name)  # <3>
            if 0 <= pos < len(self._components):  # <4>
                return self._components[pos]
        msg = '{.__name__!r} object has no attribute {!r}'  # <5>
        raise AttributeError(msg.format(cls, name))
```
这么实现之后, 有一个新的问题, 如果我们使用`v.x` 设定值之后, 向量分量没有变, 要改写Vector 类中设置属性的逻辑, 具体而言就是要实现`__setattr__` 方法.

### Vector类第4版：散列和快速等值测试
functools.reduce() 可以替换成sum()，下面说说它的原理。它的关键思想是，把一系列值归约成单个值。reduce() 函数的第一个参数是**接受两个参数的函数**，第二个参数是一个可迭代的对象。在此基础上我们可以实现新的 `__hash__`:
```Python
def __eq__(self, other): 
    return tuple(self) == tuple(other)
def __hash__(self):
    hashes = (hash(x) for x in self._components) 
    return functools.reduce(operator.xor, hashes, 0) 
```
使用的时候最好提供三个参数,`reduce(function, iterable,initializer)`.  
而判等的函数也可以有优化:
```Python
def __eq__(self, other):
    return len(self) == len(other) and all(a == b for a, b in zip(self, other))
```
### Vector类第5版：格式化
我们同样要在这里做格式化, 但是不使用极坐标，而使用球面坐标（也叫超球面坐标），因为Vector 类支持n 个维度，而超过四维后，球体变成了“超球体”. [具体实现](https://github.com/fluentpython/example-code/blob/master/10-seq-hacking/vector_v5.py)有很多dirty的细节问题不用深究.

## Chap 11: 接口 从协议到抽象基类
接口：从鸭子类型的代表特征动态协议，到使接口更明确、**能验证实现是否符合规定的抽象基类**. Python 社区不建议你自己编写抽象基类，因为很容易过度设计。 
### Python文化中的接口和协议
我们把协议定义为**非正式的接口**，是让Python 这种动态类型语言实现多态的方式, 具体的实现是，Python 语言没有interface 关键字，而且除了抽象基类，每个类都有接口：类实现或继承的公开属性, 包括特殊方法.  
**受保护的属性和私有属性不在接口中**, 接口的一个补充定义是: 对象公开方法的子集，让对象在系统中扮演特定的角色. 而协议是接口，但不是正式的（只由文档和约定定义），因此**协议不能像正式接口那样施加限制**.
### Python喜欢序列
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210610095419.png"/></div>

鉴于序列协议的重要性，如果没有`__iter__` 和`__contains__` 方法，Python 会**调用`__getitem__` 方法，设法让迭代和`in` 运算符可用**。这意味着Python 中的迭代是鸭子类型的一种极端形式：**为了迭代对象，解释器会尝试调用两个不同的方法**。  
### 使用猴子补丁在运行时实现协议
如果尝试打乱[FrenchDeck 实例](https://github.com/fluentpython/example-code/blob/master/01-data-model/frenchdeck.py)，会出现异常. 原因是，shuffle 函数要调换集合中元素的位置，而FrenchDeck 只实现了不可变的序列协议。可变的序列还必须提供`__setitem__` 方法, 下面是一个运行时修正:
```Python
>>> def set_card(deck, position, card):
...     deck._cards[position] = card
... 
>>> FrenchDeck.__setitem__ = set_card
```
这种技术叫做**猴子补丁：在运行时修改类或模块，而不改动源码**。

### _Alex Martelli_ 的水禽
鸭子类型意味着忽略对象的真正类型, 转而**关注对象有没有实现所需的方法/签名和语义**,这关注的是形态和举止的相似性, 属于**表型系统学**.然而，平行进化往往会导致不相关的种产生相似的特征, 生物（和其他学科）遇到的这个问题，迫切需要（甚至是催生出）表征学之外的分类方式解决，即**支序系统学**, 这种分类学主要根据从共同祖先那里继承的特征分类，而不是单独进化的特征.参照水禽的分类学演化，我建议在鸭子类型的基础上增加白鹅类型: **只要cls 是抽象基类， 即cls 的元类是`abc.ABCMeta`， 就可以使用`isinstance(obj, cls)`**。  
要抑制住创建抽象基类的冲动。滥用抽象基类会造成灾难性后果，**表明语言太注重表面形式**，这对以实用和务实著称的 Python 可不是好事。抽象基类是用于封装框架引入的一般性概念和抽象的, 我们基本上不需要自己编写新的抽象基类，只要**正确使用现有的抽象基类**，就能获得 _99.9_ % 的好处，而不用冒着设计不当导致的巨大风险。 
### 定义抽象基类的子类
我们先明确吧 [FrenchDeck2 声明为 MutableSequence的子类](https://github.com/fluentpython/example-code/blob/master/11-iface-abc/frenchdeck2.py)
* 支持洗牌只需实现`__setitem__` 方法
* 继承MutableSequence 的类必须实现`__delitem__` 方法和 `insert`

需要注意的是, 导入时Python 不会检查抽象方法的实现，在运行时实例化FrenchDeck2 类时才会真正检查。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210610144006.png"/></div>

### 标准库中的抽象基类
#### collections.abc模块中的抽象基类
Python 3.4 在collections.abc 模块中定义了16 个抽象基类，下面是简要的 UML 类图:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210610144216.png"/></div>

* Iterable、Container 和Sized: 各个集合都应该继承这三个抽象基类, 或者至少实现兼容的协议
* Sequence、Mapping 和Set: 这三个是主要的不可变集合类型，而且各自都有可变的子类。
* MappingView
* Callable 和Hashable: 这两个抽象基类与集合没有太大的关系, 主要作用是为内置函数`isinstance` 提供支持，以一种安全的方式判断对象能不能调用或散列
* Iterator: Iterable 的子类
  
#### 抽象基类的数字塔
[numbers 包](https://docs.python.org/3/library/numbers.html)定义的是“数字塔”（即各个抽象基类的层次结构是线性的）, 其中Number 是位于最顶端的超类，随后是Complex 子类，依次往下，最底端是Integral 类, 想检查一个数是不是整数，可以使用`isinstance(x, numbers.Integral)`.`decimal.Decimal` 没有注册为`numbers.Real` 的虚拟子类，如果你的程序需要Decimal 的精度，要防止与其他低精度数字类型混淆，尤其是浮点数。 

### 定义并使用一个抽象基类
// TODO:

## Chap 12: 继承的优缺点
推出继承的初衷是让新手顺利使用只有专家才能设计出来的框架。我们在这里主要注意两个细节:
* subclassing from built-in types 的缺点
* 多重继承和方法解析顺序

### 子类化内置类型很麻烦
在Python 2.2 之前，内置类型无法作为基类, 后面可以后依旧要注意:**内置类型（使用C 语言编写的那部分）不会调用用户定义的类覆盖的特殊方法**(例如，dict 的子类覆盖的`__getitem__()` 方法不会被内置类型的get() 方法调用)。
```Python
>>> class DoppelDict(dict):
...     def __setitem__(self, key, value):
...     super().__setitem__(key, [value] * 2) 
...
>>> dd = DoppelDict(one=1) 
>>> dd
{'one': 1}
>>> dd['two'] = 2 
>>> dd
{'one': 1, 'two': [2, 2]}
>>> dd.update(three=3) 
>>> dd
{'three': 3, 'one': 1, 'two': [2, 2]}
```
从上面的例子中可以看到, `__init__` 和 `update` 方法都没有使用我们 override 的`__setitem__` 方法.内置类型的方法调用的其他类的方法，如果被覆盖了，也不会被调用。  
如果使用 `collections.UserDict`, 那么就不存在这种问题. 
### 多重继承和方法解析顺序
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210611094641.png"/></div>

任何实现多重继承的语言都要处理潜在的命名冲突, 在C++ 中，程序员必须使用类名限定方法调用来避免这种歧义(在 D 中`using B::pong()`), Python 也可以这么做:
```Python
>>> from diamond import *
>>> d = D()
>>> d.pong() 
pong: <diamond.D object at 0x10066c278>
>>> C.pong(d) 
PONG: <diamond.D object at 0x10066c278>
>>> D.__mro__
(<class '__main__.D'>, <class '__main__.B'>, <class '__main__.C'>, <class '__main__.A'>, <class 'object'>)
```
Python 会按照特定的顺序遍历继承图,这个顺序叫方法解析顺序, 可以通过 `__mro__` 得到.  
若想把方法调用委托给超类，推荐的方式是使用内置的`super()` 函数, 有时可能需要绕
过方法解析顺序，直接调用某个超类的方法 —— 这样做有时更方便。这时候必须显式传入 `self` 参数. 
### 多重继承的真实应用
多重继承能发挥积极作用,GoF 一书中的适配器模式用的就是多重继承, 它在GUI 工具包Tkinter中被广泛使用. 
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210611100911.png"/></div>

### 处理多重继承
多重继承容易得出令人费解和脆弱的设计, 我们给出了一些建议:
1. 把接口继承和实现继承区分开
   1. 继承接口，创建子类型，实现“是什么”关系
   2. 继承实现，通过重用避免代码重复
2. 使用抽象基类显式表示接口: 如果类的作用是定义接口，应该明确把它定义为抽象基类
3. 通过混入重用代码: 实现重用，但不体现“is-a”关系，应该把那个类明确地定义为混入类
4. 在名称中明确指明混入
5. 抽象基类可以作为混入，反过来则不成立
6. 不要子类化多个具体类:具体类可以没有，**或最多只有一个具体超类**
7. 为用户提供聚合类:如果抽象基类或混入的组合对客户代码非常有用，那就提供一个类，使用易于理解的方式把它们结合起来
8. **优先使用对象组合，而不是类继承**

### Django通用视图中的混入
Django 中视图是可调用的对象, 参数是表示HTTP 请求的对象，返回值是一个表示HTTP 响应的对象。我们要关注的是这些响应对象。最初的通用视图是函数，不能扩展, Django 1.3 引入了基于类的视图，而且还通过基类、混入和拿来即用的具体类提供了一些通用视图类。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210611111744.png"/></div>

View 的具体子类应该实现处理方法，但它们为什么不在View 接口中呢？原因是：**子类只需实现它们想支持的处理方法**。Django 基于类的视图API 是多重继承更好的示例。尤其是，Django 的混入类易于理解：各个混入的目的明确，而且名称的后缀都是 _...Mixin_。

## Chap 13: 正确重载运算符
在 Python 中, 不但中缀运算符和一元运算符可以重载, 函数调用`()`/属性访问`.` 和元素访问`[]` 也可以重载.在这里我们主要关心前者.
### 运算符重载基础
Python 对运算符重载施加了一些限制:
* 不能重载内置类型的运算符
* 不能新建运算符，只能重载现有的
* 某些运算符不能重载——is、and、or 和not(但是为运算符`& | ~`可以)

### 一元运算符
支持一元运算符很简单，只需实现相应的特殊方法。这些特殊方法只有一个参数，`self`: **始终返回一个新对象**。也就是说，不能修改self，要创建并返回合适类型的新实例.  
我们在[这里](https://github.com/fluentpython/example-code/blob/master/13-op-overloading/vector_v6.py) 重新定义了 `__abs__`/`__neg__`/`__pos__`, 没有支持`__invert__`在尝试计算`~v`的时候就会抛出一个 TypeError.  
一般 `x == +x` 总是成立的, 但是有两个特例, 一是涉及精度的时候, 新实例和老的精度不同, 故不相等. 一是从使用角度出发, 在`Collection.Counter`中新实例会将负值和零相应的计数去除. 
### 重载向量加法运算符`+`
序列应该支持+ 运算符（用于拼接），以及* 运算符（用于重复复制）。我们[定义了`+`之后](https://github.com/fluentpython/example-code/blob/master/13-op-overloading/vector_v7.py),原则上可以使用向量去加任何元素可迭代的对象, 但是, 如果对调操作数, 混合类型的加法就会失败, 为此我们需要实现 “右向”（right）特殊方法`__radd__`, 实现后调用过程就如此:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210616152324.png"/></div>

### 重载标量乘法运算符`*`
涉及 _vector_ 操作数的积有很多种, 我们依旧可以实现最简单可用的 `__mul__`/`__rmul__`,但是提供不兼容的操作数时会出问题, 我们可以使用白鹅类型检查, 并且提供更好的错误提示:
```Python
def __mul__(self, scalar):
    if isinstance(scalar, numbers.Real): 
        return Vector(n * scalar for n in self)
    else: 
        return NotImplemented
def __rmul__(self, scalar):
    return self * scalar 
```
Python 3.5 引入了[matrix multiplication](https://github.com/fluentpython/example-code/blob/master/13-op-overloading/vector_py3_5.py).
### 众多比较运算符
比较运算符和之前的`+`/`*`有量个很明显的区别:
* 正向和反向调用使用的**不是同一系列方法**: 正向的`__gt__` 方法调用的是反向的`__lt__` 方法，并把参数对调。
* 对`==` 和`!=` 来说，如果反向调用失败，Python 会比较对象的ID，而不是抛出 TypeError

在[v8实现](https://github.com/fluentpython/example-code/blob/master/13-op-overloading/vector_v8.py)中, 我们使用了白鹅类型解决了vector 和 tuple 相等判断为 true 的问题. 而 ne 就不用实现, 定义了`__eq__` 方法，而且它不返回NotImplemented，`__ne__` 会对`__eq__` 返回的结果取反。

### 增量赋值运算符
对不可变类型来说，如果定义了`__add__` 方法的话，不用编写额外的代码，+= 就能使用. 如果实现了就地运算符方法, 它们会**就地修改左操作数，而不会创建新对象作为结果**, 因此不可变类型，如Vector 类，一定不能实现就地特殊方法。

# Part V: Control Flow
## Chap 14: 可迭代的对象、迭代器和生成器
扫描内存中放不下的数据集时，我们要找到一种**惰性获取**数据项的方式，即**按需一次获取一个数据项**, 这就是迭代器模式(Iterator pattern)。为了抽象出迭代器模式，Python 2.2 中加入了 `yield` 关键字, 用于构建生成器, 其作用和迭代器一样. 

### Sentence类第1版：单词序列
实现一个最基本的[序列](https://github.com/fluentpython/example-code/blob/master/14-it-generator/sentence.py), 只需要有 `__getitem__` 和 `__len__`.
#### 序列可以迭代的原因：iter函数
解释器需要迭代对象x 时，会自动调用iter(x):
1. 检查对象是否实现了`__iter__` 方法，如果实现了就调用它，获取一个迭代器。
2. 如果没有实现`__iter__` 方法，但是实现了__getitem__ 方法，Python 会创建一个迭代器，尝试按顺序（从索引0 开始）获取元素。
3. 如果尝试失败，Python 抛出TypeError 异常，通常会提示“C object is not iterable”

虽然前面定义的Sentence 类是可以迭代的，但却无法通过`issubclass(Sentence, abc.Iterable)` 测试。

### 可迭代的对象与迭代器的对比
* 可迭代的对象: 使用iter 内置函数可以获取迭代器的对象(实现了 `__iter__`或者`__getitem__`)  
    标准的迭代器接口有两个方法:
    * `__next__`
    * `__iter__`: 返回self
  <div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617100606.png"/></div>

* **迭代器** 迭代器是这样的对象：实现了无参数的`__next__` 方法，返回序列中的下一个元素；如果没有元素了，那么抛出 _StopIteration_ 异常, Python 中的迭代器实现了 `__iter__`, 因此是可迭代的

### Sentence类第2版：典型的迭代器
这里实现了一个不符合 Python 习惯做法的[迭代器版本](https://github.com/fluentpython/example-code/blob/master/14-it-generator/sentence_iter.py), 它通过一个辅助的类和下标索引, 实现 GoF 中描述的迭代器模式. 
#### 把Sentence变成迭代器：坏主意
构建可迭代的对象和迭代器时经常会出现错误，原因是混淆了二者。迭代器可以迭代，但是可迭代的对象不是迭代器。GoF 中说,迭代器模式可用来：
* 访问一个聚合对象的内容而无需暴露它的内部表示
* 支持对聚合对象的多种遍历
* 为遍历不同的聚合结构提供一个统一的接口(即支持多态迭代)

为了“支持多种遍历”，必须能从同一个可迭代的实例中获取多个独立的迭代器，而且各个迭代器要能维护自身的内部状态，因此这一模式正确的实现方式是，**每次调用 `iter(my_iterable)` 都新建一个独立的迭代器**。
### Sentence类第3版：生成器函数
实现相同功能，但却符合 Python [习惯的方式](https://github.com/fluentpython/example-code/blob/master/14-it-generator/sentence_gen.py)是，用生成器函数代替SentenceIterator 类。

#### 生成器函数的工作原理
只要Python 函数的定义体中有yield 关键字，该函数就是生成器函数。调用生成器函数时，会返回一个生成器对象。生成器函数会**创建一个生成器对象，包装生成器函数的定义体**。把生成器传给`next(...)`函数时, 生成器函数会想抢执行下一个 yield 语句.

### Sentence类第4版：惰性实现
设计 _Iterator_ 接口时需要考虑到惰性：next(my_iterator) 一次生成一个元素. 目前实现的几版Sentence 类都不具有惰性，因为`__init__` 方法急迫地构建好了文本中的单词列表，然后将其绑定到self.words 属性上。  
`re.finditer` 函数是`re.findall` 函数的惰性版本，我们基于此建立一个[惰性的版本](https://github.com/fluentpython/example-code/blob/master/14-it-generator/sentence_gen2.py).
* 不再需要words 列表
* finditer 函数构建一个迭代器, 产出MatchObject实例。
* match.group() 方法从MatchObject 实例中提取匹配正则表达式的具体文本。

### Sentence类第5版：生成器表达式
简单的生成器函数，可以替换成生成器表达式:生成器表达式可以理解为列表推导的惰性版本(不会迫切地构建列表，而是返回一个生成器，按需惰性生成元素)。  
把生成器表达式返回的值赋值时候, 只需调用相应的函数，虽然调用时会返回一个生成器，但是这里并不使用。**只有for 循环迭代时，函数的定义体才会真正执行**。于是我们有了生成器表达式的[版本](https://github.com/fluentpython/example-code/blob/master/14-it-generator/sentence_genexp.py):
* `__iter__` 方法中不是使用生成器函数(没有yield), 而是使用生成器表达式构建生成器，然后将其返回。

生成器表达式**是创建生成器的简洁句法**，这样无需先定义函数再调用。不过，**生成器函数灵活得多**，可以使用多个语句实现复杂的逻辑，也可以作为协程使用. 一般而言如果生成器表达式要分成多行写的情况下建议写生成器函数.

### 标准库中的生成器函数
标准库提供了很多生成器，从用于逐行迭代纯文本文件的对象，到出色的`os.walk` 函数.  
第一组是用于过滤的生成器函数：从输入的可迭代对象中产出元素的子集，而且不修改元
素本身。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617105408.png"/></div>

下一组是用于映射的生成器函数：在输入的单个可迭代对象（map 和starmap 函数处理多
个可迭代的对象）中的各个元素上做计算，然后返回结果。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617105518.png"/></div>

接下来这一组是用于合并的生成器函数，这些函数都从输入的多个可迭代对象中产出元
素:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617105602.png"/></div>

有些生成器函数会从一个元素中产出多个值，扩展输入的可迭代对象:
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617105644.png"/></div>

最后一组生成器函数用于产出输入的可迭代对象中的全部元素，不过会以某种方式重新排列。
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617105734.png"/></div>

### yield from
如果生成器函数需要产出另一个生成器生成的值，传统的解决方法是使用嵌套的for 循环:
```Python
>>> def chain(*iterables):
...     for it in iterables:
...         for i in it:
...             yield i
...
>>> s = 'ABC'
>>> t = tuple(range(3))
>>> list(chain(s, t))
['A', 'B', 'C', 0, 1, 2]
```
Since Python 3.3:
```Python
>>> def chain(*iterables):
...     for i in iterables:
...         yield from i
...
>>> list(chain(s, t))
['A', 'B', 'C', 0, 1, 2]
```

### 可迭代的归约函数
<div align=center><img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210617110143.png"/></div>

上图函数都接受一个可迭代的对象，然后返回单个结果。这些函数叫“归约”函数、“合拢”函数或“累加”函数。

### 深入分析iter函数
iter 函数还有一个鲜为人知的用法：传入两个参数，使用常规的函数或任何可调用的对象创建迭代器。第一个参数必须是可调用的对象，用于不断调用（没有参数），产出各个值；第二个值是哨符.
```Python
with open('mydata.txt') as fp:
    for line in iter(fp.readline, '\n'):
        process_line(line)
```