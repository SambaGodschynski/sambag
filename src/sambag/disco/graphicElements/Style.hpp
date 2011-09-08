/*
 * Style.h
 *
 *  Created on: 19.09.2011
 *      Author: samba
 */

#ifndef STYLE_H_
#define STYLE_H_

#include "sambag/disco/IDrawContext.hpp"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Fly weight switch
#define USE_DISCO_STYLE_FLY_WEIGHT
#ifndef USE_DISCO_STYLE_FLY_WEIGHT
#define FLYWEIGHT(type, name) boost::flyweight<type> name
#else
#define FLYWEIGHT(type, name) type name
#endif
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// hash functions first ...
namespace boost {
extern size_t hash_value(const sambag::com::ColorRGBA &o);
extern size_t hash_value(const sambag::disco::IDrawContext::Dash &o);
extern size_t hash_value(const sambag::disco::Font &o);
}
// .. then boost flyweight
#include <boost/flyweight.hpp>

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// A container for styling information.
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
	//-------------------------------------------------------------------------
	static const Number DEFAULT_NUMBER;
	static const Number DEFAULT_STROKE_WIDTH;
	static const LineCapStyle DEFAULT_LINE_CAP_STYLE;
	static const FillRule DEFAULT_FILL_RULE;
	static const ColorRGBA DEFAULT_COLOR;
	static const Dash DEFAULT_DASH;
	static const boost::logic::tribool DEFAULT_BOOL;
	static const Font DEFAULT_FONT;
private:
	//-------------------------------------------------------------------------
	FLYWEIGHT(Number,_strokeWidth);
	FLYWEIGHT(LineCapStyle,_lineCapStyle);
	FLYWEIGHT(FillRule,_fillRule);
	FLYWEIGHT(ColorRGBA,_strokeColor);
	FLYWEIGHT(ColorRGBA,_fillColor);
	FLYWEIGHT(Font,_font);
	Dash _dash; // TODO: make flyweight too
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
	/**
	 * copy value from b to self. Copies attribute only if not setted to keep
	 * a cascading effect.
	 * @param b
	 */
	void copyFrom (const Style&b);
	//-------------------------------------------------------------------------
	const Number & strokeWidth() const {
		if (_strokeWidth==NO_NUMBER) return DEFAULT_STROKE_WIDTH;
		return _strokeWidth;
	}
	//-------------------------------------------------------------------------
	Style & strokeWidth(const Number &v) {
		_strokeWidth = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const LineCapStyle & lineCapStyle() const {
		if(_lineCapStyle==NO_LINE_CAP_STYLE) return DEFAULT_LINE_CAP_STYLE;
		return _lineCapStyle;
	}
	//-------------------------------------------------------------------------
	Style lineCapStyle(const LineCapStyle &v) {
		_lineCapStyle = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const FillRule & fillRule() const {
		if(_fillRule==NO_FILL_RULE) return DEFAULT_FILL_RULE;
		return _fillRule;
	}
	//-------------------------------------------------------------------------
	Style & fillRule(const FillRule &v) {
		_fillRule = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & strokeColor() const {
		if (_strokeColor == NO_COLOR || _strokeColor == NONE_COLOR)
			return DEFAULT_COLOR;
		return _strokeColor;
	}
	//-------------------------------------------------------------------------
	Style & strokeColor(const ColorRGBA &v) {
		_strokeColor = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & fillColor() const {
		if(_fillColor==NO_COLOR) return DEFAULT_COLOR;
		return _fillColor;
	}
	//-------------------------------------------------------------------------
	Style & fillColor(const ColorRGBA &v) {
		_fillColor = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const Dash & dash() const {
		if(_dash==NO_DASH) return DEFAULT_DASH;
		return _dash;
	}
	//-------------------------------------------------------------------------
	Style & dash(const Dash &v) {
		_dash = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	const bool isFilled() const {
		if ( _fillColor == NONE_COLOR )
			return false;
		return true;
	}
	//-------------------------------------------------------------------------
	const bool isStroked() const {
		if (_strokeColor == NO_COLOR ||  _strokeColor == NONE_COLOR )
			return false;
		return true;
	}
	//-------------------------------------------------------------------------
	void setStrokeStyle( IDrawContext::Ptr cn ) {
		cn->setStrokeWidth(strokeWidth());
		cn->setLineCap(lineCapStyle());
		if (_dash!=NO_DASH)
			cn->setDash(_dash);
		cn->setSourceColor(strokeColor());
	}
	//-------------------------------------------------------------------------
	void setFillStyle( IDrawContext::Ptr cn ) {
		cn->setFillRule(_fillRule);
		cn->setSourceColor(fillColor());
	}
	//-------------------------------------------------------------------------
	Font font() const {
		// keep cascading effect: check every font value
		if (_font==NO_FONT) return DEFAULT_FONT;
		Font res = _font;
		if (_font.fontFace == NO_FONT.fontFace)
			res.fontFace = DEFAULT_FONT.fontFace;
		if (_font.size == NO_FONT.size)
			res.size = DEFAULT_FONT.size;
		if (_font.slant == NO_FONT.slant)
			res.slant = DEFAULT_FONT.slant;
		if (_font.weight== NO_FONT.weight)
			res.weight = DEFAULT_FONT.weight;
		return res;
	}
	//-------------------------------------------------------------------------
	Style & font_( const Font& f ) {
		_font = f;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontFace( const Font::FontFace & v) {
		_font.fontFace = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSize( const Font::Size & v) {
		_font.size = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSlant( const Font::Slant & v) {
		_font.slant = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontWeight( const Font::Weight & v) {
		_font.weight = v;
		return *this;
	}

};
}}} // namespace

#endif /* STYLE_H_ */
