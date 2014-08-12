/*
 * LuaSvgObject.cpp
 *
 *  Created on: Sun Jul 27 16:30:42 2014
 *      Author: Samba Godschysnki
 */

#include "LuaSvgObject.hpp"
#include <sambag/disco/components/SvgComponent.hpp>


namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class LuaSvgObject
//=============================================================================
//-----------------------------------------------------------------------------
std::string LuaSvgObject::toString(lua_State *lua) const {
    try {
        return getObject()->toString();
    }  catch(const std::exception &ex) {
        lua::pushLuaError(lua, ex.what());
    } catch(...) {
        lua::pushLuaError(lua, "unknown error");
    }
    return "";
}
//-----------------------------------------------------------------------------
LuaSvgObject::SceneGraph::Ptr LuaSvgObject::getSceneGraph() const {
    SceneGraph::Ptr res = _scene.lock();
    if (!res) {
        throw std::runtime_error("LuaSvgObject::SceneGraph == NULL");
    }
    return res;
}
//-----------------------------------------------------------------------------
IDrawable::Ptr LuaSvgObject::getObject() const {
    IDrawable::Ptr res = _object.lock();
    if (!res) {
        throw std::runtime_error("LuaSvgObject::Object == NULL");
    }
    return res;
}
//-----------------------------------------------------------------------------
std::string LuaSvgObject::getId(lua_State *lua) {
    return getSceneGraph()->getIdName(getObject());
}
//-----------------------------------------------------------------------------
sambag::lua::IgnoreReturn LuaSvgObject::getClasses(lua_State *lua) {
    std::vector<std::string> classes;
    getSceneGraph()->getClassNamesX(getObject(), classes);
    lua_createtable(lua, classes.size(), 0);
    int top = lua_gettop(lua);
    int luaC = 1;
    BOOST_FOREACH(const std::string &x, classes) {
        lua::push(lua, luaC++);
        lua::push(lua, x);
        lua_settable(lua, top);
    }
    return sambag::lua::IgnoreReturn();
}
//-----------------------------------------------------------------------------
void LuaSvgObject::setVisible(lua_State *lua, bool x) {
    getSceneGraph()->setVisible(getObject(), x);
}
//-----------------------------------------------------------------------------
bool LuaSvgObject::isVisible(lua_State *lua) {
    return getSceneGraph()->isVisible(getObject());
}
//-----------------------------------------------------------------------------
void LuaSvgObject::setSceneGraph(SceneGraph::Ptr scene) {
    _scene = scene;
}
//-----------------------------------------------------------------------------
void LuaSvgObject::setObject(IDrawable::Ptr obj) {
    _object = obj;
}
//-----------------------------------------------------------------------------
LuaSvgObject::Ptr
LuaSvgObject::createAndPush(lua_State *lua,
    IDrawable::Ptr object, LuaSvgObject::SceneGraph::Ptr scene)
{
    Ptr neu;
    neu = Ptr(new LuaSvgObject());
    neu->createLuaObject(lua, "sambag.disco.components.LuaSvgObject");
    neu->setObject(object);
    neu->setSceneGraph(scene);
    return neu;

}
}}} // namespace(s)

