/*
 * LuaHelper.hpp
 *
 *  Created on: 13 Feb 2012
 *      Author: samba
 */

#ifndef LUAHELPER_HPP_
#define LUAHELPER_HPP_

#include <lua.hpp>
#include "ILuaTable.hpp"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <sambag/com/Helper.hpp>
#include <loki/NullType.h>

// in lua < 502 lua_len doesn't exists.
#if LUA_VERSION_NUM < 502
	#define SAMBAG_LUA_LEN_WORKAROUND(a1, a2) lua_objlen((a1), (a2)) 
#else
	#define SAMBAG_LUA_LEN_WORKAROUND(a1, a2) lua_len((a1), (a2)) 
#endif

// 'declaration does not declare anything' issue on mac. see:
// http://stackoverflow.com/questions/8173620/c-boost-1-48-type-traits-and-cocoa-inclusion-weirdness
#ifdef __APPLE__
	#ifdef check
	#undef check
	#endif
#endif
#include <boost/type_traits.hpp>

namespace sambag { namespace lua {
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef boost::shared_ptr<lua_State> LuaStateRef;
inline LuaStateRef createLuaStateRef(bool openLibs = true) {
	LuaStateRef lRef(luaL_newstate(), &lua_close);
	if (!openLibs)
		return lRef;
	luaL_openlibs(lRef.get());
	return lRef;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
struct LuaException {
	std::string errMsg;
	LuaException(const std::string &errMsg) : errMsg(errMsg) {}
};
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * returns true if type at index
 * @param L
 * @param index
 * @return
 */
template <typename T>
bool isType(lua_State *L, int index) {
	if (boost::is_convertible<T*, ILuaTable*>::value)
		return lua_istable(L, index) == 1;
	if (boost::is_arithmetic<T>::value)
		return lua_isnumber(L, index) == 1;
	return false;
}
//-----------------------------------------------------------------------------
template <>
inline bool isType<std::string>(lua_State *L, int index) {
	return lua_isstring(L, index) == 1;
}
//-----------------------------------------------------------------------------
template <>
inline bool isType<bool>(lua_State *L, int index) {
	return lua_isboolean(L, index) == 1;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace {
typedef com::Int2Type<0> NoImpl;
typedef com::Int2Type<1> Impl;
//-----------------------------------------------------------------------------
template <typename T>
inline void __getNumber(T &outValue, lua_State *L, int index, NoImpl) {}
//-----------------------------------------------------------------------------
template <typename T>
inline void  __getNumber(T &outValue, lua_State *L, int index, Impl) {
	outValue = (T)lua_tonumber(L, index);
}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __getILuaTable(T &outValue, lua_State *L, int index, NoImpl) {}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __getILuaTable(T &outValue, lua_State *L, int index, Impl) {
	outValue.getFromStack(L, index);
}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __getString(T &outValue, lua_State *L, int index, NoImpl) {}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __getString(T &outValue, lua_State *L, int index, Impl) {
	outValue = std::string(lua_tostring(L, index));
}
} // namespace
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * @param out destination value
 * @param L lua state
 * @param index lua stack index
 * @return true if succeed
 */
template <typename T>
bool get(T &out, lua_State *L, int index) {
	if (!isType<T>(L, index))
		return false;
	// if T is Number
	enum {isNumber = boost::is_arithmetic<T>::value};
	__getNumber(out, L, index, com::Int2Type<isNumber>());
	// if T is instance of ILuaTable
	enum {isILuaTable=boost::is_convertible<T*, ILuaTable*>::value};
	__getILuaTable(out, L, index, com::Int2Type<isILuaTable>());
	// if T is instance of ILuaTable
	enum {isString=boost::is_convertible<T*, std::string*>::value};
	__getString(out, L, index, com::Int2Type<isString>());
	return true;
}
/**
 * @note bool needs extra impl because the lack of 
 * boost::is_bool<T>
 */
template <>
inline bool get<bool>(bool &out, lua_State *L, int index) {
	if (!isType<bool>(L, index))
		return false;
	out = lua_toboolean(L, index);
	return true;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline size_t getLen(lua_State *L, int index) {
	SAMBAG_LUA_LEN_WORKAROUND(L, index);
	size_t s;
	if (!get(s, L, -1))
		return 0;
	lua_pop(L, 1);
	return s;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// executation helper
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
struct ExecutionFailed : public LuaException {
	ExecutionFailed(const std::string &errMsg) : LuaException(errMsg) {}
};
struct NoFunction {};
//-----------------------------------------------------------------------------
namespace {
	inline void __getF(lua_State *L, const std::string &fName)
	{
		lua_getglobal(L, fName.c_str());
		if (!lua_isfunction(L, -1)==1)
			throw NoFunction();
	}
	inline void __callF(lua_State *L,
			int narg=0,
			int nret=0)
	{
		if (lua_pcall(L, narg, nret, 0)!=0) {
			throw ExecutionFailed(std::string(lua_tostring(L, -1)));
		}
	}
} // namespace
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
inline bool hasFunction(lua_State *L, const std::string &fName) {
	lua_getglobal(L, fName.c_str());
	return lua_isfunction(L, -1)==1;
}
inline void executeString(lua_State *L, const std::string &str) {
	luaL_loadstring (L, str.c_str());
	__callF(L);
}
inline void executeFile(lua_State *L, const std::string &filename) {
	luaL_loadfile(L, filename.c_str());
	__callF(L);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace {
typedef com::Int2Type<0> NoImpl;
typedef com::Int2Type<1> Impl;
//-----------------------------------------------------------------------------
template <typename T>
inline void __pushNumber(const T &value, lua_State *L, NoImpl) {}
//-----------------------------------------------------------------------------
template <typename T>
inline void  __pushNumber(const T &value, lua_State *L, Impl) {
	lua_pushnumber(L, value);
}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __pushILuaTable(const T &value, lua_State *L, NoImpl) {}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __pushILuaTable(const T &value, lua_State *L, Impl) {
	value.pushIntoStack(L);
}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __pushString(const T &value, lua_State *L, NoImpl) {}
//-----------------------------------------------------------------------------
template <typename T> // template needed to be usable for get().
inline void __pushString(const T &value, lua_State *L, Impl) {
	lua_pushstring(L, value.c_str());
}
} // namespace
//-----------------------------------------------------------------------------
/**
 * push value into lua stack
 * @param value
 * @param L
 */
template <typename T>
void push(lua_State *L, const T &value) {
	// if T is Number
	enum {isNumber = boost::is_arithmetic<T>::value};
	__pushNumber(value, L, com::Int2Type<isNumber>());
	// if T is instance of ILuaTable
	enum {isILuaTable=boost::is_convertible<T*, ILuaTable*>::value};
	__pushILuaTable(value, L, com::Int2Type<isILuaTable>());
	// if T is instance of ILuaTable
	enum {isString=boost::is_convertible<T*, std::string*>::value};
	__pushString(value, L, com::Int2Type<isString>());
}
/**
 * @note bool needs extra impl because the lack of 
 * boost::is_bool<T>
 */
template <>
inline void push<bool>(lua_State *L, const bool &value) {
    lua_pushboolean(L, value);
}
/**
 * @note useful when mixing register function and manually stack
 * manipulation
 */
template <>
inline void push<Loki::NullType>(lua_State *L, const Loki::NullType &value)
{
}
//-----------------------------------------------------------------------------
/**
 * overload used for implicit string construction.
 * @param cStr
 * @param L
 */
inline void push(lua_State *L, const char * cStr) {
	__pushString(std::string(cStr), L, com::Int2Type<1>());
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace {
	struct _IntoTuple {
		lua_State *L;
		bool succeed;
		int index;
		template <typename T>
		void operator()(T &value) {
			if (!succeed)
				return;
			if (!get(value, L, index--))
				succeed = false;
		}
		_IntoTuple(lua_State *L, int startIndex = -1) : L(L), succeed(true), index(startIndex) {}
	};
	struct _PushFromTuple {
		lua_State *L;
		template <typename T>
		void operator()(const T &value) {
			push(L, value);
		}
		_PushFromTuple(lua_State *L) : L(L) {}
	};
} // namespace
//-----------------------------------------------------------------------------
/**
 * push boost::tuple values into stack.
 * @param L
 * @param t
 * @return true, when succeed.
 */
template <
	class T0, class T1, class T2,
	class T3, class T4, class T5,
	class T6, class T7, class T8, class T9
>
void push(lua_State *L,
		  const boost::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &t)
{
	_PushFromTuple pft(L);
	sambag::com::foreach(t, pft);
}
//-----------------------------------------------------------------------------
template <typename Tuple>
bool get(lua_State *L, Tuple &t, int index = -1) {
	_IntoTuple into(L, index);
	sambag::com::foreach(t, into);
	return into.succeed;
}
//-----------------------------------------------------------------------------
/**
 * put values from stack into tuple and pop it from stack.
 * If operation failed nothing will be popped.
 * @param L
 * @param t
 * @return true, when succeed.
 */
template <typename Tuple>
bool pop(lua_State *L, Tuple &t) {
	if(!get(L, t))
		return false;
	lua_pop(L, (int)boost::tuples::length<Tuple>::value);
	return true;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
/**
 * calls lua function. throws NoFunction when function not exists,
 * or ExecutionFailed when Lua error occurs.
 * @param L
 * @param fName function name
 * @param nResult number of results
 */
inline void callLuaFunc(lua_State *L, const std::string &fName, int nResult = 0)
{
	__getF(L, fName);
	__callF(L, 0, nResult);

}
//-----------------------------------------------------------------------------
/**
 * calls lua function. throws NoFunction when function not exists,
 * or ExecutionFailed when Lua error occurs.
 * @param L
 * @param fName function name
 * @param args Lua function arguments
 */
template <typename ArgTuple>
inline void callLuaFunc(lua_State *L,
		const std::string &fName,
		const ArgTuple &args)
{
	__getF(L, fName);
	push(L, args);
	__callF(L,
		(int)boost::tuples::length<ArgTuple>::value, // num args
		0
	);
}
//-----------------------------------------------------------------------------
/**
 * calls lua function. throws NoFunction when function not exists,
 * or ExecutionFailed when Lua error occurs.
 * @param L
 * @param fName function name
 * @param return Lua function retrun values
 * @param args Lua function arguments
 */
template <typename RetTuple, typename ArgTuple>
inline void callLuaFunc(lua_State *L,
		const std::string &fName,
		const ArgTuple &args,
		RetTuple &ret)
{
	__getF(L, fName);
	push(L, args);
	__callF(L,
		(int)boost::tuples::length<ArgTuple>::value, // num args
		(int)boost::tuples::length<RetTuple>::value // num rets
	);
	pop(L, ret);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
/**
 * @param L the lua state objekt
 * @param name the name of the global variable
 * @param outValue value of global lua variable
 * @return true when succeed
 */
template <typename T>
bool getGlobal(lua_State *L, T &outValue, const std::string &name) {
	lua_getglobal(L, name.c_str());
	if (!isType<T>(L, -1))
		return false;
	get(outValue, L, -1);
	lua_pop(L, 1);
	return true;
}
//-----------------------------------------------------------------------------
/**
 * Like bool getGlobal(), but it uses the failed value when
 * operation was unsuccessfull instead returning a boolean.
 * @param outValue value of global lua variable
 * @param failed value when operation was unsuccessfull
 * @param L the lua state objekt
 * @param name the name of the global variable
 */
template <typename T>
void getGlobal(lua_State *L, T &outValue, const T &failed, const std::string &name) {
	if (!getGlobal(L, outValue, name)) {
		outValue = failed;
	}
}
}} //namespaces
#endif /* LUAHELPER_HPP_ */
