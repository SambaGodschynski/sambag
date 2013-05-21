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
	void *_win = NULL;
	com::get(osParent, _win);
	WindowRef win = (WindowRef)_win;
	if (!win) {
		SAMBAG_LOG_WARN<<"initAsNestedWindow failed because parent==NULL";
		return;
	}
	Impl::openNested(win, area.x(), area.y(), area.width(), area.height());
    
    //get real window bounds (in case of vst area location dosen't fit with the
    // window
    Nb x=0, y=0, h=0, w=0;
    Impl::getBounds(x,y,w,h);
    bounds = Rectangle(x,y,w,h);
	visible = true;
    onCreated();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::_close() {
    if (!isVisible()) {
        return;
    }
    if (carbonWorkaround) {
        carbonWorkaround->closeWindow();
        visible = false;
        return;
    }
	Impl::closeWindow();
    visible = false;
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::close() {
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
    
    /*if (getFlag(WindowFlags::WND_RAW)) {
        carbonWorkaround = carbonWorkaround::createWindow(bounds, carbonWorkaround::IWindow::SYSTEM_MENU | carbonWorkaround::IWindow::TITLE_BAR);
        carbonWorkaround->showWindow();
        visible = true;
        updateTitle();
        onCreated();
        return;
    }*/
    
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
		boost::bind(&CocoaWindowImpl::_open, this, parent)
	);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::updateWindowToBounds(const Rectangle &r) {
	bounds = r;
	boundsUpdated();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::updateBoundsToWindow() {
    if (carbonWorkaround) {
        carbonWorkaround->setSize(bounds);
        return;
    }
	Impl::setBounds(bounds.x(), bounds.y(), bounds.width(), bounds.height());
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__boundsChanged(Nb x, Nb y, Nb w, Nb h) {
    std::cout<<std::hex<<this<<std::dec<<"CocoaWindowImpl::__boundsChanged"<<x<<", "<<y<<", "<<w<<", "<<h<<std::endl;
	updateWindowToBounds(Rectangle(x,y,w,h));
}
//-----------------------------------------------------------------------------
void * CocoaWindowImpl::getSystemHandle() const {
    if (carbonWorkaround) {
        return carbonWorkaround->getHandle();
    }
	return Impl::getWindowRef();
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
	return bounds;
}
//-----------------------------------------------------------------------------
Rectangle CocoaWindowImpl::getBounds() const {
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
    if (carbonWorkaround) {
        return carbonWorkaround->setCaption(getTitle());
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
    if (carbonWorkaround) {
        return;
    }
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
void CocoaWindowImpl::__windowWillCose() {
    onDestroy();
}

}}} // namespace(s)

#endif // DISCO_USE_COCOA
