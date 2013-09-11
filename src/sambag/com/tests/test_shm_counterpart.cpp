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


int main(int argc, const char **argv) {
    using namespace sambag::com::interprocess;
    SharedMemoryHolder shmh("sambag.unit_test", 64000);
    String::Class *opc = String::findOrCreate("opc", shmh.get());
    if (!opc) {
        std::cout<<argv[0]<<": no opcode"<<std::endl;
        return 1;
    }
    if (*opc=="sum") {
        sum_int();
        return 0;
    }
    std::cout<<argv[0]<<": unknown opcode"<<std::endl;
    return 1;
}