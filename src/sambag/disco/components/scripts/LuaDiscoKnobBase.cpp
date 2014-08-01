/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoKnobBase.cpp
 *
 *  Created on: Fri Aug  1 13:37:08 2014
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
    registerClassFunctions<Functions1, TupleAccessor>(
	lua,
	boost::make_tuple(boost::bind(&LuaDiscoKnobBase::getValue, this, lua),
		boost::bind(&LuaDiscoKnobBase::setValue, this, lua, _1),
		boost::bind(&LuaDiscoKnobBase::addListener, this, lua, _1)),
	index, 
	getUId() 
	); 

	
    
}
//-----------------------------------------------------------------------------
void LuaDiscoKnobBase::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<Functions1>(getUId());
	unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}

}}} // namespace(s)

