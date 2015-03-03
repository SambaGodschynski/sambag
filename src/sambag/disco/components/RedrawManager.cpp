/*
 * RedrawManager.cpp
 *
 *  Created on: Thu May 24 15:49:16 2012
 *      Author: Johannes Unger
 */

#include "RedrawManager.hpp"
#include "AContainer.hpp"
#include "Graphics.hpp"
#include <algorithm>
#include <boost/foreach.hpp>
#include "Window.hpp"
#include <boost/timer/timer.hpp>
#include <boost/tuple/tuple.hpp>
#include <sambag/disco/TimedUpdater.hpp>


namespace {
    /**
     * delaying window invalidate if time of execution is earlier than
     */
    enum { SAMBAG_REFRESH_TRIGGER_TIME = 33 };
    typedef boost::timer::nanosecond_type NanosecondsType;
	typedef boost::timer::cpu_timer CPUTimer;
    typedef boost::shared_ptr<CPUTimer> CPUTimerPtr;
    const  NanosecondsType ONE_MILLI_IN_NANO = 1000000;
}




namespace sambag { namespace disco { namespace components {
namespace {
	typedef boost::unordered_map<Window::Ptr, RedrawManager::Ptr>
		ManagerMap;
	ManagerMap managerMap;
	void onWindowClose(void *, const OnCloseEvent &ev, Window::WPtr win);
	RedrawManager::Ptr getManager(Window::Ptr win) {
		if (!win) {
			return RedrawManager::Ptr();
		}
		RedrawManager::Ptr res = managerMap[win];
		if (!res) {
			res = RedrawManager::Ptr(new RedrawManager());
			managerMap[win] = res;
			win->addOnCloseEventListener(
				boost::bind(&onWindowClose, _1, _2, Window::WPtr(win))
			);
		}
		return res;
	}
	void onWindowClose(void *, const OnCloseEvent &ev, Window::WPtr _win) {
		Window::Ptr win = _win.lock();		
		if (!win) {
			return;
		}
		managerMap.erase(win);
	}
}
//=============================================================================
//  Class RedrawManager
//=============================================================================
//-----------------------------------------------------------------------------
namespace {
	template <class T>
	struct RefreshWindow {
		bool update(const T &val) {
            WindowPtr win = val.lock();
            if (!win) {
                // seems that is no job anymore
                return true;
            }
            Rectangle bounds;
            win->getClientProperty("redrawManager.bounds", bounds);
            
            CPUTimerPtr tm;
            win->getClientProperty("redrawManager.timer", tm);
            
            if (!tm) {
                // dito
                return true;
            }
            
            long timestamp = (tm->elapsed().wall / ONE_MILLI_IN_NANO);
            long lastRefresh = 0;
            win->getClientProperty("redrawManager.timestamp", lastRefresh);
            if ((timestamp-lastRefresh) < SAMBAG_REFRESH_TRIGGER_TIME) {
                // not yet, not yet
                return false;
            }
            win->putClientProperty("redrawManager.timestamp", timestamp);
            win->invalidateWindow(bounds);
            return true;
        }
	};
}
void RedrawManager::invalidateWindow(WindowPtr win, Rectangle bounds) {
    /**
     * try to avoid frequently refreshing so the invalidate will be delayed
     * if the last execution was earlier than SAMBAG_REFRESH_TRIGGER_TIME
     */
	if (!win) {
		return;
        
	}
    static CPUTimerPtr timer = CPUTimerPtr(new CPUTimer);
    long timestamp = (timer->elapsed().wall / ONE_MILLI_IN_NANO);
    long lastRefresh = 0;
    win->getClientProperty("redrawManager.timestamp", lastRefresh);
    if ((timestamp-lastRefresh) > SAMBAG_REFRESH_TRIGGER_TIME) {
        // last invalidation was after refresh trigger time
        // so refresh immediately
        win->putClientProperty("redrawManager.timestamp", timestamp);
        win->invalidateWindow(bounds);
        return;
    }
    // delay refreshing
    typedef TimedUpdater<WindowWPtr,
        RefreshWindow,
        SAMBAG_REFRESH_TRIGGER_TIME> Updater;
    win->putClientProperty("redrawManager.bounds", bounds );
    win->putClientProperty("redrawManager.timer", timer);
	Updater::instance().update( WindowWPtr(win) );
}
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
	// invalidate containing window:
	Window::Ptr win = c->getTopLevelAncestor();
	//SAMBAG_ASSERT(win); // if c is visible win has to be valid.
    invalidateWindow(win, r);
}
//-------------------------------------------------------------------------
void RedrawManager::updateDirtyComponent(
	RedrawManager::ComponentMap &dirtyComponents, AComponentPtr c,
	const Rectangle &r)
{
	dirtyComponents[c] = r;
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
		Rectangle r = it->second;
		// compute union
		r = Rectangle(minimize(rect.x0(), r.x0()),
			Point2D(maximize(rect.x1(), r.x1())));
		updateDirtyComponent(dirtyComponents, c, r);
		return true;
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void RedrawManager::addInvalidComponent(AComponentPtr invalidComponent) {

	AContainer::Ptr validateRoot = invalidComponent->getValidateRoot();

	if (!validateRoot) {
		return;
	}
	BOOST_FOREACH(AComponent::WPtr c, invalidComponents) {
		if (validateRoot == c.lock()) {
			return;
		}
	}
	invalidComponents.push_back(validateRoot);
}
//-----------------------------------------------------------------------------
RedrawManager::Ptr RedrawManager::currentManager(AComponentPtr c) {
	if (!c) {
		return RedrawManager::Ptr();
	}
	Window::Ptr win = c->getFirstContainer<Window>();
	if (!win) // no window no redraw
		return RedrawManager::Ptr();
	return getManager(win);
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
void RedrawManager::markCompletelyDirty(AComponentPtr aComponent) {
	addDirtyRegion(aComponent, Rectangle(0, 0, INT_MAX, INT_MAX));
}
//-----------------------------------------------------------------------------
void RedrawManager::markCompletelyClean(AComponentPtr aComponent) {
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
	Coordinate dx, dy, rootDx, rootDy;
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
		updateDirtyComponent(dirtyComponents, rootDirtyComponent, r);
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
	ComponentMap tmp;
	SAMBAG_BEGIN_SYNCHRONIZED(lock) // swap for thread safety
		tmp.swap(dirtyComponents);
	SAMBAG_END_SYNCHRONIZED
	drawDirtyRegions(tmp);
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

		boost::geometry::intersection(
			Rectangle(localBoundsX,
				localBoundsY,
				localBoundsW,
				localBoundsH
			), rect, rect
		);

		if (dirtyComponent->isShowing()) {
			IDrawContext::Ptr cn = dirtyComponent->getDrawContext();
			Graphics g(cn);
			// If the Graphics goes away, it means someone disposed of
			// the window, don't do anything.
			if (cn) {
				cn->save();
				rect.inset(1., 1.);
				/**
				 * avoid smearing:
				 * see issue #278
				 */
                Rectangle nr = Rectangle(
					floor( rect.x() ),
					floor( rect.y() ),
					ceil( rect.width() ),
					ceil( rect.height() ));
				//SAMBAG_LOG_TRACE<<nr;
                g.setClip(nr);
				dirtyComponent->draw(g.getPtr());
				cn->restore();
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
}
//-----------------------------------------------------------------------------
void RedrawManager::removeInvalidComponent(AComponentPtr component) {
	AContainer::Ptr c = boost::dynamic_pointer_cast<AContainer>(component);
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
//    java.util.AList < Component > ic;
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
		AComponentPtr bufferComponent, IDrawContext::Ptr cn, const Rectangle &r)
{
	//if (!paintManager.paint(paintingComponent, bufferComponent, g, x, y, w, h)) {
	cn->setClip(r);
	paintingComponent->drawToOffscreen(cn, r, r.x1());
}
//-----------------------------------------------------------------------------
void RedrawManager::copyArea(AComponentPtr c, IDrawContext::Ptr cn,
		const Rectangle &src, const Point2D &dst, bool clip)
{
	cn->copyAreaTo(cn, src, dst);
}
}}} // RedrawManager::RedrawManager::namespace(s)
