#ifndef MYSTL_UNINITIALIZED_H_
#define MYSTL_UNINITIALIZED_H_

#include "iterator.h"
#include "type_traits.h"
namespace mystl {

//uninitialized_copy
template<class InputIter, class ForwardIter>
inline ForwardIter
    uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
        return __uninitialized_copy(first, last, result, value_type(result));
}

template <class InputIter, class ForwardIter, class T>
inline ForwardIter
    __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, T*) {
        typedef typename __type_traits<T>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <class InputIter, class ForwardIter>
inline ForwardIter
    __uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __true_type) {
        copy(first, last, result);
}

template <class InputIter, class ForwardIter>
inline ForwardIter
    __uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __false_type) {
        ForwardIter cur = result;
        for ( ; first != last; ++first, ++cur)
            construct(&*cur, *first);
        return cur;
}

inline char* uninitialized_copy(const char* first, const char* last, char* result) {
    memmove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

//uninitialized_copy_n
template <class InputIter, class Size, class ForwardIter>
inline pair<InputIter, ForwardIter>
    uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
        return __uninitialized_copy_n(first, n, result, iterator_category(first));
}

template <class InputIter, class Size, class ForwardIter>
inline pair<InputIter, ForwardIter>
__uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
    return __uninitialized_copy_n(first, n, result, iterator_category(first));
}

//此处只需要实现两个特化版本即可。根据迭代器tag继承关系，实现random 和 input之后，其他非random会自动选用input的copy_n
template <class RandomAccessIter, class Size, class ForwardIter>
inline pair<RandomAccessIter, ForwardIter>
__uninitialized_copy_n(RandomAccessIter first, Size n, ForwardIter result, random_access_iterator_tag) {
    RandomAccessIter last = first + n;
    return pair<RandomAccessIter, ForwardIter> (last, uninitialized_copy(first, last, result));
}

template <class InputIter, class Size. class ForwardIter>
pair<InputIter, ForwardIter>
__uninitialized_copy_n(InputIter first, Size n, ForwardIter result, input_iterator_tag) {
    ForwardIter cur = result;
    for (; n > 0; --n, ++first, ++cur)
        construct(&*cur, *first);
    return pair<InputIter, ForwardIter>(first, cur);
}

//uninitialized_fill
template <class ForwardIter, class T>
inline void uninitialized_fill(ForwardIter first, ForwardIter last, const T& x) {
    __uninitialized_fill(first, last, x, value_type(first));
}

template <class ForwardIter, class T, class T1>
inline void __uninitialized_fill(ForwardIter first, ForwardIter last, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    __uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIter, class T>
inline void __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const T& x, __false_type) {
    ForwardIter cur = first;
    for ( ; cur != last; ++cur)
        construct(&*cur, x);
}

template <class ForwardIter, class T>
inline void __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const T& x, __true_type) {
    fill(first, last, x);
}

//uninitialized_fill_n
template <class ForwardIter, class Size, class T>
inline ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, const T& x) {
    return __uninitialized_fill_n(first, n, x, value_type(first));
}

template <class ForwardIter, class Size, class T, class T1>
inline ForwardIter
__uninitialized_fill_n(ForwardIter first, Size n, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <class ForwardIter, class Size, class T>
inline ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const T& x, __false_type) {
    ForwardIter cur = first;
    for ( ; n > 0; --n, ++cur)
        construct(&*cur, x);
    return cur;
}

template <class ForwardIter, class Size, class T>
inline ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n, const T& x, __true_type) {
    return fill_n(first, n, x);
}

}
#endif
