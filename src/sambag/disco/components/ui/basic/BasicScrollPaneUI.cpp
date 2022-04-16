/*
 * BasicScrollPaneUI.cpp
 *
 *  Created on: Sun Jul 15 11:39:23 2012
 *      Author: Johannes Unger
 */

#include "BasicScrollPaneUI.hpp"
#include <sambag/disco/components/ScrollPane.hpp>
#include <sambag/disco/components/Viewport.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
//  Class BasicScrollPaneUI
//=============================================================================
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::syncScrollPaneWithViewport() {
	ScrollPanePtr scrollpane = getScrollPane();
	ViewportPtr viewport = scrollpane->getViewport();
	ScrollPane::AScrollbarPtr vsb = scrollpane->getVerticalScrollBar();
	ScrollPane::AScrollbarPtr hsb = scrollpane->getHorizontalScrollBar();
	ViewportPtr rowHead = scrollpane->getRowHeader();
	ViewportPtr colHead = scrollpane->getColumnHeader();

	if (viewport) {
		Dimension extentSize = viewport->getExtentSize();
		Dimension viewSize = viewport->getViewSize();
		Point2D viewPosition = viewport->getViewPosition();

		if (vsb) {
			Coordinate extent = extentSize.height();
			Coordinate max = viewSize.height();
			Coordinate value =
				std::max((Number)0, std::min((Number)viewPosition.y(), max - extent));
			vsb->setValues(value, extent, 0, max);
		}

		if (hsb) {
			Coordinate extent = extentSize.width();
			Coordinate max = viewSize.width();
			Coordinate value;

			value =
				std::max((Number)0, std::min((Number)viewPosition.x(), max - extent));

			oldExtent = extent;
			hsb->setValues(value, extent, 0, max);
		}

		if (rowHead) {
			Point2D p = rowHead->getViewPosition();
			p.y (viewport->getViewPosition().y());
			p.x(0);
			rowHead->setViewPosition(p);
		}

		if (colHead) {
			Point2D p = colHead->getViewPosition();
			p.x(viewport->getViewPosition().x());
			p.y(0);
			colHead->setViewPosition(p);
		}
	}
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::installUI(AComponentPtr c) {
	Super::installUI(c);
	ScrollPane::Ptr scrollpane  = boost::dynamic_pointer_cast<ScrollPane>(c);
	SAMBAG_ASSERT(scrollpane);
	_scrollpane = scrollpane;
	ScrollPane::AScrollbarPtr vsb = scrollpane->getVerticalScrollBar();
	if (vsb) {
		vsb->EventSender<ScrollPane::ScrollbarModel::StateChangedEvent>::
			addTrackedEventListener(
			boost::bind(&BasicScrollPaneUI::vsbStateChanged, this, _1, _2),
			self
		);
	}
	ScrollPane::AScrollbarPtr hsb = scrollpane->getHorizontalScrollBar();
	if (hsb) {
		hsb->EventSender<ScrollPane::ScrollbarModel::StateChangedEvent>::
			addTrackedEventListener(
			boost::bind(&BasicScrollPaneUI::hsbStateChanged, this, _1, _2),
			self
		);
	}
	ViewportPtr viewport = scrollpane->getViewport();
	if (viewport) {
		viewport->EventSender<Viewport::StateChangedEvent>::
			addTrackedEventListener(
			boost::bind(&BasicScrollPaneUI::viewportStateChanged, this, _1, _2),
			self
		);
	}
	scrollpane->EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&BasicScrollPaneUI::onMouse, this, _1, _2),
		self
	);
	scrollpane->setMouseWheelEventsEnabled(true);
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::vsbStateChanged(void *src,
		const ScrollPane::ScrollbarModel::StateChangedEvent &ev)
{
	const ScrollPane::ScrollbarModel &model = ev.getSrc();
	ScrollPanePtr scrollpane = getScrollPane();
	Point2D p = scrollpane->getViewport()->getViewPosition();
	p.y(model.getValue());
	scrollpane->getViewport()->setViewPosition(p);
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::hsbStateChanged(void *src,
		const ScrollPane::ScrollbarModel::StateChangedEvent &ev)
{
	const ScrollPane::ScrollbarModel &model = ev.getSrc();
	ScrollPanePtr scrollpane = getScrollPane();
	Point2D p = scrollpane->getViewport()->getViewPosition();
	Coordinate value = model.getValue();
	p.x(value);
	scrollpane->getViewport()->setViewPosition(p);
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::viewportStateChanged(void *src,
		const Viewport::StateChangedEvent &ev)
{
	syncScrollPaneWithViewport();
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::onMouse(void *, const events::MouseEvent &ev) {
	enum { Filter = events::MouseEvent::DISCO_MOUSE_WHEEL }; 
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::mouseWheelRotated(const events::MouseEvent &ev) {
	ScrollPane::Ptr scrollpane = _scrollpane.lock();
	SAMBAG_ASSERT(scrollpane);
	if (!scrollpane) {
		return;
	}
	ScrollPane::AScrollbarPtr sb = scrollpane->getVerticalScrollBar();
	if (!sb) {
		return;
	}
	Coordinate c = sb->getBlockIncrement() * ev.getWheelRotation();
	sb->setValue( sb->getValue() + c );
}
}}}}} // namespace(s)
