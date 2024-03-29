/*
 * TestInterprocess.hpp
 *
 *  Created on: Wed Sep 11 14:21:01 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTINTERPROCESS_H
#define SAMBAG_TESTINTERPROCESS_H

#include <sambag/com/Interprocess.hpp>

namespace CppUnit {
std::ostream & operator<<(std::ostream &os,
    const sambag::com::interprocess::ManagedSharedMemory &m);
bool operator == (const sambag::com::interprocess::ManagedSharedMemory &a,
                  const sambag::com::interprocess::ManagedSharedMemory &b);
}

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/com/UnitEx.hpp>

namespace tests {
//=============================================================================
class TestInterprocess : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestInterprocess );
	CPPUNIT_TEST( testCreatingSharedMemory );
    CPPUNIT_TEST( testVector );
    CPPUNIT_TEST( testSet );
    CPPUNIT_TEST( testMap );
    CPPUNIT_TEST( testSharedPtr );
    CPPUNIT_TEST( testSharedMemory );
    CPPUNIT_TEST_KNOWN_ISSUE( testSharedMemoryInterArch );
    CPPUNIT_TEST( testPlacementAllocator1 );
    CPPUNIT_TEST( testPlacementAllocator2 );
    CPPUNIT_TEST( testPlacementAllocator3 );
    CPPUNIT_TEST( testPlacementAllocator4 );
	CPPUNIT_TEST_SUITE_END();
public:
	void testCreatingSharedMemory();
    void testVector();
    void testSet();
    void testMap();
    void testSharedPtr();
    void testSharedMemory();
    void testSharedMemoryInterArch();
    void testPlacementAllocator1();
    void testPlacementAllocator2();
    void testPlacementAllocator3();
    void testPlacementAllocator4();
};

} // namespace

#endif /* SAMBAG_TESTINTERPROCESS_H */
