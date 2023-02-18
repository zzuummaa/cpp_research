cmake_minimum_required(VERSION 3.21)
project(cpp_research)

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark REQUIRED)

add_executable(constructor_exception constructor_exception.cpp)

add_executable(mycopy mycopy.cpp)

add_executable(partial_specialization partial_specialization.cpp)

add_executable(polymorphic_containers polymorphic_containers.cpp)

add_executable(template_param_type template_param_type.cpp)

add_executable(virtual_methods virtual_methods.cpp)

add_executable(virtual_vs_method_pointer virtual_vs_method_pointer.cpp)
target_link_libraries(virtual_vs_method_pointer benchmark::benchmark)
set_target_properties(virtual_vs_method_pointer PROPERTIES COMPILE_FLAGS "-save-temps")