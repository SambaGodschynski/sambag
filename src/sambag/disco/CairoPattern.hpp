/*
 * CairoPattern.hpp
 *
 *  Created on: Dec 15, 2011
 *      Author: samba
 */

#ifndef DISCO_CAIROPATTERN_HPP_
#define DISCO_CAIROPATTERN_HPP_

#include <cairo.h>
#include "IPattern.hpp"
#include "sambag/com/Common.hpp"
#include "CairoHelper.hpp"
#include "ColorRGBA.hpp"

namespace sambag { namespace disco {
//-----------------------------------------------------------------------------
typedef boost::shared_ptr<cairo_pattern_t> CairoPatternRef;
inline void destroyCairoPattern(cairo_pattern_t *p) {
	cairo_pattern_destroy(p);
}
inline CairoPatternRef createPatternRef(cairo_pattern_t *p) {
	return CairoPatternRef(p, &destroyCairoPattern);
}
//=============================================================================
/**
 * A pattern is the basic fill structure for every drawing operation.
 * Its structure is similar to Cairos cairo_pattern_t
 */
class CairoPatternBase : public virtual IPattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoPatternBase> Ptr;
private:
	//-------------------------------------------------------------------------
	Number opacity;
protected:
	//-------------------------------------------------------------------------
	CairoPatternRef cairoPatternRef;
	//-------------------------------------------------------------------------
	virtual void _setOpacity() {}
public:
	//-------------------------------------------------------------------------
	CairoPatternRef getCairoPatternRef() const {
		return cairoPatternRef;
	}
	//-------------------------------------------------------------------------
	CairoPatternBase(CairoPatternRef ref) : cairoPatternRef(ref) {}
	//-------------------------------------------------------------------------
	virtual void setOpacity(const Number &v) {
		if (opacity==v)
			return;
		opacity = v;
		_setOpacity();
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const {
		return opacity;
	}
	//-------------------------------------------------------------------------
	virtual void setMatrix (const sambag::math::Matrix &m) {
		if (m.size1() != 3 && m.size2() != 3)
			return;
		cairo_matrix_t cm;
		discoMatrixToCairoMatrix(m, cm);
		cairo_pattern_set_matrix(cairoPatternRef.get(), &cm);
	}
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrix() const {
		cairo_matrix_t cm;
		Matrix res(3,3);
		cairo_pattern_get_matrix(cairoPatternRef.get(), &cm);
		discoMatrixToCairoMatrix(res, cm);
		return res;
	}
};
//=============================================================================
class CairoSolidPattern : public CairoPatternBase, public ASolidPattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoSolidPattern> Ptr;
private:
	//-------------------------------------------------------------------------
	CairoSolidPattern(CairoPatternRef ref) : CairoPatternBase(ref) {}
protected:
	//-------------------------------------------------------------------------
	virtual void _setOpacity() {
		ColorRGBA col = getSolidColor();
		// there is no way to set the solid color, so we have to
		// re-create the pattern
		cairoPatternRef = createPatternRef(
				cairo_pattern_create_rgba(col.getR(),
							col.getG(),
							col.getB(),
							getOpacity())
				);
	}
public:
	//-------------------------------------------------------------------------
	ColorRGBA getSolidColor() const {
		Number r,g,b,a;
		cairo_pattern_get_rgba(cairoPatternRef.get(), &r, &g, &b, &a);
		return ColorRGBA(r,g,b,a);
	}
	//-------------------------------------------------------------------------
	static Ptr create(const ColorRGBA &col) {
		CairoPatternRef ref = createPatternRef(
			cairo_pattern_create_rgba(col.getR(),
					col.getG(),
					col.getB(),
					col.getA())
		);
		Ptr neu(new CairoSolidPattern(ref));
		return neu;
	}
};
//=============================================================================
class CairoGradient : public virtual IGradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoGradient> Ptr;
protected:
	//-------------------------------------------------------------------------
	const CairoPatternBase &base;
	//-------------------------------------------------------------------------
	CairoGradient(const CairoPatternBase &base) : base(base) {}
public:
	//-------------------------------------------------------------------------
	void getStops( ColorStops &out,
			size_t startIndex = 0,
			size_t _endIndex = INT_MAX) const
	{
		int endIndex = _endIndex;
		if (endIndex==INT_MAX) {
			cairo_pattern_get_color_stop_count(base.getCairoPatternRef().get(),
					&endIndex);
		}
		if (startIndex>_endIndex) // important for vector.reserve()
			return;
		out.reserve(endIndex-startIndex);
		for(int i=startIndex; i<endIndex; ++i) {
			out.push_back(getColorStop(i));
		}
	}
	//-------------------------------------------------------------------------
	void addColorStops(const ColorStops &newStops) {
		boost_for_each(const ColorStop &col, newStops) {
			addColorStop(col);
		}
	}
	//-------------------------------------------------------------------------
	void addColorStop ( const ColorStop &colStop ) {
		ColorRGBA col;
		Number offset;
		boost::tie(col, offset) = colStop;
		cairo_pattern_add_color_stop_rgba( base.getCairoPatternRef().get(),
			offset,
			col.getR(),
			col.getG(),
			col.getB(),
			col.getA()
		);
	}
	//-------------------------------------------------------------------------
	void addColorStop ( const ColorRGBA &col, const Number &offset ) {
		addColorStop(ColorStop(col, offset));
	}
	//-------------------------------------------------------------------------
	size_t getNumStop() const {
		int res;
		cairo_pattern_get_color_stop_count(base.getCairoPatternRef().get(), &res);
		return res;
	}
	//-------------------------------------------------------------------------
	ColorStop getColorStop ( size_t index ) const {
		Number offset, r, g, b, a;
		cairo_pattern_get_color_stop_rgba(base.getCairoPatternRef().get(), index,
			&offset, &r, &g, &b, &a);
		return ColorStop(ColorRGBA(r,g,b,a), offset);
	}
};
//=============================================================================
class CairoLinearPattern :
	public CairoPatternBase,
	public CairoGradient,
	public ALinearPattern
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoLinearPattern> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Point2D> LinearPoints;
private:
	//-------------------------------------------------------------------------
	CairoLinearPattern(CairoPatternRef ref) :
		CairoPatternBase(ref), CairoGradient((CairoPatternBase&)*this) {}
protected:
	//-------------------------------------------------------------------------
	virtual void _setOpacity() {
		// there is no way to set the solid color, so we have to
		// re-create the pattern
		ColorStops stops;
		getStops(stops);
		cairo_matrix_t cm;
		cairo_pattern_get_matrix(cairoPatternRef.get(), &cm);
		boost_for_each(ColorStop &s, stops) {
			Number a = s.get<IGradient::COLOR>().getA();
			s.get<IGradient::COLOR>().setA(a*getOpacity());
		}
		Point2D p0, p1;
		boost::tie(p0, p1) = getLinearPoints();
		cairoPatternRef = createPatternRef(
			cairo_pattern_create_linear(p0.x(), p0.y(), p1.x(), p1.y())
		);
		cairo_pattern_set_matrix(cairoPatternRef.get(), &cm);
		addColorStops(stops);
	}
public:
	//-------------------------------------------------------------------------
	LinearPoints getLinearPoints() const {
		Number x0, y0, x1, y1;
		cairo_pattern_get_linear_points( cairoPatternRef.get(),
			&x0, &y0, &x1, &y1);
		return LinearPoints(Point2D(x0, y0),Point2D(x1, y1));
	}
	//-------------------------------------------------------------------------
	static Ptr create(const LinearPoints &p) {
		Point2D p0, p1;
		boost::tie(p0, p1) = p;
		CairoPatternRef ref = createPatternRef(
			cairo_pattern_create_linear(p0.x(), p0.y(), p1.x(), p1.y())
		);
		Ptr neu(new CairoLinearPattern(ref));
		return neu;
	}
};
//=============================================================================
class CairoRadialPattern :
	public CairoPatternBase,
	public CairoGradient,
	public ARadialPattern
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoRadialPattern> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Number, Point2D, Number> RadialCircles;
private:
	//-------------------------------------------------------------------------
	CairoRadialPattern(CairoPatternRef ref) :
		CairoPatternBase(ref), CairoGradient((CairoPatternBase&)*this) {}
protected:
	//-------------------------------------------------------------------------
	virtual void _setOpacity() {
		// there is no way to set the solid color, so we have to
		// re-create the pattern
		ColorStops stops;
		getStops(stops);
		cairo_matrix_t cm;
		cairo_pattern_get_matrix(cairoPatternRef.get(), &cm);
		boost_for_each(ColorStop &s, stops) {
			Number a = s.get<IGradient::COLOR>().getA();
			s.get<IGradient::COLOR>().setA(a*getOpacity());
		}
		Point2D c0, c1;
		Number r0, r1;
		boost::tie(c0, r0, c1, r1) = getRadialCircles();
		cairoPatternRef = createPatternRef(
			cairo_pattern_create_radial(c0.x(), c0.y(), r0, c1.x(), c1.y(), r1)
		);
		cairo_pattern_set_matrix(cairoPatternRef.get(), &cm);
		addColorStops(stops);
	}
public:
	//-------------------------------------------------------------------------
	RadialCircles getRadialCircles() const {
		Number cx0, cy0, cx1, cy1, r0, r1;
		cairo_pattern_get_radial_circles( cairoPatternRef.get(),
			&cx0, &cy0, &r0, &cx1, &cy1, &r1);
		return RadialCircles(Point2D(cx0, cy0), r0, Point2D(cx1, cy1), r1);
	}
	//-------------------------------------------------------------------------
	static Ptr create(const RadialCircles &p) {
		Point2D c0, c1;
		Number r0, r1;
		boost::tie(c0, r0, c1, r1) = p;
		CairoPatternRef ref = createPatternRef(
			cairo_pattern_create_radial(c0.x(), c0.y(), r0, c1.x(), c1.y(), r1)
		);
		Ptr neu(new CairoRadialPattern(ref));
		return neu;
	}

};
//=============================================================================
class CairoSurfacePattern :
	public CairoPatternBase,
	public ASurfacePattern
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoSurfacePattern> Ptr;
private:
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
	//-------------------------------------------------------------------------
	CairoSurfacePattern(CairoPatternRef ref) :
		CairoPatternBase(ref) {}
public:
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	static Ptr create(ISurface::Ptr _surf);
};
}}
#endif /* CAIROPATTERN_HPP_ */
