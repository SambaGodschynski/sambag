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
	// NO_VALUES are for recognize whether a value is setted or not.
	static const Dash NO_DASH;
	static const Font::FontFace NO_FONT_FACE;
	static const Font NO_FONT;
	//-------------------------------------------------------------------------
	static const Style DEFAULT_STYLE;
	static const Style NULL_STYLE;
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
	FLYWEIGHT(Font::FontFace, _fontFace);
	FLYWEIGHT(Font::Size, _fontSize);
	FLYWEIGHT(Font::Slant, _fontSlant);
	FLYWEIGHT(Font::Weight, _fontWeight);
	Dash _dash; // TODO: make flyweight
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>methods
public:
	//-------------------------------------------------------------------------
	Style() :
		_strokeWidth(NULL_NUMBER),
		_lineCapStyle(IDrawContext::NO_LINE_CAP),
		_fillRule(IDrawContext::NO_FILL_RULE),
		_strokeColor(ColorRGBA::NULL_COLOR),
		_fillColor(ColorRGBA::NULL_COLOR),
		_fontFace(NO_FONT.fontFace),
		_fontSize(NO_FONT.size),
		_fontSlant(NO_FONT.slant),
		_fontWeight(NO_FONT.weight),
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
		_fontFace == b._fontFace &&
		_fontSize == b._fontSize &&
		_fontSlant == b._fontSlant &&
		_fontWeight == b._fontWeight &&
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
		return Font(_fontFace, _fontSize, _fontSlant, _fontWeight);
	}
	//-------------------------------------------------------------------------
	Style & font( const Font& f ) {
		fontFace(f.fontFace);
		fontSize(f.size);
		fontSlant(f.slant);
		fontWeight(f.weight);
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontFace( const Font::FontFace & v) {
		_fontFace = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSize( const Font::Size & v) {
		_fontSize = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSlant( const Font::Slant & v) {
		_fontSlant = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontWeight( const Font::Weight & v) {
		_fontWeight = v;
		return *this;
	}

};
}}} // namespace

#endif /* STYLE_H_ */
