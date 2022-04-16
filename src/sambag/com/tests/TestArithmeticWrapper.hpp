/*
 * TestArithmeticWrapper.hpp
 *
 *  Created on: Fri May 18 14:57:39 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTARITHMETICWRAPPER_H
#define SAMBAG_TESTARITHMETICWRAPPER_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestArithmeticWrapper : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestArithmeticWrapper );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testMath );
	CPPUNIT_TEST_SUITE_END();
public:
	void testConstructor();
	void testMath();
};

} // namespace

#endif /* SAMBAG_TESTARITHMETICWRAPPER_H */
