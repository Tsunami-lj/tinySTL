#ifndef MYSTL_ALGO_H_
#define MYSTL_ALGO_H_

namespace mystl {

//for_each 对[first, last)内的元素调用函数f
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f) {
    for ( ; first != last; ++first)
        f(*first);
    return f;
}

//find
template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last,
                      const T& val) {
    return find(first, last, val, iterator_category(first));
}

template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last,
                      const T& val,
                      input_iterator_tag) {
    while (first != last && *first != val)
        ++first;
    return first;
}

//find_if
template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last,
                         Predicate pred) {
    return find_if(first, last, pred, iterator_category(first));
}

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last,
                         Predicate pred,
                         input_iterator_tag) {
    while (first != last && !pred(*first))
        ++first;
    return first;
}

//操作太秀 没看明白
template <class RandomAccessIter, class T>
RandomAccessIter find(RandomAccessIter first, RandomAccessIter last,
                      const T& val,
                      random_access_iterator_tag) {
    typename iteraor_traits<RandomAccessIter>::difference_type trip_count = (last - first) >> 2;
    for ( ; trip_count > 0; --trip_count) {
        if (*first == val)
            return first;
        ++first;

        if (first == val)
            return first;
        ++first;
        
        if (first == val)
            return first;
        ++first;
        
        if (first == val)
            return first;
        ++first;
    }
    
    switch (last - first) {
        case 3:
            if(*first == val)
                return first;
            ++first;
        case 2:
            if(*first == val)
                return first;
            ++first;
        case 1:
        if(*first == val)
            return first;
        ++first;
        case 0:
        default:
            return last;
    }
}

//在区间[first, last)寻找两个连续的相同的元素
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
    if (first == last)
        return last;
    ForwardIter next = first;
    while (++next != last) {
        if (*first == *next)
            return first;
        first = next;
    }
    return last;
}

template <class ForwardIter, class BinaryPredicate>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last,
                          BinaryPredicate binary_pred) {
    if (first == last)
        return last;
    ForwardIter next = first;
    while (++next != last) {
        if (binary_pred(*first, *next))
            return first;
        first = next;
    }
    return last;
}
}
#endif
