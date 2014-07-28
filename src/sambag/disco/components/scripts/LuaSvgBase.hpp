/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaSvgBase.hpp
 *
 *  Created on: Sun Jul 27 16:38:34 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaSvgBase_H
#define SAMBAG_LuaSvgBase_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>



namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaSvgBase : public sambag::lua::ALuaObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef sambag::lua::ALuaObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaSvgBase> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaSvgBase> WPtr;
    //-------------------------------------------------------------------------
    
private:
protected:
    //-------------------------------------------------------------------------
    LuaSvgBase() {}
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(getObjectById, sambag::lua::IgnoreReturn (std::string));
	SAMBAG_LUA_FTAG(getObjectsByClass, sambag::lua::IgnoreReturn (std::string));
    typedef LOKI_TYPELIST_2(Frx_getObjectById_Tag, 
	Frx_getObjectsByClass_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @return a @see DiscoObject by svg id or nil 
	*/
	virtual sambag::lua::IgnoreReturn getObjectById(lua_State *lua, const std::string & id) = 0;
	/**
	* @return a sequence of @see DiscoObjects by svg class 
	*/
	virtual sambag::lua::IgnoreReturn getObjectsByClass(lua_State *lua, const std::string & _class) = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief field getter and setter
     */
    
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
    virtual ~LuaSvgBase() {}
private:
public:
}; // LuaSvgBase
}}}

#endif /* SAMBAG_LuaSvgBase_H */

