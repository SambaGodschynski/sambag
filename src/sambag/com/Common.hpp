/*
 * Common.hpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#ifndef SAMBAG_COMMON_HPP_
#define SAMBAG_COMMON_HPP_

#include <sambag/math/VectorN.hpp>
#include <math.h>
#include <boost/geometry.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/logic/tribool.hpp>

namespace sambag {
namespace com {
//#############################################################################
// metrics
//#############################################################################
typedef double Number;
extern const Number NULL_NUMBER;

//#############################################################################
// geometry
//#############################################################################
//=============================================================================
using namespace boost::geometry;
typedef model::d2::point_xy<Number> Point2D;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
extern const Point2D NULL_POINT2D;
//=============================================================================
//=============================================================================
class Rectangle {
//=============================================================================
public:
	typedef model::box<Point2D> Box;
private:
	Box box;
public:
	//-------------------------------------------------------------------------
	const Point2D & getX0() const {
		return box.min_corner();
	}
	//-------------------------------------------------------------------------
	const Box & getBox() const {
		return box;
	}
	//-------------------------------------------------------------------------
	void setX0(const Point2D & val) {
		Point2D diff = val;
		subtract_point(diff, box.min_corner());
		add_point(box.min_corner(), diff);
		add_point(box.max_corner(), diff);
	}
	//-------------------------------------------------------------------------
	Number getWidth() const {
		Point2D result = box.max_corner();
		subtract_point(result, box.min_corner());
		return result.x();

	}
	//-------------------------------------------------------------------------
	Number getHeight() const {
		Point2D result = box.max_corner();
		subtract_point(result, box.min_corner());
		return result.y();
	}
	//-------------------------------------------------------------------------
	void setWidth(const Number &w) {
		box.max_corner().x(box.min_corner().x() + w);
	}
	//-------------------------------------------------------------------------
	void setHeight(const Number &h) {
		box.max_corner().y(box.min_corner().y() + h);
	}
	//-------------------------------------------------------------------------
	Rectangle(Point2D x0 = Point2D(0, 0), Point2D x1 = Point2D(0, 0)) :
		box(Box(x0, x1)) {
	}
	//-------------------------------------------------------------------------
	Rectangle(Point2D _x0, const Number &width, const Number &height)

	{
		Point2D x0(_x0), x1(width, height);
		add_point(x1, x0);
		Rectangle::box = Box(x0, x1);
	}
	//-------------------------------------------------------------------------
	Rectangle(const Number &x, const Number &y, const Number &width,
			const Number &height) {
		Point2D x0(x, y), x1(width, height);
		add_point(x1, x0);
		Rectangle::box = Box(x0, x1);
	}
};
extern const Rectangle NULL_RECTANGLE;
//#############################################################################
// colors
//#############################################################################
//=============================================================================
// red, green, blue, alpha as 0..1 values
struct ColorRGBA: public sambag::math::VectorN<Number, 4> {
//=============================================================================
	enum {
		R = 1, G, B, A
	};
	Number getR() const {
		return get<R> ();
	}
	Number getG() const {
		return get<G> ();
	}
	Number getB() const {
		return get<B> ();
	}
	Number getA() const {
		return get<A> ();
	}
	void setR(const Number & val) {
		set<R> (val);
	}
	void setG(const Number & val) {
		set<G> (val);
	}
	void setB(const Number & val) {
		set<B> (val);
	}
	void setA(const Number & val) {
		set<A> (val);
	}
	void setValues(const Number &r, const Number &g, const Number &b,
			const Number &a) {
		setR(r);
		setG(g);
		setB(b);
		setA(a);
	}
	ColorRGBA(Number r = 0.0, Number g = 0.0, Number b = 0.0, Number a = 1.0) {
		setValues(r, g, b, a);
	}
	typedef boost::tuple<Number, Number, Number, Number> Tuple;
	Tuple asTuple() const {
		return Tuple(getR(), getG(), getB(), getA());
	}
	static const ColorRGBA NULL_COLOR;
};
//#############################################################################
// Math
//#############################################################################
//=============================================================================
typedef boost::numeric::ublas::matrix<Number> Matrix;
extern const Matrix IDENTITY_MATRIX;
extern const Matrix NULL_MATRIX;
//=============================================================================
using namespace boost::numeric::ublas;
//=============================================================================
// Missing comparator in Matrix.hpp
template<typename T>
bool operator==(const matrix<T>& m, const matrix<T>& n) {
//=============================================================================
	bool returnValue = (m.size1() == n.size1()) && (m.size2() == n.size2());
	if (returnValue) {
		for (unsigned int i = 0; returnValue && i < m.size1(); ++i) {
			for (unsigned int j = 0; returnValue && j < m.size2(); ++j) {
				returnValue &= m(i, j) == n(i, j);
			}
		}
	}
	return returnValue;
}
//=============================================================================
template<typename T>
bool operator!=(const matrix<T>& m, const matrix<T>& n) {
//=============================================================================
	return !(m == n);
}
//#############################################################################
// Misc.
//#############################################################################
//=============================================================================
#define for_each BOOST_FOREACH
#define reverse_for_each BOOST_REVERSE_FOREACH
//=============================================================================
using namespace boost::logic;
typedef tribool TriBool;

}
} // namespaces

#endif /* COMMON_HPP_ */
