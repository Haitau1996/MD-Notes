# 数据结构与算法分析 Java 语言描述

## 表/栈和队列

### 抽象数据类型

抽象数据类型是带有一组操作的一些对象的集合. 

### 表ADT

#### 表的简单数组实现

```Java
int [ ] arr = new int[ 10 ];
...
// 需要扩容的时候
int [ ] newArr = new int[ arr.length * 2 ];
for( int i = 0; i < arr.length; i++ )
newArr[ i ] = arr[ i ];
arr = newArr;
```

插入和删除的有潜在的昂贵开销, 而且需要考虑插入和删除的位置. 