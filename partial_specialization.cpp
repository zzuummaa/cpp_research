// partial_specialization_of_class_templates.cpp
#include <cstdio>

template<class T>
struct PTS {
    enum {
        IsPointer = 0,
        IsPointerToDataMember = 0
    };
};

template<class T>
struct PTS<T*> {
    enum {
        IsPointer = 1,
        IsPointerToDataMember = 0
    };
};

template<class T, class U>
struct PTS<T U::*> {
    enum {
        IsPointer = 0,
        IsPointerToDataMember = 1
    };
};

struct S {
};

struct K {
    int a;
};

int main() {
    printf_s("PTS<S>::IsPointer == %d \nPTS<S>::IsPointerToDataMember == %d\n",
             PTS<S>::IsPointer, PTS<S>:: IsPointerToDataMember);
    printf_s("PTS<S*>::IsPointer == %d \nPTS<S*>::IsPointerToDataMember == %d\n"
            , PTS<S*>::IsPointer, PTS<S*>:: IsPointerToDataMember);
    printf_s("PTS<int S::*>::IsPointer == %d \nPTS"
             "<int S::*>::IsPointerToDataMember == %d\n",
             PTS<int S::*>::IsPointer, PTS<int S::*>::
             IsPointerToDataMember);
    printf_s("PTS<int S::*>::IsPointer == %d \nPTS"
             "<int S::*>::IsPointerToDataMember == %d\n",
             PTS<int S::*>::IsPointer, PTS<int K::*>::
             IsPointerToDataMember);
}