#include <cstdint>
#include <stddef.h>
#include <stdlib.h>
#include <optional>
#include "memory/memory.hpp"

class allocator
{
   public:
   static allocator getInstance();
   void * reserve(size_t size);
   void collect();
   void free(void * addr);

   private:
   allocator(){}
   void delete_block(void * addr);
   void* get_block(size_t block_size);
   void scan_stack_memory_references(raw_pointer low, raw_pointer high);
   void scan_heap_memory_references();
   void clean_allocated_memory();
   memory<ActiveManagement> free_memory{};
   memory<PassiveManagement> allocated_memory{};
};