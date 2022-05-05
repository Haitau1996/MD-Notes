enum class Color {
    Red,
    Green,
    Blue
};
int main()
{
    Color c{ 5 };          // 编译成功
    Color c1 = 5;          // 编译失败
    Color c2 = { 5 };      // 编译失败
    Color c3(5);           // 编译失败
}

enum class Color1 : char {};
enum Color2 : short {};

int main()
{
    Color1 c{ 7 };
    Color2 c1{ 11 };
    Color2 c2 = Color2{ 5 };
}