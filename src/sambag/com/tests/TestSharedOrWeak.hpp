/*
 * TestSharedOrWeak.hpp
 *
 *  Created on: Fri May 18 14:57:39 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTSHAREDORWEAK_H
#define SAMBAG_TESTSHAREDORWEAK_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestSharedOrWeak : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestSharedOrWeak );
	CPPUNIT_TEST( testSharedOrWeak );
	CPPUNIT_TEST( testSTLContainer );
	CPPUNIT_TEST_SUITE_END();
public:
	void testSharedOrWeak();
	void testSTLContainer();
};

} // namespace

#endif /* SAMBAG_TESTSHAREDORWEAK_H */
