
#ifndef DB_TEST_H
#define DB_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "sambag/cpsqlite/DataBase.hpp"

namespace tests {
//=============================================================================
class DBTest : public CPPUNIT_NS::TestFixture {
//=============================================================================
	CPPUNIT_TEST_SUITE( DBTest );
	CPPUNIT_TEST( testExecute );
	CPPUNIT_TEST( testExecuter );
	CPPUNIT_TEST( testParameterBinding );
	CPPUNIT_TEST/*_EXCEPTION*/( testMultipleFiles/*, cpsqlite::DataBaseLocationIsLink*/ );
	CPPUNIT_TEST_SUITE_END();
private:
public:
	void setUp(){}
	void tearDown(){}
	void testMultipleFiles();
	void testExecute();
	void testExecuter();
	void testConstructor();
	void testParameterBinding();
	virtual ~DBTest();
};
} // namespace tests
#endif 
