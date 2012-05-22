/*
 * RedrawManager.cpp
 *
 *  Created on: Thu May 24 15:49:16 2012
 *      Author: Johannes Unger
 */

#include "RedrawManager.hpp"
#include "AContainer.hpp"
#include "Graphics.hpp"
namespace sambag { namespace disco { namespace components {
namespace {
	RedrawManager::Ptr holder;
}
//=============================================================================
//  Class RedrawManager
//=============================================================================
//-----------------------------------------------------------------------------
void RedrawManager::addDirtyRegion(AComponentPtr c, const Rectangle &r) {
	/* Special cases we don't have to bother with.
	 */
	if ((r.getWidth() <= 0) || (r.getHeight() <= 0) || (!c)) {
		return;
	}

	if ((c->getWidth() <= 0) || (c->getHeight() <= 0)) {
		return;
	}

	if (extendDirtyRegion(c, r)) {
		// Component was already marked as dirty, region has been
		// extended, no need to continue.
		return;
	}

	/* Make sure that c and all it ancestors (up to an Applet or
	 * Window) are visible.  This loop has the same effect as
	 * checking c.isShowing() (and note that it's still possible
	 * that c is completely obscured by an opaque ancestor in
	 * the specified rectangle).
	 */
	if (!c->isShowing())
		return;

	SAMBAG_BEGIN_SYNCHRONIZED(lock)
		if (extendDirtyRegion(c, r)) {
			// In between last check and this check another thread
			// queued up runnable, can bail here.
			return;
		}
		dirtyComponents.insert(std::make_pair(c, r));
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
bool RedrawManager::extendDirtyRegion(AComponentPtr c, const Rectangle &rect) {
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
	ComponentMap::iterator it = dirtyComponents.find(c);
	if (it==dirtyComponents.end())
		return false;
	// A non-null r implies c is already marked as dirty,
	// and that the parent is valid. Therefore we can
	// just union the rect and bail.
	Rectangle &r = it->second;
	// compute union
	r = Rectangle(minimize(rect.x0(), r.x0()),
		Point2D(maximize(rect.x1(), r.x1())));
	return true;
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void RedrawManager::addInvalidComponent(AComponentPtr invalidComponent) {

	AContainer::Ptr validateRoot = invalidComponent->getValidateRoot();

	if (!validateRoot) {
		return;
	}
	BOOST_FOREACH(AComponent::Ptr c, invalidComponents) {
		if (validateRoot == c) {
			return;
		}
	}
	invalidComponents.push_back(validateRoot);
}
//-----------------------------------------------------------------------------
RedrawManager::Ptr RedrawManager::currentManager(AComponentPtr c) {
	if (!holder) {
		holder = Ptr(new RedrawManager());
	}
	return holder;
}
//-----------------------------------------------------------------------------
Rectangle RedrawManager::getDirtyRegion(AComponentPtr aComponent) const {
	Rectangle r;
	ComponentMap::const_iterator it;
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
		it = dirtyComponents.find(aComponent);
	SAMBAG_END_SYNCHRONIZED
	if (it==dirtyComponents.end())
		return Rectangle(0, 0, 0, 0);
	else
		return it->second;
}
//-----------------------------------------------------------------------------
Dimension RedrawManager::getDoubleBufferMaximumSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
IImageSurface::Ptr RedrawManager::getOffscreenBuffer(AComponentPtr c,
		const Coordinate &proposedWidth,
		const Coordinate & proposedHeight) const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return IImageSurface::Ptr();
}
//-----------------------------------------------------------------------------
IImageSurface::Ptr RedrawManager::getVolatileOffscreenBuffer(AComponentPtr c,
		const Coordinate &proposedWidth,
		const Coordinate & proposedHeight) const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return IImageSurface::Ptr();
}
//-----------------------------------------------------------------------------
bool RedrawManager::isCompletelyDirty(AComponentPtr aComponent) const {
	Rectangle r;
	r = getDirtyRegion(aComponent);
	if (r.getWidth() == INT_MAX && r.getHeight() == INT_MAX)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------
bool RedrawManager::isDoubleBufferingEnabled() const {
	return false;
}
//-----------------------------------------------------------------------------
void RedrawManager::markCompletelyClean(AComponentPtr aComponent) {
	addDirtyRegion(aComponent, Rectangle(0, 0, INT_MAX, INT_MAX));
}
//-----------------------------------------------------------------------------
void RedrawManager::markCompletelyDirty(AComponentPtr aComponent) {
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
			dirtyComponents.erase(aComponent);
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void RedrawManager::adjustRoots(AComponentPtr root,
		Components &roots, int index)
{
	if (roots.empty())
		return;
	Components::iterator s = roots.begin();
	for (int i=0; i<index; ++i)
		++s;
	Components::iterator it = roots.end();
	--it;
	for (; it!=s; --it) {
		AComponent::Ptr c = *it;
		for (;;) {
			if (c == root || !c) {
				break;
			}
			c = c->getParent();
		}
		if (c == root) {
			it = roots.erase(it);
		}
	}
}
//-----------------------------------------------------------------------------
void RedrawManager::collectDirtyComponents(ComponentMap &dirtyComponents,
		AComponentPtr dirtyComponent, Components &roots)
{
	int dx, dy, rootDx, rootDy;
	AComponentPtr component, rootDirtyComponent, parent;
	Rectangle cBounds, tmp;

	// Find the highest parent which is dirty.  When we get out of this
	// rootDx and rootDy will contain the translation from the
	// rootDirtyComponent's coordinate system to the coordinates of the
	// original dirty component.  The tmp Rect is also used to compute the
	// visible portion of the dirtyRect.

	component = rootDirtyComponent = dirtyComponent;

	Coordinate x = dirtyComponent->getX();
	Coordinate y = dirtyComponent->getY();
	Coordinate w = dirtyComponent->getWidth();
	Coordinate h = dirtyComponent->getHeight();

	dx = rootDx = 0;
	dy = rootDy = 0;
	tmp = dirtyComponents[dirtyComponent];

	//SwingUtilities.computeIntersection(0, 0, w, h, tmp);
	boost::geometry::intersection<Rectangle::Base, Rectangle::Base, Rectangle::Base>
		(Rectangle(0,0,w,h), tmp, tmp);

	if (tmp.getWidth() <=0 || tmp.getHeight() <= 0) {
		return;
	}

	for (;;) {
		parent = component->getParent();
		if (!parent)
			break;

		component = parent;
		dx += x;
		dy += y;
		Point2D p = tmp.x0();
		boost::geometry::add_point(p, Point2D(x,y));
		tmp.translate(p);

		x = component->getX();
		y = component->getY();
		w = component->getWidth();
		h = component->getHeight();
		// tmp = SwingUtilities.computeIntersection(0, 0, w, h, tmp);
		boost::geometry::intersection<Rectangle::Base, Rectangle::Base, Rectangle::Base>
			(Rectangle(0,0,w,h), tmp, tmp);
		if (tmp.getWidth() <=0 || tmp.getHeight() <= 0) {
			return;
		}

		if (dirtyComponents.find(component) != dirtyComponents.end()) {
			rootDirtyComponent = component;
			rootDx = dx;
			rootDy = dy;
		}
	}

	if (dirtyComponent != rootDirtyComponent) {
		Rectangle r;
		Point2D p = tmp.x0();
		// tmp.translate(Point2D(tmp.x0.x() + rootDx - dx, tmp.x0.y() + rootDy - dy));
		boost::geometry::add_point(p, Point2D(rootDx - dx, rootDy - dy));
		tmp.translate(p);
		r = dirtyComponents[rootDirtyComponent];
		//computeUnion(tmp.x, tmp.y, tmp.width, tmp.height, r);
		r = Rectangle(minimize(tmp.x0(), r.x0()),
			Point2D(maximize(tmp.x1(), r.x1())));
	}

	// If we haven't seen this root before, then we need to add it to the
	// list of root dirty Views.
	if (std::find(roots.begin(), roots.end(), rootDirtyComponent)
		== roots.end())
	{
		roots.push_back(rootDirtyComponent);
	}
}
//-----------------------------------------------------------------------------
void RedrawManager::drawDirtyRegions() {
	SAMBAG_BEGIN_SYNCHRONIZED(lock) // swap for thread safety
		ComponentMap tmp = tmpDirtyComponents;
		tmpDirtyComponents = dirtyComponents;
		dirtyComponents = tmp;
		dirtyComponents.clear();
	SAMBAG_END_SYNCHRONIZED
	drawDirtyRegions(tmpDirtyComponents);
}
//-------------------------------------------------------------------------
void RedrawManager::drawDirtyRegions(ComponentMap &tmpDirtyComponents) {
	int count;
	Components roots;
	AComponent::Ptr dirtyComponent;

	count = tmpDirtyComponents.size();
	if (count == 0) {
		return;
	}

	Rectangle rect;
	Coordinate localBoundsX = 0;
	Coordinate localBoundsY = 0;
	Coordinate localBoundsH;
	Coordinate localBoundsW;
	//Enumeration keys;

	//roots = Components(count);

	BOOST_FOREACH (ComponentMap::value_type &p, tmpDirtyComponents) {
		collectDirtyComponents(tmpDirtyComponents, p.first, roots);
	}

	count = roots.size();
	painting = true;

	BOOST_FOREACH(AComponent::Ptr dirtyComponent, roots) {
		rect = tmpDirtyComponents[dirtyComponent];
		localBoundsH = dirtyComponent->getHeight();
		localBoundsW = dirtyComponent->getWidth();

		boost::geometry::intersection<Rectangle::Base, Rectangle::Base, Rectangle::Base>
				(Rectangle(localBoundsX,localBoundsY,localBoundsW,localBoundsH), rect, rect);

		if (dirtyComponent->isShowing()) {
			IDrawContext::Ptr cn = dirtyComponent->getDrawContext();
			Graphics g(cn);
			// If the Graphics goes away, it means someone disposed of
			// the window, don't do anything.
			if (cn) {
				g.setClip(rect);
				dirtyComponent->draw(g.getPtr());
			}
		}
		//// If the repaintRoot has been set, service it now and
		//// remove any components that are children of repaintRoot.
		//if (repaintRoot != null) {
		//	adjustRoots(repaintRoot, roots, i + 1);
		//	count = roots.size();
		//	paintManager.isRepaintingRoot = true;
		//	repaintRoot.paintImmediately(0, 0, repaintRoot.getWidth(),
		//			repaintRoot.getHeight());
		//	paintManager.isRepaintingRoot = false;
		//	// Only service repaintRoot once.
		//	repaintRoot = null;
		//}
	}
	painting = false;
	tmpDirtyComponents.clear();
}
//-----------------------------------------------------------------------------
void RedrawManager::removeInvalidComponent(AComponentPtr component) {
	AContainer::Ptr c = boost::shared_dynamic_cast<AContainer>(component);
	if (c)
		invalidComponents.remove(c);
}
//-----------------------------------------------------------------------------
void RedrawManager::setCurrentManager(RedrawManager::Ptr aRepaintManager) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void RedrawManager::setDoubleBufferingEnabled(bool aFlag) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void RedrawManager::setDoubleBufferMaximumSize(const Dimension &d) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
std::string RedrawManager::toString() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return "";
}
//-----------------------------------------------------------------------------
void RedrawManager::validateInvalidComponents() {
	SAMBA_LOG_NOT_YET_IMPL();
//    java.util.List < Component > ic;
//	synchronized(this) {
//		if(invalidComponents == null) {
//			return;
//		}
//		ic = invalidComponents;
//		invalidComponents = null;
//	}
//	int n = ic.size();
//	for (int i = 0; i < n; i++) {
//		ic.get(i).validate();
//	}
}
//-----------------------------------------------------------------------------
bool RedrawManager::isDrawing() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
void RedrawManager::draw(AComponentPtr paintingComponent,
		AComponentPtr bufferComponent, IDrawContext::Ptr g, const Rectangle &r)
{
	SAMBA_LOG_NOT_YET_IMPL();
}
}}} // RedrawManager::RedrawManager::namespace(s)
