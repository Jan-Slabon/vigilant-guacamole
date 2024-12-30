cmake_minimum_required(VERSION 3.10)
project(GC VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(GarbageCollector src/test/allocator_test.cpp)

add_library(Allocator src/allocator/gc_allocator.cpp src/memory/memory.cpp src/sys_deps/sys_mem_alloc.cpp)

include_directories(src)

target_link_libraries(GarbageCollector PUBLIC Allocator)
target_link_libraries(GarbageCollector PRIVATE Threads::Threads)
target_include_directories(GarbageCollector PUBLIC
                           "src"
                           )