#include <cppunit/config/SourcePrefix.h>
#include "DBTest.hpp"
#include <boost/filesystem.hpp>
#include "sambag/com/FileSystem.hpp"
#include <boost/assign/std/list.hpp> 
#include <map>
#include <limits.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::DBTest );

const std::string DB_LOCATION = "sqlite3Database.file";

const std::string INIT_DB = std::string ("CREATE TABLE IF NOT EXISTS folder ( ") +
	"id INTEGER PRIMARY KEY AUTOINCREMENT, " +
	"parentFolder INTEGER,"
	"name TEXT, " + 
	"FOREIGN KEY(parentFolder) REFERENCES folder(id) ON UPDATE CASCADE " +
	"ON DELETE CASCADE," + 
	"UNIQUE ( id, name ) );";

const std::string INSERT_DB = std::string ("INSERT INTO folder(name) VALUES ('root');") +
	("INSERT INTO folder(name, parentFolder) VALUES ('neu', 1);");
 
namespace {
// initalisiert DB
void initTestDB ( sambag::cpsqlite::DataBase::Ptr db ) {
	sambag::cpsqlite::DataBase::Executer::Ptr ex = db->getExecuter();
	ex->execute( INIT_DB );
}
} // namespace


namespace tests {
//=============================================================================
void DBTest::testExecute() {
//=============================================================================
	using namespace std;
	using namespace sambag;
	using namespace cpsqlite;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> init DB.
	CPPUNIT_ASSERT ( !boost::filesystem::exists( DB_LOCATION ) );
	DataBase::Ptr db = DataBase::getDataBase( DB_LOCATION );
	DataBase::Executer::Ptr ex = db->getExecuter();
	DataBase::Results results;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> folder ist leer
	initTestDB ( db );
	ex->execute( "SELECT * FROM folder;", results );
	CPPUNIT_ASSERT ( results.size() == 0 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> datensaetze einfuegen
	ex->execute( INSERT_DB );
	ex->execute("SELECT * FROM folder;", results);
	CPPUNIT_ASSERT_EQUAL ( (size_t)2, results.size() );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> datensaetze checken
	CPPUNIT_ASSERT_EQUAL ( std::string("1"), results[0]->get("ID") );
	CPPUNIT_ASSERT_EQUAL ( std::string("1"), results[0]->get("id") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)1, results[0]->getConv<size_t>("id") );
	CPPUNIT_ASSERT_EQUAL ( std::string("root"), results[0]->get("name") );
	CPPUNIT_ASSERT_EQUAL ( std::string("NULL"), results[0]->get("parentFolder") );
	CPPUNIT_ASSERT_EQUAL ( std::string("2"), results[1]->get("id") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)2, results[1]->getConv<size_t>("id") );
	CPPUNIT_ASSERT_EQUAL ( std::string("neu"), results[1]->get("name") );
	CPPUNIT_ASSERT_EQUAL ( std::string("1"), results[1]->get("parentFolder") );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> covertion-return-null
	CPPUNIT_ASSERT_EQUAL ( (int)INT_MAX, results[0]->getConv<int>("name", INT_MAX) );
	CPPUNIT_ASSERT_EQUAL ( (double)-1.0, results[0]->getConv<double>("name", -1.0) );
	CPPUNIT_ASSERT_EQUAL ( (double)2.0, results[1]->getConv<double>("id", -1.0) );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>foreign key cascade testen
	ex->execute("DELETE FROM folder WHERE id = 1;");
	ex->execute("SELECT * FROM folder;", results);
	CPPUNIT_ASSERT_EQUAL ( (size_t)0, results.size() );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>foo
	CPPUNIT_ASSERT_THROW ( ex->execute("foo;"), DataBaseException );

}
const std::string INIT_DB2 = std::string ("CREATE TABLE IF NOT EXISTS dates ( ") +
	"id INTEGER PRIMARY KEY AUTOINCREMENT, " +
	"int_val INTEGER NOT NULL, " +
	"text_val TEXT NOT NULL, " +
	"null_val TEXT NULL, " +
	"real_val REAL NOT NULL );";
//=============================================================================
void DBTest::testParameterBinding() {
//=============================================================================
	using namespace std;
	using namespace sambag;
	using namespace cpsqlite;
	using namespace boost::assign;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> init DB.
	DataBase::Ptr db = DataBase::getDataBase( DB_LOCATION );
	DataBase::Executer::Ptr ex = db->getExecuter();
	DataBase::Results results;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create tables
	try {	
		ex->execute( INIT_DB2 );
	} catch ( DataBaseQueryFailed &ex ) {
		CPPUNIT_ASSERT_MESSAGE ( ex.errorMessage, false );	
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>some binding parameter;
	size_t index = 1;
	ParameterList pl;
	pl += IntParameter::create ( index++, 15);
	pl += TextParameter::create( index++, "text in deutsch." );
	pl += NullParameter::create( index++ );
	pl += DoubleParameter::create( index++, 0.0012345 );
	//pl += Int64Parameter::create(4, 15);
	//pl += ValueParameter::create(5, NULL);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>insert data
	string q = "INSERT INTO dates ( int_val, text_val, null_val, real_val ) VALUES ( ?, ?, ?, ? );";
	try {	
		ex->execute ( q, pl ); 
	} catch ( DataBaseQueryFailed &ex ) {
		CPPUNIT_ASSERT_MESSAGE ( ex.errorMessage, false );	
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>get folder
	q = "SELECT * FROM dates WHERE id = 1;";
	try {	
		ex->execute ( q, pl, results ); 
	} catch ( DataBaseQueryFailed &ex ) {
		CPPUNIT_ASSERT_MESSAGE ( ex.errorMessage, false );	
	}
	CPPUNIT_ASSERT ( !results.empty() );
	CPPUNIT_ASSERT_EQUAL ( string("15"), results[0]->get("int_val") );
	CPPUNIT_ASSERT_EQUAL ( string("text in deutsch."), results[0]->get("text_val") );
	CPPUNIT_ASSERT_EQUAL ( string("NULL"), results[0]->get("null_val") );
	CPPUNIT_ASSERT_EQUAL ( string("0.0012345"), results[0]->get("real_val") );
	
}
//=============================================================================
void DBTest::testExecuter() {
//=============================================================================
	using namespace std;
	using namespace sambag;
	using namespace cpsqlite;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> init DB.
	DataBase::Ptr db = DataBase::getDataBase ( "testDB4.db" );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>getExecuter
	DataBase::Executer::Ptr ex1 = db->getExecuter();
	CPPUNIT_ASSERT ( ex1 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>get another Executer
	DataBase::Executer::Ptr ex2 = db->getExecuter();
	CPPUNIT_ASSERT_EQUAL ( ex1, ex2 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>release ex1
	ex1.reset();
	CPPUNIT_ASSERT_EQUAL( (DataBase::Int)0, ex2->lastInsertRowId() );
}
//=============================================================================
void DBTest::testMultipleFiles() { 
//=============================================================================
	using namespace std;
	using namespace sambag;
	using namespace cpsqlite;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>gleiche locs.
	DataBase::Ptr db = DataBase::getDataBase ( "testDB1.db" );
	DataBase::Ptr db2 = DataBase::getDataBase( "testDB1.db" );
	CPPUNIT_ASSERT ( db == db2 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>ungleiche locs.
	db2 = DataBase::getDataBase( "testDB2.db" );
	CPPUNIT_ASSERT ( db != db2 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>gleiche locs. aber andere pfadangabe
	std::string loc = boost::filesystem::initial_path().string() + "/testDB1.db";
	db2 = DataBase::getDataBase( loc );
	CPPUNIT_ASSERT_MESSAGE ( loc, db == db2 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>getNumDataBases == 0
	db2.reset();
	db.reset();
	CPPUNIT_ASSERT ( DataBase::getNumDataBases() == 0 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>loc = syslnk
	//boost::filesystem::create_symlink( "testDB3.db", "testDB2.db" ); // wird nicht von win unterst.
	//db2 = DataBase::getDataBase( "testDB3.db" ); // EXCEPTION
}
//=============================================================================
DBTest::~DBTest() {
//=============================================================================
	boost::filesystem::remove( DB_LOCATION );
}
} // namespace tests
