/*
 * TestWindow.hpp
 *
 *  Created on: Mon Jun 18 15:17:41 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTWINDOW_H
#define SAMBAG_TESTWINDOW_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/windowImpl/WindowTestToolkit.hpp>

namespace tests {
//=============================================================================
class TestWindow : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestWindow );
	CPPUNIT_TEST( testComponentScreenPointConverts );
	CPPUNIT_TEST_SUITE_END();
	sambag::disco::components::WindowTestToolkit<>::Ptr wfac;
public:
	void setUp();
	void tearDown();
	void testComponentScreenPointConverts();
};

} // namespace

#endif /* SAMBAG_TESTWINDOW_H */
