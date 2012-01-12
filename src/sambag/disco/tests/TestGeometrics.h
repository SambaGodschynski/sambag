/*
 * TestGeometrics.h
 *
 *  Created on: Jan 9, 2012
 *      Author: samba
 */

#ifndef TESTGEOMETRICS_H_
#define TESTGEOMETRICS_H_

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestGeometrics : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestGeometrics );
	CPPUNIT_TEST( testRectangle );
	CPPUNIT_TEST_SUITE_END();
public:
	void testRectangle();

};

} // namespace

#endif /* TESTGEOMETRICS_H_ */
