/*
 * Viewport.cpp
 *
 *  Created on: Sat Jun 23 19:29:57 2012
 *      Author: Johannes Unger
 */

#include "Viewport.hpp"
#include "ui/ALookAndFeel.hpp"
#include "RedrawManager.hpp"
#include <sambag/disco/IDiscoFactory.hpp>
#include "Graphics.hpp"
#include "ViewportLayout.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Viewport
//=============================================================================
//-----------------------------------------------------------------------------
Viewport::Viewport() : scrollMode(BLIT_SCROLL_MODE) {
	setName("Viewport");
	setLayout(createLayoutManager());
	setOpaque(true);
	lastPaintPosition = NULL_POINT2D;
}
//-----------------------------------------------------------------------------
Viewport::~Viewport() {
	if (redrawTimer) {
		if (redrawTimer->isRunning())
			redrawTimer->stop();
	}
}
//----------------------------------------------------------------------------
ui::AComponentUIPtr Viewport::createComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<Viewport>();
}
//-------------------------------------------------------------------------
void Viewport::addComponent(AComponentPtr child, int index) {
	setView(child);
}
//-----------------------------------------------------------------------------
bool Viewport::computeBlit(const Coordinate &dx, const Coordinate &dy,
		Point2D & blitFrom, Point2D & blitTo,
		Dimension & blitSize, Rectangle & blitPaint)
{
	Coordinate dxAbs = std::abs(dx);
	Coordinate dyAbs = std::abs(dy);
	Dimension extentSize = getExtentSize();

	if ((dx == 0) && (dy != 0) && (dyAbs < extentSize.height())) {
		if (dy < 0) {
			blitFrom.y(-dy);
			blitTo.y(0);
			blitPaint.y(extentSize.height() + dy);
		} else {
			blitFrom.y(0);
			blitTo.y(dy);
			blitPaint.y(0);
		}

		blitPaint.x(0); blitFrom.x(0); blitTo.x(0);

		blitSize.width(extentSize.width());
		blitSize.height(extentSize.height() - dyAbs);

		blitPaint.width(extentSize.width());
		blitPaint.height(dyAbs);

		return true;
	}
	else if ((dy == 0) && (dx != 0) && (dxAbs < extentSize.width())) {
		if (dx < 0) {
			blitFrom.x(-dx);
			blitTo.x(0);
			blitPaint.x(extentSize.width() + dx);
		} else {
			blitFrom.x(0);
			blitTo.x(dx);
			blitPaint.x(0);
		}

		blitPaint.y(0); blitFrom.y(0); blitTo.y(0);

		blitSize.width(extentSize.width() - dxAbs);
		blitSize.height(extentSize.height());

		blitPaint.width(dxAbs);
		blitPaint.height(extentSize.height());
		return true;
	}
	else {
		return false;
	}
}
//-----------------------------------------------------------------------------
ALayoutManagerPtr Viewport::createLayoutManager() {
	return ViewportLayout::getSharedInstance();
}
//-----------------------------------------------------------------------------
void Viewport::fireStateChanged() {
	EventSender<ViewportChanged>::notifyListeners(
			this,
			ViewportChanged(*this)
	);
}
//-----------------------------------------------------------------------------
bool Viewport::needsRepaintAfterBlit() {
	return false;
}
//-----------------------------------------------------------------------------
bool Viewport::isPaintingOrigin() const {
	return scrollMode == BACKINGSTORE_SCROLL_MODE;
}
//-----------------------------------------------------------------------------
std::string Viewport::paramString() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return "";
}
//-----------------------------------------------------------------------------
Dimension Viewport::getExtentSize() const {
	return getSize();
}
//-----------------------------------------------------------------------------
Insets Viewport::getInsets() const {
	return Insets(0, 0, 0, 0);
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr Viewport::getBackingStoreContext(IDrawContext::Ptr cn) {
	IDrawContext::Ptr bsg = getDiscoFactory()->createContext(backingStoreImage);
	bsg->setFillColor(cn->getFillColor());
	bsg->setStrokeColor(cn->getStrokeColor());
	bsg->setFont(cn->getCurrentFont());
	bsg->setClip(cn->clipExtends());
	return bsg;
}
//-----------------------------------------------------------------------------
void Viewport::drawViaBackingStore(IDrawContext::Ptr cn) {
	IDrawContext::Ptr bsc = getBackingStoreContext(cn);
	Super::draw(bsc);
	cn->drawSurface(backingStoreImage);
}
//-----------------------------------------------------------------------------
void Viewport::drawViaBackingStore(IDrawContext::Ptr cn,
		const Rectangle &oClip)
{
	IDrawContext::Ptr bsc = getBackingStoreContext(cn);
	Super::draw(bsc);
	cn->setClip(oClip);
	cn->drawSurface(backingStoreImage);
}
//-----------------------------------------------------------------------------
AComponentPtr Viewport::getView() const {
	return (getComponentCount() > 0) ? getComponent(0) : AComponentPtr();
}
//-----------------------------------------------------------------------------
Viewport::ScrollMode Viewport::getScrollMode() const {
	return scrollMode;
}
//-----------------------------------------------------------------------------
Point2D Viewport::getViewLocation() const {
	AComponentPtr view = getView();
	if (view)
		return view->getLocation();
	return Point2D(0, 0);
 }
//-----------------------------------------------------------------------------
Point2D Viewport::getViewPosition() const {
	AComponentPtr view = getView();
	if (view) {
		Point2D p = view->getLocation();
		p.x(-p.x());
		p.y(-p.y());
		return p;
	} else {
		return Point2D(0, 0);
	}
}
//-----------------------------------------------------------------------------
Rectangle Viewport::getViewRect() const {
	Dimension ex = getExtentSize();
	return Rectangle( getViewPosition(),
		ex.width(),
		ex.height()
	);
}
//-----------------------------------------------------------------------------
Dimension Viewport::getViewSize() const {
	AComponentPtr view = getView();
	if (!view) {
		return Dimension(0, 0);
	} else if (isViewSizeSet) {
		return view->getSize();
	} else {
		return view->getPreferredSize();
	}
}
//-----------------------------------------------------------------------------
void Viewport::redrawTimerClbk(void *src, const TimerEvent &ev) {
	// waitingForRepaint will be false if a paint came down
	// with the complete clip rect, in which case we don't
	// have to cause a repaint.
	if (waitingForRepaint) {
		Super::redraw();
	}
}
//-----------------------------------------------------------------------------
Timer::Ptr Viewport::createRedrawTimer() {
	Timer::Ptr timer = Timer::create(300);
	timer->EventSender<TimerEvent>::addTrackedEventListener(
		boost::bind(&Viewport::redrawTimerClbk, this, _1, _2),
		getPtr()
	);
	return timer;
}
//-----------------------------------------------------------------------------
void Viewport::draw(IDrawContext::Ptr cn) {
	Coordinate width = getWidth();
	Coordinate height = getHeight();

	if ((width <= 0) || (height <= 0)) {
		return;
	}

	if (inBlitPaint) {
		// We invoked paint as part of copyArea cleanup, let it through.
		Super::draw(cn);
		return;
	}

	if (repaintAll) {
		repaintAll = false;
		Rectangle clipB = cn->clipExtends();
		if (clipB.width() < getWidth() || clipB.height() < getHeight()) {
			waitingForRepaint = true;
			if (!redrawTimer) {
				redrawTimer = createRedrawTimer();
			}
			redrawTimer->stop();
			redrawTimer->start();
			// We really don't need to paint, a future repaint will
			// take care of it, but if we don't we get an ugly flicker.
		} else {
			if (redrawTimer) {
				redrawTimer->stop();
			}
			waitingForRepaint = false;
		}
	} else if (waitingForRepaint) {
		// Need a complete repaint before resetting waitingForRepaint
		Rectangle clipB = cn->clipExtends();
		if (clipB.width() >= getWidth() && clipB.height() >= getHeight()) {
			waitingForRepaint = false;
			redrawTimer->stop();
		}
	}

	if (!backingStore || isBlitting() || !getView()) {
		Super::draw(cn);
		lastPaintPosition = getViewLocation();
		return;
	}

	// If the view is smaller than the viewport and we are not opaque
	// (that is, we won't paint our background), we should set the
	// clip. Otherwise, as the bounds of the view vary, we will
	// blit garbage into the exposed areas.
	const Rectangle &viewBounds = getView()->getBounds();
	if (!isOpaque()) {
		Graphics g(cn);
		g.clipRect(Rectangle(0, 0, viewBounds.width(), viewBounds.height()));
	}

	if (!backingStoreImage) {
		// Backing store is enabled but this is the first call to paint.
		// Create the backing store, paint it and then copy to g.
		// The backing store image will be created with the size of
		// the viewport. We must make sure the clip region is the
		// same size, otherwise when scrolling the backing image
		// the region outside of the clipped region will not be painted,
		// and result in empty areas.
		try {
			backingStoreImage =
					getDiscoFactory()->createImageSurface((int)width, (int)height);
		} catch (...) {
			setScrollMode(Viewport::BLIT_SCROLL_MODE);
			return;
		}
		Rectangle clip = cn->clipExtends();
		if (clip.width() != width || clip.height() != height) {
			if (!isOpaque()) {
				cn->setClip(Rectangle(0, 0,
						std::min(viewBounds.width(), width),
						std::min(viewBounds.height(), height)));
			} else {
				cn->setClip(Rectangle(0, 0, width, height));
			}
			drawViaBackingStore(cn, clip);
		} else {
			drawViaBackingStore(cn);
		}
	} else {
		if (!scrollUnderway || lastPaintPosition == getViewLocation()) {
			// No scrolling happened: repaint required area via backing store.
			drawViaBackingStore(cn);
		} else {
			// The image was scrolled. Manipulate the backing store and flush it to g.
			Point2D blitFrom;
			Point2D blitTo;
			Dimension blitSize;
			Rectangle blitPaint;

			Point2D newLocation = getViewLocation();
			Coordinate dx = newLocation.x() - lastPaintPosition.x();
			Coordinate dy = newLocation.y() - lastPaintPosition.y();
			bool canBlit = computeBlit(dx, dy, blitFrom, blitTo, blitSize,
					blitPaint);
			if (!canBlit) {
				// The image was either moved diagonally or
				// moved by more than the image size: paint normally.
				drawViaBackingStore(cn);
			} else {
				Coordinate bdx = blitTo.x() - blitFrom.x();
				Coordinate bdy = blitTo.y() - blitFrom.y();

				// Move the relevant part of the backing store.
				Rectangle clip = cn->clipExtends();
				// We don't want to inherit the clip region when copying
				// bits, if it is inherited it will result in not moving
				// all of the image resulting in garbage appearing on
				// the screen.
				cn->setClip(Rectangle(0, 0, width, height));
				IDrawContext::Ptr bsc = getBackingStoreContext(cn);
				Graphics bsg(bsc);
				bsg.copyArea(Rectangle(blitFrom.x(), blitFrom.y(), blitSize.width(),
						blitSize.height()), Point2D(bdx, bdy));

				cn->setClip(Rectangle(clip.x(),
						clip.y(), clip.width(), clip.height()));
				// Draw the rest of the view; the part that has just been exposed.
				Rectangle r; // = viewBounds.intersection(blitPaint);
				boost::geometry::intersection<Rectangle::Base,
				Rectangle::Base, Rectangle::Base>(viewBounds,blitPaint,r);
				bsg.setClip(r);
				Super::draw(bsc);

				// Copy whole of the backing store to g.
				cn->drawSurface(backingStoreImage);
			}
		}
	}
	lastPaintPosition = getViewLocation();
	scrollUnderway = false;
}
//-----------------------------------------------------------------------------
bool Viewport::isBlitting() const {
	AComponentPtr view = getView();
	return (scrollMode == BLIT_SCROLL_MODE) && view->isOpaque();
}
//-----------------------------------------------------------------------------
void Viewport::remove(AComponentPtr child) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::redraw(const Rectangle &r) {
//  TODO: only a workarund see: #171
//	AContainerPtr parent = getParent();
//	if(parent) {
//		Rectangle d(
//			r.x() + getX(), r.y() + getY(),
//			r.width(), r.height()
//		);
//		parent->redraw(d);
//	}
//	else
		Super::redraw(r);
}
//-----------------------------------------------------------------------------
void Viewport::setBounds(const Rectangle &r) {
	bool sizeChanged = (getWidth() != r.width()) || (getHeight() != r.height());
	if (sizeChanged) {
		backingStoreImage.reset();
	}
	Super::setBounds(r);
	if (sizeChanged) {
		fireStateChanged();
	}
}
//-----------------------------------------------------------------------------
void Viewport::scrollRectToVisible(const Rectangle & contentRect) {
	AComponentPtr view = getView();

	if (!view) {
		return;
	} else {
		if (!view->isValid()) {
			// If the view is not valid, validate. scrollRectToVisible
			// may fail if the view is not valid first, contentRect
			// could be bigger than invalid size.
			validateView();
		}
		Coordinate dx, dy;

		dx = positionAdjustment(getWidth(),
				contentRect.width(), contentRect.x());
		dy = positionAdjustment(getHeight(),
				contentRect.height(), contentRect.y());

		if (dx != 0 || dy != 0) {
			Point2D viewPosition = getViewPosition();
			Dimension viewSize = view->getSize();
			Coordinate startX = viewPosition.x();
			Coordinate startY = viewPosition.y();
			Dimension extent = getExtentSize();

			viewPosition.x(viewPosition.x() - dx);
			viewPosition.y(viewPosition.y() - dy);
			// Only constrain the location if the view is valid. If the
			// the view isn't valid, it typically indicates the view
			// isn't visible yet and most likely has a bogus size as will
			// we, and therefore we shouldn't constrain the scrolling
			if (view->isValid()) {
				if (viewPosition.x() + extent.width() > viewSize.width()) {
					viewPosition.x( std::max( (Number)0,
							viewSize.width() - extent.width()));
				} else if (viewPosition.x() < 0) {
					viewPosition.x(0);
				}
				if (viewPosition.y() + extent.height() > viewSize.height()) {
					viewPosition.y( std::max( (Number)0,
							viewSize.height() - extent.height()));
				} else if (viewPosition.y() < 0) {
					viewPosition.y(0);
				}
			}
			if (viewPosition.x() != startX || viewPosition.y() != startY) {
				setViewPosition(viewPosition);
				// NOTE: How Viewport currently works with the
				// backing store is not foolproof. The sequence of
				// events when setViewPosition
				// (scrollRectToVisible) is called is to reset the
				// views bounds, which causes a repaint on the
				// visible region and sets an ivar indicating
				// scrolling (scrollUnderway). When
				// Viewport.paint is invoked if scrollUnderway is
				// true, the backing store is blitted.  This fails
				// if between the time setViewPosition is invoked
				// and paint is received another repaint is queued
				// indicating part of the view is invalid. There
				// is no way for Viewport to notice another
				// repaint has occured and it ends up blitting
				// what is now a dirty region and the repaint is
				// never delivered.
				// It just so happens Table encounters this
				// behavior by way of scrollRectToVisible, for
				// this reason scrollUnderway is set to false
				// here, which effectively disables the backing
				// store.
				scrollUnderway = false;
			}
		}
	}
}
//-----------------------------------------------------------------------------
Coordinate Viewport::positionAdjustment(const Coordinate &parentWidth,
		const Coordinate &childWidth, const Coordinate &childAt)
{

	//   +-----+
	//   | --- |     No Change
	//   +-----+
	if (childAt >= 0 && childWidth + childAt <= parentWidth) {
		return 0;
	}

	//   +-----+
	//  ---------   No Change
	//   +-----+
	if (childAt <= 0 && childWidth + childAt >= parentWidth) {
		return 0;
	}

	//   +-----+          +-----+
	//   |   ----    ->   | ----|
	//   +-----+          +-----+
	if (childAt > 0 && childWidth <= parentWidth) {
		return -childAt + parentWidth - childWidth;
	}

	//   +-----+             +-----+
	//   |  --------  ->     |--------
	//   +-----+             +-----+
	if (childAt >= 0 && childWidth >= parentWidth) {
		return -childAt;
	}

	//   +-----+          +-----+
	// ----    |     ->   |---- |
	//   +-----+          +-----+
	if (childAt <= 0 && childWidth <= parentWidth) {
		return -childAt;
	}

	//   +-----+             +-----+
	//-------- |      ->   --------|
	//   +-----+             +-----+
	if (childAt < 0 && childWidth >= parentWidth) {
		return -childAt + parentWidth - childWidth;
	}

	return 0;
}
//-----------------------------------------------------------------------------
void Viewport::validateView() {
	AComponentPtr validateRoot = getValidateRoot();

	if (!validateRoot) {
		return;
	}

	// Validate the root.
	validateRoot->validate();

	// And let the RepaintManager it does not have to validate from
	// validateRoot anymore.
	RedrawManager::Ptr rm = RedrawManager::currentManager(getPtr());

	if (rm) {
		rm->removeInvalidComponent(validateRoot);
	}
}
//-----------------------------------------------------------------------------
void Viewport::setBorder(IBorder::Ptr border) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Viewport::setExtentSize(const Dimension & newExtent) {
	Dimension oldExtent = getExtentSize();
	if (newExtent!=oldExtent) {
		setSize(newExtent);
		fireStateChanged();
	}
}
//-----------------------------------------------------------------------------
void Viewport::setScrollMode(ScrollMode mode) {
	scrollMode = mode;
	backingStore = mode == BACKINGSTORE_SCROLL_MODE;
}
//-----------------------------------------------------------------------------
void Viewport::setView(AComponentPtr view) {
	/* Remove the viewport's existing children, if any.
	 * Note that removeAll() isn't used here because it
	 * doesn't call remove()
	 */
	int n = getComponentCount();
	for (int i = n - 1; i >= 0; --i) {
		remove(getComponent(i));
	}

	isViewSizeSet = false;

	if (view) {
		Super::add(view);
	}

	if (hasHadValidView) {
		// Only fire a change if a view has been installed.
		fireStateChanged();
	} else if (view) {
		hasHadValidView = true;
	}
	revalidate();
	Super::redraw();
}
//-----------------------------------------------------------------------------
void Viewport::setViewPosition(const Point2D & p) {
	AComponentPtr view = getView();
	if (!view) {
		return;
	}

	Coordinate oldX, oldY, x = p.x(), y = p.y();

	/* Collect the old x,y values for the views location
	 * and do the song and dance to avoid allocating
	 * a Rectangle object if we don't have to.
	 */

	oldX = view->getX();
	oldY = view->getY();

	/* The view scrolls in the opposite direction to mouse
	 * movement.
	 */
	Coordinate newX = -x;
	Coordinate newY = -y;

	if ((oldX != newX) || (oldY != newY)) {
		if (!waitingForRepaint && isBlitting() && canUseWindowBlitter()) {
			RedrawManager::Ptr rm = RedrawManager::currentManager(getPtr());
			if (!rm) {
				return;
			}
			Rectangle dirty = rm->getDirtyRegion(view);
			//if (dirty.contains(jview.getVisibleRect())) {
			if (boost::geometry::within<Rectangle::Base, Rectangle::Base>
				(dirty, view->getVisibleRect())) {
				IDrawContext::Ptr cn = view->getDrawContext();
				flushViewDirtyRegion(cn, dirty);
				view->setLocation(Point2D(newX, newY));
				cn->setClip(Rectangle(0, 0, getWidth(),
						std::min(getHeight(), view->getHeight())));
				// Repaint the complete component if the blit succeeded
				// and needsRepaintAfterBlit returns true.
				repaintAll = (windowBlitDraw(cn) && needsRepaintAfterBlit());
				rm->markCompletelyClean(getParent());
				rm->markCompletelyClean(getPtr());
				rm->markCompletelyClean(view);

			} else {
				// The visible region is dirty, no point in doing copyArea
				view->setLocation(Point2D(newX, newY));
				repaintAll = false;
			}
		} else {
			scrollUnderway = true;
			// This calls setBounds(), and then repaint().
			view->setLocation(Point2D(newX, newY));
			repaintAll = false;
		}
		// we must validate the hierarchy to not break the hw/lw mixing
		revalidate();
		fireStateChanged();
	}
}
//-----------------------------------------------------------------------------
void Viewport::setViewSize(const Dimension & newSize) {
	AComponent::Ptr view = getView();
		if (view) {
			Dimension oldSize = view->getSize();
			if (newSize != oldSize) {
				// scrollUnderway will be true if this is invoked as the
				// result of a validate and setViewPosition was previously
				// invoked.
				scrollUnderway = false;
				view->setSize(newSize);
				isViewSizeSet = true;
				fireStateChanged();
			}
		}
}
//-----------------------------------------------------------------------------
Dimension Viewport::toViewCoordinates(const Dimension & size) const {
	return size;
}
//-----------------------------------------------------------------------------
Point2D Viewport::toViewCoordinates(const Point2D &p) const {
	return p;
}
//-----------------------------------------------------------------------------
void Viewport::flushViewDirtyRegion(IDrawContext::Ptr cn,
		Rectangle &dirty)
{
	Graphics g(cn);
	AComponentPtr view = getView();
	if (dirty.width() > 0 && dirty.height() > 0) {
		dirty.x((view->getX()));
		dirty.y((view->getY()));
		Rectangle clip = g.clipExtends();
		g.clipRect(Rectangle(dirty.x(),
				dirty.y(), dirty.width(), dirty.height()));
		clip = cn->clipExtends();
		// Only paint the dirty region if it is visible.
		if (clip.width() > 0 && clip.height() > 0) {
			drawView(cn);
		}
	}
}
//-----------------------------------------------------------------------------
bool Viewport::windowBlitDraw(IDrawContext::Ptr cn) {
	Coordinate width = getWidth();
	Coordinate height = getHeight();

	if ((width == 0) || (height == 0)) {
		return false;
	}

	bool retValue;
	RedrawManager::Ptr rm = RedrawManager::currentManager(getPtr());
	if (!rm) {
		return false;
	}
	AComponentPtr view = getView();

	if (lastPaintPosition == NULL_POINT2D
			|| lastPaintPosition == getViewLocation()) {
		drawView(cn);
		retValue = false;
	} else {
		// The image was scrolled. Manipulate the backing store and flush
		// it to g.
		Point2D blitFrom;
		Point2D blitTo;
		Dimension blitSize;
		Rectangle blitPaint;

		Point2D newLocation = getViewLocation();
		Coordinate dx = newLocation.x() - lastPaintPosition.x();
		Coordinate dy = newLocation.y() - lastPaintPosition.y();
		bool canBlit = computeBlit(dx, dy, blitFrom, blitTo, blitSize,
				blitPaint);
		if (!canBlit) {
			drawView(cn);
			retValue = false;
		} else {
			// Prepare the rest of the view; the part that has just been
			// exposed.
			Rectangle r; // = view.getBounds().intersection(blitPaint);
			boost::geometry::intersection<Rectangle::Base, Rectangle::Base,
			Rectangle::Base>(view->getBounds(), blitPaint, r);
			r.x( r.x() - view->getX() );
			r.y( r.y() - view->getY() );

			blitDoubleBuffered(view, cn, r.x(), r.y(), r.width(), r.height(),
					blitFrom.x(), blitFrom.y(), blitTo.x(), blitTo.y(), blitSize.width(),
					blitSize.height());
			retValue = true;
		}
	}
	lastPaintPosition = getViewLocation();
	return retValue;
}
//-----------------------------------------------------------------------------
void Viewport::blitDoubleBuffered(AComponentPtr view, IDrawContext::Ptr cn,
		const Coordinate &clipX, const Coordinate &clipY,
		const Coordinate &clipW, const Coordinate &clipH,
		const Coordinate &blitFromX, const Coordinate &blitFromY,
		const Coordinate &blitToX, const Coordinate &blitToY,
		const Coordinate &blitW, const Coordinate &blitH)
{
	// NOTE:
	//   blitFrom/blitTo are in JViewport coordinates system
	//     not the views coordinate space.
	//   clip* are in the views coordinate space.
	RedrawManager::Ptr rm = RedrawManager::currentManager(getPtr());
	if (!rm) {
		return;
	}
	Coordinate bdx = blitToX - blitFromX;
	Coordinate bdy = blitToY - blitFromY;

	// Shift the scrolled region
	rm->copyArea(getPtr(), cn,
		Rectangle(blitFromX, blitFromY, blitW, blitH),
		Point2D(bdx, bdy), false);

	// Paint the newly exposed region.
	Coordinate x = view->getX();
	Coordinate y = view->getY();
	cn->translate(Point2D(x, y));
	cn->setClip(Rectangle(clipX, clipY, clipW, clipH));
	view->drawForceDoubleBuffered(cn);
	cn->translate(Point2D(-x, -y));
}
//-----------------------------------------------------------------------------
void Viewport::drawView(IDrawContext::Ptr cn) {
	Rectangle clip = cn->clipExtends();
	AComponentPtr view = getView();

	if (view->getWidth() >= getWidth()) {
		// Graphics is relative to JViewport, need to map to view's
		// coordinates space.
		Coordinate x = view->getX();
		Coordinate y = view->getY();
		cn->translate(Point2D(x, y));
		cn->setClip(Rectangle(clip.x() - x, clip.y() - y,
				clip.width(), clip.height()));
		view->drawForceDoubleBuffered(cn);
		cn->translate(Point2D(-x, -y));
		cn->setClip(Rectangle(clip.x(),
				clip.y(), clip.width(), clip.height()));
	} else {
		// To avoid any problems that may result from the viewport being
		// bigger than the view we start painting from the viewport.
		inBlitPaint = true;
		drawForceDoubleBuffered(cn);
		inBlitPaint = false;
	}
}
//-----------------------------------------------------------------------------
bool Viewport::canUseWindowBlitter() {
	if (!isShowing() || !getView()) {
		return false;
	}
	if (isDrawing()) {
		// We're in the process of painting, don't blit. If we were
		// to blit we would draw on top of what we're already drawing,
		// so bail.
		return false;
	}

	Rectangle dirtyRegion =
			RedrawManager::currentManager(getPtr())->getDirtyRegion(
					getParent());

	if (dirtyRegion.width() > 0 && dirtyRegion.height() > 0) {
		// Part of the scrollpane needs to be repainted too, don't blit.
		return false;
	}

	Rectangle clip(0, 0, getWidth(), getHeight());
	Rectangle oldClip;
	Rectangle tmp2;
	AContainerPtr parent;
	AComponentPtr lastParent;
	Coordinate x, y, w, h;

	for (parent = getPtr(); parent; parent
			= parent->getParent()) {
		x = parent->getX();
		y = parent->getY();
		w = parent->getWidth();
		h = parent->getHeight();

		oldClip = clip;
		//SwingUtilities.computeIntersection(0, 0, w, h, clip);
		boost::geometry::intersection<Rectangle::Base,
		Rectangle::Base, Rectangle::Base>(Rectangle(0,0,w,h), clip, clip);
		if (clip != oldClip)
			return false;

		if (lastParent) {
			const AContainer::Components &comps = parent->getComponents();
			int index = 0;
			for (int i = comps.size() - 1; i >= 0; --i) {
				if (comps[i] == lastParent) {
					index = i - 1;
					break;
				}
			}

			while (index >= 0) {
				tmp2 = comps[index]->getBounds();
				//if (tmp2.intersects(clip))
				if (boost::geometry::intersects<Rectangle::Base,
						Rectangle::Base>(tmp2, clip))
					return false;
				index--;
			}
		}
//		clip.x += x;
//		clip.y += y;
		clip.translate(Point2D(x,y));
		lastParent = parent;
	}
	if (!parent) {
		// No Window parent.
		return false;
	}
	return true;
}
}}} // namespace(s)
