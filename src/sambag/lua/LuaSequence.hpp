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
#include <boost/shared_array.hpp>

namespace sambag { namespace lua {
//-------------------------------------------------------------------------
struct NoSequenceEx {};
//-------------------------------------------------------------------------
struct OutOfBoundsEx {};
//=============================================================================
/**
 * Implements ILuaTable using a boost::shared_array<T>.
 * Transforms between a Lua-sequence(index:1..n) and a C-array(index:0..n).
 */
template <typename T>
class LuaSequence : public ILuaTable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef T Type;
private:
	//-------------------------------------------------------------------------
	size_t size;
	//-------------------------------------------------------------------------
	typedef boost::shared_array<T> Data;
	//-------------------------------------------------------------------------
	Data data;
public:
	//-------------------------------------------------------------------------
	/**
	 * allocates new table data.
	 * @param size
	 */
	void alloc(size_t _size) {
		size = _size;
		data = Data(new T[size]);
	}
	//-------------------------------------------------------------------------
	LuaSequence(size_t size = 0) : size(size) {
		if (size>0)
			alloc(size);
	}
	//-------------------------------------------------------------------------
	virtual ~LuaSequence() {
	}
	//-------------------------------------------------------------------------
	T & operator[](size_t i) {
		if (i>=size || !data)
			throw OutOfBoundsEx();
		return data[i];
	}
	//-------------------------------------------------------------------------
	const T & operator[](size_t i) const {
		if (i>=size || !data)
			throw OutOfBoundsEx();
		return data[i];
	}
	//-------------------------------------------------------------------------
	size_t getSize() const {
		return size;
	}
	//-------------------------------------------------------------------------
	virtual bool getFromStack(lua_State *L, int index) {
		//TODO: throw no sequence
		size = getLen(L, index);
		alloc(size);
		lua_pushnil(L);  /* first key */
		--index;
		bool res = true;
		while (lua_next(L, index) != 0) {
			// TODO: check sequence
			// uses 'key' (at index -2) and 'value' (at index -1)
			size_t i;
			if (get(i, L, -2) && --i/*lua sequence starts with 1*/<size) {
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
	virtual void pushIntoStack(lua_State *L) const {
		lua_newtable(L);
		int top = lua_gettop(L);
		for (size_t i=0; i<size; ++i) {
			lua_pushnumber(L, i+1); //lua sequence starts with 1
			push(data[i], L);
			lua_settable(L, top);
		}
	}
};

}} //namespaces

#endif /* LUATABLE_HPP_ */
