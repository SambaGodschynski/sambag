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
	Font() : fontFace("serif"),
			  size(11.),
			  slant(SLANT_NORMAL),
			  weight(WEIGHT_NORMAL)
	{
	}
	virtual ~Font(){}
};

}} //namespaces
#endif /* FONT_HPP_ */
