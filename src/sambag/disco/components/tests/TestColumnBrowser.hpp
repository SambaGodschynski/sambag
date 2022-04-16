/*
 * TestColumnBrowser.hpp
 *
 *  Created on: Thu Mar 13 15:04:40 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTCOLUMNBROWSER_H
#define SAMBAG_TESTCOLUMNBROWSER_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestColumnBrowser : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestColumnBrowser );
	CPPUNIT_TEST(testSelectPath);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSelectPath();
};

} // namespace

#endif /* SAMBAG_TESTCOLUMNBROWSER_H */
