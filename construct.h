#ifndef MYSTL_CONSTRUCT_H_
#define MYSTL_CONSTRUCT_H_

#include <new>
#include "type_traits"

namespace mystl {

template <class T>
inline void construct(T* p) {
    new (p) T();
}

template <class T1, class T2>
inline void construct(T1* p, T2& value) {
    new (p) T1(value);
}

template <class T>
inline void destroy(T* p) {
    p->~T();
}

template <class ForwardIterator>
inline void destroy (ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first));
}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
    typedef typename __type_traits<T>::has_trivial_destructor trival_destructor;
    __destroy_aux(first, last, trival_destructor());
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for( ; first != last; ++first)
        destroy(&*first);
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {
}

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}
}
