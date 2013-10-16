/*
 * Interprocess.hpp
 * Helper for boost interprocess.
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_INTERPROCESS_H
#define SAMBAG_INTERPROCESS_H

/*
 according to: http://stackoverflow.com/questions/11915142/64bit-and-32bit-process-intercommunication-boostmessage-queue#15171596
*/
/*namespace boost { namespace interprocess { namespace ipcdetail {
    //Rounds "orig_size" by excess to round_to bytes
    template<class SizeType, class ST2>
    inline SizeType get_rounded_size(SizeType orig_size, ST2 round_to) {
        return ((orig_size-1)/round_to+1)*round_to;
    }
}}}*/

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
#include <boost/interprocess/mem_algo/simple_seq_fit.hpp>
#include <boost/unordered_map.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <utility>
#include <boost/static_assert.hpp>

namespace sambag {  namespace com { namespace interprocess {
/*typedef bi::basic_managed_shared_memory <char,
    bi::simple_seq_fit<bi::mutex_family, OffsetPtr<void>::Class >,
    bi::iset_index> ManagedSharedMemory;*/
typedef boost::interprocess::managed_shared_memory ManagedSharedMemory;

//-----------------------------------------------------------------------------
/**
 * Releases shared memory when last instance is gone.
 */
class SharedMemoryHolder {
    ManagedSharedMemory *shm;
    SharedMemoryHolder(const SharedMemoryHolder&) {}
    std::string name;
    void initMemory(size_t size, size_t tried = 0);
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
template <class _ManagedSharedMemory>
struct GetSegmentManager {
    typedef typename _ManagedSharedMemory::segment_manager Value;
};

//-----------------------------------------------------------------------------
template <typename T>
struct Allocator {
    typedef boost::interprocess::allocator<T, GetSegmentManager<ManagedSharedMemory>::Value> Class;
};
//-----------------------------------------------------------------------------
template <typename T>
struct Vector {
    typedef T ValueType;
    typedef typename Allocator<ValueType>::Class TheAllocator;
    typedef boost::interprocess::vector<T, TheAllocator> Class;
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
    typedef boost::interprocess::set<T, TheComparator, TheAllocator> Class;
    static Class * findOrCreate(const char * name, ManagedSharedMemory &shm)
    {
        TheAllocator alloc(shm.get_segment_manager());
        return shm.find_or_construct< Class >(name)(TheComparator(), alloc);
    }
};
//-----------------------------------------------------------------------------
struct String {
    typedef Allocator<char>::Class TheAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, TheAllocator> Class;
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
    typedef boost::interprocess::map<K, V, TheComparator, TheAllocator> Class;
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
    typedef typename boost::interprocess::managed_shared_ptr<T, ManagedSharedMemory>::type Class;
    static Class create(const char * name, ManagedSharedMemory &shm) {
        return boost::interprocess::make_managed_shared_ptr( shm.construct<T>(name)(), shm);
    }
};

//-----------------------------------------------------------------------------
template <typename T>
struct WeakPtr {
    typedef T ValueType;
    typedef typename boost::interprocess::managed_weak_ptr<T, ManagedSharedMemory>::type Class;
};

}}} // namespace(s)

#endif /* SAMBAG_INTERPROCESS_H */
