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
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <sambag/com/Helper.hpp>

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
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace {
//-----------------------------------------------------------------------------
inline bool __get(int &out, lua_State *L, int index) {
	out = (int)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
inline bool __get(unsigned int &out, lua_State *L, int index) {
	out = (unsigned int)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
inline bool __get(float &out, lua_State *L, int index) {
	out = (float)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
inline bool __get(double &out, lua_State *L, int index) {
	out = (double)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
inline bool __get(std::string &out, lua_State *L, int index) {
	out = std::string(lua_tostring(L, index));
	return true;
}
//-----------------------------------------------------------------------------
inline bool __get(ILuaTable &out, lua_State *L, int index) {
	return out.getFromStack(L, index);
}
} // namespace
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * @param out destination value
 * @param L lua state
 * @param index lua stack index
 * @return true if succeed
 */
//-----------------------------------------------------------------------------
template <typename T>
bool get(T &out, lua_State *L, int index) {
	if (!isType<T>(L, index))
		return false;
	return __get(out, L, index);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline size_t getLen(lua_State *L, int index) {
	lua_len(L, index);
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
//-----------------------------------------------------------------------------
inline void push(int v, lua_State *L) {
	lua_pushnumber(L, v);
}
//-----------------------------------------------------------------------------
inline void push(unsigned int v, lua_State *L) {
	lua_pushnumber(L, v);
}
//-----------------------------------------------------------------------------
inline void push(float v, lua_State *L) {
	lua_pushnumber(L, v);
}
//-----------------------------------------------------------------------------
inline void push(const double &v, lua_State *L) {
	lua_pushnumber(L, v);
}
//-----------------------------------------------------------------------------
inline void push(const std::string &v, lua_State *L) {
	lua_pushstring(L, v.c_str());
}
//-----------------------------------------------------------------------------
inline void push(const ILuaTable &v, lua_State *L) {
	v.pushIntoStack(L);
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
			push(value, L);
		}
		_PushFromTuple(lua_State *L) : L(L) {}
	};
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
//-----------------------------------------------------------------------------
/**
 * push tuple values into stack.
 * @param L
 * @param t
 * @return true, when succeed.
 */
template <typename Tuple>
void push(lua_State *L, const Tuple &t) {
	_PushFromTuple pft(L);
	sambag::com::foreach(t, pft);
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
inline void callLuaFunc(lua_State *L, const std::string &fName, int nResult)
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
bool getGlobal(T &outValue, lua_State *L, const std::string &name) {
	lua_getglobal(L, name.c_str());
	if (!isType<T>(L, -1))
		return false;
	get(outValue, L, -1);
	lua_pop(L, 1);
	return true;
}
}} //namespaces


#endif /* LUAHELPER_HPP_ */
