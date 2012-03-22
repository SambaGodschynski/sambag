/*
 * LuaMap.hpp
 *
 *  Created on: 15 Feb 2012
 *      Author: samba
 */

#ifndef LUAMAP_HPP_
#define LUAMAP_HPP_

#include "ILuaTable.hpp"
#include "LuaHelper.hpp"
#include <lua.hpp>
#include <boost/shared_array.hpp>
#include <boost/foreach.hpp>

namespace sambag { namespace lua {
//=============================================================================
/**
 * Implements ILuaTable using std::map<Key, Value>.
 */
template <typename Key, typename Value>
class LuaMap : public ILuaTable, public std::map<Key, Value> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::map<Key, Value> Map;
	//-------------------------------------------------------------------------
	virtual ~LuaMap() {
	}
	//-------------------------------------------------------------------------
	size_t getSize() const {
		return typename Map::size();
	}
	//-------------------------------------------------------------------------
	virtual bool getFromStack(lua_State *L, int index) {
		lua_pushnil(L);  /* first key */
		--index;
		bool res = true;
		while (lua_next(L, index) != 0) {
			// TODO: check sequence
			// uses 'key' (at index -2) and 'value' (at index -1)
			Key k;
			if (get(k, L, -2)) {
				Value v;
				if (!get(v, L, -1))
					return false;
				(*this)[k] = v;
			} else { // invalid key
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
		BOOST_FOREACH(const typename Map::value_type &it, *this) {
			push(it.first, L);
			push(it.second, L);
			lua_settable(L, top);
		}
	}
};

}} //namespaces

#endif /* LUAMAP_HPP_ */

