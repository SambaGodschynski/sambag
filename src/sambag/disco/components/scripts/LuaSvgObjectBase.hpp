/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaSvgObjectBase.hpp
 *
 *  Created on: Tue Aug 12 15:33:14 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaSvgObjectBase_H
#define SAMBAG_LuaSvgObjectBase_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>



namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaSvgObjectBase : public sambag::lua::ALuaObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef sambag::lua::ALuaObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaSvgObjectBase> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaSvgObjectBase> WPtr;
    //-------------------------------------------------------------------------
    
private:
protected:
    //-------------------------------------------------------------------------
    LuaSvgObjectBase() {}
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(getId, std::string ());
	SAMBAG_LUA_FTAG(getClasses, sambag::lua::IgnoreReturn ());
	SAMBAG_LUA_FTAG(setVisible, void (bool));
	SAMBAG_LUA_FTAG(isVisible, bool ());
	SAMBAG_LUA_FTAG(setStyle, void (std::string));
	SAMBAG_LUA_FTAG(getStyle, std::string ());
	SAMBAG_LUA_FTAG(calculateStyle, std::string ());
    typedef LOKI_TYPELIST_7(Frx_getId_Tag, 
	Frx_getClasses_Tag, 
	Frx_setVisible_Tag, 
	Frx_isVisible_Tag, 
	Frx_setStyle_Tag, 
	Frx_getStyle_Tag, 
	Frx_calculateStyle_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @return the object svg id
	*/
	virtual std::string getId(lua_State *lua) = 0;
	/**
	* @return a sequence of svg class names
	*/
	virtual sambag::lua::IgnoreReturn getClasses(lua_State *lua) = 0;
	/**
	* @brief set objects visibility
	*/
	virtual void setVisible(lua_State *lua, bool x) = 0;
	/**
	* @return true if object visible
	*/
	virtual bool isVisible(lua_State *lua) = 0;
	/**
	* @brief set style string
	*/
	virtual void setStyle(lua_State *lua, const std::string & style) = 0;
	/**
	* @return style string
	*/
	virtual std::string getStyle(lua_State *lua) = 0;
	/**
	* @return calculated style string
	*/
	virtual std::string calculateStyle(lua_State *lua) = 0;
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
    virtual ~LuaSvgObjectBase() {}
private:
public:
}; // LuaSvgObjectBase
}}}

#endif /* SAMBAG_LuaSvgObjectBase_H */

