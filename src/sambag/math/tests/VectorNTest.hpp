
#ifndef VECTOR_N_TEST_H
#define VECTOR_N_TEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class VectorNTest : public CPPUNIT_NS::TestFixture {
//=============================================================================
	CPPUNIT_TEST_SUITE( VectorNTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testToString );
	CPPUNIT_TEST( testOperations );
	CPPUNIT_TEST( testCreators );
	CPPUNIT_TEST_SUITE_END();
private:
public:
	void setUp(){}
	void tearDown(){}
	void testConstructor();
	void testOperations();
	void testCreators();
	void testToString();
};
} // namespace tests
#endif 
