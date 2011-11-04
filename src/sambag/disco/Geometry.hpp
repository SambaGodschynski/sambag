/*
 * Geometry.hpp
 *
 *  Created on: Nov 21, 2011
 *      Author: samba
 */

#ifndef DISCO_GEOMETRY_HPP_
#define DISCO_GEOMETRY_HPP_

#include <boost/geometry.hpp>
#include "Coordinate.hpp"
#include "sambag/com/Common.hpp"
#include <math.h>

namespace sambag {
namespace disco {
//#############################################################################
// geometry
//#############################################################################
//=============================================================================
using namespace boost;
using namespace sambag::com;
/**
 * @typedef Point2D.
 */
typedef boost::geometry::model::d2::point_xy<Coordinate> Point2D;
//-----------------------------------------------------------------------------
inline bool operator==(const Point2D &a, const Point2D &b) {
	return boost::geometry::equals(a,b);
}
//-----------------------------------------------------------------------------
inline bool operator!=(const Point2D &a, const Point2D &b) {
	return !boost::geometry::equals(a,b);
}
//=============================================================================
extern const Point2D NULL_POINT2D;
//=============================================================================
//=============================================================================
/**
 * @class Rectangle.
 * extends boost::geometry::box
 */
class Rectangle: public boost::geometry::model::box<Point2D> {
	//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::geometry::model::box<Point2D> Base;
private:
public:
	//-------------------------------------------------------------------------
	const Point2D & getX0() const {
		return min_corner();
	}
	//-------------------------------------------------------------------------
	void setX0(const Point2D & val) {
		Point2D diff = val;
		geometry::subtract_point(diff, min_corner());
		geometry::add_point(min_corner(), diff);
		geometry::add_point(max_corner(), diff);
	}
	//-------------------------------------------------------------------------
	Number getWidth() const {
		Point2D result = max_corner();
		geometry::subtract_point(result, min_corner());
		return result.x();

	}
	//-------------------------------------------------------------------------
	Number getHeight() const {
		Point2D result = max_corner();
		geometry::subtract_point(result, min_corner());
		return result.y();
	}
	//-------------------------------------------------------------------------
	void setWidth(const Number &w) {
		max_corner().x(min_corner().x() + w);
	}
	//-------------------------------------------------------------------------
	void setHeight(const Number &h) {
		max_corner().y(min_corner().y() + h);
	}
	//-------------------------------------------------------------------------
	Rectangle(const Base &box) :
		Base(box) {
	}
	//-------------------------------------------------------------------------
	Rectangle(Point2D x0 = Point2D(0, 0), Point2D x1 = Point2D(0, 0)) :
		Base(x0, x1) {
	}
	//-------------------------------------------------------------------------
	Rectangle(Point2D _x0, const Number &width, const Number &height)

	{
		Point2D x0(_x0), x1(width, height);
		geometry::add_point(x1, x0);
		*this = Base(x0, x1);
	}
	//-------------------------------------------------------------------------
	Rectangle(const Number &x, const Number &y, const Number &width,
			const Number &height) {
		Point2D x0(x, y), x1(width, height);
		geometry::add_point(x1, x0);
		*this = Base(x0, x1);
	}
};
extern const Rectangle NULL_RECTANGLE;
}
} // namespaces


#endif /* GEOMETRY_HPP_ */
