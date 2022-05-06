class Base {
public:
    virtual void some_func() {}
    virtual void foo(int x) {}
    virtual void bar() const {}
    void baz() {}
};

class Derived : public Base {
public:
    virtual void sone_func() {} // 拼写错误， 没有重写
    virtual void foo(int &x) {} // 形参类型不同， 无法重写
    virtual void bar() {}       // const 属性不同， 无法重写
    virtual void baz() {}       // Base 中不是虚函数， 无法重写
};