class Base {
public:
    virtual void foo() noexcept {}
};
class Derived : public Base {
public:
    void foo() override {};//ERROR
};

class Base {
public:
    virtual void foo() {}
};
class Derived : public Base {
public:
    void foo() noexcept override {};// OK
};