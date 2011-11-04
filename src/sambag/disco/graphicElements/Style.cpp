/*
 * Style.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "Style.hpp"
#include "sambag/com/Common.hpp"
#include <float.h>

//=============================================================================
// hash functions
//=============================================================================
namespace boost {
//-----------------------------------------------------------------------------
size_t hash_value(const sambag::com::ColorRGBA &o) {
	std::size_t seed = 0;
	boost::hash_combine(seed, o.getR());
	boost::hash_combine(seed, o.getG());
	boost::hash_combine(seed, o.getB());
	boost::hash_combine(seed, o.getA());
	return seed;
}
//-----------------------------------------------------------------------------
size_t hash_value(const sambag::disco::IDrawContext::Dash &o) {
	using namespace sambag::disco;
	std::size_t seed = 0;
	boost::hash_combine(seed, o.get<IDrawContext::DASH_OFFSET>());
	boost::hash_combine(seed, o.get<IDrawContext::DASH_COUNT>());
	return seed;
}
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
	neu.strokeColor(Style::NONE_COLOR);
	neu.fillColor(ColorRGBA(0,0,0,1.0));
	neu.font(Font());
	neu.strokeWidth(1.0);
	return neu;
}
} // namespace
//=============================================================================
// struct Style
//=============================================================================
//-----------------------------------------------------------------------------
// consider def. order while referencing each other
const ColorRGBA Style::NONE_COLOR = ColorRGBA(-1.0, -1.0, -1.0, -1.0);
const Style::Dash Style::NO_DASH = Style::Dash(NULL, 0, DBL_MAX);
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
	if (_strokeColor!=ColorRGBA::NULL_COLOR)
		cn->setStrokeColor(_strokeColor);
	if (_strokeWidth!=NULL_NUMBER)
		cn->setStrokeWidth(_strokeWidth);
	if (_dash!=NO_DASH)
		cn->setDash(_dash);
	if (_fillColor!=ColorRGBA::NULL_COLOR)
		cn->setFillColor(_fillColor);
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
const Style & Style::getNullStyle() {
	if (!NULL_STYLE_SINGLETON) {
		NULL_STYLE_SINGLETON = Ptr(new Style());
	}
	return *(NULL_STYLE_SINGLETON.get());
}


}}} // namespaces
