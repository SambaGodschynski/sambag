/*
 * Interprocess.hpp
 * Helper for boost interprocess.
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_INTERPROCESS_H
#define SAMBAG_INTERPROCESS_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/set.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/weak_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>
#include <utility>
#include <boost/integer.hpp>
#include <boost/static_assert.hpp>

namespace sambag {  namespace com { namespace interprocess {

//-----------------------------------------------------------------------------
/**
 * Common types for 32/64 bit
 */
typedef boost::int_t<32>::exact Integer;
typedef boost::uint_t<32>::exact UInteger;


namespace bi = ::boost::interprocess;
typedef bi::managed_shared_memory ManagedSharedMemory;
//-----------------------------------------------------------------------------
/**
 * Releases shared memory when last instance is gone.
 */
class SharedMemoryHolder {
    BOOST_STATIC_ASSERT(sizeof(Integer) == 4);
    BOOST_STATIC_ASSERT(sizeof(UInteger) == 4);
    ManagedSharedMemory *shm;
    SharedMemoryHolder(const SharedMemoryHolder&) {}
    std::string name;
    void initMemory(size_t size, int tried = 0);
    Integer *ref_counter;
public:
    static const std::string NAME_REF_COUNTER;
    SharedMemoryHolder(const char *name, size_t size);
    SharedMemoryHolder() {}
    ~SharedMemoryHolder();
    ManagedSharedMemory & get() { return *shm; }
    const ManagedSharedMemory & get() const { return *shm; }
};
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
template <class _ManagedSharedMemory>
struct GetSegmentManager {
    typedef typename _ManagedSharedMemory::segment_manager Value;
};

//-----------------------------------------------------------------------------
template <typename T>
struct Allocator {
    typedef bi::allocator<T, GetSegmentManager<ManagedSharedMemory>::Value> Class;
};
//-----------------------------------------------------------------------------
template <typename T>
struct Vector {
    typedef T ValueType;
    typedef typename Allocator<ValueType>::Class TheAllocator;
    typedef bi::vector<T, TheAllocator> Class;
    static Class * findOrCreate(const char * name, ManagedSharedMemory &shm)
    {
        TheAllocator alloc(shm.get_segment_manager());
        return shm.find_or_construct< Class >(name)(alloc);
    }
};
//-----------------------------------------------------------------------------
template <typename T,
    template <class> class Comparator=std::less 
>
struct Set {
    typedef T ValueType;
    typedef typename Allocator<ValueType>::Class TheAllocator;
    typedef Comparator<T> TheComparator;
    typedef bi::set<T, TheComparator, TheAllocator> Class;
    static Class * findOrCreate(const char * name, ManagedSharedMemory &shm)
    {
        TheAllocator alloc(shm.get_segment_manager());
        return shm.find_or_construct< Class >(name)(TheComparator(), alloc);
    }
};
//-----------------------------------------------------------------------------
struct String {
    typedef Allocator<char>::Class TheAllocator;
    typedef bi::basic_string<char, std::char_traits<char>, TheAllocator> Class;
    static Class * findOrCreate(const char * name, ManagedSharedMemory &shm)
    {
        TheAllocator alloc(shm.get_segment_manager());
        return shm.find_or_construct< Class >(name)(alloc);
    }
};
//-----------------------------------------------------------------------------
template <typename K,
    typename V,
    template <class> class Comparator=std::less >
struct Map {
    typedef std::pair<const K, V> ValueType;
    typedef typename Allocator<ValueType>::Class TheAllocator;
    typedef Comparator<K> TheComparator;
    typedef bi::map<K, V, TheComparator, TheAllocator> Class;
    static Class * findOrCreate(const char * name, ManagedSharedMemory &shm)
    {
        TheAllocator alloc(shm.get_segment_manager());
        return shm.find_or_construct< Class >(name)(TheComparator(), alloc);
    }

};

//-----------------------------------------------------------------------------
template <typename T>
struct SharedPtr {
    typedef T ValueType;
    typedef typename bi::managed_shared_ptr<T, ManagedSharedMemory>::type Class;
    static Class create(const char * name, ManagedSharedMemory &shm) {
        return bi::make_managed_shared_ptr( shm.construct<T>(name)(), shm);
    }
};

//-----------------------------------------------------------------------------
template <typename T>
struct WeakPtr {
    typedef T ValueType;
    typedef typename bi::managed_weak_ptr<T, ManagedSharedMemory>::type Class;
};

}}} // namespace(s)

#endif /* SAMBAG_INTERPROCESS_H */
