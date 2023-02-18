#include <iostream>
#include <stdexcept>

class Data
{
public:
    explicit Data(int id, bool isThrow = false) : id(id)
    {
        if (isThrow)
        {
            throw std::runtime_error("");
        }
        std::cout << "Data" << id << ": constructed" << std::endl;
    }

    ~Data()
    {
        std::cout << id << ": deleted" << std::endl;
    }

private:
    int id;
};

class DataHolder
{
public:
    DataHolder() : data1(1), data2(2, true)
    {
        std::cout << "RawPointerHolder: constructed" << std::endl;
    }

    ~DataHolder()
    {
        std::cout << "RawPointerHolder: deleted" << std::endl;
    }

private:
    Data data1;
    Data data2;
};

int main()
{
    try
    {
        DataHolder dataHolder;
    }
    catch(...)
    {
        std::cout << "Catch exception" << std::endl;
    }

    return 0;
}