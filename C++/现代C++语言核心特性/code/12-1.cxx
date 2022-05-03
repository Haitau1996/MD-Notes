class X1
{
public:
    X1() { CommonInit(0, 0.); }
    X1(int a) { CommonInit(a, 0.); }
    X1(double b) { CommonInit(0, b); }
    X1(int a, double b) { CommonInit(a, b); }
private:
    void CommonInit(int a, double b)
    {
        a_ = a;
        b_ = b;
    }
    int a_;
    double b_;
};