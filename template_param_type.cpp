#include <iostream>
#include <memory>

template<typename T>
void func(T&& a)
{
    std::cout << typeid(a).name() << std::endl;
}

template<typename T>
void proxy(T&& a)
{
    func(std::forward<T>(a));
}

struct A
{
    std::unique_ptr<int> val = std::make_unique<int>(2);
};

int main()
{
    A a;
    proxy([](){});
}