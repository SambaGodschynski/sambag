/*
 * Style.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "Style.hpp"
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
//=============================================================================
// struct Style
//=============================================================================
//-----------------------------------------------------------------------------
const Number Style::NO_NUMBER = NULL_NUMBER;
const Style::LineCapStyle Style::NO_LINE_CAP_STYLE = IDrawContext::NO_LINE_CAP;
const Style::FillRule Style::NO_FILL_RULE = IDrawContext::NO_FILL_RULE;
const ColorRGBA Style::NO_COLOR = com::NULL_COLOR;
const ColorRGBA Style::NONE_COLOR = com::NONE_COLOR;
const Style::Dash Style::NO_DASH = Style::Dash(NULL, 0, DBL_MAX);
const Font Style::NO_FONT = Font("no-xxx-font", -1.0, Font::SLANT_UNDEFINED, Font::WEIGHT_UNDEFINED);
//------------------------------------------------------------------------------
// define NULL_STYLE after all const objects above,
// because a Style object needs them.
// (otherwise you probably earn a segmentation fault.)
const Style NULL_STYLE;
//-------------------------------------------------------------------------
void Style::intoContext( IDrawContext::Ptr cn ) const {
	if (_strokeColor!=NO_COLOR)
		cn->setStrokeColor(_strokeColor);
	if (_strokeWidth!=NO_NUMBER)
		cn->setStrokeWidth(_strokeWidth);
	if (_dash!=NO_DASH)
		cn->setDash(_dash);
	if (_fillColor!=NO_COLOR)
		cn->setFillColor(_fillColor);
	if (_font != NO_FONT)
		cn->setFont(_font);
}



}}} // namespaces
