/*
 * BasicMenuItemUI.hpp
 *
 *  Created on: Mon Jun 11 06:52:26 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICMENUITEMUI_H
#define SAMBAG_BASICMENUITEMUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/IButtonUI.hpp>
#include <sambag/disco/components/AButton.hpp>
#include "BasicMenuItemListener.hpp"
#include <sambag/disco/svg/HtmlColors.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/ISurface.hpp>
#include <sambag/disco/IPattern.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {

//=============================================================================
/** 
  * @class BasicMenuItemUI.
  */
template <class ButtonModell>
class BasicMenuItemUI : public IButtonUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicMenuItemUI<ButtonModell> > Ptr;
	//-------------------------------------------------------------------------
	typedef AButton<ButtonModell> AbstractButton;
protected:
	//-------------------------------------------------------------------------
	BasicMenuItemUI();
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
		return Ptr(new BasicMenuItemUI<ButtonModell>());
	}
	//-------------------------------------------------------------------------
	/**
	 * Paints the specified component appropriately for the look and feel.
	 * @param cn
	 * @param c
	 */
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's preferred size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getPreferredSize(AComponentPtr c);
}; // BasicMenuItemUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class ButtonModell>
BasicMenuItemUI<ButtonModell>::BasicMenuItemUI() {

}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicMenuItemUI<ButtonModell>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	typename AbstractButton::Ptr b = boost::dynamic_pointer_cast<AbstractButton>(c);
	if (!b)
		return;
	if (b->isButtonRollover()) {
		if (b->isButtonPressed())
			cn->setFillColor(svg::HtmlColors::getColor("slategrey"));
		else
			cn->setFillColor(svg::HtmlColors::getColor("lightblue"));
	} else
		cn->setFillColor(svg::HtmlColors::getColor("aliceblue"));
	cn->setStrokeColor(c->getForeground());
	cn->rect(Rectangle(0,0,c->getWidth(), c->getHeight()));
	cn->fill();
//	cn->rect(Rectangle(0,0,c->getWidth(), c->getHeight()));
//	cn->setStrokeWidth(1);
//	cn->stroke();
	cn->setFont(b->getFont());
	cn->setFillColor(c->getForeground());
	std::string str = b->getText();
	Rectangle txt = cn->textExtends(str);
	cn->translate( Point2D( 10,
			c->getHeight() / 2.0 + txt.getHeight() / 2.0
	));
	cn->textPath(str);
	cn->fill();
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicMenuItemUI<ButtonModell>::installUI(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::dynamic_pointer_cast<AbstractButton>(c);
	BOOST_ASSERT(b);
	typedef BasicMenuItemListener<ButtonModell> ButtonListener;
	b->EventSender<events::MouseEvent>::addTrackedEventListener(
			boost::bind(&ButtonListener::onMouseEvent, _1, _2),
			b
	);
	b->EventSender<typename ButtonModell::StateChangedEvent>::
	addTrackedEventListener(
			boost::bind(&BasicMenuItemUI<ButtonModell>::onButtonStateChanged,
					this, _1, _2),
			b
	);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicMenuItemUI<ButtonModell>::onButtonStateChanged(void *src, const
			typename AbstractButton::StateChangedEvent &ev)
{
	const AbstractButton *_btn =
			dynamic_cast<const AbstractButton*>(&(ev.getSrc()));
	BOOST_ASSERT(_btn);
	typename AbstractButton::Ptr btn = _btn->getPtr();
	btn->redraw();
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
Dimension BasicMenuItemUI<ButtonModell>::getPreferredSize(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::dynamic_pointer_cast<AbstractButton>(c);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	// TODO: handle font style/size
	cn->setFont(b->getFont());
	Rectangle txtEx = cn->textExtends(b->getText());
	return Dimension(txtEx.getWidth() + 35, txtEx.getHeight() + 15);
}


///////////////////////////////////////////////////////////////////////////////
}}}}} // namespace(s)

#endif /* SAMBAG_BASICMENUITEMUI_H */
