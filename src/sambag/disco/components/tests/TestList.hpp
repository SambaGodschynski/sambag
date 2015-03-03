/*
 * TestList.hpp
 *
 *  Created on: Wed Sep  5 13:53:00 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTLIST_H
#define SAMBAG_TESTLIST_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestList : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestList );
	CPPUNIT_TEST(testDefaultListModel);
	CPPUNIT_TEST_SUITE_END();
public:
	void testDefaultListModel();
};

} // namespace

#endif /* SAMBAG_TESTLIST_H */
