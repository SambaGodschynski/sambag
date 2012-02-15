/*
 * ILuaTable.hpp
 *
 *  Created on: 14 Feb 2012
 *      Author: samba
 */

#ifndef ILUATABLE_HPP_
#define ILUATABLE_HPP_

#include <lua.hpp>

namespace sambag { namespace lua {
//=============================================================================
class ILuaTable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	/**
	 * gets tablevalues from lua stack and insert it into ILuaTable object.
	 * @param L lua state
	 * @param index lua stack index
	 * @return true if suceed
	 */
	virtual bool getFromStack(lua_State *L, int index) = 0;
	//-------------------------------------------------------------------------
	/**
	 * pushs ILuaTable object into lua stack
	 * @param L lua state
	 * @return true if suceed
	 */
	virtual bool pushIntoStack(lua_State *L) = 0;
};
//=============================================================================
/**
 * Implements ILuaTable using a simple array. Assumes that lua table keys are
 * index numbers.
 */
template <typename T>
class LuaTableArray : public ILuaTable {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	size_t size;
	//-------------------------------------------------------------------------
	T *data;
public:
	//-------------------------------------------------------------------------
	LuaTableArray() : size(0), data(NULL) {}
	//-------------------------------------------------------------------------
	virtual bool getFromStack(lua_State *L, int index) {

	}
	//-------------------------------------------------------------------------
	virtual bool pushIntoStack(lua_State *L) {

	}
};

}} //namespaces

#endif /* ILUATABLE_HPP_ */
