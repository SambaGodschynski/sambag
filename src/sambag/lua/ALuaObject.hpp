/*
 * LuaPObject.hpp
 *
 *  Created on: Fri Mar 14 11:47:30 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ALUAOBJECT_H
#define SAMBAG_ALUAOBJECT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include <string>
#include <loki/Typelist.h>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#define SAMBAG_LUA_FTAG(_name, fsig) struct Frx_ ## _name ## _Tag { \
    typedef boost::function<fsig> Function;                     \
    static const char * name() { return #_name; } }

namespace sambag { namespace lua {
//=============================================================================
/** 
  * @class ALuaPObject.
  * @brief lua object base class
  */
class ALuaObject :
    public boost::enable_shared_from_this<ALuaObject>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ALuaObject> Ptr;
    typedef boost::weak_ptr<ALuaObject> WPtr;
    //-------------------------------------------------------------------------
    SAMBAG_LUA_FTAG(__gc, void());
    SAMBAG_LUA_FTAG(__tostring, std::string());
    SAMBAG_LUA_FTAG(__eq, bool());
    typedef Loki::NullType Functions;
    typedef LOKI_TYPELIST_3(Frx___gc_Tag,
                            Frx___tostring_Tag,
                            Frx___eq_Tag
    ) MetaFunctions;
protected:
    //-------------------------------------------------------------------------
    /**
     * @called from meta __gc
     */
    static void __destroy(lua_State *lua, WPtr obj);
    //-------------------------------------------------------------------------
    ALuaObject();
    //-------------------------------------------------------------------------
    /**
     * @brief called from __tostring.
     */
    virtual std::string toString(lua_State *lua) const {
        return luaName;
    }
    //-------------------------------------------------------------------------
    /**
     * @brief called from __eq. Arguments a and b are still stored in stack.
     */
    virtual bool isequal(lua_State *lua) const;
    //-------------------------------------------------------------------------
    /**
     * @brief called when lua object will be removed.
     */
    virtual void __gc(lua_State *lua);
    //-------------------------------------------------------------------------
    /**
     * @brief called when lua object will be transformed into string
     */
    virtual std::string __tostring(lua_State *lua);
    //-------------------------------------------------------------------------
    /**
     * @brief creates a lua object
     */
    virtual void createLuaObject(lua_State *lua, const std::string &name);
    //-------------------------------------------------------------------------
    /**
     * @brief registeres lua object fields
     * @note called during @see createObject()
     */
    virtual void addLuaFields(lua_State *lua, int index);
protected:
private:
    //-------------------------------------------------------------------------
    std::string luaName;
public:
    //-------------------------------------------------------------------------
    virtual ~ALuaObject() {}
    //-------------------------------------------------------------------------
    std::string getLuaName() const { return luaName; }
}; // ALuaObject
}} // namespace(s)

#endif /* SAMBAG_ALUAOBJECT_H */
