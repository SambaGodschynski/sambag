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

namespace sambag { namespace disco {
//=============================================================================
/**
 * @class Coordinate
 */
struct Coordinate {
//=============================================================================
	//-------------------------------------------------------------------------
	enum Type { // TODO: remove types
			NONE,
			PX,
			IN,
			CM,
			MM,
			PT,
			PC,
			EM,
			EX,
			PERCENT
		};
	//-------------------------------------------------------------------------
	sambag::com::Number value;
	//-------------------------------------------------------------------------
	Type type;
	//-------------------------------------------------------------------------
	Coordinate(const sambag::com::Number value=0) : value(value), type(NONE) {}
	//-------------------------------------------------------------------------
	operator sambag::com::Number() const {
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
