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
 * @class LuaScript
 *
 *	Usage:
 *	struct FooFunction_tag {
 *		typedef boost::function< void ( int, float ) > Function;
 *		static const char * { return "foo"; }
 *	};
 *
 *	struct BarFunction_tag {
 *		typedef boost::function< int ( string ) > Function;
 *		static const char * { return "bar"; }
 *	};
 *
 *	LuaScript script;
 *
 *	script.registerFunction<FooFunction_tag>(
 *		boost::bind(&Dummy::foo, &dummy, _1, _2)
 *	);
 *
 *	script.registerFunction<BarFunction_tag>(
 *		boost::bind(&Dummy::bar, &dummy, _1, _2)
 *	);
 *
 *	// execute script
 *	script.execFile("luaScriptFile.lua");
 *	script.execString(luaScriptString);
 *
 *	// get global script variable, throws
 *	int g1 = script.getGlobal<int>("g1");
 * // with alternative error value, nothrow:
 *	string g2 = script.getGlobal<string>("g2", "errorString");
 *
 */
class LuaScript {
//=============================================================================
public:
private:
	//-------------------------------------------------------------------------
	LuaStateRef luaState;
public:
	//-------------------------------------------------------------------------
	LuaScript();
	//-------------------------------------------------------------------------
	~LuaScript();
	//-------------------------------------------------------------------------
	LuaStateRef getL() const {
		return luaState;
	}
	//-------------------------------------------------------------------------
	void execFile(const std::string &filename);
	//-------------------------------------------------------------------------
	void execString(const std::string &script);
	//-------------------------------------------------------------------------
	/**
	 * @param name
	 * @return value of global lua variable 'name'
	 * @throw ExecutionFailed
	 */
	template <typename T>
	T getGlobal(const std::string &name);
	//-------------------------------------------------------------------------
	/**
	 * @param name
	 * @param errValue return value when 'name' not accessible
	 * @return value of global lua variable 'name'
	 */
	template <typename T>
	T getGlobal(const std::string &name, const T &errValue);
	//-------------------------------------------------------------------------
	/**
	 * Template parameter FunctionTag concept:
	 * 		struct Function_tag {
	 * 			typedef boost::function< void () > Function; // Function Type
	 * 			static const char * { return "foo"; } // lua function name to register
	 * 		};
	 * 	Note: a FunctionTagType can be registered once.
	 * @param f boost::function object
	 */
	template <class FunctionTag>
	void registerFunction(const typename FunctionTag::Function &f);
};

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
//===================================================================
// NumArgs: 0
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<0>, // num args
		Int2Type<0>) // is void?
{
	typename Function::result_type r = f();
	push(r, L); // return value into lua stack
}
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<0>, // num args
		Int2Type<1>) // is void?
{
	f();
}
//===================================================================
// NumArgs: 1
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<1>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	get(a1, L, -1);
	// call
	typename Function::result_type r = f(a1);
	// return value into lua stack
	push(r, L);
}
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<1>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	get(a1, L, -1);
	// call
	f(a1);
}
//===================================================================
// NumArgs: 2
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<2>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg1_type a2;
	get(a2, a1, L, -1);
	// call
	typename Function::result_type r = f(a1, a2);
	// return value into lua stack
	push(r, L);
}
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<2>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg1_type a2;
	get(a2, a1, L, -1);
	// call
	f(a1, a2);
}
//===================================================================
// NumArgs: 3
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<3>, // num args
		Int2Type<0>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg1_type a2;
	typename Function::arg1_type a3;
	get(a3, a2, a1, L, -1);
	// call
	typename Function::result_type r = f(a1, a2, a3);
	// return value into lua stack
	push(r, L);
}
//-------------------------------------------------------------------
template <typename Function>
void callF(const Function &f,
		lua_State *L,
		Int2Type<3>, // num args
		Int2Type<1>) // is void?
{
	// get argument(s) from stack
	typename Function::arg1_type a1;
	typename Function::arg1_type a2;
	typename Function::arg1_type a3;
	get(a3, a2, a1, L, -1);
	// call
	f(a1, a2, a3);
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
void LuaScript::registerFunction(const typename FunctionTag::Function &f) {
	typedef RegisterHelperClass<FunctionTag> Helper;
	lua_State *L = luaState.get();
	Helper::fMap[L] = f;
	lua_register(L, FunctionTag::name(), &Helper::luaCallback);
}

}} // namespace
#endif /* LUASCRIPT_HPP_ */
