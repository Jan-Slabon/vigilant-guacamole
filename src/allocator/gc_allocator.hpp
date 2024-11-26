#include <cstdint>
#include <stddef.h>
#include <stdlib.h>

class memory_list
{
  public:
      memory_list* next_block;
      void* memory_pool;
      uint16_t block_size;
};


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
   memory_list* free_memory_list;
   memory_list* allocated_memory_list;
};