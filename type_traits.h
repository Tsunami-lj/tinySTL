#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

//use standard type traits to get the type
//#include <type_traits>

struct __true_type {
};

struct __false_type {
};

//泛化
template <class T>
struct __type_traits {
    typedef __true_type     this_dummy_member_must_be_first;
    typedef __false_type    has_trival_default_constructor;
    typedef __false_type    has_trivial_copy_constructor;
    typedef __false_type    has_trival_assignment_operator;
    typedef __false_type    has_trival_destructor;
    typedef __false_type    is_POD_type; // plain old data : 就是struct 没有function 只有data
};

//特化
template<> struct __type_traits<char> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
}

template<>  struct __type_traits<signed char> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned char> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<short> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned short> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<int> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned int> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<long> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned long> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<floa> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

template<> struct __type_traits<double> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};


template<> struct __type_traits<long double> {
    typedef __true_type    has_trival_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trival_assignment_operator;
    typedef __true_type    has_trival_destructor;
    typedef __true_type    is_POD_type;
};

//指针偏特化
template <class T>
struct __type_traits<T*> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};
