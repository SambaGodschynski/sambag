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

#define NULL_POINT2D sambag::disco::getNullPoint2D()
#define NULL_DIMENSION sambag::disco::getNullDimension()
#define NULL_RECTANGLE sambag::disco::getNullRectangle()

namespace sambag { namespace disco {
//#############################################################################
// geometry
//#############################################################################
//=============================================================================
using namespace sambag::com;
//=============================================================================
/**
 * @typedef Point2D.
 */
typedef boost::geometry::model::d2::point_xy<Coordinate> Point2D;
//=============================================================================
//-----------------------------------------------------------------------------
inline bool operator==(const Point2D &a, const Point2D &b) {
	return boost::geometry::equals(a,b);
}
//-----------------------------------------------------------------------------
inline bool operator!=(const Point2D &a, const Point2D &b) {
	return !boost::geometry::equals(a,b);
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
// Point Helper
///////////////////////////////////////////////////////////////////////////////
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
//-----------------------------------------------------------------------------
/**
 * @return p+=Point2D(x,y)
 * @param x
 * @param y
 */
inline Point2D 
addTo(const Point2D &p, const Coordinate& x, const Coordinate &y = 0.) 
{
	Point2D r = p;
	r.x( r.x() + x );
	r.y( r.y() + y );
	return r;
}
//-----------------------------------------------------------------------------
/**
 * @return p*=Point2D(x,y)
 * @param x
 * @param y
 */
inline Point2D 
mulTo(const Point2D &p, const Coordinate& x, const Coordinate &y = 0.) 
{
	Point2D r = p;
	r.x( r.x() * x );
	r.y( r.y() * y );
	return r;
}
//=============================================================================
inline Point2D getNullPoint2D() {
	return Point2D(NULL_NUMBER, NULL_NUMBER);
}
//=============================================================================
inline std::ostream & operator << (std::ostream &os, const Point2D &p)
{
	os << "Point2D(" << p.x() << ", " << p.y() << ")";
	return os;
}

//=============================================================================
/**
 * @class Dimension
 */
class Dimension {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	Point2D size;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	Dimension(const Coordinate &width=0, const Coordinate &height=0) :
		size(width, height) {}
	//-------------------------------------------------------------------------
	const Coordinate & width() const {
		return size.x();
	}
	//-------------------------------------------------------------------------
	const Coordinate & height() const {
		return size.y();
	}
	//-------------------------------------------------------------------------
	Dimension & width(const Coordinate &v) {
		size.x(v);
        return *this;
	}
	//-------------------------------------------------------------------------
	Dimension & height(const Coordinate &v) {
		size.y(v);
        return *this;
	}
	//-------------------------------------------------------------------------
	bool operator==(const Dimension &b) const {
		return size == b.size;
	}
	//-------------------------------------------------------------------------
	bool operator!=(const Dimension &b) const {
		return size != b.size;
	}
};
inline Dimension getNullDimension() {
	return Dimension(NULL_NUMBER, NULL_NUMBER);
}
//=============================================================================
/**
 * @class Rectangle.
 */
class Rectangle {
//=============================================================================
private:
	Point2D _x0;
	Point2D _size;
public:
	typedef boost::geometry::coordinate_type<Point2D>::type CoordinateType;
	// constructors ///////////////////////////////////////////////////////////
	Rectangle() {}
	Rectangle(const Dimension &dim) :
		_x0(0,0), _size(dim.width(), dim.height()) {}
	Rectangle(Point2D _x0, const Coordinate &width, const Coordinate &height) :
		_x0(_x0), _size(width, height)
	{
	}
	Rectangle(const Coordinate &x, const Coordinate &y,
		  const Coordinate &w, const Coordinate &h) : _x0(x, y), _size(w, h) {}
	Rectangle(const Point2D &x0, const Dimension &size)
		: _x0(x0), _size(size.width(), size.height()) {}
	Rectangle(const Point2D &x0_, const Point2D &x1_) {
		x0(minimize(x0_, x1_));
		x1(maximize(x0_, x1_));
	}
	// getter /////////////////////////////////////////////////////////////////
	const Coordinate & x() const { return _x0.x(); }
	const Coordinate & y() const { return _x0.y(); }
	const Coordinate & width() const { return _size.x(); }
	const Coordinate & height() const { return _size.y(); }
	const Point2D & x0() const { return _x0; }
	Dimension size() const { return Dimension(_size.x(), _size.y()); }
	Point2D x1() const {
		Point2D res = _x0;
		boost::geometry::add_point(res, _size);
		return res;
	}
	// setter /////////////////////////////////////////////////////////////////
	void x0(const Point2D &val) { _x0 = val; }
	void x1(const Point2D &val) {
		_size = val;
		boost::geometry::subtract_point(_size, _x0);
	}
	Rectangle & x(const Coordinate &val) {
        _x0.x(val);
        return *this;
    }
	Rectangle & y(const Coordinate &val) {
        _x0.y(val);
        return *this;
    }
	Rectangle & width(const Coordinate &val) {
        _size.x(val);
        return *this;
    }
	Rectangle & height(const Coordinate &val) {
        _size.y(val);
        return *this;
    }
	Rectangle & size(const Dimension &val) {
		width(val.width());
		height(val.height());
        return *this;
	}
	bool isEmpty() const {
		return boost::geometry::equals(x0(), x1());
	}
	// handy stuff ////////////////////////////////////////////////////////////
	bool contains(const Point2D &p) const {
		Point2D _x0 = x0();
		Point2D _x1 = x1();
		return p.x() > _x0.x() && p.y() > _x0.y() &&
				p.x() < _x1.x() && p.y() < _x1.y();
	}
	bool operator==(const Rectangle &b) const {
		return boost::geometry::equals(*this, b);
	}
	bool operator!=(const Rectangle &b) const {
		return !(*this==b);
	}
	Rectangle & inset(const Coordinate &_x, const Coordinate &_y) {
		Coordinate w = _x / 2.;
		x( x() - w );
		y( y() - w );
		width( width() + _y );
		height( height() + _y );
        return *this;
	}
	// deprecated stuff ///////////////////////////////////////////////////////
	/**
	 * @deprecated Rectangle extends boost::geometry::box<> formally
	 */
	typedef Rectangle Base;
	/**
	 * @deprecated use witdh()
	 */
	const Coordinate & getWidth() const { return _size.x(); }
	/**
	 * @deprecated use height()
	 */
	const Coordinate & getHeight() const { return _size.y(); }
	/**
	 * @deprecated use setWitdh()
	 */
	void setWidth(const Coordinate &val) { _size.x(val); }
	/**
	 * @deprecated use height()
	 */
	void setHeight(const Coordinate &val) { _size.y(val); }
	/**
	 * @deprecated use x0()
	 * translates whole rectangle to x0
	 * @param val
	 */
	void translate(const Point2D & val) { _x0 = val; }
	/**
	 * @deprecated use size()
	 * translates whole rectangle to x0
	 * @param val
	 */
	Dimension getDimension() const { return size(); }
};
}} // namespace sambag::disco
///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace geometry { namespace traits {
template <>
struct tag<sambag::disco::Rectangle> {
	typedef box_tag type;
};
template<>
struct point_type<sambag::disco::Rectangle> {
	typedef sambag::disco::Point2D type;
};
template<std::size_t Dimension>
struct indexed_access<sambag::disco::Rectangle, min_corner, Dimension> {
	typedef sambag::disco::Rectangle::CoordinateType coordinate_type;

	static inline coordinate_type get(sambag::disco::Rectangle const& b) {
		return geometry::get<Dimension>(b.x0());
	}

	static inline void set(sambag::disco::Rectangle& b,
			coordinate_type const& value) {
		sambag::disco::Point2D p = b.x0();
		geometry::set<Dimension>(p, value);
		b.x0(p);
	}
};
template<std::size_t Dimension>
struct indexed_access<sambag::disco::Rectangle, max_corner, Dimension> {
	typedef sambag::disco::Rectangle::CoordinateType coordinate_type;

	static inline coordinate_type get(sambag::disco::Rectangle const& b) {
		return geometry::get<Dimension>(b.x1());
	}

	static inline void set(sambag::disco::Rectangle& b,
			coordinate_type const& value) {
		sambag::disco::Point2D p = b.x1();
		geometry::set<Dimension>(p, value);
		b.x1(p);
	}
};
}}} // namespace boost::geometry::traits
///////////////////////////////////////////////////////////////////////////////
namespace sambag { namespace disco {
//-----------------------------------------------------------------------------
inline Rectangle union_(const Rectangle &a, const Rectangle &b) {
	/*
	 * boost geometries alg. unfortunately doesn't work:
	 * no type named ‘const_reference’ in
	 * ‘class boost::geometry::model::box<boost::geometry::model::d2::
	 * 		point_xy<sambag::disco::Coordinate> >’
	 */
	return Rectangle(
			minimize(a.x0(), b.x0()),
			maximize(a.x1(), b.x1())
	);
}
//-----------------------------------------------------------------------------
inline Rectangle intersect(const Rectangle &a, const Rectangle &b) {
	Rectangle res;
	boost::geometry::intersection<Rectangle::Base,
		Rectangle::Base, Rectangle::Base> (a, b, res);
	return res;
}
//=============================================================================
inline std::ostream & operator << (std::ostream &os, const Rectangle &r)
{
	os << "Rectangle(" << r.x0() << ", " << r.x1() << ")";
	return os;
}

//=============================================================================
/**
 * @class Insets.
 * An Insets object is a representation of the borders of a container.
 * It specifies the space that a container must leave at each of its edges.
 * The space can be a border, a blank space, or a title.
 */
class Insets : public Rectangle {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::geometry::model::box<Point2D> Base;
private:
public:
	//-------------------------------------------------------------------------
	Insets & left(Coordinate &v) {
		Rectangle::x(v);
        return *this;
	}
	//-------------------------------------------------------------------------
	const Coordinate & left() const {
		return Rectangle::x();
	}
	//-------------------------------------------------------------------------
	Insets & top(Coordinate &v) {
		Rectangle::y(v);
        return *this;
	}
	//-------------------------------------------------------------------------
	const Coordinate & top() const {
		return Rectangle::y();
	}
	//-------------------------------------------------------------------------
	Insets & right(Coordinate &v) {
		Point2D p = Rectangle::x1();
		p.x(v);
		Rectangle::x1(p);
        return *this;
	}
	//-------------------------------------------------------------------------
	const Coordinate & right() const {
		return Rectangle::x1().x();
	}
	//-------------------------------------------------------------------------
	Insets & bottom(Coordinate &v) {
		Point2D p = Rectangle::x1();
		p.y(v);
		Rectangle::x1(p);
        return *this;
	}
	//-------------------------------------------------------------------------
	const Coordinate & bottom() const {
		return Rectangle::x1().y();
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
inline Rectangle getNullRectangle() {
	return Rectangle(NULL_NUMBER, NULL_NUMBER, NULL_NUMBER, NULL_NUMBER);
}
}} // namespaces


#endif /* GEOMETRY_HPP_ */
