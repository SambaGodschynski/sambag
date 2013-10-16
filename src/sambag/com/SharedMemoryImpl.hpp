/*
* SharedMemory.hpp
 * Helper for boost interprocess.
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SHARED_MEMORY_IMPL_H
#define SAMBAG_SHARED_MEMORY_IMPL_H


/**
 * get shm in windows is slightly different to the default way.
 */
#ifdef WIN32
	#include <boost/interprocess/windows_shared_memory.hpp>
#else
	#include <boost/interprocess/shared_memory_object.hpp>
#endif

#include <boost/interprocess/mapped_region.hpp>

#endif //SAMBAG_SHARED_MEMORY_IMPL_H
