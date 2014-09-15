/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoButtonBase.hpp
 *
 *  Created on: Mon Sep 15 20:57:01 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoButtonBase_H
#define SAMBAG_LuaDiscoButtonBase_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>

#include <sambag/disco/components/scripts/LuaDiscoObject.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaDiscoButtonBase : public LuaDiscoObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaDiscoObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaDiscoButtonBase> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaDiscoButtonBase> WPtr;
    //-------------------------------------------------------------------------
    
private:
protected:
    //-------------------------------------------------------------------------
    LuaDiscoButtonBase() {}
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(getText, std::string ());
	SAMBAG_LUA_FTAG(setText, void (std::string));
    typedef LOKI_TYPELIST_2(Frx_getText_Tag, 
	Frx_setText_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @return button text
	*/
	virtual std::string getText(lua_State *lua) = 0;
	/**
	* @brief set button text
	*/
	virtual void setText(lua_State *lua, const std::string & text) = 0;
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
    virtual ~LuaDiscoButtonBase() {}
private:
public:
}; // LuaDiscoButtonBase
}}}

#endif /* SAMBAG_LuaDiscoButtonBase_H */

