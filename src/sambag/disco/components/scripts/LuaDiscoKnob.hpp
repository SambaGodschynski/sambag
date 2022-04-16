/*
 * LuaDiscoKnob.hpp
 *
 *  Created on: Sun Jul 27 17:21:58 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoKnob_H
#define SAMBAG_LuaDiscoKnob_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>

#include "LuaDiscoKnobBase.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaDiscoKnob : public LuaDiscoKnobBase {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaDiscoObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaDiscoKnob> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaDiscoKnob> WPtr;
    //-------------------------------------------------------------------------
protected:
    //-------------------------------------------------------------------------
    void onChanged(lua_State *lua, const std::string &expr);
public:
    //-------------------------------------------------------------------------
    virtual ~LuaDiscoKnob() {}
    //-------------------------------------------------------------------------
    static Ptr createAndPush(lua_State *lua, Component::Ptr comp);
    ///////////////////////////////////////////////////////////////////////////
    // impl.
	virtual float getValue(lua_State *lua);
	virtual void setValue(lua_State *lua, float x);
	virtual void addListener(lua_State *lua, const std::string & expr);
private:
public:
}; // LuaDiscoKnob

namespace {
    const bool LuaKnobReg =
        LuaDiscoObject::Factory::instance()._registerExtension("SvgComponent::Dummy(#disco-knob)",
            &LuaDiscoKnob::createAndPush
        );
}


}}}

#endif /* SAMBAG_LuaDiscoKnob_H */

