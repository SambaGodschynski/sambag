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
#include <sambag/com/exceptions/IllegalStateException.hpp>

namespace sambag { namespace lua {
namespace {
    typedef boost::unordered_set<ALuaObject::Ptr> Holder;
    Holder holder;
}
//=============================================================================
//  Class ALuaObject
//=============================================================================
//-----------------------------------------------------------------------------
const std::string ALuaObject::FIELDNAME_UID = SLUA_FIELDNAME_UID;
//-----------------------------------------------------------------------------
ALuaObject::MutexMap ALuaObject::mutexMap;
//-----------------------------------------------------------------------------
ALuaObject::Mutex & ALuaObject::getLock(lua_State *lua) {
    MutexMap::iterator it = mutexMap.find(lua);
    if (it==mutexMap.end()) {
        throw std::runtime_error("no registered mutex found");
    }
    return *(it->second.get());
}
//-----------------------------------------------------------------------------
void ALuaObject::createLock(lua_State *lua) {
    MutexMap::iterator it = mutexMap.find(lua);
    if (it!=mutexMap.end()) {
        return;
    }
    mutexMap[lua] = MutexPtr( new Mutex() );
}
//-----------------------------------------------------------------------------
ALuaObject::ALuaObject() {
}
//-----------------------------------------------------------------------------
ALuaObject::~ALuaObject() {
}
//-----------------------------------------------------------------------------
void ALuaObject::__destroy(lua_State *lua, ALuaObject::WPtr _obj) {
    ALuaObject::Ptr obj = _obj.lock();
    if (!obj) {
        return;
    }
    obj->__lua_gc(lua);
    Holder::iterator it = holder.find(obj);
    if (it==holder.end()) {
        SAMBAG_LOG_WARN<<"failed to unregister ALuaObject";
        return;
    }
    holder.erase(it);
    unregisterClassFunctions<Functions>(obj->getUId());
    unregisterClassFunctions<MetaFunctions>(obj->getUId());
    
    //obj will be killed after leaving scope
}
//-----------------------------------------------------------------------------
void ALuaObject::__lua_gc(lua_State *lua) {
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
    createLock(lua);
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
    
    lua_getfield(lua, index, FIELDNAME_UID.c_str());
    if (!lua_isstring(lua, -1)) {
        SAMBAG_THROW(sambag::com::exceptions::IllegalStateException,
        "failed to access sambag.lua.ALuaObject.UID");
    }
    boost::tuple<std::string> _uid;
    pop(lua, _uid);
    this->uid = boost::get<0>(_uid);
    
    addLuaFields(lua, index);
}
//-----------------------------------------------------------------------------
void ALuaObject::addLuaFields(lua_State * lua, int index) {
}

}} // namespace(s)
