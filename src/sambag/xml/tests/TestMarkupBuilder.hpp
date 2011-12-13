/*
 * TestMarkupBuilder.h
 *
 *  Created on: Dec 10, 2011
 *      Author: samba
 */

#ifndef TESTMARKUPBUILDER_H_
#define TESTMARKUPBUILDER_H_

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestMarkupBuilder : public CPPUNIT_NS::TestFixture {
//=============================================================================
	CPPUNIT_TEST_SUITE( TestMarkupBuilder );
	CPPUNIT_TEST( testSimpleTagCreator );
	CPPUNIT_TEST( testHtmlCreator );
	CPPUNIT_TEST_SUITE_END();
private:
public:
	void testSimpleTagCreator();
	void testHtmlCreator();
};
} // namespace tests

#endif /* TESTMARKUPBUILDER_H_ */
