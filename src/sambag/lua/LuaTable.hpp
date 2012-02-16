/*
 * LuaTable.hpp
 *
 *  Created on: 15 Feb 2012
 *      Author: samba
 */

#ifndef LUATABLE_HPP_
#define LUATABLE_HPP_

#include "ILuaTable.hpp"
#include "LuaHelper.hpp"
#include <lua.hpp>

namespace sambag { namespace lua {
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
	//-------------------------------------------------------------------------
	void alloc(size_t size) {
		if (data) {
			delete[] data;
			data = NULL;
		}
		data = new T[size];
	}
public:
	//-------------------------------------------------------------------------
	LuaTableArray() : size(0), data(NULL) {}
	//-------------------------------------------------------------------------
	virtual ~LuaTableArray() {
		delete[] data;
		data = NULL;
	}
	//-------------------------------------------------------------------------
	size_t getSize() const {
		return size;
	}
	//-------------------------------------------------------------------------
	virtual bool getFromStack(lua_State *L, int index) {
		size = getLen(L, index);
		alloc(size);
		lua_pushnil(L);  /* first key */
		--index;
		bool res = true;
		while (lua_next(L, index) != 0) {
			// uses 'key' (at index -2) and 'value' (at index -1)
			size_t i;
			if (get(i, L, -2) && i<size) {
				res = get<T>(data[i], L, -1);
			} else { // invalid index value
				std::cout<<i<<"; ";
				return false;
			}
			lua_pop(L, 1);
		}
		return res;
	}
	//-------------------------------------------------------------------------
	virtual bool pushIntoStack(lua_State *L) {

	}
};

}} //namespaces

#endif /* LUATABLE_HPP_ */
