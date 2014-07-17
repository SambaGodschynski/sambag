#include "SvgKnobUI.hpp"
#include <sambag/disco/IDrawable.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <exception>
#include <sambag/disco/components/HitStrategy.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui {
namespace {
    struct KnobHitStrategy : public HitStrategy {
        typedef boost::shared_ptr<KnobHitStrategy> Ptr;
        virtual bool operator()(AComponentPtr c, const Point2D &p);
        static Ptr get();
    private:
        static Ptr _instance;
    };
    KnobHitStrategy::Ptr KnobHitStrategy::_instance;
    KnobHitStrategy::Ptr KnobHitStrategy::get()
    {
        if (!_instance) {
            _instance = Ptr(new KnobHitStrategy());
        }
        return _instance;
    }
    bool KnobHitStrategy::operator()(AComponentPtr c, const Point2D &p)
    {
        Point2D center(c->getWidth()/2., c->getHeight()/2.);
        Coordinate x = p.x() - center.x();
        Coordinate y = p.y() - center.y();
        Coordinate radius = c->getHeight()/2. * 0.7;
        return x*x + y*y <= radius*radius;
    }
} // namespace
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
    Model::Ptr model = getModel();
    model->com::events::EventSender<StateChanged>::addTrackedEventListener(
		boost::bind(&SvgKnobUI::onKnobStateChanged, this, _2),
		shared_from_this()
	);
    rotateKnob(getModel()->getValue());
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
    main->setHitStrategy(KnobHitStrategy::get());
   	SvgComponent::Dummy::Ptr handle =
        getFirstChildOfClass(".disco-knob-handle", main);
    handle->setHitStrategy(KnobHitStrategy::get());
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
void SvgKnobUI::rotateKnob(double value) {
    try {
        SvgComponent::Dummy::Ptr handle = getHandle();
        SvgComponent::Ptr svg = getSvgComponent(handle);
        svg::graphicElements::ISceneGraph::Ptr g = getSceneGraph(svg);
        Coordinate max = getUIPropertyCached<DegreePropertyTag>(270.);
        g->setTransfomationTo(svg->getDrawable(handle),rotate2D(value*max));
        svg->redraw();
    } catch(...) {
    }
}
//-----------------------------------------------------------------------------
void SvgKnobUI::onKnobStateChanged(const StateChanged &ev) {
    rotateKnob(getModel()->getValue());
}
//-----------------------------------------------------------------------------
void SvgKnobUI::mouseEntered(const events::MouseEvent &ev) {
}
//-----------------------------------------------------------------------------
void SvgKnobUI::mouseExited(const events::MouseEvent &ev) {
}
}}}}