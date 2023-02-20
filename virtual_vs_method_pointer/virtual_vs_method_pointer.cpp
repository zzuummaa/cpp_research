#include "template_facade.h"
#include "virtual_facade.h"

#include <benchmark/benchmark.h>

static void MethodPointerGetValue(benchmark::State& state) {
    auto facades = MakeTestData<IntFacade>()();

    for (auto _: state) {
        for (auto& facade : facades)
        {
            auto val = facade.Get();
            benchmark::DoNotOptimize(val);
        }
    }
}

static void MethodPointerGetType(benchmark::State& state) {
    auto facades = MakeTestData<IntFacade>()();

    for (auto _: state) {
        for (auto& facade : facades)
        {
            auto val = &facade.Type();
            benchmark::DoNotOptimize(val);
        }
    }
}

static void VirtualMethodGetValue(benchmark::State& state) {
    auto facades = MakeTestData<IDIntFacade>()();

    for (auto _: state) {
        for (auto& facade : facades)
        {
            auto val = facade->Get();
            benchmark::DoNotOptimize(val);
        }
    }
}

static void VirtualMethodGetType(benchmark::State& state) {
    auto facades = MakeTestData<IDIntFacade>()();

    for (auto _: state) {
        for (auto& facade : facades)
        {
            auto val = &facade->Type();
            benchmark::DoNotOptimize(val);
        }
    }
}

BENCHMARK(MethodPointerGetValue);
BENCHMARK(VirtualMethodGetValue);
BENCHMARK(MethodPointerGetType);
BENCHMARK(VirtualMethodGetType);

BENCHMARK_MAIN();