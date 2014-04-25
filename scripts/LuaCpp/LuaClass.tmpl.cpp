/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * $$CLASS_NAME$$.cpp
 *
 *  Created on: $$DATE$$
 *      Author: Samba Godschysnki
 */

#include "$$CLASS_NAME$$.hpp"
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

$$NS$$
//=============================================================================
//  Class $$CLASS_NAME$$
//=============================================================================
//-----------------------------------------------------------------------------
void $$CLASS_NAME$$::addLuaFields(lua_State *lua, int index) 
{
    using namespace sambag::lua;
    Super::addLuaFields(lua, index);
    // register functions
    $$LUA_REGISTER$$
    $$LUA_INIT_FIELDS$$
    
}
//-----------------------------------------------------------------------------
void $$CLASS_NAME$$::__lua_gc(lua_State *lua) {
    using namespace sambag::lua;
    $$LUA_UNREGISTER$$
    Super::__lua_gc(lua);
}
$$FIELD_SETTER_GETTER$$
$$LUA_CALL_IMPL$$
$$LUA_CALL_FS$$
$$NS_END$$ // namespace(s)
