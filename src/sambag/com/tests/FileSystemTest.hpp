
#ifndef COM_N_TEST_H
#define COM_N_TEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class ComTests : public CPPUNIT_NS::TestFixture {
//=============================================================================
	CPPUNIT_TEST_SUITE( ComTests );
	CPPUNIT_TEST( testDirWalker );
	CPPUNIT_TEST( testDirWalkerAbort );
	CPPUNIT_TEST_SUITE_END();
private:
public:
	void setUp(){}
	void tearDown(){}
	void testDirWalker();
	void testDirWalkerAbort();
};
} // namespace tests
#endif //ComTests
