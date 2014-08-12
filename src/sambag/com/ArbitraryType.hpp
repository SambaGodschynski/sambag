/*
 * ArbitraryType.hpp
 *
 *  Created on: Thu May 24 21:19:18 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ABSTRACTTYPE_H
#define SAMBAG_ABSTRACTTYPE_H

#include <sambag/com/Exception.hpp>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace sambag { namespace com {

// ArbitraryType: now I recognize how clunky
// this name is. I implemented this class
// before without the knowledge what
// boost::any is doing.
// TODO: for the future, replace all Arbitrary occurrences with any.
struct ArbitraryType {
	boost::any value;
	typedef boost::shared_ptr<ArbitraryType> Ptr;
};


//=============================================================================
/**
 * Gets value of ArbitraryType.
 * @param src
 * @param target
 */
template <typename T>
bool get(ArbitraryType::Ptr src, T &target) {
	try {
		target = boost::any_cast<T>(src->value);
	} catch(...) {
		return false;
	}
	return true;
}
//=============================================================================
template <typename T>
ArbitraryType::Ptr createObject(const T &v) {
	ArbitraryType::Ptr res( new ArbitraryType() );
	res->value = v;
	return res;
}
//-----------------------------------------------------------------------------
inline ArbitraryType::Ptr createObject(const char *v) {
	ArbitraryType::Ptr res( new ArbitraryType() );
	res->value = std::string(v);
	return res;
}
}} // namespace(s)

#endif /* SAMBAG_ABSTRACTTYPE_H */
