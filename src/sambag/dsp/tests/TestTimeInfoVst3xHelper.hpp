/*
 * TestTimeInfoVst3xHelper.hpp
 *
 *  Created on: Fri Feb 13 21:12:03 2015
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTTIMEINFOVST3XHELPER_H
#define SAMBAG_TESTTIMEINFOVST3XHELPER_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestTimeInfoVst3xHelper : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestTimeInfoVst3xHelper );
	CPPUNIT_TEST( testFilterConv );
    CPPUNIT_TEST( testFilterConv2 );
    CPPUNIT_TEST( testPODConv );
	CPPUNIT_TEST_SUITE_END();
public:
	void testFilterConv();
    void testFilterConv2();
    void testPODConv();
};

} // namespace

#endif /* SAMBAG_TESTTIMEINFOVST3XHELPER_H */
