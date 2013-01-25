/*
 * TestFontCache.cpp
 *
 *  Created on: Fri Jan 25 11:20:27 2013
 *      Author: Johannes Unger
 */

#include "TestFontCache.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/FontCache.hpp>
#include "sambag/disco/IDiscoFactory.hpp"
#include "HelperForTesting.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestFontCache );

namespace tests {
//=============================================================================
//  Class TestFontCache
//=============================================================================
//-----------------------------------------------------------------------------
void TestFontCache::setUp() {
	setupEnv();
}
//-----------------------------------------------------------------------------
void TestFontCache::testGetFontMap() {
	using namespace sambag::disco;
	FontCache &fc = FontCache::instance();
	IImageSurface::Ptr surface = boost::shared_dynamic_cast<IImageSurface>(
		fc.getGlyphMap( Font().setSize(38.) ).first
	);
	testPng("testGetFontMap", surface);
}
//-----------------------------------------------------------------------------
void TestFontCache::testDrawText() {
	using namespace sambag::disco;
	FontCache &fc = FontCache::instance();
	IImageSurface::Ptr surface = getDiscoFactory()->createImageSurface(320, 200);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext(surface);
	cn->setFont( Font().setSize(38.) );
	fc.drawText(cn,"Hello World!");
	cn->moveTo(Point2D(0, 50.));
	fc.drawText(cn,"Hällo Börld!");
	testPng("testDrawText", surface);
}
} //namespace
