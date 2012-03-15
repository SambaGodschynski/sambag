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
#include <lua.hpp>

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
 *
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
 * 		- a FunctionTagType can be registered once.
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
// Int2Value
//=============================================================================
template <int I>
struct Int2Type {
	enum {Value=I};
};
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
		Int2Type<0>, // num args
		Int2Type<0>) // is void?
{
	typename Function::result_type r = f();
	push(r, L); // return value into lua stack
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<0>, // num args
		Int2Type<1>) // is void?
{
	f();
}
//=============================================================================
// NumArgs: 1
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<1>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	if (!get(a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	typename Function::result_type r = f(a1);
	// return value into lua stack
	push(r, L);
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<1>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	if (!get(a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	f(a1);
}
//=============================================================================
// NumArgs: 2
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<2>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	if (!get(a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	typename Function::result_type r = f(a1, a2);
	// return value into lua stack
	push(r, L);
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<2>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	if (!get(a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	f(a1, a2);
}
//=============================================================================
// NumArgs: 3
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<3>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	typename Function::arg3_type a3;
	if (!get(a3, a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	typename Function::result_type r = f(a1, a2, a3);
	// return value into lua stack
	push(r, L);
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<3>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	typename Function::arg3_type a3;
	if (!get(a3, a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	f(a1, a2, a3);
}
//=============================================================================
// NumArgs: 4
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<4>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	typename Function::arg3_type a3;
	typename Function::arg4_type a4;
	if (!get(a4, a3, a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	typename Function::result_type r = f(a1, a2, a3, a4);
	// return value into lua stack
	push(r, L);
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<4>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	typename Function::arg3_type a3;
	typename Function::arg4_type a4;
	if (!get(a4, a3, a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	f(a1, a2, a3, a4);
}
//=============================================================================
// NumArgs: 5
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<5>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	typename Function::arg3_type a3;
	typename Function::arg4_type a4;
	typename Function::arg5_type a5;
	if (!get(a5, a4, a3, a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	typename Function::result_type r = f(a1, a2, a3, a4, a5);
	// return value into lua stack
	push(r, L);
}
//-----------------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<5>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg2_type a2;
	typename Function::arg3_type a3;
	typename Function::arg4_type a4;
	typename Function::arg5_type a5;
	if (!get(a5, a4, a3, a2, a1, L, -1))
		throwArgumentsMismatch<Function>(L);
	// call
	f(a1, a2, a3, a4, a5);
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
			Int2Type<Function::arity>(), // num args
			Int2Type<IsVoidValue>() // isVoid
		);
		return IsVoidValue == 1 ? 0 : 1;
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

}} // namespace
#endif /* LUASCRIPT_HPP_ */
