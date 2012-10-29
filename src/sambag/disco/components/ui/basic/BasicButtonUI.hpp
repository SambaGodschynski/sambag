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
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/ISurface.hpp>
#include <sambag/disco/IPattern.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <sambag/disco/svg/graphicElements/Style.hpp>

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
	typedef IButtonUI Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicButtonUI<ButtonModell> > Ptr;
	//-------------------------------------------------------------------------
	typedef AButton<ButtonModell> AbstractButton;
protected:
	//-------------------------------------------------------------------------
	BasicButtonUI();
	//-------------------------------------------------------------------------
	virtual void installListener(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual void installDefaults(AComponentPtr c);
private:
	//-------------------------------------------------------------------------
	IPattern::Ptr bk, roll, press, disabled;
	//-------------------------------------------------------------------------
	svg::graphicElements::Style sNormal, sRoll, sPress, sDisabled;
	//-------------------------------------------------------------------------
	typedef BasicButtonListener<ButtonModell> ButtonListener;
	//-------------------------------------------------------------------------
	ButtonListener listener;
	//-------------------------------------------------------------------------
	IPattern::Ptr 
		createPattern(const ColorRGBA &start, const ColorRGBA &end) const;
	//-------------------------------------------------------------------------
	svg::graphicElements::Style createDefaultStyle() const;
public:
	//-------------------------------------------------------------------------
	virtual bool contains(AComponent::Ptr c, const Point2D &p);
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
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's maximum size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getMaximumSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual Dimension getMinimumSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's preferred size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getPreferredSize(AComponentPtr c);
}; // BasicButtonUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class ButtonModell>
BasicButtonUI<ButtonModell>::BasicButtonUI() {
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
svg::graphicElements::Style 
BasicButtonUI<ButtonModell>::createDefaultStyle() const
{
	svg::graphicElements::Style style;
	style.fillColor(ColorRGBA(0,0,0,0));
	style.strokeColor(ColorRGBA(0,0,0,1.));
	style.strokeWidth(1.5);
	return style;
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	typename AbstractButton::Ptr b = boost::shared_dynamic_cast<AbstractButton>(c);
	if (!b)
		return;
	// consider clipping
	cn->translate(Point2D(2,2));
	cn->scale(Point2D(0.8, 0.8));
	
	if (!b->isEnabled()) {
		sDisabled.intoContext(cn);
	} else if (b->isButtonRollover()) {
		if (b->isButtonPressed())
			sPress.intoContext(cn);
		else
			sRoll.intoContext(cn);
	} else
		sNormal.intoContext(cn);
	cn->rect(Rectangle(0,0,c->getWidth(), c->getHeight()), 5);
	cn->fill();
	cn->rect(Rectangle(0,0,c->getWidth(), c->getHeight()), 5);
	cn->stroke();
	cn->setFont(b->getFont());
	std::string str = sambag::com::normString(b->getText());
	Rectangle txt = cn->textExtends(str);
	cn->setFillColor(cn->getStrokeColor());
	cn->translate( Point2D( 10,
			c->getHeight() / 2.0 + txt.getHeight() / 2.0
	));
	cn->textPath(str);
	cn->fill();
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installUI(AComponentPtr c) {
	installDefaults(c);
	installListener(c);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installListener(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(c);
	BOOST_ASSERT(b);
	b->EventSender<events::MouseEvent>::addTrackedEventListener(
			boost::bind(&ButtonListener::onMouseEvent, &listener, _1, _2),
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
IPattern::Ptr 
BasicButtonUI<ButtonModell>::
createPattern(const ColorRGBA &start, const ColorRGBA &end) const
{
	ILinearPattern::Ptr lp =
		getDiscoFactory()->createLinearPattern(Point2D(), Point2D(0.,10.));
	lp->addColorStop(start, 0);
	lp->addColorStop(start, 0.5);
	lp->addColorStop(end, .51);
	lp->addColorStop(end, 1.0);
	return lp;
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installDefaults(AComponentPtr c) {
	ui::UIManager &m = ui::getUIManager();
	sNormal = sPress = sRoll = sDisabled = createDefaultStyle();
	m.getProperty("Button.normal", sNormal);
	/*bk = createPattern(
		ColorRGBA(1.,1.,1.,1.),
	    end
	);*/
	m.getProperty("Button.pressed", sPress);
	m.getProperty("Button.rollover", sRoll);
	m.getProperty("Button.disabled", sDisabled);
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
//------------------------------------------------------------------------------
template <class ButtonModell>
Dimension BasicButtonUI<ButtonModell>::getMaximumSize(AComponentPtr c) {
	return getPreferredSize(c);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
Dimension BasicButtonUI<ButtonModell>::getMinimumSize(AComponentPtr c) {
	return getPreferredSize(c);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
Dimension BasicButtonUI<ButtonModell>::getPreferredSize(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(c);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	// TODO: handle font style/size
	cn->setFont(b->getFont());
	Rectangle txtEx = cn->textExtends(sambag::com::normString(b->getText()));
	return Dimension(txtEx.getWidth() + 35, txtEx.getHeight() + 15);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
bool BasicButtonUI<ButtonModell>::contains(AComponent::Ptr c,
		const Point2D &p)
{
	return Super::contains(c, p);
}

///////////////////////////////////////////////////////////////////////////////
}}}}} // namespace(s)

#endif /* SAMBAG_BASICBUTTONUI_H */
