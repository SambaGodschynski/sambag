/*
 * Geometry.hpp
 *
 *  Created on: Nov 21, 2011
 *      Author: samba
 */

#ifndef DISCO_GEOMETRY_HPP_
#define DISCO_GEOMETRY_HPP_

#include "Coordinate.hpp"
#include "sambag/com/Common.hpp"
#include <math.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace sambag { namespace disco {
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
//-----------------------------------------------------------------------------
/**
 * @param a
 * @param b
 * @return a point contains the minimum values of a and b.
 * e.g: a(3,4) b(4,1) => (3,1)
 */
inline Point2D minimize(const Point2D &a, const Point2D &b) {
	return Point2D(
		a.x() < b.x() ? a.x() : b.x(),
		a.y() < b.y() ? a.y() : b.y()
	);
}
/**
 * @param a
 * @param b
 * @return a point with the contains values of a and b.
 * e.g: a(3,4) b(4,1) => (4,4)
 */
inline Point2D maximize(const Point2D &a, const Point2D &b) {
	return Point2D(
		a.x() > b.x() ? a.x() : b.x(),
		a.y() > b.y() ? a.y() : b.y()
	);
}
//=============================================================================
extern const Point2D NULL_POINT2D;

//=============================================================================
/**
 * @class Dimension
 */
struct Dimension : public Point2D {
//=============================================================================
	//-------------------------------------------------------------------------
	Dimension(const Coordinate &width=0, const Coordinate &height=0) :
		Point2D(width, height) {}
	//-------------------------------------------------------------------------
	const Coordinate & width() const {
		return Point2D::x();
	}
	//-------------------------------------------------------------------------
	const Coordinate & height() const {
		return Point2D::y();
	}
	//-------------------------------------------------------------------------
	void width(const Coordinate &v) {
		Point2D::x(v);
	}
	//-------------------------------------------------------------------------
	void height(const Coordinate &v) {
		Point2D::y(v);
	}
};
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
	const Point2D & x0() const {
		return min_corner();
	}
	//-------------------------------------------------------------------------
	Point2D & x0() {
		return min_corner();
	}
	//-------------------------------------------------------------------------
	const Point2D & x1() const {
		return max_corner();
	}
	//-------------------------------------------------------------------------
	Point2D & x1() {
		return max_corner();
	}
	//-------------------------------------------------------------------------
	/**
	 * translates whole rectangle to x0
	 * @param val
	 */
	void translate(const Point2D & val) {
		Point2D diff = val;
		geometry::subtract_point(diff, min_corner());
		geometry::add_point(min_corner(), diff);
		geometry::add_point(max_corner(), diff);
	}
	//-------------------------------------------------------------------------
	Coordinate getWidth() const {
		Point2D result = max_corner();
		geometry::subtract_point(result, min_corner());
		return result.x();

	}
	//-------------------------------------------------------------------------
	Coordinate getHeight() const {
		Point2D result = max_corner();
		geometry::subtract_point(result, min_corner());
		return result.y();
	}
	//-------------------------------------------------------------------------
	void setWidth(const Coordinate &w) {
		max_corner().x(min_corner().x() + w);
	}
	//-------------------------------------------------------------------------
	void setHeight(const Coordinate &h) {
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
	Rectangle(Point2D _x0, const Coordinate &width, const Coordinate &height)

	{
		Point2D x0(_x0), x1(width, height);
		geometry::add_point(x1, x0);
		*this = Base(x0, x1);
	}
	//-------------------------------------------------------------------------
	Rectangle(const Coordinate &x, const Coordinate &y, const Coordinate &width,
			const Coordinate &height) {
		Point2D x0(x, y), x1(width, height);
		geometry::add_point(x1, x0);
		*this = Base(x0, x1);
	}
	//-------------------------------------------------------------------------
	bool operator==(const Rectangle &b) const {
		return boost::geometry::equals<Base, Base>(*this, b);
	}
	//-------------------------------------------------------------------------
	bool operator!=(const Rectangle &b) const {
		return !(*this==b);
	}
};
//=============================================================================
/**
 * @class Insets.
 * An Insets object is a representation of the borders of a container.
 * It specifies the space that a container must leave at each of its edges.
 * The space can be a border, a blank space, or a title.
 */
class Insets : Rectangle {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::geometry::model::box<Point2D> Base;
private:
public:
	//-------------------------------------------------------------------------
	void left(Coordinate &v) {
		min_corner().x(v);
	}
	//-------------------------------------------------------------------------
	const Coordinate & left() const {
		return min_corner().x();
	}
	//-------------------------------------------------------------------------
	void top(Coordinate &v) {
		min_corner().y(v);
	}
	//-------------------------------------------------------------------------
	const Coordinate & top() const {
		return min_corner().y();
	}
	//-------------------------------------------------------------------------
	void right(Coordinate &v) {
		max_corner().x(v);
	}
	//-------------------------------------------------------------------------
	const Coordinate & right() const {
		return max_corner().x();
	}
	//-------------------------------------------------------------------------
	void bottom(Coordinate &v) {
		max_corner().y(v);
	}
	//-------------------------------------------------------------------------
	const Coordinate & bottom() const {
		return max_corner().y();
	}
	//-------------------------------------------------------------------------
	Insets(const Coordinate &left = 0,
			const Coordinate &top = 0,
			const Coordinate &right = 0,
			const Coordinate &bottom = 0) :
		Rectangle(Point2D(left, top), Point2D(right, bottom))
	{
	}
};
extern const Rectangle NULL_RECTANGLE;
}
} // namespaces


#endif /* GEOMETRY_HPP_ */
