/*
 * $$CLASS_NAME$$.cpp
 *
 *  Created on: $$DATE$$
 *      Author: Samba Godschysnki
 */

#include "$$CLASS_NAME$$.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>


$$NS$$
//=============================================================================
//  Class $$CLASS_NAME$$
//=============================================================================
//-----------------------------------------------------------------------------
LuaModelObject::LuaModelObject() {
}
//-----------------------------------------------------------------------------
void $$CLASS_NAME$$::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    $$LUA_REGISTER$$
    $$LUA_INIT_FIELDS$$
    
}
//-----------------------------------------------------------------------------
void LuaModelObject::__lua_gc(lua_State *lua) {
    $$LUA_UNREGISTER$$
    Super::__lua_gc(lua);
}
$$FIELD_SETTER_GETTER$$
$$NS_END$$ // namespace(s)
