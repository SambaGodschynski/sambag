/*
 * TestOne2One.hpp
 *
 *  Created on: Thu Oct 18 14:27:42 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTONE2ONE_H
#define SAMBAG_TESTONE2ONE_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestOne2One : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestOne2One );
	CPPUNIT_TEST( testSimple );
	CPPUNIT_TEST_SUITE_END();
public:
	void testSimple();
	void testCustom();
};

} // namespace

#endif /* SAMBAG_TESTONE2ONE_H */
