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
   void free(void * addr);

   private:
   allocator(){}
   void delete_block(void * addr);
   void* get_block(size_t block_size);
   memory<ActiveManagement> free_memory{};
   memory<PassiveManagement> allocated_memory{};
};