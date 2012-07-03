/*
 * BasicScrollbarUI.hpp
 *
 *  Created on: Tue Jul  3 10:11:59 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICSCROLLBARUI_H
#define SAMBAG_BASICSCROLLBARUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/ALayoutManager.hpp>
#include <sambag/disco/components/Scrollbar.hpp>
#include "BasicArrowButton.hpp"
#include <sambag/disco/components/ui/UIManager.hpp>
#include <sambag/disco/components/Graphics.hpp>
#include <sambag/disco/components/Timer.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicScrollbarUI.
  */
template <class ComponentModell>
class BasicScrollbarUI :
	public AComponentUI,
	public ALayoutManager
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicScrollbarUI> Ptr;
	//-------------------------------------------------------------------------
	typedef AComponentUI Super;
	//-------------------------------------------------------------------------
	typedef AScrollbar<ComponentModell> ScrollBarType;
protected:
	//-------------------------------------------------------------------------
	Timer::Ptr scrollTimer;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<BasicScrollbarUI> WPtr;
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	typename ScrollBarType::Ptr scrollbar;
	//-------------------------------------------------------------------------
	Coordinate scrollBarWidth;
	//-------------------------------------------------------------------------
	void drawTrack(IDrawContext::Ptr cn,
			AComponentPtr c, const Rectangle &trackBounds);
	//-------------------------------------------------------------------------
	void drawThumb(IDrawContext::Ptr cn,
			AComponentPtr c, const Rectangle &trackBounds);

	//-------------------------------------------------------------------------
	void layoutVScrollbar(typename ScrollBarType::Ptr sb);
	//-------------------------------------------------------------------------
	void layoutHScrollbar(typename ScrollBarType::Ptr sb);
	/**
	 * Distance between the increment button and the track. This may be a negative
	 * number. If negative, then an overlap between the button and track will occur,
	 * which is useful for shaped buttons.
	 */
	Coordinate incrGap;
	Coordinate decrGap;
	Coordinate scrollBarValue;
	Button::Ptr incrButton;
	Button::Ptr decrButton;
	Rectangle thumbRect;
	Rectangle trackRect;
	Dimension minimumThumbSize;
	Dimension maximumThumbSize;
	sambag::com::ArithmeticWrapper<bool> isDragging;
	sambag::com::ArithmeticWrapper<bool> thumbActive;
	ColorRGBA thumbHighlightColor;
	ColorRGBA thumbLightShadowColor;
	ColorRGBA thumbDarkShadowColor;
	ColorRGBA thumbColor;
	ColorRGBA trackColor;
	ColorRGBA trackHighlightColor;
	//-------------------------------------------------------------------------
	/**
	 * Returns the smallest acceptable size for the thumb.  If the scrollbar
	 * becomes so small that this size isn't available, the thumb will be
	 * hidden.
	 * <p>
	 * <b>Warning </b>: the value returned by this method should not be
	 * be modified, it's a shared static constant.
	 *
	 * @return The smallest acceptable size for the thumb.
	 * @see #getMaximumThumbSize
	 */
	const Dimension & getMinimumThumbSize() const {
		return minimumThumbSize;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the largest acceptable size for the thumb.  To create a fixed
	 * size thumb one make this method and <code>getMinimumThumbSize</code>
	 * return the same value.
	 * <p>
	 * <b>Warning </b>: the value returned by this method should not be
	 * be modified, it's a shared static constant.
	 *
	 * @return The largest acceptable size for the thumb.
	 * @see #getMinimumThumbSize
	 */
	const Dimension & getMaximumThumbSize() const {
		return maximumThumbSize;
	}
	//-------------------------------------------------------------------------
	void setThumbBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	/**
	 * Sets whether or not the mouse is currently over the thumb.
	 *
	 * @param active True indicates the thumb is currently active.
	 */
	void setThumbRollover(bool active) {
		if (thumbActive != active) {
			thumbActive = active;
			scrollbar->redraw(getThumbBounds());
		}
	}
	//-------------------------------------------------------------------------
	/**
	 * Return the current size/location of the thumb.
	 * <p>
	 * <b>Warning </b>: the value returned by this method should not be
	 * be modified, it's a reference to the actual rectangle, not a copy.
	 *
	 * @return The current size/location of the thumb.
	 * @see #setThumbBounds
	 */
	const Rectangle & getThumbBounds() const {
		return thumbRect;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the current bounds of the track, i.e. the space in between
	 * the increment and decrement buttons, less the insets.  The value
	 * returned by this method is updated each time the scrollbar is
	 * laid out (validated).
	 * <p>
	 * <b>Warning </b>: the value returned by this method should not be
	 * be modified, it's a reference to the actual rectangle, not a copy.
	 *
	 * @return the current bounds of the scrollbar track
	 * @see #layoutContainer
	 */
	const Rectangle & getTrackBounds() const {
		return trackRect;
	}
	//-------------------------------------------------------------------------
	enum TrackHighlight{
		NO_HIGHLIGHT,
		DECREASE_HIGHLIGHT,
		INCREASE_HIGHLIGHT
	};
	TrackHighlight trackHighlight;
private:
	//-------------------------------------------------------------------------
	Coordinate getValue(typename ScrollBarType::Ptr sb) {
		//return (useCachedValue) ? scrollBarValue : sb.getValue();
		return sb->getValue();
	}
	//-------------------------------------------------------------------------
	void installDefaults();
	//-------------------------------------------------------------------------
	void installComponents();
	//-------------------------------------------------------------------------
	void installListeners();
	//-------------------------------------------------------------------------
	void configureScrollBarColors();
	//-------------------------------------------------------------------------
	// delay in milli seconds
	sambag::com::ArithmeticWrapper<long, 60> scrollSpeedThrottle ;
public:
	//-------------------------------------------------------------------------
	virtual bool contains(AComponent::Ptr c, const Point2D &p);
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new BasicScrollbarUI<ComponentModell>());
		res->self = res;
		return res;
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
	/**
	 * Returns the specified component's preferred size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getPreferredSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	// LayoutManager impl.
	virtual Dimension preferredLayoutSize(AContainerPtr scrollbarContainer)
	{
		return getPreferredSize(scrollbarContainer);
	}
	//-------------------------------------------------------------------------
	virtual Dimension minimumLayoutSize(AContainerPtr scrollbarContainer) {
		return getMinimumSize(scrollbarContainer);
	}
	//-------------------------------------------------------------------------
	virtual void layoutContainer(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/////////////
	// Listener
	////////////
private:
	//-------------------------------------------------------------------------
	struct ScrollListener {
		void onScrollTimer(void *src, const Timer::Event &ev);
	};
	//-------------------------------------------------------------------------
	ScrollListener scrollListener;
public:
	//-------------------------------------------------------------------------
	void onArrowIncr(void *src, const events::ActionEvent &ev);
	//-------------------------------------------------------------------------
	void onArrowDecr(void *src, const events::ActionEvent &ev);
	//-------------------------------------------------------------------------
	void onTrack(void *src, const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void onModelChanged(void *src,const DefaultBoundedRangeModelChanged &ev);
}; // BasicScrollbarUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class M>
bool BasicScrollbarUI<M>::contains(AComponent::Ptr c, const Point2D &p) {
	return Super::contains(c, p);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::installUI(AComponentPtr c) {
	scrollbar = boost::shared_dynamic_cast<ScrollBarType>(c);
	SAMBAG_ASSERT(scrollbar);
	thumbRect = Rectangle(0, 0, 0, 0);
	trackRect = Rectangle(0, 0, 0, 0);
	installDefaults();
	installComponents();
	installListeners();

}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::configureScrollBarColors() {
	UIManager &m = getUIManager();
	m.getProperty("ScrollBar.thumbHighlight", thumbHighlightColor);
	m.getProperty("ScrollBar.thumbShadow", thumbLightShadowColor);
	m.getProperty("ScrollBar.thumbDarkShadow", thumbDarkShadowColor);
	m.getProperty("ScrollBar.thumb", thumbColor);
	m.getProperty("ScrollBar.track", trackColor);
	m.getProperty("ScrollBar.trackHighlight", trackHighlightColor);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::installDefaults() {
	UIManager::instance().getProperty("ScrollBar.width",
			scrollBarWidth);
	if (scrollBarWidth <= 0) {
		scrollBarWidth = 16;
	}
	UIManager::instance().getProperty("ScrollBar.minimumThumbSize",
			minimumThumbSize);
	UIManager::instance().getProperty("ScrollBar.maximumThumbSize",
			maximumThumbSize);

	trackHighlight = NO_HIGHLIGHT;

	if (!scrollbar->getLayout()) {
		scrollbar->setLayout(self.lock());
	}
	configureScrollBarColors();

	scrollBarValue = scrollbar->getValue();

	UIManager::instance().getProperty("ScrollBar.incrementButtonGap",
			incrGap);
	UIManager::instance().getProperty("ScrollBar.decrementButtonGap",
			decrGap);

	std::string scaleKey;
	scrollbar->getClientProperty("Component.sizeVariant", scaleKey);
	if (scaleKey.length() > 0) {
		if ("large" == scaleKey) {
			scrollBarWidth = scrollBarWidth * 1.15;
			incrGap = incrGap * 1.15;
			decrGap = decrGap * 1.15;
		} else if ("small" == scaleKey) {
			scrollBarWidth = scrollBarWidth * 0.857;
			incrGap = incrGap * 0.857;
			decrGap = decrGap * 0.714;
		} else if ("mini" == scaleKey) {
			scrollBarWidth = scrollBarWidth * 0.714;
			incrGap = incrGap * 0.714;
			decrGap = decrGap *0.714;
		}
	}
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::installComponents() {
	switch (scrollbar->getOrientation()) {
	case ScrollBarType::VERTICAL:
		incrButton = BasicArrowButton::create(BasicArrowButton::SOUTH);
		decrButton = BasicArrowButton::create(BasicArrowButton::NORTH);
		break;

	case ScrollBarType::HORIZONTAL:
		incrButton = BasicArrowButton::create(BasicArrowButton::EAST);
		decrButton = BasicArrowButton::create(BasicArrowButton::WEST);
		break;
	default:
		break;
	}
	scrollbar->add(incrButton);
	scrollbar->add(decrButton);
	// Force the children's enabled state to be updated.
	scrollbar->setEnabled(scrollbar->isEnabled());
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::installListeners() {

	if (incrButton) {
		incrButton->EventSender<events::ActionEvent>::addTrackedEventListener(
			boost::bind(&BasicScrollbarUI<M>::onArrowIncr, this, _1, _2),
			self
		);
	}
	if (decrButton) {
		decrButton->EventSender<events::ActionEvent>::addTrackedEventListener(
			boost::bind(&BasicScrollbarUI<M>::onArrowDecr, this, _1, _2),
			self
		);
	}

	//scrollListener = createScrollListener();
	scrollTimer = Timer::create(scrollSpeedThrottle);
	scrollTimer->EventSender<Timer::Event>::addTrackedEventListener(
		boost::bind(&ScrollListener::onScrollTimer, &scrollListener, _1, _2),
		self
	);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	drawTrack(cn, c, getTrackBounds());
	drawThumb(cn, c, getThumbBounds());
}
//-----------------------------------------------------------------------------
template <class M>
Dimension BasicScrollbarUI<M>::getMaximumSize(AComponentPtr c) {
	return Dimension(INT_MAX, INT_MAX);
}
//-----------------------------------------------------------------------------
template <class M>
Dimension BasicScrollbarUI<M>::getPreferredSize(AComponentPtr c) {
	return (
		scrollbar->getOrientation() == AScrollbar<M>::VERTICAL) ? Dimension(
		scrollBarWidth, 48.) : Dimension(48., scrollBarWidth
	);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::drawTrack(IDrawContext::Ptr cn,
		AComponentPtr c, const Rectangle &trackBounds)
{
	cn->setFillColor(trackColor);
	cn->rect(trackBounds);
	cn->fill();

//	if (trackHighlight == DECREASE_HIGHLIGHT) {
//		paintDecreaseHighlight( g);
//	} else if (trackHighlight == INCREASE_HIGHLIGHT) {
//		paintIncreaseHighlight( g);
//	}
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::drawThumb(IDrawContext::Ptr cn,
		AComponentPtr c, const Rectangle &thumbBounds)
{

	if (!scrollbar->isEnabled()) {
		return;
	}

	Graphics g(cn);

	Coordinate w = thumbBounds.width();
	Coordinate h = thumbBounds.height();

	g.translate(thumbBounds.x0());

	g.setStrokeColor(thumbDarkShadowColor);
	g.rect(Rectangle(0, 0, w - 1, h - 1));
	g.stroke();
	g.setFillColor(thumbColor);
	g.rect(Rectangle(0, 0, w - 1, h - 1));
	g.fill();

	g.setStrokeColor(thumbHighlightColor);
	g.drawLine(1, 1, 1, h - 2);
	g.drawLine(2, 1, w - 3, 1);

	g.setStrokeColor(thumbLightShadowColor);
	g.drawLine(2, h - 2, w - 2, h - 2);
	g.drawLine(w - 2, 1, w - 2, h - 3);

	g.translate(Point2D(-thumbBounds.x0().x(), -thumbBounds.x0().y()));
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::layoutVScrollbar(typename ScrollBarType::Ptr sb)
{
	Dimension sbSize = sb->getSize();
	Insets sbInsets = sb->getInsets();

	/*
	 * Width and left edge of the buttons and thumb.
	 */
	Coordinate itemW = sbSize.width() - (sbInsets.left() + sbInsets.right());
	Coordinate itemX = sbInsets.left();


	Coordinate decrButtonH = decrButton->getPreferredSize().height();
	Coordinate decrButtonY = sbInsets.top();

	Coordinate incrButtonH = incrButton->getPreferredSize().height();
	Coordinate incrButtonY = sbSize.height() - (sbInsets.bottom() + incrButtonH);

	/* The thumb must fit within the height left over after we
	 * subtract the preferredSize of the buttons and the insets
	 * and the gaps
	 */
	Coordinate sbInsetsH = sbInsets.top() + sbInsets.bottom();
	Coordinate sbButtonsH = decrButtonH + incrButtonH;
	Coordinate gaps = decrGap + incrGap;
	Coordinate trackH = sbSize.height() - (sbInsetsH + sbButtonsH) - gaps;

	/* Compute the height and origin of the thumb.   The case
	 * where the thumb is at the bottom edge is handled specially
	 * to avoid numerical problems in computing thumbY.  Enforce
	 * the thumbs min/max dimensions.  If the thumb doesn't
	 * fit in the track (trackH) we'll hide it later.
	 */
	Coordinate min = sb->getMinimum();
	Coordinate extent = sb->getVisibleAmount();
	Coordinate range = sb->getMaximum() - min;
	Coordinate value = getValue(sb);

	Coordinate thumbH = (range <= 0) ? getMaximumThumbSize().height() :
			(Coordinate)(trackH * (extent / range));
	thumbH = std::max(thumbH, getMinimumThumbSize().height());
	thumbH = std::min(thumbH, getMaximumThumbSize().height());

	Coordinate thumbY = incrButtonY - incrGap - thumbH;
	if (value < (sb->getMaximum() - sb->getVisibleAmount())) {
		Coordinate thumbRange = trackH - thumbH;
		thumbY = (int) (0.5f
				+ (thumbRange * ((value - min) / (range - extent))));
		thumbY = thumbY + decrButtonY + decrButtonH + decrGap;
	}

	/* If the buttons don't fit, allocate half of the available
	 * space to each and move the lower one (incrButton) down.
	 */
	Coordinate sbAvailButtonH = (sbSize.height() - sbInsetsH);
	if (sbAvailButtonH < sbButtonsH) {
		incrButtonH = decrButtonH = sbAvailButtonH / 2.;
		incrButtonY = sbSize.height() - (sbInsets.bottom() + incrButtonH);
	}
	decrButton->setBounds(itemX, decrButtonY, itemW, decrButtonH);
	incrButton->setBounds(itemX, incrButtonY, itemW, incrButtonH);

	/* Update the trackRect field.
	 */
	Coordinate itrackY = decrButtonY + decrButtonH + decrGap;
	Coordinate itrackH = incrButtonY - incrGap - itrackY;
	trackRect = Rectangle(itemX, itrackY, itemW, itrackH);

	/* If the thumb isn't going to fit, zero it's bounds.  Otherwise
	 * make sure it fits between the buttons.  Note that setting the
	 * thumbs bounds will cause a repaint.
	 */
	if (thumbH >= trackH) {
		// This is used primarily for GTK L&F, which expands the
		// thumb to fit the track when it would otherwise be hidden.
		setThumbBounds(Rectangle(itemX, itrackY, itemW, itrackH));
	} else {
		if ((thumbY + thumbH) > incrButtonY - incrGap) {
			thumbY = incrButtonY - incrGap - thumbH;
		}
		if (thumbY < (decrButtonY + decrButtonH + decrGap)) {
			thumbY = decrButtonY + decrButtonH + decrGap + 1;
		}
		setThumbBounds(Rectangle(itemX, thumbY, itemW, thumbH));
	}
}
//-------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::layoutHScrollbar(typename ScrollBarType::Ptr sb)
{
	Dimension sbSize = sb->getSize();
	Insets sbInsets = sb->getInsets();

	/* Height and top edge of the buttons and thumb.
	 */
	Coordinate itemH = sbSize.height() - (sbInsets.top() + sbInsets.bottom());
	Coordinate itemY = sbInsets.top();

	/* Nominal locations of the buttons, assuming their preferred
	 * size will fit.
	 */
	Coordinate leftButtonW = decrButton->getPreferredSize().width();
	Coordinate rightButtonW =  incrButton->getPreferredSize().width();
	Coordinate leftButtonX = sbInsets.left();
	Coordinate rightButtonX = sbSize.width() - (sbInsets.right() + rightButtonW);
	Coordinate leftGap = decrGap;
	Coordinate rightGap = incrGap;

	/* The thumb must fit within the width left over after we
	 * subtract the preferredSize of the buttons and the insets
	 * and the gaps
	 */
	Coordinate sbInsetsW = sbInsets.left() + sbInsets.right();
	Coordinate sbButtonsW = leftButtonW + rightButtonW;
	Coordinate trackW = sbSize.width() - (sbInsetsW + sbButtonsW) - (leftGap
			+ rightGap);

	/* Compute the width and origin of the thumb.  Enforce
	 * the thumbs min/max dimensions.  The case where the thumb
	 * is at the right edge is handled specially to avoid numerical
	 * problems in computing thumbX.  If the thumb doesn't
	 * fit in the track (trackH) we'll hide it later.
	 */
	Coordinate min = sb->getMinimum();
	Coordinate max = sb->getMaximum();
	Coordinate extent = sb->getVisibleAmount();
	Coordinate range = max - min;
	Coordinate value = getValue(sb);

	Coordinate thumbW = (range <= 0) ? getMaximumThumbSize().width() :
			(Coordinate) (trackW * (extent / range));
	thumbW = std::max(thumbW, getMinimumThumbSize().width());
	thumbW = std::min(thumbW, getMaximumThumbSize().width());

	Coordinate thumbX = rightButtonX - rightGap - thumbW;
	if (value < (max - sb->getVisibleAmount())) {
		float thumbRange = trackW - thumbW;
		thumbX = (0.5f + (thumbRange * ((value - min) / (range
					- extent))));
		thumbX = thumbX + leftButtonX + leftButtonW + leftGap;
	}

	/* If the buttons don't fit, allocate half of the available
	 * space to each and move the right one over.
	 */
	Coordinate sbAvailButtonW = (sbSize.width() - sbInsetsW);
	if (sbAvailButtonW < sbButtonsW) {
		rightButtonW = leftButtonW = sbAvailButtonW / 2;
		rightButtonX = sbSize.width()
				- (sbInsets.right() + rightButtonW + rightGap);
	}

	decrButton->setBounds(leftButtonX, itemY, leftButtonW,
			itemH);
	incrButton->setBounds(rightButtonX, itemY,
			rightButtonW, itemH);

	/* Update the trackRect field.
	 */
	Coordinate itrackX = leftButtonX + leftButtonW + leftGap;
	Coordinate itrackW = rightButtonX - rightGap - itrackX;
	trackRect = Rectangle(itrackX, itemY, itrackW, itemH);

	/* Make sure the thumb fits between the buttons.  Note
	 * that setting the thumbs bounds causes a repaint.
	 */
	if (thumbW >= (int) trackW) {

		setThumbBounds(Rectangle(itrackX, itemY, itrackW, itemH));

	} else {
		if (thumbX + thumbW > rightButtonX - rightGap) {
			thumbX = rightButtonX - rightGap - thumbW;
		}
		if (thumbX < leftButtonX + leftButtonW + leftGap) {
			thumbX = leftButtonX + leftButtonW + leftGap + 1;
		}
		setThumbBounds(Rectangle(thumbX, itemY, thumbW, itemH));
	}
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::layoutContainer(AContainerPtr c) {
	/* If the user is dragging the value, we'll assume that the
	 * scrollbars layout is OK modulo the thumb which is being
	 * handled by the dragging code.
	 */
	if (isDragging) {
		return;
	}

	typename ScrollBarType::Ptr scrollbar =
			boost::shared_dynamic_cast<ScrollBarType>(c);
	SAMBAG_ASSERT(scrollbar);
	switch (scrollbar->getOrientation()) {
	case ScrollBarType::VERTICAL:
		layoutVScrollbar(scrollbar);
		break;

	case ScrollBarType::HORIZONTAL:
		layoutHScrollbar(scrollbar);
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::setThumbBounds(const Rectangle &r) {
	 /* If the thumbs bounds haven't changed, we're done.
	 */
	if (thumbRect == r) {
		return;
	}

	/* Update thumbRect, and repaint the union of x,y,w,h and
	 * the old thumbRect.
	 */
	Rectangle nr = union_(thumbRect, r);
	thumbRect = r;
	//scrollbar->redraw(Rectangle(minX, minY, maxX - minX, maxY - minY));
	scrollbar->redraw(nr);

	// Once there is API to determine the mouse location this will need
	// to be changed.
	setThumbRollover(false);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::onArrowIncr(void *src,
		const events::ActionEvent &ev)
{
	std::cout<<"incr"<<std::endl;
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::onArrowDecr(void *src,
		const events::ActionEvent &ev)
{
	std::cout<<"decr"<<std::endl;
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::onTrack(void *src, const events::MouseEvent &ev)
{
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::onModelChanged(void *src,
		const DefaultBoundedRangeModelChanged &ev)
{

}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::ScrollListener::onScrollTimer(void *src,
		const Timer::Event &ev)
{

}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICSCROLLBARUI_H */
