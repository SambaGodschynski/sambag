/*
 * LuaDiscoKnob.cpp
 *
 *  Created on: Sun Jul 27 17:22:06 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoKnob.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaDiscoKnob
//=============================================================================
//-----------------------------------------------------------------------------
LuaDiscoKnob::Ptr
LuaDiscoKnob::createAndPush(lua_State *lua, Component::Ptr comp)
{
    Ptr neu(new LuaDiscoKnob());
    neu->createLuaObject(lua, "sambag.disco.components.LuaDiscoKnob");
    return neu;
}
}}} // namespace(s)

