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
void BasicScrollPaneUI::installUI(AComponentPtr c) {
	Super::installUI(c);
	ScrollPane::Ptr pane  = boost::shared_dynamic_cast<ScrollPane>(c);
	SAMBAG_ASSERT(pane);
	ScrollPane::AScrollbarPtr vsb = pane->getVerticalScrollBar();
	vsb->EventSender<ScrollPane::ScrollbarModel::StateChangedEvent>::
		addTrackedEventListener(
		boost::bind(&BasicScrollPaneUI::vsbStateChanged, this, _1, _2, pane),
		self
	);
	ScrollPane::AScrollbarPtr hsb = pane->getHorizontalScrollBar();
	hsb->EventSender<ScrollPane::ScrollbarModel::StateChangedEvent>::
		addTrackedEventListener(
		boost::bind(&BasicScrollPaneUI::hsbStateChanged, this, _1, _2, pane),
		self
	);
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::vsbStateChanged(void *src,
		const ScrollPane::ScrollbarModel::StateChangedEvent &ev,
		ScrollPanePtr pane)
{
	const ScrollPane::ScrollbarModel &model = ev.getSrc();
	Point2D p = pane->getViewport()->getViewPosition();
	p.y(model.getValue());
	pane->getViewport()->setViewPosition(p);
}
//-----------------------------------------------------------------------------
void BasicScrollPaneUI::hsbStateChanged(void *src,
		const ScrollPane::ScrollbarModel::StateChangedEvent &ev,
		ScrollPanePtr pane)
{
	const ScrollPane::ScrollbarModel &model = ev.getSrc();
	Point2D p = pane->getViewport()->getViewPosition();
	Coordinate value = model.getValue();
	p.x(value);
	pane->getViewport()->setViewPosition(p);
}

}}}}} // namespace(s)
