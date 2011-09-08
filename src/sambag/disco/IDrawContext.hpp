/*
 * IDrawContext.h
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#ifndef IDRAWCONTEXT_H_
#define IDRAWCONTEXT_H_

#include <boost/shared_ptr.hpp>
#include <sambag/com/Common.hpp>
#include <math.h>
#include <string>
#include "Font.hpp"
#include <boost/tuple/tuple.hpp>

namespace sambag { namespace disco {

using namespace sambag::com;

//=============================================================================
// Class Path
// a base class for path. A Path is exchangeable only on his concrete
// implementation. Means: A Path created by CairoContext fits only
// to CairoContext.
//=============================================================================
struct Path {
	typedef boost::shared_ptr<Path> Ptr;
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
	// < array of dashes, number of dashes, offset >
	typedef boost::tuple<Number*, int, Number> Dash;
	//-------------------------------------------------------------------------
	enum {
		 DASH_ARRAY,
		 DASH_COUNT,
		 DASH_OFFSET
	};
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IDrawContext> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>drawing
	//-------------------------------------------------------------------------
	virtual void moveTo( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void lineTo( const Point2D &p0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void curveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 ) = 0;
	//-------------------------------------------------------------------------
	virtual void arc(
		const Point2D &c,
		const Number &r,
		const Number &angle1 = 0,
		const Number &angle2 = M_PI *2.0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect ) = 0;
	//-------------------------------------------------------------------------
	virtual void fill() = 0;
	//-------------------------------------------------------------------------
	virtual void stroke() = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Text
	//-------------------------------------------------------------------------
	virtual void setFont( const Font &font ) = 0;
	//-------------------------------------------------------------------------
	virtual Font getCurrentFont() const = 0;
	//-------------------------------------------------------------------------
	virtual void textPath( const std::string &text ) = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Styling
	//-------------------------------------------------------------------------
	enum LineCapStyle {
		 LINE_CAP_BUTT,
		 LINE_CAP_ROUND,
		 LINE_CAP_SQUARE
	};
	//-------------------------------------------------------------------------
	virtual void setStrokeWidth( const Number &val ) = 0;
	//-------------------------------------------------------------------------
	virtual Number getStrokeWidth() const = 0;
	//-------------------------------------------------------------------------
	virtual void setSourceColor( const ColorRGBA &val ) = 0;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getSourceColor() const = 0;
	//-------------------------------------------------------------------------
	virtual void setDash( const Dash &dash ) = 0;
	//-------------------------------------------------------------------------
	virtual Dash getDash() const = 0;
	//-------------------------------------------------------------------------
	virtual void disableDash() = 0;
	//-------------------------------------------------------------------------
	virtual void setLineCap ( LineCapStyle style ) = 0;
	//-------------------------------------------------------------------------
	virtual LineCapStyle getLineCap () const = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Misc.
	//-------------------------------------------------------------------------
	virtual Point2D getCurrentPoint() const = 0;
	//-------------------------------------------------------------------------
	virtual bool hasCurrentPoint() const = 0;
	//-------------------------------------------------------------------------
	virtual void clip() = 0;
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPath() const = 0;
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPathFlat() const = 0;
	//-------------------------------------------------------------------------
	virtual void appendPath( Path::Ptr path ) = 0;
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
