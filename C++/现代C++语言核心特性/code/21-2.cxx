template <class T> 
T copy(const T &o) noexcept(std::is_fundamental<T>::value) {
    …
}