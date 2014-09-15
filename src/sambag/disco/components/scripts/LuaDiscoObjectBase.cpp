/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoObjectBase.cpp
 *
 *  Created on: Mon Sep 15 20:56:44 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoObjectBase.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaDiscoObjectBase
//=============================================================================
//-----------------------------------------------------------------------------
void LuaDiscoObjectBase::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    registerClassFunctions<Functions1, TupleAccessor>(
	lua,
	boost::make_tuple(boost::bind(&LuaDiscoObjectBase::addOnEnterListener, this, lua, _1),
		boost::bind(&LuaDiscoObjectBase::addOnExitListener, this, lua, _1),
		boost::bind(&LuaDiscoObjectBase::addOnClickedListener, this, lua, _1)),
	index, 
	getUId() 
	); 

	
    
}
//-----------------------------------------------------------------------------
void LuaDiscoObjectBase::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<Functions1>(getUId());
	unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}

}}} // namespace(s)

