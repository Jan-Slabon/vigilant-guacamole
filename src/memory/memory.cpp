#include "memory.hpp"


memory_block illegal_element = memory_block(nullptr, std::numeric_limits<uint16_t>::max());

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

memory_list memory::end(){return memory_list(&illegal_element);}

void memory::insert(memory_block* ptr)
{
  memory_list prev;
  for(auto it = begin(); it != end(); ++it) {prev = it;}
  prev.push_next(ptr);
  size++;
}
/*
Returns any block from the list matching the given size and removes it from the list
*/
std::optional<memory_block*> memory::release_block(size_t block_size)
{
  memory_list prev;
  for(auto it = begin(); it != end(); ++it)
  {
    
    if(it.get_block_size() == block_size)
    {
      size--;
      return prev.pop_next();
    }
    prev = it;
  }
  return {};
}
/*
Returns a block from the list matching the given payload addres and removes it from the list
*/
std::optional<memory_block*> memory::release_block(void * addr)
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
