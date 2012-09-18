#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/tests/FileSystemTest.hpp>
#include <sambag/com/tests/TestThread.hpp>
#include <sambag/com/tests/TestSharedOrWeak.hpp>
#include <sambag/com/tests/TestPropertyChanged.hpp>
#include <sambag/com/tests/TestHelper.hpp>
#include <sambag/com/tests/TestArithmeticWrapper.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( tests::ComTests );
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestThread );
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestSharedOrWeak );
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestPropertyChanged );
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestHelper );
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestArithmeticWrapper );

