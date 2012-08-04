/*
 * ScrollPaneLayout.cpp
 *
 *  Created on: Fri Jul  6 09:48:21 2012
 *      Author: Johannes Unger
 */

#include "ScrollPaneLayout.hpp"
#include "Viewport.hpp"
#include "ScrollPane.hpp"
#include "Scrollbar.hpp"
#include "IScrollable.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include "IBorder.hpp"
#include "ui/UIManager.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ScrollPaneLayout
//=============================================================================
//-----------------------------------------------------------------------------
AComponentPtr ScrollPaneLayout::addSingletonComponent(
		AComponentPtr oldC, AComponentPtr newC)
{
	if (oldC && oldC != newC) {
		oldC->getParent()->remove(oldC);
	}
	return newC;
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::adjustForVSB(bool wantsVSB,
		Rectangle &available, Rectangle &vsbR,
		const Insets &vpbInsets)
{
	Coordinate oldWidth = vsbR.width();
	if (wantsVSB) {
		Coordinate vsbWidth = std::max((Coordinate)0,
				std::min(vsb->getPreferredSize().width(), available.width()));

		available.width( available.width() -  vsbWidth);
		vsbR.x0().x(available.x0().x() + available.width() + vpbInsets.right());
		vsbR.width(vsbWidth);
	} else {
		available.width( available.width() + oldWidth);
	}
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::adjustForHSB(bool wantsHSB, Rectangle &available,
		Rectangle &hsbR, const Insets &vpbInsets)
{
	Coordinate oldHeight = hsbR.height();
	if (wantsHSB) {
		Coordinate hsbHeight = std::max((Coordinate)0,
				std::min(available.height(), hsb->getPreferredSize().height()));

		available.height( available.height() -  hsbHeight);
		hsbR.x0().y(available.x0().y() + available.height() + vpbInsets.bottom());
		hsbR.height(hsbHeight);
	} else {
		available.height( available.height() +  oldHeight);
	}
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::addLayoutComponent(const AComponentSharedOrWeak &c,
	ArbitraryType::Ptr constraint)
{
	ScrollPane::Area location;
	com::get(constraint, location);
	switch(location) {
	case ScrollPane::VIEWPORT:
		viewport = boost::shared_dynamic_cast<Viewport>(
			addSingletonComponent(viewport, c));
		break;
	case ScrollPane::VERTICAL_SCROLLBAR:
		vsb = boost::shared_dynamic_cast<Scrollbar>(
			addSingletonComponent(vsb, c));
		break;
	case ScrollPane::HORIZONTAL_SCROLLBAR:
		hsb = boost::shared_dynamic_cast<Scrollbar>(
			addSingletonComponent(hsb, c));
		break;
	case ScrollPane::ROW_HEADER:
		rowHead = boost::shared_dynamic_cast<Viewport>(
			addSingletonComponent(rowHead, c));
		break;
	case ScrollPane::COLUMN_HEADER:
		colHead = boost::shared_dynamic_cast<Viewport>(
				addSingletonComponent(colHead, c));
		break;
	case ScrollPane::LOWER_LEFT_CORNER:
		lowerLeft = addSingletonComponent(lowerLeft, c);
		break;
	case ScrollPane::LOWER_RIGHT_CORNER:
		lowerRight = addSingletonComponent(lowerRight, c);
		break;
	case ScrollPane::UPPER_LEFT_CORNER:
		upperLeft = addSingletonComponent(upperLeft, c);
		break;
	case ScrollPane::UPPER_RIGHT_CORNER:
		upperRight = addSingletonComponent(upperRight, c);
		break;
	default:
		SAMBAG_THROW(com::exceptions::IllegalArgumentException,
				"invalid layout key");
		break;
	}
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPaneLayout::getColumnHeader() const {
	return colHead;
}
//-----------------------------------------------------------------------------
AComponentPtr ScrollPaneLayout::getCorner(ScrollPane::Area loc) const {
	switch (loc) {
	case ScrollPane::LOWER_LEFT_CORNER:
		return lowerLeft;
	case ScrollPane::LOWER_RIGHT_CORNER:
		return lowerRight;
	case ScrollPane::UPPER_LEFT_CORNER:
		return upperLeft;
	case ScrollPane::UPPER_RIGHT_CORNER:
		return upperRight;
	default:
		return AComponentPtr();
	}
}
//-----------------------------------------------------------------------------
ScrollPaneLayout::AScrollbarPtr
ScrollPaneLayout::getHorizontalScrollBar() const
{
	 return hsb;
}
//-----------------------------------------------------------------------------
ScrollPaneLayout::HorizontalScrollbarPolicy ScrollPaneLayout::
	getHorizontalScrollBarPolicy() const
{
	return hsbPolicy;
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPaneLayout::getRowHeader() const {
	return rowHead;
}
//-----------------------------------------------------------------------------
ScrollPaneLayout::AScrollbarPtr
ScrollPaneLayout::getVerticalScrollBar() const
{
	return vsb;
}
//-----------------------------------------------------------------------------
ScrollPaneLayout::VerticalScrollbarPolicy ScrollPaneLayout::
	getVerticalScrollBarPolicy() const
{
	return vsbPolicy;
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPaneLayout::getViewport() const {
	 return viewport;
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::layoutContainer(AContainerPtr parent) {
	/* Sync the (now obsolete) policy fields with the
	 * ScrollPane.
	 */
	ScrollPane::Ptr scrollPane = boost::shared_dynamic_cast<ScrollPane>(parent);
	vsbPolicy = scrollPane->getVerticalScrollBarPolicy();
	hsbPolicy = scrollPane->getHorizontalScrollBarPolicy();

	Rectangle availR = scrollPane->getBounds();
	availR.x0().x(0); availR.x0().y(0);

	Insets insets = parent->getInsets();
	availR.x0().x( insets.left() );
	availR.x0().y( insets.top() );
	availR.width( availR.width() - (insets.left() + insets.right() ));
	availR.height( availR.height() - (insets.top() + insets.bottom() ));

	/* If there's a visible column header remove the space it
	 * needs from the top of availR.  The column header is treated
	 * as if it were fixed height, arbitrary width.
	 */

	Rectangle colHeadR(0, availR.x0().y(), 0, 0);

	if ((colHead) && (colHead->isVisible())) {
		Coordinate colHeadHeight = std::min(availR.height(),
				colHead->getPreferredSize().height());
		colHeadR.height(colHeadHeight);
		availR.x0().y( availR.x0().y() + colHeadHeight);
		availR.height( availR.height() - colHeadHeight);
	}

	/* If there's a visible row header remove the space it needs
	 * from the left or right of availR.  The row header is treated
	 * as if it were fixed width, arbitrary height.
	 */

	Rectangle rowHeadR(0, 0, 0, 0);

	if ((rowHead) && (rowHead->isVisible())) {
		Coordinate rowHeadWidth = std::min(availR.width(),
				rowHead->getPreferredSize().width());
		rowHeadR.width(rowHeadWidth);
		availR.width( availR.width() - rowHeadWidth);
		rowHeadR.x0().x(availR.x0().x());
		availR.x0().x(availR.x0().x() + rowHeadWidth);
	}

	/* If there's a ScrollPane.viewportBorder, remove the
	 * space it occupies for availR.
	 */

	IBorder::Ptr viewportBorder = scrollPane->getViewportBorder();
	Insets vpbInsets;
	if (viewportBorder) {
		vpbInsets = viewportBorder->getBorderInsets(parent);
		availR.x0().x(availR.x0().x() + vpbInsets.left());
		availR.x0().y(availR.x0().y() + vpbInsets.top());
		availR.width(availR.width() - vpbInsets.left() + vpbInsets.right());
		availR.height(availR.height() - vpbInsets.top() + vpbInsets.bottom());
	} else {
		vpbInsets = Insets(0, 0, 0, 0);
	}

	/* At this point availR is the space available for the viewport
	 * and scrollbars. rowHeadR is correct except for its height and y
	 * and colHeadR is correct except for its width and x.  Once we're
	 * through computing the dimensions  of these three parts we can
	 * go back and set the dimensions of rowHeadR.height, rowHeadR.y,
	 * colHeadR.width, colHeadR.x and the bounds for the corners.
	 *
	 * We'll decide about putting up scrollbars by comparing the
	 * viewport views preferred size with the viewports extent
	 * size (generally just its size).  Using the preferredSize is
	 * reasonable because layout proceeds top down - so we expect
	 * the viewport to be laid out next.  And we assume that the
	 * viewports layout manager will give the view it's preferred
	 * size.  One exception to this is when the view implements
	 * Scrollable and Scrollable.getViewTracksViewport{Width,Height}
	 * methods return true.  If the view is tracking the viewports
	 * width we don't bother with a horizontal scrollbar, similarly
	 * if view.getViewTracksViewport(Height) is true we don't bother
	 * with a vertical scrollbar.
	 */

	AComponentPtr view = (viewport) ? viewport->getView() : AComponentPtr();
	Dimension viewPrefSize = (view) ? view->getPreferredSize()
			: Dimension(0, 0);

	Dimension extentSize = (viewport) ? viewport->toViewCoordinates(
			availR.getSize()) : Dimension(0, 0);

	bool viewTracksViewportWidth = false;
	bool viewTracksViewportHeight = false;
	bool isEmpty = (availR.width() < 0 || availR.height() < 0);
	IScrollable::Ptr sv = boost::shared_dynamic_cast<IScrollable>(view);
	// Don't bother checking the Scrollable methods if there is no room
	// for the viewport, we aren't going to show any scrollbars in this
	// case anyway.
	if (!isEmpty && sv) {
		viewTracksViewportWidth = sv->getScrollableTracksViewportWidth();
		viewTracksViewportHeight = sv->getScrollableTracksViewportHeight();
	} else {
		sv=IScrollable::Ptr();
	}

	/* If there's a vertical scrollbar and we need one, allocate
	 * space for it (we'll make it visible later). A vertical
	 * scrollbar is considered to be fixed width, arbitrary height.
	 */

	Rectangle vsbR(0, availR.x0().y() - vpbInsets.top(), 0, 0);

	bool vsbNeeded;
	if (isEmpty) {
		vsbNeeded = false;
	} else if (vsbPolicy == ScrollPane::VERTICAL_SCROLLBAR_ALWAYS) {
		vsbNeeded = true;
	} else if (vsbPolicy == ScrollPane::VERTICAL_SCROLLBAR_NEVER) {
		vsbNeeded = false;
	} else { // vsbPolicy == VERTICAL_SCROLLBAR_AS_NEEDED
		vsbNeeded = !viewTracksViewportHeight && (viewPrefSize.height()
				> extentSize.height());
	}

	if (vsb && vsbNeeded) {
		adjustForVSB(true, availR, vsbR, vpbInsets);
		extentSize = viewport->toViewCoordinates(availR.getSize());
	}

	/* If there's a horizontal scrollbar and we need one, allocate
	 * space for it (we'll make it visible later). A horizontal
	 * scrollbar is considered to be fixed height, arbitrary width.
	 */

	Rectangle hsbR(availR.x0().x() - vpbInsets.left(), 0, 0, 0);
	bool hsbNeeded;
	if (isEmpty) {
		hsbNeeded = false;
	} else if (hsbPolicy == ScrollPane::HORIZONTAL_SCROLLBAR_ALWAYS) {
		hsbNeeded = true;
	} else if (hsbPolicy == ScrollPane::HORIZONTAL_SCROLLBAR_NEVER) {
		hsbNeeded = false;
	} else { // hsbPolicy == HORIZONTAL_SCROLLBAR_AS_NEEDED
		hsbNeeded = !viewTracksViewportWidth && (viewPrefSize.width()
				> extentSize.width());
	}

	if (hsb && hsbNeeded) {
		adjustForHSB(true, availR, hsbR, vpbInsets);

		/* If we added the horizontal scrollbar then we've implicitly
		 * reduced  the vertical space available to the viewport.
		 * As a consequence we may have to add the vertical scrollbar,
		 * if that hasn't been done so already.  Of course we
		 * don't bother with any of this if the vsbPolicy is NEVER.
		 */
		if (vsb && !vsbNeeded && (vsbPolicy
				!= ScrollPane::VERTICAL_SCROLLBAR_NEVER)) {

			extentSize = viewport->toViewCoordinates(availR.getSize());
			vsbNeeded = viewPrefSize.height() > extentSize.height();

			if (vsbNeeded) {
				adjustForVSB(true, availR, vsbR, vpbInsets);
			}
		}
	}

	/* Set the size of the viewport first, and then recheck the Scrollable
	 * methods. Some components base their return values for the Scrollable
	 * methods on the size of the Viewport, so that if we don't
	 * ask after resetting the bounds we may have gotten the wrong
	 * answer.
	 */

	if (viewport) {
		viewport->setBounds(availR);

		if (sv) {
			extentSize = viewport->toViewCoordinates(availR.getSize());

			bool oldHSBNeeded = hsbNeeded;
			bool oldVSBNeeded = vsbNeeded;
			viewTracksViewportWidth = sv->getScrollableTracksViewportWidth();
			viewTracksViewportHeight = sv->getScrollableTracksViewportHeight();
			if (vsb && vsbPolicy == ScrollPane::VERTICAL_SCROLLBAR_AS_NEEDED) {
				bool newVSBNeeded = !viewTracksViewportHeight
						&& (viewPrefSize.height() > extentSize.height());
				if (newVSBNeeded != vsbNeeded) {
					vsbNeeded = newVSBNeeded;
					adjustForVSB(vsbNeeded, availR, vsbR, vpbInsets);
					extentSize = viewport->toViewCoordinates(availR.getSize());
				}
			}
			if (hsb && hsbPolicy == ScrollPane::HORIZONTAL_SCROLLBAR_AS_NEEDED)
			{
				bool newHSBbNeeded = !viewTracksViewportWidth
						&& (viewPrefSize.width() > extentSize.width());
				if (newHSBbNeeded != hsbNeeded) {
					hsbNeeded = newHSBbNeeded;
					adjustForHSB(hsbNeeded, availR, hsbR, vpbInsets);
					if (vsb && !vsbNeeded && (vsbPolicy
							!= ScrollPane::VERTICAL_SCROLLBAR_NEVER)) {

						extentSize = viewport->toViewCoordinates(
								availR.getSize());
						vsbNeeded = viewPrefSize.height() > extentSize.height();

						if (vsbNeeded) {
							adjustForVSB(true, availR, vsbR, vpbInsets);
						}
					}
				}
			}
			if (oldHSBNeeded != hsbNeeded || oldVSBNeeded != vsbNeeded) {
				viewport->setBounds(availR);
				// You could argue that we should recheck the
				// Scrollable methods again until they stop changing,
				// but they might never stop changing, so we stop here
				// and don't do any additional checks.
			}
		}
	}

	/* We now have the final size of the viewport: availR.
	 * Now fixup the header and scrollbar widths/heights.
	 */
	vsbR.height(availR.height() + vpbInsets.top() + vpbInsets.bottom());
	hsbR.width(availR.width() + vpbInsets.left() + vpbInsets.right());
	rowHeadR.height(availR.height() + vpbInsets.top() + vpbInsets.bottom());
	rowHeadR.x0().y(availR.x0().y() - vpbInsets.top());
	colHeadR.width(availR.width() + vpbInsets.left() + vpbInsets.right());
	colHeadR.x0().x(availR.x0().x() - vpbInsets.left());

	/* Set the bounds of the remaining components.  The scrollbars
	 * are made invisible if they're not needed.
	 */

	if (rowHead) {
		rowHead->setBounds(rowHeadR);
	}

	if (colHead) {
		colHead->setBounds(colHeadR);
	}

	if (vsb) {
		if (vsbNeeded) {
			bool fill = false;
			ui::getUIManager().getProperty("ScrollPane.fillUpperCorner", fill);
			if (colHead && fill) {
				if (!upperRight) {
					// This is used primarily for GTK L&F, which needs to
					// extend the vertical scrollbar to fill the upper
					// corner near the column header.  Note that we skip
					// this step (and use the default behavior) if the
					// user has set a custom corner component.
					vsbR.x0().y(colHeadR.x0().y());
					vsbR.height(vsbR.height() + colHeadR.height());
				}
			}
			vsb->setVisible(true);
			vsb->setBounds(vsbR);
		} else {
			vsb->setVisible(false);
		}
	}

	if (hsb) {
		if (hsbNeeded) {
			bool fill = false;
			ui::getUIManager().getProperty("ScrollPane.fillLowerCorner", fill);
			if (rowHead && fill) {
				if (!lowerLeft) {
					// This is used primarily for GTK L&F, which needs to
					// extend the horizontal scrollbar to fill the lower
					// corner near the row header.  Note that we skip
					// this step (and use the default behavior) if the
					// user has set a custom corner component.
					hsbR.x0().x(rowHeadR.x0().x());
					hsbR.width( hsbR.width() + rowHeadR.width());
				}
			}
			hsb->setVisible(true);
			hsb->setBounds(hsbR);
		} else {
			hsb->setVisible(false);
		}
	}

	if (lowerLeft) {
		lowerLeft->setBounds(rowHeadR.x0().x(), hsbR.x0().y(),
			rowHeadR.width(), hsbR.height());
	}

	if (lowerRight) {
		lowerRight->setBounds(vsbR.x0().x(), hsbR.x0().y(),
				vsbR.width(), hsbR.height());
	}

	if (upperLeft) {
		upperLeft->setBounds(rowHeadR.x0().x(), colHeadR.x0().y(),
				rowHeadR.width(), colHeadR.height());
	}

	if (upperRight) {
		upperRight->setBounds(vsbR.x0().x(), colHeadR.x0().y(),
				vsbR.width(), colHeadR.height());
	}
}
//-----------------------------------------------------------------------------
Dimension ScrollPaneLayout::
	preferredLayoutSize(AContainerPtr parent)
{
	/* Sync the (now obsolete) policy fields with the
	 * ScrollPane.
	 */
	ScrollPane::Ptr scrollPane = boost::shared_dynamic_cast<ScrollPane>(parent);
	vsbPolicy = scrollPane->getVerticalScrollBarPolicy();
	hsbPolicy = scrollPane->getHorizontalScrollBarPolicy();

	Insets insets = parent->getInsets();
	Coordinate prefWidth = insets.left() + insets.right();
	Coordinate prefHeight = insets.top() + insets.bottom();

	/* Note that viewport.getViewSize() is equivalent to
	 * viewport.getView().getPreferredSize() modulo a null
	 * view or a view whose size was explicitly set.
	 */

	Dimension extentSize = NULL_DIMENSION;
	Dimension viewSize = NULL_DIMENSION;
	AComponentPtr view;

	if (viewport) {
		extentSize = viewport->getPreferredSize();
		view = viewport->getView();
		if (view) {
			viewSize = view->getPreferredSize();
		} else {
			viewSize = Dimension();
		}
	}

	/* If there's a viewport add its preferredSize.
	 */

	if (extentSize!=NULL_DIMENSION) {
		prefWidth = prefWidth + extentSize.width();
		prefHeight = prefHeight +extentSize.height();
	}

	/* If there's a ScrollPane.viewportBorder, add its insets.
	 */

	IBorder::Ptr viewportBorder = scrollPane->getViewportBorder();
	if (viewportBorder) {
		Insets vpbInsets = viewportBorder->getBorderInsets(parent);
		prefWidth = prefWidth + vpbInsets.left() + vpbInsets.right();
		prefHeight = prefHeight + vpbInsets.top() + vpbInsets.bottom();
	}

	/* If a header exists and it's visible, factor its
	 * preferred size in.
	 */

	if ((rowHead) && rowHead->isVisible()) {
		prefWidth = prefWidth + rowHead->getPreferredSize().width();
	}

	if ((colHead) && colHead->isVisible()) {
		prefHeight = prefHeight + colHead->getPreferredSize().height();
	}

	/* If a scrollbar is going to appear, factor its preferred size in.
	 * If the scrollbars policy is AS_NEEDED, this can be a little
	 * tricky:
	 *
	 * - If the view is a Scrollable then scrollableTracksViewportWidth
	 * and scrollableTracksViewportHeight can be used to effectively
	 * disable scrolling (if they're true) in their respective dimensions.
	 *
	 * - Assuming that a scrollbar hasn't been disabled by the
	 * previous constraint, we need to decide if the scrollbar is going
	 * to appear to correctly compute the ScrollPanes preferred size.
	 * To do this we compare the preferredSize of the viewport (the
	 * extentSize) to the preferredSize of the view.  Although we're
	 * not responsible for laying out the view we'll assume that the
	 * Viewport will always give it its preferredSize.
	 */

	if ((vsb) && (vsbPolicy != ScrollPane::VERTICAL_SCROLLBAR_NEVER)) {
		if (vsbPolicy == ScrollPane::VERTICAL_SCROLLBAR_ALWAYS) {
			prefWidth = prefWidth + vsb->getPreferredSize().width();
		} else if ((viewSize!=NULL_DIMENSION) && (extentSize!=NULL_DIMENSION))
		{
			bool canScroll = true;
			IScrollable::Ptr scrollable =
				boost::shared_dynamic_cast<IScrollable>(view);
			if (scrollable) {
				canScroll
						= !scrollable->getScrollableTracksViewportHeight();
			}
			if (canScroll && (viewSize.height() > extentSize.height())) {
				prefWidth += vsb->getPreferredSize().width();
			}
		}
	}

	if ((hsb) && (hsbPolicy != ScrollPane::HORIZONTAL_SCROLLBAR_NEVER)) {
		if (hsbPolicy == ScrollPane::HORIZONTAL_SCROLLBAR_ALWAYS) {
			prefHeight += hsb->getPreferredSize().height();
		} else if ((viewSize!=NULL_DIMENSION) && (extentSize!=NULL_DIMENSION))
		{
			bool canScroll = true;
			IScrollable::Ptr scrollable =
				boost::shared_dynamic_cast<IScrollable>(view);
			if (scrollable) {
				canScroll = !scrollable->getScrollableTracksViewportWidth();
			}
			if (canScroll && (viewSize.width() > extentSize.width())) {
				prefHeight += hsb->getPreferredSize().height();
			}
		}
	}
	return Dimension(prefWidth, prefHeight);
}
//-----------------------------------------------------------------------------
Dimension ScrollPaneLayout::minimumLayoutSize(AContainerPtr parent) {
    /* Sync the (now obsolete) policy fields with the
	 * ScrollPane.
	 */
	ScrollPane::Ptr scrollPane = boost::shared_dynamic_cast<ScrollPane>(parent);
	vsbPolicy = scrollPane->getVerticalScrollBarPolicy();
	hsbPolicy = scrollPane->getHorizontalScrollBarPolicy();

	Insets insets = parent->getInsets();
	Coordinate minWidth = insets.left() + insets.right();
	Coordinate minHeight = insets.top() + insets.bottom();

	/* If there's a viewport add its minimumSize.
	 */

	if (viewport) {
		Dimension size = viewport->getMinimumSize();
		minWidth += size.width();
		minHeight += size.height();
	}

	/* If there's a ScrollPane.viewportBorder, add its insets.
	 */

	IBorder::Ptr viewportBorder = scrollPane->getViewportBorder();
	if (viewportBorder) {
		Insets vpbInsets = viewportBorder->getBorderInsets(parent);
		minWidth += vpbInsets.left() + vpbInsets.right();
		minHeight += vpbInsets.top() + vpbInsets.bottom();
	}

	/* If a header exists and it's visible, factor its
	 * minimum size in.
	 */

	if ((rowHead) && rowHead->isVisible()) {
		Dimension size = rowHead->getMinimumSize();
		minWidth += size.width();
		minHeight = std::max(minHeight, size.height());
	}

	if ((colHead) && colHead->isVisible()) {
		Dimension size = colHead->getMinimumSize();
		minWidth = std::max(minWidth, size.width());
		minHeight += size.height();
	}

	/* If a scrollbar might appear, factor its minimum
	 * size in.
	 */

	if ((vsb) && (vsbPolicy != ScrollPane::VERTICAL_SCROLLBAR_NEVER)) {
		Dimension size = vsb->getMinimumSize();
		minWidth += size.width();
		minHeight = std::max(minHeight, size.height());
	}

	if ((hsb) && (hsbPolicy != ScrollPane::HORIZONTAL_SCROLLBAR_NEVER)) {
		Dimension size = hsb->getMinimumSize();
		minWidth = std::max(minWidth, size.width());
		minHeight += size.height();
	}

	return Dimension(minWidth, minHeight);
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::removeLayoutComponent(AComponentPtr c) {
	if (c == viewport) {
		viewport.reset();
	} else if (c == vsb) {
		vsb.reset();
	} else if (c == hsb) {
		hsb.reset();
	} else if (c == rowHead) {
		rowHead.reset();
	} else if (c == colHead) {
		colHead.reset();
	} else if (c == lowerLeft) {
		lowerLeft.reset();
	} else if (c == lowerRight) {
		lowerRight.reset();
	} else if (c == upperLeft) {
		upperLeft.reset();
	} else if (c == upperRight) {
		upperRight.reset();
	}
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::
	setHorizontalScrollBarPolicy(ScrollPane::HorizontalScrollbarPolicy x)
{
	hsbPolicy = x;
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::
	setVerticalScrollBarPolicy(ScrollPane::VerticalScrollbarPolicy x)
{
	vsbPolicy = x;
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::syncWithScrollPane(ScrollPanePtr sp) {
	viewport = sp->getViewport();
	vsb = sp->getVerticalScrollBar();
	hsb = sp->getHorizontalScrollBar();
	rowHead = sp->getRowHeader();
	colHead = sp->getColumnHeader();
	lowerLeft = sp->getCorner(ScrollPane::LOWER_LEFT_CORNER);
	lowerRight = sp->getCorner(ScrollPane::LOWER_RIGHT_CORNER);
	upperLeft = sp->getCorner(ScrollPane::UPPER_LEFT_CORNER);
	upperRight = sp->getCorner(ScrollPane::UPPER_RIGHT_CORNER);
	vsbPolicy = sp->getVerticalScrollBarPolicy();
	hsbPolicy = sp->getHorizontalScrollBarPolicy();
}
}}} // ScrollPaneLayout::namespace(s)
