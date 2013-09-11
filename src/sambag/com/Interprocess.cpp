/*
 * Interprocess.cpp
 *
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#include "Interprocess.hpp"

namespace sambag {  namespace com { namespace interprocess {
//=============================================================================
// class SharedMemoryHolder 
//=============================================================================
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
}
//-----------------------------------------------------------------------------
SharedMemoryHolder::SharedMemoryHolder(const char *name, size_t size) : name(name)
{
    initMemory(size);
}
//-----------------------------------------------------------------------------
SharedMemoryHolder::~SharedMemoryHolder() {
    bi::shared_memory_object::remove(name.c_str());
}
}}} // namespace(s)
