/*
 * TestVstMidiEventAdapter.hpp
 *
 *  Created on: Fri Nov 30 19:08:19 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTVSTMIDIEVENTADAPTER_H
#define SAMBAG_TESTVSTMIDIEVENTADAPTER_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestVstMidiEventAdapter : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestVstMidiEventAdapter );
	CPPUNIT_TEST( testCreateNCopy );
    CPPUNIT_TEST( testTrim );
	CPPUNIT_TEST_SUITE_END();
public:
	void testCreateNCopy();
    void testTrim();
};

} // namespace

#endif /* SAMBAG_TESTVSTMIDIEVENTADAPTER_H */
