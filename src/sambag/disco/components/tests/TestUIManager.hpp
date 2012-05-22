/*
 * TestUIManager.hpp
 *
 *  Created on: Sun Jun  3 17:19:41 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTUIMANAGER_H
#define SAMBAG_TESTUIMANAGER_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestUIManager : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestUIManager );
	CPPUNIT_TEST( testSingleton );
	CPPUNIT_TEST_SUITE_END();
public:
	void testSingleton();
};

} // namespace

#endif /* SAMBAG_TESTUIMANAGER_H */
