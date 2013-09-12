/*
 * test_shm_counterpart.cpp
 *
 *  Created on: Wed Sep 11 14:21:01 2013
 *      Author: Johannes Unger
 */

#include <sambag/com/Interprocess.hpp>
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
    typedef Vector<int> IntVector;
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


int main(int argc, const char **argv) {
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
    std::cout<<argv[0]<<": unknown opcode"<<std::endl;
    return 1;
}