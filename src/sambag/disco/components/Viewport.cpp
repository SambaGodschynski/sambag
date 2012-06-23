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
}
//----------------------------------------------------------------------------
ui::AComponentUIPtr Viewport::getComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<Viewport>();
}
//-----------------------------------------------------------------------------
void Viewport::addImpl(AComponentPtr child, int index) {
	SAMBA_LOG_NOT_YET_IMPL();
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
	SAMBA_LOG_NOT_YET_IMPL();
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
