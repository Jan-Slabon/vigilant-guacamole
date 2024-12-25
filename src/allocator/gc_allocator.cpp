#include <stdio.h>
#include "gc_allocator.hpp"


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

allocator allocator::getInstance()
{
  static allocator instance{};
  return instance;
}