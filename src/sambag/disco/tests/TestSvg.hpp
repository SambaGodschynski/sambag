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
	CPPUNIT_TEST( testSvgLine );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void testSvgLine();
};

} // namespace

#endif /* TESTSVG_HPP_ */
