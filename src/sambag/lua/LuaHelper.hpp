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
#include <boost/static_assert.hpp>

namespace sambag { namespace lua {
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * returns true if value at index type T
 * @param L
 * @param index
 * @return
 */
template <typename T>
bool check(lua_State *L, int index) {
	return false;
}
//-----------------------------------------------------------------------------
template <>
bool check<int>(lua_State *L, int index) {
	return lua_isnumber(L, index) == 1;
}
//-----------------------------------------------------------------------------
template <>
bool check<unsigned int>(lua_State *L, int index) {
	return lua_isnumber(L, index) == 1;
}
//-----------------------------------------------------------------------------
template <>
bool check<float>(lua_State *L, int index) {
	return lua_isnumber(L, index) == 1;
}
//-----------------------------------------------------------------------------
template <>
bool check<double>(lua_State *L, int index) {
	return lua_isnumber(L, index) == 1;
}
//-----------------------------------------------------------------------------
template <>
bool check<std::string>(lua_State *L, int index) {
	return lua_isstring(L, index) == 1;
}
//-----------------------------------------------------------------------------
template <>
bool check<ILuaTable>(lua_State *L, int index) {
	return lua_istable(L, index) == 1;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace {
//-----------------------------------------------------------------------------
bool __get(int &out, lua_State *L, int index) {
	out = (int)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
bool __get(unsigned int &out, lua_State *L, int index) {
	out = (unsigned int)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
bool __get(float &out, lua_State *L, int index) {
	out = (float)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
bool __get(double &out, lua_State *L, int index) {
	out = (double)lua_tonumber(L, index);
	return true;
}
//-----------------------------------------------------------------------------
bool __get(std::string &out, lua_State *L, int index) {
	out = std::string(lua_tostring(L, index));
	return true;
}
//-----------------------------------------------------------------------------
bool __get(ILuaTable &out, lua_State *L, int index) {
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
 * @return flag shows the value index which get-operation failed:
 * 0 - succeed, 1 first value failed, 10 second value failed, 11 ..., 100 ...
 */
template <
	typename T0,
	typename T1
>
int get(T0 &o0,
		T1 &o1,
		lua_State *L,
		int index)
{
	int res = 0;
	if (!get<T0>(o0, L, index--))
		res = 1;
	if (!get<T1>(o1, L, index))
		res |= 1<<2;
	return res;
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2
>
int get(T0 &o0,
		T1 &o1,
		T2 &o2,
		lua_State *L,
		int index)
{
	int res = get<T0, T1>(o0, o1, L, index);
	index-= 2;
	if (!get<T2>(o2, L, index))
		res |= 1<<3;
	return res;
}
//-----------------------------------------------------------------------------
template <
	typename T0,
	typename T1,
	typename T2,
	typename T3
>
int get(T0 &o0,
		T1 &o1,
		T2 &o2,
		T3 &o3,
		lua_State *L,
		int index)
{
	int res = get<T0, T1, T2>(o0, o1, o2, L, index);
	index-= 3;
	if (!get<T3>(o3, L, index))
		res |= 1<<4;
	return res;
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
struct ExecutionFailed {
	std::string errMsg;
	ExecutionFailed(const std::string &errMsg) : errMsg(errMsg) {}
};
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline void executeString(lua_State *L,
		const std::string &str,
		int nargs=0,
		int nres=0)
{
	luaL_loadstring (L, str.c_str());
	if (lua_pcall(L, nargs, nres, 0)) {
		throw ExecutionFailed(std::string(lua_tostring(L, -1)));
	}
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
}} //namespaces


#endif /* LUAHELPER_HPP_ */
