#include <stddef.h>
#include "memory/raw_pointer.hpp"
namespace monotonic
{
    class allocator
    {
        public:
        allocator() = delete;
        /*Creates monotonic allocator with heap of size given in MB*/
        allocator(size_t mem_size);
        /*Alocates memory for variable of given type*/
        template<typename T>
        T* alloc()
        {
            raw_pointer return_mem = stack_top;
            stack_top += sizeof(T);
            return reinterpret_cast<T*>((void*)return_mem);
        }
        /*Releases all allocated memory*/
        void free();
        private:
        void * mem_pool;
        raw_pointer stack_top;
    };
}