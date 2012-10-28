/*
 * Coordinate.hpp
 *
 *  Created on: Nov 28, 2011
 *      Author: samba
 */

#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include "sambag/com/Common.hpp"
#include <string>
#include <ostream>
#include <boost/serialization/access.hpp>

namespace sambag { namespace disco {
//=============================================================================
/**
 * @class Coordinate
 * TODO: change to fundamental type.
 */
struct Coordinate {
//=============================================================================
	//-------------------------------------------------------------------------
private:
	sambag::com::Number value;
	//-------------------------------------------------------------------------
	friend class boost::serialization::access;
	//-------------------------------------------------------------------------
	template <typename Archive> 
	void serialize(Archive &ar, const unsigned int version) {
		ar & value;
	}
	//-------------------------------------------------------------------------
public:
	Coordinate(const sambag::com::Number value=0) : value(value) {}
	//-------------------------------------------------------------------------
	inline operator sambag::com::Number() const {
		return value;
	}
	//-------------------------------------------------------------------------
	std::string toString() const;
	//-------------------------------------------------------------------------
	void operator +=(const Coordinate &v) {
		value +=v;
	}
	//-------------------------------------------------------------------------
	void operator -=(const Coordinate &v) {
		value -=v;
	}
	//-------------------------------------------------------------------------
	void operator *=(const Coordinate &v) {
		value *=v;
	}
	//-------------------------------------------------------------------------
	void operator /=(const Coordinate &v) {
		value /=v;
	}
};
inline std::ostream & operator<<(std::ostream &os, const Coordinate &obj) {
	os << obj.toString();
	return os;
}
}} // namespaces


#endif /* COORDINATE_HPP_ */
