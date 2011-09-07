
#ifndef XML_OBJ_TEST_H
#define XML_OBJ_TEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class XML2ObjectTest : public CPPUNIT_NS::TestFixture {
//=============================================================================
  CPPUNIT_TEST_SUITE( XML2ObjectTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testBuildStructure );
  CPPUNIT_TEST_SUITE_END();
private:
public:
  void testConstructor();
  void testBuildStructure();
};
} // namespace tests
#endif 
