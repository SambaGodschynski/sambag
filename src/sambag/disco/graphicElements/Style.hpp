/*
 * Style.h
 *
 *  Created on: 19.09.2011
 *      Author: samba
 */

#ifndef STYLE_H_
#define STYLE_H_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/disco/Geometry.hpp"
#include "sambag/disco/Coordinate.hpp"
#include "sambag/disco/IPattern.hpp"
#include "sambag/disco/IDiscoFactory.hpp"

// hash functions first ...
namespace boost {
extern size_t hash_value(const sambag::disco::ColorRGBA &o);
extern size_t hash_value(const sambag::disco::Dash &o);
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
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>properties
	//-------------------------------------------------------------------------
	// NO_VALUES are for recognize whether a value is setted or not.
	static const Font::FontFace NO_FONT_FACE;
	static const Font NO_FONT;
	//-------------------------------------------------------------------------
	static const Style DEFAULT_STYLE;
	//-------------------------------------------------------------------------
	// special case color none: its not the same like NO_COLOR.
	// NO_COLOR is unsetted. NONE_COLOR is setted but none.
	static const ColorRGBA NONE_COLOR;
private:
	//-------------------------------------------------------------------------
	FLYWEIGHT(Coordinate, _strokeWidth);
	FLYWEIGHT(LineCapStyle, _lineCapStyle);
	FLYWEIGHT(FillRule, _fillRule);
	FLYWEIGHT(IPattern::Ptr, _strokePattern);
	FLYWEIGHT(IPattern::Ptr, _fillPattern);
	FLYWEIGHT(Font::FontFace, _fontFace);
	FLYWEIGHT(Coordinate, _fontSize);
	FLYWEIGHT(Font::Slant, _fontSlant);
	FLYWEIGHT(Font::Weight, _fontWeight);
	FLYWEIGHT(Coordinate, _fillOpacity);
	FLYWEIGHT(Coordinate, _strokeOpacity);
	Dash::Ptr _dash; // TODO: make flyweight
	typedef boost::shared_ptr<Style> Ptr;
	static Ptr NULL_STYLE_SINGLETON;
public:
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>methods
	//-------------------------------------------------------------------------
	static const Style & getNullStyle();
	//-------------------------------------------------------------------------
	void add(const Style &b);
	//-------------------------------------------------------------------------
	Style() :
		_strokeWidth(NULL_NUMBER),
		_lineCapStyle(IDrawContext::NO_LINE_CAP),
		_fillRule(IDrawContext::NO_FILL_RULE),
		_strokePattern(ASolidPattern::Ptr()),
		_fillPattern(ASolidPattern::Ptr()),
		_fontFace(NO_FONT.fontFace),
		_fontSize(NO_FONT.size),
		_fontSlant(NO_FONT.slant),
		_fontWeight(NO_FONT.weight),
		_fillOpacity(NULL_NUMBER),
		_strokeOpacity(NULL_NUMBER),
		_dash(Dash::Ptr())
	{
	}
	//-------------------------------------------------------------------------
	bool operator == (const Style &b) const {
		return _strokeWidth == b._strokeWidth &&
		_lineCapStyle == b._lineCapStyle &&
		_fillRule == b._fillRule &&
		_strokePattern == b._strokePattern &&
		_fillPattern == b._fillPattern &&
		_fontFace == b._fontFace &&
		_fontSize == b._fontSize &&
		_fontSlant == b._fontSlant &&
		_fontWeight == b._fontWeight &&
		_fillOpacity == b._fillOpacity &&
		_strokeOpacity == b._strokeOpacity &&
		_dash == b._dash;
	}
	//-------------------------------------------------------------------------
	bool operator != (const Style &b) const {
		return !((*this)==b);
	}
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
	const Coordinate & strokeWidth() const {
		return _strokeWidth;
	}
	//-------------------------------------------------------------------------
	Style & strokeOpacity(const Coordinate &v) {
		_strokeOpacity = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Coordinate strokeOpacity() const {
		return _strokeOpacity;
	}
	//-------------------------------------------------------------------------
	Style & fillOpacity(const Coordinate &v) {
		_fillOpacity = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Coordinate fillOpacity() const {
		return _fillOpacity;
	}
	//-------------------------------------------------------------------------
	Style & strokeWidth(const Coordinate &v) {
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
	IPattern::Ptr strokePattern() const {
		return _strokePattern;
	}
	//-------------------------------------------------------------------------
	Style & strokePattern(IPattern::Ptr v) {
		_strokePattern = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	IPattern::Ptr fillPattern() const {
		return _fillPattern;
	}
	//-------------------------------------------------------------------------
	Style & fillColor(const ColorRGBA &col) {
		_fillPattern = getDiscoFactory()->createSolidPattern(col);
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & strokeColor(const ColorRGBA &col) {
		_strokePattern = getDiscoFactory()->createSolidPattern(col);
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fillPattern(IPattern::Ptr v) {
		_fillPattern = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Dash::Ptr dash() const {
		return _dash;
	}
	//-------------------------------------------------------------------------
	Style & dash(Dash::Ptr v) {
		_dash = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	void intoContext( IDrawContext::Ptr cn ) const;
	//-------------------------------------------------------------------------
	Style & fontFace( const Font::FontFace & v) {
		_fontFace = v;
		return *this;
	}
	//-------------------------------------------------------------------------
	Style & fontSize( const Coordinate & v) {
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
	//-------------------------------------------------------------------------
	const Font::FontFace & fontFace() const {
		return _fontFace;
	}
	//-------------------------------------------------------------------------
	const Coordinate & fontSize() const {
		return _fontSize;
	}
	//-------------------------------------------------------------------------
	const Font::Slant & fontSlant() const {
		return _fontSlant;
	}
	//-------------------------------------------------------------------------
	const Font::Weight & fontWeight() const {
		return _fontWeight;
	}

};
}}} // namespace

#endif /* STYLE_H_ */
