/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaCppTestClass.hpp
 *
 *  Created on: Fri Apr 25 11:42:53 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaCppTestClass_H
#define SAMBAG_LuaCppTestClass_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/TypeList.h>
#include <sambag/lua/ALuaObject.hpp>



namespace tests { 
//=============================================================================
class LuaCppTestClass : public sambag::lua::ALuaObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef sambag::lua::ALuaObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaCppTestClass> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaCppTestClass> WPtr;
private:
protected:
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(add, void (int, int));
    typedef LOKI_TYPELIST_1(Frx_add_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @brief TODO
	*/
	virtual void add(lua_State *lua, int a, int b) = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief field getter and setter
     * @note works only when lua class table is on stack
     */
    virtual std::string get_name(lua_State *lua);
	virtual void set_name(lua_State *lua, const std::string & value);
    //-------------------------------------------------------------------------
    /**
     * @override 
     */	
    virtual void addLuaFields(lua_State *lua, int index);
    ///////////////////////////////////////////////////////////////////////////
    // Lua call implements request, return true if a script implements
    // the xxx function
    bool script_impl_whoAreYou(lua_State *lua);
    ///////////////////////////////////////////////////////////////////////////
    // there are two ways to call a LC functions: you can use the regular
    // xxx() call where the argument and return treatment is handled
    // by LuaHelper. Or you use raw_xxx(narg, nret) where you have to handle return and
    // args for your self (e.g. when using unsupported LuaHelper types).
    std::string whoAreYou(lua_State *lua);
	void raw_whoAreYou(lua_State *lua, int narg, int nret);
public:
    //-------------------------------------------------------------------------
    LuaCppTestClass();
    //-------------------------------------------------------------------------
    /**
     * @brief called when lua object will be removed.
     */
    virtual void __lua_gc(lua_State *lua);
public:
    //-------------------------------------------------------------------------
    virtual ~LuaCppTestClass() {}
private:
public:
}; // LuaCppTestClass
}

#endif /* SAMBAG_LuaCppTestClass_H */

