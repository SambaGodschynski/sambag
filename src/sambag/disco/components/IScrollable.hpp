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
	 * @return
	 */
	virtual Dimension getPreferredScrollableViewportSize() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Components that display logical rows or columns should compute the
	 * scroll increment that will completely expose one block of rows or
	 * columns, depending on the value of orientation.
	 * @param visibleRect
	 * @param orientation
	 * @param direction
	 * @return
	 */
	virtual int getScrollableBlockIncrement(const Rectangle &visibleRect,
			int orientation, int direction) const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Return true if a viewport should always force the height of this
	 * Scrollable to match the height of the viewport.
	 * @return
	 */
	virtual bool getScrollableTracksViewportHeight() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Return true if a viewport should always force the width of this
	 * Scrollable to match the width of the viewport.
	 * @return
	 */
	virtual bool getScrollableTracksViewportWidth() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Components that display logical rows or columns should compute the
	 * scroll increment that will completely expose one new row or column,
	 * depending on the value of orientation.
	 * @param visibleRect
	 * @param orientation
	 * @param direction
	 * @return
	 */
	virtual int getScrollableUnitIncrement(const Rectangle &visibleRect,
		int orientation, int direction) const = 0;

}; // IScrollable
}}} // namespace(s)

#endif /* SAMBAG_ISCROLLABLE_H */
