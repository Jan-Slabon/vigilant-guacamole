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
void * page_alloc(size_t size);

void * small_alloc(size_t size);
#endif

#ifdef __linux__
#include <sys/mman.h>
#include <unistd.h>
void * page_alloc(size_t size);

void * small_alloc(size_t size);
void * get_stack_bottom();
#endif

#endif // SYS_MEM_ALLOC_HPP