/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaSvgObjectBase.cpp
 *
 *  Created on: Fri Sep 26 19:23:44 2014
 *      Author: Samba Godschysnki
 */

#include "LuaSvgObjectBase.hpp"

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaSvgObjectBase
//=============================================================================
//-----------------------------------------------------------------------------
void LuaSvgObjectBase::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    registerClassFunctions<Functions1, TupleAccessor>(
	lua,
	boost::make_tuple(boost::bind(&LuaSvgObjectBase::getId, this, lua),
		boost::bind(&LuaSvgObjectBase::getClasses, this, lua),
		boost::bind(&LuaSvgObjectBase::setVisible, this, lua, _1),
		boost::bind(&LuaSvgObjectBase::isVisible, this, lua),
		boost::bind(&LuaSvgObjectBase::setStyle, this, lua, _1),
		boost::bind(&LuaSvgObjectBase::getStyle, this, lua),
		boost::bind(&LuaSvgObjectBase::calculateStyle, this, lua),
		boost::bind(&LuaSvgObjectBase::getBounds, this, lua),
		boost::bind(&LuaSvgObjectBase::setText, this, lua, _1),
		boost::bind(&LuaSvgObjectBase::getText, this, lua)),
	index, 
	getUId() 
	); 

	
    
}
//-----------------------------------------------------------------------------
void LuaSvgObjectBase::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<Functions1>(getUId());
	unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}

}}} // namespace(s)

