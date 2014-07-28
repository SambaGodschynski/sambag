/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoObjectBase.hpp
 *
 *  Created on: Mon Jul 28 10:25:19 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoObjectBase_H
#define SAMBAG_LuaDiscoObjectBase_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>



namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaDiscoObjectBase : public sambag::lua::ALuaObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef sambag::lua::ALuaObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaDiscoObjectBase> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaDiscoObjectBase> WPtr;
    //-------------------------------------------------------------------------
    
private:
protected:
    //-------------------------------------------------------------------------
    LuaDiscoObjectBase() {}
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(getId, std::string ());
	SAMBAG_LUA_FTAG(getClasses, sambag::lua::IgnoreReturn ());
    typedef LOKI_TYPELIST_2(Frx_getId_Tag, 
	Frx_getClasses_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @return the object svg id
	*/
	virtual std::string getId(lua_State *lua) = 0;
	/**
	* @return a sequence of svg class names
	*/
	virtual sambag::lua::IgnoreReturn getClasses(lua_State *lua) = 0;
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
    virtual ~LuaDiscoObjectBase() {}
private:
public:
}; // LuaDiscoObjectBase
}}}

#endif /* SAMBAG_LuaDiscoObjectBase_H */

