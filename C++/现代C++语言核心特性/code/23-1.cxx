void f(int)
{
    std::cout << "int" << std::endl;
}

void f(char *)
{
    std::cout << "char *" << std::endl;
}

f(NULL);// ERROR 存在二义性， 不知道调用哪个
f(reinterpret_cast<char *>(NULL)); // 可以调用 f(void *)