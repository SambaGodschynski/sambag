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
void CocoaWindowImpl::__processDraw(CGContextRef context, int x, int y, int w, int h)
{
	QuartzSurface::Ptr sf = QuartzSurface::create(context,w,h);
	SAMBAG_ASSERT(sf);
	this->processDraw(sf);
	//cairo_surface_flush(sf->getCairoSurface());
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::startMainApp() {
	Impl::startMainApp();
}
//-----------------------------------------------------------------------------
CocoaWindowImpl::CocoaWindowImpl() :
	bounds(Rectangle(0,0,1,1)),
	visible(false)
{
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::initAsNestedWindow(ArbitraryType::Ptr osParent,
	const Rectangle &area)
{
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::_close() {
	Impl::closeWindow();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::close() {
	getWindowToolkit()->invokeLater(
		boost::bind(&CocoaWindowImpl::_close, this)
	);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::_open(AWindowImplPtr parent) {
	Impl *parentImpl = dynamic_cast<Impl*>(parent.get());
	Impl::openWindow(
			parentImpl,
			(int)bounds.x(),
			(int)bounds.y(),
			(int)bounds.width(),
			(int)bounds.height()
	);
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
	Impl::setBounds(bounds.x(), bounds.y(), bounds.width(), bounds.height());
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__boundsChanged(int x, int y, int w, int h) {
	updateWindowToBounds(Rectangle(x,y,w,h));
}
//-----------------------------------------------------------------------------
void * CocoaWindowImpl::getSystemHandle() const {
	return NULL;
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
	return Rectangle();
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

	if (!isVisible())
		return;

	updateBoundsToWindow();
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::setTitle(const std::string &_title) {
	title = _title;
	if (!isVisible())
		return;
	//updateTitle();
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
void CocoaWindowImpl::__handleMouseButtonPressEvent(int x, int y, int buttons)
{
	handleMouseButtonPressEvent(x,y,buttons);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__handleMouseButtonReleaseEvent(int x, int y, int buttons)
{
	handleMouseButtonReleaseEvent(x,y,buttons);
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::__handleMouseMotionEvent(int x, int y) {
	handleMouseMotionEvent(x,y);
}
}}} // namespace(s)

#endif // DISCO_USE_COCOA
