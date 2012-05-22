/*
 * TestLookAndFeel.hpp
 *
 *  Created on: Sun Jun  3 15:28:03 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTLOOKANDFEEL_H
#define SAMBAG_TESTLOOKANDFEEL_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestLookAndFeel : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLookAndFeel );
	CPPUNIT_TEST( testGetUI );
	CPPUNIT_TEST_SUITE_END();
public:
	void testGetUI();
};

} // namespace

#endif /* SAMBAG_TESTLOOKANDFEEL_H */
