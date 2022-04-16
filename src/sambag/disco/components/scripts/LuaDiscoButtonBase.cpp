/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoButtonBase.cpp
 *
 *  Created on: Mon Sep 15 20:57:02 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoButtonBase.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaDiscoButtonBase
//=============================================================================
//-----------------------------------------------------------------------------
void LuaDiscoButtonBase::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    registerClassFunctions<Functions1, TupleAccessor>(
	lua,
	boost::make_tuple(boost::bind(&LuaDiscoButtonBase::getText, this, lua),
		boost::bind(&LuaDiscoButtonBase::setText, this, lua, _1)),
	index, 
	getUId() 
	); 

	
    
}
//-----------------------------------------------------------------------------
void LuaDiscoButtonBase::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<Functions1>(getUId());
	unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}

}}} // namespace(s)

