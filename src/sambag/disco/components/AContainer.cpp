/*
 * AContainer.cpp
 *
 *  Created on: Wed May 16 12:00:29 2012
 *      Author: Johannes Unger
 */

#include "AContainer.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include <sambag/com/Helper.hpp>
#include <boost/foreach.hpp>
#include "Graphics.hpp"
#include "RedrawManager.hpp"
#include "ui/AComponentUI.hpp"
#include "RootPane.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class AContainer
//=============================================================================
//-----------------------------------------------------------------------------
AContainer::AContainer() {

}
//-----------------------------------------------------------------------------
AContainer::~AContainer() {
}
//-----------------------------------------------------------------------------
int AContainer::__countHierarchyMembers_() const {
	// Verify descendantsCount is correct
	int sum = 0;
	BOOST_FOREACH(AComponent::Ptr comp, components) {
		sum += comp->__countHierarchyMembers_();
	}
	return descendantsCount + 1;
}
//-----------------------------------------------------------------------------
int AContainer::__dispatchHierarchyEvents_(events::HierarchyEvent::Type id,
		AComponentPtr changed, AContainerPtr changedParent, long changeFlags) {
	checkTreeLock();
	BOOST_FOREACH(AComponent::Ptr comp, components) {
		comp->__dispatchHierarchyEvents_(id, changed,
				changedParent, changeFlags);
	}
	return AComponent::__dispatchHierarchyEvents_(id, changed, changedParent,
			changeFlags);
}
//-----------------------------------------------------------------------------
void AContainer::checkAddToSelf(AComponent::Ptr comp) const {
	AContainer::Ptr tmp = boost::shared_dynamic_cast<AContainer>(comp);
	if (!tmp)
		return;
	for (AContainer::Ptr cn = getPtr(); cn; cn = cn->getParent()) {
		if (cn == comp) {
			SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
					"adding container's parent to itself");
		}
	}
}
//-----------------------------------------------------------------------------
void AContainer::addComponent(AComponent::Ptr comp, int index) {
	if (!comp)
		return;
	if (index > (int) components.size() || (index < 0 && index != -1)) {
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
				"illegal component position");
	}
	checkAddToSelf(comp);
	/* TODO: check device
	 if (thisGC != null) {
	 comp.checkGD(thisGC.getDevice().getIDstring());
	 }*/
	//Reparent the component and tidy up the tree's state.
	if (comp->getParent()) {
		comp->getParent()->remove(comp);
		if (index > (int) components.size()) {
			SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
					"illegal component position");
		}
	}
	if (index == -1) {
		components.push_back(comp);
	} else {
		Components::iterator it = components.begin();
		it += index;
		components.insert(it, comp);
	}
	// update ui
	if (!comp->isUiSettedByUser()) {
		RootPane::Ptr root = getTopLevelRootPane();
		if (root) {
			ui::ALookAndFeelPtr laf = root->getCurrentLookAndFeel();
			if (laf)
				comp->installLookAndFeel(laf);
		}
	}
	comp->__updateParent_(getPtr());
	adjustDescendants(comp->__countHierarchyMembers_());
	__invalidateIfValid_();
}
//-----------------------------------------------------------------------------
void AContainer::dispatchAddEvents(AComponent::Ptr comp) {
	using namespace events;
	dispatchEvent(
			ContainerEvent(getPtr(), ContainerEvent::COMPONENT_ADDED, comp),
			this);
	comp->__dispatchHierarchyEvents_(HierarchyEvent::HIERARCHY_CHANGED, comp,
			getPtr(), HierarchyEvent::PARENT_CHANGED);
}
//-----------------------------------------------------------------------------
AComponent::Ptr AContainer::add(AComponent::Ptr comp, int index,
		com::ArbitraryType::Ptr constraint)
{
	if (!comp)
		return AComponent::Ptr();
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		addComponent(comp, index);
		if (layoutMgr)
			layoutMgr->addLayoutComponent(comp, constraint);
	SAMBAG_END_SYNCHRONIZED
	dispatchAddEvents(comp);
	return comp;
}
//-----------------------------------------------------------------------------
void AContainer::doLayout() {
	ALayoutManager::Ptr lm = layoutMgr;
	if (lm) {
		lm->layoutContainer(getPtr());
	}
}
//-----------------------------------------------------------------------------
AComponent::Ptr AContainer::getComponentAt(const Point2D &p) const {
	if (!contains(p)) {
		return AComponent::Ptr();
	}
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		BOOST_FOREACH(AComponent::Ptr comp, components) {
			Point2D trP = p;
			geometry::subtract_point(trP, comp->getLocation());
			if (comp) {
				if (comp->contains(trP)) {
					return comp;
				}
			}
		}
	SAMBAG_END_SYNCHRONIZED
	return getPtr();
}
//-----------------------------------------------------------------------------
Coordinate AContainer::getAlignmentX() const {
	Coordinate xAlign = NULL_NUMBER;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		if (layoutMgr)
			xAlign = layoutMgr->getLayoutAlignmentX(getPtr());
	SAMBAG_END_SYNCHRONIZED
	if (xAlign==NULL_NUMBER)
		return AComponent::getAlignmentX();
	return xAlign;
}
//-----------------------------------------------------------------------------
Coordinate AContainer::getAlignmentY() const {
	Coordinate yAlign = NULL_NUMBER;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		if (layoutMgr)
			yAlign = layoutMgr->getLayoutAlignmentY(getPtr());
	SAMBAG_END_SYNCHRONIZED
	if (yAlign==NULL_NUMBER)
		return AComponent::getAlignmentY();
	return yAlign;
}
//-----------------------------------------------------------------------------
AComponent::Ptr AContainer::getComponent(size_t n) const {
	return components.at(n);
}
//-----------------------------------------------------------------------------
size_t AContainer::getComponentCount() const {
	return components.size();
}
//-----------------------------------------------------------------------------
AContainer::Components & AContainer::getComponents() {
	return components;
}
//-----------------------------------------------------------------------------
const AContainer::Components & AContainer::getComponents() const {
	return components;
}
//-----------------------------------------------------------------------------
int AContainer::getComponentZOrder(AComponent::Ptr comp) const {
	if (!comp) {
		return -1;
	}
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		// Quick check - container should be immediate parent of the component
		if (comp->getParent() != getPtr()) {
			return -1;
		}
		return sambag::com::indexOf(components, comp);SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AContainer::adjustDescendants(int num) {
	if (num == 0)
		return;

	descendantsCount += num;
	adjustDecendantsOnParent(num);
}
//-----------------------------------------------------------------------------
void AContainer::adjustDecendantsOnParent(int num) {
	AContainerPtr parent = getParent();
	if (!parent)
		return;
	parent->adjustDescendants(num);
}
//-----------------------------------------------------------------------------
Insets AContainer::getInsets() const {
	if (border)
		border->getBorderInsets(getPtr());
	return Insets();
}
//-----------------------------------------------------------------------------
ALayoutManager::Ptr AContainer::getLayout() const {
	return layoutMgr;
}
//-----------------------------------------------------------------------------
Dimension AContainer::getMaximumSizeAlt() {
	Dimension dim = maxSize;
	if (dim == NULL_DIMENSION || !(isMaximumSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			maxSize = (layoutMgr) ? layoutMgr->maximumLayoutSize(getPtr())
					: AComponent::getPreferredSize();
			dim = maxSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
}
//-----------------------------------------------------------------------------
Dimension AContainer::getMaximumSize() {
	if (isMaximumSizeSet()) {
		return getMaximumSizeAlt();
	}
	Dimension size = NULL_DIMENSION;
	if (ui) {
		size = ui->getMaximumSize(getPtr());
	}
	return (size != NULL_DIMENSION) ? size : getMaximumSizeAlt();
}
//-----------------------------------------------------------------------------
Dimension AContainer::getMinimumSizeAlt() {
	Dimension dim = minSize;
	if (dim == NULL_DIMENSION || !(isMinimumSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			minSize = (layoutMgr) ? layoutMgr->minimumLayoutSize(getPtr())
					: AComponent::getPreferredSize();
			dim = minSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
}
//-----------------------------------------------------------------------------
Dimension AContainer::getMinimumSize() {
	if (isMinimumSizeSet()) {
		return getMinimumSizeAlt();
	}
	Dimension size = NULL_DIMENSION;
	if (ui) {
		size = ui->getMinimumSize(getPtr());
	}
	return (size != NULL_DIMENSION) ? size : getMinimumSizeAlt();
}
//-----------------------------------------------------------------------------
Dimension AContainer::getPreferredSizeAlt() {
	Dimension dim = prefSize;
	if (dim == NULL_DIMENSION || !(isPreferredSizeSet() || isValid())) {
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			prefSize = (layoutMgr) ? layoutMgr->preferredLayoutSize(getPtr())
					: AComponent::getPreferredSize();
			dim = prefSize;
		SAMBAG_END_SYNCHRONIZED
	}
	return dim;
}
//-----------------------------------------------------------------------------
Dimension AContainer::getPreferredSize() {
	if (isPreferredSizeSet()) {
		return getPreferredSizeAlt();
	}
	Dimension size = NULL_DIMENSION;
	if (ui) {
		size = ui->getPreferredSize(getPtr());
	}
	return (size != NULL_DIMENSION) ? size : getPreferredSizeAlt();
}
//-----------------------------------------------------------------------------
AComponentPtr AContainer::findComponentAt(const Point2D &p,
		bool includeSelf)
{
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		BOOST_FOREACH(AComponent::Ptr comp, components) {
			Point2D trP = p;
			geometry::subtract_point(trP, comp->getLocation());
			if (comp && comp->isVisible() && comp->contains(trP))
			{
				// found a component that intersects the point, see if there
				// is a deeper possibility.
				AContainer::Ptr con = boost::shared_dynamic_cast<AContainer>(comp);
				if (con) {
					AComponent::Ptr deeper = con->findComponentAt(
						trP,
						includeSelf);
					if (deeper) {
						return deeper;
					}
				} else {
					return comp;
				}
			}
		}
	SAMBAG_END_SYNCHRONIZED
	if (contains(p) && includeSelf) {
		return getPtr();
	}
	return AComponent::Ptr();
}
//-----------------------------------------------------------------------------
bool AContainer::isAncestorOf(AComponent::Ptr c) const {
	if (!c)
		return false;
	AContainer::Ptr p = c->getParent();
	if (!p) {
		return false;
	}
	while (p) {
		if (p == getPtr()) {
			return true;
		}
		p = p->getParent();
	}
	return false;
}
//-----------------------------------------------------------------------------
bool AContainer::isValidateRoot() const {
	return false;
}
//-----------------------------------------------------------------------------
void AContainer::invalidate() {
	if (layoutMgr)
		layoutMgr->invalidateLayout(getPtr());
	AComponent::invalidate();
}
//-----------------------------------------------------------------------------
void AContainer::invalidateParent() {
	if (!isValidateRoot()) {
		AComponent::invalidateParent();
	}
}
//-----------------------------------------------------------------------------
void AContainer::draw(IDrawContext::Ptr cn) {
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
		if (!rectangleIsObscured(clipRect)) {
			drawComponent(co.getPtr());
			drawBorder(co.getPtr());
			drawChildren(co.getPtr());
		}
	}
}
//-----------------------------------------------------------------------------
AContainer::ObscuredState AContainer::getObscuredState(int compIndex,
		const Rectangle &rect) const
{
	ObscuredState retValue = NOT_OBSCURED;

	for (int i = compIndex - 1; i >= 0; i--) {
		AComponent::Ptr sibling = getComponent(i);
		if (!sibling->isVisible()) {
			continue;
		}
		Rectangle siblingRect;
		bool opaque;
		opaque = sibling->isOpaque();
		if (!opaque) {
			if (retValue == PARTIALLY_OBSCURED) {
				continue;
			}
		}
		else {
			opaque = true;
		}
		siblingRect = sibling->getBounds();
		bool within =
				boost::geometry::within<Rectangle::Base, Rectangle::Base>
					(bounds, siblingRect);

		if (opaque && within) {
			return COMPLETELY_OBSCURED;
		}
		else {
			bool intersects =
					boost::geometry::intersects<Rectangle::Base, Rectangle::Base>
						(bounds, siblingRect);
			if (retValue == NOT_OBSCURED && intersects)
				retValue = PARTIALLY_OBSCURED;
		}
	}
	return retValue;
}
//-----------------------------------------------------------------------------
bool AContainer::rectangleIsObscured(const Rectangle &r) {
	BOOST_FOREACH(AComponent::Ptr child, components) {
		const Rectangle &cr = child->getBounds();
		bool within =
				boost::geometry::within<Rectangle::Base, Rectangle::Base>
					(r, cr);
		if (within && child->isVisible()) {
			return false; //TODO: child->isOpaque();
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
void AContainer::drawChildren(IDrawContext::Ptr context) {
	ScratchGraphics sg(context);
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		int i = getComponentCount() - 1;
		if (i < 0) {
			return;
		}
		// If we are only to paint to a specific child, determine
		// its index.
		/*if (paintingChild != null &&
				(paintingChild instanceof JComponent) &&
				paintingChild.isOpaque()) {
			for (; i >= 0; i--) {
				if (getComponent(i) == paintingChild) {
					break;
				}
			}
		}*/
		Rectangle clipBounds = sg.clipExtends();
		if (clipBounds == NULL_RECTANGLE) {
			clipBounds = Rectangle(0, 0, getWidth(), getHeight());
		}
		for (; i >= 0; i--) {
			AComponent::Ptr comp = getComponent(i);
			if (!comp)
				continue;
			if (!comp->isVisible())
				continue;

			Rectangle cr = comp->getBounds();
			if (!sg.hitClip(cr)) // formally g !!
				continue;
			if (i > 0) {
				Rectangle tmp;
				boost::geometry::intersection<Rectangle::Base,
				Rectangle::Base, Rectangle::Base> (cr, clipBounds, tmp);
				if(getObscuredState(i, tmp) == COMPLETELY_OBSCURED) {
					continue;
				}
			} // if (i>0)
			sg.save();
			sg.translate(cr.x0());
			sg.setClip(Rectangle(Point2D(0,0), cr.getWidth(), cr.getHeight()));
			sg.setStrokeColor(comp->getForeground());
			comp->draw(sg.getPtr());
			sg.restore();
		} // for (; i >= 0; i--)
	SAMBAG_END_SYNCHRONIZED
}
//-------------------------------------------------------------------------
void AContainer::drawComponents(IDrawContext::Ptr cn) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::remove(AComponent::Ptr comp) {
	if (comp->getParent() == getPtr()) {
		int index = sambag::com::indexOf(components, comp);
		if (index >= 0) {
			remove(index);
		}
	}
}
//-----------------------------------------------------------------------------
void AContainer::remove(size_t index) {
	using namespace events;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		if (index < 0 || index >= components.size()) {
			SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
					"out of bounds");
		}
		AComponent::Ptr comp = components.at(index);

		if (layoutMgr) {
			layoutMgr->removeLayoutComponent(comp);
		}
		adjustDescendants(-(comp->__countHierarchyMembers_()));
		comp->__updateParent_(AContainer::Ptr());
		//components.remove(index);
		Components::iterator it = components.begin();
		it += index;
		components.erase(it);
		//
		__invalidateIfValid_();

		dispatchEvent(
				ContainerEvent(getPtr(), ContainerEvent::COMPONENT_REMOVED,
						comp), this);

		comp->__dispatchHierarchyEvents_(HierarchyEvent::HIERARCHY_CHANGED,
				comp, getPtr(), HierarchyEvent::PARENT_CHANGED);
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AContainer::removeAll() {
	using namespace events;
	if (components.empty())
		return;
	SAMBAG_BEGIN_SYNCHRONIZED (getTreeLock())
		adjustDescendants(-descendantsCount);
		Components::iterator it = components.end();
		bool _break = false;
		do {
			if (--it == components.begin())
				_break = true;
			AComponent::Ptr comp = *it;
			components.erase(it);
			if (layoutMgr) {
				layoutMgr->removeLayoutComponent(comp);
			}
			comp->__updateParent_(AContainer::Ptr());
			dispatchEvent(
					ContainerEvent(getPtr(), ContainerEvent::COMPONENT_REMOVED,
							comp), this);
			comp->__dispatchHierarchyEvents_(HierarchyEvent::HIERARCHY_CHANGED,
					comp, getPtr(), HierarchyEvent::PARENT_CHANGED);
		} while (_break);
		__invalidateIfValid_();
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AContainer::setLayout(ALayoutManager::Ptr mgr) {
	layoutMgr = mgr;
	__invalidateIfValid_();
}
//-----------------------------------------------------------------------------
void AContainer::transferFocusBackward() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::validate() {
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		if (!isValid()) {
			validateTree();
		}
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AContainer::invalidateTree() {
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		BOOST_FOREACH(AComponent::Ptr comp, components) {
			AContainer::Ptr con = boost::shared_dynamic_cast<AContainer>(comp);
			if (con && (!comp->isValid() || descendUnconditionallyWhenValidating))
			{
				con->invalidateTree();
			} else {
				comp->__invalidateIfValid_();
			}
		}
		__invalidateIfValid_();
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void AContainer::update(IDrawContext::Ptr cn) {
	if (isShowing()) {
		Graphics g(cn);
		g.clearRect(getBounds());
		draw(cn);
	}
}
//-----------------------------------------------------------------------------
void AContainer::validateTree() {
	checkTreeLock();
	if (!isValid() || descendUnconditionallyWhenValidating) {
		if (!isValid()) {
			doLayout();
		}
		BOOST_FOREACH(AComponent::Ptr comp, components) {
			AContainer::Ptr con = boost::shared_dynamic_cast<AContainer>(comp);
			if (con && (!comp->isValid() || descendUnconditionallyWhenValidating))
			{
				con->validateTree();
			} else {
				comp->validate();
			}
		}
	}
	AComponent::validate();
}
//-------------------------------------------------------------------------
void AContainer::trackMouseEnterEvents(AComponentPtr target,
		const events::MouseEvent &ev)
{
	using namespace events;
	AComponentPtr lastMouseTarget = _lastMouseTarget.lock();
	if (target == lastMouseTarget)
		return;
	if (lastMouseTarget) {
//		MouseEvent nev = MouseEvent(getPtr(), ev.getLocation(),
//				ev.getButtons(), MouseEvent::MOUSE_EXITED);
		MouseEvent nev = ev;
		nev.updateSoure(getPtr()).updateType(MouseEvent::MOUSE_EXITED);
		retargetMouseEvent(lastMouseTarget, nev);
		lastMouseTarget->EventSender<MouseEvent>::notifyListeners(
				lastMouseTarget.get(),
				nev
		);
	}
	if (target) {
//		MouseEvent nev = MouseEvent(getPtr(), ev.getLocation(),
//				ev.getButtons(), MouseEvent::MOUSE_ENTERED);
		MouseEvent nev = ev;
		nev.updateSoure(getPtr()).updateType(MouseEvent::MOUSE_ENTERED);
		retargetMouseEvent(target, nev);
		 target->EventSender<MouseEvent>::notifyListeners(target.get(), nev);
	}
	lastMouseTarget = target;
}
//-----------------------------------------------------------------------------
void AContainer::processMouseEvent(const events::MouseEvent &ev) {
	using namespace events;
	AComponent::Ptr target = findComponentAt(ev.getLocation());
	AComponentPtr lastMouseTarget = _lastMouseTarget.lock();

	if (ev.getType() == MouseEvent::MOUSE_DRAGGED) {
		if(!lastMouseTarget)
			return;
		if (lastMouseTarget==getPtr())
			AComponent::processMouseEvent(ev);
		else {
			events::MouseEvent nev = ev;
			retargetMouseEvent(lastMouseTarget, nev);
			lastMouseTarget->processMouseEvent(nev);
		}
		return;
	}

	if (ev.getType() == MouseEvent::MOUSE_RELEASED) {
		if(lastMouseTarget) {
			events::MouseEvent nev = ev;
			retargetMouseEvent(lastMouseTarget, nev);
			if (lastMouseTarget==getPtr())
				AComponent::processMouseEvent(nev);
			else
				lastMouseTarget->processMouseEvent(nev);
			return;
		}
	}

	trackMouseEnterEvents(target, ev);

	if (!target || target == getPtr()) {
		AComponent::processMouseEvent(ev);
		return;
	}
	// MouseEvent nev = MouseEvent(getPtr(), ev);
	MouseEvent nev = ev;
	nev.updateSoure(getPtr());
	retargetMouseEvent(target, nev);
	target->processMouseEvent(nev);
}
//-----------------------------------------------------------------------------
void AContainer::retargetMouseEvent(AComponentPtr c, events::MouseEvent &ev)
{
	using namespace events;
 	Point2D p = ev.getLocation();
	AComponent::Ptr component;

	for(component = c; component; component = component->getParent()) {
		boost::geometry::subtract_point(p, component->getLocation());
	}
	//ev = MouseEvent(c, p, ev.getButtons(), ev.getType());
	ev.updateSoure(c).updateLocation(p);
}
//-----------------------------------------------------------------------------
std::string AContainer::parameterString() const {
	std::stringstream os;
	os << AComponent::parameterString();
	os << ",alignmentX=" << getAlignmentX() <<
			",alignmentY=" << getAlignmentY();
			//",border=" << borderString <<
			//",flags=" << flags <<
	if (isMaximumSizeSet())
		os << ",maximumSize=" << maxSize;
	if (isMinimumSizeSet())
		os << ",maximumSize=" << minSize;
	if (isPreferredSizeSet())
		os << ",maximumSize=" << prefSize;
	return os.str();
}
//-----------------------------------------------------------------------------
void AContainer::installLookAndFeel (ui::ALookAndFeelPtr laf) {
	AComponent::installLookAndFeel(laf);
	BOOST_FOREACH(AComponent::Ptr c, components) {
		c->installLookAndFeel(laf);
	}
}
//-----------------------------------------------------------------------------
void AContainer::printComponentTree(std::ostream &ss) const {
	ss << toString() << std::endl;
	BOOST_FOREACH(AComponent::Ptr c, components) {
		ss << " " << c->toString() << std::endl;
	}
}
}}}
// namespace(s)
