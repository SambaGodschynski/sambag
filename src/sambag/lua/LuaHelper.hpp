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

namespace sambag { namespace lua {
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * returns true if type at index
 * @param L
 * @param index
 * @return
 */
template <typename T>
bool check(lua_State *L, int index) {
	if (boost::is_convertible<T*, ILuaTable*>::value)
		return lua_istable(L, index) == 1;
	if (boost::is_arithmetic<T>::value)
		return lua_isnumber(L, index) == 1;
	return false;
}
//-----------------------------------------------------------------------------
template <>
bool check<std::string>(lua_State *L, int index) {
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
/**
 * @param out destination value
 * @param L lua state
 * @param index lua stack index
 * @return true if succeed
 */
//-----------------------------------------------------------------------------
template <typename T>
bool get(T &out, lua_State *L, int index) {
	if (!check<T>(L, index))
		return false;
	return __get(out, L, index);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
/**
 * @param o0
 * @param o1
 * @param L
 * @param index
 * @return true if succeed
 */
template <
	typename T0,
	typename T1
>
bool get(T0 &o0,
		T1 &o1,
		lua_State *L,
		int index)
{
	if (!get<T0>(o0, L, index--))
		return false;
	if (!get<T1>(o1, L, index))
		return false;
	return true;
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2
>
bool get(T0 &o0,
		T1 &o1,
		T2 &o2,
		lua_State *L,
		int index)
{
	if (!get<T0, T1>(o0, o1, L, index))
		return false;
	index-= 2;
	if (!get<T2>(o2, L, index))
		return false;
	return true;
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2,
	typename T3
>
bool get(T0 &o0,
		T1 &o1,
		T2 &o2,
		T3 &o3,
		lua_State *L,
		int index)
{
	if (!get<T0, T1, T2>(o0, o1, o2, L, index))
		return false;
	index-= 3;
	if (!get<T3>(o3, L, index))
		return false;
	return true;
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2,
	typename T3,
	typename T4
>
bool get(T0 &o0,
		T1 &o1,
		T2 &o2,
		T3 &o3,
		T4 &o4,
		lua_State *L,
		int index)
{
	if (!get<T0, T1, T2, T3>(o0, o1, o2, o3, L, index))
		return false;
	index-= 4;
	if (!get<T4>(o4, L, index))
		return false;
	return true;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
size_t getLen(lua_State *L, int index) {
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
struct ExecutionFailed {
	std::string errMsg;
	ExecutionFailed(const std::string &errMsg) : errMsg(errMsg) {}
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
inline void executeString(lua_State *L, const std::string &str) {
	luaL_loadstring (L, str.c_str());
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
//-----------------------------------------------------------------------------
/**
 * pushs o0 up to oN into lua stack.
 * @param o0
 * @param o1
 * @param L
 */
template <
	typename T0,
	typename T1
>
void push(const T0 &o0,
		const T1 &o1,
		lua_State *L)
{
	push(o0,L);
	push(o1,L);
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2
>
void push(const T0 &o0,
		const T1 &o1,
		const T2 &o2,
		lua_State *L)
{
	push(o0, o1 ,L);
	push(o2, L);
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2,
	typename T3
>
void push(const T0 &o0,
		const T1 &o1,
		const T2 &o2,
		const T3 &o3,
		lua_State *L)
{
	push(o0, o1 ,o2, L);
	push(o3, L);
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2,
	typename T3,
	typename T4
>
void push(const T0 &o0,
		const T1 &o1,
		const T2 &o2,
		const T3 &o3,
		const T4 &o4,
		lua_State *L)
{
	push(o0, o1 ,o2, o3, L);
	push(o4, L);
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
template <typename T>
void callLuaFunc(lua_State *L,
		const std::string &fName,
		int nResult,
		const T &o1)
{
	__getF(L, fName);
	push(o1,L);
	__callF(L, 1, nResult);

}
//-----------------------------------------------------------------------------
template <typename T1,
	typename T2>
void callLuaFunc(lua_State *L,
		const std::string &fName,
		int nResult,
		const T1 &o1,
		const T2 &o2)
{
	__getF(L, fName);
	push(o1,o2,L);
	__callF(L, 2, nResult);

}
//-----------------------------------------------------------------------------
template <typename T1,
	typename T2,
	typename T3>
void callLuaFunc(lua_State *L,
		const std::string &fName,
		int nResult,
		const T1 &o1,
		const T2 &o2,
		const T3 &o3)
{
	__getF(L, fName);
	push(o1,o2,o3,L);
	__callF(L, 3, nResult);

}
//-----------------------------------------------------------------------------
template <typename T1,
	typename T2,
	typename T3,
	typename T4>
void callLuaFunc(lua_State *L,
		const std::string &fName,
		int nResult,
		const T1 &o1,
		const T2 &o2,
		const T3 &o3,
		const T4 &o4)
{
	__getF(L, fName);
	push(o1,o2,o3,o4,L);
	__callF(L, 4, nResult);

}
//-----------------------------------------------------------------------------
template <typename T1,
	typename T2,
	typename T3,
	typename T4,
	typename T5>
void callLuaFunc(lua_State *L,
		const std::string &fName,
		int nResult,
		const T1 &o1,
		const T2 &o2,
		const T3 &o3,
		const T4 &o4,
		const T5 &o5)
{
	__getF(L, fName);
	push(o1,o2,o3,o4,o5,L);
	__callF(L, 5, nResult);

}
}} //namespaces


#endif /* LUAHELPER_HPP_ */
