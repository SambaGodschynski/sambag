/*
 * TestGenFlowLayout.hpp
 *
 *  Created on: Sat Dec  8 14:37:13 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTGENFLOWLAYOUT_H
#define SAMBAG_TESTGENFLOWLAYOUT_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestGenFlowLayout : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestGenFlowLayout );
	CPPUNIT_TEST( testRectangle );
	CPPUNIT_TEST( testBounds );
	CPPUNIT_TEST_SUITE_END();
public:
	void testDiscoComponent();
	void testRectangle();
	void testBounds();
};

} // namespace

#endif /* SAMBAG_TESTGENFLOWLAYOUT_H */
