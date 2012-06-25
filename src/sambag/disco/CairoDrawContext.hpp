/*
 * CairoDrawContext.h
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#ifndef CAIRODRAWCONTEXT_H_
#define CAIRODRAWCONTEXT_H_

#include "IDrawContext.hpp"
#include "cairo.h"
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "CairoSurface.hpp"
#include "CairoHelper.hpp"
#include "CairoPattern.hpp"
#include <list>
#include <boost/scoped_array.hpp>

namespace sambag { namespace disco {
//=============================================================================
// Class CairoPath
// a wrapper for cairo_path_t
//=============================================================================
class CairoPath : public Path {
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoPath> Ptr;
private:
	//-------------------------------------------------------------------------
	cairo_path_t * cpath;
protected:
	//-------------------------------------------------------------------------
	CairoPath( cairo_path_t * cpath ) : cpath(cpath) {}
public:
	//-------------------------------------------------------------------------
	static Ptr create( cairo_path_t *cpath ) {
		Ptr neu(new CairoPath(cpath));
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~CairoPath() {
		cairo_path_destroy(cpath);
	}
	//-------------------------------------------------------------------------
	cairo_path_t * getPath() const { return cpath; }
};
//-----------------------------------------------------------------------------
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
	enum PatternInUse { INVALID, STROKE, FILL };
	//-------------------------------------------------------------------------
	PatternInUse patternInUse;
	//-------------------------------------------------------------------------
	void _save();
	//-------------------------------------------------------------------------
	void _restore();
	//-------------------------------------------------------------------------
	typedef boost::tuple<
		CairoPatternBase::Ptr,
		CairoPatternBase::Ptr,
		Font,
		Dash::Ptr
	> StateInfo;
	//-------------------------------------------------------------------------
	typedef std::list<StateInfo> StateStack;
	//-------------------------------------------------------------------------
	StateStack stack;
	//-------------------------------------------------------------------------
	CairoSurface::Ptr surfaceRef;
	//-------------------------------------------------------------------------
	cairo_surface_t *surface;
	//-------------------------------------------------------------------------
	Font currentFont;
	//-------------------------------------------------------------------------
	CairoPatternBase::Ptr fillPattern;
	//-------------------------------------------------------------------------
	CairoPatternBase::Ptr strokePattern;
	//-------------------------------------------------------------------------
	Dash::Ptr currentDash; // exists only to hold ptr

protected:
	//-------------------------------------------------------------------------
	cairo_t *context;
	//-------------------------------------------------------------------------
	CairoDrawContext( CairoSurface::Ptr surface );
	//-------------------------------------------------------------------------
	/**
	 * sets the current fill pattern to cairo context
	 */
	void setFillPattern();
	//-------------------------------------------------------------------------
	/**
	 * sets the current stroke pattern to cairo context
	 */
	void setStrokePattern();
public:
	//-------------------------------------------------------------------------
	static Ptr create( CairoSurface::Ptr surface ) {
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
	virtual void moveTo( const Point2D &p0 ) {
		cairo_move_to ( context, p0.x(), p0.y() );
	}
	//-------------------------------------------------------------------------
	virtual void lineTo( const Point2D &p0 ) {
		cairo_line_to ( context, p0.x(), p0.y() );
	}
	//-------------------------------------------------------------------------
	virtual void relMoveTo( const Point2D &p0 ) {
		if (!cairo_has_current_point(context)) {
			cairo_move_to ( context, p0.x(), p0.y() );
			return;
		}
		cairo_rel_move_to ( context, p0.x(), p0.y() );
	}
	//-------------------------------------------------------------------------
	virtual void relLineTo( const Point2D &p0 ) {
		if (!cairo_has_current_point(context)) {
			cairo_line_to ( context, p0.x(), p0.y() );
			return;
		}
		cairo_rel_line_to ( context, p0.x(), p0.y() );
	}
	//-------------------------------------------------------------------------
	virtual void closePath() {
		cairo_close_path(context);
	}
	//-------------------------------------------------------------------------
	virtual void curveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 )
	{
		cairo_curve_to(
			context, x1.x(), x1.y(), x2.x(), x2.y(), x3.x(), x3.y()
		);
	}
	//-------------------------------------------------------------------------
	virtual void relCurveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 )
	{
		if (!cairo_has_current_point(context)) {
			moveTo(Point2D(0,0));
		}
		cairo_rel_curve_to(
			context, x1.x(), x1.y(), x2.x(), x2.y(), x3.x(), x3.y()
		);
	}
	//-------------------------------------------------------------------------
	virtual void quadraticCurveTo( const Point2D &x1, const Point2D &x2 ) {
		Point2D x0 = getCurrentPoint();
		cairo_curve_to (context,
		                2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x0.x(),
		                2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x0.y(),
		                2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x2.x(),
		                2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x2.y(),
		                x2.x(), x2.y());
	}
	//-------------------------------------------------------------------------
	virtual void relQuadraticCurveTo( const Point2D &x1, const Point2D &x2 ) {
		if (!cairo_has_current_point(context)) {
			moveTo(Point2D(0,0));
		}
		Point2D x0 = Point2D(0,0);
		cairo_rel_curve_to (context,
		                    2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x0.x(),
		                    2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x0.y(),
		                    2.0 / 3.0 * x1.x() + 1.0 / 3.0 * x2.x(),
		                    2.0 / 3.0 * x1.y() + 1.0 / 3.0 * x2.y(),
		                    x2.x(), x2.y());
	}
	//-------------------------------------------------------------------------
	virtual void arc(
		const Point2D &c,
		const Coordinate &r,
		const Coordinate &angle1 = 0,
		const Coordinate &angle2 = M_PI *2.0 )
	{
		cairo_arc( context, c.x(), c.y(), r, angle1, angle2 );
	}
	//-------------------------------------------------------------------------
	virtual void arcNegative(
		const Point2D &c,
		const Coordinate &r,
		const Coordinate &angle1 = 0,
		const Coordinate &angle2 = M_PI *2.0 )
	{
		cairo_arc_negative( context, c.x(), c.y(), r, angle1, angle2 );
	}
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect ) {
		cairo_rectangle(
			context,
			rect.x0().x(),
			rect.x0().y(),
			rect.getWidth(),
			rect.getHeight()
		);
	}
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect, const Number &cornerRadius );
	//-------------------------------------------------------------------------
	virtual void stroke() {
		if (!isStroked())
			return;
		if (patternInUse != STROKE) {
			setStrokePattern();
			patternInUse = STROKE;
		}
		cairo_stroke(context);
	}
	//-------------------------------------------------------------------------
	virtual void fill() {
		if (!isFilled())
			return;
		if (patternInUse != FILL) {
			setFillPattern();
			patternInUse = FILL;
		}
		cairo_fill(context);
	}
	//-------------------------------------------------------------------------
	virtual void fillPreserve() {
		if (!isFilled())
			return;
		if (!isFilled())
			return;
		if (patternInUse != FILL) {
			setFillPattern();
			patternInUse = FILL;
		}
		cairo_fill_preserve(context);
	}
	//-------------------------------------------------------------------------
	/**
	 * draws an surface.
	 * 	 * @param a CairoSurface all others will be ignored.
	 */
	virtual void drawSurface(ISurface::Ptr surface, Number opacity = 1.0);
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
	virtual void setFontFace(const Font::FontFace &fontFace) {
		currentFont.fontFace = fontFace;
		cairo_select_font_face (
			context,
			currentFont.fontFace.c_str(),
			(cairo_font_slant_t)currentFont.slant,
			(cairo_font_weight_t)currentFont.weight
		);
	}
	//-------------------------------------------------------------------------
	virtual void setFontWeight(const Font::Weight &weight) {
		currentFont.weight = weight;
		cairo_select_font_face (
			context,
			currentFont.fontFace.c_str(),
			(cairo_font_slant_t)currentFont.slant,
			(cairo_font_weight_t)currentFont.weight
		);
	}
	//-------------------------------------------------------------------------
	virtual void setFontSlant(const Font::Slant &slant) {
		currentFont.slant = slant;
		cairo_select_font_face (
			context,
			currentFont.fontFace.c_str(),
			(cairo_font_slant_t)currentFont.slant,
			(cairo_font_weight_t)currentFont.weight
		);
	}
	//-------------------------------------------------------------------------
	virtual void setFontSize(const Font::Size &size) {
		currentFont.size = size;
		cairo_set_font_size (context, currentFont.size);
	}
	//-------------------------------------------------------------------------
	virtual Font getCurrentFont() const {
		return currentFont;
	}
	//-------------------------------------------------------------------------
	virtual void textPath( const std::string &text ) {
		cairo_text_path( context, text.c_str() );
	}
	//-------------------------------------------------------------------------
	virtual void setFillPattern(IPattern::Ptr pattern);
	//-------------------------------------------------------------------------
	virtual void setStrokePattern(IPattern::Ptr pattern);
	//-------------------------------------------------------------------------
	virtual Rectangle textExtends(const std::string &str) const {
		cairo_text_extents_t ex;
		cairo_text_extents(context, str.c_str(), &ex);
		Point2D p = getCurrentPoint();
		boost::geometry::add_point(p, Point2D(ex.x_bearing, ex.y_bearing));
		return Rectangle(p, ex.width, ex.height);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Styling
	//-------------------------------------------------------------------------
	virtual bool isStroked() const {
		if (!strokePattern) {
			return false;
		}
		return strokePattern->getOpacity() > 0.0;
	}
	//-------------------------------------------------------------------------
	virtual bool isFilled() const {
		if (!fillPattern) {
			return false;
		}
		return fillPattern->getOpacity() > 0.0;
	}
	//-------------------------------------------------------------------------
	virtual void setStrokeWidth( const Coordinate &val ) {
		cairo_set_line_width(context, val);
	}
	//-------------------------------------------------------------------------
	virtual Coordinate getStrokeWidth() const {
		return Coordinate(cairo_get_line_width(context));
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: @deprecated use setFillPattern
	 * @param val
	 */
	virtual void setFillColor( const ColorRGBA &val ) {
		patternInUse = INVALID;
		fillPattern = CairoSolidPattern::create(val);
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO:  @deprecated use getFillPattern
	 * @return
	 */
	virtual ColorRGBA getFillColor() const {
		CairoSolidPattern::Ptr sPt =
				boost::shared_dynamic_cast<CairoSolidPattern>(fillPattern);
		if (!sPt)
			return false;
		return sPt->getSolidColor();
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: @deprecated use setStrokePattern
	 * @param val
	 */
	virtual void setStrokeColor( const ColorRGBA &val ) {
		patternInUse = INVALID;
		strokePattern = CairoSolidPattern::create(val);
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: depreciated use getStrokePattern
	 * @param val
	 */
	virtual ColorRGBA getStrokeColor() const {
		CairoSolidPattern::Ptr sPt =
				boost::shared_dynamic_cast<CairoSolidPattern>(strokePattern);
		if (!sPt)
			return false;
		return sPt->getSolidColor();
	}
	//-------------------------------------------------------------------------
	virtual void setDash( Dash::Ptr dash ) {
		currentDash = dash;
		if (!dash) {
			disableDash();
			return;
		}
		// copy dashes to plain array
		boost::scoped_array<Number> bff(new Number[dash->size()]);
		for(size_t i=0; i<dash->size(); ++i) {
			bff[i] = (*dash)[i];
		}
		cairo_set_dash(
			context,
			bff.get(),
			dash->size(),
			dash->offset()
		);
	}
	//-------------------------------------------------------------------------
	virtual void setClip(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual void disableDash() {
		cairo_set_dash( context, NULL, 0, 0 );
	}
	//-------------------------------------------------------------------------
	virtual Dash::Ptr getDash() const {
		return currentDash;
	}
	//-------------------------------------------------------------------------
	virtual void setLineCap ( LineCapStyle style ) {
		cairo_set_line_cap( context, (cairo_line_cap_t)style );
	}
	//-------------------------------------------------------------------------
	virtual LineCapStyle getLineCap () const {
		return (LineCapStyle)cairo_get_line_cap(context);
	}
	//-------------------------------------------------------------------------
	virtual void setFillRule( FillRule rule ) {
		cairo_set_fill_rule(context, (cairo_fill_rule_t)rule);
	}
	//-------------------------------------------------------------------------
	virtual FillRule getFillRule() const {
		return (FillRule)cairo_get_fill_rule(context);
	}
	//-------------------------------------------------------------------------
	virtual void setLineJoin(LineJoin join) {
		cairo_set_line_join(context, (cairo_line_join_t)join);
	}
	//-------------------------------------------------------------------------
	virtual LineJoin getLineJoin() const {
		return (LineJoin)cairo_get_line_join(context);
	}
	//-------------------------------------------------------------------------
	virtual void setMiterLimit(const Number &v) {
		cairo_set_miter_limit(context, v);
	}
	//-------------------------------------------------------------------------
	virtual Number getMiterLimit() const {
		return cairo_get_miter_limit(context);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Misc.
	//-------------------------------------------------------------------------
	virtual void save() {
		_save();
		cairo_save(context);
	}
	//-------------------------------------------------------------------------
	virtual void restore() {
		_restore();
		cairo_restore(context);
	}
	//-------------------------------------------------------------------------
	virtual Point2D getCurrentPoint() const {
		if (!cairo_has_current_point(context)) {
			return Point2D(0,0);
		}
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
	//-------------------------------------------------------------------------
	virtual Rectangle clipExtends() const {
		double x0, y0, x1, y1;
		x0 = y0 = x1 = y1 = NULL_NUMBER;
		cairo_clip_extents(context, &x0, &y0, &x1, &y1);
		return Rectangle(Point2D(x0,y0), Point2D(x1, y1));
	}
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPath() const {
		cairo_path_t * path = cairo_copy_path(context);
		return CairoPath::create(path);
	}
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPathFlat() const {
		cairo_path_t * path = cairo_copy_path_flat(context);
		return CairoPath::create(path);
	}
	//-------------------------------------------------------------------------
	virtual void appendPath( Path::Ptr path ) {
		CairoPath::Ptr cp = boost::shared_dynamic_cast<CairoPath, Path>(path);
		if (!cp) return;
		cairo_append_path(context, cp->getPath());
	}
	//-------------------------------------------------------------------------
	virtual void newSubPath() {
		cairo_new_sub_path(context);
	}
	//-------------------------------------------------------------------------
	virtual ISurface::Ptr getSurface() const {
		return surfaceRef;
	}
	//-------------------------------------------------------------------------
	virtual Rectangle pathExtends() const {
		Number x,y,x1,y1;
		cairo_path_extents(context, &x, &y, &x1, &y1);
		return Rectangle(Point2D(x,y), Point2D(x1, y1));
	}
	//-------------------------------------------------------------------------
	virtual void copyTo(IDrawContext::Ptr cn) const;
	//-------------------------------------------------------------------------
	virtual void copyAreaTo(IDrawContext::Ptr cn,
			const Rectangle &src, const Point2D &dst) const;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Transformation
	//-------------------------------------------------------------------------
	virtual void translate( const Point2D &p0 ) {
		cairo_translate( context, p0.x(), p0.y() );
	}
	//-------------------------------------------------------------------------
	virtual void scale( const Point2D &p0 ) {
		cairo_scale( context, p0.x(), p0.y() );
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
	//-------------------------------------------------------------------------
	virtual void getMatrix( Matrix &m ) {
		if ( m.size1() != 3 || m.size2() != 3 ) return;
		cairo_matrix_t cm;
		cairo_get_matrix(context, &cm);
		cairoMatrixToDiscoMatrix(cm, m);
	}
};

}} // namespaces

#endif /* CAIRODRAWCONTEXT_H_ */
