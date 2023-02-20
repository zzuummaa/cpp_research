#include "template_facade.h"
#include "virtual_facade.h"

#include <benchmark/benchmark.h>

#include <vector>
#include <ctime>
#include <memory>

constexpr size_t FACADES_COUNT = 100;
const std::time_t RAND_SEED = std::time(nullptr);



static void MethodPointer(benchmark::State& state) {
    std::vector<IntFacade> facades;

    std::srand(RAND_SEED);
    while (facades.size() < FACADES_COUNT) {
        int value = std::rand();
        int type = std::rand() % 3;

        if (type == 0) facades.emplace_back(static_cast<char>(value));
        if (type == 1) facades.emplace_back(static_cast<int>(value));
        if (type == 2) facades.emplace_back(static_cast<double>(value));
    }

    for (auto _: state) {
        for (auto& facade : facades)
        {
            auto val = facade.Type();
            benchmark::DoNotOptimize(val);
        }
    }
}
// Register the function as a benchmark
BENCHMARK(MethodPointer);

static void VirtualMethod(benchmark::State& state) {
    DIntFacade<int> facadeInt(2);
    DIntFacade<char> facadeChar(2);

    std::vector<std::unique_ptr<IDIntFacade>> facades;

    std::srand(RAND_SEED);
    while (facades.size() < FACADES_COUNT) {
        int value = std::rand();
        int type = std::rand() % 3;

        if (type == 0) facades.push_back(std::make_unique<DIntFacade<char>>(value));
        if (type == 1) facades.push_back(std::make_unique<DIntFacade<int>>(value));
        if (type == 2) facades.push_back(std::make_unique<DIntFacade<double>>(value));
    }

    for (auto _: state) {
        for (auto& facade : facades)
        {
            auto val = facade->Type();
            benchmark::DoNotOptimize(val);
        }
    }
}

BENCHMARK(VirtualMethod);

BENCHMARK_MAIN();