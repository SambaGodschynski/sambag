/*
 * TestInterprocess.cpp
 *
 *  Created on: Wed Sep 11 14:21:01 2013
 *      Author: Johannes Unger
 */

#include "TestInterprocess.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/Interprocess.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <ostream>
#include <sambag/com/Common.hpp>

#ifdef WIN32
    const char * COUNTERPART_EXEC = "./test_shm_counterpart.exe";
#else   
    const char * COUNTERPART_EXEC = "./test_shm_counterpart";
#endif


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestInterprocess );

std::ostream & operator<<(std::ostream &os,
    const sambag::com::interprocess::ManagedSharedMemory &m)
{
    using namespace sambag::com::interprocess;
    ManagedSharedMemory::const_named_iterator it = m.named_begin();
    for (; it!=m.named_end(); ++it) {
        os<<it->name()<<", ";
    }
    return os;
}

bool operator == (const sambag::com::interprocess::ManagedSharedMemory &a,
                  const sambag::com::interprocess::ManagedSharedMemory &b)
{
    return sambag::com::toString(a) == sambag::com::toString(b);
}

namespace tests {
//=============================================================================
//  Class TestInterprocess
//=============================================================================
//-----------------------------------------------------------------------------
void TestInterprocess::testCreatingSharedMemory() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder m1("M1", 1000);
    CPPUNIT_ASSERT_EQUAL( (size_t)1000, m1.get().get_size() );
    
    {
        SharedMemoryHolder m2("M1", 500);
        CPPUNIT_ASSERT_EQUAL( (size_t)1000, m2.get().get_size() );
        SharedMemoryHolder m3("M2", 500);
        CPPUNIT_ASSERT_EQUAL( (size_t)500, m3.get().get_size() );
        // releasing m2
    }
    // assume m1 is still valid
    CPPUNIT_ASSERT( m1.get().check_sanity() );
}
//-----------------------------------------------------------------------------
void TestInterprocess::testVector() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("M1", 64000);
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
    Vector<float>::Class *vector =
        Vector<float>::findOrCreate("v1", shmh.get());
    for (int i=0; i<10; ++i) {
        vector->push_back(i*1.0f);
    }
    CPPUNIT_ASSERT_EQUAL((size_t)10, vector->size());
    {
        SharedMemoryHolder shmh2("M1", 64000);
        CPPUNIT_ASSERT_EQUAL(shmh2.get(), shmh2.get());
        Vector<float>::Class *vector2 =
            Vector<float>::findOrCreate("v1", shmh2.get());
        CPPUNIT_ASSERT(*vector == *vector2);
        
        Vector<float>::Class *vector3 =
            Vector<float>::findOrCreate("v2", shmh2.get());
        CPPUNIT_ASSERT(*vector != *vector3);
    }
}
//-----------------------------------------------------------------------------
void TestInterprocess::testMap() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("M1", 64000);
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
    typedef Map<std::string, float> TheMap;
    TheMap::Class *map = TheMap::findOrCreate("map1", shmh.get());
    map->insert( TheMap::ValueType("bauer", 1.0) );
    map->insert( TheMap::ValueType("laufer", 1.1) );
    map->insert( TheMap::ValueType("koenig", 1.5) );
    {
        SharedMemoryHolder shmh2("M1", 64000);
        CPPUNIT_ASSERT_EQUAL(shmh2.get(), shmh2.get());
        TheMap::Class *map2 = TheMap::findOrCreate("map1", shmh2.get());
        CPPUNIT_ASSERT(*map == *map2);
        TheMap::Class *map3 = TheMap::findOrCreate("map2", shmh2.get());
        CPPUNIT_ASSERT(*map != *map3);
    }
    shmh.get().destroy<TheMap::Class>("map2");
    shmh.get().destroy<TheMap::Class>("map1");
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
}
//-----------------------------------------------------------------------------
void TestInterprocess::testSharedPtr() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("M1", 64000);
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
    typedef SharedPtr<std::string> StringPtr;
    typedef WeakPtr<std::string> StringWPtr;
    {
        StringPtr::Class pStr = StringPtr::create("pStr1", shmh.get());
        CPPUNIT_ASSERT_EQUAL(std::string("pStr1, "), sambag::com::toString(shmh.get()) );
        *pStr = "nuff nuff";
        CPPUNIT_ASSERT_EQUAL((long int)1, pStr.use_count());
        {
            StringPtr::Class pStr2 = pStr;
            StringWPtr::Class wpStr = pStr;
            CPPUNIT_ASSERT_EQUAL((long int)2, pStr.use_count());
            StringPtr::Class pStr3 = wpStr.lock();
            CPPUNIT_ASSERT_EQUAL((long int)3, pStr.use_count());
            CPPUNIT_ASSERT_EQUAL(std::string("nuff nuff"), *pStr3);
        }
        CPPUNIT_ASSERT_EQUAL((long int)1, pStr.use_count());
    }
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
}
//-----------------------------------------------------------------------------
void TestInterprocess::testSharedMemory() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    typedef Vector<int> IntVector;
    IntVector::Class *v = IntVector::findOrCreate("to_sum", shmh.get());
    for (int i=1; i<=100; ++i) {
        v->push_back(i);
    }
    String::Class *opc = String::findOrCreate("opc", shmh.get());
    *opc="sum";
    CPPUNIT_ASSERT_EQUAL((int) 0, std::system(COUNTERPART_EXEC));
    
    String::Class *str_res = String::findOrCreate("result", shmh.get());
    CPPUNIT_ASSERT_EQUAL(std::string("result=5050"), std::string(str_res->c_str()));
}
} //namespace
