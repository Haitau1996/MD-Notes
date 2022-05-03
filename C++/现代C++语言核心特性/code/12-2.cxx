class X2
{
public:
    X2() { CommonInit(0, 0.); }
    X2(int a) { CommonInit(a, 0.); }
    X2(double b) { CommonInit(0, b); }
    X2(int a, double b) { CommonInit(a, b); }
private:
    void CommonInit(int a, double b)
    {
        a_ = a;
        b_ = b;
        c_ = "hello world";
    }
    int a_;
    double b_;
    std::string c_;
};