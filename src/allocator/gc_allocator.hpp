#include <cstdint>
#include <stddef.h>
#include <stdlib.h>
#include <optional>

#include "memory/memory.hpp"

class allocator
{
   public:
   static allocator* getInstance();
   void * reserve(size_t size);
   void free(void * addr);

   private:
   allocator(){}
   static allocator* alloc;
   void delete_block(void * addr);
   void* get_block(size_t block_size);
   memory free_memory;
   memory allocated_memory;
};