/*
 * TestPropertyChanged.cpp
 *
 *  Created on: Fri May 18 20:29:04 2012
 *      Author: Johannes Unger
 */

#include "TestPropertyChanged.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/events/PropertyChanged.hpp>

namespace tests {
//=============================================================================
//  Class TestPropertyChanged
//=============================================================================
//-----------------------------------------------------------------------------
void TestPropertyChanged::testCreateAndGetter() {
	using namespace sambag::com::events;
	using namespace sambag::com;
	{
		PropertyChanged p01("p01", "0string", "1string");
		std::string o, n;
		p01.getOldValue(o); p01.getNewValue(n);
		CPPUNIT_ASSERT_EQUAL(std::string("0string"), o);
		CPPUNIT_ASSERT_EQUAL(std::string("1string"), n);
	} {
		PropertyChanged p01("p02", 10, 20);
		int o, n;
		p01.getOldValue(o); p01.getNewValue(n);
		CPPUNIT_ASSERT_EQUAL((int)10, o);
		CPPUNIT_ASSERT_EQUAL((int)20, n);
	} {
		PropertyChanged p02("p02", 10, 20);
		std::string o = "failed";
		p02.getOldValue(o);
		// PropertyChanged's pair init value will be returned
		CPPUNIT_ASSERT_EQUAL(std::string(""), o);
	}
}

} //namespace
