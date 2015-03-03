/*
 * TestFlowLayout.hpp
 *
 *  Created on: Wed May 30 07:19:36 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTFLOWLAYOUT_H
#define SAMBAG_TESTFLOWLAYOUT_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/Forward.hpp>

namespace tests {
//=============================================================================
class TestFlowLayout : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestFlowLayout );
	CPPUNIT_TEST ( testLayout );
	CPPUNIT_TEST ( testAlignment );
	CPPUNIT_TEST ( testGap );
	CPPUNIT_TEST ( testPreferredLayoutSize );
	CPPUNIT_TEST ( testMinimumLayoutSize );
	CPPUNIT_TEST ( testToString );
	CPPUNIT_TEST_SUITE_END();
private:
	sambag::disco::components::AContainerPtr con;
	enum { NUM_COMPOS = 5 };
	sambag::disco::components::AComponentPtr comps[NUM_COMPOS];
public:
	void setUp();
	void testLayout();
	void testAlignment();
	void testGap();
	void testPreferredLayoutSize();
	void testMinimumLayoutSize();
	void testToString();
};

} // namespace

#endif /* SAMBAG_TESTFLOWLAYOUT_H */
