/*
 * PropertyChanged.hpp
 *
 *  Created on: Fri May 18 19:51:36 2012
 *      Author: Johannes Unger
 */

#include <sambag/com/Exception.hpp>
#ifndef SAMBAG_PROPERTYCHANGED_H
#define SAMBAG_PROPERTYCHANGED_H

namespace sambag {
namespace com {
namespace events {

//=============================================================================
struct PropertyContainerBase {
	virtual ~PropertyContainerBase() {
	}
};
//=============================================================================
template<typename T>
struct PropertyContainer: public PropertyContainerBase {
	T oldValue;
	T newValue;
	PropertyContainer(const T&o, const T&n) :
		oldValue(o), newValue(n) {
	}
};

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
	PropertyContainerBase *content;
public:
	//-------------------------------------------------------------------------
	SAMBAG_EXCEPTION_CLASS(IncompatibleType);
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
			const T &newValue) :
		name(name), content(NULL) {
		content = new PropertyContainer<T> (oldValue, newValue);
	}
	//-------------------------------------------------------------------------
	/**
	 * @param v
	 * @param name
	 * @param oldValue
	 * @param newValue
	 */
	PropertyChanged(const std::string &name, const char *oldValue,
			const char *newValue) :
		name(name), content(NULL) {
		content = new PropertyContainer<std::string> (oldValue, newValue);
	}
	//-------------------------------------------------------------------------
	virtual ~PropertyChanged() {
		if (content)
			delete content;
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @return oldValue converted to T
	 * @throw IncompatibleType
	 */
	template<typename T>
	void getOldValue(T &outVal) const {
		typedef PropertyContainer<T> DestT;
		DestT * cn = dynamic_cast<DestT*> (content);
		if (!cn)
			SAMBAG_RAISE_ERROR(IncompatibleType, "cannot convert newValue to T");
		outVal = cn->oldValue;
	}
	//-------------------------------------------------------------------------
	/**
	 * uses stringstream to convert between T and string
	 * @return newValue converted to T
	 * @throw IncompatibleType
	 */
	template<typename T>
	void getNewValue(T &outVal) const {
		typedef PropertyContainer<T> DestT;
		DestT * cn = dynamic_cast<DestT*> (content);
		if (!cn)
			SAMBAG_RAISE_ERROR(IncompatibleType, "cannot convert newValue to T");
		outVal = cn->newValue;
	}
}; // PropertyChanged
}
}
} // namespace(s)

#endif /* SAMBAG_PROPERTYCHANGED_H */
