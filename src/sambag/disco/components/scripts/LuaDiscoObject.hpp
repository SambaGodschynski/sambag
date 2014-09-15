/*
 * LuaDiscoObject.hpp
 *
 *  Created on: Sun Jul 27 16:30:34 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoObject_H
#define SAMBAG_LuaDiscoObject_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include "LuaDiscoObjectBase.hpp"
#include <sambag/disco/components/Forward.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <sambag/disco/components/SvgComponent.hpp>
#include <loki/Singleton.h>
#include <sambag/disco/components/events/MouseEvent.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
class LuaDiscoObject : public LuaDiscoObjectBase {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaDiscoObjectBase Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaDiscoObject> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaDiscoObject> WPtr;
    //-------------------------------------------------------------------------
    typedef SvgComponent::Dummy Component;
    //-------------------------------------------------------------------------
    struct Factory {
        friend struct Loki::CreateUsingNew<Factory>;
        typedef boost::function<Ptr(lua_State *lua, Component::Ptr)> Creator;
        typedef std::map<std::string, Creator> CreatorMap;
        //---------------------------------------------------------------------
        bool _registerExtension(const std::string &id, const Creator &f);
        //---------------------------------------------------------------------
        Ptr createAndPush(lua_State *lua, Component::Ptr comp);
        //---------------------------------------------------------------------
        static Factory & instance();
        private:
            CreatorMap creatorMap;
    };
private:
    //-------------------------------------------------------------------------
    Component::WPtr component;
protected:
    //-------------------------------------------------------------------------
    LuaDiscoObject() {}
public:
    //-------------------------------------------------------------------------
    virtual std::string toString(lua_State *lua) const;
    //-------------------------------------------------------------------------
    Component::Ptr getComponent() const;
    //-------------------------------------------------------------------------
    SvgComponent::Ptr getSvgComponent() const;
    //-------------------------------------------------------------------------
    virtual ~LuaDiscoObject() {}
    ///////////////////////////////////////////////////////////////////////////
    // impl.
    virtual void addOnEnterListener(lua_State *lua, const std::string & expr);
    virtual void addOnExitListener(lua_State *lua, const std::string & expr);
    virtual void addOnClickedListener(lua_State *lua, const std::string & expr);
	virtual void setVisible(lua_State *lua, bool x);
	virtual bool isVisible(lua_State *lua);
private:
public:
}; // LuaDiscoObject
}}}

#endif /* SAMBAG_LuaDiscoObject_H */

