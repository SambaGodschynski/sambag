/*
 * AComponent.cpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#include "AComponent.hpp"
#include "AContainer.hpp"
#include <sambag/com/Common.hpp>
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>
#include "Window.hpp"
#include "ui/AComponentUI.hpp"
#include "Graphics.hpp"
#include <boost/tuple/tuple.hpp>
#include "RedrawManager.hpp"
#include "RootPane.hpp"

namespace sambag { namespace disco { namespace components {
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
const std::string AComponent::PROPERTY_FOCUSABLE = "focusable";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_UI = "ui";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_BORDER = "border";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_ENABLED = "enabled";
//-----------------------------------------------------------------------------
const std::string  AComponent::PROPERTY_CLIENTPROPERTY = "client_property";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_BOUNDS = "size";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_FONT = "font";
//-----------------------------------------------------------------------------
const std::string AComponent::PROPERTY_OPAQUE = "opaque";
//-----------------------------------------------------------------------------
const float AComponent::TOP_ALIGNMENT = 0.0f;
//-----------------------------------------------------------------------------
const float AComponent::CENTER_ALIGNMENT = 0.5f;
//-----------------------------------------------------------------------------
const float AComponent::BOTTOM_ALIGNMENT = 1.0f;
//-----------------------------------------------------------------------------
const float AComponent::LEFT_ALIGNMENT = 0.0f;
//-----------------------------------------------------------------------------
const float AComponent::RIGHT_ALIGNMENT = 1.0f;
//-----------------------------------------------------------------------------
AComponent::AComponent() : flags(0) {

}
//-----------------------------------------------------------------------------
AComponent::~AComponent() {

}
//-----------------------------------------------------------------------------
bool AComponent::isTreeLocked() {
	// TODO
	return true;
}
//-----------------------------------------------------------------------------
void AComponent::checkTreeLock() {
	if (!isTreeLocked())
		SAMBAG_THROW(sambag::com::exceptions::IllegalStateException,
				"This function should be called while holding treeLock");
}
//-----------------------------------------------------------------------------
bool AComponent::contains(Point2D p) const {
	if (ui)
		return ui->contains(getPtr(), p);
	const Coordinate &width = bounds.getWidth();
	const Coordinate &height = bounds.getHeight();
	return (p.x() >= 0) && (p.x() < width) && (p.y() >= 0) && (p.y() < height);
}
//-----------------------------------------------------------------------------
int AComponent::__dispatchHierarchyEvents_(events::HierarchyEvent::Type id,
		AComponentPtr changed, AContainerPtr changedParent, long changeFlags) {
	using namespace events;
	switch (id) {
	case HierarchyEvent::HIERARCHY_CHANGED: {
		HierarchyEvent e(self.lock(), id, changed, changedParent, changeFlags);
		dispatchEvent(e, this);
		return 1;
	}
	case HierarchyEvent::ANCESTOR_MOVED:
	case HierarchyEvent::ANCESTOR_RESIZED: {
		HierarchyEvent e(self.lock(), id, changed, changedParent);
		dispatchEvent(e, this);
		return 1;
	}
	default:
		SAMBAG_ASSERT(false);
		break;
	}
	return 0;
}
//-----------------------------------------------------------------------------
const Rectangle & AComponent::getBounds() const {
	return bounds;
}
//-----------------------------------------------------------------------------
std::string AComponent::parameterString() const {
	std::stringstream os;
	os << getName() << "," << bounds.x0().x() << "," << bounds.x0().y();
	os << "," << bounds.getWidth() << "x" << bounds.getHeight();
	if (!isValid()) {
		os << ",invalid";
	}
	if (!visible) {
		os << ",hidden";
	}
	if (!enabled) {
		os << ",disabled";
	}
	return os.str();
}

//-----------------------------------------------------------------------------
std::string AComponent::toString() const {
	std::stringstream ss;
	ss << "[" << parameterString() << "]";
	return ss.str();
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
const Point2D & AComponent::getLocation() const {
	return bounds.x0();
}
//-----------------------------------------------------------------------------
Dimension AComponent::getMaximumSize() {
	Dimension dim;
	if (ui) {
		dim = ui->getMaximumSize(getPtr());
		if (dim!=NULL_DIMENSION)
			return dim;
	}
	if (isMaximumSizeSet()) {
		return maxSize;
	}
	return Dimension(9999.,9999.);
}
//-----------------------------------------------------------------------------
Dimension AComponent::getMinimumSize() {
	Dimension dim;
	if (ui) {
		dim = ui->getMinimumSize(getPtr());
		if (dim!=NULL_DIMENSION)
			return dim;
	}
	dim = minSize;
	if (dim == NULL_DIMENSION || !(isMinimumSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			minSize = getSize();
			dim = minSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
}
//-----------------------------------------------------------------------------
const std::string & AComponent::getName() const {
	return name;
}
//-----------------------------------------------------------------------------
AContainerPtr AComponent::getParent() const {
	return _parent.lock();
}
//-----------------------------------------------------------------------------
Dimension AComponent::getPreferredSize() {
	Dimension dim;
	if (ui) {
		dim = ui->getPreferredSize(getPtr());
		if (dim!=NULL_DIMENSION)
			return dim;
	}
	dim = prefSize;
	if (dim == NULL_DIMENSION || !(isPreferredSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
			prefSize = AComponent::getMinimumSize();
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
const Coordinate & AComponent::getX() const {
	return bounds.x0().x();
}
//-----------------------------------------------------------------------------
const Coordinate & AComponent::getY() const {
	return bounds.x0().y();
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr AComponent::getDrawContext() const {
	// need to translate coordinate spaces and clip relative
	// to the parent.
	AContainerPtr parent = getParent();
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
	AContainerPtr parent = getParent();
	return (parent) ? parent->getBackground() : ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
ColorRGBA AComponent::getForeground() const {
	ColorRGBA foreground = this->foreground;
	if (foreground != ColorRGBA::NULL_COLOR) {
		return foreground;
	}
	AContainerPtr parent = getParent();
	return (parent) ? parent->getForeground() : ColorRGBA::NULL_COLOR;
}
//-----------------------------------------------------------------------------
AComponent::Lock & AComponent::getTreeLock() const {
	return treeLock;
}
//-----------------------------------------------------------------------------
AContainerPtr AComponent::getRootContainer() const {
	AContainer::Ptr p = getParent();
	if (!p)
		return AContainerPtr();
	while (p->getParent()) {
		p = p->getParent();
	}
	return p;
}
//-----------------------------------------------------------------------------
Point2D AComponent::getLocationOnScreen(const Point2D &p) const {
	if (!isVisible())
		return NULL_POINT2D;
 	Point2D tmp = p;
 	boost::geometry::add_point(tmp, getLocation());
	AContainerPtr parent = getParent();
	if (!parent)
		return tmp;
	boost::geometry::add_point(tmp, parent->getLocationOnScreen(Point2D(0,0)));
	return tmp;
}
//-----------------------------------------------------------------------------
Point2D AComponent::getLocationOnComponent(const Point2D &p) const {
	if (!isVisible())
		return NULL_POINT2D;
 	Point2D tmp = p;
 	boost::geometry::subtract_point(tmp, getLocationOnScreen(Point2D(0,0)));
	return tmp;
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr
AComponent::getComponentDrawContext(IDrawContext::Ptr cn) const
{
	//    Graphics componentGraphics = g;
	//      if (ui != null && DEBUG_GRAPHICS_LOADED) {
	//          if ((DebugGraphics.debugComponentCount() != 0) &&
	//                  (shouldDebugGraphics() != 0) &&
	//                  !(g instanceof DebugGraphics)) {
	//              componentGraphics = new DebugGraphics(g,this);
	//          }
	//      }
	cn->setStrokeColor(getForeground());
	cn->setFillColor(getForeground());
	cn->setFont(getFont());
	return cn;
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
	AContainer::Ptr p = getParent();
	if (!p)
		return false;
	return p->isDisplayable();
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
	return getFlag(IS_OPAQUE);
}
//-----------------------------------------------------------------------------
void AComponent::setOpaque(bool isOpaque) {
	bool oldValue = getFlag(IS_OPAQUE);
	setFlag(IS_OPAQUE, isOpaque);
	setFlag(OPAQUE_SET, true);
	firePropertyChanged(PROPERTY_OPAQUE, oldValue, isOpaque);
}
//-----------------------------------------------------------------------------
bool AComponent::isShowing() const {
	if (visible) {
		AContainerPtr parent = getParent();
		if (parent)
			return parent->isShowing();
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
	AContainerPtr parent = getParent();
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
void AComponent::drawChildren(IDrawContext::Ptr context) {
}
//-----------------------------------------------------------------------------
void AComponent::redraw() {
	redraw(Rectangle (0, 0, getWidth(),  getHeight()));
}
//-----------------------------------------------------------------------------
void AComponent::redraw(const Rectangle &r) {
	RedrawManager::Ptr rd = RedrawManager::currentManager(getPtr());
	SAMBAG_ASSERT(rd);
	rd->addDirtyRegion(getPtr(), r);
}
//-----------------------------------------------------------------------------
void AComponent::requestFocus() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponent::drawBorder(IDrawContext::Ptr cn) {
	if (!border) {
		return;
	}
	border->paintBorder(getPtr(), cn, Rectangle(0, 0, getWidth(), getHeight()));
}
//-----------------------------------------------------------------------------
void AComponent::draw(IDrawContext::Ptr cn) {
	if ((getWidth() <= 0) || (getHeight() <= 0)) {
		return;
	}

	IDrawContext::Ptr componentGraphics = getComponentDrawContext(cn);
	ScratchGraphics co(componentGraphics);

	RedrawManager::Ptr repaintManager = RedrawManager::currentManager(getPtr());
	Rectangle clipRect = co.clipExtends();
	if (clipRect == NULL_RECTANGLE) {
		clipRect = Rectangle(0,0,getWidth(), getHeight());
	}

	if(clipRect.getWidth() > getWidth()) {
		clipRect.setWidth(getWidth());
	}
	if(clipRect.getHeight() > getHeight()) {
		clipRect.setHeight(getHeight());
	}
	if (repaintManager->isDoubleBufferingEnabled()) {
		repaintManager->draw(getPtr(), getPtr(), co.getPtr(), clipRect);
	}
	else {
		drawComponent(co.getPtr());
		drawBorder(co.getPtr());
	}
}
//-----------------------------------------------------------------------------
void AComponent::redrawParentIfNeeded(const Rectangle &r) {
	AContainerPtr parent = getParent();
	if (parent && isShowing()) {
		// Have the parent redraw the area this component occupied.
		parent->redraw(r);
		// Have the parent redraw the area this component *now* occupies.
		redraw();
	}
}
//-----------------------------------------------------------------------------
void AComponent::invalidateParent() {
	AContainerPtr parent = getParent();
	if (parent) {
		parent->__invalidateIfValid_();
	}
}
//-----------------------------------------------------------------------------
void AComponent::__invalidateIfValid_() {
	if (isValid()) {
		invalidate();
	}
}
//-----------------------------------------------------------------------------
bool AComponent::isPreferredSizeSet() const {
	return prefSizeSet;
}
//-----------------------------------------------------------------------------
void AComponent::revalidate() {
	if (!getParent()) {
		return;
	}
	invalidate();
	RedrawManager::currentManager(getPtr())->addInvalidComponent(getPtr());
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
		AContainerPtr parent = getParent();
		if (parent) {
			parent->__invalidateIfValid_();
		}
		firePropertyChanged(PROPERTY_BOUNDS, oldBounds, r);
		redrawParentIfNeeded(oldBounds);
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
	firePropertyChanged(PROPERTY_PREFERREDSIZE, old, preferredSize);
}
//-----------------------------------------------------------------------------
void AComponent::setMaximumSize(const Dimension &s) {
	Dimension old = maxSize;
	maxSize = s;
	maxSizeSet = true;
	firePropertyChanged(PROPERTY_MAXIMUMSIZE, old, s);
}
//-----------------------------------------------------------------------------
void AComponent::setMinimumSize(const Dimension &s) {
	Dimension old = minSize;
	minSize = s;
	minSizeSet = true;
	firePropertyChanged(PROPERTY_MINIMUMSIZE, old, s);
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
	using namespace events;
	if (visible)
		return;
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		visible = true;
		//TODO:
		//mixOnShowing();
		AContainerPtr parent = getParent();
		__dispatchHierarchyEvents_(HierarchyEvent::HIERARCHY_CHANGED, self.lock(),
				parent, HierarchyEvent::SHOWING_CHANGED);

		redraw();
		//TODO: EVENT
		/*ComponentEvent e = new ComponentEvent(this,
		 ComponentEvent.COMPONENT_SHOWN);*/
		//Toolkit.getEventQueue().postEvent(e);
		if (parent)
			parent->invalidate();

	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::hide() {
	using namespace events;
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
		AContainerPtr parent = getParent();
		__dispatchHierarchyEvents_(HierarchyEvent::HIERARCHY_CHANGED, self.lock(),
				parent, HierarchyEvent::SHOWING_CHANGED);
		redraw();
		/* TODO: Events
		 * if (componentListener != null ||
		 (eventMask & AWTEvent.COMPONENT_EVENT_MASK) != 0 ||
		 Toolkit.enabledOnToolkit(AWTEvent.COMPONENT_EVENT_MASK)) {
		 ComponentEvent e = new ComponentEvent(this,
		 ComponentEvent.COMPONENT_HIDDEN);
		 Toolkit.getEventQueue().postEvent(e);
		 }
		 */
		if (parent)
			parent->invalidate();

	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::setEnabled(bool b) {
	if (b==isEnabled())
		return;
	bool oldEnabled = isEnabled();
	if (b) {
		enable();
	} else {
		disable();
	}
	firePropertyChanged(PROPERTY_ENABLED, oldEnabled, (bool)enabled);
	if (enabled != oldEnabled) {
		redraw();
	}
}
//-----------------------------------------------------------------------------
void AComponent::setFocusable(bool b) {
	bool oldFocusable;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		oldFocusable = focusable;
		focusable = b;
	SAMBAG_END_SYNCHRONIZED
	//isFocusTraversableOverridden = FOCUS_TRAVERSABLE_SET;
	firePropertyChanged(PROPERTY_FOCUSABLE, oldFocusable, (bool) focusable);
	/* TODO: Focus
	 * if (oldFocusable && !focusable) {
	 if (isFocusOwner() && KeyboardFocusManager.isAutoFocusTransferEnabled()) {
	 transferFocus(true);
	 }
	 KeyboardFocusManager.clearMostRecentFocusOwner(this);
	 }*/
}
//-----------------------------------------------------------------------------
void AComponent::setIgnoreRepaint(bool b) {
	ignoreRepaint = b;
}
//-----------------------------------------------------------------------------
void AComponent::setLocation(const Point2D &p) {
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		Rectangle r = bounds;
		r.translate(p);
		setBounds(r);
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AComponent::setName(const std::string &name) {
	std::string old = AComponent::name;
	SAMBAG_BEGIN_SYNCHRONIZED(getObjectLock())
		AComponent::name = name;
		nameExplicitlySet = true;
	SAMBAG_END_SYNCHRONIZED
	firePropertyChanged(PROPERTY_NAME, old, name);
}
//-----------------------------------------------------------------------------
void AComponent::setSize(const Dimension &d) {
	setBounds(Rectangle(bounds.x0(), d.width(), d.height()));
}
//-----------------------------------------------------------------------------
void AComponent::setVisible(bool b) {
	if (b==isVisible())
		return;
	if (b)
		show();
	else
		hide();
	AContainerPtr parent = getParent();
	if (parent) {
		parent->redraw(getBounds());
	}
	// Some (all should) LayoutManagers do not consider components
	// that are not visible. As such we need to revalidate when the
	// visible bit changes.
	revalidate();
}
//-----------------------------------------------------------------------------
void AComponent::setBackground(const ColorRGBA &c) {
	ColorRGBA oldColor = background;
	background = c;
	// This is a bound property, so report the change to
	// any registered listeners.  (Cheap if there are none.)
	firePropertyChanged(PROPERTY_BACKGROUND, oldColor, c);
	if ((oldColor != ColorRGBA::NULL_COLOR) ? oldColor != c :
		((c != ColorRGBA::NULL_COLOR) && c!=oldColor))
	{
		redraw();
	}
}
//-----------------------------------------------------------------------------
void AComponent::setForeground(const ColorRGBA &c) {
	ColorRGBA oldColor = foreground;
	foreground = c;
	// This is a bound property, so report the change to
	// any registered listeners.  (Cheap if there are none.)
	firePropertyChanged(PROPERTY_FOREGROUND, oldColor, c);
	if ((oldColor != ColorRGBA::NULL_COLOR) ? oldColor != c :
		((c != ColorRGBA::NULL_COLOR) && c!=oldColor))
	{
		redraw();
	}
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
	SAMBAG_END_SYNCHRONIZED
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
	if (ui) {
		return ui->getBaseline(getPtr(), width, height);
	}
	return -1;
}
//-----------------------------------------------------------------------------
IBorder::Ptr AComponent::getBorder() const {
	return border;
}
//-----------------------------------------------------------------------------
bool AComponent::getIgnoreRepaint() const {
	return ignoreRepaint;
}
//-----------------------------------------------------------------------------
void AComponent::grabFocus() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
AComponent::Lock & AComponent::getObjectLock() const {
	return objectLock;
}
//-----------------------------------------------------------------------------
void AComponent::drawComponent(IDrawContext::Ptr context) {
	if (!ui)
		return;
	ScratchGraphics g(context);
	ui->update(g.getPtr(), getPtr());
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
void AComponent::setBorder(IBorder::Ptr br) {
	IBorder::Ptr oldBorder = border;
	border = br;
	firePropertyChanged(PROPERTY_BORDER, oldBorder, border);
	if (border != oldBorder) {
		if (border || oldBorder || !(border->getBorderInsets(
				getPtr()) == oldBorder->getBorderInsets(getPtr())))
		{
			revalidate();
		}
		redraw();
	}
}
//-----------------------------------------------------------------------------
ArbitraryType::Ptr AComponent::getClientProperty(const std::string &key) const
{
	PropertyMap::const_iterator it = propertyMap.find(key);
	if (it==propertyMap.end())
		return ArbitraryType::Ptr();
	return it->second;
}
//-----------------------------------------------------------------------------
void AComponent::putClientPropertyImpl(const std::string &key,
		ArbitraryType::Ptr value)
{
	PropertyMap::iterator it = propertyMap.find(key);
	if (it==propertyMap.end()) {
		if (!value)
			return;
		bool inserted;
		boost::tie(it, inserted) =
			propertyMap.insert(std::make_pair(key, value));
		SAMBAG_ASSERT(inserted);
		clientPropertyChanged(key, ArbitraryType::Ptr(), value);
		firePropertyChanged(PROPERTY_CLIENTPROPERTY, ArbitraryType::Ptr(), value);
		return;
	}
	ArbitraryType::Ptr old = it->second;
	if (!value) {
		propertyMap.erase(it);
		clientPropertyChanged(key, old, value);
		firePropertyChanged(PROPERTY_CLIENTPROPERTY, old, value);
		return;
	}
	it->second = value;
	clientPropertyChanged(key, old, value);
	firePropertyChanged(PROPERTY_CLIENTPROPERTY, old, value);
}
//-----------------------------------------------------------------------------
void AComponent::processMouseEvent(const events::MouseEvent &ev) {
	using namespace events;
	EventSender<MouseEvent>::notifyListeners(
		this,
		ev
	);
}
//-----------------------------------------------------------------------------
RootPanePtr AComponent::getTopLevelRootPane() const {
	AContainer::Ptr p = boost::shared_dynamic_cast<AContainer>(getPtr());
	if (!p)
		p = getParent();
	for (; p; p = p->getParent()) {
		RootPanePtr rt = boost::shared_dynamic_cast<RootPane>(p);
		if (rt)
			return rt;
	}
	return RootPanePtr();
}
//-----------------------------------------------------------------------------
WindowPtr AComponent::getTopLevelAncestor() const {
	AContainer::Ptr p = boost::shared_dynamic_cast<AContainer>(getPtr());
	if (!p)
		p = getParent();
	for (; p; p = p->getParent()) {
		WindowPtr w = boost::shared_dynamic_cast<Window>(p);
		if (w)
			return w;
	}
	return WindowPtr();
}
//-----------------------------------------------------------------------------
AContainerPtr AComponent::getValidateRoot() const {
	AContainer::Ptr c = getParent();
	for (; c; c = c->getParent()) {
		if (!c->isDisplayable() ) {
			return AContainer::Ptr();
		}
		if (c->isValidateRoot()) {
			return c;
		}
	}
	return AContainer::Ptr();
}
//-----------------------------------------------------------------------------
void AComponent::setUserUI (ui::AComponentUIPtr cui) {
	if (!cui)
		return;
	uiSettedByUser = true;
	installUI(cui);
}
//-----------------------------------------------------------------------------
void AComponent::removeUserUI () {
	ui::AComponentUI::Ptr old = ui;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		ui = ui::AComponentUI::Ptr();
		uiSettedByUser = false;
	SAMBAG_END_SYNCHRONIZED
	revalidate();
	redraw();
	firePropertyChanged(PROPERTY_UI, old, ui);
}
//-----------------------------------------------------------------------------
void AComponent::installUI(ui::AComponentUIPtr cui) {
	ui::AComponentUI::Ptr old = ui;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		ui = cui;
	SAMBAG_END_SYNCHRONIZED
	ui->installUI(getPtr());
	revalidate();
	redraw();
	firePropertyChanged(PROPERTY_UI, old, ui);
}
//-----------------------------------------------------------------------------
void AComponent::installLookAndFeel(ui::ALookAndFeelPtr laf) {
	if (isUiSettedByUser())
		return;
	if (!laf)
		return;
	ui::AComponentUI::Ptr cui = getComponentUI(laf);
	if (!cui)
		return;
	installUI(cui);
}
//-----------------------------------------------------------------------------
void AComponent::setFont(const Font &_font) {
	Font olf = font;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		font = _font;
	SAMBAG_END_SYNCHRONIZED
	firePropertyChanged(PROPERTY_FONT, olf, font);
}
//-----------------------------------------------------------------------------
const Font & AComponent::getFont() const {
	return font;
}
//-----------------------------------------------------------------------------
Font & AComponent::getFont() {
	return font;
}
//-----------------------------------------------------------------------------
void AComponent::printComponentTree(std::ostream &ss) const {
	ss << toString() << std::endl;
}
//-----------------------------------------------------------------------------
void AComponent::computeVisibleRect(AComponent::Ptr c, Rectangle &out) {
	using namespace boost;
	AContainer::Ptr p = c->getParent();
	Rectangle bounds = c->getBounds();

	if (!p) {
		out = Rectangle(0, 0, bounds.getWidth(), bounds.getHeight());
	} else {
		computeVisibleRect(p, out);
		geometry::subtract_point(out.x0(), bounds.x0());
		geometry::intersection<Rectangle::Base, Rectangle::Base, Rectangle::Base>
		(
				Rectangle(0,0,bounds.getWidth(), bounds.getHeight()),
				out,
				out
		);
	}
}
//-----------------------------------------------------------------------------
Rectangle AComponent::getVisibleRect() const {
	Rectangle res;
	computeVisibleRect(res);
	return res;
}
//-----------------------------------------------------------------------------
void AComponent::drawToOffscreen(IDrawContext::Ptr cn,
		const Rectangle &rect, const Point2D &max)
{

	Coordinate x = rect.x0().x(),
		y = rect.x0().y(),
		w = rect.width(),
		h = rect.height(),
		maxX = max.x(),
		maxY = max.y();

	setFlag(ANCESTOR_USING_BUFFER, true);
	if ((y + h) < maxY || (x + w) < maxX) {
		setFlag(IS_PAINTING_TILE, true);

		if (getFlag(IS_REPAINTING)) {
			// Called from paintImmediately (RepaintManager) to fill
			// repaint request
			draw(cn);
		} else {
			// Called from paint() to repair damage
			if (!rectangleIsObscured(rect)) {
				drawComponent(cn);
				drawBorder(cn);
			}
			drawChildren(cn);
		}
	}
	setFlag(ANCESTOR_USING_BUFFER, false);
	setFlag(IS_PAINTING_TILE, false);

}
//-----------------------------------------------------------------------------
void AComponent::drawForceDoubleBuffered(IDrawContext::Ptr cn) {
	RedrawManager::Ptr rm = RedrawManager::currentManager(getPtr());
	Rectangle clip = cn->clipExtends();
	setFlag(IS_REPAINTING, true);
	rm->draw(getPtr(), getPtr(), cn, clip);
	setFlag(IS_REPAINTING, false);
}
//-----------------------------------------------------------------------------
bool AComponent::isDrawing() const {
	if (getFlag(ANCESTOR_USING_BUFFER))
		return true;
	AContainerPtr component = boost::shared_dynamic_cast<AContainer>(getPtr());
	while (component) {
		if (component->getFlag(ANCESTOR_USING_BUFFER)) {
			return true;
		}
		component = component->getParent();
	}
	return false;
}
//-----------------------------------------------------------------------------
void AComponent::setFlag(Flag aFlag, bool b) {
	if (b) {
		flags |= (1 << aFlag);
	} else {
		flags &= ~(1 << aFlag);
	}
}
//-----------------------------------------------------------------------------
bool AComponent::getFlag(unsigned int aFlag) const {
	unsigned int mask = (1 << aFlag);
	return ((flags & mask) == mask);
}

}}} // namespace(s)
