#include <iterator>
#include <limits>
#include <cstdint>
#include <optional>

class memory_list;
class memory_block
{
   public:
   memory_block() : memory_pool{nullptr}, block_size{0}, next_block{nullptr}{}
   memory_block(void* mem_pool, size_t block_size) : memory_pool{mem_pool}, block_size{block_size}, next_block{nullptr}{}
   memory_block(size_t block_size) : memory_pool{this + 1}, block_size{block_size}, next_block{nullptr}{}
   friend memory_list;
   memory_block* next_block;
   void* memory_pool;
   size_t block_size;
};
class memory_list
{
   using iterator_category = std::forward_iterator_tag;
   using difference_type   = std::ptrdiff_t;
   using value_type        = memory_block;
   using pointer           = value_type*;
   public:
      memory_list()
      {
         block = reinterpret_cast<memory_block*>(malloc(sizeof(memory_block)));
         block->block_size = 0;
         block->memory_pool = nullptr;
         block->next_block = nullptr;
      }
      memory_list(memory_block * ptr) : block{ptr}{}
      value_type operator*(){return *block;}
      pointer operator->() {return block;}
      memory_list& operator++();
      memory_list operator++(int){memory_list tmp = *this; ++(*this); return tmp;}

      size_t get_block_size(){ return block->block_size;}
      std::optional<memory_block*> pop_next();
      void push_next(memory_block* el);
      void split_block(size_t block_size);
      bool left_neighbour(memory_block* ptr);
      bool right_neighbour(memory_block * ptr);

      friend bool operator== (const memory_list& a, const memory_list& b){return a.block == b.block;}
      friend bool operator!= (const memory_list& a, const memory_list& b) { return !(a==b); }
   //private:
   memory_block * block{};
};

class memory
{
   public:
   void insert(memory_block* ptr, bool merge_blocks = false);
   std::optional<memory_block*> release_block(size_t block_size);
   std::optional<memory_block*> release_block(void * addr);
   memory(){}
   memory_list begin(){return head;}
   memory_list end();
   uint16_t size=0;
   memory_list head{};
};