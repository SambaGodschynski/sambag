/*
 * DefaultBoundedRangeModel.hpp
 *
 *  Created on: Mon Jul  2 10:50:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTBOUNDEDRANGEMODEL_H
#define SAMBAG_DEFAULTBOUNDEDRANGEMODEL_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <sambag/disco/Geometry.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/com/events/ChangedEvent.hpp>
#include <sambag/com/events/Events.hpp>

namespace sambag { namespace disco { namespace components {
namespace sce = sambag::com::events;
class DefaultBoundedRangeModel;
typedef sce::ChangedEvent<DefaultBoundedRangeModel>
	DefaultBoundedRangeModelChanged;
//=============================================================================
/** 
  * @class DefaultBoundedRangeModel.
  */
class DefaultBoundedRangeModel :
	public sce::EventSender<DefaultBoundedRangeModelChanged>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef DefaultBoundedRangeModelChanged StateChangedEvent;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<DefaultBoundedRangeModel> Ptr;
protected:
	void fireStateChanged() {
		EventSender<DefaultBoundedRangeModelChanged>::notifyListeners(
				this, DefaultBoundedRangeModelChanged(*this)
		);
	}
private:
	Coordinate value;
	Coordinate extent;
	Coordinate minValue;
	Coordinate maxValue;
	sambag::com::ArithmeticWrapper<bool> isAdjusting;
public:
	//-------------------------------------------------------------------------
	DefaultBoundedRangeModel() : maxValue(100.) {}
	//-------------------------------------------------------------------------
	DefaultBoundedRangeModel(const Coordinate &value,
			const Coordinate &extent, const Coordinate &minValue,
			const Coordinate &maxValue);
	//-------------------------------------------------------------------------
	/**
	 * Returns the model's extent.
	 * @return
	 */
	const Coordinate & getExtent() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the model's maximum.
	 * @return
	 */
	const Coordinate & getMaximum() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the model's minimum.
	 * @return
	 */
	const Coordinate & getMinimum() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the model's current value.
	 * @return
	 */
	const Coordinate & getValue() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if the value is in the process of changing as a result of
	 * actions being taken by the user.
	 * @return
	 */
	bool getValueIsAdjusting() const;
	//-------------------------------------------------------------------------
	/**
	 * Sets the extent to n after ensuring that n is greater than or equal to
	 * zero and falls within the model's constraints:
	 * @param n
	 */
	void setExtent(const Coordinate & n);
	//-------------------------------------------------------------------------
	/**
	 * Sets the maximum to n after ensuring that n that the other three
	 * roperties obey the model's constraints:
	 * @param n
	 */
	void setMaximum(const Coordinate & n);
	//-------------------------------------------------------------------------
	/**
	 * 	Sets the minimum to n after ensuring that n that the other three
	 * 	properties obey the model's constraints:
	 * @param n
	 */
	void setMinimum(const Coordinate & n);
	//-------------------------------------------------------------------------
	/**
	 * Sets all of the BoundedRangeModel properties after forcing the
	 * arguments to obey the usual constraints:
	 * @param newValue
	 * @param newExtent
	 * @param newMin
	 * @param newMax
	 * @param adjusting
	 */
	void setRangeProperties(Coordinate newValue,
			Coordinate newExtent, Coordinate newMin,
			Coordinate newMax, bool adjusting);
	//-------------------------------------------------------------------------
	/**
	 * Sets the current value of the model.
	 * @param n
	 */
	void setValue(const Coordinate & n);
	//-------------------------------------------------------------------------
	/**
	 * Sets the valueIsAdjusting property.
	 * @param b
	 */
	void setValueIsAdjusting(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Returns a string that displays all of the BoundedRangeModel properties.
	 * @return
	 */
	std::string toString() const;
}; // DefaultBoundedRangeModel
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTBOUNDEDRANGEMODEL_H */
