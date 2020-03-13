#ifndef MYSTL_ALLOC_H_
#define MYSTL_ALLOC_H_

#include <cstddef>
namespace mystl {
const int ALIGN = 8;
const int NFREELISTS = 16;
const int MAX_BYTES = 128;

const int DEFAULT_BLOCKS = 20;
class Alloc {
private:
    //向上调整到8的倍数
    static size_t ROUND_UP(size_t bytes) {
        return (((bytes) + ALIGN - 1) & ~(ALIGN - 1));
    }
private:
    // 嵌入式指针，指向下块空闲内存j块
    union obj {
        union obj* free_list_link;
    };

private:
    static obj* volatile free_list [NFREELISTS];
    
    //通过字节数获取freelist链表索引
    static size_t FREELIST_INDEX(size_t bytes) {
        return ((bytes) + ALIGN - 1) / ALIGN - 1;
    }
    
    //pool首指针，尾指针，以及当前分配总量
    static char* start_free;
    static char* end_free;
    static size_t heap_size;
    
public:
    static void * allocate(size_t n) {
        obj* volatile *my_free_list;
        obj* result;
        
        //超过最大块直接调用malloc
        if (n > (size_t)MAX_BYTES) {
            return (std::malloc(n));
        }
        
        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        //pool不够 需要补充内存
        if (!result) {
            void * r = refill(ROUND_UP(n));
            return r;
        }
        *my_free_list = result->free_list_link;
        return (result);
    }
    
    static void deallocate (void * p, size_t n) {
        obj* volatile *my_free_list;
        obj* res;
	obj* q = (obj*)p;
        
        if (n > (size_t)MAX_BYTES) {
            std::free(p);
            return ;
        }
        //头插法，将还回的内存插入
        my_free_list = free_list + FREELIST_INDEX(n);
        res = *my_free_list;
        q->free_list_link = res;
        *my_free_list = q;
    }
    
    static char* chunk_alloc(size_t size, int &nobjs) {
        char* result;
        size_t total_bytes = size * nobjs;
        size_t remain_bytes = end_free - start_free;
        
        if (remain_bytes >= total_bytes) { //尝试分配20个
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if (remain_bytes >= size) { //尝试至少分配一个
            result = start_free;
            nobjs = remain_bytes / size;
            total_bytes = nobjs * size;
            start_free += total_bytes;
            return result;
        } else { //一个都不够
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            //先处理碎块
            if (remain_bytes > 0) {
                obj* volatile *my_free_list = free_list + FREELIST_INDEX(remain_bytes);
                ((obj*)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            start_free = (char*)std::malloc(bytes_to_get);
            if (!start_free) {
                obj * volatile * my_free_list;
                obj * temp;
                for (int i = size + ALIGN; i < MAX_BYTES; i += ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    temp = *my_free_list;
                    if (temp) {
                        *my_free_list = temp->free_list_link;
                        start_free = (char *)temp;
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = NULL;
                //山穷水尽，需要throw exception了
                return NULL;
            }
            end_free = start_free + bytes_to_get;
            heap_size += bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }
    static void* refill(size_t n) {
        int nobjs = DEFAULT_BLOCKS;
        char* chunk = chunk_alloc(n, nobjs);
        obj* volatile *my_free_list;
        obj* result;
        obj* current_obj;
        obj* next_obj;
        
        if (nobjs == 1)
            return chunk;
        my_free_list = free_list + FREELIST_INDEX(n);
        result = (obj *)chunk;
        *my_free_list = next_obj = (obj *) (chunk + n);
        for (int i = 1; ; i++) {
            current_obj = next_obj;
            next_obj = (obj *)((char *)next_obj + n);
            if (i == nobjs - 1) {
                current_obj->free_list_link = NULL;
                break;
            } else {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }
};
char* Alloc::start_free = NULL;
char* Alloc::end_free = NULL;
size_t Alloc::heap_size = 0;
}

template <class T, class Alloc>
class simple_alloc {
	static T* allocate(size_t n) {
		return !n ? NULL : (T*)Alloc::allocate(n * sizeof(T)); 
	}
	static T* allocate(void) {
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n) {
		if (n)
			Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T *p) {
		Alloc::deallocate(p, sizeof(T));
	}
};
#endif
