/*
 * BasicKnobUI.hpp
 *
 *  Created on: Wed Aug 29 20:16:24 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICKNOBUI_H
#define SAMBAG_BASICKNOBUI_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/AKnob.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <math.h>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
namespace sdce = sambag::disco::components::events;

//=============================================================================
/** 
  * @class BasicKnobUI.
  */
template <class ComponentModel>
class BasicKnobUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef BasicKnobUI<ComponentModel> Class;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
	//-------------------------------------------------------------------------
	typedef AKnob<ComponentModel> KnobType;
	//-------------------------------------------------------------------------
	typedef typename ComponentModel::StateChangedEvent StateChanged;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<KnobType> KnobTypePtr;
		//-------------------------------------------------------------------------
	typedef boost::weak_ptr<KnobType> KnobTypeWPtr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Class> WPtr;
protected:
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	KnobTypeWPtr knob;
	//-------------------------------------------------------------------------
	BasicKnobUI() {}
private:
	//-------------------------------------------------------------------------
	Point2D firstPoint;
	Point2D lastPoint;
	Coordinate startValue;
	Coordinate oldValue;
	sambag::com::ArithmeticWrapper<bool> modeLinear;
	Coordinate entryState;
	Coordinate range;
	Coordinate coef;
	Coordinate radius;
	Coordinate startAngle;
	Coordinate rangeAngle;
	Coordinate halfAngle;
	Coordinate aCoef;
	Coordinate bCoef;
	//-------------------------------------------------------------------------
	Point2D getPivot(AComponentPtr c) {
		Coordinate w = c->getWidth(), h = c->getHeight();
		return Point2D(w/2., h/2.);
	}
	//-------------------------------------------------------------------------
	Coordinate valueFromPoint(const Point2D &p) const;
	//-------------------------------------------------------------------------
	void compute();
public:
	//-------------------------------------------------------------------------
	KnobTypePtr getKnob() const {
		return knob.lock();
	}
	//-------------------------------------------------------------------------
	virtual void onKnobStateChanged(void *src, const typename StateChanged &ev) 
	{
		getKnob()->redraw();
	}
	//-------------------------------------------------------------------------
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	void onMouse(void *src, const sdce::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mousePressed(const sdce::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseDragged(const sdce::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void installDefaults(AComponentPtr c);
	//-------------------------------------------------------------------------
	void installListener(AComponentPtr c);
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new Class());
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	virtual Dimension getPreferredSize(AComponentPtr c) {
		return Dimension(25, 25);
	}
	//-------------------------------------------------------------------------
	/**
	 * Paints the specified component appropriately for the look and feel.
	 * @param cn
	 * @param c
	 */
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
}; // BasicKnobUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::installUI(AComponentPtr c) {
	knob = boost::shared_dynamic_cast<KnobType>(c);
	installDefaults(c);
	installListener(c);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	cn->translate(getPivot(getKnob()));
	cn->arc(Point2D(0, 0), getKnob()->getWidth() / 2.5);
	cn->setFillColor(getKnob()->getForeground());
	cn->stroke();
	
	cn->setFillColor(ColorRGBA(0));
	cn->translate(Point2D(-5., 2.5));
	cn->setFontSize(11.);
	cn->textPath(sambag::com::toString(getKnob()->getValue()));
	cn->fill();
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::onMouse(void *src, const sdce::MouseEvent &ev) {
	enum { Filter = 
		sdce::MouseEvent::DISCO_MOUSE_PRESSED |
		sdce::MouseEvent::DISCO_MOUSE_DRAGGED
	};
	sdce::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::mousePressed(const sdce::MouseEvent &ev) {
	if (ev.getButtons() != sdce::MouseEvent::DISCO_BTN1) {
		return;
	}
	UIManager &m = getUIManager();
	firstPoint = ev.getLocation();
	lastPoint = Point2D(-1., -1.);
	startValue = oldValue;

	modeLinear = false;
	entryState = getKnob()->getValue();
	range = 200.;
	m.getProperty("Knob.range", range);

	Coordinate vmax = getKnob()->getMaximum(), vmin = getKnob()->getMinimum();
	coef = (vmax - vmin) / range;

	std::string mode = "linear";
	m.getProperty("Knob.mode", mode);
	modeLinear = mode == "linear";

	
	if (modeLinear) {
		lastPoint = firstPoint;
	}
	else {
		//CPoint where2 (where);
		//where2.offset (-size.left, -size.top);
		startValue = valueFromPoint(firstPoint);
	}
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::mouseDragged(const sdce::MouseEvent &ev) {
	if (ev.getButtons() != sdce::MouseEvent::DISCO_BTN1) {
		return;
	}
	Coordinate vmax = getKnob()->getMaximum(), vmin = getKnob()->getMinimum();
	Coordinate middle = (vmax - vmin) * 0.5;
	Coordinate value = getKnob()->getValue();
	const Point2D &where = ev.getLocation();
	if (where != lastPoint) {
		lastPoint = where;
		if (modeLinear) {
			Coordinate diff = (firstPoint.x() - where.x()) + (where.y() - firstPoint.y());
			value = entryState - diff * coef;
		} // if (modeLinear)
		else {
			//where.offset (-size.left, -size.top);
			value = valueFromPoint (where);
			if (startValue - value > middle)
				value = vmax;
			else if (value - startValue > middle)
				value = vmin;
			else
				startValue = value;
		} // else
	}
	getKnob()->setValue(value);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::installDefaults(AComponentPtr c) {
	radius = c->getWidth() / 2.f;
	rangeAngle = 1.f;
	startAngle = 5. * M_PI / 4.;
	rangeAngle = -3. * M_PI / 2.;
	compute();
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::installListener(AComponentPtr c) {
	c->EventSender<sdce::MouseEvent>::addTrackedEventListener(
		boost::bind(&Class::onMouse, this, _1, _2),
		getPtr()
	);
	typename getKnob()->EventSender<StateChanged>::addTrackedEventListener(
		boost::bind(&Class::onKnobStateChanged, this, _1, _2),
		getPtr()
	);
}
//-----------------------------------------------------------------------------
template <class M>
Coordinate BasicKnobUI<M>::valueFromPoint(const Point2D &p) const {
	Coordinate vmax = getKnob()->getMaximum(), vmin = getKnob()->getMinimum();
	const Coordinate _2PI = 2. * M_PI;
	Coordinate v;
	Coordinate alpha = (Coordinate) atan2(radius - p.x(), p.y() - radius);
	if (alpha < 0.f)
		alpha += _2PI;

	Coordinate alpha2 = alpha - startAngle;
	if (rangeAngle < 0.) {
		alpha2 -= rangeAngle;
		Coordinate alpha3 = alpha2;
		if (alpha3 < 0.f)
			alpha3 += _2PI;
		else if (alpha3 > _2PI)
			alpha3 -= _2PI;
		if (alpha3 > halfAngle - rangeAngle)
			v = vmax;
		else if (alpha3 > -rangeAngle)
			v = vmin;
		else
		{
			if (alpha2 > halfAngle - rangeAngle)
				alpha2 -= _2PI;
			else if (alpha2 < -halfAngle)
				alpha2 += _2PI;
			v = aCoef * alpha2 + vmax;
		}
	}
	else
	{
		Coordinate alpha3 = alpha2;
		if (alpha3 < 0.f)
			alpha3 += _2PI;
		else if (alpha3 > _2PI)
			alpha3 -= _2PI;
		if (alpha3 > rangeAngle + halfAngle)
			v = vmin;
		else if (alpha3 > rangeAngle)
			v = vmax;
		else
		{
			if (alpha2 > rangeAngle + halfAngle)
				alpha2 -= _2PI;
			else if (alpha2 < -halfAngle)
				alpha2 += _2PI;
			v = aCoef * alpha2 + vmin;
		}
	}
	return v;
}
//-----------------------------------------------------------------------------
template <class M>
void BasicKnobUI<M>::compute() {
	Coordinate vmax = getKnob()->getMaximum(), vmin = getKnob()->getMinimum();
	aCoef = (vmax - vmin) / rangeAngle;
	bCoef = vmin - aCoef * startAngle;
	halfAngle = ((float)M_PI - fabsf (rangeAngle)) * 0.5f;
}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICKNOBUI_H */
