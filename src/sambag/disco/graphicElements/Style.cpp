/*
 * Style.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "Style.hpp"
#include "sambag/com/Common.hpp"
#include <float.h>
#include <boost/static_assert.hpp>

//=============================================================================
// hash functions
//=============================================================================
namespace boost {
//-----------------------------------------------------------------------------
size_t hash_value(const sambag::disco::ColorRGBA &o) {
	std::size_t seed = 0;
	boost::hash_combine(seed, o.getR());
	boost::hash_combine(seed, o.getG());
	boost::hash_combine(seed, o.getB());
	boost::hash_combine(seed, o.getA());
	return seed;
}
//-----------------------------------------------------------------------------
/*size_t hash_value(const Dash &o) {
	using namespace sambag::disco;
	std::size_t seed = 0;
	return seed;
}*/
//-----------------------------------------------------------------------------
size_t hash_value(const sambag::disco::Font &o) {
	using namespace sambag::disco;
	std::size_t seed = 0;
	boost::hash_combine(seed, o.fontFace);
	boost::hash_combine(seed, o.size);
	boost::hash_combine(seed, (int)o.slant);
	boost::hash_combine(seed, (int)o.weight);
	return seed;
}
} // namespace boost

namespace sambag { namespace disco { namespace graphicElements {
namespace {
Style createDefaultStyle() {
	Style neu;
	//neu.strokeColor(Style::NONE_COLOR);
	neu.fillColor(ColorRGBA(0,0,0,1.0));
	neu.font(Font());
	neu.strokeWidth(1.0);
	neu.fillOpacity(1.0);
	neu.strokeOpacity(1.0);
	return neu;
}
} // namespace
//=============================================================================
// struct Style
//=============================================================================
//-----------------------------------------------------------------------------
// consider def. order while referencing each other
const ColorRGBA Style::NONE_COLOR = ColorRGBA(-1.0, -1.0, -1.0, -1.0);
const Font::FontFace Style::NO_FONT_FACE = "undefined-disco-font";
const Font Style::NO_FONT = Font(Style::NO_FONT_FACE, NULL_NUMBER, Font::SLANT_UNDEFINED, Font::WEIGHT_UNDEFINED);
//-----------------------------------------------------------------------------
// define NULL_STYLE after all const objects above,
// because a Style object needs them.
// (otherwise you probably earn a segmentation fault.)
Style::Ptr Style::NULL_STYLE_SINGLETON;
const Style Style::DEFAULT_STYLE = createDefaultStyle();
//-----------------------------------------------------------------------------
void Style::intoContext( IDrawContext::Ptr cn ) const {
	if (_strokePattern) {
		if (_strokeOpacity!=NULL_NUMBER)
			_strokePattern->setOpacity(strokeOpacity());
		cn->setStrokePattern(_strokePattern);
	}
	if (_fillPattern) {
		if (_fillOpacity!=NULL_NUMBER)
			_fillPattern->setOpacity(fillOpacity());
		cn->setFillPattern(_fillPattern);
	}
	if (_strokeWidth!=NULL_NUMBER)
		cn->setStrokeWidth(_strokeWidth);
	if (_dash!=Dash::Ptr())
		cn->setDash(_dash);
	if (_fontFace != NO_FONT.fontFace)
		cn->setFontFace(_fontFace);
	if (_fontSize != NO_FONT.size)
		cn->setFontSize(_fontSize);
	if (_fontSlant != NO_FONT.slant)
		cn->setFontSlant(_fontSlant);
	if (_fontWeight != NO_FONT.weight)
		cn->setFontWeight(_fontWeight);
}
//-----------------------------------------------------------------------------
#define CHECK_NULL_VAL(member,bmember,nullval) (member)==(nullval)&&(bmember)!=(nullval)
void Style::add( const Style &b ) {
	if ( !_strokePattern && b._strokePattern )
		strokePattern(b.strokePattern());
	if ( CHECK_NULL_VAL( _strokeWidth, b._strokeWidth, NULL_NUMBER) )
		strokeWidth(b.strokeWidth());
	if ( !_dash &&  b._dash )
		dash(b.dash());
	if ( !_fillPattern && b._fillPattern )
		fillPattern(b.fillPattern());
	if (CHECK_NULL_VAL(_fontFace, b._fontFace, NO_FONT.fontFace) )
		fontFace(b.fontFace());
	if ( CHECK_NULL_VAL(_fontSize, b._fontSize, NO_FONT.size) )
		fontSize(b.fontSize());
	if ( CHECK_NULL_VAL(_fontSlant, b._fontSlant, NO_FONT.slant) )
		fontSlant(b.fontSlant());
	if ( CHECK_NULL_VAL(_fontWeight, b._fontWeight, NO_FONT.weight) )
		fontWeight(b.fontWeight());
	if ( CHECK_NULL_VAL(_strokeOpacity, b._strokeOpacity, NULL_NUMBER) )
		strokeOpacity(b.strokeOpacity());
	if ( CHECK_NULL_VAL(_fillOpacity, b._fillOpacity, NULL_NUMBER) )
		fillOpacity(b.fillOpacity());
}
//-----------------------------------------------------------------------------
const Style & Style::getNullStyle() {
	if (!NULL_STYLE_SINGLETON) {
		NULL_STYLE_SINGLETON = Ptr(new Style());
	}
	return *(NULL_STYLE_SINGLETON.get());
}


}}} // namespaces
