/*
 * TestHelper.hpp
 *
 *  Created on: 28.09.2011
 *      Author: samba
 */

#ifndef TESTHELPER_HPP_
#define TESTHELPER_HPP_

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestHelper : public CPPUNIT_NS::TestFixture {
//=============================================================================
	CPPUNIT_TEST_SUITE( TestHelper );
	CPPUNIT_TEST( testFromContainer );
	CPPUNIT_TEST( testTupleForeach );
	CPPUNIT_TEST_SUITE_END();
private:
public:
	void testTupleForeach();
	void testFromContainer();
};
} // namespace tests

#endif /* TESTHELPER_HPP_ */
