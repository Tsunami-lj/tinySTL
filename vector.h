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

    void push_back(const T& x) {
        if (finish != end_of_storage) {
            construct(finish, x);
            ++finish;
        } else
            insert_aux(end(), x);
    }
    
    void pop_back() {
        --finish;
        destroy(finish);
    }

    // 返回值为finish的位置
    iterator insert(iterator position, const T& x) {
        szie_type n = position - begin();
        if (finish != end_of_storage && position == end()) {
            construct(finish, x);
            ++finish;
        } else
            insert_aux(position, x);
        return begin() + n;
    }

    iterator insert(iterator position) {
        return insert(position, T());
    }
    
    void insert(iterator pos, size_type n, const T& x);
    void insert(iterator pos, int n, const T& x) {
        insert(pos, (szie_type) n, x);
    }
    void insert(iterator pos, long n, const T& x) {
        insert(pos, (size_type) n, x);
    }
    
    //返回值为删除的迭代器位置，但是需要注意的是从position开始的所有的迭代器已经失效了
    iterator erase(iterator position) {
        if (position + 1 != end())
            copy(position + 1, finish, position);
        --finish;
        destroy(finish);
        return position;
    }
    
    iterator erase(iterator first, iterator last) {
        iterator i = copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }
    
    void swap(vector<T, Alloc>& x) {
        swap(start, x.start);
        swap(finish, x.finish);
        swap(end_of_storage, x.end_of_storage);
    }
    
    void reverse(size_type n) {
        if (capacity < n) {
            const size_type old_size = size();
            iterator tmp = allocate_and_copy(n, start, finish);
            destroy(start, finish);
            start = tmp;
            finish = tmp + old_size;
            end_of_storage = start + n;
        }
    }

    void resize(size_type new_size) {
        return resize(new_size, T());
    }

    void resize(size_type new_size, const T& x) {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }

    void clear() {
        erase(begin(), end());
    }

}

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
    if (finish != end_of_storage) {
        //先将finish位置构造一个对象
        construct(finish, *(finish - 1));
        ++finish;
        T x_copy = x;
        //将[postion,finish)的元素往后挪一格
        copy_backward(position, finish - 2, finish - 1);
        *position = x_copy;
    } else {
        const size_type old_size = size();
        const size_type len = old_size != 0 : 2 * old_size : 1; //非0以两倍扩张，否则置为1
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        
        new_finish = uninitialized_copy(start, position, new_start);
        construct(new_finish, x);
        ++new_finish;
        new_finish = uninitialized_copy(position, finish, new_finish);
        
        destroy(begin(), end());
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

template <class T, class Alloc>
inline bool operator ==(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <class T, class Alloc>
inline bool operator <(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& x) {
    if (&x != this) {
        if (x.size() > capacity()) { //当前分配的内存不够，需要重新分配内存，并销毁当前内存
            iterator tmp = allocate_and_copy(x.end() - x.begin(), x.begin(), x.end());
            destroy(start, finish);
            deallocate();
            start = tmp;
            end_of_storage = start + x.size();
        } else if (size() >= x.size()) { //当前已用内存都够大时，需要将多出的空间元素destroy
            iterator i = copy(x.begin(), x.end(), start);
            destroy(i, finish);
        } else { //当前已用内存太小，但是分配内存足够,需要注意的是这些未使用已申请的内存需要用uninitialized_copy，这里面会调用构造函数，而普通的copy不会
            copy(x.begin(), x.begin() + size(), start);
            uninitialized_copy(x.begin() + size(), x.end(), finish);
        }
        finish = start + x.size();
    }
    return *this;
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
    if (n != 0) {
        //之前申请的剩余空间足够的情况下
        if (size_type(end_of_storage - finish) >= n) {
            T x_copy = x;
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            if (elems_after > n) {
                //先将[finish - n, finish)元素填到[finish, finish + n)
                //针对未初始化过的位置要调用构造函数，不能直接用copy
                uninitialized_copy(finish - n, finish, finish);
                finish += n;
                //再将[position, finish - n)的元素填到[xxx, finish)的位置上
                copy_backward(position, old_finish - n, old_finish);
                //最后将[postiton, position + n)填满x
                fill(position, position + n, x_copy);
            } else {
                //将[finish, finish + n - elms_after)填充为x
                uninitialized_fill_n(finish, n - elems_after, x_copy);
                finish += n - elems_after;
                //将[position, old_finish)的元素直接拷贝到finish + n - elems_after起始的位置
                uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                //最后将[postiton, old_finish)填满x
                fill(position, old_finish, x_copy);
                
            }
        }
    } else {
        const size_type old_size = size();
        const size_type len = old_size + max(old_size, n); //要么两倍扩张，要么原大小+n扩张
        iterator new_start = data_allocator::data_allocator(len);
        iterator new_finish = new_start;
        //将[start, position)拷贝到新起点
        new_finish = uninitialized_copy(start, position, new_start);
        //填充n个x
        new_finish = uninitialized_fill_n(new_finish, n, x);
        //将[position, old_finish)拷贝到当前末尾
        new_finish = uninitialized_copy(position, finish, new_finish);
        //释放原来的内存[start, finish)
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position,
                              const_iterator first,
                              const_iterator last) {
    if (first != last) {
        size_type n = 0;
        distance(first, last, n);
        if (size_type(end_of_storage - finish) >= n) {
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            if (elms_after > n) {
                uninitialized_copy(finish - n, finish, finish);
                finish += n;
                copy_backward(position, old_finish - n, old_finish);
                copy(first, last, position);
            } else {
                //现将[first,last)中超出的元素尾巴部分构造好
                uninitialized_copy(first + elems_after, last, finish);
                finish += n - elems_after;
                //将[position, old_finish)拷贝到刚才更新完的尾部
                uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                //最后将[first, last)区间内刚才的前半段拷贝过来
                copy(first, first + elems_after, position);
            }
        } else {
            const size_type old_size = size();
            const size_type len = old_size + max(old_size, n);
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            //将[start, position)拷贝到新起点
            new_finish = uninitialized_copy(start, position, new_start);
            //将[first, last)拷贝到新终点
            new_finish = uninitialized_copy(first, last, new_finish);
            //将[position, old_finish)拷贝到当前末尾
            new_finish = uninitialized_copy(position, finish, new_finish);
            destroy(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
}
}
#endif
