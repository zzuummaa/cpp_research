#pragma once

#include "test_data.h"

#include <type_traits>
#include <typeinfo>
#include <utility>

enum class Operation
{
    GetValue,
    GetType,
    Move,
    Deinit
};

union StorageData
{
    char Value[sizeof(void*)];
    void* Pointer;
};

struct LocalStorage
{
    template<typename T>
    static void Init(StorageData& aData, T aValue)
    {
        auto* p = &Access<T>(aData);
        new (p)T(aValue);
    }

    template<typename T>
    static T& Access(StorageData& aData)
    {
        void* p = static_cast<void*>(&aData.Value[0]);
        return *static_cast<T*>(p);
    }

    template<typename T>
    static void Deinit(StorageData& aData)
    {
        auto& value = Access<T>(aData);
        value.~T();
    }
};

struct DynamicStorage
{
    template<typename T>
    static void Init(StorageData& aData, T aValue)
    {
        aData.Pointer = new T(aValue);
    }

    template<typename T>
    static T& Access(StorageData& aData)
    {
        return *static_cast<T>(aData.Pointer);
    }

    template<typename T>
    static void Deinit(StorageData& aData)
    {
        delete static_cast<T*>(aData.Pointer);
    }
};

template<typename T>
struct Manager {
    static constexpr bool IsLocalStorage = sizeof(T) <= sizeof(StorageData);

    using TStorage = std::conditional_t<
        IsLocalStorage,
        LocalStorage,
        DynamicStorage>;

    static void Perform(Operation aOperation, StorageData& aStorage, StorageData& outStorage)
    {
        switch (aOperation)
        {
            case Operation::GetValue:
            {
                auto& value = TStorage::template Access<T>(aStorage);
                auto& outValue = LocalStorage::Access<int>(outStorage);
                outValue = static_cast<int>(value);
                break;
            }
            case Operation::GetType:
            {
                outStorage.Pointer = const_cast<std::type_info*>(&typeid(T));
                break;
            }
            case Operation::Move:
            {
                auto& value = TStorage::template Access<T>(aStorage);
                auto& outValue = TStorage::template Access<T>(outStorage);
                new (&outValue)T(std::move(value));
                break;
            }
            case Operation::Deinit:
            {
                TStorage::template Deinit<T>(aStorage);
                break;
            }
        }
    }
};

struct IntFacade {
    template<typename T>
    explicit IntFacade(const T& aValue) : Storage()
    {
        using TStorage = typename Manager<T>::TStorage;

        Perform = Manager<T>::Perform;
        TStorage::template Init<T>(Storage, aValue);
    }

    IntFacade(const IntFacade&) = delete;

    IntFacade(IntFacade&& aOther) : Perform(nullptr), Storage()
    {
        aOther.Perform(Operation::Move, aOther.Storage, Storage);
        std::swap(Perform, aOther.Perform);
    }

    ~IntFacade()
    {
        if (Perform)
        {
            StorageData data{};
            Perform(Operation::Deinit, Storage, data);
        }
    }

    int Get()
    {
        StorageData data{};
        Perform(Operation::GetValue, Storage, data);
        return LocalStorage::Access<int>(data);
    }

    const std::type_info& Type()
    {
        StorageData data{};
        Perform(Operation::GetType, Storage, data);
        return *static_cast<std::type_info*>(data.Pointer);
    }

private:

    void(*Perform)(Operation aOperation, StorageData& aStorage, StorageData& outStorage);
    StorageData Storage;
};

template<>
struct MakeTestData<IntFacade>
{
    auto operator()()
    {
        std::vector<IntFacade> facades;

        std::srand(RAND_SEED);
        while (facades.size() < FACADES_COUNT) {
            int value = std::rand();
            int type = std::rand() % 3;

            if (type == 0) facades.emplace_back(static_cast<char>(value));
            if (type == 1) facades.emplace_back(static_cast<int>(value));
            if (type == 2) facades.emplace_back(static_cast<double>(value));
        }

        return facades;
    }
};
