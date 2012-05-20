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
const std::string AComponent::PROPERTY_NAME = "name";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_FOREGROUND = "foreground";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_BACKGROUND = "background";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_PREFERREDSIZE = "preferredSize";
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
	return bounds.getHeight();
}
//-----------------------------------------------------------------------------
Point2D AComponent::getLocation() const {
	return bounds.x0();
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
	return prefSize;
}
//-----------------------------------------------------------------------------
Dimension AComponent::getSize() const {
	return Dimension(bounds.getWidth(), bounds.getHeight());
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getWidth() const {
	return bounds.getWidth();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getX() const {
	return bounds.x0().x();
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getY() const {
	return bounds.x0().y();
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr AComponent::getIDrawContext() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return IDrawContext::Ptr();
}
//-----------------------------------------------------------------------------
ColorRGBA AComponent::getBackground() const {
	ColorRGBA background = this->background;
	if (background != ColorRGBA::NULL_COLOR) {
		return background;
	}
	AContainer::Ptr parent = this->parent;
	return (parent) ? parent->getBackground() : ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
ColorRGBA AComponent::getForeground() const {
	ColorRGBA foreground = this->foreground;
	if (foreground != ColorRGBA::NULL_COLOR) {
		return foreground;
	}
	AContainer::Ptr parent = this->parent;
	return (parent) ? parent->getForeground() : ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
AComponent::Lock & AComponent::getTreeLock() {
	return treeLock;
}
//-----------------------------------------------------------------------------
AContainerPtr AComponent::getRootContainer() const {
	AContainer::Ptr p = parent;
	while (p) {
		p = p->getParent();
	}
	return p;
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
void AComponent::repaintParentIfNeeded(const Rectangle &r) {
	if (parent && isShowing()) {
		// Have the parent redraw the area this component occupied.
		parent->repaint(r);
		// Have the parent redraw the area this component *now* occupies.
		repaint();
	}
}
//-----------------------------------------------------------------------------
void AComponent::invalidateParent() {
	if (parent) {
		parent->invalidateIfValid();
	}
}
//-----------------------------------------------------------------------------
void AComponent::invalidateIfValid() {
	if (isValid()) {
		invalidate();
	}
}
//-----------------------------------------------------------------------------
bool AComponent::isPreferredSizeSet() const {
	return prefSizeSet;
}
//-----------------------------------------------------------------------------
void AComponent::repaint(const Rectangle &r) {
	if (!parent)
		return;
	// Needs to be translated to parent coordinates since
	// a parent native container provides the actual repaint
	// services.  Additionally, the request is restricted to
	// the bounds of the component.
	Number x = r.x0().x(), y = r.x0().y(), width = r.getWidth(), height =
			r.getHeight();
	Number thisX = bounds.x0().x(), thisY = bounds.x0().y(), thisWidth =
			bounds.getWidth(), thisHeight = bounds.getHeight();
	if (x < 0) {
		width += x;
		x = 0;
	}
	if (y < 0) {
		height += y;
		y = 0;
	}

	Number pwidth = (width > thisWidth) ? thisWidth : width;
	Number pheight = (height > thisHeight) ? thisHeight : height;

	if (pwidth <= 0 || pheight <= 0) {
		return;
	}

	Number px = thisX + x;
	Number py = thisY + y;
	parent->repaint(Rectangle(px, py, pwidth, pheight));

}
//-----------------------------------------------------------------------------
void AComponent::setBounds(const Rectangle &r) {
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		bool resized = (bounds.getWidth() != r.getWidth())
				|| (bounds.getHeight() != r.getHeight());
		bool moved = bounds.x0() != r.x0();
		if (!resized && !moved) {
			return;
		}
		Rectangle oldBounds = bounds;
		bounds = r;
		if (resized) {
			isPacked = false;
			invalidate();
		}
		if (parent) {
			parent->invalidateIfValid();
		}
		// TODO: events
		// notifyNewBounds(resized, moved);
		repaintParentIfNeeded(oldBounds);
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::setPreferredSize(const Dimension &preferredSize) {
	Dimension old = prefSize;
	// If the preferred size was set, use it as the old value, otherwise
	// use null to indicate we didn't previously have a set preferred
	// size.
	prefSize = preferredSize;
	prefSizeSet = true;
	EventSender<PropertyChanged>::notifyListeners(this,
			PropertyChanged(PROPERTY_PREFERREDSIZE, old, preferredSize));
}
//-----------------------------------------------------------------------------
void AComponent::clearMostRecentFocusOwnerOnHide() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::transferFocus(bool clearOnFailure) {
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
		clearMostRecentFocusOwnerOnHide();
		enabled = false;
		/* // TODO: focus
		 // A disabled lw container is allowed to contain a focus owner.
		 if ((isFocusOwner() || (containsFocus() )) &&
		 KeyboardFocusManager.isAutoFocusTransferEnabled())
		 {
		 // Don't clear the global focus owner. If transferFocus
		 // fails, we want the focus to stay on the disabled
		 // Component so that keyboard traversal, et. al. still
		 // makes sense to the user.
		 transferFocus(false);
		 }*/
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::show() {
	if (visible)
		return;
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		visible = true;
		//TODO:
		//mixOnShowing();

		//TODO: EVENT
		/*createHierarchyEvents(HierarchyEvent.HIERARCHY_CHANGED, this,
		 parent, HierarchyEvent.SHOWING_CHANGED,
		 Toolkit.enabledOnToolkit(AWTEvent.HIERARCHY_EVENT_MASK));*/

		repaint();
		//TODO: EVENT
		/*ComponentEvent e = new ComponentEvent(this,
		 ComponentEvent.COMPONENT_SHOWN);*/
		//Toolkit.getEventQueue().postEvent(e);
		AContainer::Ptr parent = this->parent;
		if (parent)
			parent->invalidate();

	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::hide() {
	isPacked = false;
	if (!visible)
		return;
	clearMostRecentFocusOwnerOnHide();
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		visible = false;
		// TODO:
		//mixOnHiding();
		/* TODO: Focus
		 if (containsFocus() && KeyboardFocusManager.isAutoFocusTransferEnabled()) {
		 transferFocus(true);
		 }*/
		/* TODO: Events
		 createHierarchyEvents(HierarchyEvent.HIERARCHY_CHANGED,
		 this, parent,
		 HierarchyEvent.SHOWING_CHANGED,
		 Toolkit.enabledOnToolkit(AWTEvent.HIERARCHY_EVENT_MASK));
		 */
		repaint();
		/* TODO: Events
		 * if (componentListener != null ||
		 (eventMask & AWTEvent.COMPONENT_EVENT_MASK) != 0 ||
		 Toolkit.enabledOnToolkit(AWTEvent.COMPONENT_EVENT_MASK)) {
		 ComponentEvent e = new ComponentEvent(this,
		 ComponentEvent.COMPONENT_HIDDEN);
		 Toolkit.getEventQueue().postEvent(e);
		 }
		 */
		AContainer::Ptr parent = this->parent;
		if (parent)
			parent->invalidate();

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
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		setBounds(p.x(), p.y(), bounds.getWidth(), bounds.getHeight());
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::setName(const std::string &name) {
	std::string old = name;
	SAMBAG_BEGIN_SYNCHRONIZED(getObjectLock())
		AComponent::name = name;
		nameExplicitlySet = true;
	SAMBAG_END_SYNCHRONIZED
	EventSender<PropertyChanged>::notifyListeners(this,
			PropertyChanged(PROPERTY_NAME, old, name));
}
//-----------------------------------------------------------------------------
void AComponent::setSize(const Dimension &d) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::setVisible(bool b) {
	if (b)
		show();
	else
		hide();
}
//-----------------------------------------------------------------------------
void AComponent::setBackground(const ColorRGBA &c) {
	ColorRGBA oldColor = background;
	background = c;
	// This is a bound property, so report the change to
	// any registered listeners.  (Cheap if there are none.)
	EventSender<PropertyChanged>::notifyListeners(this,
			PropertyChanged(PROPERTY_BACKGROUND, oldColor, c));
}
//-----------------------------------------------------------------------------
void AComponent::setForeground(const ColorRGBA &c) {
	ColorRGBA oldColor = foreground;
	foreground = c;
	// This is a bound property, so report the change to
	// any registered listeners.  (Cheap if there are none.)
	EventSender<PropertyChanged>::notifyListeners(this,
			PropertyChanged(PROPERTY_FOREGROUND, oldColor, c));
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
