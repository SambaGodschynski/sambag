/*
 * LuaDiscoKnob.cpp
 *
 *  Created on: Sun Jul 27 17:22:06 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoKnob.hpp"
#include <sambag/disco/components/DefaultBoundedRangeModel.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaDiscoKnob
//=============================================================================
//-----------------------------------------------------------------------------
LuaDiscoKnob::Ptr
LuaDiscoKnob::createAndPush(lua_State *lua, Component::Ptr comp)
{
    Ptr neu(new LuaDiscoKnob());
    neu->createLuaObject(lua, "sambag.disco.components.LuaDiscoKnob");
    return neu;
}
//-----------------------------------------------------------------------------
void LuaDiscoKnob::onChanged(lua_State *lua, const std::string &expr) {
    try {
        lua::executeString(lua, expr);
    } catch(const std::exception &ex) {
        SAMBAG_LOG_ERR<<"LuaDiscoKnob::onChanged failed: "<<ex.what();
    } catch (...) {
        SAMBAG_LOG_ERR<<"LuaDiscoKnob::onChanged failed: unkown error";
    }
}
//-----------------------------------------------------------------------------
float LuaDiscoKnob::getValue(lua_State *lua) {
    boost::shared_ptr<DefaultBoundedRangeModel> model =
        getComponent()->getModel<DefaultBoundedRangeModel>();
    SAMBAG_ASSERT(model);
    return model->getValue();
}
//-----------------------------------------------------------------------------
void LuaDiscoKnob::setValue(lua_State *lua, float x) {
    boost::shared_ptr<DefaultBoundedRangeModel> model =
        getComponent()->getModel<DefaultBoundedRangeModel>();
    SAMBAG_ASSERT(model);
    return model->setValue(x);
}
//-----------------------------------------------------------------------------
void LuaDiscoKnob::addListener(lua_State *lua, const std::string & expr) {
    typedef DefaultBoundedRangeModel::StateChangedEvent Event;
    boost::shared_ptr<DefaultBoundedRangeModel> model =
        getComponent()->getModel<DefaultBoundedRangeModel>();
    SAMBAG_ASSERT(model);
    model->EventSender<Event>::addTrackedEventListener(
        boost::bind(&LuaDiscoKnob::onChanged, this, lua, expr),
        shared_from_this()
    );
}
}}} // namespace(s)

