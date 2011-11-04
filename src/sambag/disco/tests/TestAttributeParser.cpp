/*
 * TestAttributeParser.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: samba
 */

#include "TestAttributeParser.hpp"
#include "sambag/disco/svg/AttributeParser.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestAttributeParser );

using namespace sambag::com;


namespace tests {
//=============================================================================
// TestAttributeParser::setUp
//=============================================================================
//-----------------------------------------------------------------------------
void TestAttributeParser::testParseColor() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>nonsense
	ColorRGBA col;
	svg::AttributeParser::parseColor("asdlasdj", col);
	CPPUNIT_ASSERT(col==ColorRGBA::NULL_COLOR);
	svg::AttributeParser::parseColor("#klklkl", col);
	CPPUNIT_ASSERT(col==ColorRGBA::NULL_COLOR);
	svg::AttributeParser::parseColor("#%&/&%$", col);
	CPPUNIT_ASSERT(col==ColorRGBA::NULL_COLOR);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>by html name(a few samples)
	svg::AttributeParser::parseColor("red", col);
	CPPUNIT_ASSERT(col==ColorRGBA(1., 0, 0, 1.));
	svg::AttributeParser::parseColor("YELLOW", col);
	CPPUNIT_ASSERT(col==ColorRGBA(1., 1.0, 0, 1.));
	svg::AttributeParser::parseColor("DarkGreen", col);
	CPPUNIT_ASSERT(col==ColorRGBA(0.0,0.392156862745,0.0, 1.0));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>by hex
	svg::AttributeParser::parseColor("#ffbb00", col);
	CPPUNIT_ASSERT(col==ColorRGBA(1., 187./255., 0, 1.));
	svg::AttributeParser::parseColor("#ff000", col);
	CPPUNIT_ASSERT(col==ColorRGBA::NULL_COLOR);
	svg::AttributeParser::parseColor("#ff00", col);
	CPPUNIT_ASSERT(col==ColorRGBA::NULL_COLOR);
	svg::AttributeParser::parseColor("#fb0", col);
	CPPUNIT_ASSERT(col==ColorRGBA(1., 187./255., 0, 1.));
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testParseTransform() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testParseCoordinate() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testParsePathInstructions() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testParsePointContainer() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testGetPathInstruction() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testHex2Int() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testGetWholeString() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testString2Number() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testStrings2Numbers() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testGetValuesFromString() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testParseOpacity() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
//-----------------------------------------------------------------------------
void TestAttributeParser::testPrepareString() {
	using namespace sambag::disco::svg;
	CPPUNIT_FAIL("Test not yet implemented.");
}
}// namespace
