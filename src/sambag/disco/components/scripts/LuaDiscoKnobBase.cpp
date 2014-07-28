/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoKnobBase.cpp
 *
 *  Created on: Sun Jul 27 17:22:06 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoKnobBase.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaDiscoKnobBase
//=============================================================================
//-----------------------------------------------------------------------------
void LuaDiscoKnobBase::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    
    
}
//-----------------------------------------------------------------------------
void LuaDiscoKnobBase::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}

}}} // namespace(s)

