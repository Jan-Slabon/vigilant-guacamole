#include <stdio.h>
#include "gc_allocator.hpp"

allocator * allocator::alloc = nullptr;

void* allocator::get_block(size_t block_size)
{
  memory_list * free_memory = free_memory_list;
  while(free_memory->next_block != nullptr && free_memory->next_block->block_size != block_size) free_memory = free_memory->next_block;
  if(free_memory->next_block != nullptr)
  {
    memory_list * second_block = allocated_memory_list->next_block;
    allocated_memory_list->next_block = free_memory->next_block;

    printf("Realocating %ld Bytes of data\n", block_size);

    free_memory->next_block = free_memory->next_block->next_block;
    allocated_memory_list->next_block->next_block = second_block;
    return allocated_memory_list->next_block->memory_pool;
  }
  auto handle = allocated_memory_list;
  while(handle->next_block != nullptr) {handle = handle->next_block; printf("O\n");}
  handle->next_block = reinterpret_cast<memory_list*>( malloc( sizeof(memory_list) ) );
  handle = handle->next_block;
  handle->next_block = nullptr;
  handle->block_size = block_size;
  handle->memory_pool = malloc(block_size);
  return handle->memory_pool;
}

void allocator::delete_block(void * addr)
{
  auto handle = allocated_memory_list;
  while(handle->next_block != nullptr && handle->next_block->memory_pool != addr) {handle = handle->next_block; printf("I\n");} // this works only because of lazy evaluation of a && b expression
  if(handle->next_block != nullptr)
  {
    memory_list* second_block = free_memory_list->next_block;
    free_memory_list->next_block = handle->next_block;

    printf("Freeing %u bits of memory\n", handle->next_block->block_size);

    handle->next_block = handle->next_block->next_block;
    free_memory_list->next_block->next_block = second_block;
  }
  else
  {
    printf("Attempting to free unclaimed memory!\n");
  }
}

void * allocator::reserve(size_t size)
{
  printf("Allocation of %lu bits\n", size);
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
      alloc->free_memory_list = reinterpret_cast<memory_list *> (malloc(sizeof(memory_list)));
      alloc->free_memory_list->next_block = nullptr;
      alloc->allocated_memory_list = reinterpret_cast<memory_list *> (malloc(sizeof(memory_list)));
      alloc->allocated_memory_list->next_block = nullptr;
  }
  return alloc;
}