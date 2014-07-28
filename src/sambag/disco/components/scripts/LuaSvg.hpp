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
protected:
    //-------------------------------------------------------------------------
    LuaSvg() {}
    //-------------------------------------------------------------------------
    /**
	* @return a @see DiscoObject by svg id or nil 
	*/
	virtual sambag::lua::IgnoreReturn
    getObjectById(lua_State *lua, const std::string & id);
    //-------------------------------------------------------------------------
	/**
	* @return a sequence of @see DiscoObjects by svg class 
	*/
	virtual sambag::lua::IgnoreReturn
    getObjectsByClass(lua_State *lua, const std::string & _class);
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

