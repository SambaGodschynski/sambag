/*
 * IScrollable.hpp
 *
 *  Created on: Sat Jun 23 20:36:26 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ISCROLLABLE_H
#define SAMBAG_ISCROLLABLE_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/Geometry.hpp>
#include "AScrollbar.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class IScrollable.
  */
class IScrollable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IScrollable> Ptr;
	//-------------------------------------------------------------------------
	/**
	 * Returns the preferred size of the viewport for a view component.
	 * @note: no const here !!
	 * @return
	 */
	virtual Dimension getPreferredScrollableViewportSize() = 0;
	//-------------------------------------------------------------------------
	/**
	 * Components that display logical rows or columns should compute the
	 * scroll increment that will completely expose one block of rows or
	 * columns, depending on the value of orientation.
	 * @param visibleRect
	 * @param orientation Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
	 * @param direction Less than zero to scroll up/left, greater than zero for down/right.
	 * @return
	 */
	virtual Coordinate getScrollableBlockIncrement(
			const Rectangle &visibleRect,
			ScrollbarConstants::Orientation orientation,
			ScrollbarConstants::Direction direction) const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Return true if a viewport should always force the height of this
	 * Scrollable to match the height of the viewport.
	 * @note: no const here !!
	 * @return
	 */
	virtual bool getScrollableTracksViewportHeight() = 0;
	//-------------------------------------------------------------------------
	/**
	 * Return true if a viewport should always force the width of this
	 * Scrollable to match the width of the viewport.
	 * @note: no const here !!
	 * @return
	 */
	virtual bool getScrollableTracksViewportWidth() = 0;
	//-------------------------------------------------------------------------
	/**
	 * Components that display logical rows or columns should compute the
	 * scroll increment that will completely expose one new row or column,
	 * depending on the value of orientation.
	 * @param visibleRect
	 * @param orientation Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
	 * @param direction Less than zero to scroll up/left, greater than zero for down/right.
	 * @return
	 */
	virtual Coordinate getScrollableUnitIncrement(
			const Rectangle &visibleRect,
			ScrollbarConstants::Orientation orientation,
			ScrollbarConstants::Direction direction) const = 0;

}; // IScrollable
}}} // namespace(s)

#endif /* SAMBAG_ISCROLLABLE_H */
