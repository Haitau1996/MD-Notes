# 深度探索 C++ 对象模型

## Chap 1: 关于对象
C 语言中, 数据和处理数据的操作(函数)是分开声明的, 语言没有支持数据和函数之间的关联性. C++ 加上封装之后, 成本增加了多少? <br>
实际上, C++ 在布局以及存取之间的额外负担主要是由 _virtual_ 引起的:
* 虚函数机制: 用以支持一个有效率的执行期绑定
* 虚基类: 用以实现多次出现在继承体系中的base class, 有一个单一而被共享的实例.

### C++ 对象模式
C++ 中有两种class data members: _static_ 和 _nonstatic_ . 同时有三种 class member functions: _static_, _nonstatic_ 和 _virtual_. 
#### 简单对象模型
```C++
class Point
{
public:
    Point( float xval );
    virtual ~Point();
    float x() const;
    static int PointCount();
protected:
    virtual ostream&
        print( ostream &os ) const;
    float _x;
    static int _point_count;
};
```
<img src="https://gitee.com/Haitau1996/picture-hosting/raw/master/img/20210331123916.jpg" div align=center/><br>
上面是一个简单模型, 以空间和执行效率为代价降低编译器复杂度,一个对象是一系列slot, 每个指向一个成员, 因此一个对象的大小就是指针大小乘以member个数, 这个模型没有用到实际的编译器中, 但是
