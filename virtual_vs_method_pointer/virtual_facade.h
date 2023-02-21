#pragma once

#include "test_data.h"

#include <typeinfo>
#include <memory>

struct IDIntFacade {
    virtual ~IDIntFacade() = default;
    virtual int Get() = 0;
    virtual const std::type_info& Type() = 0;
};

template<typename T>
struct DIntFacade : public IDIntFacade {
    explicit DIntFacade(const T& aValue) : mValue(aValue) {}

    int Get() override
    {
        return mValue;
    }

    const std::type_info& Type() override
    {
        return typeid(T);
    }
private:
    T mValue;
};

template<>
struct MakeTestData<IDIntFacade>
{
    auto operator()()
    {
        std::vector<std::unique_ptr<IDIntFacade>> facades;

        std::srand(RAND_SEED);
        while (facades.size() < FACADES_COUNT) {
            int value = std::rand();
            int type = std::rand() % 3;

            if (type == 0) facades.push_back(std::make_unique<DIntFacade<char>>(value));
            if (type == 1) facades.push_back(std::make_unique<DIntFacade<int>>(value));
            if (type == 2) facades.push_back(std::make_unique<DIntFacade<double>>(value));
        }

        return facades;
    }
};
