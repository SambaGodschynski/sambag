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
	boost::flyweight<Number> _strokeWidth;
	boost::flyweight<LineCapStyle> _lineCapStyle;
	boost::flyweight<FillRule> _fillRule;
	boost::flyweight<ColorRGBA> _strokeColor;
	boost::flyweight<ColorRGBA> _fillColor;
	boost::flyweight<Font> _font;
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
	 * copy value from b to self, only if b.value is NOT undefined(NO_VALUE)
	 * and self.value IS undefined.
	 * @param b
	 */
	void copyFrom (const Style&b);
	//-------------------------------------------------------------------------
	const Number & strokeWidth() const {
		if (_strokeWidth==NO_NUMBER) return DEFAULT_STROKE_WIDTH;
		return _strokeWidth;
	}
	//-------------------------------------------------------------------------
	void strokeWidth(const Number &v) {
		_strokeWidth = v;
	}
	//-------------------------------------------------------------------------
	const LineCapStyle & lineCapStyle() const {
		if(_lineCapStyle==NO_LINE_CAP_STYLE) return DEFAULT_LINE_CAP_STYLE;
		return _lineCapStyle;
	}
	//-------------------------------------------------------------------------
	void lineCapStyle(const LineCapStyle &v) {
		_lineCapStyle = v;
	}
	//-------------------------------------------------------------------------
	const FillRule & fillRule() const {
		if(_fillRule==NO_FILL_RULE) return DEFAULT_FILL_RULE;
		return _fillRule;
	}
	//-------------------------------------------------------------------------
	void fillRule(const FillRule &v) {
		_fillRule = v;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & strokeColor() const {
		if(_strokeColor==NO_COLOR) return DEFAULT_COLOR;
		return _strokeColor;
	}
	//-------------------------------------------------------------------------
	void strokeColor(const ColorRGBA &v) {
		_strokeColor = v;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & fillColor() const {
		if(_fillColor==NO_COLOR) return DEFAULT_COLOR;
		return _fillColor;
	}
	//-------------------------------------------------------------------------
	void fillColor(const ColorRGBA &v) {
		_fillColor = v;
	}
	//-------------------------------------------------------------------------
	const Dash & dash() const {
		if(_dash==NO_DASH) return DEFAULT_DASH;
		return _dash;
	}
	//-------------------------------------------------------------------------
	void dash(const Dash &v) {
		_dash = v;
	}
	//-------------------------------------------------------------------------
	const bool isFilled() const {
		if (_fillColor == NO_COLOR ) return false;
		return true;
	}
	//-------------------------------------------------------------------------
	const bool isStroked() const {
		if (_strokeColor == NO_COLOR ) return false;
		return true;;
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
		cn->setSourceColor(_fillColor);
	}
	//-------------------------------------------------------------------------
	const Font & font() const {
		if (_font==NO_FONT) return DEFAULT_FONT;
		return _font;
	}
	//-------------------------------------------------------------------------
	void font( const Font& f ) {
		_font = f;
	}

};
}}} // namespace

#endif /* STYLE_H_ */
