/*
 * IDrawContext.h
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#ifndef IDRAWCONTEXT_H_
#define IDRAWCONTEXT_H_

#include <boost/shared_ptr.hpp>
#include <math.h>
#include <string>
#include "Geometry.hpp"
#include "sambag/com/Common.hpp"
#include "sambag/math/Matrix.hpp"
#include "Font.hpp"
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include "ISurface.hpp"
#include "sambag/com/FileHandler.hpp"
#include "IPattern.hpp"
#include "ColorRGBA.hpp"
#include "Dash.hpp"

namespace sambag { namespace disco {

using namespace sambag::com;
using namespace sambag::math;

//=============================================================================
// Class Path
// a base class for path. A Path is exchangeable only on his concrete
// implementation. Means: A Path created by CairoContext fits only
// to CairoContext.
//=============================================================================
struct Path { // TODO: change to interface
	typedef boost::shared_ptr<Path> Ptr;
	// TODO: use boost::iterator if possible
	virtual ~Path() {}
};

//=============================================================================
// Interface IDrawContext.
// Declares all draw operations for disco.
// Main purpose of this interface is to have an abstraction layer for the Cairo
// draw context. It's not to understand as a Cairo binding for C++.
//=============================================================================
class IDrawContext {
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IDrawContext> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>drawing
	//-------------------------------------------------------------------------
	virtual void moveTo( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void lineTo( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void relMoveTo( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void relLineTo( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void closePath() = 0;
	//-------------------------------------------------------------------------
	virtual void curveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 ) = 0;
	//-------------------------------------------------------------------------
	virtual void relCurveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 ) = 0;
	//-------------------------------------------------------------------------
	virtual void quadraticCurveTo( const Point2D &x1, const Point2D &x2 ) = 0;
	//-------------------------------------------------------------------------
	virtual void relQuadraticCurveTo( const Point2D &x1, const Point2D &x2 ) = 0;
	//-------------------------------------------------------------------------
	virtual void arc(
		const Point2D &c,
		const Coordinate &r,
		const Coordinate &angle1 = 0,
		const Coordinate &angle2 = M_PI *2.0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void arcNegative(
		const Point2D &c,
		const Coordinate &r,
		const Coordinate &angle1 = 0,
		const Coordinate &angle2 = M_PI *2.0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect ) = 0;
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect, const Number &cornerRadius ) = 0;
	//-------------------------------------------------------------------------
	virtual void fill() = 0;
	//-------------------------------------------------------------------------
	virtual void fillPreserve() = 0;
	//-------------------------------------------------------------------------
	virtual void stroke() = 0;
	//-------------------------------------------------------------------------
	virtual void drawSurface(ISurface::Ptr, Number opacity = 1.0) = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Text
	//-------------------------------------------------------------------------
	virtual void setFont( const Font &font ) = 0;
	//-------------------------------------------------------------------------
	virtual void setFontFace(const Font::FontFace &fontFace) = 0;
	//-------------------------------------------------------------------------
	virtual void setFontWeight(const Font::Weight &weight) = 0;
	//-------------------------------------------------------------------------
	virtual void setFontSlant(const Font::Slant &slant) = 0;
	//-------------------------------------------------------------------------
	virtual void setFontSize(const Font::Size &size) = 0;
	//-------------------------------------------------------------------------
	virtual Font getCurrentFont() const = 0;
	//-------------------------------------------------------------------------
	virtual void textPath( const std::string &text ) = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Styling
	//-------------------------------------------------------------------------
	enum LineCapStyle {
		 NO_LINE_CAP = -1,
		 LINE_CAP_BUTT,
		 LINE_CAP_ROUND,
		 LINE_CAP_SQUARE
	};
	enum LineJoin {
		NO_LINE_JOIN = -1,
		LINE_JOIN_MITER,
		LINE_JOIN_ROUND,
		LINE_JOIN_BEVEL
	};
	enum FillRule {
	   NO_FILL_RULE = -1,
	   FILL_RULE_WINDING,
	   FILL_RULE_EVEN_ODD
	};
	//-------------------------------------------------------------------------
	virtual void setStrokeWidth( const Coordinate &val ) = 0;
	//-------------------------------------------------------------------------
	virtual Coordinate getStrokeWidth() const = 0;
	//-------------------------------------------------------------------------
	virtual void setFillColor( const ColorRGBA &val ) = 0;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getFillColor() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * @return true if strokeColor.alpha > 0
	 */
	virtual bool isStroked() const = 0;
	//-------------------------------------------------------------------------
	/**
	* @return true if fillColor.alpha > 0
	*/
	virtual bool isFilled() const = 0;
	//-------------------------------------------------------------------------
	virtual void setStrokeColor( const ColorRGBA &val ) = 0;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getStrokeColor() const = 0;
	//-------------------------------------------------------------------------
	virtual void setDash( Dash::Ptr dash ) = 0;
	//-------------------------------------------------------------------------
	virtual Dash::Ptr getDash() const = 0;
	//-------------------------------------------------------------------------
	virtual void disableDash() = 0;
	//-------------------------------------------------------------------------
	virtual void setLineCap ( LineCapStyle style ) = 0;
	//-------------------------------------------------------------------------
	virtual LineCapStyle getLineCap () const = 0;
	//-------------------------------------------------------------------------
	virtual void setFillRule( FillRule rule ) = 0;
	//-------------------------------------------------------------------------
	virtual FillRule getFillRule() const = 0;
	//-------------------------------------------------------------------------
	virtual void setFillPattern(IPattern::Ptr pattern) = 0;
	//-------------------------------------------------------------------------
	virtual void setStrokePattern(IPattern::Ptr pattern) = 0;
	//-------------------------------------------------------------------------
	virtual void setLineJoin(LineJoin join) = 0;
	//-------------------------------------------------------------------------
	virtual LineJoin getLineJoin() const = 0;
	//-------------------------------------------------------------------------
	virtual void setMiterLimit(const Number &v) = 0;
	//-------------------------------------------------------------------------
	virtual Number getMiterLimit() const = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Misc.
	//-------------------------------------------------------------------------
	virtual void save() = 0;
	//-------------------------------------------------------------------------
	virtual void restore() = 0;
	//-------------------------------------------------------------------------
	virtual Point2D getCurrentPoint() const = 0;
	//-------------------------------------------------------------------------
	virtual bool hasCurrentPoint() const = 0;
	//-------------------------------------------------------------------------
	virtual void clip() = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle clipExtends() const = 0;
	//-------------------------------------------------------------------------
	virtual void setClip(const Rectangle &r) = 0;
	//-------------------------------------------------------------------------
	virtual void newSubPath() = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle textExtends(const std::string &str) const = 0;
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPath() const = 0;
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPathFlat() const = 0;
	//-------------------------------------------------------------------------
	virtual void appendPath( Path::Ptr path ) = 0;
	//-------------------------------------------------------------------------
	virtual ISurface::Ptr getSurface() const = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle pathExtends() const = 0;
	//-------------------------------------------------------------------------
	virtual void copyTo(IDrawContext::Ptr cn) const = 0;
	//-------------------------------------------------------------------------
	virtual void copyAreaTo(IDrawContext::Ptr cn,
			const Rectangle &src, const Point2D &dst) const = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Transformation
	//-------------------------------------------------------------------------
	virtual void translate( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void scale( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void rotate( const Number &angle ) = 0;
	//-------------------------------------------------------------------------
	virtual void identityMatrix() = 0;
	//-------------------------------------------------------------------------
	virtual void transform ( const Matrix &m ) = 0;
	//-------------------------------------------------------------------------
	virtual void getMatrix( Matrix &m ) = 0;
};
}} // namespaces

#endif /* IDRAWCONTEXT_H_ */
