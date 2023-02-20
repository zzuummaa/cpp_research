#pragma once

#include <typeinfo>

struct IDIntFacade {
    virtual ~IDIntFacade() = default;
    virtual int Get() = 0;
    virtual const char* Type() = 0;
};

template<typename T>
struct DIntFacade : public IDIntFacade {
    DIntFacade(const T& aValue) : mValue(aValue) {}

    int Get() override {
        return mValue;
    }

    const char* Type() override
    {
        return typeid(T).name();
    }
private:
    T mValue;
};
