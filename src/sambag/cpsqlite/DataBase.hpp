#ifndef SAMBAG_DATABASE_H_
#define SAMBAG_DATABASE_H_

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "sambag/com/FileSystem.hpp"
#include "sqlite3.h"
#include <string>
#include <sstream>
#include <map> 
#include <boost/unordered_map.hpp>
#include "Parameter.hpp"

namespace sambag {
namespace cpsqlite {
using namespace std;
//============================================================================================================
// Class DataBase Exceptions
//============================================================================================================
struct DataBaseException {};
struct DataBaseConnectionFailed : public DataBaseException {};
struct DataBaseLocationIsLink : public DataBaseException {};
struct DataBaseQueryTimeout : public DataBaseException {};
struct DataBaseQueryFailed : public DataBaseException {
	typedef int ResultCode;
	typedef std::string ErrorMessage;
	typedef std::string Query;
	ErrorMessage errorMessage;
	ResultCode resultCode;
	Query query;
	DataBaseQueryFailed ( const ResultCode &resultCode, 
		                  const ErrorMessage &errorMessage,
						  const Query query ) : 
		errorMessage(errorMessage), resultCode(resultCode), query(query) {}
};
//============================================================================================================
// Class DataBase : <Singleton>
// Wrapper for SQLITE3. For every db location exists one Database object.
//============================================================================================================
class DataBase {
public:
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	struct Result {
		typedef std::string Column;
		typedef std::string Entry;
		typedef boost::unordered_map<Column, Entry> Column2Entry; // unordered = mapping via hash
		typedef boost::shared_ptr<Result> Ptr;
		//...................................................................................................
		static const Entry NULL_ENTRY;
		//...................................................................................................
		Column2Entry col2Entry;
		//...................................................................................................
		const Entry & get( const Column &col ) const;
		//...................................................................................................
		template<typename T>
		T getConv( const Column &col ) const {
			stringstream ss;
			T ret;
			ss<<get(col);
			ss>>ret;
			return ret;
		}
		//...................................................................................................
		template<typename T>
		T getConv( const Column &col, const T &null /*returned if covertion failed*/ ) const {
			stringstream ss;
			T ret;
			ss<<get(col);
			ss>>ret;
			return ss.eof() ? ret : null;
		}
	};
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	class Executer;
	//--------------------------------------------------------------------------------------------------------
	static const size_t EXECUTE_LOCK_TIMEOUT = 10;
	//--------------------------------------------------------------------------------------------------------
	typedef com::Location Path;
	//--------------------------------------------------------------------------------------------------------
        typedef vector<Result::Ptr> Results;
	//--------------------------------------------------------------------------------------------------------
	typedef boost::shared_ptr<DataBase> Ptr;
	//--------------------------------------------------------------------------------------------------------
	typedef sqlite3_int64 Int;
	//--------------------------------------------------------------------------------------------------------
	typedef Path U;
	//--------------------------------------------------------------------------------------------------------
	typedef boost::weak_ptr<DataBase> V;
	//--------------------------------------------------------------------------------------------------------
	typedef std::map<U, V> Location2DataBase;
	//--------------------------------------------------------------------------------------------------------
	typedef boost::shared_ptr<Location2DataBase> Location2DataBasePtr;
private:
	//--------------------------------------------------------------------------------------------------------
	boost::weak_ptr<Executer> executer;
	//--------------------------------------------------------------------------------------------------------
	Path location;
	//--------------------------------------------------------------------------------------------------------
	// as static object => lifetime vague; problems with access in destructor
	Location2DataBasePtr dieLate;
	//--------------------------------------------------------------------------------------------------------
	boost::weak_ptr<DataBase> self;
	//--------------------------------------------------------------------------------------------------------
	DataBase( const Path &dbLocation );
public:
	boost::shared_ptr<Executer> getExecuter();
	//--------------------------------------------------------------------------------------------------------
	const Path & getLocation() const { return location; }
	//--------------------------------------------------------------------------------------------------------
	static size_t getNumDataBases();
	//--------------------------------------------------------------------------------------------------------
	virtual ~DataBase();
	//--------------------------------------------------------------------------------------------------------
	// erzeugt DataBase::Ptr oder liefert vorhandenen Ptr. Location darf kein Link sein.
	static Ptr getDataBase ( const std::string &dbLocation );
};
//============================================================================================================
// Class Executer : <Singleton> => one per database
// connects to sqlite and executes sql commands on it.
//============================================================================================================
class DataBase::Executer {
public:
	//--------------------------------------------------------------------------------------------------------
	typedef boost::shared_ptr<Executer> Ptr;
private:
	//--------------------------------------------------------------------------------------------------------
	DataBase *db;
	//--------------------------------------------------------------------------------------------------------
	sqlite3 *sqlite;	
	//--------------------------------------------------------------------------------------------------------
	void _execute ( const std::string &query, DataBase::Results &out_results, ParameterList &l );
public:
	//--------------------------------------------------------------------------------------------------------
	DataBase::Int lastInsertRowId() { return sqlite3_last_insert_rowid(sqlite); }
	//--------------------------------------------------------------------------------------------------------
	Executer ( DataBase *parent );
	//--------------------------------------------------------------------------------------------------------
	~Executer ();
	//--------------------------------------------------------------------------------------------------------
	void execute ( const std::string &query ) {
		DataBase::Results res;	
		ParameterList pL;	
		_execute ( query, res, pL );
	}
	//--------------------------------------------------------------------------------------------------------
	// out_results will be pre-cleared.
	void execute ( const std::string &query, DataBase::Results &out_results ) {
		ParameterList pL;	
		_execute ( query, out_results, pL );
	}
	//--------------------------------------------------------------------------------------------------------
	// executes with parameter binding; values in query has to replaced with '?' as placeholder
	void execute ( const std::string &query,       
		           ParameterList &pL ) 
	{
		DataBase::Results res;	
		_execute ( query, res, pL );		
	}
	//--------------------------------------------------------------------------------------------------------
	// executes with parameter binding; values in query has to replaced with '?' as placeholder
	// out_results will be pre-cleared.
	void execute ( const std::string &query,       
		           ParameterList &pL, 
				   DataBase::Results &out_results ) 
	{
		_execute ( query, out_results, pL );		
	}
};
} // namepsace cpsqlite
} // namespace sambag

#endif
