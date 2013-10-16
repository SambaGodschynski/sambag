/*
 * Interprocess.cpp
 *
 *  Created on: Wed Sep 11 14:18:18 2013
 *      Author: Johannes Unger
 */

#include "Interprocess.hpp"
#include <boost/interprocess/mapped_region.hpp>
#include <sambag/com/Exception.hpp>
#include <sambag/com/Common.hpp>

namespace sambag {  namespace com { namespace interprocess {
//=============================================================================
// class SharedMemoryHolder 
//=============================================================================
//-----------------------------------------------------------------------------
const std::string SharedMemoryHolder::NAME_REF_COUNTER = "ref_counter";
//-----------------------------------------------------------------------------
void SharedMemoryHolder::initMemory(size_t size, size_t tried) {
    try {
        SAMBAG_LOG_INFO<<"alloc "<<name<<": ...";
        shm = new ManagedSharedMemory(boost::interprocess::open_or_create, name.c_str(), size);
        if (!shm->check_sanity()) {
            SAMBAG_LOG_INFO<<"  + check sanity failed. trying again.";
            boost::interprocess::shared_memory_object::remove(name.c_str());
            initMemory(size, tried+1);
        }
        SAMBAG_LOG_INFO<<"alloc "<<name<<": SUCCEED";
    } catch (std::exception &ex) {
        if (tried>0) {
            SAMBAG_LOG_ERR<<"alloc "<<name<<": FAILED, "<<ex.what();
            throw;
        }
        boost::interprocess::shared_memory_object::remove(name.c_str());
        initMemory(size, tried+1);
    } catch (...) {
        if (tried>0) {
            SAMBAG_LOG_ERR<<"alloc "<<name<<": FAILED";
            throw;
        }
        boost::interprocess::shared_memory_object::remove(name.c_str());
        initMemory(size, tried+1);
    }
    ref_counter = get().find_or_construct<Integer>(NAME_REF_COUNTER.c_str())(0);
    ++(*ref_counter);
}
//-----------------------------------------------------------------------------
SharedMemoryHolder::SharedMemoryHolder(const char *name, size_t size) : name(name)
{
    initMemory(size);
}
//-----------------------------------------------------------------------------
SharedMemoryHolder::~SharedMemoryHolder() {
    SAMBAG_LOG_INFO<<"removing "<<name<<": ...";
    int refc = --(*ref_counter);
    delete shm;
    if ( refc == 0) {
        try {
            boost::interprocess::shared_memory_object::remove(name.c_str());
        } catch(const std::exception &ex) {
            SAMBAG_LOG_FATAL<<"removing "<<name<<": FAILED, "<<ex.what();
        } catch(...) {
            SAMBAG_LOG_FATAL<<"removing "<<name<<": FAILED";
        }
    }
    SAMBAG_LOG_INFO<<"removing "<<name<<": SUCCEED";
}
}}} // namespace(s)
