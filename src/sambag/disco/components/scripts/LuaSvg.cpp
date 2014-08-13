/*
 * LuaSvg.cpp
 *
 *  Created on: Sun Jul 27 16:29:56 2014
 *      Author: Samba Godschysnki
 */

#include "LuaSvg.hpp"
#include "LuaDiscoObject.hpp"
#include "LuaSvgObject.hpp"
#include <sambag/disco/svg/graphicElements/SceneGraphHelper.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaSvg
//=============================================================================
//-----------------------------------------------------------------------------
void LuaSvg::push(lua_State *lua, IDrawable::Ptr x) {
    svg::graphicElements::ISceneGraph::Ptr g = svg->getSceneGraph();
    if (!x) {
        lua_pushnil(lua);
        return;
    }
    SvgComponent::Dummy::Ptr comp = svg->getDummy(x);
    if (comp) {
        LuaDiscoObject::Factory::instance().createAndPush(lua, comp);
        return;
    }
    LuaSvgObject::createAndPush(lua, x, g);
}
//-----------------------------------------------------------------------------
sambag::lua::IgnoreReturn
LuaSvg::getObjectById(lua_State *lua, const std::string & id)
{
    IDrawable::Ptr obj = svg->getSceneGraph()->getElementById(id);
    push(lua, obj);
    return sambag::lua::IgnoreReturn();
}
//-----------------------------------------------------------------------------
sambag::lua::IgnoreReturn
LuaSvg::getObjectsByClass(lua_State *lua, const std::string & _class)
{
    std::vector<IDrawable::Ptr> objs;
    svg->getSceneGraph()->getElementsByClassX(_class, objs);
    
    lua_createtable(lua, objs.size(), 0);
    int top = lua_gettop(lua);
    int luaIndex = 1;
    BOOST_FOREACH(IDrawable::Ptr x, objs) {
        lua_pushnumber(lua, luaIndex++);
        push(lua, x);
        lua_settable(lua, top);
    }
    return sambag::lua::IgnoreReturn();
}
//-----------------------------------------------------------------------------
sambag::lua::IgnoreReturn
LuaSvg::select(lua_State *lua, const std::string & sel)
{
    std::vector<IDrawable::Ptr> objs;
    svg->getSceneGraph()->select(sel, objs);
    
    lua_createtable(lua, objs.size(), 0);
    int top = lua_gettop(lua);
    int luaIndex = 1;
    BOOST_FOREACH(IDrawable::Ptr x, objs) {
        lua_pushnumber(lua, luaIndex++);
        push(lua, x);
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
//-----------------------------------------------------------------------------
void LuaSvg::redraw(lua_State *lua) {
    svg->redraw();
}
}}} // namespace(s)

