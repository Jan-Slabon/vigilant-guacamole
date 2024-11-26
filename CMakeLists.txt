cmake_minimum_required(VERSION 3.10)
project(GC VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_executable(GarbageCollector src/test/allocator_test.cpp)

add_library(Allocator src/allocator/gc_allocator.cpp)

target_link_libraries(GarbageCollector PUBLIC Allocator)
target_include_directories(GarbageCollector PUBLIC
                           "src"
                           )