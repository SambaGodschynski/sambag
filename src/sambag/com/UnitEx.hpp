/*
 * UnitEx.hpp
 *
 *  Created on: Sun Nov 24 15:49:08 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_UNITEX_H
#define SAMBAG_UNITEX_H

#include <boost/shared_ptr.hpp>
#include <cppunit/extensions/HelperMacros.h>

namespace sambag { namespace com { namespace unitex {

/**
 * option: ignore known issues.
 */
extern bool ignoreKnownIssues;
extern void processArguments(int argc, char **argv);


}}} // namespace(s)


#define CPPUNIT_TEST_KNOWN_ISSUE( testMethod )            \
    if  (!sambag::com::unitex::ignoreKnownIssues)         \
    CPPUNIT_TEST_SUITE_ADD_TEST(                          \
        ( new CPPUNIT_NS::TestCaller<TestFixtureType>(    \
                  context.getTestNameFor( #testMethod),   \
                  &TestFixtureType::testMethod,           \
                  context.makeFixture() ) ) ) 



#endif /* SAMBAG_UNITEX_H */
