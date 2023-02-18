#include <iostream>
#include <any>

struct Consumer
{
    void operator()(int) {}
};

int main()
{
    std::any a(1);

    std::cout << std::any_cast<int>(a) << std::endl;

    return 0;
}