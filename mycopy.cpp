#include <iostream>
#include <vector>
#include <cassert>
#include <iterator>
#include <functional>

using namespace std;

template<typename In,
		 typename Out,
		 enable_if_t<!is_fundamental<In>::value, bool> = true,
		 enable_if_t<!is_fundamental<Out>::value, bool> = true,
		 enable_if_t<!is_same_v<In, Out>, bool> = true >
Out mycopy(In begin, In end, Out dest) {
	Out res = dest;
	cout << "!fundamental && !pointer" << endl;
	for (auto it = begin; it != end; ++it) {
		*dest = *it;
		dest++;
	}
	return res;
}

template<typename In,
		 typename Out,
		 enable_if_t<is_pointer<In>::value, bool> = true,
		 enable_if_t<is_pointer<Out>::value, bool> = true,
		 enable_if_t<is_same_v<In, Out>, bool> = true>
Out mycopy(In begin, In end, Out dest) {
	cout << "fundamental || pointer" << endl;
	memmove(dest, begin, distance(begin, end) * sizeof(typename iterator_traits<In>::value_type));
	return dest;
}

template<typename T1, typename T2>
bool comp(const std::vector<T1>& aLhs, const std::vector<T2>& aRhs)
{
    if (aLhs.size() != aRhs.size())
    {
        return false;
    }
    for (size_t i = 0; i < aLhs.size(); ++i)
    {
        if (!(aLhs[i] == aRhs[i]))
        {
            return false;
        }
    }
    return true;
}

class AddressContainer
{
    uintptr_t mAddress;
public:

    AddressContainer() : mAddress(reinterpret_cast<uintptr_t>(this))
    {
        assert(isSelfAddress());
    }

    AddressContainer(const AddressContainer& aOther)
        : mAddress(aOther.isSelfAddress() ? reinterpret_cast<uintptr_t>(this) : aOther.mAddress)
    {
    }

    AddressContainer& operator=(const AddressContainer& aOther)
    {
        mAddress = aOther.isSelfAddress() ? reinterpret_cast<uintptr_t>(this) : aOther.mAddress;
        return *this;
    }

    bool operator==(const AddressContainer& aOther) const {
        return isSelfAddress() == aOther.isSelfAddress();
    }

    [[nodiscard]] bool isSelfAddress() const
    {
        return reinterpret_cast<AddressContainer*>(mAddress) == this;
    }
};

int main()
{
    vector<int> a = {1, 3, 5};
    vector<int> b;

    b.resize(a.size());

    // Одинаковые итераторы
    mycopy(a.begin(), a.end(), b.begin());
    assert(comp(a, b));

    b.clear();
    b.resize(a.size());

    // Разные итераторы
    mycopy(a.begin(), a.end(), std::back_inserter(b));
    assert(comp(a, b));

    b.clear();
    b.resize(a.size());

    // Указатели на одинаковые типы
    mycopy(a.data(), a.data() + a.size(), b.data());
    assert(comp(a, b));

    b.clear();
    b.resize(a.size());

    // Указатели с разными квалификаторами на одинаковые типы (должен вызваться memmove)
    const auto& aCRef = a;
    mycopy(aCRef.data(), aCRef.data() + aCRef.size(), b.data());
    assert(comp(aCRef, b));

    vector<char> c;
    c.resize(a.size());

    // Указатели на разные типы
    mycopy(a.data(), a.data() + a.size(), c.data());
    assert(comp(a, c));

    vector<AddressContainer> d = {{}, {}, {}};
    vector<AddressContainer> e;
    e.resize(d.size());

    // Указатели на нефундаментальный тип
    mycopy(d.data(), d.data() + d.size(), e.data());
    assert(comp(d, e));
}
