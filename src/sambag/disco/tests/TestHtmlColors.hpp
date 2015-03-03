/*
 * TestHtmlColors.hpp
 *
 *  Created on: Tue Aug 12 15:53:20 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTHTMLCOLORS_H
#define SAMBAG_TESTHTMLCOLORS_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestHtmlColors : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestHtmlColors );
	CPPUNIT_TEST( testToRGB );
	CPPUNIT_TEST_SUITE_END();
public:
	void testToRGB();
};

} // namespace

#endif /* SAMBAG_TESTHTMLCOLORS_H */
