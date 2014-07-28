/*
 *
 * LuaDiscoButton.hpp
 *
 *  Created on: Sun Jul 27 17:22:31 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoButton_H
#define SAMBAG_LuaDiscoButton_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>
#include "LuaDiscoButtonBase.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaDiscoButton : public LuaDiscoButtonBase {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaDiscoObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaDiscoButton> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaDiscoButton> WPtr;
    //-------------------------------------------------------------------------
private:
protected:
    //-------------------------------------------------------------------------
    LuaDiscoButton() {}
    //-------------------------------------------------------------------------
    void onAction(lua_State *lua, const std::string &expr);
public:
    //-------------------------------------------------------------------------
    IDrawable::Ptr getTextElement() const;
    //-------------------------------------------------------------------------
    static Ptr createAndPush(lua_State *lua, Component::Ptr comp);
    //-------------------------------------------------------------------------
    virtual ~LuaDiscoButton() {}
    ///////////////////////////////////////////////////////////////////////////
    virtual std::string getText(lua_State *lua);
	virtual void setText(lua_State *lua, const std::string & text);
    virtual void addOnClickedListener(lua_State *lua, const std::string & expr);
private:
public:
}; // LuaDiscoButton

namespace {
    const bool LuaButtonReg =
        LuaDiscoObject::Factory::instance()._registerExtension("SvgComponent::Dummy(#disco-button)",
            &LuaDiscoButton::createAndPush
        );
}

}}}

#endif /* SAMBAG_LuaDiscoButton_H */

