#ifndef MYSTL_ALGOBASE_H_
#define MYSTL_ALGOBASE_H_

#include "iterator.h"
#include "pair.h"
#include "type_traits.h"

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

//copy backward
//将 [first, last)区间内的元素复制到 [result - (last - first), result)内
//eg: v1 = {1, 2, 3, 4, 5}, v2 = {0, 0, 0, 0}; copy_backward(v1.begin(), v1.begin() + 3, v2.end() - 1)
//可得到 v2 = {1, 2, 3, 0};
template <class BidrectionalIter1, class BidrectionalIter2>
inline BidrectionalIter2 copy_backward(BidrectionalIter1 first, BidrectionalIter1 last, BidrectionalIter2 result) {
    typedef typename __type_traits<typename iterator_traits<BidrectionalIter2>::value_type>::has_trivial_assign Trival;
    return __copy_backward_dispatch<BidrectionalIter1, BidrectionalIter2, Trival>
                ::copy(first, last, result);
}

template <class BidrectionalIter1, BidrectionalIter2, class BoolType>
struct __copy_backword_dispatch {
    typedef typename iterator_traits<BidrectionalIter1>::iterator_category Category;
    typedef typename iterator_traits<BidrectionalIter1>::difference_type Distance;
    static BidrectionalIter2 copy(BidrectionalIter1 first,
                                  BidrectionalIter1 last,
                                  BidrectionalIter2 result) {
        return __copy_backward(first, last, Category(), (Distance *) 0);
    }
};

template <class T>
struct __copy_backward_dispatch<T*, T*, __true_type> {
    static T* copy(const T* first, const T* last, T* result) {
        const ptrdiff_t num = last - first;
        memmove(result - num, first, sizeof(T) * num);
        return result - num;
    }
};

template <class T>
struct __copy_backward_dispatch<const T*, T*, __true_type> {
    static T* copy(const T* first, const T* last, T* result) {
        return __copy_backward_dispatch<T*, T*, __true_type>
            ::copy(first, last, result);
    }
};

template <class BidrectionalIter1, class BidrectionalIter2, class Distance>
inline BidrectionalIter2 __copy_backward(BidrectionalIter1 first,
                                         BidrectionalIter1 last,
                                         BidrectionalIter2 result,
                                         bidrectional_iterator_tag,
                                         Distance*) {
    while (first != last) {
        *--result = *--first;
    }
    return result;
}

template <class RandomAccessIter, class BidrectionalIter, class Distance>
inline BidrectionalIter2 __copy_backward(RandomAccessIter first,
                                         RandomAccessIter last,
                                         BidrectionalIter2 result,
                                         random_access_iterator_tag,
                                         Distance*) {
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

//fill
template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) {
    for ( ; first != last; ++first)
        *first = value;
}

//fill_n
template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value) {
    for ( ; n > 0; --n, ++first)
        *first = value;
    return first;
}

//equal and mismatch
//比较两个序列不同的迭代器位置
template <class InputIter1, class InputIter2>
pair<InputIter1, InputIter2> mismatch(InputIter1 first1,
                                      InputIter1 last1,
                                      InputIter2 first2) {
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return pair<InputIter1, InputIter2>(first1, first2);
}

//自定义比较函数，比较两个序列是否相同
template <class InputIter1, class InputIter2, class Compare>
pair<InputIter1, InputIter2> mismatch(InputIter1 first1,
                                      InputIter1 last1,
                                      InputIter2 first2,
                                      Compare comp) {
    while (first1 != last1 && comp(*first1, *first2)) {
        ++first1;
        ++first2;
    }
    return pair<InputIter1, InputIter2>(first1, first2);
}

template <class InputIter1, class InputIter2>
inline bool equal(InputIter1 first1, InputIter1 last1,
                  InputIter2 first2) {
    for ( ; first1 != last1; ++first1, ++first2)
        if (*first1 != *first2)
            return false;
    return true;
}

template <class InputIter1, class InputIter2, class Compare>
inline bool equal(InputIter1 first1, InputIter1 last1,
                  InputIter2 first2, Compare comp) {
    for ( ; first1 != last1; ++first1, ++first2)
        if (!comp(*first1, *first2))
            return false;
    return true;
}

template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2) {
    for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIter1, class InputIter2, class Compare>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2,
                             Compare comp) {
    for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

inline bool lexicographical_compare(const unsigned char* first1,
                                    const unsigned char* last1,
                                    const unsigned char* first2,
                                    const unsigned char* last2) {
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    const int result = memcmp(first1, first2, min(len1, len2));
    return result ? result < 0 : len1 < len2;
}
#endif
