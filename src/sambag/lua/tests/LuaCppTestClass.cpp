/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaCppTestClass.cpp
 *
 *  Created on: Fri Apr 25 11:42:53 2014
 *      Author: Samba Godschysnki
 */

#include "LuaCppTestClass.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>

namespace {
  template <typename RetType>
  struct __CallImpl {
    template <class Tuple>
    static RetType hauRein(lua_State *lua, const std::string &fName, const Tuple &args) 
    {
      boost::tuple<RetType> res;
      sambag::lua::callLuaFunc(lua, fName, args, res);
      return boost::get<0>(res);
    }
  };
  template <>
  struct __CallImpl<void> {
    template <class Tuple>
    static void hauRein(lua_State *lua, const std::string &fName, const Tuple &args) 
    {
      sambag::lua::callLuaFunc(lua, fName, args);
    }
  };
} // namespace

namespace tests { 
//=============================================================================
//  Class LuaCppTestClass
//=============================================================================
//-----------------------------------------------------------------------------
void LuaCppTestClass::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    registerClassFunctions<Functions1, TupleAccessor>(
	lua,
	boost::make_tuple(boost::bind(&LuaCppTestClass::add, this, lua, _1, _2)),
	index, 
	getUId() 
	); 

	
    set_name(lua, "peter");
    
}
//-----------------------------------------------------------------------------
void LuaCppTestClass::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    unregisterClassFunctions<Functions1>(getUId());
	unregisterClassFunctions<MetaFunctions>(getUId());
	
    Super::__lua_gc(lua);
}
void LuaCppTestClass::set_name(lua_State *lua, const std::string & value) {
    using namespace sambag::lua;
    push(lua, value);
    lua_setfield(lua, -1, "name");
}
        
std::string LuaCppTestClass::get_name(lua_State *lua) {
    using namespace sambag::lua;
    lua_getfield(lua, -1, "name");
    boost::tuple<std::string> res;
    pop(lua, res);
    return boost::get<0>(res);
}
        
bool LuaCppTestClass::script_impl_whoAreYou(lua_State *lua)
{
    return sambag::lua::hasFunction(lua, "whoAreYou");
}

std::string LuaCppTestClass::whoAreYou(lua_State *lua)
{
    return __CallImpl<std::string>::hauRein(lua, "whoAreYou", boost::tuple<>());
}

void LuaCppTestClass::raw_whoAreYou(lua_State *lua, int narg, int nret)
{
    lua_getglobal(lua, "whoAreYou");
    if (!lua_isfunction(lua, -1)==1) {
        throw sambag::lua::NoFunction();
    }
    if (lua_pcall(lua, narg, nret, 0)!=0) {
        throw sambag::lua::ExecutionFailed(std::string(lua_tostring(lua, -1)));
    }    
}

} // namespace(s)

