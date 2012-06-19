/*
 * TestPopupMenu.hpp
 *
 *  Created on: Mon Jun 18 12:11:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTPOPUPMENU_H
#define SAMBAG_TESTPOPUPMENU_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestPopupMenu : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestPopupMenu );
	CPPUNIT_TEST( testAddElements );
	CPPUNIT_TEST_SUITE_END();
public:
	void testAddElements();
};

} // namespace

#endif /* SAMBAG_TESTPOPUPMENU_H */
