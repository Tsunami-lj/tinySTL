#ifndef MYSTL_VECTOR_H_
#define MYSTL_VECTOR_H_

namespace mystl {

template <class T, class Alloc = alloc>
class vector {
public:
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    
    typedef reverse_iterator<const_iterator>    const_reverse_iterator;
    typedef reverse_iterator<iterator>          reverse_iterator;
    typedef reverse_iterator<const_iterator, value_type, const_reference,
                             difference_type>   const_reference_iterator;
    typedef reverse_iterator<iterator, value_type, reference, difference_type> reverse_iterator;
    
protected:
    typedef alloc data_allocator; //定义内存分配器
    iterator start;  //当前使用内存d起点
    iterator finish; //当前使用内存终点
    iterator end_of_storage;//当前可用内存终点
    void insert_aux(iterator position, const T& x);
    void deallocate() {
        if (start)
            data_allocator::deallocate(start, end_of_storage - start);
    }
    
    //初始化，分配内存，并插入n个值为value的元素
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }
    
    iterator allocate_and_copy(size_type n, const_iterator first, const_iterator last) {
        iterator result = data_allocator::allocate(n);
        uninitialized_copy(first, last, result);
        return result;
    }
public:
    //构造函数
    vector() : start(0), finish(0), end_of_storage(0) {}

    vector(size_type n, const T& value) {
        fill_initialize(n, value);
    }

    vector(int n, const T& value) {
        fill_initialize(n, value);
    }

    vector(long n, const T& value) {
        fill_initialize(n, value);
    }

    explicit vector(size_type n) {
        fill_initialize(n, T());
    }

    //拷贝构造函数
    vector(const vector<T, Alloc>& x) {
        start = allocate_and_copy(x.end() - x.begin(), x.begin(), x.end());
        finish = start + (x.end() - x.begin());
        end_of_storage = finish;
    }
    
    //使用两个迭代器之间的元素初始化
    vector(const_iterator first, const_iterator last) {
        size_type n = 0;
        distance(first, last, n);
        start = allocate_and_copy(n, first, last);
        finish = start + n;
        end_of_storage = finish;
    }
    
    //拷贝赋值函数
    vector<T, Alloc>& operator=(const vector<T, Alloc>& x);
    
    //析构函数
    ～vector() {
        destroy(start, finish);
        deallocate();
    }
public:
    iterator begin() {
        return start;
    }
    
    const_iterator begin() const {
        return start;
    }
    
    iterator end() {
        return finish;
    }
    
    const_iterator end() const {
        return finish;
    }
    
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }
    
    size_type size() const {
        retrn size_type(end() - begin());
    }
    
    szie_type max_size() const_reverse_iterator {
        return size_type(-1) / sizeof(T);
    }
    
    size_type capacity() const {
        return size_type(end_of_storage - begin());
    }
    
    bool empty() const {
        return begin() == end();
    }
    
    reference operator[](size_type n) {
        return *(begin() + n);
    }
    
    reference at(size_type n) {
        return (*this)[n];
    }
    
    reference front() {
        return *begin();
    }
    
    const_reference front() const {
        return *begin();
    }
    
    reference back() {
        return *(end() - 1);
    }

    const_reference back() const {
        return *(end() - 1);
    }
    
    
}


}
#endif
