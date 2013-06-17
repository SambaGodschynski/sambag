/*
 * ArbitraryType.hpp
 *
 *  Created on: Thu May 24 21:19:18 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ABSTRACTTYPE_H
#define SAMBAG_ABSTRACTTYPE_H

#include <sambag/com/Exception.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace sambag { namespace com {

//=============================================================================
/** 
  * @class ArbitraryType.
  * Use this instead of unsafe void* constructions.
  */
class ArbitraryType {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ArbitraryType> Ptr;
	//-------------------------------------------------------------------------
	virtual ~ArbitraryType() {}
}; // ArbitraryType
//=============================================================================
/**
  * @class ArbitraryType.
  */
template <typename T>
class ConcreteType : public ArbitraryType {
//=============================================================================
protected:
	//-------------------------------------------------------------------------
	ConcreteType() {}
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ConcreteType> Ptr;
	//-------------------------------------------------------------------------
	T value;
	//-------------------------------------------------------------------------
	operator T() const { return value; }
	//-------------------------------------------------------------------------
	static Ptr create(const T &value) {
		Ptr res(new ConcreteType());
		res->value = value;
		return res;
	}
}; // ArbitraryType
//=============================================================================
/**
 * Gets value of ArbitraryType.
 * @param src
 * @param target
 */
template <typename T>
void get(ArbitraryType::Ptr src, T &target) {
	typedef ConcreteType<T> Dst;
	typedef boost::shared_ptr<Dst> DstPtr;
	DstPtr dst = boost::dynamic_pointer_cast<Dst>(src);
	if (!dst)
		return;
	target = *dst;
}
//=============================================================================
template <typename T>
boost::shared_ptr< ConcreteType<T> > createObject(const T &v) {
	return ConcreteType<T>::create(v);
}
//-----------------------------------------------------------------------------
inline boost::shared_ptr< ConcreteType< std::string> >
createObject(const char *v) {
	return ConcreteType<std::string>::create(std::string(v));
}
}} // namespace(s)

#endif /* SAMBAG_ABSTRACTTYPE_H */
