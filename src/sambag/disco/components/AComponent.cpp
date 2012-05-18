/*
 * AComponent.cpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#include "AComponent.hpp"
#include "AContainer.hpp"
#include <sambag/com/Common.hpp> // for SAMBA_LOG_NOT_YET_IMPL
// (\w+\(.*?\)) -> fname()
// /\*\*(\R.*?)*\*/\R -> doxycomment

namespace sambag {
namespace disco {
namespace components {
//=============================================================================
// class AComponent
//=============================================================================
//-----------------------------------------------------------------------------
AComponent::AComponent() {

}
//-----------------------------------------------------------------------------
AComponent::~AComponent() {

}
//-----------------------------------------------------------------------------
Rectangle AComponent::getBoundingBox() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Rectangle();
}
//-----------------------------------------------------------------------------
std::string AComponent::toString() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return std::string();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getAlignmentX() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getAlignmentY() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
AComponent::Ptr AComponent::getComponentAt(const Coordinate & x,
		const Coordinate & y) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Ptr();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getHeight() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
Point2D AComponent::getLocation() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Point2D();
}
//-----------------------------------------------------------------------------
Dimension AComponent::getMaximumSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Dimension AComponent::getMinimumSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
std::string AComponent::getName() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return std::string();
}
//-----------------------------------------------------------------------------
AContainerPtr AComponent::getParent() const {
	return parent;
}
//-----------------------------------------------------------------------------
Dimension AComponent::getPreferredSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Dimension AComponent::getSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getWidth() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getX() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getY() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr AComponent::getIDrawContext() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return IDrawContext::Ptr();
}
//-----------------------------------------------------------------------------
ColorRGBA AComponent::getBackground() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
ColorRGBA AComponent::getForeground() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
AComponent::Lock & AComponent::getTreeLock() {
	return treeLock;
}
//-----------------------------------------------------------------------------
bool AComponent::hasFocus() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
void AComponent::invalidate() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
bool AComponent::isDisplayable() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isEnabled() const {
	return enabled;
}
//-----------------------------------------------------------------------------
bool AComponent::isFocusable() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isFocusOwner() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isOpaque() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isShowing() const {
	if (visible) {
		return (parent) || parent->isShowing();
	}
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isValid() const {
	return valid;
}
//-----------------------------------------------------------------------------
bool AComponent::isVisible() const {
	return visible;
}
//-----------------------------------------------------------------------------
bool AComponent::isRecursivelyVisible() const {
	if (!parent)
		return visible;
	return visible && parent->isRecursivelyVisible();
}
//-----------------------------------------------------------------------------
bool AComponent::isMaximumSizeSet() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isMinimumSizeSet() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
bool AComponent::isBackgroundSet() const {
	return background != ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
bool AComponent::isForegroundSet() const {
	return foreground != ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
void AComponent::draw(IDrawContext::Ptr context) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::drawAll(IDrawContext::Ptr context) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::redraw() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::requestFocus() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setBounds(const Rectangle &r) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::enable() {
	if (enabled)
		return;
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		enabled = true;
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::disable() {
	if (!enabled)
		return;
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		enabled = false;
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::setEnabled(bool b) {
	if (b) {
		enable();
	} else {
		disable();
	}
}
//-----------------------------------------------------------------------------
void AComponent::setFocusable(bool b) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setIgnoreRepaint(bool b) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setLocation(const Point2D &p) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setName(const std::string &name) {
	std::string old = name;
	SAMBAG_BEGIN_SYNCHRONIZED(getObjectLock())
		AComponent::name = name;
		nameExplicitlySet = true;
	SAMBAG_END_SYNCHRONIZED
	EventSender<PropertyChanged>::notifyListeners(this,
			PropertyChanged("name", old, name));
}
//-----------------------------------------------------------------------------
void AComponent::setSize(const Dimension &d) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setVisible(bool b) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setBackground(const ColorRGBA &c) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setForeground(const ColorRGBA &c) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::transferFocus() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::transferFocusBackward() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::validate() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::update(IDrawContext::Ptr cn) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
ActionMap::Ptr AComponent::getActionMap() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ActionMap::Ptr();
}
//-----------------------------------------------------------------------------
bool AComponent::getAutoscrolls() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getBaseLine(const Coordinate &width,
		const Coordinate &height) {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
IBorder::Ptr AComponent::getBorder() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return IBorder::Ptr();
}
//-----------------------------------------------------------------------------
void AComponent::grabFocus() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-------------------------------------------------------------------------
AComponent::Lock & AComponent::getObjectLock() {
	return objectLock;
}
//-----------------------------------------------------------------------------
void AComponent::drawBorder(IDrawContext::Ptr context) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::drawComponent(IDrawContext::Ptr context) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setActionMap(ActionMap::Ptr am) const {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setAutoscrolls(bool b) const {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setBorder(IBorder::Ptr br) const {
	SAMBA_LOG_NOT_YET_IMPL();
}

}
}
} // namespace(s)
