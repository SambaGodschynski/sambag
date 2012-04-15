/*
 * LuaSequence.hpp
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
#include <vector>

namespace sambag { namespace lua {
//=============================================================================
/**
 * Implements ILuaTable using std::vector
 * Transforms between a Lua-sequence(index:1..n) and a C-array(index:0..n).
 */
template <typename T>
class LuaSequence : public ILuaTable, public std::vector<T> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::vector<T> Vector;
	//-------------------------------------------------------------------------
	LuaSequence() {}
	//-------------------------------------------------------------------------
	LuaSequence(size_t size, const T &init = T()) : Vector(size, init) {}
	//-------------------------------------------------------------------------
	virtual ~LuaSequence() {}
	//-------------------------------------------------------------------------
	virtual bool getFromStack(lua_State *L, int index) {
		//TODO: throw no sequence
		size_t size = getLen(L, index);
		Vector::reserve(size);
		lua_pushnil(L);  /* first key */
		--index;
		bool res = true;
		while (lua_next(L, index) != 0) {
			// TODO: check sequence
			// uses 'key' (at index -2) and 'value' (at index -1)
			size_t i;
			if (get(i, L, -2)) {
				T v;
				if( !get<T>(v, L, -1) )
					return false;
				Vector::push_back(v);
			} else { // invalid index value
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
		for (size_t i=0; i<Vector::size(); ++i) {
			lua_pushnumber(L, i+1); //lua sequence starts with 1
			push(L, Vector::at(i));
			lua_settable(L, top);
		}
	}
};
//-----------------------------------------------------------------------------
struct OutOfBoundsEx {};
//=============================================================================
/**
 * Implements ILuaTable using a extern array.
 * Transforms between a Lua-sequence(index:1..n) and a C-array(index:0..n).
 */
template <typename T>
class LuaSequenceEx : public ILuaTable {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	size_t _size;
	//-------------------------------------------------------------------------
	T *data;
public:
	//-------------------------------------------------------------------------
	/**
	 * Creates LuaSequenceEx with given array. Assumes that array is allocated.
	 * LuaSequenceEx will not release array.
	 * @param data
	 * @param size
	 * @return
	 */
	LuaSequenceEx(T *data=NULL, size_t size = 0) : _size(size), data(data) {}
	//-------------------------------------------------------------------------
	virtual ~LuaSequenceEx() {
	}
	//-------------------------------------------------------------------------
	T & operator[](size_t i) {
		if (i>=_size || !data)
			throw OutOfBoundsEx();
		return data[i];
	}
	//-------------------------------------------------------------------------
	const T & operator[](size_t i) const {
		if (i>=_size || !data)
			throw OutOfBoundsEx();
		return data[i];
	}
	//-------------------------------------------------------------------------
	size_t size() const {
		return _size;
	}
	//-------------------------------------------------------------------------
	virtual bool getFromStack(lua_State *L, int index) {
		//TODO: throw no sequence
		size_t tsize = getLen(L, index);
		if (tsize>_size)
			throw OutOfBoundsEx();
		lua_pushnil(L);  /* first key */
		--index;
		while (lua_next(L, index) != 0) {
			// TODO: check sequence
			// uses 'key' (at index -2) and 'value' (at index -1)
			size_t i;
			if (get(i, L, -2) && --i/*lua sequence starts with 1*/<_size) {
				if( !get<T>(data[i], L, -1) )
					return false;
			} else { // invalid index value
				throw OutOfBoundsEx();
			}
			lua_pop(L, 1);
		}
		return true;
	}
	//-------------------------------------------------------------------------
	virtual void pushIntoStack(lua_State *L) const {
		lua_newtable(L);
		int top = lua_gettop(L);
		for (size_t i=0; i<_size; ++i) {
			lua_pushnumber(L, i+1); //lua sequence starts with 1
			push(L, data[i]);
			lua_settable(L, top);
		}
	}
};
}} //namespaces

#endif /* LUATABLE_HPP_ */
