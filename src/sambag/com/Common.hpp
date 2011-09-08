/*
 * Common.hpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#ifndef SAMBAG_COMMON_HPP_
#define SAMBAG_COMMON_HPP_

#include <sambag/math/VectorN.hpp>

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

}} // namespaces

#endif /* COMMON_HPP_ */
