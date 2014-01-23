/*
 * TestTimeInfoVst2xHelper.hpp
 *
 *  Created on: Thu Jan 23 14:21:06 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTTIMEINFOVST2XHELPER_H
#define SAMBAG_TESTTIMEINFOVST2XHELPER_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestTimeInfoVst2xHelper : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestTimeInfoVst2xHelper );
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

#endif /* SAMBAG_TESTTIMEINFOVST2XHELPER_H */
