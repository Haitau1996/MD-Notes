# Hou Jie C++ 11 新特性

## 演进 环境 与资源

重要的帮助网页:
- cplusplus.com
- cppreference.com
- gcc.gnu.org

全文检索工具 : wingrep,source insight

### Variadic Templates

可变参数函数的实例,帮助做recursive,n个写成 1 + 其他
```C++
void print(){}   // 一定要包含,不然args为空时候无法通过,相当于递归的base case
template <typename T, typename... Types>
void print(const T& fitstArg, const Types&... args){
    std::cout << fitstArg << std::endl;
    print(args...);
}
print(7.5,"hello",bitset<16>(377),43);
```
...为所谓的parameter pack,可以是模板参数包,函数参数包<font color=red> sizeof...(args) 可以得到参数的个数 </font>.
![variadic template](figure/v2-1.png)<br>
函数可以有多个候选,优先调用特化的版本,没有的时候才调用泛化的版本.//todo: vedio 2:26min