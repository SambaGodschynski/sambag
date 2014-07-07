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
void SvgKnobUI::installListeners(AComponent::Ptr c) {
    installHandleListeners();
}
//-----------------------------------------------------------------------------
void SvgKnobUI::installUI(AComponentPtr c) {
    Super::installUI(c);
   	SvgComponent::Dummy::Ptr main =
        boost::dynamic_pointer_cast<SvgComponent::Dummy>(c);
    if (!main) {
        throw std::runtime_error("SvgKnobUI: wrong component");
    }
    this->main = main;
    installModel(main);
   	SvgComponent::Dummy::Ptr handle =
        getFirstChildOfClass(".disco-knob-handle", main);
    this->handle = handle;
    getModel()->setMaximum(1);
    getModel()->setMinimum(0);
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
    SvgComponent::Ptr svg = getSvgComponent(handle);
    svg::graphicElements::SceneGraph::Ptr g = getSceneGraph(svg);
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
}}}}