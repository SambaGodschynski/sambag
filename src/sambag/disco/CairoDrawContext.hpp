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


extern bool isFatalCairoStatus(cairo_status_t);

#define DISCO_CONTEXT_CHECK_ERROR 1
#ifdef DISCO_CONTEXT_CHECK_ERROR
#define SAMBAG_CHECK_CONTEXT_STATE(cn){		  \
	cairo_status_t status = cairo_status(cn); \
	if (status!=CAIRO_STATUS_SUCCESS)	{     \
		if (isFatalCairoStatus(status))		  \
			throw std::runtime_error(cairo_status_to_string(status)); \
	}}
#else
#define SAMBAG_CHECK_CONTEXT_STATE(cn)
#endif



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
	virtual void moveTo( const Point2D &p0 );
	//-------------------------------------------------------------------------
	virtual void lineTo( const Point2D &p0 );
	//-------------------------------------------------------------------------
	virtual void relMoveTo( const Point2D &p0 );
	//-------------------------------------------------------------------------
	virtual void relLineTo( const Point2D &p0 );
	//-------------------------------------------------------------------------
	virtual void closePath();
	//-------------------------------------------------------------------------
	virtual void curveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 );
	//-------------------------------------------------------------------------
	virtual void relCurveTo(
		const Point2D &x1,
		const Point2D &x2,
		const Point2D &x3 );
	//-------------------------------------------------------------------------
	virtual void quadraticCurveTo( const Point2D &x1, const Point2D &x2 );
	//-------------------------------------------------------------------------
	virtual void relQuadraticCurveTo( const Point2D &x1, const Point2D &x2 );
	//-------------------------------------------------------------------------
	virtual void arc(
		const Point2D &c,
		const Coordinate &r,
		const Coordinate &angle1 = 0,
		const Coordinate &angle2 = M_PI *2.0 );
	//-------------------------------------------------------------------------
	virtual void arcNegative(
		const Point2D &c,
		const Coordinate &r,
		const Coordinate &angle1 = 0,
		const Coordinate &angle2 = M_PI *2.0 );
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect );
	//-------------------------------------------------------------------------
	virtual void rect( const Rectangle &rect, const Number &cornerRadius );
	//-------------------------------------------------------------------------
	virtual void stroke();
	//-------------------------------------------------------------------------
	virtual void fill();
	//-------------------------------------------------------------------------
	virtual void fillPreserve();
	//-------------------------------------------------------------------------
	/**
	 * draws an surface.
	 * 	 * @param a CairoSurface all others will be ignored.
	 */
	virtual void drawSurface(ISurface::Ptr surface, Number opacity = 1.0);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Text
	//-------------------------------------------------------------------------
	virtual void setFont( const Font &font );
	//-------------------------------------------------------------------------
	virtual void setFontFace(const Font::FontFace &fontFace);
	//-------------------------------------------------------------------------
	virtual void setFontWeight(const Font::Weight &weight);
	//-------------------------------------------------------------------------
	virtual void setFontSlant(const Font::Slant &slant);
	//-------------------------------------------------------------------------
	virtual void setFontSize(const Font::Size &size);
	//-------------------------------------------------------------------------
	virtual Font getCurrentFont() const;
	//-------------------------------------------------------------------------
	virtual void textPath( const std::string &text );
	//-------------------------------------------------------------------------
	virtual void setFillPattern(IPattern::Ptr pattern);
	//-------------------------------------------------------------------------
	virtual void setStrokePattern(IPattern::Ptr pattern);
	//-------------------------------------------------------------------------
	virtual Rectangle textExtends(const std::string &str) const;
	//-------------------------------------------------------------------------
	virtual TextExtends textExtendsX(const std::string &str) const;
	//-------------------------------------------------------------------------
	virtual Number getCurrentFontHeight() const;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Styling
	//-------------------------------------------------------------------------
	virtual bool isStroked() const;
	//-------------------------------------------------------------------------
	virtual bool isFilled() const;
	//-------------------------------------------------------------------------
	virtual void setStrokeWidth( const Coordinate &val );
	//-------------------------------------------------------------------------
	virtual Coordinate getStrokeWidth() const;
	//-------------------------------------------------------------------------
	/**
	 * TODO: @deprecated use setFillPattern
	 * @param val
	 */
	virtual void setFillColor( const ColorRGBA &val );
	//-------------------------------------------------------------------------
	/**
	 * TODO:  @deprecated use getFillPattern
	 * @return
	 */
	virtual ColorRGBA getFillColor() const;
	//-------------------------------------------------------------------------
	/**
	 * TODO: @deprecated use setStrokePattern
	 * @param val
	 */
	virtual void setStrokeColor( const ColorRGBA &val );
	//-------------------------------------------------------------------------
	/**
	 * TODO: depreciated use getStrokePattern
	 * @param val
	 */
	virtual ColorRGBA getStrokeColor() const;
	//-------------------------------------------------------------------------
	virtual void setDash( Dash::Ptr dash );
	//-------------------------------------------------------------------------
	virtual void setClip(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual void resetClip();
	//-------------------------------------------------------------------------
	virtual void disableDash();
	//-------------------------------------------------------------------------
	virtual Dash::Ptr getDash() const;
	//-------------------------------------------------------------------------
	virtual void setLineCap ( LineCapStyle style );
	//-------------------------------------------------------------------------
	virtual LineCapStyle getLineCap () const;
	//-------------------------------------------------------------------------
	virtual void setFillRule( FillRule rule );
	//-------------------------------------------------------------------------
	virtual FillRule getFillRule() const;
	//-------------------------------------------------------------------------
	virtual void setLineJoin(LineJoin join);
	//-------------------------------------------------------------------------
	virtual LineJoin getLineJoin() const;
	//-------------------------------------------------------------------------
	virtual void setMiterLimit(const Number &v);
	//-------------------------------------------------------------------------
	virtual Number getMiterLimit() const;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Misc.
	//-------------------------------------------------------------------------
	virtual void save();
	//-------------------------------------------------------------------------
	virtual void restore();
	//-------------------------------------------------------------------------
	virtual Point2D getCurrentPoint() const;
	//-------------------------------------------------------------------------
	virtual bool hasCurrentPoint() const;
	//-------------------------------------------------------------------------
	virtual void clip();
	//-------------------------------------------------------------------------
	virtual Rectangle clipExtends() const;
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPath() const;
	//-------------------------------------------------------------------------
	virtual Path::Ptr copyPathFlat() const;
	//-------------------------------------------------------------------------
	virtual void appendPath( Path::Ptr path );
	//-------------------------------------------------------------------------
	virtual void newSubPath();
	//-------------------------------------------------------------------------
	virtual ISurface::Ptr getSurface() const;
	//-------------------------------------------------------------------------
	virtual Rectangle pathExtends() const;
	//-------------------------------------------------------------------------
	virtual void copyTo(IDrawContext::Ptr cn) const;
	//-------------------------------------------------------------------------
	virtual void copyAreaTo(IDrawContext::Ptr cn,
			const Rectangle &src, const Point2D &dst) const;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Transformation
	//-------------------------------------------------------------------------
	virtual void translate( const Point2D &p0 );
	//-------------------------------------------------------------------------
	virtual void scale( const Point2D &p0 );
	//-------------------------------------------------------------------------
	virtual void rotate( const Number &angle );
	//-------------------------------------------------------------------------
	virtual void identityMatrix();
	//-------------------------------------------------------------------------
	// assumes a 3x3 matrix
	virtual void transform ( const Matrix &m );
	//-------------------------------------------------------------------------
	virtual void getMatrix(Matrix &m);
};

}} // namespaces

#endif /* CAIRODRAWCONTEXT_H_ */
