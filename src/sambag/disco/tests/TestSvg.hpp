/*
 * TestSvg.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef TESTSVG_HPP_
#define TESTSVG_HPP_
#include "sambag/disco/CairoDrawContext.hpp"
#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestSvg : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestSvg );
	CPPUNIT_TEST( testSvgFirstElements );
	CPPUNIT_TEST( testSvgTransform01 );
	CPPUNIT_TEST( testSvgTransform02 );
	CPPUNIT_TEST( testSvgTransform03 );
	CPPUNIT_TEST( testSvgTransform04 );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void testSvgFirstElements();
	void testSvgTransform01();
	void testSvgTransform02();
	void testSvgTransform03();
	void testSvgTransform04();
};

} // namespace

#endif /* TESTSVG_HPP_ */
