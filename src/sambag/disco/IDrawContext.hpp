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

namespace sambag { namespace disco {

using namespace sambag::com;

//=============================================================================
// Interface IDrawContext.
// Declares all draw operations for disco.
//=============================================================================
class IDrawContext {
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IDrawContext> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>drawing
	//-------------------------------------------------------------------------
	virtual void drawLine( const Point2D &x, const Point2D &y ) = 0;
	//-------------------------------------------------------------------------
	virtual void drawArc(
			const Point2D &c,
			const Number &r,
			const Number &angle1 = 0,
			const Number &angle2 = M_PI *2.0 ) = 0;
	//-------------------------------------------------------------------------
	virtual void fillArc(
			const Point2D &c,
			const Number &r,
			const Number &angle1 = 0,
			const Number &angle2 = M_PI *2.0 ) = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Styling
	//-------------------------------------------------------------------------
	virtual void setStrokeWidth( const Number &val ) = 0;
	//-------------------------------------------------------------------------
	virtual Number getStrokeWidth() const = 0;
	//-------------------------------------------------------------------------
	virtual void setStrokeColor( const ColorRGBA &val ) = 0;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getStrokeColor( ) const = 0;
	//-------------------------------------------------------------------------
	virtual void setFillColor( const ColorRGBA &val ) = 0;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getFillColor( ) const = 0;
};

}} // namespaces

#endif /* IDRAWCONTEXT_H_ */
