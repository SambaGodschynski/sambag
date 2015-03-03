/*
 * CellRendererPane.hpp
 *
 *  Created on: Wed Sep  5 12:05:09 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CELLRENDERERPANE_H
#define SAMBAG_CELLRENDERERPANE_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class CellRendererPane.
  */
class CellRendererPane : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CellRendererPane> Ptr;
protected:
	//-------------------------------------------------------------------------
	CellRendererPane();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(CellRendererPane)
	//-------------------------------------------------------------------------
	/**
	 * Construct a CellRendererPane object.
	 */
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	/**
	 * Overridden to avoid propagating a invalidate up the tree when the
	 * cell renderer child is configured.
	 */
	virtual void invalidate() {}
	//-------------------------------------------------------------------------
	/**
	 * Shouldn't be called.
	 */
	virtual void draw(IDrawContext::Ptr cn) {}
	//-------------------------------------------------------------------------
	/**
	 * Shouldn't be called.
	 */
	virtual void update(IDrawContext::Ptr cn) {}
	//-------------------------------------------------------------------------
	/**
	 * If the specified component is already a child of this then we don't
	 * bother doing anything - stacking order doesn't matter for cell
	 * renderer components (CellRendererPane doesn't paint anyway).<
	 */
	virtual AComponent::Ptr add(const AComponentSharedOrWeak &x, int index = -1,
			com::ArbitraryType::Ptr constraint = ArbitraryType::Ptr());
	//-------------------------------------------------------------------------
	/**
	 * Paint a cell renderer component c on graphics object g.  Before the component
	 * is drawn it's reparented to this (if that's necessary), it's bounds
	 * are set to w,h and the graphics object is (effectively) translated to x,y.
	 * If it's a JComponent, double buffering is temporarily turned off. After
	 * the component is painted it's bounds are reset to -w, -h, 0, 0 so that, if
	 * it's the last renderer component painted, it will not start consuming input.
	 * The Container p is the component we're actually drawing on, typically it's
	 * equal to this.getParent(). If shouldValidate is true the component c will be
	 * validated before painted.
	 */
	virtual void drawComponent(IDrawContext::Ptr cn, AComponentPtr c,
			AContainerPtr p, const Rectangle &bounds, bool shouldValidate);
	//-------------------------------------------------------------------------
	/**
	 * Calls this.paintComponent(g, c, p, x, y, w, h, false).
	 */
	void drawComponent(IDrawContext::Ptr cn, AComponentPtr c, AContainerPtr p,
			const Rectangle &bounds)
	{
		drawComponent(cn, c, p, bounds, false);
	}

}; // CellRendererPane
}}} // namespace(s)

#endif /* SAMBAG_CELLRENDERERPANE_H */
