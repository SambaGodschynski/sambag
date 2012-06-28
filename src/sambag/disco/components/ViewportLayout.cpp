/*
 * ViewportLayout.cpp
 *
 *  Created on: Sat Jun 23 19:30:18 2012
 *      Author: Johannes Unger
 */

#include "ViewportLayout.hpp"
#include "ui/ALookAndFeel.hpp"
#include "Viewport.hpp"
#include "IScrollable.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ViewportLayout
//=============================================================================
//-----------------------------------------------------------------------------
ViewportLayout::Ptr ViewportLayout::__sharedInstance;
//-----------------------------------------------------------------------------
ViewportLayout::Ptr ViewportLayout::getSharedInstance() {
	if (!__sharedInstance)
		__sharedInstance = create();
	return __sharedInstance;
}
//-----------------------------------------------------------------------------
ViewportLayout::ViewportLayout() {
}
//-----------------------------------------------------------------------------
void ViewportLayout::addLayoutComponent(AComponentPtr c) {
}
//-----------------------------------------------------------------------------
void ViewportLayout::layoutContainer(AContainerPtr parent) {
	Viewport::Ptr vp = boost::shared_dynamic_cast<Viewport>(parent);
	AComponentPtr view = vp->getView();
	if (!view) {
		return;
	}
	IScrollable::Ptr scrollable=boost::shared_dynamic_cast<IScrollable>(view);

	/* All of the dimensions below are in view coordinates, except
	 * vpSize which we're converting.
	 */

	Dimension viewPrefSize = view->getPreferredSize();
	Dimension vpSize = vp->getSize();
	Dimension extentSize = vp->toViewCoordinates(vpSize);
	Dimension viewSize = viewPrefSize;

	if (scrollable) {
		if (scrollable->getScrollableTracksViewportWidth()) {
			viewSize.width(vpSize.width());
		}
		if (scrollable->getScrollableTracksViewportHeight()) {
			viewSize.height(vpSize.height());
		}
	}

	Point2D viewPosition = vp->getViewPosition();

	/* If the new viewport size would leave empty space to the
	 * right of the view, right justify the view or left justify
	 * the view when the width of the view is smaller than the
	 * container.
	 */
	if (scrollable || vp->getParent() ) {
		if ((viewPosition.x() + extentSize.width()) > viewSize.width()) {
			viewPosition.x(std::max((Number)0, viewSize.width() - extentSize.width()));
		}
	} else {
		if (extentSize.width() > viewSize.width()) {
			viewPosition.x(viewSize.width() - extentSize.width());
		} else {
			viewPosition.x(
					std::max(
							(Number)0,
							std::min(viewSize.width() - extentSize.width(),
									(Number)viewPosition.x())));
		}
	}

	/* If the new viewport size would leave empty space below the
	 * view, bottom justify the view or top justify the view when
	 * the height of the view is smaller than the container.
	 */
	if ((viewPosition.y() + extentSize.height()) > viewSize.height()) {
		viewPosition.y( std::max((Number)0,
			viewSize.height() - extentSize.height()));
	}

	/* If we haven't been advised about how the viewports size
	 * should change wrt to the viewport, i.e. if the view isn't
	 * an instance of Scrollable, then adjust the views size as follows.
	 *
	 * If the origin of the view is showing and the viewport is
	 * bigger than the views preferred size, then make the view
	 * the same size as the viewport.
	 */
	if (!scrollable) {
		if ((viewPosition.x() == 0) && (vpSize.width() > viewPrefSize.width()))
		{
			viewSize.width(vpSize.width());
		}
		if ((viewPosition.y() == 0) && (vpSize.height() > viewPrefSize.height()))
		{
			viewSize.height(vpSize.height());
		}
	}
	vp->setViewPosition(viewPosition);
	vp->setViewSize(viewSize);
}
//-----------------------------------------------------------------------------
Dimension ViewportLayout::minimumLayoutSize(AContainerPtr parent) {
	return Dimension(4, 4);
}
//-----------------------------------------------------------------------------
Dimension ViewportLayout::preferredLayoutSize(AContainerPtr parent) {
	Viewport::Ptr vp = boost::shared_dynamic_cast<Viewport>(parent);
	SAMBAG_ASSERT(vp);
	AComponentPtr view = vp->getView();
	IScrollable::Ptr scrollable=boost::shared_dynamic_cast<IScrollable>(view);
	if (!view) {
		return Dimension(0, 0);
	} else if (scrollable) {
		return scrollable->getPreferredScrollableViewportSize();
	} else {
		return view->getPreferredSize();
	}
}
//-----------------------------------------------------------------------------
void ViewportLayout::removeLayoutComponent(AComponentPtr c) {

}
}}} // namespace(s)
