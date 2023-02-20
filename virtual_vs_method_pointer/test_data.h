#pragma once

#include <ctime>
#include <vector>

constexpr size_t FACADES_COUNT = 100;
const std::time_t RAND_SEED = std::time(nullptr);

template<typename T>
struct MakeTestData
{
    static_assert(sizeof(MakeTestData<T>) == -1, "Unimplemented");
};
