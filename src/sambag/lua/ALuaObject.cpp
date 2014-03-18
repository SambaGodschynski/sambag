/*
 * LuaPObject.cpp
 *
 *  Created on: Fri Mar 14 11:47:30 2014
 *      Author: Johannes Unger
 */

#include "ALuaObject.hpp"
#include <boost/bind.hpp>
#include <sambag/lua/LuaRegisterFunction.hpp>
#include <sambag/com/Common.hpp>
#include <boost/unordered_set.hpp>

namespace sambag { namespace lua {
namespace {
    typedef boost::unordered_set<ALuaObject::Ptr> Holder;
    Holder holder;
}
//=============================================================================
//  Class ALuaObject
//=============================================================================
//-----------------------------------------------------------------------------
ALuaObject::ALuaObject() {
}
//-----------------------------------------------------------------------------
void ALuaObject::__destroy(lua_State *lua, ALuaObject::WPtr _obj) {
    ALuaObject::Ptr obj = _obj.lock();
    if (!obj) {
        return;
    }
    obj->__gc(lua);
    Holder::iterator it = holder.find(obj);
    if (it==holder.end()) {
        SAMBAG_LOG_WARN<<"failed to unregister ALuaObject";
        return;
    }
    holder.erase(it);
    
    //obj will be killed after leaving scope
}
//-----------------------------------------------------------------------------
void ALuaObject::__gc(lua_State *lua) {
    SAMBAG_LOG_TRACE<<toString(lua)<<" __gc()";
}
//-----------------------------------------------------------------------------
std::string ALuaObject::__tostring(lua_State *lua) {
    return toString(lua);
}
//-----------------------------------------------------------------------------
bool ALuaObject::isequal(lua_State *lua) const {
    return (bool)lua_rawequal(lua, -1, -2);
}
//-----------------------------------------------------------------------------
void ALuaObject::createLuaObject(lua_State * lua, const std::string &name)
{
    ALuaObject::Ptr self = shared_from_this();
    holder.insert(self);
    luaName = name;
    int index = createClass<Functions, MetaFunctions, TupleAccessor>
    (
        lua,
        boost::make_tuple(),
        boost::make_tuple(
            boost::bind(&ALuaObject::__destroy, lua, ALuaObject::WPtr(shared_from_this())),
            boost::bind(&ALuaObject::__tostring, this, lua),
            boost::bind(&ALuaObject::isequal, this, lua)
        ),
        name
    );
    addLuaFields(lua, index);
}
//-----------------------------------------------------------------------------
void ALuaObject::addLuaFields(lua_State * lua, int index) {
}

}} // namespace(s)
