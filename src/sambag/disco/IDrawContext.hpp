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

namespace sambag { namespace disco {

using namespace sambag::com;

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
	virtual void setStrokeWidth( const Number &val ) = 0;
	//-------------------------------------------------------------------------
	virtual Number getStrokeWidth() const = 0;
	//-------------------------------------------------------------------------
	virtual void setSourceColor( const ColorRGBA &val ) = 0;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getSourceColor() const = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Misc.
	//-------------------------------------------------------------------------
	virtual Point2D getCurrentPoint() const = 0;
	//-------------------------------------------------------------------------
	virtual bool hasCurrentPoint() const = 0;
	//-------------------------------------------------------------------------
	virtual void clip() = 0;
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

};

}} // namespaces

#endif /* IDRAWCONTEXT_H_ */
