#include <benchmark/benchmark.h>

#include <vector>
#include <ctime>
#include <memory>

constexpr size_t FACADES_COUNT = 100;
const std::time_t RAND_SEED = std::time(nullptr);

template<typename T>
struct Holder {
    T Value;

    static int GetInt(void* aData) {
        auto* holder = static_cast<Holder*>(aData);
        return holder->Value;
    }

    static const char* Type()
    {
        return typeid(char).name();
    }
};

struct IntFacade {
    template<typename T>
    IntFacade(const T& aValue) {
        static_assert(sizeof(Holder<T>) <= sizeof(data));

        auto* holder = static_cast<Holder<T>*>(AccessData());
        holder->Value = aValue;
        GetInt = Holder<T>::GetInt;
        GetType = Holder<T>::Type;
    }

    int Get() {
        return GetInt(AccessData());
    }

    const char* Type()
    {
        return GetType();
    }

private:
    char data[16];

    int (* GetInt)(void*);
    const char* (* GetType)();

    void* AccessData() {
        return &data[0];
    }
};

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
            auto* val = facade.Type();
            benchmark::DoNotOptimize(val);
        }
    }
}
// Register the function as a benchmark
BENCHMARK(MethodPointer);

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
            auto* val = facade->Type();
            benchmark::DoNotOptimize(val);
        }
    }
}

BENCHMARK(VirtualMethod);

BENCHMARK_MAIN();