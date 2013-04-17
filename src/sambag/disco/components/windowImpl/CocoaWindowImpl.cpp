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
void CocoaWindowImpl::close() {
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::_open(AWindowImplPtr parent) {
	Impl::openWindow();
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
	//TODO: implement
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
//-------------------------------------------------------------------------
std::string CocoaWindowImpl::getTitle() const {
	return title;
}
//-----------------------------------------------------------------------------
void CocoaWindowImpl::invalidateWindow(const Rectangle &area) {
}
}}} // namespace(s)

#endif // DISCO_USE_COCOA
