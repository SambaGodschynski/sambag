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
#include <boost/geometry.hpp>

namespace sambag { namespace com {
//#############################################################################
// metrics
//#############################################################################
typedef double Number;

//#############################################################################
// geometry
//#############################################################################
//=============================================================================
using namespace boost::geometry;
typedef model::d2::point_xy<Number> Point2D;
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
	const Point2D & getX0() const { return box.min_corner(); }
	//-------------------------------------------------------------------------
	const Point2D & getX1() const { return box.max_corner(); }
	//-------------------------------------------------------------------------
	const Box & getBox() const { return box; }
	//-------------------------------------------------------------------------
	void setX0 ( const Point2D & val ) {
		box.min_corner().x( val.x() );
		box.min_corner().y( val.y() );
	}
	//-------------------------------------------------------------------------
	void setX1 ( const Point2D & val ) {
		box.max_corner().x( val.x() );
		box.max_corner().y( val.y() );
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
	Rectangle( Point2D x0 = Point2D() , Point2D x1 = Point2D() ) :
		box( Box(x0, x1) ){}
	//-------------------------------------------------------------------------
	Rectangle( Point2D _x0 = Point2D() ,
			   const Number &width = 0,
			   const Number &height = 0 )

	{
		Point2D x0(_x0), x1(width, height);
		add_point(x1, x0);
		Rectangle::box = Box(x0, x1);
	}
	//-------------------------------------------------------------------------
	Rectangle( const Number &x = 0,
			   const Number &y = 0,
			   const Number &width = 0,
			   const Number &height = 0 )
	{
		Point2D x0(x,y), x1(width, height);
		add_point(x1, x0);
		Rectangle::box = Box(x0, x1);
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
