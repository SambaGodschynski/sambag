/*
 * TestPattern.h
 *
 *  Created on: Dec 1, 2011
 *      Author: samba
 */

#ifndef TESTPATTERN_H_
#define TESTPATTERN_H_

#include "sambag/disco/IPattern.hpp"
#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestPattern : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestPattern );
	CPPUNIT_TEST( testConstruction );
	CPPUNIT_TEST_SUITE_END();
public:
	void testConstruction();
};

} // namespace
#endif /* TESTPATTERN_H_ */
