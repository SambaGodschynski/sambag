/*
 * LuaModelObject.hpp
 *
 *  Created on: Fri Mar 14 11:47:35 2014
 *      Author: Johannes Unger
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
	$$FIELDS$$
	//-------------------------------------------------------------------------
	/**
	 * @override 
	 */	
	virtual void addLuaFields(lua_State *lua, int index);
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
