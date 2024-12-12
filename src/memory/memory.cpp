#include "memory.hpp"
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>

memory_block illegal_element = memory_block(nullptr, std::numeric_limits<size_t>::max());

memory_list& memory_list::operator++()
{
  if(block->next_block)
  {
    block = block->next_block;
    return *this;
  }
  else
  {
    block = &illegal_element;
    return *this;
  }
}
void memory_list::split_block(size_t block_size)
{
  size_t remaining_size = get_block_size() - block_size - sizeof(memory_block);
  size_t memory_block_offset = reinterpret_cast<size_t>(block) + sizeof(memory_block) + block_size;
  memory_block * new_block = reinterpret_cast<memory_block*>(memory_block_offset);
  new (new_block) memory_block(remaining_size);
  block->block_size = block_size;
  new_block->next_block = block->next_block;
  block->next_block = new_block;
  printf("Partitioning the memory block for %zd and %zd\n", block->block_size, remaining_size);
}
bool memory_list::left_neighbour(memory_block* ptr)
{
  if(ptr == nullptr) return false;
  size_t ptr_right_corner = reinterpret_cast<size_t>(ptr) + sizeof(memory_block) + ptr->block_size;
  return reinterpret_cast<memory_block*>(ptr_right_corner) == block;
}
bool memory_list::right_neighbour(memory_block* ptr)
{
  if(ptr == nullptr) return false;
  size_t right_corner = reinterpret_cast<size_t>(block) + sizeof(memory_block) + block->block_size;
  return reinterpret_cast<memory_block*>(right_corner) == ptr;
}
std::optional<memory_block*> memory_list::pop_next()
{
  if(block->next_block != nullptr)
  {
    auto skipped_block = block->next_block;
    block->next_block = block->next_block->next_block;
    skipped_block->next_block = nullptr;
    return skipped_block;
  }
  return {};
}
void memory_list::push_next(memory_block* el)
{
  auto tmp = block->next_block;
  block->next_block = el;
  el->next_block = tmp;
}

template<>
memory<ActiveManagement>::memory()
{
  void * ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  assert(("Mmap failed to allocate memory", ptr != (void *)-1));

  head.block = reinterpret_cast<memory_block*>(ptr);
  head.block->block_size = PAGE_SIZE - sizeof(memory_block);
  head.split_block(0);
  head.block->memory_pool = nullptr;
}
template<>
memory<PassiveManagement>::memory()
{
  void * ptr = sbrk(sizeof(memory_block));
  assert(("Sbrk failed to allocate memory", ptr != (void*)-1));

  head.block = reinterpret_cast<memory_block*>(ptr);
  head.block->block_size = 0;
  head.block->memory_pool = nullptr;
}
template<class ManagementPolicy>
memory_list memory<ManagementPolicy>::end(){return memory_list(&illegal_element);}
template<>
void memory<ActiveManagement>::insert(memory_block* ptr)
{
  memory_list prev;
  for(auto it = begin(); it != end(); ++it) 
  {
    if(it.right_neighbour(ptr) && it != begin())
    {
      printf("Merging blocks of size %zd and %zd\n", it->block_size, ptr->block_size);
      it->block_size += ptr->block_size + sizeof(memory_block); // We are adding block to the memory pool of left aligned block in memory
      if(it.right_neighbour(it->next_block))
      {
        printf("Merging blocks of size %zd and %zd\n", it->block_size, it->next_block->block_size);
        it->block_size += it->next_block->block_size + sizeof(memory_block);
      }
      return;
    }
    else if(it.left_neighbour(ptr))
    {
      printf("Merging blocks of size %zd and %zd\n", ptr->block_size, it->block_size);
      ptr->block_size += it->block_size + sizeof(memory_block); // We are adding block to the memory pool of left aligned block in memory
      prev->next_block = it->next_block;
      prev.push_next(ptr);
      return;
    }
    else if(prev.block != nullptr && prev->memory_pool < ptr && it->memory_pool > ptr) // we are adding blocks in increasing pointer value order 
    {
      prev.push_next(ptr);
    }
    prev = it;
  }
  prev.push_next(ptr);
  size++;
}
template<>
void memory<PassiveManagement>::insert(memory_block* ptr)
{
  memory_list prev;
  for(auto it = begin(); it != end(); ++it) 
  {
    prev = it;
  }
  prev.push_next(ptr);
  size++;
}
/*
Returns any block from the list matching the given size and removes it from the list
*/
template<class ManagementPolicy>
std::optional<memory_block*> memory<ManagementPolicy>::release_block(size_t block_size)
{
  memory_list prev;
  for(auto it = begin(); it != end(); ++it)
  {
    
    if(it.get_block_size() == block_size)
    {
      size--;
      return prev.pop_next();
    }
    else if(it.get_block_size() > block_size + sizeof(memory_block))
    {
      it.split_block(block_size);
      return prev.pop_next();
    }
    prev = it;
  }
  return {};
}
/*
Returns a block from the list matching the given payload addres and removes it from the list
*/
template<class ManagementPolicy>
std::optional<memory_block*> memory<ManagementPolicy>::release_block(void * addr)
{
  memory_list prev;
  for(auto it = begin(); it != end(); ++it)
  {
    if(it->memory_pool == addr)
    {
      size--;
      return prev.pop_next();
    }
    prev = it;
  }
  return {};
}
/*
Preallocates block_size amount of free memory
*/
template<>
void memory<ActiveManagement>::reserve(size_t block_size)
{
  void * ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if(ptr == (void *)-1 || PAGE_SIZE < block_size)
  {
    printf("Mmap failed to prealocate memory\n");
  }
  else
  {
    new (ptr) memory_block(PAGE_SIZE);
    insert(static_cast<memory_block*>(ptr));
  }
}

template std::optional<memory_block*> memory<PassiveManagement>::release_block(void*);
template std::optional<memory_block*> memory<ActiveManagement>::release_block(void*);

template std::optional<memory_block*> memory<PassiveManagement>::release_block(size_t);
template std::optional<memory_block*> memory<ActiveManagement>::release_block(size_t);