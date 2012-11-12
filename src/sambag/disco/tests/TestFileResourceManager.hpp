/*
 * TestFileResourceManager.hpp
 *
 *  Created on: Thu Nov  8 13:34:42 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTGRAPHICSREPOSITIORY_H
#define SAMBAG_TESTGRAPHICSREPOSITIORY_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestFileResourceManager : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestFileResourceManager );
	CPPUNIT_TEST( testImageLoader );
	CPPUNIT_TEST_SUITE_END();
public:
	virtual void setUp();
	virtual void tearDown();
	void testImageLoader();
};

} // namespace

#endif /* SAMBAG_TESTGRAPHICSREPOSITIORY_H */
