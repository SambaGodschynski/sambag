/*
 * AComponentUI.hpp
 *
 *  Created on: Wed May 23 14:01:15 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COMPONENTUI_H
#define SAMBAG_COMPONENTUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/IDrawContext.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {

//=============================================================================
/** 
  * @class AComponentUI.
  * The base class for all UI delegate objects in the pluggable look and
  * feel architecture. The UI delegate object for a component is responsible
  * for implementing the aspects of the component that depend on the look and feel.
  */
class AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AComponentUI> Ptr;
protected:
	//-------------------------------------------------------------------------
	AComponentUI() {}
public:
	//-------------------------------------------------------------------------
	/**
	 * Returns true if the specified x,y location
	 * is contained within the look and feel's defined
	 * shape of the specified component.
	 * @param c
	 * @param x
	 * @param y
	 * @return
	 */
	virtual bool contains(AComponentPtr c, const Point2D &p);
	//-------------------------------------------------------------------------
	/**
	 * Returns the baseline.
	 * @param c
	 * @param width
	 * @param height
	 * @return
	 */
	virtual int getBaseline(AComponentPtr c, int width, int height);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's maximum size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getMaximumSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual Dimension getMinimumSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's preferred size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getPreferredSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Paints the specified component appropriately for the look and feel.
	 * @param cn
	 * @param c
	 */
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Reverses configuration which was done on the specified component
	 * during installUI.
	 * @param c
	 */
	virtual void uninstallUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Notifies this UI delegate that it is time to paint the specified component.
	 * @param cn
	 * @param c
	 */
	virtual void update(IDrawContext::Ptr cn, AComponentPtr c);

}; // AComponentUI
}}}} // namespace(s)

#endif /* SAMBAG_COMPONENTUI_H */
