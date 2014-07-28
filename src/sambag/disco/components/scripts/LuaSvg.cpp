/*
 * LuaSvg.cpp
 *
 *  Created on: Sun Jul 27 16:29:56 2014
 *      Author: Samba Godschysnki
 */

#include "LuaSvg.hpp"
#include "LuaDiscoObject.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaSvg
//=============================================================================
//-----------------------------------------------------------------------------
sambag::lua::IgnoreReturn
LuaSvg::getObjectById(lua_State *lua, const std::string & id)
{
    SvgComponent::Dummy::Ptr comp = svg->getDummyById(id);
    if (!comp) {
        lua_pushnil(lua);
        return sambag::lua::IgnoreReturn();
    }
    LuaDiscoObject::Factory::instance().createAndPush(lua, comp);
    return sambag::lua::IgnoreReturn();
}
//-----------------------------------------------------------------------------
sambag::lua::IgnoreReturn
LuaSvg::getObjectsByClass(lua_State *lua, const std::string & _class)
{
    std::vector<SvgComponent::DummyPtr> dummies;
    svg->getDummiesByClass(_class, dummies);
    lua_createtable(lua, dummies.size(), 0);
    int top = lua_gettop(lua);
    int luaIndex = 1;
    BOOST_FOREACH(SvgComponent::DummyPtr x, dummies) {
        lua_pushnumber(lua, luaIndex++);
        LuaDiscoObject::Factory::instance().createAndPush(lua, x);
        lua_settable(lua, top);
    }
    return sambag::lua::IgnoreReturn();
}
//-----------------------------------------------------------------------------
LuaSvg::Ptr LuaSvg::createAndPush(lua_State *lua, SvgComponentPtr svg) {
    Ptr neu(new LuaSvg());
    neu->svg=svg;
    neu->createLuaObject(lua, "sambag.disco.components.LuaSvg");
    return neu;
}
}}} // namespace(s)

