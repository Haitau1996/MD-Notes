template<class T>
void swap(T& a, T& b)
noexcept(noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b))))
{
    static_assert(noexcept(T(std::move(a)))
        && noexcept(a.operator=(std::move(b))));
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}