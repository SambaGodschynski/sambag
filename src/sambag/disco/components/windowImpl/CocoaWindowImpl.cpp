/*
 * CocoaWindowImpl.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#include "CocoaWindowImpl.hpp"
#include "cocoaimpl/_CocoaWindowImpl.h"
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/disco/QuartzSurface.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class CocoaWindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__processDraw(CGContextRef context, Nb x, Nb y, Nb w, Nb h)
{
	QuartzSurface::Ptr sf = QuartzSurface::create(context,w,h);
	SAMBAG_ASSERT(sf);
	this->processDraw(sf);
}
//-----------------------------------------------------------------------------
CocoaWindowImpl::CocoaWindowImpl() :
	visible(false),
    bounds(Rectangle(0,0,1,1))
{
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::initAsNestedWindow(ArbitraryType::Ptr osParent,
	const Rectangle &area)
{
	void *win = NULL;
	com::get(osParent, win);
	if (!win) {
		SAMBAG_LOG_WARN<<"initAsNestedWindow failed because parent==NULL";
		return;
	}
	Impl::openNested(win, area.x(), area.y(), area.width(), area.height());
	bounds = area;
	visible = true;
	onCreated();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::_close() {
    if (!isVisible()) {
        return;
    }
    visible = false;
 	Impl::closeWindow();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::close() {
    if (getFlag(WND_NESTED)) {
        _close();
        return;
    }
    getWindowToolkit()->invokeLater(
		boost::bind(&CocoaWindowImpl::_close, this)
	);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::_open(AWindowImplPtr parent) {
    if (isVisible()) {
        return;
    }
	Impl *parentImpl = dynamic_cast<Impl*>(parent.get());
    
	Impl::openWindow(
			parentImpl,
			(int)bounds.x(),
			(int)bounds.y(),
			(int)bounds.width(),
			(int)bounds.height()
	);
	visible = true;
	updateTitle();
}
//-------------------------------------------------------------------------
void CocoaWindowImpl::__onCreated() {
     onCreated();   
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::open(AWindowImplPtr parent) {
	getWindowToolkit()->invokeLater(
		boost::bind(&CocoaWindowImpl::_open, this, parent),
        50,
        shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::updateWindowToBounds(const Rectangle &r) {
	bounds = r;
	boundsUpdated();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::updateBoundsToWindow() {
	Nb x = bounds.x();
	Nb y = bounds.y();
	Nb w = bounds.width();
	Nb h = bounds.height();
	if (getFlag(WindowFlags::WND_NESTED)) {
		Rectangle r = getHostBounds();
		x = r.x();
		y = r.y();
	}
	Impl::setBounds(x,y,w,h);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__boundsChanged(Nb x, Nb y, Nb w, Nb h) {
	updateWindowToBounds(Rectangle(x,y,w,h));
}
//-----------------------------------------------------------------------------
void * CocoaWindowImpl::getSystemHandle() const {
	return Impl::getWindowRef();
}
//-----------------------------------------------------------------------------
void * CocoaWindowImpl::getNSView() const {
    return Impl::getViewRef();
}
//-----------------------------------------------------------------------------
CocoaWindowImpl::~CocoaWindowImpl() {
}
//-----------------------------------------------------------------------------
bool CocoaWindowImpl::isVisible() const {
	return visible;
}
//-----------------------------------------------------------------------------
Rectangle CocoaWindowImpl::getHostBounds() const {
	//get real window bounds (in case of vst area location dosen't fit with the
	// window
	Nb x=0, y=0, h=0, w=0;
	Impl::getBounds(x,y,w,h);
	Rectangle _bounds = Rectangle(x,y,w,h);
	return _bounds;
}
//-----------------------------------------------------------------------------
Rectangle CocoaWindowImpl::getBounds() const {
	if (getFlag(WindowFlags::WND_NESTED)) {
		Rectangle res = bounds;
		res.x0(Point2D(0,0));
		return res;
	}
	return bounds;
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::setBounds(const Rectangle &r) {
	Rectangle curr = getBounds();
	if (curr == r)
		return;
	bounds = r;

	if (bounds.getWidth() < 1.)
		bounds.setWidth(1.);
	if (bounds.getHeight() < 1.)
		bounds.setHeight(1.);
    updateBoundsToWindow();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::updateTitle() {
	if (!isVisible()) {
		return;
	}
	Impl::setTitle(getTitle());
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::setTitle(const std::string &_title) {
	title = _title;
	updateTitle();
}
//-----------------------------------------------------------------------------
std::string CocoaWindowImpl::getTitle() const {
	return title;
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::invalidateWindow(const Rectangle &area) {
	Impl::invalidateWindow(area.x(),
			area.y(),
			area.width(),
			area.height());
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__handleMouseButtonPressEvent(Nb x, Nb y, Nb buttons)
{
	handleMouseButtonPressEvent(x,y,buttons);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__handleMouseButtonReleaseEvent(Nb x, Nb y, Nb buttons)
{
	handleMouseButtonReleaseEvent(x,y,buttons);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__handleMouseMotionEvent(Nb x, Nb y) {
	handleMouseMotionEvent(x,y);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__handleMouseWheelEvent(Nb x, Nb y, Nb wheelRotation) {
    handleMouseWheelEvent(x,y,wheelRotation);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__windowWillCose() {
    visible = false;
    onDestroy();
}

}}} // namespace(s)

#endif // DISCO_USE_COCOA
