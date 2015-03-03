/*
 * testPropertyChanged.hpp
 *
 *  Created on: Fri May 18 20:29:04 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTPROPERTYCHANGED_H
#define SAMBAG_TESTPROPERTYCHANGED_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestPropertyChanged : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestPropertyChanged );
	CPPUNIT_TEST( testCreateAndGetter );
	CPPUNIT_TEST_SUITE_END();
public:
	void testCreateAndGetter();
};

} // namespace

#endif /* SAMBAG_TESTPROPERTYCHANGED_H */
