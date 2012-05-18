/*
 * ArithmeticWrapper.hpp
 *
 *  Created on: Fri May 18 14:40:59 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ARITHMETICWRAPPER_H
#define SAMBAG_ARITHMETICWRAPPER_H

// 'declaration does not declare anything' issue on mac. see:
// http://stackoverflow.com/questions/8173620/c-boost-1-48-type-traits-and-cocoa-inclusion-weirdness
#ifdef __APPLE__
	#ifdef check
	#undef check
	#endif
#endif
#include <boost/type_traits.hpp>

namespace sambag { namespace com {
//=============================================================================
/** 
  * @class ArithmeticWrapper.
  * Useful in classes, so its going to be unnecessary to implement an extra
  * constructor initializer.
  */
template <typename T, int InitValue=0>
class ArithmeticWrapper {
//=============================================================================
BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);
private:
	//-------------------------------------------------------------------------
	T value;
public:
	//-------------------------------------------------------------------------
	ArithmeticWrapper(T value = (T)InitValue) : value(value) {}
	//-------------------------------------------------------------------------
	inline operator T() const {
		return value;
	}
	//-------------------------------------------------------------------------
	inline T operator +=(T b) {
		value+=b;
		return value;
	}
	//-------------------------------------------------------------------------
	inline T operator -=(T b) {
		value-=b;
		return value;
	}
	//-------------------------------------------------------------------------
	inline T operator *=(T b) {
		value*=b;
		return value;
	}
	//-------------------------------------------------------------------------
	inline T operator /=(T b) {
		value/=b;
		return value;
	}
}; // ArithmeticWrapper
}} // namespace(s)

#endif /* SAMBAG_ARITHMETICWRAPPER_H */
