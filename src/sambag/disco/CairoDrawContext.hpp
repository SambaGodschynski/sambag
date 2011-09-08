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
	ColorRGBA currentColor;
	//-------------------------------------------------------------------------
	cairo_surface_t *surface;
	//-------------------------------------------------------------------------
	Font currentFont;
protected:
	//-------------------------------------------------------------------------
	cairo_t *context;
	//-------------------------------------------------------------------------
	CairoDrawContext( cairo_surface_t *surface );
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
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>helper
	//-------------------------------------------------------------------------
	// assumes a 3x3 matrix
	static void discoMatrixToCairoMatrix ( const Matrix &dM, cairo_matrix_t &cM ) {
		if ( dM.size1() != 3 || dM.size2() != 3 ) return;
		cM.xx = dM(0,0); cM.yx = dM(1,0);
		cM.xy = dM(0,1); cM.yy = dM(1,1);
		cM.x0 = dM(0,2); cM.y0 = dM(1,2);
	}
	//-------------------------------------------------------------------------
	// assumes a 3x3 matrix
	static void cairoMatrixToDiscoMatrix ( const cairo_matrix_t &cM, Matrix &dM ) {
		if ( dM.size1() != 3 || dM.size2() != 3 ) return;
		dM(0,0) = cM.xx; dM(0,1) = cM.xy; dM(0,2) = cM.x0;
		dM(1,0) = cM.yx; dM(1,1) = cM.yy; dM(1,2) = cM.y0;
		dM(2,0) = 0;     dM(2,1) = 0    ; dM(2,2) = 0;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>drawing
	//-------------------------------------------------------------------------
	virtual void moveTo( const Point2D &p0 ) {
		cairo_move_to ( context, p0.getX(), p0.getY() );
	}
	//-------------------------------------------------------------------------
	virtual void lineTo( const Point2D &p0 ) {
		cairo_line_to ( context, p0.getX(), p0.getY() );
	}
	//-------------------------------------------------------------------------
	virtual void curveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 )
	{
		cairo_curve_to(
			context,
			x1.getX(),
			x1.getY(),
			x2.getX(),
			x2.getY(),
			x3.getX(),
			x3.getY()
		);
	}
	//-------------------------------------------------------------------------
	virtual void arc(
		const Point2D &c,
		const Number &r,
		const Number &angle1 = 0,
		const Number &angle2 = M_PI *2.0 )
	{
		cairo_arc( context, c.getX(), c.getY(), r, angle1, angle2 );
	}
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect ) {
		cairo_rectangle(
			context,
			rect.getX0().getX(),
			rect.getX0().getY(),
			rect.getWidth(),
			rect.getHeight()
		);
	}
	//-------------------------------------------------------------------------
	virtual void stroke() {
		cairo_stroke(context);
	}
	//-------------------------------------------------------------------------
	virtual void fill() {
		cairo_fill(context);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Text
	//-------------------------------------------------------------------------
	virtual void setFont( const Font &font ) {
		cairo_select_font_face (
			context,
			font.fontFace.c_str(),
			(cairo_font_slant_t)font.slant,
			(cairo_font_weight_t)font.weight
		);
		cairo_set_font_size (context, font.size);
		currentFont = font;
	}
	//-------------------------------------------------------------------------
	virtual Font getCurrentFont() const {
		return currentFont;
	}
	//-------------------------------------------------------------------------
	virtual void textPath( const std::string &text ) {
		cairo_text_path( context, text.c_str() );
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
	virtual void setSourceColor( const ColorRGBA &col ) {
		cairo_set_source_rgba( context, col.getR(), col.getG(), col.getB(), col.getA() );
		currentColor = col;
	}
	//-------------------------------------------------------------------------
	virtual ColorRGBA getSourceColor() const { return currentColor; }
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Misc.
	//-------------------------------------------------------------------------
	virtual Point2D getCurrentPoint() const {
		Number x=0, y=0;
		cairo_get_current_point( context, &x, &y );
		return Point2D(x,y);
	}
	//-------------------------------------------------------------------------
	virtual bool hasCurrentPoint() const {
		return cairo_has_current_point(context);
	}
	//-------------------------------------------------------------------------
	virtual void clip() {
		cairo_clip(context);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Transformation
	//-------------------------------------------------------------------------
	virtual void translate( const Point2D &p0 ) {
		cairo_translate( context, p0.getX(), p0.getY() );
	}
	//-------------------------------------------------------------------------
	virtual void scale( const Point2D &p0 ) {
		cairo_scale( context, p0.getX(), p0.getY() );
	}
	//-------------------------------------------------------------------------
	virtual void rotate( const Number &angle ) {
		cairo_rotate( context, angle );
	}
	//-------------------------------------------------------------------------
	virtual void identityMatrix() {
		cairo_identity_matrix( context );
	}
	//-------------------------------------------------------------------------
	// assumes a 3x3 matrix
	virtual void transform ( const Matrix &m ) {
		if ( m.size1() != 3 || m.size2() != 3 ) return;
		cairo_matrix_t cm;
		discoMatrixToCairoMatrix( m, cm );
		cairo_transform(context, &cm);
	}
};

}} // namespaces

#endif /* CAIRODRAWCONTEXT_H_ */
