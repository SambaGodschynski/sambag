/*
 * LuaSvg.hpp
 *
 *  Created on: Sun Jul 27 16:30:00 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaSvg_H
#define SAMBAG_LuaSvg_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "LuaSvgBase.hpp"
#include <sambag/disco/IDrawable.hpp>

namespace sambag { namespace disco { namespace components { 
class SvgComponent;
typedef boost::shared_ptr<SvgComponent> SvgComponentPtr;
//=============================================================================
class LuaSvg : public LuaSvgBase {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaSvgBase Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaSvg> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaSvg> WPtr;
    //-------------------------------------------------------------------------
private:
    //-------------------------------------------------------------------------
    SvgComponentPtr svg;
    //-------------------------------------------------------------------------
    /**
     * @brief tries to create a LuaDiscoObject or, if fails, a LuaSvgObject.
     * If none of them succeed it pushes nil.
     */
    void push(lua_State *lua, IDrawable::Ptr x);
protected:
    //-------------------------------------------------------------------------
    LuaSvg() {}
    ///////////////////////////////////////////////////////////////////////////
	virtual sambag::lua::IgnoreReturn
    getObjectById(lua_State *lua, const std::string & id);
	virtual sambag::lua::IgnoreReturn
    getObjectsByClass(lua_State *lua, const std::string & _class);
	virtual sambag::lua::IgnoreReturn
    select(lua_State *lua, const std::string & sel);
    virtual void redraw(lua_State *lua);
public:
    //-------------------------------------------------------------------------
    virtual ~LuaSvg() {}
    //-------------------------------------------------------------------------
    static Ptr createAndPush(lua_State *lua, SvgComponentPtr svg);
private:
public:
}; // LuaSvg
}}}

#endif /* SAMBAG_LuaSvg_H */

