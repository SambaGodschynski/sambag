/*
 * BasicCheckBoxUI.hpp
 *
 *  Created on: Mon Dec  3 12:58:21 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICCHECKBOXUI_H
#define SAMBAG_BASICCHECKBOXUI_H

#include <boost/shared_ptr.hpp>
#include "BasicButtonUI.hpp"
#include "BasicCheckBoxListener.hpp"

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicCheckBoxUI.
  */
template <class _ButtonModel>
class BasicCheckBoxUI : public BasicButtonUI<_ButtonModel> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _ButtonModel ButtonModel; 
	//-------------------------------------------------------------------------
	typedef BasicCheckBoxUI<ButtonModel> Class; 
	//-------------------------------------------------------------------------
	typedef BasicButtonUI<_ButtonModel> Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
protected:
	//-------------------------------------------------------------------------
	BasicCheckBoxUI(){}
	//-------------------------------------------------------------------------
	virtual void installListener(AComponentPtr c);
	//-------------------------------------------------------------------------
	void installDefaults(AComponentPtr c);
	//-------------------------------------------------------------------------
	Rectangle box;
	//-------------------------------------------------------------------------
	virtual void drawBox(IDrawContext::Ptr cn, 
		const Rectangle &bounds, bool isSelected, bool isRollOver);
private:
	//-------------------------------------------------------------------------
	typedef BasicCheckBoxListener<ButtonModel> MouseListener;
	//-------------------------------------------------------------------------
	MouseListener listener;
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new Class());
	}
	//-------------------------------------------------------------------------
	virtual Dimension getPreferredSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
}; // BasicCheckBoxUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicCheckBoxUI<ButtonModell>::drawBox(IDrawContext::Ptr cn,
	const Rectangle &bounds, bool isSelected, bool isRollOver) 
{
	if (isRollOver) {
		cn->rect(bounds);
		ColorRGBA fill = cn->getFillColor();
		fill.setA(0.5);
		cn->setFillColor(fill);
		cn->fill();
	}

	cn->rect(bounds);
	cn->stroke();

	if (!isSelected)
		return;
	Rectangle cross = bounds;
	cross.inset(-7., -7.);
	Point2D x0 = cross.x0();
	Point2D x1 = cross.x1();
	cn->moveTo(x0);
	cn->lineTo(x1);
	cn->moveTo( Point2D(x0.x(), x1.y()) );
	cn->lineTo( Point2D(x1.x(), x0.y()) );
	cn->stroke();
}	
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicCheckBoxUI<ButtonModell>::installDefaults(AComponentPtr c) {
	Super::installDefaults(c);
	box = Rectangle(0,0,15,15);
	c->setOpaque(true);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicCheckBoxUI<ButtonModell>::installListener(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(c);
	BOOST_ASSERT(b);
	b->EventSender<events::MouseEvent>::addTrackedEventListener(
			boost::bind(&MouseListener::onMouseEvent, &listener, _1, _2),
			b
	);
	b->EventSender<typename ButtonModell::StateChangedEvent>::
	addTrackedEventListener(
			boost::bind(&Super::onButtonStateChanged,
					this, _1, _2),
			b
	);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicCheckBoxUI<ButtonModell>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	typename AbstractButton::Ptr b = boost::shared_dynamic_cast<AbstractButton>(c);
	if (!b)
		return;
	sNormal.intoContext(cn);
	// draw box
	drawBox(cn, 
		Rectangle(5., c->getHeight() / 2.0 - box.height() / 2.0, box.width(), box.height()),
		b->isButtonSelected(),
		b->isButtonRollover()
	);
	// drawLabel
	std::string str = sambag::com::normString(b->getText());
	Rectangle txt = cn->textExtends(str);
	cn->setFillColor(cn->getStrokeColor());
	cn->moveTo(Point2D(box.width()+10., 
		c->getHeight() / 2.0 + txt.getHeight() / 2.0));
	cn->textPath(str);
	cn->fill();
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
Dimension BasicCheckBoxUI<ButtonModell>::getPreferredSize(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(c);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	// TODO: handle font style/size
	cn->setFont(b->getFont());
	Rectangle txtEx = cn->textExtends(sambag::com::normString(b->getText()));
	return Dimension(txtEx.getWidth() + box.width() + 10., 
		std::max(txtEx.getHeight() + 15., box.height() + 5.));
}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICCHECKBOXUI_H */
