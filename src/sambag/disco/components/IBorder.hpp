/*
 * IBorder.hpp
 *
 *  Created on: Wed May 16 15:20:28 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IBORDER_H
#define SAMBAG_IBORDER_H

#include <boost/shared_ptr.hpp>
#include "Forward.hpp"
#include <sambag/disco/IDrawContext.hpp>

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class IBorder.
  */
class IBorder {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IBorder> Ptr;
public:
	//-------------------------------------------------------------------------
	/**
	 * Returns the insets of the border.
	 * @param c
	 * @return
	 */
	virtual Insets getBorderInsets(AComponentPtr c) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Returns whether or not the border is opaque.
	 * @return
	 */
	virtual bool isBorderOpaque() = 0;
	//-------------------------------------------------------------------------
	/**
	 * Paints the border for the specified component with the specified
	 * position and size.
	 * @param c
	 * @param g
	 * @param x
	 * @param y
	 * @param width
	 * @param height
	 */
	virtual void paintBorder(AComponentPtr c,
			IDrawContext::Ptr g, const Rectangle &r) = 0;

}; // IBorder
}}} // namespace(s)

#endif /* SAMBAG_IBORDER_H */
