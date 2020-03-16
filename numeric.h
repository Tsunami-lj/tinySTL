#ifndef MYSTL_NUMERIC_H_
#define MYSTL_NUMERIC_H_
namespace mystl {

//对元素求和
template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
    for ( ; first != last; ++first)
        init = init + *first;
    return init;
}

//对元素按指定操作符计算
template <class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
    for ( ; first != last; ++first)
        init = binary_op(init, *first);
    return init;
}

//区间内积
template <class InputIterator1, class InputIterator2, class T>
T innner_product(InputIterator1 first1, InputIterator1 last1,
                 InputIterator2 first2, T init) {
    for ( ; first1 != last1; ++first1, ++first2)
        init = init + (*first1 * *first2);
    return init;
}

//指定区间运算符
template <class InputIterator1, class InputIterator2, class T,
          class BinaryOperation1, class BinaryOperation2>
T innner_product(InputIterator1 first1, InputIterator1 last1,
                 InputIterator2 first2, T init,
                 BinaryOperation1 binary_op1,
                 BinaryOperation2 binary_op2) {
    for ( ; first1 != last1; ++first1, ++ first2)
        init = binary_op1(init, binary_op2(*first1, *first2));
    return int;
}

//部分求和
template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last)
        return result;
    *result = *first;
    return __partial_sum(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*) {
    T value = *first;
    while (++first != last) {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}

//指定部分求和运算符
template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op) {
    if (first == last)
        return result;
    *result = *first;
    return __partial_sum(first, last, result, value_type(first), binary_op);
}

template <class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
    T value = *first;
    while (++first != last) {
        value = binary(value, *first);
        *++result = value;
    }
    return ++result;
}

//相邻元素前后做差
template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference (InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last)
        return result;
    *result = *first;
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    value_type value = *first;
    while (++first != last) {
        value_type tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
    return ++result;
}

//通过指定运算符对相邻元素前后做差
template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference (InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if (first == last)
        return result;
    *result = *first;
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    value_type value = *first;
    while (++first != last) {
        value_type tmp = *first;
        *++result = binary_op(tmp, value);
        value = tmp;
    }
    return ++result;
}

//填充连续数
template <class ForwardIterator, class T>
void itoa(ForwardIterator first, ForwardIterator last, T value) {
    while (first != last) {
        *++first = value++;
    }
}
}
#endif
