rsp - регистр, указывающий на вершину текущего стек фрейма
rbp - регистр, указывающий на базу текущего стек фрейма
eax - регистр, в который сохраняется возвращаемое значение функции

$4 - число "4"

8(%rbp) - взять смещение на 8 байтов относительно значения "rbp"
leaq	(%rsi,%r12,8), %r14 - присваивает значение из "r14" в "%rsi + %r12 * 8"
movq    %rsi, %r15 - присваевает значение из "rsi" в "r15"

(%rip) - глобальная адресация
_ZTV10DIntFacadeIiE+16(%rip) - адрес _ZTV10DIntFacadeIiE смещенный на 16 байт

@startuml
object "IntFacade<T>::VTable" as VTable
object "IntFacade<T>" as IntFacade
object "unique_ptr<IIntFacade>" as unique_ptr

VTable : int (*Get)()
VTable : type_info (*Type)()

IntFacade : VTable*
IntFacade : T mValue
IntFacade o-- VTable

unique_ptr : mPtr
unique_ptr *-- IntFacade
@enduml


@startuml
object IntFacade

IntFacade : void (*mManager)(Operation, AnyData&, AnyData&)
IntFacade : AnyData mStore
@enduml