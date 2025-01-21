#include "allocator/monotonic_allocator.hpp"
#include "sys_deps/sys_mem_alloc.hpp"

namespace monotonic
{
    allocator::allocator(size_t mem_size) : mem_pool{page_alloc(mem_size * 1e6)}, stack_top{mem_pool}{}

    void allocator::free()
    {
        stack_top = mem_pool;
    }
}