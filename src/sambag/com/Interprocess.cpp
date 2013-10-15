/*
 * Interprocess.cpp
 *
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#include "Interprocess.hpp"
#include <boost/interprocess/mapped_region.hpp>
#include <sambag/com/Exception.hpp>

namespace sambag {  namespace com { namespace interprocess {
//=============================================================================
// class SharedMemoryHolder 
//=============================================================================
//-----------------------------------------------------------------------------
const std::string SharedMemoryHolder::NAME_REF_COUNTER = "ref_counter";
//-----------------------------------------------------------------------------
void SharedMemoryHolder::initMemory(size_t size, int tried) {
    try {
        shm = ManagedSharedMemory(bi::open_or_create, name.c_str(), size);
    } catch (...) {
        if (tried>0) {
            throw;
        }
        bi::shared_memory_object::remove(name.c_str());
        initMemory(size, tried+1);
    }
    ref_counter = get().find_or_construct<int>(NAME_REF_COUNTER.c_str())(0);
    ++(*ref_counter);
}
//-----------------------------------------------------------------------------
SharedMemoryHolder::SharedMemoryHolder(const char *name, size_t size) : name(name)
{
    initMemory(size);
}
//-----------------------------------------------------------------------------
void SharedMemoryHolder::initMemory(const char *name, size_t size) {
    this->name = std::string(name);
    initMemory(size);
}
//-----------------------------------------------------------------------------
SharedMemoryHolder::~SharedMemoryHolder() {
    if (--(*ref_counter) == 0) {
        bi::shared_memory_object::remove(name.c_str());
    }
}
}}} // namespace(s)
