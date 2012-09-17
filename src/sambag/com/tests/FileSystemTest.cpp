#include <cppunit/config/SourcePrefix.h>
#include "FileSystemTest.hpp"
#include "sambag/com/FileSystem.hpp"
#include <list>
#include <stack>
#include <iostream>
#include <boost/assign/std/list.hpp>
#include <boost/algorithm/string.hpp>
#include <string>

// Registers the fixture into the 'registry'
//CPPUNIT_TEST_SUITE_REGISTRATION( tests::ComTests );

using namespace sambag;

const std::string FILENAME = boost::algorithm::erase_all_copy( std::string(__FILE__), std::string("../") );

static com::Location testFolders;

//=============================================================================
// compares directory iteration with ExpectedTree;
struct WalkerVis : public com::IWalkerVisitor {
//=============================================================================
	//-------------------------------------------------------------------------
	bool abort;
	//-------------------------------------------------------------------------
	bool abortAfter2Files;
	//-------------------------------------------------------------------------
	size_t numDirs, numFiles;
	//-------------------------------------------------------------------------
	WalkerVis( bool abortAfter2Files = false) : 
		abort(false), abortAfter2Files(abortAfter2Files), numDirs(0), numFiles(0) {}
	//-------------------------------------------------------------------------	
	// called while starting iterate path. If returned with false scanning path will be skipped.
	virtual bool changeDirectory ( const com::Location & path ) {	
		using namespace std;		
		if ( path.filename() == ".svn" ) return false;			
		++numDirs;
		return true;
	}
	//------------------------------------------------------------------------
	// called when visiting new file. 
	virtual void file ( const com::Location & file ) {
		if ( ++numFiles == 2 ) abort = true;	
	}
};

namespace tests {
//=============================================================================
void ComTests::setUp() {
	static bool init = false;
	if (init)
		return;
	init = true;
	testFolders = boost::filesystem::absolute(
		com::Location(FILENAME).parent_path().string() + std::string("/TestFolders")
	);
}
//=============================================================================
void ComTests::testDirWalker() {
//=============================================================================
	using namespace sambag;	
	WalkerVis vis;
	CPPUNIT_ASSERT_MESSAGE ( "TestFolder tree expected.", exists(testFolders) );
	com::dirWalker ( testFolders, vis );
	CPPUNIT_ASSERT_EQUAL ( (size_t)4, vis.numDirs );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10, vis.numFiles );
}
//=============================================================================
void ComTests::testDirWalkerAbort() {
//=============================================================================
	using namespace sambag;	
	WalkerVis vis(true);
	CPPUNIT_ASSERT_MESSAGE ( "TestFolder tree expected.", exists(testFolders) );
	com::dirWalker ( testFolders, vis, &vis.abort/*switches to true after 2 files passed*/);
	CPPUNIT_ASSERT_EQUAL ( (size_t)2, vis.numFiles );
}
} // namespace tests
