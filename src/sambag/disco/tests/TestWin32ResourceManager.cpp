/*
 * TestWin32ResourceManager.cpp
 *
 *  Created on: Thu Jan 17 13:00:24 2013
 *      Author: Johannes Unger
 */

#ifdef DISCO_USE_WIN32

#include "TestWin32ResourceManager.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/Win32IntResourceManager.hpp>
#include "win32_resources/resource.h"
#include <windows.h>
#include <string>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestWin32ResourceManager );

namespace tests {
//=============================================================================
//  Class TestWin32ResourceManager
//=============================================================================
//-----------------------------------------------------------------------------
TestWin32ResourceManager::TestWin32ResourceManager() {
	using namespace sambag::disco;
	Win32IntResourceManager &m = Win32IntResourceManager::instance();
	HINSTANCE hi = (HINSTANCE)GetModuleHandle(NULL);
	m.setInstance(hi);
	m.registerString("testText",  SAMBAG_RES_TESTTXT1);
	m.registerImage("testImage", SAMBAG_RES_TESTPNG1);
}
//-----------------------------------------------------------------------------
void TestWin32ResourceManager::testTextResource() {
	using namespace sambag::disco;
	Win32IntResourceManager &m = Win32IntResourceManager::instance();
	std::string txt = m.getString("testText");
	CPPUNIT_ASSERT_EQUAL(std::string("Hello World!\n"), txt);
}
//-----------------------------------------------------------------------------
void TestWin32ResourceManager::testImageResource() {
	using namespace sambag::disco;
	Win32IntResourceManager &m = Win32IntResourceManager::instance();
	Win32IntResourceManager::ImagePtr img = m.getImage("testImage");
	CPPUNIT_ASSERT(img);
}
//-----------------------------------------------------------------------------
void TestWin32ResourceManager::testExceptions() {
}
} //namespace

#endif //#ifdef DISCO_USE_WIN32