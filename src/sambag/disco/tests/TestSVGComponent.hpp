/*
 * TestSVGComponent.hpp
 *
 *  Created on: Sun Apr 13 20:38:40 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTSVGCOMPONENT_H
#define SAMBAG_TESTSVGCOMPONENT_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestSVGComponent : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestSVGComponent );
	CPPUNIT_TEST( testComponentTree );
	CPPUNIT_TEST_SUITE_END();
public:
	void testComponentTree();
    
};

} // namespace

#endif /* SAMBAG_TESTSVGCOMPONENT_H */
