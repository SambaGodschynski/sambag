/*
 * TestLuaCpp.hpp
 *
 *  Created on: Fri Apr 25 10:05:22 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTLUACPP_H
#define SAMBAG_TESTLUACPP_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestLuaCpp : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLuaCpp );
	// CPPUNIT_TEST( XXX );
	CPPUNIT_TEST_SUITE_END();
public:
	//void XXX();
};

} // namespace

#endif /* SAMBAG_TESTLUACPP_H */
