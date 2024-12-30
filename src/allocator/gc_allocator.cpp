#include <stdio.h>
#include "gc_allocator.hpp"
#include "sys_deps/sys_mem_alloc.hpp"

void* allocator::get_block(size_t block_size)
{
  std::optional<memory_block*> reused_block = free_memory.release_block(block_size);
  if(reused_block)
  {
    printf("Realocating %zd Bytes of data\n", block_size);
    allocated_memory.insert(reused_block.value());
    printf("Alocated memory inserted into allocated memory list\n");
    return reused_block.value()->memory_pool;
  }
  printf("Alocating %zd Bytes of data\n", block_size);
  free_memory.reserve(block_size);
  auto new_block = free_memory.release_block(block_size);
  if(new_block)
  {
    allocated_memory.insert(new_block.value());
    return new_block.value()->memory_pool;
  }
  printf("Unable to allocate %zd bytes of memory\n", block_size);
  return nullptr;
}

void allocator::delete_block(void * addr)
{
  std::optional<memory_block *> deleted_block = allocated_memory.release_block(addr);

  if(deleted_block)
  {
    printf("Freeing %zd bits of memory\n", deleted_block.value()->block_size);
    free_memory.insert(deleted_block.value());
  }
  else
  {
    printf("Attempting to free unclaimed memory!\n");
  }
}

void * allocator::reserve(size_t size)
{
  void* memory_block = get_block(size);
  return memory_block;
}

void allocator::free(void * addr)
{
  delete_block(addr);
}

void allocator::scan_stack_memory_references(raw_pointer low, raw_pointer high)
{
  printf("Scannig from Low = %p to High = %p\n", low, high);
  for(raw_pointer ptr = low; ptr < high; ptr++)
  {
    for(auto it = allocated_memory.begin(); it != allocated_memory.end(); ++it)
    {
      void * value = reinterpret_cast<void*>(*reinterpret_cast<long unsigned*> (*ptr));
      if(it.block <= value && it->memory_pool + it->block_size >= value)
      {
        it->is_referenced = true;
      }
    }
  }
}
void allocator::scan_heap_memory_references()
{
  for(auto it = allocated_memory.begin(); it != allocated_memory.end(); ++it)
  {
    if(it->is_referenced == false)
    {
      continue;
    }
    for(raw_pointer ptr = it->memory_pool; ptr < it->memory_pool + it->block_size; ptr++)
    {
      for(auto it2 = allocated_memory.begin(); it2 != allocated_memory.end(); ++it2)
      {
        if(it2->memory_pool <= ptr && it2->memory_pool + it2->block_size >= ptr)
        {
          it2->is_referenced = true;
        }
      }
    }
  }
}
void allocator::clean_allocated_memory()
{
  memory_list prev = allocated_memory.begin();
  for(auto it = ++allocated_memory.begin(); it != allocated_memory.end(); ++it)
  {
    if(it->is_referenced == false)
    {
      prev.block->next_block = it.block->next_block;
      free_memory.insert(it.block);
      it = prev;
    }
    else
    {
      prev = it;
    }
  }
}
void allocator::collect()
{
  extern unsigned long end, etext; // These are symbols defined by the linker
  void * stack_top;
  void * stack_bottom = get_stack_bottom();
  asm volatile ("mov %%rbp, %0;" : "=r" (stack_top));
  printf("Starting memory cleanup\n");
  scan_stack_memory_references(&etext, &end);
  printf("BSS and Data segements scaned\n");
  scan_stack_memory_references(stack_top, stack_bottom);
  printf("Stack scaned\n");
  scan_heap_memory_references();
  printf("Heap referenced memory marked\n");
  clean_allocated_memory();
  printf("Heap memory garbege collected\n");
  for(auto it = allocated_memory.begin(); it != allocated_memory.end(); ++it){it.block->is_referenced = false;}
}

allocator allocator::getInstance()
{
  static allocator instance{};
  return instance;
}