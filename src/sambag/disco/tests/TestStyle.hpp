/*
 * TestStyle.hpp
 *
 *  Created on: Tue Aug 12 17:34:14 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTSTYLE_H
#define SAMBAG_TESTSTYLE_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestStyle : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestStyle );
	CPPUNIT_TEST( testToString );
	CPPUNIT_TEST_SUITE_END();
public:
	void testToString();
};

} // namespace

#endif /* SAMBAG_TESTSTYLE_H */
