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
#include <sambag/disco/svg/Style.hpp>
#include "Helper.hpp"
#include <sambag/disco/DiscoHelper.hpp>

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
	//-------------------------------------------------------------------------
	virtual void installClientDefaults(AComponentPtr c);
	//-------------------------------------------------------------------------
	svg::Style sNormal, sRoll, sPress, sDisabled;
	//-------------------------------------------------------------------------
	virtual Rectangle getBtnRect(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual svg::Style getStyle(typename AbstractButton::Ptr b);
	//-------------------------------------------------------------------------
	Insets margin, padding;
	//-------------------------------------------------------------------------
	Coordinate cornerRadius, iconGap;
	//-------------------------------------------------------------------------
	bool _drawRect;
private:
	//-------------------------------------------------------------------------
	typedef BasicButtonListener<ButtonModell> ButtonListener;
	//-------------------------------------------------------------------------
	ButtonListener listener;
	//-------------------------------------------------------------------------
	svg::Style createDefaultStyle() const;
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
	 * Paints the specified component appropriately for the look and feel.
	 * @param cn
	 * @param c
	 */
	virtual void drawRect(IDrawContext::Ptr cn, 
		AComponentPtr c, const Rectangle &bounds);
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
svg::Style 
BasicButtonUI<ButtonModell>::createDefaultStyle() const
{
	svg::Style style;
	style.fillColor(ColorRGBA(0,0,0,0));
	style.strokeColor(ColorRGBA(0,0,0,1.));
	style.strokeWidth(1.);
	return style;
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
Rectangle 
BasicButtonUI<ButtonModell>::getBtnRect(IDrawContext::Ptr cn, AComponentPtr c)
{
	Insets ins = c->getInsets();
	double sw = cn->getStrokeWidth();
	Rectangle bounds(ins.left() + margin.left() + sw,
		ins.top() + margin.top() + sw,
		c->getWidth() - ins.right() - margin.right() - sw, 
		c->getHeight() - ins.bottom() - margin.right() - sw
	);
	bounds.x( c->getWidth() / 2. - bounds.width() / 2. );
	bounds.y( c->getHeight() / 2. - bounds.height() / 2. );
	return bounds;
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
svg::Style BasicButtonUI<ButtonModell>::
getStyle(typename AbstractButton::Ptr b)
{
	if (!b->isEnabled()) {
		return sDisabled;
	} else if (b->isButtonRollover()) {
		if (b->isButtonPressed())
			return sPress;
		else
			return sRoll;
	} else
		return sNormal;
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::drawRect(IDrawContext::Ptr cn, 
	AComponentPtr c, const Rectangle &bounds) 
{
	cn->rect(bounds, cornerRadius);
	cn->fill();
	cn->rect(bounds, cornerRadius);
	cn->stroke();
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	typename AbstractButton::Ptr b = boost::dynamic_pointer_cast<AbstractButton>(c);
	if (!b)
		return;
	// draw rect
	Rectangle bounds = getBtnRect(cn, c);
	svg::Style style = getStyle(b);
	style.intoContext(cn);
	if (_drawRect) {
		IPattern::Ptr pat = style.fillPattern();
		if ( isGradient(pat) ) {
			alignPattern(cn, pat, bounds);
		}
		drawRect(cn, c, bounds);
	}

	cn->setFont(b->getFont());
	std::string str = sambag::com::normString(b->getText());
	cn->setFillColor(cn->getStrokeColor());

	// draw icon and text
	Helper::drawIconAndText(cn, b->getIcon(), str, bounds);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installUI(AComponentPtr c) {
	installDefaults(c);
	installClientDefaults(c); // object specific defaults
	installListener(c);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installListener(AComponentPtr c) {
	typename AbstractButton::Ptr b =
			boost::dynamic_pointer_cast<AbstractButton>(c);
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
void BasicButtonUI<ButtonModell>::installDefaults(AComponentPtr c) {
	ui::UIManager &m = ui::getUIManager();
	sNormal = sPress = sRoll = sDisabled = createDefaultStyle();
	m.getProperty("Button.normal", sNormal);
	m.getProperty("Button.pressed", sPress);
	m.getProperty("Button.rollover", sRoll);
	m.getProperty("Button.disabled", sDisabled);
	cornerRadius = 2.;
	m.getProperty("Button.cornerRadius", cornerRadius);
	iconGap = 2.;
	m.getProperty("Button.iconGap", iconGap);
	_drawRect = true;
	m.getProperty("Button.drawRect", _drawRect);

	padding = Insets(15., 15., 10., 10.);
	margin = Insets(5., 5., 5., 5.);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonUI<ButtonModell>::installClientDefaults(AComponentPtr c) {
	// consider:
	// if no client property setted getClientProperty() has no effect
	c->getClientProperty("Button.normal", sNormal);
	c->getClientProperty("Button.pressed", sPress);
	c->getClientProperty("Button.rollover", sRoll);
	c->getClientProperty("Button.disabled", sDisabled);
	c->getClientProperty("padding", padding);
	c->getClientProperty("margin", margin);
	c->getClientProperty("Button.cornerRadius", cornerRadius);
	c->getClientProperty("Button.iconGap", iconGap);
	c->getClientProperty("Button.drawRect", _drawRect);
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
    AContainer::Ptr parent = btn->getParent();
    if (!parent) {
        return;
    }
	parent->redraw();
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
			boost::dynamic_pointer_cast<AbstractButton>(c);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	// TODO: handle font style/size
	cn->setFont(b->getFont());
	Rectangle txtEx = cn->textExtends(sambag::com::normString(b->getText()));
	txtEx.height( 
		std::max( (double)cn->getCurrentFontHeight(), (double)txtEx.height() )
	);

	Dimension res(txtEx.getWidth() + padding.left() + padding.right(), 
		txtEx.getHeight() + padding.top() + padding.height());

	ISurface::Ptr icon = b->getIcon();
	if (!icon)
		return res;
	Rectangle iconBounds = Helper::calcIconBounds(icon, cn->getCurrentFontHeight());
	res.width( res.width() + iconBounds.width() + 25.5);
	return res;
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
