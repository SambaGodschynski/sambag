/*
 * AComponent.cpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#include "AComponent.hpp"
#include "AContainer.hpp"
#include <sambag/com/Common.hpp> // for SAMBA_LOG_NOT_YET_IMPL
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
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
const std::string AComponent::PROPERTY_MAXIMUMSIZE = "maximumSize";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_MINIMUMSIZE = "minimumSize";
//-----------------------------------------------------------------------------
AComponent::AComponent() {

}
//-----------------------------------------------------------------------------
AComponent::~AComponent() {

}
//-----------------------------------------------------------------------------
bool AComponent::contains(Point2D p) const {
	const Coordinate &width = bounds.getWidth();
	const Coordinate &height = bounds.getHeight();
	return (p.x() >= 0) && (p.x() < width) && (p.y() >= 0) && (p.y() < height);
}
//-----------------------------------------------------------------------------
int AComponent::dispatchHierarchyEvents(HierarchyEvent::Type id,
		AComponentPtr changed, AContainerPtr changedParent, long changeFlags) {
	switch (id) {
	case HierarchyEvent::HIERARCHY_CHANGED: {
		HierarchyEvent e(self.lock(), id, changed, changedParent, changeFlags);
		dispatchEvent(e);
		return 1;
	}
	case HierarchyEvent::ANCESTOR_MOVED:
	case HierarchyEvent::ANCESTOR_RESIZED: {
		HierarchyEvent e(self.lock(), id, changed, changedParent);
		dispatchEvent(e);
		return 1;
	}
	default:
		SAMBAG_ASSERT(false);
		break;
	}
	return 0;
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
	return CENTER_ALIGNMENT;
}
//-----------------------------------------------------------------------------
Coordinate AComponent::getAlignmentY() const {
	return CENTER_ALIGNMENT;
}
//-----------------------------------------------------------------------------
AComponent::Ptr AComponent::getComponentAt(const Point2D &p) const {
	return contains(p) ? getPtr() : AComponent::Ptr();
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
Dimension AComponent::getMaximumSize() {
	Dimension dim = maxSize;
	if (dim == NULL_DIMENSION || !(isMinimumSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			maxSize = getSize();
			dim = maxSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
}
//-----------------------------------------------------------------------------
Dimension AComponent::getMinimumSize() {
	Dimension dim = minSize;
	if (dim == NULL_DIMENSION || !(isMinimumSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			minSize = getSize();
			dim = minSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
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
Dimension AComponent::getPreferredSize() {
	Dimension dim = prefSize;
	if (dim == NULL_DIMENSION || !(isPreferredSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
			prefSize = getMinimumSize();
			dim = prefSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
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
IDrawContext::Ptr AComponent::getDrawContext() const {
	// need to translate coordinate spaces and clip relative
	// to the parent.
	if (!parent)
		return IDrawContext::Ptr();
	IDrawContext::Ptr cn = parent->getDrawContext();
	if (!cn)
		return IDrawContext::Ptr();
	cn->translate(bounds.x0());
	cn->setClip(Rectangle(0, 0, bounds.getWidth(), bounds.getHeight()));
	return cn;

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
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		/* Nullify cached layout and size information.
		 * For efficiency, propagate invalidate() upwards only if
		 * some other component hasn't already done so first.
		 */
		valid = false;
		if (!isPreferredSizeSet()) {
			prefSize = NULL_DIMENSION;
		}
		if (!isMinimumSizeSet()) {
			minSize = NULL_DIMENSION;
		}
		if (!isMaximumSizeSet()) {
			maxSize = NULL_DIMENSION;
		}
		invalidateParent();
	SAMBAG_END_SYNCHRONIZED
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
	return maxSizeSet;
}
//-----------------------------------------------------------------------------
bool AComponent::isMinimumSizeSet() const {
	return minSizeSet;
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
void AComponent::revalidate() {
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		invalidate();
		AContainer::Ptr root = getContainer();
		if (!root) {
			// There's no parents. Just validate itself.
			validate();
		} else {
			while (!root->isValidateRoot()) {
				if (!root->getContainer()) {
					// If there's no validate roots, we'll validate the
					// topmost container
					break;
				}
				root = root->getContainer();
			}
			root->validate();
		}SAMBAG_END_SYNCHRONIZED
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
	dispatchEvent(PropertyChanged(PROPERTY_PREFERREDSIZE, old, preferredSize));
}
//-----------------------------------------------------------------------------
void AComponent::setMaximumSize(const Dimension &s) {
	Dimension old = maxSize;
	maxSize = s;
	maxSizeSet = true;
	dispatchEvent(PropertyChanged(PROPERTY_MAXIMUMSIZE, old, s));
}
//-----------------------------------------------------------------------------
void AComponent::setMinimumSize(const Dimension &s) {
	Dimension old = minSize;
	minSize = s;
	minSizeSet = true;
	dispatchEvent(PropertyChanged(PROPERTY_MINIMUMSIZE, old, s));
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

		dispatchHierarchyEvents(HierarchyEvent::HIERARCHY_CHANGED, self.lock(),
				parent, HierarchyEvent::SHOWING_CHANGED);

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
		dispatchHierarchyEvents(HierarchyEvent::HIERARCHY_CHANGED, self.lock(),
				parent, HierarchyEvent::SHOWING_CHANGED);
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
	ignoreRepaint = b;
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
	dispatchEvent(PropertyChanged(PROPERTY_NAME, old, name));
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
	dispatchEvent(PropertyChanged(PROPERTY_BACKGROUND, oldColor, c));
}
//-----------------------------------------------------------------------------
void AComponent::setForeground(const ColorRGBA &c) {
	ColorRGBA oldColor = foreground;
	foreground = c;
	// This is a bound property, so report the change to
	// any registered listeners.  (Cheap if there are none.)
	dispatchEvent(PropertyChanged(PROPERTY_FOREGROUND, oldColor, c));
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
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		bool wasValid = isValid();
		if (!wasValid) {
			doLayout();
		}
		valid = true;
		if (!wasValid) {
			//TODO: mixOnValidating();
		}SAMBAG_END_SYNCHRONIZED
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
	if (width < 0 || height < 0) {
		SAMBAG_THROW( sambag::com::exceptions::IllegalArgumentException,
				"Width and height must be >= 0"
		);
	}
	return -1;
}
//-----------------------------------------------------------------------------
IBorder::Ptr AComponent::getBorder() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return IBorder::Ptr();
}
//-------------------------------------------------------------------------
bool AComponent::getIgnoreRepaint() const {
	return ignoreRepaint;
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
