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
    template<class It, class FunctionTuple, class FunctionTagList=It>
    struct __AddFunc {
        static void add(lua_State *L, const FunctionTuple &tuple, int top) {
            typedef typename It::Head T;
            typedef typename It::Tail Next;
            typedef RegisterHelperClass<T> Helper;
            enum { Index = Loki::TL::IndexOf<FunctionTagList, T>::value };
            Helper::fMap[L] = boost::get<Index>(tuple);
            lua_pushstring(L, T::name());
            lua_pushcfunction(L, &Helper::luaCallback);
            lua_settable(L, top);
            __AddFunc<Next, FunctionTuple, FunctionTagList>::add(L, tuple, top);
        }
        static void add(lua_State *L, const FunctionTuple &tuple) {
            typedef typename It::Head T;
            typedef typename It::Tail Next;
            enum { Index = Loki::TL::IndexOf<FunctionTagList, T>::value };
            registerFunction<T>(L, boost::get<Index>(tuple));
            __AddFunc<Next, FunctionTuple, FunctionTagList>::add(L, tuple);
        }
    };
    template<class FunctionTuple, class FunctionTagList>
    struct __AddFunc<Loki::NullType, FunctionTuple, FunctionTagList> {
        static void add(lua_State *L, const FunctionTuple &tuple, int top) {}
        static void add(lua_State *L, const FunctionTuple &tuple) {}
    };
    
}

/**
 * @brief register functions within a module
 */
template <class FunctionTagList, class FunctionTuple>
void registerFunctions(
	lua_State *L,
    const FunctionTuple &tuple,
	const std::string &modulename)
{
    lua_getglobal(L, modulename.c_str());
    if (lua_istable(L, -1) != 1) {
        lua_newtable(L);
        int top = lua_gettop(L);
        __AddFunc<FunctionTagList, FunctionTuple>::add(L, tuple, top);
        lua_setglobal(L, modulename.c_str());
        return;
    }
    int top = lua_gettop(L);
    __AddFunc<FunctionTagList, FunctionTuple>::add(L, tuple, top);
    
}

/**
 * @brief register functions
 */
template <class FunctionTagList, class FunctionTuple>
void registerFunctions(
	lua_State *L,
    const FunctionTuple &tuple)
{
    __AddFunc<FunctionTagList, FunctionTuple>::add(L, tuple);
}






}} // namespace
#endif /* LUASCRIPT_HPP_ */
