/*
 * CairoDrawContext.h
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#ifndef CAIRODRAWCONTEXT_H_
#define CAIRODRAWCONTEXT_H_

#include "IDrawContext.hpp"
#include "cairo/cairo.h"
#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco {
//=============================================================================
//  Class CairoDrawContext:
//    implements IDrawContext with Cairos draw operations.
class CairoDrawContext: public sambag::disco::IDrawContext {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoDrawContext> Ptr;
private:
	//-------------------------------------------------------------------------
	ColorRGBA fillColor;
	//-------------------------------------------------------------------------
	ColorRGBA strokeColor;
	//-------------------------------------------------------------------------
	cairo_surface_t *surface;
protected:
	//-------------------------------------------------------------------------
	cairo_t *context;
	//-------------------------------------------------------------------------
	CairoDrawContext( cairo_surface_t *surface );
	//-------------------------------------------------------------------------
	void _setColor( const ColorRGBA &col ) {
		cairo_set_source_rgba( context, col.getR(), col.getG(), col.getB(), col.getA() );
	}
public:
	//-------------------------------------------------------------------------
	static Ptr create( cairo_surface_t *surface ) {
		Ptr neu( new CairoDrawContext(surface) );
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~CairoDrawContext();
	//-------------------------------------------------------------------------
	cairo_t * getCairoContext() const {
		return context;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>drawing
	//-------------------------------------------------------------------------
	virtual void drawLine( const Point2D &p0, const Point2D &p1 ) {
		cairo_move_to ( context, p0.getX(), p0.getY() );
		cairo_line_to ( context, p1.getX(), p1.getY() );
		_setColor( strokeColor );
		cairo_stroke(context);
	}
	//-------------------------------------------------------------------------
	virtual void drawArc(
			const Point2D &c,
			const Number &r,
			const Number &angle1 = 0,
			const Number &angle2 = M_PI *2.0 )
	{
		cairo_arc( context, c.getX(), c.getY(), r, angle1, angle2 );
		_setColor( strokeColor );
		cairo_stroke(context);
	}
	//-------------------------------------------------------------------------
	virtual void fillArc(
			const Point2D &c,
			const Number &r,
			const Number &angle1 = 0,
			const Number &angle2 = M_PI *2.0 )
	{
		cairo_arc( context, c.getX(), c.getY(), r, angle1, angle2 );
		_setColor( fillColor );
		cairo_fill(context);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Styling
	//-------------------------------------------------------------------------
	virtual void setStrokeWidth( const Number &val ) {
		cairo_set_line_width(context, val);
	}
	//-------------------------------------------------------------------------
	virtual Number getStrokeWidth() const {
		return (Number)cairo_get_line_width(context);
	}
	//-------------------------------------------------------------------------
	virtual void setStrokeColor( const ColorRGBA &val ) { strokeColor = val; }
	//-------------------------------------------------------------------------
	virtual ColorRGBA getStrokeColor() const { return strokeColor; }
	//-------------------------------------------------------------------------
	virtual void setFillColor( const ColorRGBA &val ) { fillColor = val; }
	//-------------------------------------------------------------------------
	virtual ColorRGBA getFillColor() const { return fillColor; }
};

}} // namespaces

#endif /* CAIRODRAWCONTEXT_H_ */
