#include <assert.h>
#include "sys_deps/sys_mem_alloc.hpp"

#define PAGE_SIZE 4096

#ifdef _WIN64
void * page_alloc(size_t size)
{
   size_t pages = size / PAGE_SIZE + 1;
   void * ptr = HeapAlloc(GetProcessHeap(), 0, pages * PAGE_SIZE);
   assert(("Page allocation failed to allocate memory", ptr != nullptr));
   return ptr;
}

void * small_alloc(size_t size)
{
    void * ptr = HeapAlloc(GetProcessHeap(), 0, size);
    assert(("Small allocation failed to allocate memory", ptr != nullptr));
    return ptr;
}
#endif

#ifdef __linux__
#include <stdio.h>
#include <pthread.h>
void * page_alloc(size_t size)
{
   size_t pages = size / PAGE_SIZE + 1;
   void * ptr = mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
   assert(("Page allocation failed to allocate memory", ptr != (void *)-1));
   return ptr;
}

void * small_alloc(size_t size)
{
    void * ptr = sbrk(size);
    assert(("Small allocation failed to allocate memory", ptr != (void*)-1));
    return ptr;
}
void * get_stack_bottom()
{
    pthread_attr_t attrs;
    assert( ("Pthread failed to instantialise attrs", pthread_getattr_np( pthread_self(), &attrs ) == 0));
    void   *stackAddr;
    size_t  stackSize;
    assert(("Pthread failed to aquire stack data", pthread_attr_getstack( &attrs, &stackAddr, &stackSize ) == 0));
    printf("Stack size = %lu stack top bottom bound %p\n", stackSize, stackAddr);
    return (void *)((char *)stackAddr + stackSize);
}
#endif
