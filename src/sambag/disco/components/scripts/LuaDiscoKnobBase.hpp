/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * LuaDiscoKnobBase.hpp
 *
 *  Created on: Sun Jul 27 21:27:08 2014
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
    
    typedef Loki::NullType Functions1;
    ///////////////////////////////////////////////////////////////////////////
    
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

