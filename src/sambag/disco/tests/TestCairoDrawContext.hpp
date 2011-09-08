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
	CPPUNIT_TEST( testSetWidth );
	CPPUNIT_TEST( testSetColor );
	CPPUNIT_TEST_SUITE_END();
public:
	void testConstructor();
	void testLine();
	void testArc();
	void testSetColor();
	void testSetWidth();
};

} // namespace

#endif /* TESTCAIRODRAWCONTEXT_H_ */
