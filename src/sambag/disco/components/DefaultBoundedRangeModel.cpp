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
		const Coordinate &extent, const Coordinate &minValue,
		const Coordinate &maxValue)
{
	if ((maxValue >= minValue) && (value >= minValue) && ((value + extent) >= value)
			&& ((value + extent) <= maxValue)) {
		this->value = value;
		this->extent = extent;
		this->minValue = minValue;
		this->maxValue = maxValue;
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
	return maxValue;
}
//-----------------------------------------------------------------------------
const Coordinate & DefaultBoundedRangeModel::getMinimum() const {
	return minValue;
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
	if (value + newExtent > maxValue) {
		newExtent = maxValue - value;
	}
	setRangeProperties(value, newExtent, minValue, maxValue, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setMaximum(const Coordinate & n) {
	Coordinate newMin = std::min(n, minValue);
	Coordinate newExtent = std::min(n - newMin, (Number)extent);
	Coordinate newValue = std::min(n - newExtent, (Number)value);
	setRangeProperties(newValue, newExtent, newMin, n, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setMinimum(const Coordinate & n) {
	Coordinate newMax = std::max(n, maxValue);
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
			!= minValue) || (newMax != maxValue) || (adjusting != isAdjusting);

	if (isChange) {
		value = newValue;
		extent = newExtent;
		minValue = newMin;
		maxValue = newMax;
		isAdjusting = adjusting;
		fireStateChanged();
	}
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setValue(const Coordinate & n) {
	Coordinate nv = std::min((Number)n, (Number)INT_MAX - extent);

	Coordinate newValue = std::max(nv, minValue);
	if (newValue + extent > maxValue) {
		newValue = maxValue - extent;
	}
	setRangeProperties(newValue, extent, minValue, maxValue, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultBoundedRangeModel::setValueIsAdjusting(bool b) {
	setRangeProperties(value, extent, minValue, maxValue, b);
}
//-----------------------------------------------------------------------------
std::string DefaultBoundedRangeModel::toString() const {
	return "DefaultBoundedRangeModel";
	std::stringstream ss;
	ss << "DefaultBoundedRangeModel ["
		<< "value=" << getValue() << ", "
		<< "extent=" << getExtent() << ", "
		<< "minValue=" << getMinimum() << ", "
		<< "maxValue=" << getMaximum() << ", "
		<< "adj=" << getValueIsAdjusting() << "]";
	return ss.str();
}

}}} // DefaultBoundedRangeModel::namespace(s)
