/*
 * ColorRGBA.hpp
 *
 *  Created on: Nov 21, 2011
 *      Author: samba
 */

#ifndef COLORRGBA_HPP_
#define COLORRGBA_HPP_

#include <sambag/math/VectorN.hpp>
#include "sambag/com/Common.hpp"
#include <algorithm>

namespace sambag { namespace disco {
//=============================================================================
/**
 * @class ColorRGBA
 * red, green, blue, alpha as 0..1 values
 */
struct ColorRGBA: public sambag::math::VectorN<sambag::com::Number, 4> {
//=============================================================================
	enum {
		R = 1, G, B, A
	};
	sambag::com::Number getR() const {
		return get<R> ();
	}
	sambag::com::Number getG() const {
		return get<G> ();
	}
	sambag::com::Number getB() const {
		return get<B> ();
	}
	sambag::com::Number getA() const {
		return get<A> ();
	}
	ColorRGBA & setR(const sambag::com::Number & val) {
		set<R> ( std::max(0., std::min(1., val)) );
		return *this;
	}
	ColorRGBA & setG(const sambag::com::Number & val) {
		set<G> ( std::max(0., std::min(1., val)) );
		return *this;
	}
	ColorRGBA & setB(const sambag::com::Number & val) {
		set<B> ( std::max(0., std::min(1., val)) );
		return *this;
	}
	ColorRGBA & setA(const sambag::com::Number & val) {
		set<A> ( std::max(0., std::min(1., val)) );
		return *this;
	}
	ColorRGBA & setValues(const sambag::com::Number &r,
			const sambag::com::Number &g, const sambag::com::Number &b,
			const sambag::com::Number &a)
	{
		setR(r);
		setG(g);
		setB(b);
		setA(a);
		return *this;
	}
	ColorRGBA(sambag::com::Number r = 0.0, sambag::com::Number g = 0.0,
			sambag::com::Number b = 0.0, sambag::com::Number a = 1.0)
	{
		setValues(r, g, b, a);
	}
	typedef boost::tuple<sambag::com::Number, sambag::com::Number,
			sambag::com::Number, sambag::com::Number> Tuple;
	Tuple asTuple() const {
		return Tuple(getR(), getG(), getB(), getA());
	}
	static const ColorRGBA NULL_COLOR;
};
}} // namespaces

#endif /* COLORRGBA_HPP_ */
