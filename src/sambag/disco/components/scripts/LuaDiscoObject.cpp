/*
 * LuaDiscoObject.cpp
 *
 *  Created on: Sun Jul 27 16:30:42 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoObject.hpp"
#include <sambag/disco/components/SvgComponent.hpp>

namespace sambag { namespace disco { namespace components {


namespace {
    template <int Ev>
    void __onMouse(lua_State *lua,
        const events::MouseEvent &ev, const std::string &expr)
    {
        if (ev.getType() != Ev) {
            return;
        }
        try {
            lua::executeString(lua, expr);
        } catch(const std::exception &ex) {
            SAMBAG_LOG_ERR<<"LuaDiscoObject::__onMouse failed: "<<ex.what();
        } catch (...) {
            SAMBAG_LOG_ERR<<"LuaDiscoObject::__onMouse failed: unkown error";
        }
    }
} // namespace(s)


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
    SvgComponent::Ptr svg = neu->getSvgComponent();
    neu->setSceneGraph(svg->getSceneGraph());
    neu->setObject(svg->getDrawable(comp));
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
//-----------------------------------------------------------------------------
void LuaDiscoObject::addOnEnterListener(lua_State *lua, const std::string & expr)
{
    enum {Ev = events::MouseEvent::DISCO_MOUSE_ENTERED};
    getComponent()->EventSender<events::MouseEvent>::addTrackedEventListener(
        boost::bind(&__onMouse<Ev>, lua, _2, expr),
        shared_from_this()
    );
}
//-----------------------------------------------------------------------------
void LuaDiscoObject::addOnExitListener(lua_State *lua, const std::string & expr)
{
    enum {Ev = events::MouseEvent::DISCO_MOUSE_EXITED};
    getComponent()->EventSender<events::MouseEvent>::addTrackedEventListener(
        boost::bind(&__onMouse<Ev>, lua, _2, expr),
        shared_from_this()
    );
}
//-----------------------------------------------------------------------------
void LuaDiscoObject::addOnClickedListener(lua_State *lua, const std::string & expr)
{
    enum {Ev = events::MouseEvent::DISCO_MOUSE_CLICKED};
    getComponent()->EventSender<events::MouseEvent>::addTrackedEventListener(
        boost::bind(&__onMouse<Ev>, lua, _2, expr),
        shared_from_this()
    );
}
//-----------------------------------------------------------------------------
void LuaDiscoObject::setVisible(lua_State *lua, bool x) {
    getComponent()->setVisible(x);
}
//-----------------------------------------------------------------------------
bool LuaDiscoObject::isVisible(lua_State *lua) {
    return getComponent()->isVisible();
}
//-----------------------------------------------------------------------------
void LuaDiscoObject::redraw(lua_State *lua) {
    getSvgComponent()->redraw();
}
}}} // namespace(s)

