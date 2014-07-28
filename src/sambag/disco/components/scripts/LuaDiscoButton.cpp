/*
 * LuaDiscoButton.cpp
 *
 *  Created on: Sun Jul 27 17:22:23 2014
 *      Author: Samba Godschysnki
 */

#include "LuaDiscoButton.hpp"
#include <sambag/disco/svg/graphicElements/ISceneGraph.hpp>
#include <sambag/disco/svg/graphicElements/Text.hpp>
#include <sambag/disco/components/DefaultButtonModell.hpp>
#include <sambag/disco/components/events/ActionEvent.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class LuaDiscoButton
//=============================================================================
//-----------------------------------------------------------------------------
LuaDiscoButton::Ptr
LuaDiscoButton::createAndPush(lua_State *lua, Component::Ptr comp)
{
    Ptr neu(new LuaDiscoButton());
    neu->createLuaObject(lua, "sambag.disco.components.LuaDiscoButton");
    return neu;
}
//-----------------------------------------------------------------------------
IDrawable::Ptr LuaDiscoButton::getTextElement() const {
    SvgComponent::Dummy::Ptr dummy = getComponent();
    SvgComponent::Ptr svg = getSvgComponent();
    using svg::graphicElements::ISceneGraph;
    ISceneGraph::Ptr g = svg->getSceneGraph();
    ISceneGraph::Elements els;
    g->getChildrenByTagX(svg->getDrawable(dummy), "text", els);
    if (els.empty()) {
        return IDrawable::Ptr();
    }
    return els[0];
}
//-----------------------------------------------------------------------------
std::string LuaDiscoButton::getText(lua_State *lua) {
    using svg::graphicElements::Text;
    Text::Ptr txt = boost::dynamic_pointer_cast<Text>(getTextElement());
    if (!txt) {
        return "";
    }
    return txt->getText();
}
//-----------------------------------------------------------------------------
void LuaDiscoButton::setText(lua_State *lua, const std::string & text) {
    using svg::graphicElements::Text;
    Text::Ptr txt = boost::dynamic_pointer_cast<Text>(getTextElement());
    if (!txt) {
        return;
    }
    txt->setText(text);
    SvgComponent::Ptr svg = getSvgComponent();
    svg->redraw();
}
//-----------------------------------------------------------------------------
void LuaDiscoButton::onAction(lua_State *lua, const std::string &expr)
{
    try {
        lua::executeString(lua, expr);
    } catch(const std::exception &ex) {
        SAMBAG_LOG_ERR<<"LuaDiscoButton::onButton failed: "<<ex.what();
    } catch (...) {
        SAMBAG_LOG_ERR<<"LuaDiscoButton::onButton failed: unkown error";
    }
}

//-----------------------------------------------------------------------------
void LuaDiscoButton::addOnClickedListener(lua_State *lua, const std::string & expr)
{
    Component::Ptr comp = getComponent();
    boost::shared_ptr<DefaultButtonModell> model =
        comp->getModel<DefaultButtonModell>();
    SAMBAG_ASSERT(model);
    model->EventSender<events::ActionEvent>::addTrackedEventListener(
        boost::bind(&LuaDiscoButton::onAction, this, lua, expr),
        shared_from_this()
    );
}
}}} // namespace(s)

