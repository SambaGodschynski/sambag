#include "SvgButtonUI.hpp"
#include <sambag/disco/IDrawable.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <exception>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
// class SvgButtonUI : public AComponentUI
//=============================================================================
//-----------------------------------------------------------------------------
namespace {
    SvgComponent::Ptr _getSvgComponent(AComponent::Ptr c) {
        SvgComponent::Ptr svg = c->getFirstContainer<SvgComponent>();
        SAMBAG_ASSERT(svg);
        return svg;
    }
    svg::graphicElements::SceneGraph::Ptr _getSceneGraph(SvgComponent::Ptr svg) {
        SAMBAG_ASSERT(svg->getSvgObject());
        svg::graphicElements::SceneGraph::Ptr g =
            svg->getSvgObject()->getRelatedSceneGraph();
        SAMBAG_ASSERT(g);
        return g;
    }
    SvgComponent::Dummy::Ptr __getHandle(SvgComponent::Ptr svg,
        svg::graphicElements::SceneGraph::Ptr g,
        SvgComponent::Dummy::Ptr c)
    {
        IDrawable::Ptr x = svg->getDrawable(c);
        SAMBAG_ASSERT(x);
        std::list<IDrawable::Ptr> l;
        g->getChildrenByClass(x, ".disco-button-handle", l);
        if (l.empty()) {
            throw std::runtime_error("SvgButtonUI: no disco-button-handle class element found.");
        }
        x = *(l.begin());
        SvgComponent::Dummy::Ptr res = svg->getDummy(x);
        SAMBAG_ASSERT(res);
        return res;
    }
    
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installHandleListeners() {
    getHandle()->com::events::EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onMouse, this, _2),
		shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installModelListeners() {
    getModel()->com::events::EventSender<StateChanged>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onStateChanged, this, _2),
		shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installPropertyListeners() {
    using namespace com::events;
    getMain()->EventSender<PropertyChanged>::addTrackedEventListener(
        boost::bind(&SvgButtonUI::onPropertyChanged, this, _2),
        shared_from_this()
    );
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installListeners(AComponent::Ptr c) {
    installModelListeners();
    installHandleListeners();
    installPropertyListeners();
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installUI(AComponentPtr c) {
   	SvgComponent::Dummy::Ptr main =
        boost::dynamic_pointer_cast<SvgComponent::Dummy>(c);
    if (!main) {
        throw std::runtime_error("SvgButtonUI: wrong component");
    }
    this->main = main;
    SvgComponent::Ptr svg = _getSvgComponent(main);
    svg::graphicElements::SceneGraph::Ptr g = _getSceneGraph(svg);
   	SvgComponent::Dummy::Ptr handle = __getHandle(svg, g, main);
    this->handle = handle;
    DefaultButtonModell::Ptr model =
        main->getModel<DefaultButtonModell>();
    if (!model) {
        throw std::runtime_error("SvgButtonUI: wrong model");
    }
    this->model = model;
    installListeners(c);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::onMouse(const events::MouseEvent &ev) {
    enum { Filter = 
		events::MouseEvent::DISCO_MOUSE_PRESSED |
        events::MouseEvent::DISCO_MOUSE_RELEASED|
        events::MouseEvent::DISCO_MOUSE_ENTERED |
        events::MouseEvent::DISCO_MOUSE_EXITED
	};
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mousePressed(const events::MouseEvent &ev) {
    std::cout<<ev.toString()<<std::endl;
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mouseReleased(const events::MouseEvent &ev) {
    std::cout<<ev.toString()<<std::endl;
}
//-----------------------------------------------------------------------------
void SvgButtonUI::onStateChanged(const StateChanged &ev) {
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mouseEntered(const events::MouseEvent &ev) {
    std::cout<<ev.toString()<<std::endl;
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mouseExited(const events::MouseEvent &ev) {
    std::cout<<ev.toString()<<std::endl;
}
//-----------------------------------------------------------------------------
void SvgButtonUI::onPropertyChanged(const com::events::PropertyChanged &ev) {
    if (ev.getPropertyName() == SvgComponent::Dummy::PROPERTY_MODEL) {
        ArbitraryType::Ptr _new;
        ev.getNewValue(_new);
        if (!_new) {
            throw std::runtime_error("SvgButtonUI model property null");
        }
        DefaultButtonModell::Ptr newModel;
        com::get(_new, newModel);
        if (!newModel) {
            return;
        }
        this->model = newModel;
        installModelListeners();
    }
}
}}}}