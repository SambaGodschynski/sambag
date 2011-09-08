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
const ColorRGBA Style::NO_COLOR = NULL_COLOR;
const Style::Dash Style::NO_DASH = Style::Dash(NULL, 0, DBL_MAX);
const Font Style::NO_FONT = Font("no-xxx-font", -1.0);
//-----------------------------------------------------------------------------
const Number Style::DEFAULT_NUMBER = 0.0;
const Number Style::DEFAULT_STROKE_WIDTH = 1.0;
const Style::LineCapStyle Style::DEFAULT_LINE_CAP_STYLE = IDrawContext::LINE_CAP_BUTT;
const Style::FillRule Style::DEFAULT_FILL_RULE = IDrawContext::FILL_RULE_WINDING;
const ColorRGBA Style::DEFAULT_COLOR = ColorRGBA(0,0,0,1);
const Style::Dash Style::DEFAULT_DASH = Style::Dash(NULL, 0, 0);
const boost::logic::tribool Style::DEFAULT_BOOL = false;
const Font Style::DEFAULT_FONT;
//------------------------------------------------------------------------------
void Style::copyFrom( const Style &b ) {
	if (_strokeWidth == NO_NUMBER && b._strokeWidth != NO_NUMBER)
		_strokeWidth = b._strokeWidth;
	if (_lineCapStyle == NO_LINE_CAP_STYLE && b._lineCapStyle != NO_LINE_CAP_STYLE)
		_lineCapStyle = b._lineCapStyle;
	if (_fillRule==NO_FILL_RULE && b._fillRule!=NO_FILL_RULE)
		_fillRule = b._fillRule;
	if (_strokeColor==NO_COLOR && b._strokeColor!=NO_COLOR)
		_strokeColor = b._strokeColor;
	if (_fillColor==NO_COLOR && b._fillColor!=NO_COLOR)
		_fillColor = b._fillColor;
	if (_dash==NO_DASH && b._dash!=NO_DASH)
		_dash = b._dash;
	if (_font==NO_FONT && b._font!=NO_FONT)
			_font = b._font;
}



}}} // namespaces
