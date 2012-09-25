/*
 * DefaultBoundedRangeModel.cpp
 *
 *  Created on: Mon Jul  2 10:50:11 2012
 *      Author: Johannes Unger
 */

#include "DefaultBoundedRangeModel.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class DefaultBoundedRangeModel
//=============================================================================
//-----------------------------------------------------------------------------
DefaultBoundedRangeModel::DefaultBoundedRangeModel(const Coordinate &value,
		const Coordinate &extent, const Coordinate &min,
		const Coordinate &max)
{
	if ((max >= min) && (value >= min) && ((value + extent) >= value)
			&& ((value + extent) <= max)) {
		this->value = value;
		this->extent = extent;
		this->min = min;
		this->max = max;
	} else {
		using namespace sambag::com::exceptions;
		SAMBAG_THROW(IllegalArgumentException, "invalid range properties");
	}
}
//-----------------------------------------------------------------------------
const Coordinate & DefaultBoundedRangeModel::getExtent() const {
	return extent;
}
//-----------------------------------------------------------------------------
const Coordinate & DefaultBoundedRangeModel::getMaximum() const {
	return max;
}
//-----------------------------------------------------------------------------
const Coordinate & DefaultBoundedRangeModel::getMinimum() const {
	return min;
}
//-----------------------------------------------------------------------------
const Coordinate & DefaultBoundedRangeModel::getValue() const {
	return value;
}
//-----------------------------------------------------------------------------
bool DefaultBoundedRangeModel::getValueIsAdjusting() const {
	return isAdjusting;
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setExtent(const Coordinate & n) {
	Coordinate newExtent = std::max((Coordinate)0, n);
	if (value + newExtent > max) {
		newExtent = max - value;
	}
	setRangeProperties(value, newExtent, min, max, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setMaximum(const Coordinate & n) {
	Coordinate newMin = std::min(n, min);
	Coordinate newExtent = std::min(n - newMin, (Number)extent);
	Coordinate newValue = std::min(n - newExtent, (Number)value);
	setRangeProperties(newValue, newExtent, newMin, n, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setMinimum(const Coordinate & n) {
	Coordinate newMax = std::max(n, max);
	Coordinate newValue = std::max(n, value);
	Coordinate newExtent = std::min(newMax - newValue, (Number)extent);
	setRangeProperties(newValue, newExtent, n, newMax, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setRangeProperties(
		Coordinate newValue, Coordinate newExtent,
		Coordinate newMin, Coordinate newMax, bool adjusting)
{
	if (newMin > newMax) {
		newMin = newMax;
	}
	if (newValue > newMax) {
		newMax = newValue;
	}
	if (newValue < newMin) {
		newMin = newValue;
	}
	/* Convert the addends to long so that extent can be
	 * Integer.MAX_VALUE without rolling over the sum.
	 */
	if (((long) newExtent + (long) newValue) > newMax) {
		newExtent = newMax - newValue;
	}

	if (newExtent < 0) {
		newExtent = 0;
	}

	bool isChange = (newValue != value) || (newExtent != extent) || (newMin
			!= min) || (newMax != max) || (adjusting != isAdjusting);

	if (isChange) {
		value = newValue;
		extent = newExtent;
		min = newMin;
		max = newMax;
		isAdjusting = adjusting;
		fireStateChanged();
	}
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setValue(const Coordinate & n) {
	Coordinate nv = std::min((Number)n, (Number)INT_MAX - extent);

	Coordinate newValue = std::max(nv, min);
	if (newValue + extent > max) {
		//newValue = max - extent;
	}
	setRangeProperties(newValue, extent, min, max, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setValueIsAdjusting(bool b) {
	setRangeProperties(value, extent, min, max, b);
}
//-----------------------------------------------------------------------------
std::string DefaultBoundedRangeModel::toString() const {
	return "DefaultBoundedRangeModel";
	std::stringstream ss;
	ss << "DefaultBoundedRangeModel ["
		<< "value=" << getValue() << ", "
		<< "extent=" << getExtent() << ", "
		<< "min=" << getMinimum() << ", "
		<< "max=" << getMaximum() << ", "
		<< "adj=" << getValueIsAdjusting() << "]";
	return ss.str();
}

}}} // DefaultBoundedRangeModel::namespace(s)
