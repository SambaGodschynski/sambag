/*
 * Style.h
 *
 *  Created on: 19.09.2011
 *      Author: samba
 */

#ifndef STYLE_H_
#define STYLE_H_

#include "sambag/disco/IDrawContext.hpp"

// hash functions first ...
namespace boost {
extern size_t hash_value(const sambag::com::ColorRGBA &o);
extern size_t hash_value(const sambag::disco::IDrawContext::Dash &o);
extern size_t hash_value(const sambag::disco::Font &o);
}
// .. then boost flyweight
#include <boost/flyweight.hpp>

//=============================================================================
// Flyweight Switch
//=============================================================================
//#define DISCO_USE_FLYWEIGHT
#ifdef DISCO_USE_FLYWEIGHT
#define FLYWEIGHT(type, name) boost::flyweight<type> (name)
#else
#define FLYWEIGHT(type, name) type (name)
#endif

namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
/**
 *  A container for styling information.
 */
class Style {
//=============================================================================
public:
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>typedefs
	typedef IDrawContext::LineCapStyle LineCapStyle;
	typedef IDrawContext::FillRule FillRule;
	typedef IDrawContext::Dash Dash;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>properties
	//-------------------------------------------------------------------------
	// NO_VALUES are for recognize whether a value is setted or not. NO_VALUES are only
	// for intern use. For returning a member which is a NO_VALUE return the
	// DEFAULT_VALUE.
	static const Number NO_NUMBER;
	static const LineCapStyle NO_LINE_CAP_STYLE;
	static const FillRule NO_FILL_RULE;
	static const ColorRGBA NO_COLOR;
	static const Dash NO_DASH;
	static const Font NO_FONT;
	//-------------------------------------------------------------------------
	// special case color none: its not the same like NO_COLOR.
	// NO_COLOR is unsetted. NONE_COLOR is setted but none.
	static const ColorRGBA NONE_COLOR;
private:
	//-------------------------------------------------------------------------
	FLYWEIGHT(Number, _strokeWidth);
	FLYWEIGHT(LineCapStyle, _lineCapStyle);
	FLYWEIGHT(FillRule, _fillRule);
	FLYWEIGHT(ColorRGBA, _strokeColor);
	FLYWEIGHT(ColorRGBA, _fillColor);
	FLYWEIGHT(Font, _font);
	Dash _dash; // TODO: make flyweight
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>methods
public:
	//-------------------------------------------------------------------------
	Style() :
		_strokeWidth(NO_NUMBER),
		_lineCapStyle(NO_LINE_CAP_STYLE),
		_fillRule(NO_FILL_RULE),
		_strokeColor( NO_COLOR ),
		_fillColor( NO_COLOR ),
		_font(NO_FONT),
		_dash(NO_DASH)
	{
	}
	//-------------------------------------------------------------------------
	bool operator == (const Style &b) const {
		return _strokeWidth == b._strokeWidth &&
		_lineCapStyle == b._lineCapStyle &&
		_fillRule == b._fillRule &&
		_strokeColor == b._strokeColor &&
		_fillColor == b._fillColor &&
		_font == b._font &&
		_dash == b._dash;
	}
	//-------------------------------------------------------------------------
	bool operator != (const Style &b) const {
		return !((*this)==b);
	}
	//-------------------------------------------------------------------------
	const Number & strokeWidth() const {
		return _strokeWidth;
	}
	//-------------------------------------------------------------------------
	Style & strokeWidth(const Number &v) {
		_strokeWidth = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const LineCapStyle & lineCapStyle() const {
		return _lineCapStyle;
	}
	//-------------------------------------------------------------------------
	Style lineCapStyle(const LineCapStyle &v) {
		_lineCapStyle = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const FillRule & fillRule() const {
		return _fillRule;
	}
	//-------------------------------------------------------------------------
	Style & fillRule(const FillRule &v) {
		_fillRule = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & strokeColor() const {
		return _strokeColor;
	}
	//-------------------------------------------------------------------------
	Style & strokeColor(const ColorRGBA &v) {
		_strokeColor = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & fillColor() const {
		return _fillColor;
	}
	//-------------------------------------------------------------------------
	Style & fillColor(const ColorRGBA &v) {
		_fillColor = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const Dash & dash() const {
		return _dash;
	}
	//-------------------------------------------------------------------------
	Style & dash(const Dash &v) {
		_dash = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	void intoContext( IDrawContext::Ptr cn ) const;
	//-------------------------------------------------------------------------
	Font font() const {
		return _font;
	}
	//-------------------------------------------------------------------------
	Style & font( const Font& f ) {
		_font = f;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontFace( const Font::FontFace & v) {
		Font f = _font.get(); // flyweight: get, change, assign
		f.fontFace = v;
		_font = f;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSize( const Font::Size & v) {
		Font f = _font.get(); // flyweight: get, change, assign
		f.size = v;
		_font = f;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSlant( const Font::Slant & v) {
		Font f = _font.get(); // flyweight: get, change, assign
		f.slant = v;
		_font = f;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontWeight( const Font::Weight & v) {
		Font f = _font.get(); // flyweight: get, change, assign
		f.weight = v;
		_font = f;
		return *this;
	}

};
extern const Style NULL_STYLE;
}}} // namespace

#endif /* STYLE_H_ */
