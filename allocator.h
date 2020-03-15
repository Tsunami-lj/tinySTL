#ifndef MYSTL_ALLOCATOR_H_
#define MYSTL_ALLOCATOR_H_

#include "alloc.h"

namespace mystl {

template <class T>
class allocator {
    typedef alloc Alloc;
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef T           value_type;
public:
    T* allocate() {
        return static_cast<T*>(Alloc::allocate(sizeof(T)));
    }
    
    T* allocate(size_type n, const void* = 0) {
        return !n ? 0 : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
    }
    
    void deallocate(pointer p) {
           Alloc::deallocate(p, sizeof(T));
    }
    
    void deallocate(pointer p, size_type n) {
        Alloc::deallocate(p, n * sizeof(T));
    }
    
    void construct(pointer p) {
        mystl::construct(p);
    }
    
    void construct(pointer p, const T& value) {
        mystl::construct(p, value);
    }
    
    void destroy(pointer p) {
        mystl::destroy(p);
    }
    
    void destroy(T* first, T* last) {
           mystl::destroy(first, last);
       }
};
}
#endif
