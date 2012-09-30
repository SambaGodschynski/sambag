/*
 * TestDefaultTreeModel.hpp
 *
 *  Created on: Thu Sep 20 10:36:33 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTDEFAULTTREEMODEL_H
#define SAMBAG_TESTDEFAULTTREEMODEL_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestDefaultTreeModel : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestDefaultTreeModel );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testAddRemove );
	CPPUNIT_TEST_SUITE_END();
public:
	void testConstructor();
	void testAddRemove();
};

} // namespace

#endif /* SAMBAG_TESTDEFAULTTREEMODEL_H */
