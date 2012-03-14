/*
 * Font.hpp
 *
 *  Created on: 15.09.2011
 *      Author: samba
 */

#ifndef DISCO_FONT_HPP_
#define DISCO_FONT_HPP_

#include <sambag/com/Common.hpp>
#include "sambag/disco/Geometry.hpp"
#include <string>

namespace sambag { namespace disco {

using namespace sambag::com;
//=============================================================================
struct Font {
//=============================================================================
	typedef std::string FontFace;
	typedef Number Size;
	enum Slant {
		SLANT_UNDEFINED = -1,
		SLANT_NORMAL,
		SLANT_ITALIC,
		SLANT_OBLIQUE,
		NUM_SLANT
	};
	enum Weight {
		WEIGHT_UNDEFINED = -1,
		WEIGHT_NORMAL,
		WEIGHT_BOLD,
		NUM_WEIGHT
	};
	FontFace fontFace;
	Size size;
	Slant slant;
	Weight weight;
	Font(const std::string& fontFace="arial",
		 Size size=11.,
		 Slant slant=SLANT_NORMAL,
		 Weight weight=WEIGHT_NORMAL
	) : fontFace(fontFace),
			  size(size),
			  slant(slant),
			  weight(weight)
	{
	}
	virtual ~Font(){}
	bool operator==( const Font &b ) const {
		return fontFace==b.fontFace && size == b.size && slant == b.slant &&
			   weight == b.weight;
	}
	bool operator!=( const Font &b ) const {
		return !( *this == b );
	}
	/**
	 * dummy function:
	 * needed by @see graphicElements::Style where you can switch
	 * between flyweight or not. If not we need a get().
	 * @return
	 */
	const Font & get() const {
		return *this;
	}
};

}} //namespaces
#endif /* FONT_HPP_ */
