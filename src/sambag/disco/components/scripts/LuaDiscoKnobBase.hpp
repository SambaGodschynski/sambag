/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoKnobBase.hpp
 *
 *  Created on: Fri Aug  1 13:37:08 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaDiscoKnobBase_H
#define SAMBAG_LuaDiscoKnobBase_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/Typelist.h>
#include <sambag/lua/ALuaObject.hpp>

#include <sambag/disco/components/scripts/LuaDiscoObject.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
class LuaDiscoKnobBase : public LuaDiscoObject {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaDiscoObject Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaDiscoKnobBase> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaDiscoKnobBase> WPtr;
    //-------------------------------------------------------------------------
    
private:
protected:
    //-------------------------------------------------------------------------
    LuaDiscoKnobBase() {}
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(getValue, float ());
	SAMBAG_LUA_FTAG(setValue, void (float));
	SAMBAG_LUA_FTAG(addListener, void (std::string));
    typedef LOKI_TYPELIST_3(Frx_getValue_Tag, 
	Frx_setValue_Tag, 
	Frx_addListener_Tag) Functions1;

	
    ///////////////////////////////////////////////////////////////////////////
    /**
	* @return the knob value
	*/
	virtual float getValue(lua_State *lua) = 0;
	/**
	* @brief set the knob value
	* @param the new value
	*/
	virtual void setValue(lua_State *lua, float x) = 0;
	/**
	* @brief adds a listener to the knob
	* @param a valid lua expression which will be
	*  	executed when knob value was changed
	*/
	virtual void addListener(lua_State *lua, const std::string & expr) = 0;
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
    virtual ~LuaDiscoKnobBase() {}
private:
public:
}; // LuaDiscoKnobBase
}}}

#endif /* SAMBAG_LuaDiscoKnobBase_H */

