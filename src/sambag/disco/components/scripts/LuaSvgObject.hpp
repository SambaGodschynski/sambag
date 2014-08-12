/*
 * LuaSvgObject.hpp
 *
 *  Created on: Sun Jul 27 16:30:34 2014
 *      Author: Samba Godschysnki
 */

#ifndef SAMBAG_LuaSvgObject_H
#define SAMBAG_LuaSvgObject_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/lua/Lua.hpp>
#include "LuaSvgObjectBase.hpp"
#include <sambag/disco/IDrawable.hpp>
#include <sambag/disco/svg/graphicElements/ISceneGraph.hpp>


namespace sambag { namespace disco { namespace components {
//=============================================================================
class LuaSvgObject : public LuaSvgObjectBase {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef LuaSvgObjectBase Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<LuaSvgObject> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<LuaSvgObject> WPtr;
    //-------------------------------------------------------------------------
    typedef svg::graphicElements::ISceneGraph SceneGraph;
private:
    //-------------------------------------------------------------------------
    IDrawable::WPtr _object;
    //-------------------------------------------------------------------------
    SceneGraph::WPtr _scene;
protected:
    //-------------------------------------------------------------------------
    LuaSvgObject() {}
public:
    //-------------------------------------------------------------------------
    void setSceneGraph(SceneGraph::Ptr scene);
    //-------------------------------------------------------------------------
    void setObject(IDrawable::Ptr obj);
    //-------------------------------------------------------------------------
    virtual std::string toString(lua_State *lua) const;
    //-------------------------------------------------------------------------
    IDrawable::Ptr getObject() const;
    //-------------------------------------------------------------------------
    SceneGraph::Ptr getSceneGraph() const;
    //-------------------------------------------------------------------------
    virtual ~LuaSvgObject() {}
    ///////////////////////////////////////////////////////////////////////////
    // impl.
	virtual std::string getId(lua_State *lua);
	virtual sambag::lua::IgnoreReturn getClasses(lua_State *lua);
	virtual void setVisible(lua_State *lua, bool x);
	virtual bool isVisible(lua_State *lua);
	virtual void setStyle(lua_State *lua, const std::string & style);
	virtual std::string getStyle(lua_State *lua);
    virtual std::string calculateStyle(lua_State *lua);

    //-------------------------------------------------------------------------
    static Ptr createAndPush(lua_State *lua, IDrawable::Ptr object, SceneGraph::Ptr scene);
}; // LuaSvgObject
}}}

#endif /* SAMBAG_LuaSvgObject_H */

