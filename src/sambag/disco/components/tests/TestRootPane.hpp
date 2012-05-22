/*
 * TestRootPane.hpp
 *
 *  Created on: Wed May 30 14:36:26 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTROOTPANE_H
#define SAMBAG_TESTROOTPANE_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/ISurface.hpp>
#include "TestComponents.hpp"
namespace tests {
//=============================================================================
class TestRootPane : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestRootPane );
	CPPUNIT_TEST( testRootPane );
	CPPUNIT_TEST( testMouseEvent );
	CPPUNIT_TEST( testRepaint );
	CPPUNIT_TEST( testLaf );
	CPPUNIT_TEST_SUITE_END();
	sambag::disco::components::RootPane::Ptr root;
	sambag::disco::IImageSurface::Ptr surf;
	enum { NUM_COMPOS = 14 };
	TestComponent::Ptr comps[NUM_COMPOS];
public:
	void setUp();
	void testRootPane();
	void testMouseEvent();
	void testRepaint();
	void testLaf();
};

} // namespace

#endif /* SAMBAG_TESTROOTPANE_H */
