/*
 * LuaScript.hpp
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#include "LuaHelper.hpp"
#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <lua.hpp>
#include <sambag/com/Helper.hpp>
#include <loki/Typelist.h>
#include <sambag/com/exceptions/IllegalArgumentException.hpp>

#ifndef LUASCRIPT_HPP_
#define LUASCRIPT_HPP_

namespace sambag { namespace lua {
//=============================================================================
/**
 *  @brief registerFunction
 *
 *	Usage:
 *	// Template parameter FunctionTag concept example 01:
 *	struct FooFunction_tag {
 *		typedef boost::function< void ( int, float ) > Function; // Function Type
 *		static const char * { return "foo"; } // lua function name to register
 *	};
 *	// example 02
 *	struct BarFunction_tag {
 *		typedef boost::function< void () > Function;
 *		static const char * { return "bar"; }
 *	};
 *
 *	lua_State *L;
 *
 *	// init lua
 *  // ...
 *	
 *  // register c/c++ function 
 *	registerFunction<FooFunction_tag>(
 *		L,
 *		boost::bind(&Dummy::foo, &dummy, _1, _2)
 *	);
 *
 *	registerFunction<BarFunction_tag>(
 *		L,
 *		boost::bind(&Dummy::bar, &dummy, _1, _2)
 *	);
 *
 *
 * 	Notes:
 * 		- Lua callback can throw ExecutionFailed
 * 		  while getting arguments from lua stack.
 * 		- max. number of Function args = 5
 * @param f boost::function object
 */
template <class FunctionTag>
void registerFunction( lua_State *L,
	const typename FunctionTag::Function &f);


namespace {
//=============================================================================
template <typename ReturnType>
struct IsVoid {
//=============================================================================
enum {Value=0};
};
template <>
struct IsVoid<void> {
	enum {Value=1};
};
//=============================================================================
template <typename ReturnType>
struct NumReturnValues {
//=============================================================================
enum {Value=1};
};
template <>
struct NumReturnValues<void> {
	enum {Value=0};
};
template <
	class T0, class T1, class T2,
	class T3, class T4, class T5,
	class T6, class T7, class T8, class T9
>
struct NumReturnValues<
	boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> 
> 
{
	enum {Value=
		boost::tuples::length< 
			boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> 
		>::value
	};
};
//=============================================================================
//-----------------------------------------------------------------------------
template <typename Function>
void throwArgumentsMismatch(lua_State *L) {
	throw ExecutionFailed("function arguments mismatch");
}
//=============================================================================
// NumArgs: 0
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<0>, // num args
		com::Int2Type<0>) // is void?
{
	push(L, f());
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<0>, // num args
		com::Int2Type<1>) // is void?
{
	f();
}
//=============================================================================
// NumArgs: 1
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<1>, // num args
		com::Int2Type<0>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	push(L, f(
		boost::get<0>(args)
	));
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<1>, // num args
		com::Int2Type<1>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	f(
		boost::get<0>(args)
	);
}
//=============================================================================
// NumArgs: 2
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<2>, // num args
		com::Int2Type<0>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	push(L, f(
		boost::get<1>(args),
		boost::get<0>(args)
	));
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<2>, // num args
		com::Int2Type<1>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	f(
		boost::get<1>(args),
		boost::get<0>(args)
	);
}
//=============================================================================
// NumArgs: 3
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<3>, // num args
		com::Int2Type<0>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg3_type,
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	push(L, f(
		boost::get<2>(args),
		boost::get<1>(args),
		boost::get<0>(args)
	));
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<3>, // num args
		com::Int2Type<1>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg3_type,
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	f(
		boost::get<2>(args),
		boost::get<1>(args),
		boost::get<0>(args)
	);
}
//=============================================================================
// NumArgs: 4
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<4>, // num args
		com::Int2Type<0>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg4_type,
		typename Function::arg3_type,
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	push(L, f(
		boost::get<3>(args),
		boost::get<2>(args),
		boost::get<1>(args),
		boost::get<0>(args)
	));
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<4>, // num args
		com::Int2Type<1>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg4_type,
		typename Function::arg3_type,
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	f(
		boost::get<3>(args),
		boost::get<2>(args),
		boost::get<1>(args),
		boost::get<0>(args)
	);
}
//=============================================================================
// NumArgs: 5
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<5>, // num args
		com::Int2Type<0>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg5_type,
		typename Function::arg4_type,
		typename Function::arg3_type,
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	push(L, f(
		boost::get<4>(args),
		boost::get<3>(args),
		boost::get<2>(args),
		boost::get<1>(args),
		boost::get<0>(args)
	));
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		com::Int2Type<5>, // num args
		com::Int2Type<1>) // is void?
{
	// get argument(s) from stack
	boost::tuple<
		typename Function::arg5_type,
		typename Function::arg4_type,
		typename Function::arg3_type,
		typename Function::arg2_type,
		typename Function::arg1_type
	> args;
	if (!pop(L, args))
		throwArgumentsMismatch<Function>(L);
	// call
	f(
		boost::get<4>(args),
		boost::get<3>(args),
		boost::get<2>(args),
		boost::get<1>(args),
		boost::get<0>(args)
	);
}
//=============================================================================
template <class FunctionTag>
struct RegisterHelperClass {
//=============================================================================
	typedef typename FunctionTag::Function Function;
	typedef std::map<lua_State*, Function> FMap;
	static FMap fMap;
	static int luaCallback(lua_State *L) {
		typename FMap::iterator it = fMap.find(L);
		if (it == fMap.end()) {
			throw ExecutionFailed(
					std::string("function callback failed with: ") + FunctionTag::name()
			);
		}
		enum {IsVoidValue = IsVoid<typename Function::result_type>::Value };
		callF<Function>( it->second,
			L,
			com::Int2Type<Function::arity>(), // num args
			com::Int2Type<IsVoidValue>() // isVoid
		);
		return NumReturnValues<typename Function::result_type>::Value;
	}
};
template <class FunctionTag>
std::map<lua_State*, typename FunctionTag::Function>
RegisterHelperClass<FunctionTag>::fMap;
} // namespace

//=============================================================================
// LuaScript Template methods
//=============================================================================
//-----------------------------------------------------------------------------
template <class FunctionTag>
void registerFunction(
	lua_State *L,
	const typename FunctionTag::Function &f)
{
	typedef RegisterHelperClass<FunctionTag> Helper;
	Helper::fMap[L] = f;
	lua_register(L, FunctionTag::name(), &Helper::luaCallback);
}
//-----------------------------------------------------------------------------
namespace {
    template<class It,
        class Functions,
        class FunctionsAccessor,
        class FunctionTagList=It>
    struct __AddFunc {
        static void add(lua_State *L, const Functions &functions, int top) {
            typedef typename It::Head T;
            typedef typename It::Tail Next;
            typedef RegisterHelperClass<T> Helper;
            enum { Index = Loki::TL::IndexOf<FunctionTagList, T>::value };
            FunctionsAccessor::template get<Index>(functions, Helper::fMap[L]);
            lua_pushstring(L, T::name());
            lua_pushcfunction(L, &Helper::luaCallback);
            lua_settable(L, top);
            __AddFunc<Next, Functions, FunctionsAccessor, FunctionTagList>::add(L, functions, top);
        }
        static void add(lua_State *L, const Functions &functions) {
            typedef typename It::Head T;
            typedef typename It::Tail Next;
            enum { Index = Loki::TL::IndexOf<FunctionTagList, T>::value };
            typename T::Function f;
            FunctionsAccessor::template get<Index>(functions, f);
            registerFunction<T>(L, f);
            __AddFunc<Next, Functions, FunctionsAccessor, FunctionTagList>::add(L, functions);
        }
        static void add(lua_State *L, const Functions &functions, luaL_Reg *reg)
        {
            typedef typename It::Head T;
            typedef typename It::Tail Next;
            enum { Index = Loki::TL::IndexOf<FunctionTagList, T>::value };
            typedef RegisterHelperClass<T> Helper;
            FunctionsAccessor::template get<Index>(functions, Helper::fMap[L]);
            reg[Index].name = T::name();
            reg[Index].func = &Helper::luaCallback;
            __AddFunc<Next, Functions, FunctionsAccessor, FunctionTagList>::add(L, functions, reg);
        }
    };
    template<class Functions,
        class FunctionsAccessor,
        class FunctionTagList>
    struct __AddFunc<Loki::NullType,
        Functions,
        FunctionsAccessor,
        FunctionTagList>
    {
        static void add(lua_State *L, const Functions &functions, int top) {}
        static void add(lua_State *L, const Functions &functions) {}
        static void add(lua_State *L, const Functions &functions, luaL_Reg *reg){}
    };
    
}

/**
 * @accessor for tuples used in @see registerFunction() when Functions
 * are stored in a tuple
 */
template <class Tuple>
struct TupleAccessor {
    template <int Index, class Result>
    static void get(const Tuple &tuple, Result &out) {
        out = boost::get<Index>(tuple);
    }
};


/**
 * @brief add functions to table on stack index
 */
template <class FunctionTagList,
    template <class> class FunctionsAccessor,
    class Functions
>
void registerFunctions(lua_State *L, const Functions &f, int index) {
    if (lua_istable(L, -1) != 1) {
        SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
        "registerFunctions: index not a table");
    }
    __AddFunc<FunctionTagList, Functions, FunctionsAccessor<Functions> >::add(L, f, index);
}

/**
 * @brief finds or creates table and add functions to it
 */
template <class FunctionTagList,
    template <class> class FunctionsAccessor,
    class Functions
>
void registerFunctions(lua_State *L, const Functions &f, const std::string &modulename) {
    lua_getglobal(L, modulename.c_str());
    if (lua_istable(L, -1) != 1) {
        lua_newtable(L);
        int top = lua_gettop(L);
        __AddFunc<FunctionTagList, Functions, FunctionsAccessor<Functions> >::add(L, f, top);
        lua_setglobal(L, modulename.c_str());
        return;
    }
    int top = lua_gettop(L);
    __AddFunc<FunctionTagList, Functions, FunctionsAccessor<Functions> >::add(L, f, top);
}


/**
 * @brief register as global functions
 */
template <class FunctionTagList,
    template <class> class FunctionsAccessor,
    class Functions
>
void registerFunctions(lua_State *L, const Functions &f)
{
    __AddFunc<FunctionTagList, Functions, FunctionsAccessor<Functions> >::add(L, f);
}
//-----------------------------------------------------------------------------
/**
 * @brief register functions as class
 * @return lua stack index of created table
 */
template <class FunctionTagList,
    template <class> class FunctionsAccessor,
    class Functions
>
int createClass(lua_State *L, const Functions &functions, const std::string &name) {
// https://stackoverflow.com/questions/11100435/how-do-i-create-a-class-object-in-lua-c-api-5-2
    int lib_id, meta_id;
    enum {Size = Loki::TL::Length<FunctionTagList>::value};
    static luaL_Reg fs[Size+1] = {{0}};
    
    __AddFunc<FunctionTagList, Functions, FunctionsAccessor<Functions> >::add(L, functions, fs);
    
    /* newclass = {} */
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);

    /* metatable = {} */
    luaL_newmetatable(L, name.c_str());
    meta_id = lua_gettop(L);

    /* metatable.__index = _methods */
    luaL_newlib(L, fs);
    lua_setfield(L, meta_id, "__index");

    /* class.__metatable = metatable */
    lua_setmetatable(L, lib_id);
    
    return lib_id;
}
//-----------------------------------------------------------------------------
/**
 * @brief register functions as class
 * @return lua stack index of created table
 */
template <class FunctionTagList,
    class MetaFunctionTagList,
    template <class> class FunctionsAccessor,
    class Functions,
    class MetaFunctions
>
int createClass(lua_State *L,
    const Functions &functions,
    const MetaFunctions &mFunctions,
    const std::string &name)
{
// https://stackoverflow.com/questions/11100435/how-do-i-create-a-class-object-in-lua-c-api-5-2
    int lib_id, meta_id;
    enum {Size = Loki::TL::Length<FunctionTagList>::value};
    enum {MSize = Loki::TL::Length<MetaFunctionTagList>::value};
    static luaL_Reg fs[Size+1] = {{0}};
    static luaL_Reg m_fs[MSize+1] = {{0}};
    
    __AddFunc<FunctionTagList, Functions, FunctionsAccessor<Functions> >::add(L, functions, fs);
    __AddFunc<MetaFunctionTagList, MetaFunctions, FunctionsAccessor<MetaFunctions> >::add(L, mFunctions, m_fs);
    
    /* newclass = {} */
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);

    /* metatable = {} */
    luaL_newmetatable(L, name.c_str());
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, m_fs, 0);

    /* metatable.__index = _methods */
    luaL_newlib(L, fs);
    lua_setfield(L, meta_id, "__index");

    /* metatable.__metatable = _meta */
    luaL_newlib(L, m_fs);
    lua_setfield(L, meta_id, "__metatable");

    /* class.__metatable = metatable */
    lua_setmetatable(L, lib_id);
    
    return lib_id;
}
//-----------------------------------------------------------------------------
/**
 * @brief register functions as class
 * @return lua stack index of created table
 */
inline int createClass(lua_State *L, const std::string &name) {
    int lib_id, meta_id;
    
    /* newclass = {} */
    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);

    /* metatable = {} */
    luaL_newmetatable(L, name.c_str());
    meta_id = lua_gettop(L);
    //luaL_setfuncs(L, _meta, 0);

    /* class.__metatable = metatable */
    lua_setmetatable(L, lib_id);
    
    return lib_id;
}

}} // namespace
#endif /* LUASCRIPT_HPP_ */
