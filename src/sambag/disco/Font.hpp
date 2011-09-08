/*
 * Font.hpp
 *
 *  Created on: 15.09.2011
 *      Author: samba
 */

#ifndef DISCO_FONT_HPP_
#define DISCO_FONT_HPP_

#include <sambag/com/Common.hpp>
#include <string>

namespace sambag { namespace disco {

using namespace sambag::com;
//=============================================================================
struct Font {
//=============================================================================
	typedef std::string FontFace;
	typedef Number Size;
	enum Slant {
		SLANT_NORMAL,
		SLANT_ITALIC,
		SLANT_OBLIQUE
	};
	enum Weight {
		WEIGHT_NORMAL,
		WEIGHT_BOLD
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
};

}} //namespaces
#endif /* FONT_HPP_ */
