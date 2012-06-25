/*
 * Viewport.cpp
 *
 *  Created on: Sat Jun 23 19:29:57 2012
 *      Author: Johannes Unger
 */

#include "Viewport.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Viewport
//=============================================================================
//-----------------------------------------------------------------------------
Viewport::Viewport() {
	setName("Viewport");
	setLayout(createLayoutManager());
	setOpaque(true);
}
//----------------------------------------------------------------------------
ui::AComponentUIPtr Viewport::getComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<Viewport>();
}
//-------------------------------------------------------------------------
void Viewport::addComponent(AComponentPtr child, int index) {
	setView(child);
}
//-----------------------------------------------------------------------------
bool Viewport::computeBlit(int dx, int dy, const Point2D & blitFrom,
		const Point2D & blitTo, const Dimension & blitSize,
		const Rectangle & blitPaint)
{
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
ALayoutManagerPtr Viewport::createLayoutManager() {
	SAMBA_LOG_NOT_YET_IMPL();
	return ALayoutManagerPtr();
}
//-----------------------------------------------------------------------------
void Viewport::fireStateChanged() const {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
bool Viewport::isPaintingOrigin() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
std::string Viewport::paramString() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return "";
}
//-----------------------------------------------------------------------------
Dimension Viewport::getExtentSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
Insets Viewport::getInsets() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Insets();
}
//-----------------------------------------------------------------------------
AComponentPtr Viewport::getView() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponentPtr();
}
//-----------------------------------------------------------------------------
Insets Viewport::getInsets(Insets insets) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Insets();
}
//-----------------------------------------------------------------------------
Viewport::ScrollMode Viewport::getScrollMode() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return (ScrollMode)0;
}
//-----------------------------------------------------------------------------
Point2D Viewport::getViewPosition() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_POINT2D;
}
//-----------------------------------------------------------------------------
Rectangle Viewport::getViewRect() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_RECTANGLE;
}
//-----------------------------------------------------------------------------
Dimension Viewport::getViewSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
void Viewport::paint(IDrawContext::Ptr cn) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::remove(AComponentPtr child) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::repaint(int x, int y, int w, int h) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::reshape(int x, int y, int w, int h) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::scrollRectToVisible(const Rectangle & contentRect) {
/*	Component view = getView();

	if (view == null) {
		return;
	} else {
		if (!view.isValid()) {
			// If the view is not valid, validate. scrollRectToVisible
			// may fail if the view is not valid first, contentRect
			// could be bigger than invalid size.
			validateView();
		}
		int dx, dy;

		dx = positionAdjustment(getWidth(), contentRect.width, contentRect.x);
		dy = positionAdjustment(getHeight(), contentRect.height, contentRect.y);

		if (dx != 0 || dy != 0) {
			Point viewPosition = getViewPosition();
			Dimension viewSize = view.getSize();
			int startX = viewPosition.x;
			int startY = viewPosition.y;
			Dimension extent = getExtentSize();

			viewPosition.x -= dx;
			viewPosition.y -= dy;
			// Only constrain the location if the view is valid. If the
			// the view isn't valid, it typically indicates the view
			// isn't visible yet and most likely has a bogus size as will
			// we, and therefore we shouldn't constrain the scrolling
			if (view.isValid()) {
				if (getParent().getComponentOrientation().isLeftToRight()) {
					if (viewPosition.x + extent.width > viewSize.width) {
						viewPosition.x = Math.max(0,
								viewSize.width - extent.width);
					} else if (viewPosition.x < 0) {
						viewPosition.x = 0;
					}
				} else {
					if (extent.width > viewSize.width) {
						viewPosition.x = viewSize.width - extent.width;
					} else {
						viewPosition.x = Math.max(
								0,
								Math.min(viewSize.width - extent.width,
										viewPosition.x));
					}
				}
				if (viewPosition.y + extent.height > viewSize.height) {
					viewPosition.y = Math.max(0,
							viewSize.height - extent.height);
				} else if (viewPosition.y < 0) {
					viewPosition.y = 0;
				}
			}
			if (viewPosition.x != startX || viewPosition.y != startY) {
				setViewPosition(viewPosition);
				// NOTE: How JViewport currently works with the
				// backing store is not foolproof. The sequence of
				// events when setViewPosition
				// (scrollRectToVisible) is called is to reset the
				// views bounds, which causes a repaint on the
				// visible region and sets an ivar indicating
				// scrolling (scrollUnderway). When
				// JViewport.paint is invoked if scrollUnderway is
				// true, the backing store is blitted.  This fails
				// if between the time setViewPosition is invoked
				// and paint is received another repaint is queued
				// indicating part of the view is invalid. There
				// is no way for JViewport to notice another
				// repaint has occured and it ends up blitting
				// what is now a dirty region and the repaint is
				// never delivered.
				// It just so happens JTable encounters this
				// behavior by way of scrollRectToVisible, for
				// this reason scrollUnderway is set to false
				// here, which effectively disables the backing
				// store.
				scrollUnderway = false;
			}
		}
	}*/
}
//-----------------------------------------------------------------------------
void Viewport::setBorder(IBorder::Ptr border) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::setExtentSize(const Dimension & newExtent) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::setScrollMode(ScrollMode mode) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::setView(AComponentPtr view) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::setViewPosition(const Point2D & p) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::setViewSize(const Dimension & newSize) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
Dimension Viewport::toViewCoordinates(const Dimension & size) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
Point2D Viewport::toViewCoordinates(Point2D p) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_POINT2D;
}
}}} // namespace(s)
