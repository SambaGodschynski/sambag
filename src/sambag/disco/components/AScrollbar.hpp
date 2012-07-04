/*
 * AScrollbar.hpp
 *
 *  Created on: Mon Jul  2 15:01:44 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ASCROLLBAR_H
#define SAMBAG_ASCROLLBAR_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include <boost/foreach.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class AScrollbar.
  */
template<class ComponentModel>
class AScrollbar : public AContainer, public ComponentModel {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef ComponentModel Model;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AScrollbar> Ptr;
	//-------------------------------------------------------------------------
	enum Orientation {
		HORIZONTAL,
		VERTICAL,
		NO_ORIENTATION
	};
	//-------------------------------------------------------------------------
	enum Direction {
		INCR = -1,
		DECR = 1
	};
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_UNITINCREMENT;
	static const std::string PROPERTY_ORIENTATION;
	static const std::string PROPERTY_BLOCKINCREMENT;
protected:
	//-------------------------------------------------------------------------
	AScrollbar() : orientation(NO_ORIENTATION) {}
	//-------------------------------------------------------------------------
	Orientation orientation;
	Coordinate unitIncrement;
	Coordinate blockIncrement;
	//-------------------------------------------------------------------------
	void checkOrientation(Orientation orientation);
public:
	//-------------------------------------------------------------------------
	/**
	 * Returns the component's orientation (horizontal or vertical).
	 *
	 * @return VERTICAL or HORIZONTAL
	 */
	Orientation getOrientation() const {
		return orientation;
	}
	//-------------------------------------------------------------------------
	/**
	 * Set the scrollbar's orientation to either VERTICAL or
	 * HORIZONTAL.
	 *
	 * @exception IllegalArgumentException
	 * @see #getOrientation
	 */
	void setOrientation(Orientation orientation);
	//-------------------------------------------------------------------------
	/**
	 * Returns the amount to change the scrollbar's value by,
	 * given a unit up/down request.  A ScrollBarUI implementation
	 * typically calls this method when the user clicks on a scrollbar
	 * up/down arrow and uses the result to update the scrollbar's
	 * value.   Subclasses my override this method to compute
	 * a value, e.g. the change required to scroll up or down one
	 * (variable height) line text or one row in a table.
	 * <p>
	 * The JScrollPane component creates scrollbars (by default)
	 * that override this method and delegate to the viewports
	 * Scrollable view, if it has one.  The Scrollable interface
	 * provides a more specialized version of this method.
	 *
	 * @param direction is -1 or 1 for up/down respectively
	 * @return the value of the unitIncrement property
	 * @see #setUnitIncrement
	 * @see #setValue
	 * @see Scrollable#getScrollableUnitIncrement
	 */
	const Coordinate & getUnitIncrement(Direction direction) const {
		return unitIncrement;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the unitIncrement property.
	 * <p>
	 * Note, that if the argument is equal to the value of Integer.MIN_VALUE,
	 * the most look and feels will not provide the scrolling to the right/down.
	 * @see #getUnitIncrement
	 * @beaninfo
	 *   preferred: true
	 *       bound: true
	 * description: The scrollbar's unit increment.
	 */
	void setUnitIncrement(const Coordinate &unitIncrement) {
		Coordinate oldValue = this->unitIncrement;
		this->unitIncrement = unitIncrement;
		firePropertyChanged(PROPERTY_UNITINCREMENT, oldValue, unitIncrement);
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the amount to change the scrollbar's value by,
	 * given a block (usually "page") up/down request.  A ScrollBarUI
	 * implementation typically calls this method when the user clicks
	 * above or below the scrollbar "knob" to change the value
	 * up or down by large amount.  Subclasses my override this
	 * method to compute a value, e.g. the change required to scroll
	 * up or down one paragraph in a text document.
	 * <p>
	 * The JScrollPane component creates scrollbars (by default)
	 * that override this method and delegate to the viewports
	 * Scrollable view, if it has one.  The Scrollable interface
	 * provides a more specialized version of this method.
	 *
	 * @param direction is -1 or 1 for up/down respectively
	 * @return the value of the blockIncrement property
	 * @see #setBlockIncrement
	 * @see #setValue
	 * @see Scrollable#getScrollableBlockIncrement
	 */
	const Coordinate & getBlockIncrement(const Coordinate &direction) const
	{
		return blockIncrement;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the blockIncrement property.
	 * <p>
	 * Note, that if the argument is equal to the value of Integer.MIN_VALUE,
	 * the most look and feels will not provide the scrolling to the right/down.
	 * @see #getBlockIncrement()
	 * @beaninfo
	 *   preferred: true
	 *       bound: true
	 * description: The scrollbar's block increment.
	 */
	void setBlockIncrement(const Coordinate &blockIncrement) {
		int oldValue = this.blockIncrement;
		this.blockIncrement = blockIncrement;
		firePropertyChanged(PROPERTY_BLOCKINCREMENT, oldValue, blockIncrement);
	}
	//-------------------------------------------------------------------------
	/**
	 * For backwards compatibility with java.awt.Scrollbar.
	 * @see Adjustable#getUnitIncrement
	 * @see #getUnitIncrement(int)
	 */
	const Coordinate & getUnitIncrement() const {
		return unitIncrement;
	}
	//-------------------------------------------------------------------------
	/**
	 * For backwards compatibility with java.awt.Scrollbar.
	 * @see Adjustable#getBlockIncrement
	 * @see #getBlockIncrement(int)
	 */
	const Coordinate & getBlockIncrement() const {
		return blockIncrement;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the scrollbar's extent, aka its "visibleAmount".  In many
	 * scrollbar look and feel implementations the size of the
	 * scrollbar "knob" or "thumb" is proportional to the extent.
	 *
	 * @return the value of the model's extent property
	 */
	const Coordinate & getVisibleAmount() const {
		return Model::getExtent();
	}
	//-------------------------------------------------------------------------
	/**
	 * Set the model's extent property.
	 *
	 * @see #getVisibleAmount
	 * @see BoundedRangeModel#setExtent
	 */
	void setVisibleAmount(const Coordinate &extent) {
		Model::setExtent(extent);
	}
	//-------------------------------------------------------------------------
	void setValues(const Coordinate &newValue, const Coordinate &newExtent,
			const Coordinate &newMin, const Coordinate &newMax)
	{
		Model::setRangeProperties(newValue, newExtent, newMin,
			newMax, Model::getValueIsAdjusting());
	}
	//-------------------------------------------------------------------------
	virtual void setEnabled(bool b);

}; // AScrollbar
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class M>
const std::string AScrollbar<M>::PROPERTY_UNITINCREMENT = "unitIncrement";
//-----------------------------------------------------------------------------
template <class M>
const std::string AScrollbar<M>::PROPERTY_ORIENTATION = "orientation";
//-----------------------------------------------------------------------------
template <class M>
const std::string AScrollbar<M>::PROPERTY_BLOCKINCREMENT = "blockIncrement";
//-----------------------------------------------------------------------------
template <class M>
void AScrollbar<M>::setOrientation(AScrollbar::Orientation orientation) {
	checkOrientation(orientation);
	Orientation oldValue = this->orientation;
	this->orientation = orientation;
	firePropertyChanged(PROPERTY_ORIENTATION, oldValue, orientation);

	if (orientation != oldValue) {
		revalidate();
	}
}
//-----------------------------------------------------------------------------
template <class M>
void AScrollbar<M>::checkOrientation(AScrollbar::Orientation orientation) {
	switch (orientation) {
	case VERTICAL:
	case HORIZONTAL:
		break;
	default:
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
				"orientation must be one of: VERTICAL, HORIZONTAL"
		);
	}
}
//-----------------------------------------------------------------------------
template <class M>
void AScrollbar<M>::setEnabled(bool b) {
	Super::setEnabled(b);
	BOOST_FOREACH(AComponentPtr c, getComponents()) {
		c->setEnabled(b);
	}

}
}}} // namespace(s)

#endif /* SAMBAG_ASCROLLBAR_H */
