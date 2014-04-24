/*
 * $$CLASS_NAME$$.hpp
 *
 *  Created on: $$DATE$$
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_$$CLASS_NAME$$_H
#define SAMBAG_$$CLASS_NAME$$_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/ALuaObject.hpp>

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
    $$F_TAGS$$
    $$F_LISTS$$
    ///////////////////////////////////////////////////////////////////////////
    $$F_IMPL$$
    //-------------------------------------------------------------------------
    /**
     * @brief field getter and setter
     * @note works only when lua class table is on stack
     */
    $$FIELDS$$
    //-------------------------------------------------------------------------
    /**
     * @override 
     */	
    virtual void addLuaFields(lua_State *lua, int index);
    ///////////////////////////////////////////////////////////////////////////
    // Lua call implements request, return true if a script implements
    // the xxx function
    $$LUA_CALL_IMPL$$
    ///////////////////////////////////////////////////////////////////////////
    // there are two ways to call a LC functions: you can use the regular
    // xxx() call where the argument and return treatment is handled
    // by LuaHelper. Or you use raw_xxx(narg, nret) where you have to handle return and
    // args for your self (e.g. when using unsupported LuaHelper types).
    $$LUA_CALL_FS$$
public:
    //-------------------------------------------------------------------------
    $$CLASS_NAME$$();
    //-------------------------------------------------------------------------
    /**
     * @brief called when lua object will be removed.
     */
    virtual void __lua_gc(lua_State *lua);
public:
    //-------------------------------------------------------------------------
    virtual $$CLASS_NAME$$() {}
private:
public:
}; // $$CLASS_NAME$$
$$NS_END$$

#endif /* SAMBAG_$$CLASS_NAME$$_H */
