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
class LuaMap : public ILuaTable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::map<Key, Value> Map;
private:
	//-------------------------------------------------------------------------
	Map map;
public:
	//-------------------------------------------------------------------------
	Map & getContainer() {
		return map;
	}
	//-------------------------------------------------------------------------
	const Map & getContainer() const {
		return map;
	}
	//-------------------------------------------------------------------------
	virtual ~LuaMap() {
	}
	//-------------------------------------------------------------------------
	Value & operator[](const Key &k) {
		return map[k];
	}
	//-------------------------------------------------------------------------
	const Value & operator[](const Key &k) const {
		return map[k];
	}
	//-------------------------------------------------------------------------
	size_t getSize() const {
		return map.size;
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
				map[k] = v;
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
		BOOST_FOREACH(const typename Map::value_type &it, map) {
			push(it.first, L);
			push(it.second, L);
			lua_settable(L, top);
		}
	}
};

}} //namespaces

#endif /* LUAMAP_HPP_ */

