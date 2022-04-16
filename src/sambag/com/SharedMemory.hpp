/*
* SharedMemory.hpp
 * Helper for boost interprocess.
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SHARED_MEMORY_H
#define SAMBAG_SHARED_MEMORY_H


/**
 * get shm in windows is slightly different to the default way.
 */
#ifdef SAMBAG_USE_WINDOWS_SHARED_MEMORY
namespace boost { namespace interprocess {
	class windows_shared_memory;
	class mapped_region;
}}
namespace sambag {  namespace com { namespace interprocess {
	typedef boost::interprocess::windows_shared_memory SharedMemoryObject;
}}}
#define SAMBAG_SHARED_MEMORY_OBJECT_CREATE(createmode, name, rw_mode, size) \
	sambag::com::interprocess::SharedMemoryObject((createmode), (name), (rw_mode), (size))
#define SAMBAG_SHARED_MEMORY_TRUNC(shm, size)
#define SAMBAG_SHARED_MEMORY_REMOVE(name)
#else
namespace boost { namespace interprocess {
    class shared_memory_object;
}}
namespace sambag {  namespace com { namespace interprocess {
	typedef class boost::interprocess::shared_memory_object SharedMemoryObject;
}}}
#define SAMBAG_SHARED_MEMORY_OBJECT_CREATE(createmode, name, rw_mode, size) \
	 sambag::com::interprocess::SharedMemoryObject((createmode), (name), (rw_mode))
#define SAMBAG_SHARED_MEMORY_TRUNC(shm, size) \
	(shm).truncate((size))
#define SAMBAG_SHARED_MEMORY_REMOVE(name) \
	sambag::com::interprocess::SharedMemoryObject::remove((name))
#endif

#include <boost/interprocess/offset_ptr.hpp>
#include <boost/integer.hpp>

//-----------------------------------------------------------------------------
/**
 * Common types for 32/64 bit
 */
namespace sambag {  namespace com { namespace interprocess {
	typedef  boost::interprocess::mapped_region MappedRegion;
	typedef boost::int_t<32>::exact Integer;
	typedef boost::uint_t<32>::exact UInteger;
	BOOST_STATIC_ASSERT(sizeof(Integer) == 4);
    BOOST_STATIC_ASSERT(sizeof(UInteger) == 4);
	/**
	 * Offset ptr which is usable for 32 and 64 bit. 
	 */
	template <class T>
	struct OffsetPtr {
		typedef boost::interprocess::offset_ptr<T, Integer, UInteger> Class;
	};
}}}

#endif //SAMBAG_SHARED_MEMORY_H
