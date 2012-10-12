/*
 * TestGeometrySerialization.hpp
 *
 *  Created on: Fri Oct 12 15:20:44 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTGEOMETRYSERIALIZATION_H
#define SAMBAG_TESTGEOMETRYSERIALIZATION_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestGeometrySerialization : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestGeometrySerialization );
	CPPUNIT_TEST( testSerialization );
	CPPUNIT_TEST_SUITE_END();
public:
	void testSerialization();
};

} // namespace

#endif /* SAMBAG_TESTGEOMETRYSERIALIZATION_H */
