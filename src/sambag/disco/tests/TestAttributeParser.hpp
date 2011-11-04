/*
 * TestAttributeParser.hpp
 *
 *  Created on: Dec 6, 2011
 *      Author: samba
 */

#ifndef TESTATTRIBUTEPARSER_HPP_
#define TESTATTRIBUTEPARSER_HPP_

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestAttributeParser : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestAttributeParser );
	CPPUNIT_TEST( testParseColor );
// TODO: perform tests
//	CPPUNIT_TEST( testParseTransform );
//	CPPUNIT_TEST( testParseCoordinate );
//	CPPUNIT_TEST( testParsePathInstructions );
//	CPPUNIT_TEST( testParseOpacity );
//	CPPUNIT_TEST( testParsePointContainer );
//	CPPUNIT_TEST( testGetPathInstruction );
//	CPPUNIT_TEST( testPrepareString );
//	CPPUNIT_TEST( testHex2Int );
//	CPPUNIT_TEST( testGetWholeString );
//	CPPUNIT_TEST( testString2Number );
//	CPPUNIT_TEST( testStrings2Numbers );
//	CPPUNIT_TEST( testGetValuesFromString );

	CPPUNIT_TEST_SUITE_END();
public:
	void testParseColor();
	void testParseTransform();
	void testParseCoordinate();
	void testParsePathInstructions();
	void testParseOpacity();
	void testParsePointContainer();
	void testGetPathInstruction();
	void testPrepareString();
	void testHex2Int();
	void testGetWholeString();
	void testString2Number();
	void testStrings2Numbers();
	void testGetValuesFromString();
};

} // namespace


#endif /* TESTATTRIBUTEPARSER_HPP_ */
