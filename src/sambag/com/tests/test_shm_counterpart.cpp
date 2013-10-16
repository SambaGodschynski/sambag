/*
 * test_shm_counterpart.cpp
 *
 *  Created on: Wed Sep 11 14:21:01 2013
 *      Author: Johannes Unger
 */

#include <sambag/com/Interprocess.hpp>
#include <sambag/com/SharedMemory.hpp>
#include <sambag/com/SharedMemoryImpl.hpp>
#include <sambag/com/PlacementAlloc.hpp>
#include <iostream>
#include <boost/foreach.hpp>
#include <sambag/com/Common.hpp>

std::ostream & operator<<(std::ostream &os,
    const sambag::com::interprocess::ManagedSharedMemory &m)
{
    using namespace sambag::com::interprocess;
    ManagedSharedMemory::const_named_iterator it = m.named_begin();
    os<<"{";
    for (; it!=m.named_end(); ++it) {
        os<<it->name()<<", ";
    }
    os<<"}";
    return os;
}


void sum_int() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    typedef Vector<Integer> IntVector;
    IntVector::Class *v = IntVector::findOrCreate("to_sum", shmh.get());
    int res=0;
    BOOST_FOREACH(int x, *v) {
        res+=x;
    }
    String::Class *str_result = String::findOrCreate("result", shmh.get());
    *str_result = ("result=" + sambag::com::toString(res)).c_str();
}

void cvector_tostring() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    String::Class *str_result = String::findOrCreate("result", shmh.get());
    typedef Vector<float> FloatVector;
    typedef Vector< FloatVector::Class > ComplexVector;
    ComplexVector::Class *cv1 = ComplexVector::findOrCreate("cv1", shmh.get());
    if (!cv1 || cv1->empty() ) {
        *str_result="{}";
        return;
    }
    std::stringstream sl;
    std::stringstream sr;
    sl<<"{";
    sr<<"{";
    for (size_t i=0; i<(*cv1)[0].size(); ++i) {
        sl<<(*cv1)[0][i]<<",";
        sr<<(*cv1)[1][i]<<",";
    }
    sl<<"}";
    sr<<"}";
    std::string tmp = "{" + sl.str() + "," + sr.str() + "}";
    *str_result = tmp.c_str();
}


void strvector_tostring() {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    String::Class *str_result = String::findOrCreate("result", shmh.get());
    typedef Vector<String::Class> StringVector;
    StringVector::Class *strv1 = StringVector::findOrCreate("strV1", shmh.get());
    
    if (!strv1 || strv1->empty() ) {
        *str_result="{}";
        return;
    }
    std::stringstream ss;
    BOOST_FOREACH( const String::Class &str, *strv1) {
        ss<<str<<" ";
    }
    
   
    *str_result = ss.str().c_str();
}


int mainManaged(int argc, const char **argv) {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    //std::cout<<shmh.get()<<std::endl;
    String::Class *opc = String::findOrCreate("opc", shmh.get());
    if (!opc || opc->empty()) {
        std::cout<<argv[0]<<": no opcode"<<std::endl;
        return 1;
    }
    if (*opc=="sum") {
        sum_int();
        return 0;
    }
    if (*opc=="cvector_tostring") {
        cvector_tostring();
        return 0;
    }
    if (*opc=="strvector_tostring") {
        strvector_tostring();
        return 0;
    }
    std::cout<<argv[0]<<": unknown opcode"<<std::endl;
    return 1;
}

int mainUnmanaged(int argc, const char **argv) {
    using namespace sambag::com::interprocess;
    typedef PlacementAlloc<Integer> Alloc;
    using namespace boost::interprocess;
    SharedMemoryObject shm = SharedMemoryObject(open_only, "interarch", read_write);
    MappedRegion mp = MappedRegion(shm, read_write);
    void *ptr = mp.get_address();
    PointerIterator pIt(ptr, 6400);
    Alloc alloc(pIt);
    Integer *i = alloc.allocate(1);
    if ( (*i) != 101 ) {
        return -1;
    }
    char *opc = Alloc::rebind<char>::other(alloc).allocate(4);
    if (std::string(opc) !="opc") {
        return -1;
    }
    
    pIt.next<Integer>(20);

    typedef OffsetPtr<Integer>::Class Ints;
    Ints *rl = Alloc::rebind<Ints>::other(alloc).allocate(2);
  
    for (size_t i=0; i<10; ++i) {
        if (rl[0][i] != (Integer)i) {
            return -1;
        }
        if (rl[1][i] != (Integer)i*2) {
            return -1;
        }
    }

    pIt.next<float>(20);

    typedef OffsetPtr<float>::Class Floats;
    Floats *frl = Alloc::rebind<Floats>::other(alloc).allocate(2);
  
    for (size_t i=0; i<10; ++i) {
        if (frl[0][i] != (Integer)i) {
            return -1;
        }
        if (frl[1][i] != (Integer)i*2) {
            return -1;
        }
    }
    return 0;
}


int main(int argc, const char **argv) {
    if (argc==2) {
        if (std::string(argv[1]) == "unmanaged") {
            return mainUnmanaged(argc, argv);
        }
    }
    return mainManaged(argc, argv);
}

