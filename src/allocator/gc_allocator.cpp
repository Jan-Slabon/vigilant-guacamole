#include <stdio.h>
#include "gc_allocator.hpp"

allocator * allocator::alloc = nullptr;


void* allocator::get_block(size_t block_size)
{
  std::optional<memory_block*> reused_block = free_memory.release_block(block_size);
  if(reused_block)
  {
    printf("Realocating %ld Bytes of data\n", block_size);
    allocated_memory.insert(reused_block.value());
    return reused_block.value()->memory_pool;
  }
  printf("Alocating %ld Bytes of data\n", block_size);
  memory_block* new_block = reinterpret_cast<memory_block*>( malloc( sizeof(memory_block) ) );
  new (new_block) memory_block(malloc(block_size), block_size);

  allocated_memory.insert(new_block);
  return new_block->memory_pool;
}

void allocator::delete_block(void * addr)
{
  std::optional<memory_block *> deleted_block = allocated_memory.release_block(addr);

  if(deleted_block)
  {
    printf("Freeing %u bits of memory\n", deleted_block.value()->block_size);
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

allocator* allocator::getInstance()
{
  if(alloc == nullptr)
  {
      alloc = reinterpret_cast<allocator *> (malloc(sizeof(allocator)));
      new (&alloc->free_memory) memory();
      new (&alloc->allocated_memory) memory();
  }
  return alloc;
}