struct X {
    int f() const noexcept
    {
        return 58;
    }
    void g() noexcept {}
};

int foo() noexcept
{
    return 42;
}