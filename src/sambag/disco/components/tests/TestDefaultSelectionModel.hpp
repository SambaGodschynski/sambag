/*
 * TestDefaultSelectionModel.hpp
 *
 *  Created on: Wed Feb 20 11:09:04 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTDEFAULTSELECTIONMODEL_H
#define SAMBAG_TESTDEFAULTSELECTIONMODEL_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestDefaultSelectionModel : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestDefaultSelectionModel );
	CPPUNIT_TEST( issue309 );
	CPPUNIT_TEST_SUITE_END();
public:
	void issue309();
};

} // namespace

#endif /* SAMBAG_TESTDEFAULTSELECTIONMODEL_H */
