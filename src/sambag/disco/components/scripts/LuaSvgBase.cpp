/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaSvgBase.cpp
 *
 *  Created on: Tue Aug 12 13:06:28 2014
 *      Author: Samba Godschysnki
 */

#include "LuaSvgBase.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaSvgBase
//=============================================================================
//-----------------------------------------------------------------------------
void LuaSvgBase::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    registerClassFunctions<Functions1, TupleAccessor>(
	lua,
	boost::make_tuple(boost::bind(&LuaSvgBase::getObjectById, this, lua, _1),
		boost::bind(&LuaSvgBase::getObjectsByClass, this, lua, _1),
		boost::bind(&LuaSvgBase::select, this, lua, _1)),
	index, 
	getUId() 
	); 

	
    
}
//-----------------------------------------------------------------------------
void LuaSvgBase::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<Functions1>(getUId());
	unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}

}}} // namespace(s)

