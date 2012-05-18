/*
 * PropertyChanged.hpp
 *
 *  Created on: Fri May 18 19:51:36 2012
 *      Author: Johannes Unger
 */

#include <string>
#include <sstream>

#ifndef SAMBAG_PROPERTYCHANGED_H
#define SAMBAG_PROPERTYCHANGED_H

namespace sambag { namespace com { namespace events {

//=============================================================================
/** 
  * @class PropertyChanged.
  */
struct PropertyChanged {
//=============================================================================
	std::string name, oldValue, newValue;
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @param v
	 * @param name
	 * @param oldValue
	 * @param newValue
	 */
	template <typename T>
	PropertyChanged(const std::string &name,
			const T &oldValue,
			const T &newValue) : name(name)
	{
		std::stringstream old, _new;
		old<<oldValue;
		_new << newValue;
		PropertyChanged::oldValue = old.str();
		PropertyChanged::newValue = _new.str();
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @param v
	 * @param name
	 * @param oldValue
	 * @param newValue
	 */
	PropertyChanged(const std::string &name,
			const std::string &oldValue,
			const std::string &newValue) :
				name(name), oldValue(oldValue), newValue(newValue)
	{
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @return oldValue converted to T
	 */
	template <typename T>
	void getOldValue(T &outVal) const {
		std::stringstream ss;
		ss << oldValue;
		ss >> outVal;
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @return newValue converted to T
	 */
	template <typename T>
	void getNewValue(T &outVal) const {
		std::stringstream ss;
		ss << newValue;
		ss >> outVal;
	}
}; // PropertyChanged
}}} // namespace(s)

#endif /* SAMBAG_PROPERTYCHANGED_H */
