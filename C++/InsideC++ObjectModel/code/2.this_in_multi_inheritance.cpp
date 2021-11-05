class A{
private:
    int a;
public:
    A(){
        printf("this ptr of A::A() is %p! \n ", this);
    }
    void funcA(){
        printf("A::FuncA: this  = %p,\n", this);
    }
}
class B{
private:
    int b;
public:
    B(){
        printf("this ptr of B::B() is %p!\n ", this);
    }
    void funcB(){
        printf("B::FuncB: this = %p, \n", this);
    }
};
class C:public A, public B{
private:
    int c;
public:
    C(){
        printf("this ptr of C::C() is %p !\n", this);
    }
    void funcC(){
        printf("C :: FuncC : this = %p, \n ", this );
    }
};