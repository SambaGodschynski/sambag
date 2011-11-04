/*
 * Coordinate.hpp
 *
 *  Created on: Nov 28, 2011
 *      Author: samba
 */

#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include "sambag/com/Common.hpp"

namespace sambag { namespace disco {
//=============================================================================
/**
 * @class Coordinate class with support for SVG's coordinate style.
 * (http://www.w3.org/TR/SVG/coords.html#Units)
 * To get a final pixel value it is necessary to have access to a IDrawContext
 * Object an its metadata.
 * !!Decided to always use px as unit. It is to expensive to implement the
 * relative unit types. Trigger for this decision was that getBoundingBox()
 * will need a IDrawContextObject to resolve value.
 *
 */
struct Coordinate {
//=============================================================================
	//-------------------------------------------------------------------------
	enum Type {
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
};

}} // namespaces


#endif /* COORDINATE_HPP_ */
