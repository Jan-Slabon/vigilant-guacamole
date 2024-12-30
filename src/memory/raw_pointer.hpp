#include <stddef.h>
class raw_pointer
{
    void * ptr;

    public:
    raw_pointer(void * ptr) : ptr{ptr}{}
    void * operator*(){
        return ptr;
    }
    raw_pointer operator+(size_t offset)
    {
        return reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr) + offset);
    }
    raw_pointer& operator++()
    {
        ptr = reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr) + 8);
        return *this;
    }
    raw_pointer operator++(int){ raw_pointer tmp = *this; ++(*this); return tmp; }
    operator void*(){return ptr;}
    /*
    Following definitions are allready provided by implicit void* cast
    */
    // friend bool operator<(raw_pointer, raw_pointer);
    // friend bool operator==(raw_pointer, raw_pointer);
    // friend bool operator!=(raw_pointer l, raw_pointer r){return !(l==r);}
    // friend bool operator<=(raw_pointer l, raw_pointer r){return l==r || l<r; }
    // friend bool operator>(raw_pointer l, raw_pointer r){return !(l<=r);}
    // friend bool operator>=(raw_pointer l, raw_pointer r){return !(r>l);}
};

// bool operator<(raw_pointer l, raw_pointer r)
// {
//     return l.ptr < r.ptr;
// }
// bool operator==(raw_pointer l, raw_pointer r)
// {
//     return l.ptr == r.ptr;
// }