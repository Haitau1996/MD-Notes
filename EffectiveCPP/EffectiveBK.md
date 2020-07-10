### Item 37 

本Item讨论的内容局限于继承一个带有默认参数值的virtual函数,因为重新定义一个non-virtual是错误的.**virtual函数是动态绑定,而默认参数值是静态绑定** 的,
```C++
class Shape {
public:
    enum ShapeColor { Red, Green, Blue };
    // all shapes must offer a function to draw themselves
    virtual void draw(ShapeColor color = Red) const = 0;
    ...
};
class Rectangle: public Shape {
public:
    // notice the different default parameter value — bad!
    virtual void draw(ShapeColor color = Green) const;
    ...
};
class Circle: public Shape {
public:
    virtual void draw(ShapeColor color) const;
    ...
};
Shape *ps; // static type = Shape*
Shape *pc = new Circle; // static type = Shape*
Shape *pr = new Rectangle; // static type = Shape*
```
这样带来一个灾难性的结果,调用`pr->draw()`,使用的是Rectangle的draw函数,而默认的参数确是Shape的,**C++做出这样的取舍是基于效率上的考虑**,如果提供相同的缺省值给bass和derived的用户,造成的结果是**代码重复又带着依赖性**,聪明的做法是考虑替代设计,如使用NVI(non-virtual-interface),让non-virtual提供default,而private virtual函数负责真正的工作:
```C++
class Shape {
public:
    enum ShapeColor { Red, Green, Blue };
    void draw(ShapeColor color = Red) const // now non-virtual
    {
    doDraw(color); // calls a virtual
    }
    ...
private:
    virtual void doDraw(ShapeColor color) const = 0; // the actual work is done in this func
}; 
class Rectangle: public Shape {
public:
    ...
private:
    virtual void doDraw(ShapeColor color) const; // note lack of a default param val.
    ... 
};
```

### Item 38 通过复合塑模出has-a 或者 is-implemented-in-terms-of
