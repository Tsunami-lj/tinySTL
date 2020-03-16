#ifndef MYSTL_ALGOBASE_H_
#define MYSTL_ALGOBASE_H_

namespace mystl {

//swap && iter_swap
template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b) {
    __iter_swap(a, b, value_type(a));
}

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*) {
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class T>
inline void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

//min && max
template <class T>
inline const T& min(const T& a, const T& b) {
    return a < b ? a : b;
}

template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? a : b;
}

template <class T>
inline const T& max(const T& a, const T& b) {
    return b < a ? a : b;
}

template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? a : b;
}

//copy
//copy完全泛化版本
template <class InputIter, class OutputIter>
inline OutputIter copy(InputIter first, InputIter last, OutputIter result) {
    typedef typename iteraror_traits<InputIter>::value_type T;
    typedef typename __type_traits<T>::has_trivial_assign Trival;
    return __copy_dispatch<InputIter, OutputIter, Trival>
        ::copy(first, last, result);
}

//__copy_dispatch完全泛化版本
template <class InputIter, class OutputIter, class BoolType>
struct __copy_dispatch {
    static OutputIter copy(InputIter first, InputIter last, OutputIter result) {
        typedef typename iterator_traits<InputIter>::iterator_category Category;
        typedef typename iterator_traits<InputIter>::difference_type Distance;
        return __copy(first, last, result, Category(), (Distance*) 0);
    }
}

//__copy_dispatch指针偏特化版本
template <class T>
struct __copy_dispatch<T*, T*, __true_type> {
    static T* copy(const T* first, const T* last, T* result) {
        return __copy_trival(first, last, result);
    }
}

//__copy_dispatch常指针偏特化版本
template <class T>
struct __copy_dispatch<const T*, T*, __true_type> {
    static T* copy(const T* first, const T* last, T* result) {
        return __copy_trival(first, last, result);
    }
}

template <class T>
inline T* __copy_trivial(const T* __first, const T* __last, T* result) {
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template <class InputIter, class OutputIter, class Distance>
inline OutputIter __copy(InputIter first, InputIter last,
                         OutputIter result,
                         input_iterator_tag, Distance*) {
    //比较迭代器是否走到last
    for ( ; first != last; ++result, ++first)
        *result = *first;
    return result;
}

template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter __copy(RandomAccessIter first, RandomAccessIter last,
                         OutputIter result,
                         random_access_iterator_tag, Distance*) {
    //循环n次，效率比input高
    for (Distance n = last - first; n > 0; --n, ++result, ++first)
        *result = *first;
    return result;
}

inline char* copy(const char* first, const char* last, char* result) {
    memmove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

#endif
