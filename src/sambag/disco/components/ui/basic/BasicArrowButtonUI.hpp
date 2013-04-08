/*
 * BasicArrowButtonUI.hpp
 *
 *  Created on: Tue Dec  4 18:02:03 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICARROWBUTTONUI_H
#define SAMBAG_BASICARROWBUTTONUI_H

#include <boost/shared_ptr.hpp>
#include "BasicButtonUI.hpp"
#include "BasicArrowButton.hpp"
namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicArrowButtonUI.
  */
template <class _ButtonModell>
class BasicArrowButtonUI : public BasicButtonUI<_ButtonModell> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicArrowButtonUI> Ptr;
	//-------------------------------------------------------------------------
	typedef _ButtonModell ButtonModell;
	//-------------------------------------------------------------------------
	typedef BasicButtonUI<_ButtonModell> Super;
	//-------------------------------------------------------------------------
	typedef BasicArrowButtonUI<ButtonModell> Class;
	//-------------------------------------------------------------------------
	typedef BasicArrowButton::Direction Direction;
protected:
	//-------------------------------------------------------------------------
	virtual void installDefaults(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual void arrowPath(IDrawContext::Ptr cn, 
		Direction dir, const Rectangle &bounds);
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new Class());
	}
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual Dimension getPreferredSize(AComponentPtr c);
}; // BasicArrowButtonUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class BM>
void BasicArrowButtonUI<BM>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	BasicArrowButton::Ptr b = boost::shared_dynamic_cast<BasicArrowButton>(c);
	if (!b)
		return;
	Rectangle bounds = Super::getBtnRect(cn, c);
	svg::graphicElements::Style style = Super::getStyle(b);
	style.intoContext(cn);
	cn->setStrokeWidth(1.);
	cn->rect(bounds);
	cn->fill();
	
	bounds.inset(-7., -7.);
	arrowPath(cn, b->getDirection(), bounds);
	ColorRGBA col =  cn->getStrokeColor();
	cn->setFillColor( col.setA(0.5) );
	cn->fill();
	arrowPath(cn, b->getDirection(), bounds);
	cn->stroke();
}
//-----------------------------------------------------------------------------
template <class BM>
Dimension BasicArrowButtonUI<BM>::getPreferredSize(AComponentPtr c) {
	return Super::getPreferredSize(c);
}
//-----------------------------------------------------------------------------
template <class BM>
void BasicArrowButtonUI<BM>::installDefaults(AComponentPtr c) {
	Super::installDefaults(c);
}
//-----------------------------------------------------------------------------
template <class BM>
void BasicArrowButtonUI<BM>::
arrowPath(IDrawContext::Ptr cn, Direction dir, const Rectangle &bounds)
{
	
	
	Point2D r(bounds.x0());
	Point2D s(bounds.x1().x(), bounds.y());
	Point2D t(bounds.x1());
	Point2D u(bounds.x(), bounds.x1().y());
	Point2D c;

	Point2D *a = NULL;
	Point2D *b = NULL;

	switch (dir) {
		case BasicArrowButton::NORTH :
			a = &t;
			b = &u;
			c = Point2D(bounds.x() + bounds.width()/2., bounds.y());
			break;
		case BasicArrowButton::SOUTH :
			a = &r;
			b = &s;
			c = Point2D(bounds.x() + bounds.width()/2., bounds.x1().y());
			break;
		case BasicArrowButton::EAST :
			a = &r;
			b = &u;
			c = Point2D(bounds.x1().x(), bounds.y() + bounds.height()/2.);
			break;
		case BasicArrowButton::WEST :
			a = &s;
			b = &t;
			c = Point2D(bounds.x(), bounds.y() + bounds.height()/2.);
			break;
	}
	
	if(!a || !b) {
		return;
	}

	cn->moveTo(*a);
	cn->lineTo(*b);
	cn->lineTo(c);
	cn->lineTo(*a);
}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICARROWBUTTONUI_H */
