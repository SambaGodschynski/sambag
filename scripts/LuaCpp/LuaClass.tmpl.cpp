/*
 * LuaModelObject.cpp
 *
 *  Created on: Fri Mar 14 11:47:35 2014
 *      Author: Johannes Unger
 */

#include "$$CLASS_NAME$$.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>


$$NS$$
//=============================================================================
//  Class $$CLASS_NAME$$
//=============================================================================
//-----------------------------------------------------------------------------
LuaModelObject::LuaModelObject() {
}
//-----------------------------------------------------------------------------
void $$CLASS_NAME$$::addLuaFields(lua_State *lua, int index) :
	$$INIT_FIELDS$$
{
    Super::addLuaFields(lua, index);
    //slua::push(lua, getTypeId());
    //lua_setfield(lua, index, "__frxtype");
	// register functions
/*    int index = createClass<Functions, MetaFunctions, TupleAccessor>
    (
        lua,
        boost::make_tuple(),
        boost::make_tuple(
            boost::bind(&ALuaObject::__destroy, lua, ALuaObject::WPtr(shared_from_this())),
            boost::bind(&ALuaObject::__tostring, this, lua),
            boost::bind(&ALuaObject::isequal, this, lua)
        ),
        name
    );*/
}
//-----------------------------------------------------------------------------
void LuaModelObject::__lua_gc(lua_State *lua) {
    Super::__lua_gc(lua);
	// unregister functions
	// unregisterClassFunctions<Functions>(obj->getUId());
    // unregisterClassFunctions<MetaFunctions>(obj->getUId());
}
$$NS_END$$ // namespace(s)
