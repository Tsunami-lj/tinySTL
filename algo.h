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

//在区间[first, last)寻找两个连续的[自定义相同]的元素
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

//count 统计区间[first, last)内值为value的个数
template <class InputIter, class T>
typename iteraor_traits<InputIter>::difference_type
count(InputIter first, InputIter last, const T& value) {
    typename iteraor_traits<InputIter>::difference_type n = 0;
    for ( ; first != last; ++first)
        if (*first == value)
            ++n;
    return n;
}

//count_if 统计区间[first, last)内满足一定条件的个数
template <class InputIter, class Predicate>
typename iteraor_traits<InputIter>::difference_type
count_if(InputIter first, InputIter last, Predicate pred) {
    typename iteraor_traits<InputIter>::difference_type n = 0;
    for ( ; first != last; ++first)
        if (pred(*first))
            ++n;
    return n;
}

//search 在区间[first1, last1)查找[first2, last2)
//此方法非常暴力，每次都是从[first2, last2)中取出一个来挨个判断，可用kmp思想改进
template <class ForwardIter1, class ForwardIter2>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2) {
    //判断区间是否为空
    if (first1 == last1 || first2 == last2)
        return first1;
    
    //如果[first2, last2)内只有一个元素，直接调用find
    ForwardIter2 tmp(first2);
    ++tmp;
    if (tmp == last2)
        return find(first1, last1, *first2);
    
    //一般情况
    ForwardIter2 p1, p;
    p1 = first2;
    ++p1;
    ForwardIter1 cur = first1;
    
    while (first1 != last1) {
        first1 = find(first1, last1, *first2);
        if (first1 == last1)
            return last1;
        
        p = p1;
        cur = first1;
        if (++cur == last1)
            return last1;
        
        while (*cur == *p) {
            if (++p == last2)
                return first1;
            if (++cur == last1)
                return last1;
        }
        ++first1;
    }
    return first1;
}

template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2,
                    BinaryPredicate predicate) {
    //判断区间是否为空
    if (first1 == last1 || first2 == last2)
        return first1;
    
    //如果[first2, last2)内只有一个元素，直接调用find
    ForwardIter2 tmp(first2);
    ++tmp;
    if (tmp == last2)
        return find(first1, last1, *first2);
    
    //一般情况
    ForwardIter2 p1, p;
    p1 = first2;
    ++p1;
    ForwardIter1 cur = first1;
    
    while (first1 != last1) {
        while (first1 != last1) {
            if (predicate(*first1, *first2))
                break;
            ++first1;
        }
        while (first1 != last1 && !predicate(*first1, *first2))
            ++first1;
        if (first1 == last1)
            return last1;
        
        p = p1;
        cur = first1;
        if (++cur == last1)
            return last1;
        
        while (predicate(*cur, *p)) {
            if (++p == last2)
                return first1;
            if (++cur == last1)
                return last1;
        }
        ++first1;
    }
    return first1;
}

//在[first, last)中寻找连续n个值为val的子序列，返回首位置。
template <class ForwardIter, class Integer, class T>
ForwardIter search_n(ForwardIter first, ForwardIter last,
                     Integer count, const T& val) {
    if (count <= 0)
        return first;
    first = find(first, last, val);
    while (first != last) {
        Integer n = count - 1;
        ForwardIter i = first;
        ++i;
        while (i != last && n != 0 && *i == val) {
            ++i;
            --n;
        }
        if (n == 0)
            return first;
        //如果只出现连续的个数少于n个，从i位置接着找一遍
        first = find(i, last, val);
    }
    return last;
}

//在[first, last)中寻找连续n个值为val的子序列，返回首位置。
template <class ForwardIter, class Integer, class T, class BinaryPredicate>
ForwardIter search_n(ForwardIter first, ForwardIter last,
                     Integer count, const T& val,
                     BinaryPredicate binary_pred) {
    if (count <= 0)
        return first;
    while (first != last) {
        if (binary_pred(*first, val))
            break;
        ++first;
    }
    while (first != last) {
        Integer n = count - 1;
        ForwardIter i = first;
        ++i;
        while (i != last && n != 0 && binary_pred(*i, val)) {
            ++i;
            --n;
        }
        if (n == 0)
            return first;
        //如果只出现连续的个数少于n个，从下一个相等的位置接着找一遍
        while (i != last) {
            if (binary_pred(*i, val))
                break;
            ++i;
        }
        first = i;
    }
    return last;
}

//swap_ranges, 交换[first1, last1)与[first2, last2)的值
template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
                         ForwardIter2 first2) {
    for ( ; first1 != last1; ++first1, ++first2)
        iter_swap(first1, first2);
    return first2;
}

//transform 将[first, last)挨个调用unaryoperation转换成result所保存的位置
//单目运算符转换
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter transform(InputIter first, InputIter last,
                     OutputIter res, UnaryOperation oper) {
    for ( ; first != last; ++first, ++res)
        *res = oper(*first);
    return res;
}

//双目运算符
template <class InputIter, class OutputIter, class BinaryOperation>
OutputIter transform(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, OutputIter res,
                     BinaryOperation binary_op) {
    for ( ; first1 != last1; ++first1, ++first2, ++res)
        *res = binary_op(*first1, *first2);
    return res;
}

// replace, replace_if, replace_copy, replace_copy_if
//将区间内[first, last)内的old_val改成new_val
template <class ForwardIter, class T>
void replace(ForwardIter first, ForwardIter last,
             const T& old_val, const T& new_val) {
    for ( ; first != last; ++first)
        if (*first == old_val)
            *first = new_val;
}

//将区间内[first, last)内满足条件的改成new_val
template <class ForwardIter, class Predicate, class T>
void replace_if(ForwardIter first, ForwardIter last,
             predicate pred, const T& new_val) {
    for ( ; first != last; ++first)
        if (pred(*first))
            *first = new_val;
}

//将[first, last)的元素拷贝到[res,...)的区域，并将其中的old_val改成new_val
template <class InputIter, class OutputIter, class T>
OutputIter replace_copy(InputIter first, InputIter last,
                        OutputIter res,
                        const T& old_val, const T& new_val) {
    for ( ; first != last; ++first, ++res)
        *res = *first == old_val ? new_val : *first;
    return res;
}

//将[first, last)的元素拷贝到[res,...)的区域，并将其中的满足条件的值改成new_val
template <class InputIter, class OutputIter, class Predicate, class T>
OutputIter replace_copy_if(InputIter first, InputIter last,
                        OutputIter res,
                        Predicate pred, const T& new_val) {
    for ( ; first != last; ++first, ++res)
        *res = pred(*first) ? new_val : *first;
    return res;
}

// generate and generate_n
//通过生成器连续生成填充到[first,last)内
template <class ForwardIter, class Generator>
void generate(ForwardIter first, ForwardIter last, Generator gen) {
    for ( ; first != last; ++first)
        *first = gen();
    return first;
}

//通过生成器连续生成填充到[first,first + n)内
template <class OutputIter, class Size, class Generator>
OutputIter generate_n(OutputIter first, Size n, Generator gen) {
    for ( ; n > 0; --n, ++first)
        *first = gen();
    return first;
}

// remove, remove_if, remove_copy, remove_copy_if
template <class ForwardIter, class T>
ForwardIter remove(ForwardIter first, ForwardIter last,
                   const T& val) {
    first = find(first, last, val);
    ForwardIter i = first;
    return first == last ? first : remove_copy(++i, last, first, val);
}

template <class InputIter, class OutputIter, class T>
OutputIter remove_copy(InputIter first, InputIter last,
                       OutputIter res, const T& val) {
    for ( ; first != last; ++first)
        if (*first != val) {
            *res = *first;
            ++res;
        }
    return res;
}

template <class InputIter, class OutputIter, class Predicate>
OutputIter remove_if(InputIter first, InputIter last,
                     Predicate pred) {
    first = find_if(first, last, pred);
    ForwardIter i = first;
    return first == last ? first : remove_copy_if(++i, last, first, pred);
}

template <class InputIter, class OutputIter, class Predicate>
OutputIter remove_copy_if(InputIter first, InputIter last,
                       OutputIter res, Predicate pred) {
    for ( ; first != last; ++first)
        if (!pred(*first)) {
            *res = *first;
            ++res;
        }
    return res;
}

}
#endif
