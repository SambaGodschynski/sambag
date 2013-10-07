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
#include <cstdlib>

#ifdef WIN32
    const char * COUNTERPART_EXEC = "./test_shm_counterpart.exe";
    const char * COUNTERPART_EXEC_32 = "./test_shm_counterpart.exe";
#else   
    const char * COUNTERPART_EXEC = "./test_shm_counterpart";
    const char * COUNTERPART_EXEC_32 = "arch -32 ./test_shm_counterpart unmanaged";
#endif


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestInterprocess );

std::ostream & operator<<(std::ostream &os,
    const sambag::com::interprocess::ManagedSharedMemory &m)
{
    using namespace sambag::com::interprocess;
    ManagedSharedMemory::const_named_iterator it = m.named_begin();
    for (; it!=m.named_end(); ++it) {
        std::string name(it->name());
        if (name==SharedMemoryHolder::NAME_REF_COUNTER) {
            continue;
        }
        os<<name<<", ";
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
    CPPUNIT_ASSERT_EQUAL( (size_t)1000, (size_t)m1.get().get_size() );
    String::Class *str = String::findOrCreate("s1", m1.get());
    *str="";
    CPPUNIT_ASSERT_EQUAL(std::string("s1, "), sambag::com::toString(m1.get()) );
    {
        SharedMemoryHolder m2("M1", 500);
        CPPUNIT_ASSERT_EQUAL( (size_t)1000, (size_t)m2.get().get_size() );
        CPPUNIT_ASSERT_EQUAL(std::string("s1, "), sambag::com::toString(m2.get()) );
        SharedMemoryHolder m3("M2", 500);
        CPPUNIT_ASSERT_EQUAL( (size_t)500, (size_t)m3.get().get_size() );
        CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(m3.get()) );
        // releasing m2, m3
    }
    {
        SharedMemoryHolder m2("M1", 1000);
        CPPUNIT_ASSERT_EQUAL(std::string("s1, "), sambag::com::toString(m2.get()) );
    }
    // assume m1 is still valid
    CPPUNIT_ASSERT_EQUAL(std::string("s1, "), sambag::com::toString(m1.get()) );
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
    CPPUNIT_ASSERT_EQUAL((size_t)10, (size_t)vector->size());
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
void TestInterprocess::testSet() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("M1", 64000);
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
    typedef Set<String::Class> StrSet;
    StrSet::Class *set = StrSet::findOrCreate("s1", shmh.get());
   
    String::TheAllocator str_alloc(shmh.get().get_segment_manager());
   
    set->insert(String::Class("a", str_alloc));
    set->insert(String::Class("b", str_alloc));
    set->insert(String::Class("c", str_alloc));
    set->insert(String::Class("c", str_alloc));
    
    CPPUNIT_ASSERT_EQUAL((size_t)3, (size_t)set->size());
}
//-----------------------------------------------------------------------------
void TestInterprocess::testMap() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("M1", 64000);
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
    typedef Map<int, float> TheMap;
    TheMap::Class *map = TheMap::findOrCreate("map1", shmh.get());
    map->insert( TheMap::ValueType(1, 1.0) );
    map->insert( TheMap::ValueType(2, 1.1) );
    map->insert( TheMap::ValueType(3, 1.5) );
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
namespace {
    struct TestClass {
        int _int;
    };

}
void TestInterprocess::testSharedPtr() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("M1", 64000);
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
    typedef SharedPtr<TestClass> TestPtr;
    typedef WeakPtr<TestClass> TestWPtr;
    {
        TestPtr::Class pStr = TestPtr::create("pStr1", shmh.get());
        //CPPUNIT_ASSERT_EQUAL(std::string("pStr1, "), sambag::com::toString(shmh.get()) );
        pStr->_int = 101;
        CPPUNIT_ASSERT_EQUAL((long int)1, pStr.use_count());
        {
            TestPtr::Class pStr2 = pStr;
            TestWPtr::Class wpStr = pStr;
            CPPUNIT_ASSERT_EQUAL((long int)2, pStr.use_count());
            TestPtr::Class pStr3 = wpStr.lock();
            CPPUNIT_ASSERT_EQUAL((long int)3, pStr.use_count());
            CPPUNIT_ASSERT_EQUAL((int)101, pStr3->_int);
        }
        CPPUNIT_ASSERT_EQUAL((long int)1, pStr.use_count());
    }
    // confirm memory is empty
    CPPUNIT_ASSERT_EQUAL(std::string(""), sambag::com::toString(shmh.get()) );
}
//-----------------------------------------------------------------------------
void TestInterprocess::testSharedMemoryInterArch() {
    /**
     * as long the boost interarchitecture (32/64 bit) managed shared memory
     * is broken, we need to use a "raw and brutal" approach:
     */
    using namespace sambag::com::interprocess;
    typedef PlacementAlloc<Integer> Alloc;
    using namespace boost::interprocess;
    static const char * NAME="interarch";
    
    struct Autoremove {
        ~Autoremove() {
            using namespace boost::interprocess;
            shared_memory_object::remove(NAME);
        }
    } autoremove;
    
    static const size_t size = 6400*sizeof(char);
    SharedMemoryObject shm = SharedMemoryObject(open_or_create, NAME, read_write);
    shm.truncate(size);
    MappedRegion mp = MappedRegion(shm, read_write);
    void *ptr = mp.get_address();
    PointerIterator pIt(ptr, size);
    Alloc alloc(pIt);
    
    Integer *i = alloc.allocate(1);
    CPPUNIT_ASSERT_EQUAL(0, *i);
    *i = 101;
    
    char *opc = Alloc::rebind<char>::other(alloc).allocate(4);
    strcpy(opc, "opc");

    Integer *r = alloc.allocate(10);
    Integer *l = alloc.allocate(10);
    for (size_t i=0; i<10; ++i) {
        r[i] = i;
        l[i] = i*2;
    }
    
    typedef OffsetPtr<Integer>::Class Ints;
    Ints *rl = Alloc::rebind<Ints>::other(alloc).allocate(2);
    rl[0] = r;
    rl[1] = l;

    float *fr = Alloc::rebind<float>::other(alloc).allocate(10);
    float *fl = Alloc::rebind<float>::other(alloc).allocate(10);
    for (size_t i=0; i<10; ++i) {
        fr[i] = i;
        fl[i] = i*2;
    }
    
    typedef OffsetPtr<float>::Class Floats;
    Floats *frl = Alloc::rebind<Floats>::other(alloc).allocate(2);
    frl[0] = fr;
    frl[1] = fl;
    
    CPPUNIT_ASSERT_EQUAL((int) 0, std::system(COUNTERPART_EXEC_32));
}
//-----------------------------------------------------------------------------
void TestInterprocess::testSharedMemory() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    typedef Vector<Integer> IntVector;
    IntVector::Class *v = IntVector::findOrCreate("to_sum", shmh.get());
    for (Integer i=1; i<=100; ++i) {
        v->push_back(i);
    }
    String::Class *opc = String::findOrCreate("opc", shmh.get());
    *opc="sum";
    CPPUNIT_ASSERT_EQUAL((int) 0, std::system(COUNTERPART_EXEC));
    String::Class *str_res = String::findOrCreate("result", shmh.get());
    CPPUNIT_ASSERT_EQUAL(std::string("result=5050"), std::string(str_res->c_str()));
    
    CPPUNIT_ASSERT_EQUAL(std::string("opc, result, to_sum, "), sambag::com::toString(shmh.get()) );
    
    typedef Vector<float> FloatVector;
    typedef Vector< FloatVector::Class > ComplexVector;
    ComplexVector::Class *cv1 = ComplexVector::findOrCreate("cv1", shmh.get());
    
    FloatVector::TheAllocator falloc(shmh.get().get_segment_manager());
    cv1->push_back(FloatVector::Class(falloc));
    cv1->push_back(FloatVector::Class(falloc));
    
    for (int i=0; i<3; ++i) {
        (*cv1)[0].push_back((float)i);
        (*cv1)[1].push_back((float)i*2);
    }

    *opc="cvector_tostring";
    CPPUNIT_ASSERT_EQUAL((int) 0, std::system(COUNTERPART_EXEC));
    CPPUNIT_ASSERT_EQUAL(std::string("{{0,1,2,},{0,2,4,}}"), std::string(str_res->c_str()));
    
    typedef String::Class IPString;
    typedef Vector< IPString > StringVector;
    StringVector::Class *sv = StringVector::findOrCreate("strV1", shmh.get());
    
    String::TheAllocator str_alloc(shmh.get().get_segment_manager());
    
    sv->push_back(IPString("fritz", str_alloc));
    sv->push_back(IPString("und", str_alloc));
    sv->push_back(IPString("spitz", str_alloc));
    
    *opc="strvector_tostring";
    CPPUNIT_ASSERT_EQUAL((int) 0, std::system(COUNTERPART_EXEC));
    CPPUNIT_ASSERT_EQUAL(std::string("fritz und spitz "), std::string(str_res->c_str()));
}
//-----------------------------------------------------------------------------
void TestInterprocess::testPlacementAllocator1() {
    using namespace sambag::com::interprocess;
    typedef PlacementAlloc<int> Alloc1;
    void * mem = malloc(sizeof(int)*20);
    PointerIterator pIt(mem, sizeof(int)*20);
    Alloc1 a1(pIt);
    CPPUNIT_ASSERT_EQUAL((size_t)20, a1.max_size());
    int *ints1 = a1.allocate(10);
    CPPUNIT_ASSERT_EQUAL((size_t)10, a1.max_size());
    for (int i=0; i<10; ++i) {
        ints1[i] = i+1;
    }
    
    int *ints2 = a1.allocate(10);
    CPPUNIT_ASSERT_EQUAL((size_t)0, a1.max_size());
    for (int i=0; i<10; ++i) {
        ints2[i] = i+11;
    }
    
    int * sum = (int*)mem;
    for (int i=0; i<20; ++i) {
        CPPUNIT_ASSERT_EQUAL((int)i+1, sum[i]);
    }
    free(mem);
}
//-----------------------------------------------------------------------------
void TestInterprocess::testPlacementAllocator2() {
    using namespace sambag::com::interprocess;
    typedef PlacementAlloc<int*> Alloc1;
    void * mem = malloc(sizeof(int)*20+sizeof(int*)*2);
    PointerIterator pIt(mem, sizeof(int)*20+sizeof(int*)*2);
    Alloc1 a1(pIt);
    int **ints = a1.allocate(2);
    
    
    typedef Alloc1::rebind<int>::other Alloc2;
    Alloc2 a2(a1);
    CPPUNIT_ASSERT_EQUAL((size_t)20, a2.max_size());
    int *il = a2.allocate(10);
    CPPUNIT_ASSERT_EQUAL((size_t)10, a2.max_size());
    CPPUNIT_ASSERT_THROW(a2.allocate(11), sambag::com::exceptions::IllegalStateException);
    int *ir = a2.allocate(10);
    ints[0] = il;
    ints[1] = ir;
    
    for (int i=0; i<10; ++i) {
        ints[0][i] = i+1;
        ints[1][i] = i+11;
    }
    
    CPPUNIT_ASSERT_EQUAL((void*)ints, mem);
    int * sum = (int*)(ints+2);
    for (int i=0; i<20; ++i) {
        CPPUNIT_ASSERT_EQUAL((int)i+1, sum[i]);
    }
    
    free(mem);
}
//-----------------------------------------------------------------------------
void TestInterprocess::testPlacementAllocator3() {
    using namespace sambag::com::interprocess;
    typedef PlacementAlloc<int> Alloc1;
    void * mem = malloc(sizeof(int)*20);
    PointerIterator pIt(mem, sizeof(int)*20);
    Alloc1 a1(pIt);
    std::vector<int, Alloc1> v1(a1);
    v1.reserve(20);
    for (int i=0; i<20; ++i) {
        v1.push_back(i);
    }
    free(mem);
}
//-----------------------------------------------------------------------------
void TestInterprocess::testPlacementAllocator4() {
    using namespace sambag::com::interprocess;
    typedef PlacementAlloc<float> Alloc1;
    void * mem = malloc(64000);
    
    PointerIterator pIt(mem, 64000);
    Alloc1 alloc(pIt);
    int *num_references = Alloc1::rebind<int>::other(alloc).allocate(1);
    *num_references = 100;
    size_t *blockSize_ist = Alloc1::rebind<size_t>::other(alloc).allocate(1);
    *blockSize_ist = 512;
    size_t *numChannels_ist = Alloc1::rebind<size_t>::other(alloc).allocate(1);
    *numChannels_ist = 2;
    
    CPPUNIT_ASSERT_EQUAL((int)100, *num_references);
    CPPUNIT_ASSERT_EQUAL((size_t)512, *blockSize_ist);
    CPPUNIT_ASSERT_EQUAL((size_t)2, *numChannels_ist);
    
    free(mem);
}

} //namespace
