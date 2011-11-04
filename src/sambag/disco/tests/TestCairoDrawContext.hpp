/*
 * TestCairoDrawContext.h
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#ifndef TESTCAIRODRAWCONTEXT_H_
#define TESTCAIRODRAWCONTEXT_H_

#include "sambag/disco/CairoDrawContext.hpp"
#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestCairoDrawContext : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestCairoDrawContext );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testLine );
	CPPUNIT_TEST( testArc );
	CPPUNIT_TEST( testRectangle );
	CPPUNIT_TEST( testCurve );
	CPPUNIT_TEST( testSetStrokeWidth );
	CPPUNIT_TEST( testSetColor );
	CPPUNIT_TEST( testMisc );
	CPPUNIT_TEST( testClip );
	CPPUNIT_TEST( testTransformation );
	CPPUNIT_TEST( testMatrixConv );
	CPPUNIT_TEST( testLineStyle );
	CPPUNIT_TEST( testPath );
	CPPUNIT_TEST( testGradient );
	CPPUNIT_TEST( testText );
	CPPUNIT_TEST( testDash );
	CPPUNIT_TEST_SUITE_END();
public:
	void testConstructor();
	void testLine();
	void testArc();
	void testRectangle();
	void testCurve();
	void testSetColor();
	void testText();
	void testLineStyle();
	void testClip();
	void testSetStrokeWidth();
	void testMatrixConv();
	void testTransformation();
	void testPath();
	void testGradient();
	void testDash();
	void testMisc();
	void setUp();
};

} // namespace

#endif /* TESTCAIRODRAWCONTEXT_H_ */
