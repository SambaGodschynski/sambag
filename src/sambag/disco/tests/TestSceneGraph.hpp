/*
 * TestSceneGraph.hpp
 *
 *  Created on: 29.10.2011
 *      Author: samba
 */

#ifndef TESTSCENEGRAPH_HPP_
#define TESTSCENEGRAPH_HPP_

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestSceneGraph : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestSceneGraph );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testBuildGraph );
	CPPUNIT_TEST( testTransformNode );
	CPPUNIT_TEST( testStyleNode );
	CPPUNIT_TEST( testIdAndClassRelations );
	CPPUNIT_TEST( testGetChildrenOf );
	CPPUNIT_TEST( testGetChildrenOfFiltered );
	CPPUNIT_TEST( testCopySubGraph );
    CPPUNIT_TEST( testCalculateStyle );
	CPPUNIT_TEST_SUITE_END();
public:
	void testConstructor();
	void testBuildGraph();
	void testTransformNode();
	void testStyleNode();
	void testIdAndClassRelations();
	void testGetChildrenOf();
	void testGetChildrenOfFiltered();
	void testCopySubGraph();
    void testCalculateStyle();
};

} // namespace


#endif /* TESTSCENEGRAPH_HPP_ */
