template <class T>
T copy(const T &o) noexcept(noexcept(T(o))) { 
    …
}