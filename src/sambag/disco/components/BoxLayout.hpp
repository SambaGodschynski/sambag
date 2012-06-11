/*
 * BoxLayout.hpp
 *
 *  Created on: Mon Jun 11 14:06:51 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BOXLAYOUT_H
#define SAMBAG_BOXLAYOUT_H

#include <boost/shared_ptr.hpp>
#include "ALayoutManager.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>
#include "SizeRequirements.hpp"
#include <sambag/com/Thread.hpp>

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class BoxLayout.
  * A layout manager that allows multiple components to be laid out either
  * vertically or horizontally. The components will not wrap so, for
  * example, a vertical arrangement of components will stay vertically
  * arranged when the frame is resized.
  */
class BoxLayout : public ALayoutManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BoxLayout> Ptr;
	enum Axis {
	//-------------------------------------------------------------------------
	/**
	 * Specifies that components should be laid out left to right.
	 */
	X_AXIS,
	//-------------------------------------------------------------------------
	/**
	 * Specifies that components should be laid out top to bottom.
	 */
	Y_AXIS,
	//-------------------------------------------------------------------------
	/**
	 * Specifies that components should be laid out in the direction of
	 * a line of text as determined by the target AContainerPtr's
	 * <code>ComponentOrientation</code> property.
	 */
	LINE_AXIS,
	//-------------------------------------------------------------------------
	/**
	 * Specifies that components should be laid out in the direction that
	 * lines flow across a page as determined by the target AContainerPtr's
	 * <code>ComponentOrientation</code> property.
	 */
	PAGE_AXIS
	}; // enum Axis
protected:
private:
	//-------------------------------------------------------------------------
	AContainerPtr target;
	//-------------------------------------------------------------------------
	Axis axis;
	SizeRequirements::Container xChildren;
	SizeRequirements::Container yChildren;
	SizeRequirements xTotal;
	SizeRequirements yTotal;
	//-------------------------------------------------------------------------
	sambag::com::Mutex lock;
	//-------------------------------------------------------------------------
	void checkContainer(AComponentPtr comp) const;
	//-------------------------------------------------------------------------
	void checkRequests();
	//-------------------------------------------------------------------------
	Axis resolveAxis( Axis axis );
protected:
	//-------------------------------------------------------------------------
	/**
	 * Creates a layout manager that will lay out components along the given axis.
	 * @param target
	 * @param axis
	 */
	BoxLayout(AContainerPtr target, Axis axis);
public:
	//-------------------------------------------------------------------------
	static Ptr create(AContainerPtr target, Axis axis) {
		return Ptr(new BoxLayout(target, axis));
	}
	//-------------------------------------------------------------------------
	/**
	 * Not used by this class.
	 * @param comp the component
	 */
	void addLayoutComponent(AComponentPtr comp);
	//-------------------------------------------------------------------------
	/**
	 * Not used by this class.
	 * @param comp the component
	 */
	void removeLayoutComponent(AComponentPtr comp);
	//-------------------------------------------------------------------------
	AContainerPtr getTarget() const {
		return target;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the axis that was used to lay out components.
	 * @return
	 */
	Axis getAxis() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the alignment along the X axis for the AContainerPtr.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentX(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Returns the alignment along the Y axis for the AContainerPtr.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentY(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Indicates that a child has changed its layout related
	 * information, and thus any cached calculations should be flushed.
	 * @param target
	 */
	void invalidateLayout(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Called by the AWT when the specified AContainerPtr needs to be laid out.
	 * @param target
	 */
	void layoutContainer(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Returns the maximum dimensions the target AContainerPtr
	 * can use to lay out the components it contains.
	 * @param target
	 * @return
	 */
	Dimension maximumLayoutSize(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Returns the minimum dimensions needed to lay out the
	 * components contained in the specified target AContainerPtr.
	 * @param target
	 * @return
	 */
	Dimension minimumLayoutSize(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 *Returns the preferred dimensions for this layout,
	 *Returns given the components in the specified target AContainerPtr.
	 * @param target
	 * @return
	 */
	Dimension preferredLayoutSize(AContainerPtr target);
	//-----------------------------------------------------------------------------
	virtual std::string toString() const;

}; // BoxLayout
}}} // namespace(s)

#endif /* SAMBAG_BOXLAYOUT_H */
