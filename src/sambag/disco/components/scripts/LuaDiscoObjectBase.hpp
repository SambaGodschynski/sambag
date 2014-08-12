/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoObjectBase.hpp
 *
 *  Created on: Tue Aug 12 10:03:59 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoObjectBase_H
#define SAMBAG_LuaDiscoObjectBase_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>

#include <sambag/disco/components/scripts/LuaSvgObject.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaDiscoObjectBase : public LuaSvgObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaSvgObject Super;
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
    SAMBAG_LUA_FTAG(addOnEnterListener, void (std::string));
	SAMBAG_LUA_FTAG(addOnExitListener, void (std::string));
    typedef LOKI_TYPELIST_2(Frx_addOnEnterListener_Tag, 
	Frx_addOnExitListener_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @brief adds a listener for the onEnter event.
	* @param the callback lua expression
	*/
	virtual void addOnEnterListener(lua_State *lua, const std::string & expr) = 0;
	/**
	* @brief adds a listener for the onExit event.
	* @param the callback lua expression
	*/
	virtual void addOnExitListener(lua_State *lua, const std::string & expr) = 0;
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

