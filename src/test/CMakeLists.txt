cmake_minimum_required(VERSION 3.13)
project(GC VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(GC_Allocator ../allocator/gc_allocator.cpp ../memory/memory.cpp ../sys_deps/sys_mem_alloc.cpp)
include_directories(..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gc_test)


file( GLOB APP_SOURCES *.cpp )
foreach( testsourcefile ${APP_SOURCES} )
    # Cut off the file extension and directory path
    get_filename_component( testname ${testsourcefile} NAME_WE )
    add_executable( ${testname} ${testsourcefile} )
    # Make sure YourLib is linked to each app
    target_link_libraries( ${testname} PRIVATE GC_Allocator )
    target_link_libraries( ${testname} PRIVATE Threads::Threads)
    target_include_directories(${testname} PUBLIC
                           ".."
                           )
endforeach( testsourcefile ${APP_SOURCES} )



# add_executable(LongHeapReferenceChain gc_heap_reference_chain.cpp)
# add_executable(StackReferencedMemory gc_stack_referenced.cpp)





# target_link_libraries(GarbageCollector PUBLIC GC_Allocator)
# target_link_libraries(GarbageCollector PRIVATE Threads::Threads)
# target_include_directories(GarbageCollector PUBLIC
#                            ".."
#                            )