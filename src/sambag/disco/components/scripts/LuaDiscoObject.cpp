/*
 * LuaDiscoObject.cpp
 *
 *  Created on: Sun Jul 27 16:30:42 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoObject.hpp"
#include <sambag/disco/components/SvgComponent.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class LuaDiscoObject::Factory
//=============================================================================
//-----------------------------------------------------------------------------
bool LuaDiscoObject::Factory::_registerExtension(const std::string &id, const Creator &f) {
    return creatorMap.insert(
        CreatorMap::value_type(id, f)
    ).second;
}
//-----------------------------------------------------------------------------
LuaDiscoObject::Ptr
LuaDiscoObject::Factory::createAndPush(lua_State *lua, Component::Ptr comp)
{
    Ptr neu;
    CreatorMap::const_iterator it = creatorMap.find(comp->getName());
    if (it!=creatorMap.end()) {
        neu = it->second(lua,comp);
    } else {
        neu = Ptr(new LuaDiscoObject());
        neu->createLuaObject(lua, "sambag.disco.components.LuaDiscoObject");
    }
    neu->component = comp;
    return neu;
}
//-----------------------------------------------------------------------------
LuaDiscoObject::Factory &LuaDiscoObject::Factory::instance() {
    typedef Loki::SingletonHolder<Factory> FactoryHolder;
    return FactoryHolder::Instance();
}
//=============================================================================
//  Class LuaDiscoObject
//=============================================================================
//-----------------------------------------------------------------------------
std::string LuaDiscoObject::toString(lua_State *lua) const {
    try {
        return getComponent()->getName();
    }  catch(const std::exception &ex) {
        lua::pushLuaError(lua, ex.what());
    } catch(...) {
        lua::pushLuaError(lua, "unknown error");
    }
    return "";
}
//-----------------------------------------------------------------------------
LuaDiscoObject::Component::Ptr LuaDiscoObject::getComponent() const {
    Component::Ptr res = component.lock();
    if (!res) {
        throw std::runtime_error("LuaDiscoObject::Component == NULL");
    }
    return res;
}
//-----------------------------------------------------------------------------
SvgComponent::Ptr LuaDiscoObject::getSvgComponent() const {
    SvgComponent::Ptr res = getComponent()->getFirstContainer<SvgComponent>();
    if (!res) {
        throw std::runtime_error("LuaDiscoObject::SvgComponent == NULL");
    }
    return res;
}
}}} // namespace(s)

