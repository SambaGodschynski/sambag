/*
 * RedrawManager.hpp
 *
 *  Created on: Thu May 24 15:49:16 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_REDRAWMANAGER_H
#define SAMBAG_REDRAWMANAGER_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/ISurface.hpp>
#include <sambag/disco/IDrawContext.hpp>
#include <boost/unordered_map.hpp>
#include <sambag/com/Thread.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class RedrawManager.
  * This class manages repaint requests, allowing the number of repaints to be
  * minimized, for example by collapsing multiple requests into a single
  * repaint for members of a component tree.
  */
class RedrawManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RedrawManager> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<AComponentPtr> Components;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<AComponentPtr, Rectangle> ComponentMap;
protected:
	//-------------------------------------------------------------------------
	/**
	 * Removes any components from roots that are children of
	 * root.
	 */
	void adjustRoots(AComponentPtr root, Components &roots, int index);
	//-------------------------------------------------------------------------
	void collectDirtyComponents(ComponentMap &dirtyComponents,
			AComponentPtr dirtyComponent,
			Components &roots);
private:
	//-------------------------------------------------------------------------
	void updateDirtyComponent(AComponentPtr c, const Rectangle &r);
	//-------------------------------------------------------------------------
	mutable sambag::com::RecursiveMutex lock;
	//-------------------------------------------------------------------------
	ComponentMap hwDirtyComponents;
	ComponentMap dirtyComponents;
	ComponentMap tmpDirtyComponents;
	Components invalidComponents;
	//-------------------------------------------------------------------------
	/**
	 * True if we're in the process of painting the dirty regions.  This is
	 * set to true in <code>paintDirtyRegions</code>.
	 */
	//-------------------------------------------------------------------------
	ArithmeticWrapper<bool> painting;
	/**
	 * If the PaintManager calls into repaintRoot during painting this field
	 * will be set to the root.
	 */
	AComponentPtr repaintRoot;
	//-------------------------------------------------------------------------
	/**
	 * Extends the dirty region for the specified component to include
	 * the new region.
	 *
	 * @return false if <code>c</code> is not yet marked dirty.
	 */
	bool extendDirtyRegion(AComponentPtr c, const Rectangle &rect);
	//-------------------------------------------------------------------------
	void drawDirtyRegions(ComponentMap &tmpDirtyComponents);
public:
	//-------------------------------------------------------------------------
	/**
	 * 	Add a component in the list of components that should be refreshed.
	 * @param c
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	void addDirtyRegion(AComponentPtr c, const Rectangle &r);
	//-------------------------------------------------------------------------
	/**
	 * Mark the component as in need of layout and queue a runnable for the event
	 * dispatching thread that will validate the components first isValidateRoot()
	 * ancestor.
	 * @param invalidComponent
	 */
	void addInvalidComponent(AComponentPtr invalidComponent);
	//-------------------------------------------------------------------------
	/**
	 * @param c
	 * @return the RepaintManager for the calling thread given a Component.
	 */
	static Ptr currentManager(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param aComponent
	 * @return the current dirty region for a component.
	 */
	Rectangle getDirtyRegion(AComponentPtr aComponent) const;
	//-------------------------------------------------------------------------
	/**
	 *
	 * @return the maximum double buffer size.
	 */
	Dimension getDoubleBufferMaximumSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @param c
	 * @param proposedWidth
	 * @param proposedHeight
	 * @return the offscreen buffer that should be used as a double buffer
	 * with the component c.
	 */
	IImageSurface::Ptr getOffscreenBuffer(AComponentPtr c,
			const Coordinate &proposedWidth,
			const Coordinate & proposedHeight) const;
	//-------------------------------------------------------------------------
	/**
	 * @param c
	 * @param proposedWidth
	 * @param proposedHeight
	 * @return // 	Return a volatile offscreen buffer that should be used as a
	 *  double buffer with the specified component c.
	 */
	IImageSurface::Ptr getVolatileOffscreenBuffer(AComponentPtr c,
			const Coordinate &proposedWidth,
			const Coordinate & proposedHeight) const;
	//-------------------------------------------------------------------------
	/**
	 * 	Convenience method that returns true if aComponent will be
	 * 	completely painted during the next paintDirtyRegions().
	 * @param aComponent
	 * @return
	 */
	bool isCompletelyDirty(AComponentPtr aComponent) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if this RepaintManager is double buffered.
	 * @return
	 */
	bool isDoubleBufferingEnabled() const;
	//-------------------------------------------------------------------------
	/**
	 * Mark a component completely clean.
	 * @param aComponent
	 */
	void markCompletelyClean(AComponentPtr aComponent);
	//-------------------------------------------------------------------------
	/**
	 * Mark a component completely dirty.
	 * @param aComponent
	 */
	void markCompletelyDirty(AComponentPtr aComponent);
	//-------------------------------------------------------------------------
	/**
	 * Paint all of the components that have been marked dirty.
	 */
	void drawDirtyRegions();
	//-------------------------------------------------------------------------
	/**
	 * 	Remove a component from the list of invalid components.
	 * @param component
	 */
	void removeInvalidComponent(AComponentPtr component);
	//-------------------------------------------------------------------------
	/**
	 * Set the RepaintManager that should be used for the calling thread.
	 * @param aRepaintManager
	 */
	static void setCurrentManager(Ptr aRepaintManager);
	//-------------------------------------------------------------------------
	/**
	 * Enables or disables double buffering in this RepaintManager.
	 * @param aFlag
	 */
	void setDoubleBufferingEnabled(bool aFlag);
	//-------------------------------------------------------------------------
	/**
	 * Set the maximum double buffer size.
	 * @param d
	 */
	void setDoubleBufferMaximumSize(const Dimension &d);
	//-------------------------------------------------------------------------
	/**
	 * Returns a std::string that displays and identifies this object's properties.
	 * @return
	 */
	std::string toString() const;
	//-------------------------------------------------------------------------
	bool isDrawing() const;
	//-------------------------------------------------------------------------
	/**
	 * Paints a region of a component (when double buffering is enabled)
	 *
	 * @param paintingComponent Component to paint
	 * @param bufferComponent Component to obtain buffer for
	 * @param g Graphics to paint to
	 * @param x X-coordinate
	 * @param y Y-coordinate
	 * @param w Width
	 * @param h Height
	 */
	void draw(AComponentPtr paintingComponent, AComponentPtr bufferComponent,
			IDrawContext::Ptr g, const Rectangle &r);
	//-------------------------------------------------------------------------
	/**
	 * Validate all of the components that have been marked invalid.
	 */
	void validateInvalidComponents();
	//-------------------------------------------------------------------------
	void copyArea(AComponentPtr c, IDrawContext::Ptr cn, const Rectangle &src,
			const Point2D &dst, bool clip);
}; // RedrawManager
}}} // namespace(s)

#endif /* SAMBAG_REDRAWMANAGER_H */
