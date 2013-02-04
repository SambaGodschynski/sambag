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
		boost::get<0>( fc.getGlyphMap( Font().setSize(56.) ) )
	);
	testPng("testGetFontMap", surface);
}
//-----------------------------------------------------------------------------
void TestFontCache::testDrawText() {
	using namespace sambag::disco;
	FontCache &fc = FontCache::instance();
	IImageSurface::Ptr surface = getDiscoFactory()->createImageSurface(420, 200);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext(surface);
	cn->setFont( Font().setSize(57.) );
	cn->setStrokeColor(ColorRGBA(1));
	cn->rect(fc.getTextBounds(cn,"\"Hello World!\""));
	cn->stroke();
	fc.drawText(cn,"\"Hello World!\"");

	cn->moveTo(Point2D(0., 50.));
	cn->rect(fc.getTextBounds(cn,"Hällo Börld!"));
	cn->stroke();
	cn->moveTo(Point2D(0., 50.));
	fc.drawText(cn,"Hällo Börld!");

	cn->setStrokeColor(ColorRGBA(0, 1));
	cn->moveTo(Point2D(0., 50.));
	cn->lineTo(Point2D(10., 50.));
	cn->stroke();

	testPng("testDrawText", surface);
}
} //namespace
