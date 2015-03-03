/*
 * TestFontCache.hpp
 *
 *  Created on: Fri Jan 25 11:20:27 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTFONTCACHE_H
#define SAMBAG_TESTFONTCACHE_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestFontCache : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestFontCache );
	CPPUNIT_TEST( testGetFontMap );
	CPPUNIT_TEST( testDrawText );
	CPPUNIT_TEST_SUITE_END();
public:
	void testGetFontMap();
	void testDrawText();
	void setUp();
};

} // namespace

#endif /* SAMBAG_TESTFONTCACHE_H */
