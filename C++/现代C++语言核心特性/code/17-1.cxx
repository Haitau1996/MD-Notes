class T {
    td::vector<int> data_;
public:
    std::vector<int>& items() { return data_; }
    // …
};

T foo() 
{
        T t;
        return t;
}
for (auto& x : foo().items()) {} // 未定义行为