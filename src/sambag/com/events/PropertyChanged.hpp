/*
 * PropertyChanged.hpp
 *
 *  Created on: Fri May 18 19:51:36 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_PROPERTYCHANGED_H
#define SAMBAG_PROPERTYCHANGED_H

#include <sambag/com/Exception.hpp>
#include <sambag/com/AbstractType.hpp>
#include <utility>
#include <string>

namespace sambag { namespace com { namespace events {
//=============================================================================
/** 
 * @class PropertyChanged.
 */
class PropertyChanged {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	std::string name;
	//-------------------------------------------------------------------------
	AbstractType::Ptr content;
public:
	//-------------------------------------------------------------------------
	const std::string & getPropertyName() const {
		return name;
	}
	//-------------------------------------------------------------------------
	/**
	 * @param v
	 * @param name
	 * @param oldValue
	 * @param newValue
	 */
	template<typename T>
	PropertyChanged(const std::string &name, const T &oldValue,
			const T &newValue) : name(name)
	{
		typedef std::pair<T, T> Content;
		content =
			ConcreteType<Content>::create(std::make_pair(oldValue, newValue));
	}
	//-------------------------------------------------------------------------
	/**
	 * @param v
	 * @param name
	 * @param oldValue
	 * @param newValue
	 */
	PropertyChanged(const std::string &name, const char *oldValue,
			const char *newValue) : name(name)
	{
		typedef std::pair<std::string, std::string> Content;
		content =
			ConcreteType<Content>::create(
					std::make_pair(std::string(oldValue), std::string(newValue))
		);
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @return oldValue converted to T
	 * @throw IncompatibleType
	 */
	template<typename T>
	void getOldValue(T &outVal) const {
		typedef std::pair<T, T> Content;
		Content cn;
		get(content, cn);
		outVal = cn.first;
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @return newValue converted to T
	 * @throw IncompatibleType
	 */
	template<typename T>
	void getNewValue(T &outVal) const {
		typedef std::pair<T, T> Content;
		Content cn;
		get(content, cn);
		outVal = cn.second;
	}
}; // PropertyChanged
}}} // namespace(s)

#endif /* SAMBAG_PROPERTYCHANGED_H */
