#include <iostream>
#include <string>
#include <vector>

union U
{
    U() : x3() {}
    ~U() { x3.~basic_string(); }
    int x1;
    float x2;
    std::string x3;
    std::vector<int> x4;
};

int main()
{
    U u;
    u.x3 = "hello world";
    std::cout << u.x3;
    u.x4.push_back(58); // 写了 x3 的构造函数， 但是是想用 x4
}