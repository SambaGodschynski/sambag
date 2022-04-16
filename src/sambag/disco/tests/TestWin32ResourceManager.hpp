/*
 * TestWin32ResourceManager.hpp
 *
 *  Created on: Thu Jan 17 13:00:24 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTWIN32RESOURCEMANAGER_H
#define SAMBAG_TESTWIN32RESOURCEMANAGER_H

#ifdef DISCO_USE_WIN32

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestWin32ResourceManager : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestWin32ResourceManager );
	CPPUNIT_TEST( testTextResource );
	CPPUNIT_TEST( testImageResource );
	CPPUNIT_TEST( testExceptions );
	CPPUNIT_TEST_SUITE_END();
public:
	TestWin32ResourceManager();
	void testTextResource();
	void testImageResource();
	void testExceptions();
};

} // namespace

#endif //#ifdef DISCO_USE_WIN32
#endif /* SAMBAG_TESTWIN32RESOURCEMANAGER_H */
