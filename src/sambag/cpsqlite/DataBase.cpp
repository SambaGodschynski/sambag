#include "DataBase.hpp"
#include <boost/algorithm/string.hpp> 
#include <list>


typedef void (*BindingCallback)( sqlite3_stmt *pStmt, void* ptrParameterList ); /* callback to execute bindig */
extern "C" int sambag_sqlite3_exec (
  sqlite3 *db,                /* The database on which the SQL executes */
  const char *zSql,           /* The prepared statement to be executed */
  sqlite3_callback xCallback, /* Invoke this callback routine */
  void *pArg,                 /* First argument to xCallback() */
  char **pzErrMsg,             /* Write error messages here */
  BindingCallback bindingCallback, /* callback to execute bindig */
  void *parameterList		      /* binding callback parameter list */
);

namespace {
	//========================================================================================================
	int sqliteCallback ( void *resultsPtr, int argc, char **argv, char **azColName );
	//========================================================================================================
	sambag::cpsqlite::DataBase::Location2DataBasePtr _loc2DB( 
		new sambag::cpsqlite::DataBase::Location2DataBase()
	);
	//========================================================================================================
	typedef std::list<sambag::cpsqlite::DataBase::Result::Ptr> ResultList;
	//========================================================================================================
	void bindingCallback ( sqlite3_stmt *pStmt, void* ptrParameterList );
} // namespace

namespace sambag {
namespace cpsqlite {
//============================================================================================================
// class DataBase::Result 
//============================================================================================================
//------------------------------------------------------------------------------------------------------------
const DataBase::Result::Entry DataBase::Result::NULL_ENTRY = "NULL";
//------------------------------------------------------------------------------------------------------------
const DataBase::Result::Entry & DataBase::Result::get( const Column &col ) const {
	if ( col2Entry.empty() ) return NULL_ENTRY;
	string in = col;
	boost::to_lower(in);
	Column2Entry::const_iterator it = col2Entry.find( in );
	if ( it == col2Entry.end() ) return NULL_ENTRY;
	return it->second;
}
//============================================================================================================
// class DataBase : 
//============================================================================================================
//------------------------------------------------------------------------------------------------------------
DataBase::Result::Entry NULL_ENTRY = "NULL";
//------------------------------------------------------------------------------------------------------------
DataBase::DataBase(const Path &dbLocation) : location(dbLocation) {
	dieLate = _loc2DB;
}
//------------------------------------------------------------------------------------------------------------
DataBase::~DataBase() {
	//if ( dieLate->empty() ) return;
	Location2DataBase::iterator it = dieLate->find (location);
	assert ( it!=dieLate->end() );
	dieLate->erase (it);
}
//------------------------------------------------------------------------------------------------------------
size_t DataBase::getNumDataBases() {
	return _loc2DB->size();
}
//------------------------------------------------------------------------------------------------------------
DataBase::Ptr DataBase::getDataBase(  const std::string &dbLocation ) {
	using namespace boost::filesystem;
	// checke location
	Path loc(dbLocation);
	loc = complete (loc);
	// besser waere => boost::filesystem::read_symlink(). erst ab boost 1.46.1
	if ( is_symlink( loc ) ) throw DataBaseLocationIsLink();
	// location schon vorhanden?
	Location2DataBase::iterator it = _loc2DB->find (loc);
	if ( it!=_loc2DB->end() ) { // ja: liefer ptr
		DataBase::Ptr ptr= it->second.lock();
		assert( ptr );
		return ptr;
	}
	
	// nein: erzeuge neue db
	Ptr neu( new DataBase( loc.string() ) );
	neu->self = neu;
	_loc2DB->insert( pair<U, V> ( loc, neu ) );
	return neu;
}
//------------------------------------------------------------------------------------------------------------
DataBase::Executer::Ptr DataBase::getExecuter() {
	Executer::Ptr tmp = executer.lock();
	if ( tmp ) return tmp;
	executer = tmp = Executer::Ptr ( new Executer( this ) );
	return tmp;
}
//============================================================================================================
// Class Executer : <Singleton> => one per database
// connects to sqlite and executes sql commands on it.
//============================================================================================================
//------------------------------------------------------------------------------------------------------------
DataBase::Executer::Executer ( DataBase *parent ) : db(parent) {
	int rc = sqlite3_open( db->getLocation().string().c_str(), &sqlite ); // rc muss 0 sein
	if ( rc != SQLITE_OK ) {
		sqlite3_close(sqlite);
		throw DataBaseConnectionFailed();
	}
	try {
		// setup db
		execute("PRAGMA foreign_keys=ON;");
        execute("PRAGMA synchronous = 1");
	} catch ( DataBaseQueryFailed ex ) {
		sqlite3_close(sqlite);
		throw DataBaseConnectionFailed();
	}
}
//------------------------------------------------------------------------------------------------------------
DataBase::Executer::~Executer () {
	sqlite3_close(sqlite);
}
//------------------------------------------------------------------------------------------------------------
void DataBase::Executer::_execute ( const std::string &query, DataBase::Results &out_results, ParameterList &pL ) {
	
	if ( query.size() > /*SQLITE_MAX_SQL_LENGTH=*/ 1000000000 ) 
		throw DataBaseQueryFailed ( INT_MAX, "query size overflow.", query );

	out_results.clear();

	char *zErrMsg = 0;
	// prepare resultlist
	ResultList res;
	BindingCallback clbk = pL.empty() ? NULL : bindingCallback;
	int rc = sambag_sqlite3_exec(sqlite, query.c_str(), sqliteCallback, &res, &zErrMsg, clbk, &pL);
	// clear parameter list
	pL.clear();
	// check result
	if( rc!=SQLITE_OK ) {
		if (zErrMsg==0) {
			throw DataBaseQueryFailed( rc, "unknown reaon.", query );
		}
		std::string msg(zErrMsg);
		msg = "SQLite3 query failed: " + msg;
		sqlite3_free(zErrMsg);
		throw DataBaseQueryFailed( rc, msg, query );
	}
	// copy resultlist => out_results
	out_results.reserve( res.size() );
	ResultList::iterator it = res.begin();
	for ( ; it!=res.end(); ++it ) {
		out_results.push_back( *it );
	}
}
} // cpsqlite namepsace
} // sambag namespace

namespace {
//============================================================================================================
int sqliteCallback ( void *resultsPtr, int argc, char **argv, char **azColName ) {
//============================================================================================================
	using namespace sambag;
	using namespace cpsqlite;
	typedef DataBase::Result::Column ColumnType;
	typedef DataBase::Result::Entry EntryType;
    ResultList *results = static_cast<ResultList*> (resultsPtr);
	DataBase::Result::Ptr res( new DataBase::Result() );
	results->push_back(res);
	for ( int i=0; i<argc; ++i ) {
		ColumnType col( azColName[i] );
		EntryType entry( argv[i] ? argv[i] : DataBase::Result::NULL_ENTRY );
		boost::to_lower(col);
		res->col2Entry.insert( pair<ColumnType, EntryType>( col, entry ) );
	}
	return 0;
}
//========================================================================================================
void bindingCallback ( sqlite3_stmt *pStmt, void* ptrParameterList ) {
	using namespace sambag::cpsqlite;
	ParameterList *pL = static_cast<ParameterList*>( ptrParameterList );
	if ( !pL || !pStmt || pL->empty() ) return;
	ParameterList::iterator it=pL->begin();
	for ( ; it!=pL->end(); ++it ) {
		(*it)->bind( pStmt );	
	}
}
} // anonyn. namespace
