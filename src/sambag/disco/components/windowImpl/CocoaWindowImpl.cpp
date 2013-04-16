/*
 * CocoaWindowImpl.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#include "CocoaWindowImpl.hpp"
#include "cocoaimpl/_CocoaWindowImpl.h"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class CocoaWindowImpl
//=============================================================================
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
void CocoaWindowImpl::open(AWindowImplPtr parent) {
	Impl::openWindow();
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

	//updateBoundsToWindow();
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
