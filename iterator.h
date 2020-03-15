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

//input
template <class T, class Distance>
struct input_ietrator {
    typedef input_ietrator_tag  iterator_category;
    typedef T                   value_type;
    typedef Distance            difference_type;
    typedef T*                  pointer;
    typedef T&                  reference;
};

//output
struct output_ietrator {
    typedef output_ietrator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    difference_type;
    typedef void                    pointer;
    typedef void                    reference;
};

//forward
template <class T, class Distance>
struct forward_ietrator {
    typedef forward_ietrator_tag    iterator_category;
    typedef T                       value_type;
    typedef Distance                difference_type;
    typedef T*                      pointer;
    typedef T&                      reference;
};

//bidirectional
template <class T, class Distance>
struct bidirectional_ietrator {
    typedef bidirectional_ietrator_tag      iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

//random_access
template <class T, class Distance>
struct random_access_ietrator {
    typedef random_access_ietrator_tag      iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

//iterator
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

//获取迭代器category
template <class Iter>
inline typename iterator_traits<Iter>::iterator_category
iteraor_category(const Iter&)
{
    typedef typename iterator_traits<Iter>::iterator_category Category;
    return Category();
}

//获取迭代器 distance type
template <class Iter>
inline typename iterator_traits<Iter>::difference_type *
distance_type(const Iter&)
{
    return static_cast<typename iterator_traits<Iter>::difference_type *>(0);
}

//获取迭代器 value type
template<class Iter>
inline typename iterator_traits<Iter>::value_type *
value_type(const Iter&)
{
    return static_cast<typename iterator_traits<Iter>::value_type *>(0);
}

}
#endif
