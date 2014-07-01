#include "SvgKnobUI.hpp"
#include <sambag/disco/IDrawable.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <exception>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
// class SvgKnobUI : public AComponentUI
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
        g->getChildrenByClass(x, ".disco-knob-handle", l);
        if (l.empty()) {
            throw std::runtime_error("SvgKnobUI: no disco-knob-handle class element found.");
        }
        x = *(l.begin());
        SvgComponent::Dummy::Ptr res = svg->getDummy(x);
        SAMBAG_ASSERT(res);
        return res;
    }
    
}
//-----------------------------------------------------------------------------
void SvgKnobUI::installHandleListeners() {
    getHandle()->com::events::EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&SvgKnobUI::onMouse, this, _2),
		shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void SvgKnobUI::installModelListeners() {
    getModel()->com::events::EventSender<StateChanged>::addTrackedEventListener(
		boost::bind(&SvgKnobUI::onKnobStateChanged, this, _2),
		shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void SvgKnobUI::installPropertyListeners() {
    using namespace com::events;
    getMain()->EventSender<PropertyChanged>::addTrackedEventListener(
        boost::bind(&SvgKnobUI::onPropertyChanged, this, _2),
        shared_from_this()
    );
}
//-----------------------------------------------------------------------------
void SvgKnobUI::installListeners(AComponent::Ptr c) {
    installModelListeners();
    installHandleListeners();
    installPropertyListeners();
}
//-----------------------------------------------------------------------------
void SvgKnobUI::installUI(AComponentPtr c) {
   	SvgComponent::Dummy::Ptr main =
        boost::dynamic_pointer_cast<SvgComponent::Dummy>(c);
    if (!main) {
        throw std::runtime_error("SvgKnobUI: wrong component");
    }
    this->main = main;
    SvgComponent::Ptr svg = _getSvgComponent(main);
    svg::graphicElements::SceneGraph::Ptr g = _getSceneGraph(svg);
   	SvgComponent::Dummy::Ptr handle = __getHandle(svg, g, main);
    this->handle = handle;
    DefaultBoundedRangeModel::Ptr model =
        main->getModel<DefaultBoundedRangeModel>();
    if (!model) {
        throw std::runtime_error("SvgKnobUI: wrong model");
    }
    this->model = model;
    model->setMaximum(1);
    model->setMinimum(0);
	main->setMouseWheelEventsEnabled(true);
    installListeners(c);
}
//-----------------------------------------------------------------------------
void SvgKnobUI::onMouse(const events::MouseEvent &ev) {
    enum { Filter = 
		events::MouseEvent::DISCO_MOUSE_PRESSED |
		events::MouseEvent::DISCO_MOUSE_DRAGGED |
		events::MouseEvent::DISCO_MOUSE_WHEEL   |
        events::MouseEvent::DISCO_MOUSE_ENTERED |
        events::MouseEvent::DISCO_MOUSE_EXITED
	};
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
namespace {
	SAMBAG_PROPERTY_TAG(RangePropertyTag, "Knob.range");
	SAMBAG_PROPERTY_TAG(ModePropertyTag, "Knob.mode");
	SAMBAG_PROPERTY_TAG(WheelPropertyTag, "Knob.wheelIncrement");
    SAMBAG_PROPERTY_TAG(DegreePropertyTag, "SvgKnob.maxDegree");
} // namespace
void SvgKnobUI::mousePressed(const events::MouseEvent &ev) {
	if (ev.getButtons() != events::MouseEvent::DISCO_BTN1) {
		return;
	}
	firstPoint = ev.getLocation();
	lastPoint = Point2D(-1., -1.);
	startValue = oldValue;

	entryState = getModel()->getValue();
	range = getUIPropertyCached<RangePropertyTag>(200.);

	Coordinate vmax = getModel()->getMaximum(), vmin = getModel()->getMinimum();
	coef = (vmax - vmin) / range;
	
	lastPoint = firstPoint;
}
//-----------------------------------------------------------------------------
void SvgKnobUI::mouseDragged(const events::MouseEvent &ev) {
	if (ev.getButtons() != events::MouseEvent::DISCO_BTN1) {
		return;
	}
	Coordinate value = getModel()->getValue();
	const Point2D &where = ev.getLocation();
	if (where != lastPoint) {
		lastPoint = where;
        Coordinate diff = (firstPoint.x() - where.x()) + (where.y() - firstPoint.y());
        value = entryState - diff * coef;
	}
	getModel()->setValue(value);
}
//-----------------------------------------------------------------------------
void SvgKnobUI::mouseWheelRotated(const events::MouseEvent &ev) {
	double incr = getUIPropertyCached<WheelPropertyTag>((double)0.1);
	if (ev.getWheelRotation() > 0) {
		incr*=-1.;
	}
	getModel()->setValue( getModel()->getValue() + incr );
}
//-----------------------------------------------------------------------------
void SvgKnobUI::onKnobStateChanged(const StateChanged &ev) {
    SvgComponent::Dummy::Ptr handle = getHandle();
    SvgComponent::Ptr svg = _getSvgComponent(handle);
    svg::graphicElements::SceneGraph::Ptr g = _getSceneGraph(svg);
    Coordinate max = getUIPropertyCached<DegreePropertyTag>(270.);
    g->setTransfomationTo(svg->getDrawable(handle),rotate2D(getModel()->getValue()*max));
    svg->redraw();
}
//-----------------------------------------------------------------------------
void SvgKnobUI::mouseEntered(const events::MouseEvent &ev) {
}
//-----------------------------------------------------------------------------
void SvgKnobUI::mouseExited(const events::MouseEvent &ev) {
}
//-----------------------------------------------------------------------------
void SvgKnobUI::onPropertyChanged(const com::events::PropertyChanged &ev) {
    if (ev.getPropertyName() == SvgComponent::Dummy::PROPERTY_MODEL) {
        ArbitraryType::Ptr _new;
        ev.getNewValue(_new);
        if (!_new) {
            throw std::runtime_error("SvgKnobUI model property null");
        }
        DefaultBoundedRangeModel::Ptr newModel;
        com::get(_new, newModel);
        if (!newModel) {
            return;
        }
        this->model = newModel;
        installModelListeners();
    }
}
}}}}