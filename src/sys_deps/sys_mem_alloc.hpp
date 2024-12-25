#ifndef SYS_MEM_ALLOC_HPP
#define SYS_MEM_ALLOC_HPP

#include <assert.h>
#define PAGE_SIZE 4096

#ifdef _WIN64
#include <windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
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
#include <sys/mman.h>
#include <unistd.h>
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
#endif

#endif // SYS_MEM_ALLOC_HPP