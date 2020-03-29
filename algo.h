#ifndef MYSTL_ALGO_H_
#define MYSTL_ALGO_H_

namespace mystl {

//median函数，求三个数的中位数
template <class T>
inline const T& median(const T& a, const T&b, const T& c) {
    if (a < b) {
        if (b < c) // a < b < c
            return b;
        else if (a < c) // a < b, c < b, a < c
            return c;
        else
            return a;
    } else if (a < c) // b < a, a < c
        return a;
    else if (b < c) // b < a, c < a, b < c
        return c;
    return b;
}

//median函数，求三个数的中位数,自定义比较函数
template <class T, class Compare>
inline const T& median(const T& a, const T&b, const T& c, Compare comp) {
    if (comp(a, b)) {
        if (comp(b, c)) // a < b < c
            return b;
        else if (comp(a, c)) // a < b, c < b, a < c
            return c;
        else
            return a;
    } else if (comp(a, c)) // b < a, a < c
        return a;
    else if (comp(b, c)) // b < a, c < a, b < c
        return c;
    return b;
}


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

// unique and unique_copy
// 拷贝并去除b重复元素，实现方式比较简单，每次判断后一个位置是否与已经放置的元素相同
//效果举例 10,20,20,20,30,30,20,20,10 => 10 20 30 20 10 0 0 0 0
template <class InputIter, class OutputIter>
inline OutputIter unique_copy(InputIter first, InputIter last,
                              OutputIter res) {
    if (first == last)
        return res;
    return __unique_copy(first, last, res,
                         iterator_category(res));
}

template <class InputIter, class ForwardIter>
ForwardIter __unique_copy(InputIter first, InputIter last,
                         ForwardIter res, forward_iterator_tag) {
    *res = *first;
    while (++first != last) {
        if (*res != *first)
            *++res = *first;
    }
    return ++res;
}

template <class InputIter, class OutputIter>
inline OutputIter __unique_copy(InputIter first, InputIter last,
                                OutputIter res,
                                output_iterator_tag) {
    return __unique_copy(first, last, res, value_type(first));
}

//由于 output iterator 只能进行写操作，所以不能有 *result != *first 这样的判断
template <class InputIter, class OutputIter, class T>
OutputIter __unique_copy(InputIter first, InputIter last,
                         OutputIter res, T*) {
    T val = *first;
    *res = val;
    while (++first != last)
        if (val != *first) {
            val = *first;
            *++res = val;
        }
    return ++res;
}

//使用比较对象cmp来比较
template <class InputIter, class OutputIter, class BinaryPredicate>
inline OutputIter unique_copy(InputIter first, InputIter last,
                              OutputIter res,
                              BinaryPredicate binary_pred) {
    if (first == last)
        return res;
    return __unique_copy(first, last, result, binary_pred, iterator_category(res));
}

template <class InputIter, class ForwardIter, class BinaryPredicate>
ForwardIter __unique_copy(InputIter first, InputIter last,
                          ForwardIter res,
                          BinaryPredicate binary_pred,
                          forward_iterator_tag) {
    *res = *first;
    while (++first != last)
        if (!binary_pred(*res, *first))
            *++res = *first;
    return ++result;
}

template <class InputIter, class OutputIter, class BinaryPredicate>
OutputIter __unique_copy(InputIter first, InputIter last,
                          OutputIter res,
                          BinaryPredicate binary_pred,
                          output_iterator_tag) {
    return __unique_copy(first, last, res, binary_pred, value_type(first));
}

template <class InputIter, class OutputIter, class BinaryPredicate, class T>
OutputIter __unique_copy(InputIter first, InputIter last,
                         OutputIter res,
                         BinaryPredicate binary_pred, T*) {
    T val = *first;
    *res = val;
    while (++first != last)
        if (!binary_pred(val, *first)) {
            val = *first;
            *++res = val;
        }
    return ++res;
}

//unique
//将[first, last)区间内连续的重复元素去掉，但是内部依然可能有相同的元素
template <class ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last) {
    first = adjacent_find(first, last);
    return unique_copy(first, last, first);
}

template <class ForwardIterm, BinaryPredicate binary_pred>
ForwardIter unique(ForwardIter first, ForwardIter last,
                   BinaryPredicate binary_pred) {
    first = adjacent_find(first, last, binary_pred);
    return unique_copy(first, last, first, binary_pred);
}

//reverse and reverse_copy, and their auxiliary functions
template <class BidrectionalIter>
inline void reverse(BidrectionalIter first, BidrectionalIter last) {
    __reverse(first, last, iterator_category(first));
}

template <class BidrectionalIter>
void __reverse(BidrectionalIter first, BidrectionalIter last,
               bidrectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last)
            return ;//小于等于一个元素，直接返回
        iter_swap(first++, last);
    }
}

//random_access版本
template <class RandomAccessIter>
void __reverse(RandomAccessIter first, RandomAccessIter last,
               random_access_iterator_tag) {
    while (first < last) {
        iter_swap(first++, --last);
    }
}

template <class BidrectionalIter, class OutputIter>
OutputIter reverse_copy(BidrectionalIter first,
                        BidrectionalIter last,
                        OutputIter res) {
    while (first != last) {
        --last;
        *res = *last;
        ++res;
    }
    return res;
}

// rotate and rotate_copy, and their auxiliary functions
//rotate区间[first, last)内的元素，使得以middle为起始元素
template <class ForwardIter>
inline ForwardIter rotate(ForwardIter first, ForwardIter middle,
                         ForwardIter last) {
    return __rotate(first, middle, last,
                    distance_type(first),
                    iterator_category(first));
}

//ForwardItery特化版本
template <class ForwardIter, class Distance>
ForwardIter __rotate(ForwardIter first,
                     ForwardIter middle,
                     ForwardIter last,
                     Distance*,
                     forward_iterator_tag) {
    if (first == middle)
        return last;
    if (last == middle)
        return first;
    
    //当middle比较靠前[first, middle) < [middle, last)的时候，一段段的将[first, middle)往后换
    ForwardIter first2 = middle;
    do {
        swap(*first++, *first2++);
        if (first == middle)
            middle = first2;
    } while (first2 != last);
    
    //此时必定是middle靠后[first, middle) > [middle, last)
    ForwardIter new_middle = first;
    
    first2 = middle;
    while (first2 != last) {
        swap(*first++, *first2++);
        if (first == middle) //此情形与上述过程一样
            middle = first2;
        else if (first2 == last)// 此时将原来的长度变成一个更小区间的子问题
            first2 = middle;
    }
    return new_middle;
}

//BidrectionalIter特化版本
template <class BidrectionalIter, class Distance>
BidrectionalIter __rotate(BidrectionalIter first,
                     BidrectionalIter middle,
                     BidrectionalIter last,
                     Distance*,
                     bidrectional_iterator_tag) {
    if (first == middle)
        return last;
    if (last == middle)
        return first;
    
    reverse(first, middle, bidrectional_iterator_tag());
    reverse(middle, last, bidrectional_iterator_tag());
    
    while (first != middle && middle != last)
        swap(*first++, *--last);
    
    if (first == middle) {
        reverse(middle, last, bidrectional_iterator_tag());
    } else {
        reverse(first, middle, bidrectional_iterator_tag());
        return first;
    }
}

//RandomAccessIter特化版本
template <class RandomAccessIter, class Distance, class T>
RandomAccessIter __rotate(RandomAccessIter first,
                     RandomAccessIter middle,
                     RandomAccessIter last,
                     Distance*, T*) {
    Distance n = last - first;
    Distance k = middle - first;
    Distance l = n - k;
    RandomAccessIter result = first + (last - middle);
    
    if (k == l) {
        swap_ranges(first, middle, middle);
        return result;
    }
    
    Distance d = gcd(n, k);
    //该算法细节较为复杂，核心思想是分块，每次将相同偏移的处理好。具体代码有时间需要慢慢扣
    for (Distance i = 0; i < d; i++) {
        T tmp = *first;
        RandomAccessIter p = first;
        
        if (k < l) {
            for (Distance j = 0; j < l/d; j++) {
                if (p > first + l) {
                    *p = *(p - l);
                    p -= l;
                }
                *p = *(p + k);
                p += k;
            }
        } else {
            for (Distance j = 0; j < k/d - 1; j++) {
                if (p < last - k) {
                    *p = *(p + k);
                    p += k;
                }
                
                *p = *(p - l);
                p -= l;
            }
        }
        *p = tmp;
        ++first;
    }
    return result;
}

template <class EuclideanRingElement>
EuclideanRingElement gcd (EuclideanRingElement m, EuclideanRingElement n) {
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <class ForwardIter, class OutputIter>
OutputIter rotate_copy(ForwardIter first, ForwardIter middle,
                       ForwardIter last, OutputIter result) {
    //先将[middle, last)拷贝到res， 再将[first, middle)拷贝
    return copy(first, middle, copy(middle, last, result));
}

template <class Distance>
inline Distance random_number(Distance n) {
    return rand() % n;
}

//random_shuffle
template <class RandomAccessIter>
inline void random_shuffle(RandomAccessIter first, RandomAccessIter last) {
    if (first == last)
        return ;
    //每个位置都跟随机的一个位置交换一下
    for (RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + random_number((i - first) + 1));
}

template <class RandomAccessIter, class RandomNumberGenerator>
void random_shuffle(RandomAccessIter first, RandomAccessIter last,
                    RandomNumberGenerator& rand) {
    if (first == last)
        return ;
    //每个位置都跟随机的一个位置交换一下
    for (RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + rand((i - first) + 1));
}

//random_sample and random_sample_n
template <class InputIter, class RandomAccessIter>
inline RandomAccessIter
random_sample(InputIter first, InputIter last,
              RandomAccessIter out_first, RandomAccessIter out_last) {
    return __random_sample(first, last, out_first, out_last - out_first);
}

template <class InputIter, class RandomAccessIter, class Distance>
RandomAccessIter __random_sample(InputIter first, InputIter last,
                                 RandomAccessIter res, const Distance n) {
    Distance m = 0;
    Distance t = n;
    for ( ; first != last && m < n; ++m, ++first)
        res[m] = *first;
    
    while (first != last) {
        ++t;
        Distance M = random_number(t);
        if (M < n)
            res[M] = *first;
        ++first;
    }
    return res + m;
}

//带自定义随机数生成器的c随机采样
template <class InputIter, class RandomAccessIter, class RandomNumberGenerator>
inline RandomAccessIter
random_sample(InputIter first, InputIter last,
              RandomAccessIter out_first, RandomAccessIter out_last,
              RandomNumberGenerator& rand) {
    return __random_sample(first, last, out_first, rand, out_last - out_first);
}

template <class InputIter, class RandomAccessIter,
          class RandomNumberGenerator, class Distance>
RandomAccessIter __random_sample(InputIter first, InputIter last,
                                 RandomAccessIter res,
                                 RandomNumberGenerator& rand,
                                 const Distance n) {
    Distance m = 0;
    Distance t = n;
    for ( ; first != last && m < n; ++m, ++first)
        res[m] = *first;
    
    while (first != last) {
        ++t;
        Distance M = rand(t);
        if (M < n)
            res[M] = *first;
        ++first;
    }
    return res + m;
}

template <class ForwardIter, class OutputIter, class Distance>
OutputIter random_sample_n(ForwardIter first, ForwardIter last,
                           OutputIter res, const Distance n) {
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n, remaining);
    
    while (m > 0) {
        //只有当生成的随机数小于m的时候我们才将他存入结果中
        if (random_number(remaining) < m) {
            *res = *first;
            ++res;
            --m;
        }

        --remaining;
        ++first;
    }
    return res;
}

template <class ForwardIter, class OutputIter, class Distance,
          class RandomNumberGenerator>
OutputIter random_sample_n(ForwardIter first, ForwardIter last,
                           OutputIter res, const Distance n,
                           RandomNumberGenerator& rand) {
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n, remaining);
    
    while (m > 0) {
        if (rand(remaining) < m) {
            *res = *first;
            ++res;
            --m;
        }

        --remaining;
        ++first;
    }
    return res;
}

// partition
template <class ForwardIter, class predicate>
inline ForwardIter partition(ForwardIter first,
                             ForwardIter last,
                             Predicate pred) {
    return __partiton(first, last, pred, iterator_category(first));
}

//Forward版本，核心思想是将符合条件的都交换到[begin, first)最后返回分割的位置
template <class ForwardIter, class Predicate>
ForwardIter __partiton(ForwardIter first,
                       ForwardIter last,
                       Predicate pred,
                       forward_iterator_tag) {
    if (first == last)
        return first;
    
    while (pred(*first))
        if (++first == last)
            return first;
    
    ForwardIter next = first;
    
    while (++next != last) {
        swap(*first, *next);
        ++first;
    }
    
    return first;
}

//Bidrectional版本，核心思想是从两端各找到第一个位置不对的元素，进行交换
template <class BidrectionalIter, class Predicate>
BidrectionalIter __partiton(BidrectionalIter first,
                            BidrectionalIter last,
                            Predicate pred,
                            bidrectional_iterator_tag) {
    while (true) {
        while (true) {
            if (first == last)
                return first;
            else if (pred(*first))
                ++first;
            else
                break;
        }
        --last;
        while (true) {
            if (first == last)
                return first;
            else if (!pred(*last))
                --last;
            else
                break;
        }
        iter_swap(first, last);
        ++first;
    }
}

//stable_partition
/*
template <class ForwardIter, class Predicate>
inline ForwardIter stable_partition(ForwardIter first,
                                    ForwardIter last,
                                    Predicate pred) {
    if (first == last)
        return first;
    return __stable_partition_aux(first, last, pred,
                                  value_type(first),
                                  distance_type(first));
}

template <class ForwardIter, class Predicate, class T, class Distance>
inline ForwardIter __stable_partition_aux(ForwardIter first,
                                          ForwardIter last,
                                          Predicate pred,
                                          T*, Distance*) {
    
}*/

//核心思想跟partition是一样的，左边找第一个不小于pivot的元素，右边着第一个不大于pivot的元素，互相交换
template <class RandomAccessIter, class T>
RandomAccessIter unguarded_partion(RandomAccessIter first,
                                     RandomAccessIter last,
                                     T pivot) {
    while (true) {
        while (*first < pivot)
            ++first;
        --last;
        while (pivot < *last)
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

//自定义比较函数的版本
template <class RandomAccessIter, class T, class Compare>
RandomAccessIter unguarded_partion(RandomAccessIter first,
                                     RandomAccessIter last,
                                     T pivot, Compare comp) {
    while (true) {
        while (comp(*first < pivot))
            ++first;
        --last;
        while (comp(pivot < *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

//partial_sort
template <class RandomAccessIter>
inline void partial_sort(RandomAccessIter first,
                         RandomAccessIter middle,
                         RandomAccessIter last) {
    __partiton_sort(first, middle, last, value_type(first));
}

template <class RandomAccessIter, class T>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle,
                    RandomAccessIter last, T*) {
    make_heap(first, middle);
    //使用最大堆，留下middle - first个top小的元素
    for (RandomAccessIter i = middle, i < last; ++i) {
        if (*i < *first)
            pop_heap(first, middle, i, T(*i), distance_type(first));
    }
    //对这部分元素进行堆排序
    sort_heap(first, middle);
}

//partial_sort with Compare
template <class RandomAccessIter, class Compare>
inline void partial_sort(RandomAccessIter first,
                         RandomAccessIter middle,
                         RandomAccessIter last,
                         Compare comp) {
    __partiton_sort(first, middle, last, value_type(first), comp);
}

template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle,
                    RandomAccessIter last, T*, Compare comp) {
    make_heap(first, middle, comp);
    //使用最大堆，留下middle - first个top小的元素
    for (RandomAccessIter i = middle, i < last; ++i) {
        if (comp(*i, *first))
            pop_heap(first, middle, i, T(*i), comp, distance_type(first));
    }
    //对这部分元素进行堆排序
    sort_heap(first, middle, comp);
}

//将[first, last)以升序排序,并将结果存入到[result_first, result_last).
template <class InputIter, class RandomAccessIter>
inline RandomAccessIter partial_sort_copy(InputIter first, InputIter last,
                                          RandomAccessIter result_first,
                                          RandomAccessIter result_last) {
    return __partial_sort_copy(first, last, result_first, result_last,
                               distance_type(result_first),
                               value_type(first));
}

template <class InputIter, class RandomAccessIter, class distance_type, class T>
RandomAccessIter __partial_sort_copy(InputIter first, InputIter last,
                                     RandomAccessIter result_first,
                                     RandomAccessIter result_last,
                                     Distance*, T*) {
    if (result_first == result_last)
        result result_last;
    RandomAccessIter result_real_last = result_first;
    while (first != last && result_real_last != result_last) {
        *result_real_last = *first;
        ++result_real_last;
        ++first;
    }
    
    //利用最大堆，选出top k小的元素
    make_heap(result_first, result_real_last);
    while (first != last) {
        if (*first < *result_first) {
            adjust_heap(result_first, Distance(0),
                        Distance(result_real_last - result_first),
                        T(*first));
            ++first;
        }
    }
    sort_heap(result_first, result_real_last);
    return result_real_last;
}

//以自定义比较函数的方式，将[first, last)以升序排序,并将结果存入到[result_first, result_last).
template <class InputIter, class RandomAccessIter, class Compare>
inline RandomAccessIter partial_sort_copy(InputIter first, InputIter last,
                                          RandomAccessIter result_first,
                                          RandomAccessIter result_last, Compare comp) {
    return __partial_sort_copy(first, last, result_first, result_last,
                               comp,
                               distance_type(result_first),
                               value_type(first));
}

template <class InputIter, class RandomAccessIter, class Distance, class T>
RandomAccessIter __partial_sort_copy(InputIter first, InputIter last,
                                     RandomAccessIter result_first,
                                     RandomAccessIter result_last,
                                     Compare comp, Distance*, T*) {
    if (result_first == result_last)
        result result_last;
    RandomAccessIter result_real_last = result_first;
    while (first != last && result_real_last != result_last) {
        *result_real_last = *first;
        ++result_real_last;
        ++first;
    }
    
    //利用最大堆，选出top k小的元素
    make_heap(result_first, result_real_last, comp);
    while (first != last) {
        if (comp(*first, *result_first)) {
            adjust_heap(result_first, Distance(0),
                        Distance(result_real_last - result_first),
                        T(*first),
                        comp);
            ++first;
        }
    }
    sort_heap(result_first, result_real_last, comp);
    return result_real_last;
}

//sort
const int stl_threshold = 16;

template <class RandomAccessIter>
inline void sort(RandomAccessIter first, RandomAccessIter last) {
    if (first != last) {
        introsort_loop(first, last,
                       value_type(first),
                       lg(last - first) * 2);
        final_insertiton_sort(first, last);
    }
}

template <class RandomAccessIter, class T, class Size>
void introsort_loop(RandomAccessIter first,
                      RandomAccessIter last,
                      T*, Size depth_limit) {
    while (last - first > stl_threshold) {
        if (depth_limit == 0) {//当迭代深度达到一定程度，改成partial_sort，即堆排序
            partial_sort(first, last, last);
            return;
        }
        --depth_limit;
        RandomAccessIter cut = unguarded_partion(first, last, T(median(*first,
                                                                         *(first + (last - first)/2),
                                                                         *(last - 1))));
        introsort_loop(cut, last, (T*) 0, depth_limit);//将原数组分成两部分，后半部分递归，前半部分迭代
        last = cut;
    }
}

//该算法设计的比较有意思，其实都是做插入排序，但是这边设置了个阈值16，即元素小于16的时候我还看下是不是能够直接放在首位置，
//如果可以直接就直接右移所有元素。但是当元素大于16的时候，我就直接从后往前找合适的插入位置，而不去判断和首元素的关系
template <class RandomAccessIter>
void final_insertion_sort(RandomAccessIter first,
                      RandomAccessIter last) {
    if (last - first > stl_threshold) {
        insertion_sort(first, first + stl_threshold);//将前16个插入排序
        unguarded_insertion_sort(first + stl_threshold, last);//剩下的线性插入
    } else
        insertion_sort(first, last);
}

template <class RandomAccessIter>
inline void unguarded_insertion_sort(RandomAccessIter first,
                                     RandomAccessIter last) {
    unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <class RandomAccessIter, class T>
void unguarded_insertion_sort_aux(RandomAccessIter first,
                                  RandomAccessIter last, T*) {
    for (RandomAccessIter i = first; i != last; ++i)
        unguared_linear_insert(i, T(*i));
}

//线性插入，从当前位置的前一个位置开始找第一个大于等于待插入值的位置
template <class RandomAccessIter, class T>
void unguared_linear_insert(RandomAccessIter last, T val) {
    RandomAccessIter next = last;
    --next;
    while (val < *next) {
        *last = *next;
        last = next;
    }
    *last = val;
}

template <class RandomAccessIter>
void insertion_sort(RandomAccessIter first, RandomAccessIter last) {
    if (first == last)
        return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        liner_insert(first, i, value_type(first));
}

template <class RandomAccessIter, class T>
inline void liner_insert(RandomAccessIter first,
                         RandomAccessIter last, T*) {
    T val = *last;

    if (val < *first) { // 如果要插入的元素小于首元素，直接向右移动一格，并将新元素放在首位置
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else // 否则线性插入
        unguared_linear_insert(last, val);
}

//lg辅助函数，在排序中用来求最大深度
template <class Size>
inline Size lg(Size n) {
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}

//sort with Compare
template <class RandomAccessIter, class Compare>
inline void sort(RandomAccessIter first, RandomAccessIter last,
                 Compare comp) {
    if (first != last) {
        introsort_loop(first, last,
                       value_type(first),
                       lg(last - first) * 2,
                       comp);
        final_insertiton_sort(first, last, comp);
    }
}

template <class RandomAccessIter, class T, class Size, class Compare>
void introsort_loop(RandomAccessIter first,
                      RandomAccessIter last,
                      T*, Size depth_limit, Compare comp) {
    while (last - first > stl_threshold) {
        if (depth_limit == 0) {//当迭代深度达到一定程度，改成partial_sort，即堆排序
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut = unguarded_partion(first, last, T(median(*first,
                                                                         *(first + (last - first)/2),
                                                                         *(last - 1), comp)),
                                                 comp);
        introsort_loop(cut, last, (T*) 0, depth_limit, comp);//将原数组分成两部分，后半部分递归，前半部分迭代
        last = cut;
    }
}

//该算法设计的比较有意思，其实都是做插入排序，但是这边设置了个阈值16，即元素小于16的时候我还看下是不是能够直接放在首位置，
//如果可以直接就直接右移所有元素。但是当元素大于16的时候，我就直接从后往前找合适的插入位置，而不去判断和首元素的关系
template <class RandomAccessIter, class Compare>
void final_insertion_sort(RandomAccessIter first,
                      RandomAccessIter last, Compare comp) {
    if (last - first > stl_threshold) {
        insertion_sort(first, first + stl_threshold, comp);//将前16个插入排序
        unguarded_insertion_sort(first + stl_threshold, last, comp);//剩下的线性插入
    } else
        insertion_sort(first, last, comp);
}

template <class RandomAccessIter, class Compare>
inline void unguarded_insertion_sort(RandomAccessIter first,
                                     RandomAccessIter last,
                                     Compare comp) {
    unguarded_insertion_sort_aux(first, last, value_type(first), comp);
}

template <class RandomAccessIter, class T, class Compare>
void unguarded_insertion_sort_aux(RandomAccessIter first,
                                  RandomAccessIter last, T*,
                                  Compare comp) {
    for (RandomAccessIter i = first; i != last; ++i)
        unguared_linear_insert(i, T(*i), comp);
}

//线性插入，从当前位置的前一个位置开始找第一个大于等于待插入值的位置
template <class RandomAccessIter, class T, class Compare>
void unguared_linear_insert(RandomAccessIter last, T val,
                            Compare comp) {
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
    }
    *last = val;
}

template <class RandomAccessIter, class Compare>
void insertion_sort(RandomAccessIter first, RandomAccessIter last,
                    Compare comp) {
    if (first == last)
        return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        liner_insert(first, i, value_type(first), comp);
}

template <class RandomAccessIter, class T, class Compare>
inline void liner_insert(RandomAccessIter first,
                         RandomAccessIter last, T*,
                         Compare comp) {
    T val = *last;

    if (comp(val, *first)) { // 如果要插入的元素小于首元素，直接向右移动一格，并将新元素放在首位置
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else // 否则线性插入
        unguared_linear_insert(last, val, comp);
}

// nth_element() and its auxiliary functions.
//核心思想是通过partion来计算是否已经到了k个元素
template <class RandomAccessIter>
inline void nth_element(RandomAccessIter first, RandomAccessIter nth,
                        RandomAccessIter last) {
    __nth_element(first, nth, last, value_type(first));
}

template <class RandomAccessIter, class T>
void __nth_element(RandomAccessIter first, RandomAccessIter __nth_element,
                   RandomNumberGenerator last, T*) {
    while (last - first > 3) {
        RandomAccessIter cur = unguarded_partion(first, last, T(median(*first,
                                                                       *(first + (last - first)/2),
                                                                       *(last - 1))));
        if (cut <= nth)
            first = cut;
        else
            last = cut;
    }
    insertion_sort(first, last);
}

// nth_element() with Compare
template <class RandomAccessIter, class Compare>
inline void nth_element(RandomAccessIter first, RandomAccessIter nth,
                        RandomAccessIter last, Compare comp) {
    __nth_element(first, nth, last, value_type(first), comp);
}

template <class RandomAccessIter, class T, class Compare>
void __nth_element(RandomAccessIter first, RandomAccessIter __nth_element,
                   RandomNumberGenerator last, T*, Compare comp) {
    while (last - first > 3) {
        RandomAccessIter cur = unguarded_partion(first, last, T(median(*first,
                                                                       *(first + (last - first)/2),
                                                                       *(last - 1),
                                                                       comp)),
                                                 comp);
        if (cut <= nth)
            first = cut;
        else
            last = cut;
    }
    insertion_sort(first, last, comp);
}

// Binary search (lower_bound, upper_bound, equal_range, binary_search).

//lower_bound
template <class ForwardIter, class T>
inline ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& val) {
    return __lower_bound(first, last, val, distance_type(first));
}

//虽然有二分的思想，不过如果是链表的话，效率是极低的
template <class ForwardIter, class T, class Distance>
ForwardIter __lower_bound(ForwardIter first, ForwardIter last,
                          const T& val, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < val) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

//自定义比较函数的版本
template <class ForwardIter, class T, class Compare>
inline ForwardIter lower_bound(ForwardIter first, ForwardIter last,
                               const T& val, Compare comp) {
    return __lower_bound(first, last, val, comp, distance_type(first));
}

//虽然有二分的思想，不过如果是链表的话，效率是极低的
template <class ForwardIter, class T, class Compare, class Distance>
ForwardIter __lower_bound(ForwardIter first, ForwardIter last,
                          const T& val, Compare comp, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

//upper_bound
template <class ForwardIter, class T>
inline ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& val) {
    return __upper_bound(first, last, val, distance_type(first));
}

//虽然有二分的思想，不过如果是链表的话，效率是极低的
template <class ForwardIter, class T, class Distance>
ForwardIter __upper_bound(ForwardIter first, ForwardIter last,
                          const T& val, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (val < *middle) {
            len = half;
        } else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
            
    }
    return first;
}

//自定义比较函数的版本
template <class ForwardIter, class T, class Compare>
inline ForwardIter upper_bound(ForwardIter first, ForwardIter last,
                               const T& val, Compare comp) {
    return __upper_bound(first, last, val, comp, distance_type(first));
}

//虽然有二分的思想，不过如果是链表的话，效率是极低的
template <class ForwardIter, class T, class Compare, class Distance>
ForwardIter __upper_bound(ForwardIter first, ForwardIter last,
                          const T& val, Compare comp, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(val, *middle)) {
           len = half;
        } else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

//equal_range
template <class ForwardIter, class T>
inline pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& val) {
    return __equal_range(first, last, val, distance_type(first));
}

template <class ForwardIter, class T, class Distance>
pair<ForwardIter, ForwardIter>
__equal_range(ForwardIter first, ForwardIter last, const T& val,
              Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle, left, right;
    
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < val) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else if (val < *middle)
            len = half;
        else {
            left = lower_bound(first, middle, val);
            advance(first, len);
            right = upper_bound(++middle, first, val);
            return pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}

//equal_range
template <class ForwardIter, class T, class Compare>
inline pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& val,
            Compare comp) {
    return __equal_range(first, last, val, comp, distance_type(first));
}

template <class ForwardIter, class T, class Compare, class Distance>
pair<ForwardIter, ForwardIter>
__equal_range(ForwardIter first, ForwardIter last, const T& val,
              Compare comp, Distance*) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIter middle, left, right;
    
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else if (comp(val, *middle))
            len = half;
        else {
            left = lower_bound(first, middle, val, comp);
            advance(first, len);
            right = upper_bound(++middle, first, val, comp);
            return pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}

//binary_search
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& val) {
    ForwardIter i = lower_bound(first, last, val);
    //这边需要注意lower_bound返回的是第一个位置，而val < 第一个位置的值的时候
    return i != last && !(val < *i);
}

//binary_search with Compare
template <class ForwardIter, class T, class Compare>
bool binary_search(ForwardIter first, ForwardIter last,
                   const T& val, Compare comp) {
    ForwardIter i = lower_bound(first, last, val, comp);
    //这边需要注意lower_bound返回的是第一个位置，而val < 第一个位置的值的时候
    return i != last && !(comp(val, *i));
}

//merge
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter merge(InputIter1 first1, InputIter1 last1,
                 InputIter2 first2, InputIter2 last2,
                 OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        } else {
             *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//merge with Compare
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter merge(InputIter1 first1, InputIter1 last1,
                 InputIter2 first2, InputIter2 last2,
                 OutputIter result, Compare Compare) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
             *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

/*待补充
template <class BidrectionalIter>
inline void inplace_merge(BidrectionalIter first,
                          BidrectionalIter middle,
                          BidrectionalIter last) {
    if (first == middle || middle == last)
        return;
    inplace_merge_aux(first, middle, last, value_type(first), distance_type(first));
}

template <class BidrectionalIter, class T, class Distance, class Compare>
inline void inplace_merge_aux(BidrectionalIter first,
                              BidrectionalIter middle,
                              BidrectionalIter last,
                              T*, Distance*) {
    Distance len1 = 0;
    distance(first, middle, len1);
    Distance len2 = 0;
    distance(middle, last, len2);
}*/

//set相关算法， 前提都是元素已排序
//集合[first1, last1)是否包含了集合[first2, last2)
template <class InputIter1, class InputIter2>
bool includes(InputIter1 first1, InputIter1 last1,
              InputIter2 first2, InputIter2 last2) {
    while (first1 != last1 && first2 != last2)
        if (*first2 < *first1)
            return false;
        else if (*first1 < *first2)
            ++first1;
        else
            ++first1, ++first2;
    return first2 == last2;
}

//集合[first1, last1)是否包含了集合[first2, last2)
template <class InputIter1, class InputIter2, class Compare>
bool includes(InputIter1 first1, InputIter1 last1,
              InputIter2 first2, InputIter2 last2,
              Compare comp) {
    while (first1 != last1 && first2 != last2)
        if (comp(*first2, *first1))
            return false;
        else if (comp(*first1, *first2))
            ++first1;
        else
            ++first1, ++first2;
    return first2 == last2;
}

//set_union，求两个集合的并集
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_union(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        } else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//set_union，求两个集合的并集 自定义比较函数
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_union(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//set_intersection, 求两个集合的交集
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            ++first1;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

//set_intersection, 求两个集合的交集 自定义比较函数
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            ++first1;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

//set_difference, 求两个集合的差集，即出现在了第一个里面，却没有出现在第二个集合里面的值
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}

//set_difference, 求两个集合的差集，即出现在了第一个里面，却没有出现在第二个集合里面的值, 自定义比较函数
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}

//set_symmetric_difference, 求两个集合的对称差集，即出现在了第一个里面，却没有出现在第二个集合里面的值,
//以及出现在了第二个里面，却没有出现在第一个集合里面的值
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    //此处跟别的地方first和last的顺序不太一样，按我的理解应该是无所谓的，两者里面必有一空
    return copy(first2, last2, copy(first1, last1,result));
}

//set_symmetric_difference, 求两个集合的对称差集，即出现在了第一个里面，却没有出现在第二个集合里面的值,
//以及出现在了第二个里面，却没有出现在第一个集合里面的值 自定义比较函数版本
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    //此处跟别的地方first和last的顺序不太一样，按我的理解应该是无所谓的，两者里面必有一空
    return copy(first2, last2, copy(first1, last1,result));
}

// min_element and max_element

//max_element 返回第一个最大值的迭代器
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last) {
    if (first == last)
        return first;
    ForwardIter result = first;
    while (++first != last)
        if (*result < *first)
            result = first;
    return result;
}

template <class ForwardIter, class Compare>
ForwardIter max_element(ForwardIter first, ForwardIter last,
                        Compare comp) {
    if (first == last)
        return first;
    ForwardIter result = first;
    while (++first != last)
        if (comp(*result, *first))
            result = first;
    return result;
}

//min_element 返回第一个最小值的迭代器
template <class ForwardIter>
ForwardIter min_element(ForwardIter first, ForwardIter last) {
    if (first == last)
        return first;
    ForwardIter result = first;
    while (++first != last)
        if (*first < *result)
            result = first;
    return result;
}

template <class ForwardIter, class Compare>
ForwardIter min_element(ForwardIter first, ForwardIter last,
                        Compare comp) {
    if (first == last)
        return first;
    ForwardIter result = first;
    while (++first != last)
        if (comp(*first, *result))
            result = first;
    return result;
}

// next_permutation and prev_permutation
// next_permutation
/*
1.Find the largest index k such that nums[k] < nums[k + 1]. If no such index exists, just reverse nums and done.
2.Find the largest index l > k such that nums[k] < nums[l].
3.Swap nums[k] and nums[l].
4.Reverse the sub-array nums[k + 1:].
 */
template <class BidrectionalIter>
bool next_permutation(BidrectionalIter first, BidrectionalIter last) {
    if (first == last)
        return false;
    BidrectionalIter i = first;
    ++i;
    if (i == last) //只有一个元素
        return false;
    i = last;
    --i;
    for (;;) {
        BidrectionalIter ii = i;
        --i;
        if (*i < *ii) { //从后往前寻找第一个相邻且递增的位置i
            BidrectionalIter j = last;
            while (!(*i < *--j));//从后往前寻找第一个小于i位置的位置j
            iter_swap(i, j);//交换两个位置的元素
            reverse(ii, last);//将区间[i + 1, j)元素逆序
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

// next_permutation 自定义比较函数
/*
1.Find the largest index k such that nums[k] < nums[k + 1]. If no such index exists, just reverse nums and done.
2.Find the largest index l > k such that nums[k] < nums[l].
3.Swap nums[k] and nums[l].
4.Reverse the sub-array nums[k + 1:].
 */
template <class BidrectionalIter, class Compare>
bool next_permutation(BidrectionalIter first, BidrectionalIter last,
                      Compare comp) {
    if (first == last)
        return false;
    BidrectionalIter i = first;
    ++i;
    if (i == last) //只有一个元素
        return false;
    i = last;
    --i;
    for (;;) {
        BidrectionalIter ii = i;
        --i;
        if (comp(*i, *ii)) { //从后往前寻找第一个相邻且递增的位置i
            BidrectionalIter j = last;
            while (!comp(*i, *--j));//从后往前寻找第一个小于i位置的位置j
            iter_swap(i, j);//交换两个位置的元素
            reverse(ii, last);//将区间[i + 1, j)元素逆序
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

//prev_permutation
template <class BidrectionalIter>
bool prev_permutation(BidrectionalIter first, BidrectionalIter last) {
    if (first == last)
        return false;
    BidrectionalIter i = first;
    ++i;
    if (i == last) //只有一个元素
        return false;
    i = last;
    --i;
    for (;;) {
        BidrectionalIter ii = i;
        --i;
        if (*ii < *i) { //从后往前寻找第一个相邻且递减的位置i
            BidrectionalIter j = last;
            while (!(*--j < *i));//从后往前寻找第一个大于i位置的位置j
            iter_swap(i, j);//交换两个位置的元素
            reverse(ii, last);//将区间[i + 1, j)元素逆序
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

//prev_permutation 自定义比较函数
template <class BidrectionalIter, class Compare>
bool prev_permutation(BidrectionalIter first, BidrectionalIter last,
                      Compare comp) {
    if (first == last)
        return false;
    BidrectionalIter i = first;
    ++i;
    if (i == last) //只有一个元素
        return false;
    i = last;
    --i;
    for (;;) {
        BidrectionalIter ii = i;
        --i;
        if (comp(*ii, *i)) { //从后往前寻找第一个相邻且递减的位置i
            BidrectionalIter j = last;
            while (!comp(*--j, *i));//从后往前寻找第一个大于i位置的位置j
            iter_swap(i, j);//交换两个位置的元素
            reverse(ii, last);//将区间[i + 1, j)元素逆序
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

//find_first_of 在[first2, last2)的区间内找到第一个[first1, last1)中的元素
template <class InputIter, class ForwardIter>
InputIter find_first_of(InputIter first1, InputIter last1,
                        ForwardIter first2, ForwardIter last2) {
    for ( ; first1 != last1; ++first1)
        for (ForwardIter iter = first2; iter != last2; ++iter)
            if (*first1 == *iter)
                return first1;
    return last1;
}

template <class InputIter, class ForwardIter, class Compare>
InputIter find_first_of(InputIter first1, InputIter last1,
                        ForwardIter first2, ForwardIter last2,
                        Compare comp) {
    for ( ; first1 != last1; ++first1)
        for (ForwardIter iter = first2; iter != last2; ++iter)
            if (comp(*first1, *iter))
                return first1;
    return last1;
}

// find_end,在[first1, last1)中寻找序列 [first2, last2)，返回最后一个匹配的位置
// Note that find_end for bidirectional iterators
// is much faster than for forward iterators.
template <class ForwardIter1, class ForwardIter2>
inline ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
                             ForwardIter2 first2, ForwardIter2 last2) {
    return __find_end(first1, last1, first2, last2,
                      iterator_category(first1),
                      iterator_category(first2));
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter1 __find_end(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        forward_iterator_tag, forward_iterator_tag) {
    if (first2 == last2)
        return last1;
    ForwardIter1 result = last1;
    while (1) {
        ForwardIter1 new_res = search(first1, last1, first2, last2);
        if (new_res == last1)
            return result;
        else {
            result = new_res;
            first1 = new_res;
            ++first1;
        }
    }
}

//双向的可以直接从后往前找，找到第一个就是值
template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter1 __find_end(BidrectionalIter1 first1, BidrectionalIter1 last1,
                             BidrectionalIter2 first2, BidrectionalIter2 last2,
                             bidrectional_iterator_tag, bidrectional_iterator_tag) {
    typedef reverse_iterator<BidrectionalIter1> revIter1;
    typedef reverse_iterator<BidrectionalIter2> revIter2;
    
    revIter1 rlast1(first1);
    revIter2 rlast2(first2);
    revIter1 rres = search(revIter1(last1), rlast1,
                           revIter2(last2), rlast2);
    
    if (rres == rlast1)
        return last1;
    BidrectionalIter1 result = rres.base();
    advance(result, -distance(first2, last2));
    return result;
}

// find_end,在[first1, last1)中寻找序列 [first2, last2)，返回最后一个匹配的位置 自定义比较函数
// Note that find_end for bidirectional iterators
// is much faster than for forward iterators.
template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
inline ForwardIter1 find_end(ForwardIter1 first1, ForwardIter1 last1,
                             ForwardIter2 first2, ForwardIter2 last2,
                             BinaryPredicate comp) {
    return __find_end(first1, last1, first2, last2,
                      iterator_category(first1),
                      iterator_category(first2),
                      comp);
}

template <class ForwardIter1, class ForwardIter2, class BinaryPredicate>
ForwardIter1 __find_end(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        forward_iterator_tag, forward_iterator_tag,
                        BinaryPredicate comp) {
    if (first2 == last2)
        return last1;
    ForwardIter1 result = last1;
    while (1) {
        ForwardIter1 new_res = search(first1, last1, first2, last2, comp);
        if (new_res == last1)
            return result;
        else {
            result = new_res;
            first1 = new_res;
            ++first1;
        }
    }
}

//双向的可以直接从后往前找，找到第一个就是值
template <class BidrectionalIter1, class BidrectionalIter2, class BinaryPredicate>
BidrectionalIter1 __find_end(BidrectionalIter1 first1, BidrectionalIter1 last1,
                             BidrectionalIter2 first2, BidrectionalIter2 last2,
                             bidrectional_iterator_tag, bidrectional_iterator_tag,
                             BinaryPredicate comp) {
    typedef reverse_iterator<BidrectionalIter1> revIter1;
    typedef reverse_iterator<BidrectionalIter2> revIter2;
    
    revIter1 rlast1(first1);
    revIter2 rlast2(first2);
    revIter1 rres = search(revIter1(last1), rlast1,
                           revIter2(last2), rlast2,
                           comp);
    
    if (rres == rlast1)
        return last1;
    BidrectionalIter1 result = rres.base();
    advance(result, -distance(first2, last2));
    return result;
}

// is_heap
template <class RandomAccessIter>
inline bool is_heap(RandomAccessIter first, RandomAccessIter last) {
    return __is_heap(first, last - first);
}

template <class RandomAccessIter, class Distance>
bool __is_heap(RandomAccessIter first, Distance n) {
    Distance parent = 0;
    for (Distance child = 1; chil < n; ++child) {
        if (first[parent] < first[child])
            return false;
        //此处是真的秀，秀的头皮发麻
        //作用是验证完两个孩子，层序移动父节点
        if (!(child & 1))
            parent++;
    }
    return true;
}

// is_heap 严格弱序化=>就是一定要分个大小出来
template <class RandomAccessIter, class StrictWeakOrdering>
inline bool is_heap(RandomAccessIter first, RandomAccessIter last,
                    StrictWeakOrdering comp) {
    return __is_heap(first, comp, last - first);
}

template <class RandomAccessIter, class Distance, class StrictWeakOrdering>
bool __is_heap(RandomAccessIter first, StrictWeakOrdering comp, Distance n) {
    Distance parent = 0;
    for (Distance child = 1; chil < n; ++child) {
        if (comp(first[parent], first[child]))
            return false;
        //此处是真的秀，秀的头皮发麻
        //作用是验证完两个孩子，层序移动父节点
        if (!(child & 1))
            parent++;
    }
    return true;
}

// is_sorted
template <class ForwardIter>
bool is_sorted(ForwardIter first, ForwardIter last) {
    if (first == last)
        return true;
    ForwardIter next = first;
    for (++next; next != las; first = next, ++next) {
        if (*next < *first)
            return false;
    }
    return true;
}

// is_sorted
template <class ForwardIter, class StrictWeakOrdering>
bool is_sorted(ForwardIter first, ForwardIter last, StrictWeakOrdering comp) {
    if (first == last)
        return true;
    ForwardIter next = first;
    for (++next; next != las; first = next, ++next) {
        if (comp(*next, *first))
            return false;
    }
    return true;
}
}
#endif
