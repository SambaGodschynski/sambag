#ifndef SAMBAG_PARAMETER_H_
#define SAMBAG_PARAMETER_H_

#include <sqlite3.h>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

namespace sambag {
namespace cpsqlite {
using namespace std;
//============================================================================================================
// Class IParameter : 
// to be used in that way: http://www.sqlite.org/c3ref/bind_blob.html
//============================================================================================================
struct IParameter {
	typedef boost::shared_ptr<IParameter> Ptr;
	virtual int bind ( sqlite3_stmt* ) = 0;
};
typedef list<IParameter::Ptr> ParameterList;
struct NullType {};
//============================================================================================================
// Class Parameter : 
//============================================================================================================
template <typename T>
struct Parameter : IParameter {
	typedef T ValueType;
	typedef boost::shared_ptr< Parameter<ValueType> > Ptr;
	T value;
	size_t index;
	Parameter( size_t index, const T &value ) : value(value), index(index) {}
	virtual int bind ( sqlite3_stmt* ) = 0;
};
//============================================================================================================
// Class Parameter : 
//============================================================================================================
template <>
struct Parameter<NullType> : IParameter {
	typedef NullType ValueType;
	typedef boost::shared_ptr< Parameter<ValueType> > Ptr;
	size_t index;
	Parameter( size_t index ) : index(index) {}
	virtual int bind ( sqlite3_stmt* ) = 0;
};
//============================================================================================================
// Class TextParameter : 
//============================================================================================================
struct NullParameter : Parameter<NullType> {
	typedef Parameter<NullType> ParameterType;
	typedef boost::shared_ptr< ParameterType > Ptr;
	NullParameter( size_t index ) : ParameterType(index) {}
	static Ptr create( size_t index ) { return Ptr( new NullParameter( index ) ); }
	virtual int bind ( sqlite3_stmt* stmt ) {
		return sqlite3_bind_null( stmt, index );
	}
};
//============================================================================================================
// Class DoubleParameter : 
//============================================================================================================
struct DoubleParameter : Parameter<double> {
	typedef Parameter<double> ParameterType;
	typedef boost::shared_ptr< ParameterType > Ptr;
	DoubleParameter( size_t index, const ParameterType::ValueType &value ) : ParameterType(index, value) {}
	static Ptr create( size_t index, const ParameterType::ValueType &value ) { 
		return Ptr( new DoubleParameter( index, value ) ); 
	}
	virtual int bind ( sqlite3_stmt* stmt) {
		return sqlite3_bind_double( stmt, index, value );
	}
};
//============================================================================================================
// Class IntParameter : 
//============================================================================================================
struct IntParameter : Parameter<int> {
	typedef Parameter<int> ParameterType;
	typedef boost::shared_ptr< ParameterType > Ptr;
	IntParameter( size_t index, const ParameterType::ValueType &value ) : ParameterType(index, value) {}
	static Ptr create( size_t index, const ParameterType::ValueType &value ) { 
		return Ptr( new IntParameter( index, value ) ); 
	}
	virtual int bind ( sqlite3_stmt* stmt) {
		return sqlite3_bind_int( stmt, index, value );
	}
};
//============================================================================================================
// Class Int64Parameter : 
//============================================================================================================
struct Int64Parameter : Parameter<sqlite3_int64> {
	typedef Parameter<sqlite3_int64> ParameterType;
	typedef boost::shared_ptr< ParameterType > Ptr;
	Int64Parameter( size_t index, const ParameterType::ValueType &value ) : ParameterType(index, value) {}
	static Ptr create( size_t index, const ParameterType::ValueType &value ) { 
		return Ptr( new Int64Parameter( index, value ) ); 
	}
	virtual int bind ( sqlite3_stmt* stmt) {
		return sqlite3_bind_int64( stmt, index, value );
	}
};
//============================================================================================================
// Class ValueParameter : 
//============================================================================================================
struct ValueParameter : Parameter<sqlite3_value*> {
	typedef Parameter<sqlite3_value*> ParameterType;
	typedef boost::shared_ptr< ParameterType > Ptr;
	ValueParameter( size_t index, const ParameterType::ValueType &value ) : ParameterType(index, value) {}
	static Ptr create( size_t index, const ParameterType::ValueType &value ) { 
		return Ptr( new ValueParameter( index, value ) ); 
	}
	virtual int bind ( sqlite3_stmt* stmt) {
		return sqlite3_bind_value( stmt, index, value );
	}
};
//============================================================================================================
// Class TextParameter : 
//============================================================================================================
struct TextParameter : Parameter<string> {
	typedef Parameter<string> ParameterType;
	typedef boost::shared_ptr< ParameterType > Ptr;
	TextParameter( size_t index, const ParameterType::ValueType &value ) : ParameterType(index, value) {}
	static Ptr create( size_t index, const ParameterType::ValueType &value ) { 
		return Ptr( new TextParameter( index, value ) ); 
	}
	virtual int bind ( sqlite3_stmt* stmt) {
		return sqlite3_bind_text( stmt, index, value.c_str(), value.size(), SQLITE_TRANSIENT );
	}
};
} // namepsace cpsqlite
} // namespace sambag

#endif
