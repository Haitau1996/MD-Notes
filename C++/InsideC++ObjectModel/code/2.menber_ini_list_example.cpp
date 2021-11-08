//case 1:
class X{
private:
    int& m_yy;
public:
    X(int& val):m_yy(val){

    }
};
// case 3
class Base{
private:
    int val1;
    int val2;
public:
    Base(int arg1, int arg2){

    }
};
class Derived : public Base{
public:
    Derived(int val1, int val2, int val3):val3(val3),Base(val1,val2){}
private:
    int val3;
};