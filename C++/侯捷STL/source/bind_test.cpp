#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;

double my_divide(double x, double y){
    return x /y;
}
struct MyPair{
    double a, b;
    double multiply(){
        return a * b;
    }
};
namespace bt{
void bind_test(){
    auto fn_five = bind(my_divide, 10, 5);// 绑定函数和参数
    cout << fn_five() << endl;
    auto fn_first = bind(my_divide,_1,2);//绑定函数和第一个参数
    cout << fn_first(10) << endl;
    auto fn_second = bind(my_divide, 10, _2);
    cout << fn_second(2) << endl;
}
}
int main(){
    bt::bind_test();
}