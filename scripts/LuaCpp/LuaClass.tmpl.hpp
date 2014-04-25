/*
 * THIS FILE IS AUTO CREATED BY THE LUACPP BUILD PROGRAM
 * EVERY CHANGES WILL BE OVERWRITTEN THE NEXT TIME 
 * THE THIS FILE IS GENERATED  
 *
 * $$CLASS_NAME$$.hpp
 *
 *  Created on: $$DATE$$
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_$$CLASS_NAME$$_H
#define SAMBAG_$$CLASS_NAME$$_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <loki/TypeList.h>
#include <sambag/lua/ALuaObject.hpp>

$$INCLUDE$$

$$NS$$
//=============================================================================
class $$CLASS_NAME$$ : public $$EXTENDS$$ {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef $$EXTENDS$$ Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<$$CLASS_NAME$$> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<$$CLASS_NAME$$> WPtr;
private:
protected:
    //-------------------------------------------------------------------------
    $$CLASS_NAME$$() {}
    //-------------------------------------------------------------------------
    $$F_TAGS$$
    $$F_LISTS$$
    ///////////////////////////////////////////////////////////////////////////
    $$F_IMPL$$
    //-------------------------------------------------------------------------
    /**
     * @brief field getter and setter
     */
    $$FIELDS$$
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
    virtual ~$$CLASS_NAME$$() {}
private:
public:
}; // $$CLASS_NAME$$
$$NS_END$$

#endif /* SAMBAG_$$CLASS_NAME$$_H */
