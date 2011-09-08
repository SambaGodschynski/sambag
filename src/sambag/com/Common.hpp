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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace sambag { namespace com {
//#############################################################################
// metrics
//#############################################################################
typedef double Number;

//#############################################################################
// geometry
//#############################################################################
//=============================================================================
struct Point2D : public sambag::math::VectorN<Number, 2> {
//=============================================================================
	enum { X = 1, Y };
	Number getX() const { return get<X>(); }
	Number getY() const { return get<Y>(); }
	void setX ( const Number & val ) { set<X>(val); }
	void setY ( const Number & val ) { set<Y>(val); }
	void setValues( const Number &x, const Number &y ) { setX(x); setY(y); }
	Point2D( Number x = 0.0, Number y = 0.0 ) {
		setValues(x,y);
	}
};
//=============================================================================
class Rectangle {
//=============================================================================
private:
	Point2D x0, x1;
public:
	const Point2D & getX0() const { return x0; }
	const Point2D & getX1() const { return x1; }
	void setX0 ( const Point2D & val ) { x0 = val; }
	void setX1 ( const Point2D & val ) { x1 = val; }
	Number getWidth() const {
		return ( x1 - x0 ).get<Point2D::X>(); //TODO
	}
	Number getHeight() const {
		return ( x1 - x0 ).get<Point2D::Y>(); //TODO
	}
	Rectangle( Point2D x0 = Point2D() , Point2D x1 = Point2D() ) :
		x0(x0), x1(x1) {}
	Rectangle( Point2D x0 = Point2D() ,
			   const Number &width = 0,
			   const Number &height = 0 ) : x0(x0)
	{
		x1 = Point2D(width, height);
		x1 += x0;
	}
	Rectangle( const Number &x = 0,
			   const Number &y = 0,
			   const Number &width = 0,
			   const Number &height = 0 ) : x0(x,y)
	{
		x1 = Point2D(width, height);
		x1 += x0;
	}
};
//#############################################################################
// colors
//#############################################################################
//=============================================================================
// red, green, blue, alpha as 0..1 values
struct ColorRGBA : public sambag::math::VectorN<Number, 4> {
//=============================================================================
	enum { R = 1, G, B, A };
	Number getR() const { return get<R>(); }
	Number getG() const { return get<G>(); }
	Number getB() const { return get<B>(); }
	Number getA() const { return get<A>(); }
	void setR ( const Number & val ) { set<R>(val); }
	void setG ( const Number & val ) { set<G>(val); }
	void setB ( const Number & val ) { set<B>(val); }
	void setA ( const Number & val ) { set<A>(val); }
	void setValues( const Number &r, const Number &g, const Number &b, const Number &a ) {
		setR(r); setG(g); setB(b); setA(a);
	}
	ColorRGBA( Number r = 0.0, Number g = 0.0, Number b = 0.0, Number a = 1.0 ) {
		setValues(r,g,b,a);
	}
};
//#############################################################################
// Math
//#############################################################################
//=============================================================================
typedef boost::numeric::ublas::matrix<Number> Matrix;
//=============================================================================
using namespace boost::numeric::ublas;
//=============================================================================
// Missing comparator in Matrix.hpp
template<typename T>
bool operator==(const matrix<T>& m, const matrix<T>& n) {
//=============================================================================
  bool returnValue =
    (m.size1() == n.size1()) &&
    (m.size2() == n.size2());
  if (returnValue) {
    for (unsigned int i = 0; returnValue && i < m.size1(); ++i) {
      for (unsigned int j = 0; returnValue && j < m.size2(); ++j) {
        returnValue &= m(i,j) == n(i,j);
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


}} // namespaces

#endif /* COMMON_HPP_ */
