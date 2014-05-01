/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * $$CLASS_NAME$$.cpp
 *
 *  Created on: $$DATE$$
 *      Author: Samba Godschysnki
 */

#include "$$CLASS_NAME$$.hpp"

$$NS$$
//=============================================================================
//  Class $$CLASS_NAME$$
//=============================================================================
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
void $$CLASS_NAME$$::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    $$LUA_UNREGISTER$$
    Super::__lua_gc(lua);
}
$$FIELD_SETTER_GETTER$$
$$NS_END$$ // namespace(s)
