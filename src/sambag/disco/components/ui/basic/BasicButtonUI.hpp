/*
 * BasicButtonUI.hpp
 *
 *  Created on: Tue Jun  5 06:38:01 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICBUTTONUI_H
#define SAMBAG_BASICBUTTONUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/IButtonUI.hpp>
#include <sambag/disco/components/AButton.hpp>
#include "BasicButtonListener.hpp"
#include <sambag/disco/svg/HtmlColors.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {

//=============================================================================
/** 
  * @class BasicButtonUI.
  */
template <class ButtonModell>
class BasicButtonUI : public IButtonUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicButtonUI<ButtonModell> > Ptr;
	//-------------------------------------------------------------------------
	typedef AButton<ButtonModell> AbstractButton;
protected:
private:
public:
	//-------------------------------------------------------------------------
	void onButtonStateChanged(void *src, const
			typename AbstractButton::StateChangedEvent &ev);
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new BasicButtonUI<ButtonModell>());
	}
	//-------------------------------------------------------------------------
	/**
	 * Paints the specified component appropriately for the look and feel.
	 * @param cn
	 * @param c
	 */
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
}; // BasicButtonUI
//=============================================================================
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	typename AbstractButton::Ptr b = boost::shared_dynamic_cast<AbstractButton>(c);
	if (!b)
		return;
	if (b->isRollover()) {
		if (b->isPressed())
			cn->setFillColor(svg::HtmlColors::getColor("red"));
		else
			cn->setFillColor(svg::HtmlColors::getColor("lightblue"));
	} else
		cn->setFillColor(b->getBackground());
	cn->setStrokeColor(c->getForeground());
	cn->rect(Rectangle(0,0,c->getWidth(), c->getHeight()), 5);
	cn->fill();
	cn->rect(Rectangle(0,0,c->getWidth(), c->getHeight()), 5);
	cn->setStrokeWidth(2);
	cn->stroke();
	cn->setFontSize(15);
	cn->setFillColor(c->getForeground());
	std::string str = b->getText();
	Rectangle txt = cn->textExtends(str);
	cn->translate( Point2D( c->getWidth() / 2.0 - txt.getWidth() / 2.0,
			c->getHeight() / 2.0 + txt.getHeight() / 2.0
	));
	cn->textPath(str);
	cn->fill();
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installUI(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(c);
	BOOST_ASSERT(b);
	typedef BasicButtonListener<ButtonModell> ButtonListener;
	b->EventSender<events::MouseEvent>::addTrackedEventListener(
			boost::bind(&ButtonListener::onMouseEvent, _1, _2),
			b
	);
	b->EventSender<typename ButtonModell::StateChangedEvent>::
	addTrackedEventListener(
			boost::bind(&BasicButtonUI<ButtonModell>::onButtonStateChanged,
					this, _1, _2),
			b
	);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::onButtonStateChanged(void *src, const
			typename AbstractButton::StateChangedEvent &ev)
{
	const AbstractButton *_btn =
			dynamic_cast<const AbstractButton*>(&(ev.getSrc()));
	BOOST_ASSERT(_btn);
	typename AbstractButton::Ptr btn = _btn->getPtr();
	btn->redraw();
}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICBUTTONUI_H */
