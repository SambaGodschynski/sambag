/*
 * PlacementAlloc.hpp
 * Helper for boost interprocess.
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_PLACEMENT_ALLOC_H
#define SAMBAG_PLACEMENT_ALLOC_H

#include <cstddef>
#include <sambag/com/exceptions/IllegalStateException.hpp>

namespace sambag {  namespace com { namespace interprocess {

/**
 * iterates over raw memory.
 */
 
class PointerIterator {
    void * ptr;
    size_t size;
public:
    PointerIterator(void *ptr, size_t bytes) : ptr(ptr), size(bytes) {}
    PointerIterator() : ptr(NULL), size(0) {}
    void * operator*() {
        return ptr;
    }
    void setPointer(void *ptr, size_t byteSize) {
        this->ptr=ptr;
        this->size = byteSize;
    }
    template <class T>
    void next(size_t num=1) {
        if (ptr==NULL) {
            SAMBAG_THROW(
                exceptions::IllegalStateException,
                "PointerIterator==NULL"
            );
        }
        T * x = (T*)ptr;
        ptr = x + num;
        size-=sizeof(T)*num;
    }
    size_t bytesLeft() const {
        return size;
    }
};
/**
 * Allocator doesn't alloc anything but assign given allready allocated memory.
 * @note when PointerIterator is going invalid, PlacementAlloc is invalid.
 */
template <class T>
struct PlacementAlloc {
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template <class U>
    struct rebind {
        typedef PlacementAlloc<U> other;
    };
    PointerIterator &it;
    T * allocate(size_t size) {
        if (size>max_size()) {
            SAMBAG_THROW(
                exceptions::IllegalStateException,
                "PlacementAlloc out of bounds."
            );
        }
        T * res = (T*) *it;
        it.next<T>(size);
        return res;
    }
    void deallocate(T* to_dealloc, size_t size) {}
    PlacementAlloc(PointerIterator &ptr_it) : it(ptr_it) {}
    template <class U>
    PlacementAlloc( const PlacementAlloc<U> &other ) : it(other.it) {}
    void construct (pointer p, const_reference val) {
        new (p) T(val);
    }
    void destroy (pointer p) {
        p->~T();
    }
    size_type max_size() const throw() {
        return it.bytesLeft() / sizeof(T);
    }
};

}}}

#endif // SAMBAG_PLACEMENT_ALLOC_H