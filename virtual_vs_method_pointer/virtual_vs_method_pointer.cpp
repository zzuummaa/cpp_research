#include "template_facade.h"
#include "virtual_facade.h"

#include <benchmark/benchmark.h>

static void MethodPointerInit(benchmark::State& state) {
    for (auto _: state) {
        auto val = IntFacade(static_cast<char>(2));
        benchmark::DoNotOptimize(val);
    }
}

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
            const auto& val = &facade.Type();
            const char* name = val->name();
            benchmark::DoNotOptimize(name);
        }
    }
}

static void VirtualMethodInit(benchmark::State& state) {
    for (auto _: state) {
        std::unique_ptr<IDIntFacade> val = std::make_unique<DIntFacade<char>>(2);
        benchmark::DoNotOptimize(val);
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
            const auto& val = facade->Type();
            const char* name = val.name();
            benchmark::DoNotOptimize(name);
        }
    }
}

BENCHMARK(MethodPointerInit);
BENCHMARK(VirtualMethodInit);
BENCHMARK(MethodPointerGetValue);
BENCHMARK(VirtualMethodGetValue);
BENCHMARK(MethodPointerGetType);
BENCHMARK(VirtualMethodGetType);

BENCHMARK_MAIN();