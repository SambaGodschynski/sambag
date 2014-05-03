/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaCppTestClass.hpp
 *
 *  Created on: Fri Apr 25 13:13:05 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaCppTestClass_H
#define SAMBAG_LuaCppTestClass_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
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
    LuaCppTestClass() {}
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(add, int (int, int));
    typedef LOKI_TYPELIST_1(Frx_add_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @brief TODO
	*/
	virtual int add(lua_State *lua, int a, int b) = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief field getter and setter
     */
    virtual std::string get_name(lua_State *lua, int index);
	virtual void set_name(lua_State *lua, const std::string & value, int index);
    //-------------------------------------------------------------------------
    /**
     * @override 
     */	
    virtual void addLuaFields(lua_State *lua, int index);
public:
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

