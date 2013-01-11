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

#include <sambag/disco/genFormatter/GenFlowLayout.hpp>
#include <sambag/disco/genFormatter/RectangleAccess.hpp>
#include <sambag/disco/genFormatter/GenericFormatter.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
namespace {
static const sambag::com::Number GAP= 5.;
Rectangle calcIconBounds(ISurface::Ptr icon, const Coordinate &textHeight) {
	if (!icon) {
		return Rectangle(0,0,0,0);	
	}
	Rectangle res(0,0,textHeight,textHeight);
	return res;
}
void drawIcon(IDrawContext::Ptr cn,
	ISurface::Ptr icon, 
	const Rectangle &bounds,
	AComponent::Ptr c) 
{
	if (!icon) {
		return;
	}
	cn->save();
	Rectangle realBounds = icon->getSize();
	if (realBounds.width() <= 0. || realBounds.height() <= 0.)
		return;
	sambag::com::Number scaleX = bounds.width() / realBounds.width();
	sambag::com::Number scaleY = bounds.height() / realBounds.height();
	cn->translate(bounds.x0());
	cn->scale(Point2D(scaleX, scaleY));
	cn->drawSurface(icon);	
	cn->restore();
}
} // namespace(s)
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
	svg::graphicElements::Style sNormal, sRoll, sPress, sDisabled;
	//-------------------------------------------------------------------------
	virtual Rectangle getBtnRect(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual void prepareContext(IDrawContext::Ptr cn, 
		typename AbstractButton::Ptr b);
	//-------------------------------------------------------------------------
	Insets margin, padding;
	//-------------------------------------------------------------------------
	Coordinate cornerRadius, iconGap;
	//-------------------------------------------------------------------------
	bool _drawRect;
private:
	//-------------------------------------------------------------------------
	IPattern::Ptr bk, roll, press, disabled;
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
void BasicButtonUI<ButtonModell>::
prepareContext(IDrawContext::Ptr cn, typename AbstractButton::Ptr b)
{
	if (!b->isEnabled()) {
		sDisabled.intoContext(cn);
	} else if (b->isButtonRollover()) {
		if (b->isButtonPressed())
			sPress.intoContext(cn);
		else
			sRoll.intoContext(cn);
	} else
		sNormal.intoContext(cn);
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
	using namespace sambag::disco::genFormatter;
	typename AbstractButton::Ptr b = boost::shared_dynamic_cast<AbstractButton>(c);
	if (!b)
		return;
	// draw rect
	prepareContext(cn, b);
	Rectangle bounds = getBtnRect(cn, c);

	if (_drawRect) {
		drawRect(cn, c, bounds);
	}

	cn->setFont(b->getFont());
	std::string str = sambag::com::normString(b->getText());
	Rectangle txtEx = cn->textExtends(str);
	cn->setFillColor(cn->getStrokeColor());
	
	// draw icon and text
	ISurface::Ptr icon = b->getIcon();
	Rectangle iconBounds = calcIconBounds(icon, txtEx.getHeight());

	typedef GenericFormatter< Rectangle,
		RectangleAccess,
		GenFlowLayout
	> Formatter;
	Formatter form;
	if (icon) {
		form.setHgap(iconGap);
	}
	form.setAlignment(Formatter::CENTER);
	form.setX(bounds.x());
	form.setY(bounds.y() + cornerRadius);
	form.setWidth(bounds.width());
	form.setHeight(bounds.height());
	form.addComponent(&iconBounds);	
	form.addComponent(&txtEx);	
	form.layout();

	drawIcon(cn, icon, iconBounds, c);
	cn->moveTo(Point2D(
		txtEx.x(), txtEx.y() + txtEx.height()
	));
	cn->textPath(str);
	cn->fill();
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
	// if not client property setted getClientProperty() has no effect
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
	
	Dimension res(txtEx.getWidth() + padding.left() + padding.right(), 
		txtEx.getHeight() + padding.top() + padding.height());

	ISurface::Ptr icon = b->getIcon();
	if (!icon)
		return res;
	Rectangle iconBounds = calcIconBounds(icon, txtEx.getHeight());
	res.width( res.width() + iconBounds.width() + GAP*5.5);
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
