#include <benchmark/benchmark.h>

#include <vector>
#include <ctime>
#include <memory>

constexpr size_t FACADES_COUNT = 100;
const std::time_t RAND_SEED = std::time(nullptr);

enum class Operation
{
    GetValue,
    GetTypeInfo,
    Deinit
};

union Data
{
    const char* TypeName;
    int Value;
};

union StorageData
{
    char Value[sizeof(void*)];
    void* Pointer;
};

struct LocalStorage
{
    template<typename T>
    static T& AccessData(StorageData& aData)
    {
        void* p = static_cast<void*>(&aData.Value[0]);
        return *static_cast<T*>(p);
    }
};

struct DynamicStorage
{
    template<typename T>
    static T& AccessData(StorageData& aData)
    {
        return *static_cast<T>(aData.Pointer);
    }
};

template<typename T>
struct Manager {
    static constexpr bool IsLocalStorage = sizeof(T) <= sizeof(StorageData);

    using TStorage = std::conditional_t<
        IsLocalStorage,
        LocalStorage,
        DynamicStorage>;

    static void Perform(Operation aOperation, StorageData& aStorage, Data* outData)
    {
        switch (aOperation)
        {
            case Operation::GetValue:
            {
                auto& value = TStorage::template AccessData<T>(aStorage);
                outData->Value = static_cast<int>(value);
                break;
            }
            case Operation::GetTypeInfo:
            {
                outData->TypeName = typeid(T).name();
                break;
            }
            case Operation::Deinit:
            {
                auto& value = TStorage::template AccessData<T>(aStorage);
                value.~T();
                break;
            }
        }
    }

    static const char* Type()
    {
        return typeid(char).name();
    }
};

struct IntFacade {
    template<typename T>
    IntFacade(const T& aValue)
    {
        using TStorage = typename Manager<T>::TStorage;

        Perform = Manager<T>::Perform;

        T* p = &TStorage::template AccessData<T>(Storage);
        new (p)T(aValue);
    }

    ~IntFacade()
    {
        Data data{};
        Perform(Operation::Deinit, Storage, &data);
    }

    int Get()
    {
        Data data{};
        Perform(Operation::GetValue, Storage, &data);
        return data.Value;
    }

    const char* Type()
    {
        Data data{};
        Perform(Operation::GetTypeInfo, Storage, &data);
        return data.TypeName;
    }

private:

    void(*Perform)(Operation aOperation, StorageData& aStorage, Data* outData);
    StorageData Storage;
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
            auto val = facade.Type();
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
            auto val = facade->Type();
            benchmark::DoNotOptimize(val);
        }
    }
}

BENCHMARK(VirtualMethod);

BENCHMARK_MAIN();