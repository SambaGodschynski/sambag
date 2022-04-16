/*
 * TestFileResourceManager.cpp
 *
 *  Created on: Thu Nov  8 13:34:42 2012
 *      Author: Johannes Unger
 */

#include "TestFileResourceManager.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/FileResourceManager.hpp>
#include <string>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestFileResourceManager );


namespace tests {
//=============================================================================
//  Class TestFileResourceManager
//=============================================================================
//-----------------------------------------------------------------------------
void TestFileResourceManager::setUp() {
	sambag::disco::FileResourceManager::init("./testimages");
}
//-----------------------------------------------------------------------------
void TestFileResourceManager::tearDown() {
}
//-----------------------------------------------------------------------------
void TestFileResourceManager::testImageLoader() {
	using namespace sambag::disco;
	CPPUNIT_ASSERT( &(getResourceManager()) );
	FileResourceManager &rep = FileResourceManager::instance();
	CPPUNIT_ASSERT(!rep.getImage("frxTest.imageA"));
	CPPUNIT_ASSERT(!rep.getImage("frxTest.imageB"));
	CPPUNIT_ASSERT(rep.registerImage("frxTest.imageA", "disco-floor.png"));
	CPPUNIT_ASSERT(rep.registerImage("frxTest.imageB", "w3c.svg"));
	// check image a 
	{
		ISurface::Ptr img = rep.getImage("frxTest.imageA");
		CPPUNIT_ASSERT(img);
		Rectangle r = img->getSize();
		CPPUNIT_ASSERT_EQUAL((Coordinate)446., r.width());
		CPPUNIT_ASSERT_EQUAL((Coordinate)446., r.height());
	}
	// check image b
	{
		ISurface::Ptr img = rep.getImage("frxTest.imageB");
		CPPUNIT_ASSERT(img);
		Rectangle r = img->getSize();
		CPPUNIT_ASSERT_EQUAL((Coordinate)480., r.width());
		CPPUNIT_ASSERT_EQUAL((Coordinate)360., r.height());
	}
}
} //namespace
