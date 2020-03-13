#ifndef MYSTL_ITERATOR_H_
#define MYSTL_ITERATOR_H_

#include "type_traits"
#include <cstddef>

namespace mystl {

// 五种迭代器类型
struct input_ietrator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_ietrator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

//迭代器模板
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

// 通过迭代器萃取对应类型
//泛化版本
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iteratoe::reference            reference;
};

//偏特化，主要是对指针和常指针
template <class T>
struct iterator_traits <T*> {
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef T*                             pointer;
    typedef T&                             reference;
};

template <class T>
struct iterator_traits <const T*> {
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef const T*                       pointer;
    typedef const T&                       reference;
};

//获取迭代器类别
template <class Iter>
inline typename iterator_traits<Iter>::iterator_category
__iteraor_category(const Iter&)
{
    typedef typename iterator_traits<Iter>::iterator_category Category;
    return Category();
}



}
