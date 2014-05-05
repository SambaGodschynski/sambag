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
namespace gradient {
//=============================================================================
//-----------------------------------------------------------------------------
void getStops( CairoPatternRef ref,
	IGradient::ColorStops &out,
		size_t startIndex = 0,
		size_t _endIndex = INT_MAX);
//-----------------------------------------------------------------------------
void addColorStops(CairoPatternRef ref, 
	const IGradient::ColorStops &newStops);
//-----------------------------------------------------------------------------
void addColorStop ( CairoPatternRef ref,
	const IGradient::ColorStop &colStop );
//-----------------------------------------------------------------------------
void addColorStop ( CairoPatternRef ref,
	const ColorRGBA &col, 
	const Number &offset );
//-----------------------------------------------------------------------------
size_t getNumStop(CairoPatternRef ref);
//-----------------------------------------------------------------------------
IGradient::ColorStop getColorStop ( CairoPatternRef ref, 
	size_t index );
} // namespace
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
public:
	//-------------------------------------------------------------------------
	CairoPatternRef getCairoPatternRef() const {
		return cairoPatternRef;
	}
	//-------------------------------------------------------------------------
        CairoPatternBase(CairoPatternRef ref);
	//-------------------------------------------------------------------------
	virtual void setOpacityValue(const Number &v) {
		opacity = v;
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacityValue() const {
		return opacity;
	}
	//-------------------------------------------------------------------------
	virtual void setMatrixValue (const sambag::math::Matrix &m) {
		if (m.size1() != 3 && m.size2() != 3)
			return;
		cairo_matrix_t cm;
		discoMatrixToCairoMatrix(m, cm);
		cairo_pattern_set_matrix(cairoPatternRef.get(), &cm);
	}
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrixValue() const {
		cairo_matrix_t cm;
		math::Matrix res(3,3);
		cairo_pattern_get_matrix(cairoPatternRef.get(), &cm);
		cairoMatrixToDiscoMatrix(cm, res);
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void setExtendType(Extend type);
	//-------------------------------------------------------------------------
	virtual Extend getExtendType() const;
};
//=============================================================================
class CairoSolidPattern : public CairoPatternBase, public virtual ISolidPattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoSolidPattern> Ptr;
private:
	//-------------------------------------------------------------------------
	CairoSolidPattern(CairoPatternRef ref) : CairoPatternBase(ref) {}
protected:
	//-------------------------------------------------------------------------
	void _setOpacity() {
		ColorRGBA col = getSolidColor();
		// there is no way to set the solid color, so we have to
		// re-create the pattern
		cairoPatternRef = createPatternRef(
				cairo_pattern_create_rgba(col.getR(),
							col.getG(),
							col.getB(),
							getOpacityValue())
				);
	}
public:
	//-------------------------------------------------------------------------
	virtual void setOpacity(const Number &v) {
		CairoPatternBase::setOpacityValue(v);
		_setOpacity();
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const {
		return CairoPatternBase::getOpacityValue();
	}
	//-------------------------------------------------------------------------
	virtual void setMatrix (const sambag::math::Matrix &m) {
		CairoPatternBase::setMatrixValue(m);
	}
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrix() const {
		return CairoPatternBase::getMatrixValue();
	}
	//-------------------------------------------------------------------------
	virtual ~CairoSolidPattern(){}
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
	//-------------------------------------------------------------------------
	virtual void setExtendType(Extend type) {
		CairoPatternBase::setExtendType(type);	
	}
	//-------------------------------------------------------------------------
	virtual Extend getExtendType() const {
		return CairoPatternBase::getExtendType();
	}
	//-------------------------------------------------------------------------
	/**
	 * @return representing color if exists. Otherwise NULL_COLOR.
	 */
	virtual ColorRGBA getColor() const {
		return getSolidColor();
	}
};
//=============================================================================
class CairoLinearPattern :
	public CairoPatternBase,
	public virtual ILinearPattern
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
		CairoPatternBase(ref){}
protected:
	//-------------------------------------------------------------------------
	virtual void _setOpacity() {
		// there is no way to set the solid color, so we have to
		// re-create the pattern
		ColorStops stops;
		gradient::getStops(cairoPatternRef, stops);
		cairo_matrix_t cm;
		cairo_pattern_get_matrix(cairoPatternRef.get(), &cm);
		boost_for_each(ColorStop &s, stops) {
			Number a = s.get<IGradient::COLOR>().getA();
			s.get<IGradient::COLOR>().setA(a*getOpacityValue());
		}
		Point2D p0, p1;
		boost::tie(p0, p1) = getLinearPoints();
		cairoPatternRef = createPatternRef(
			cairo_pattern_create_linear(p0.x(), p0.y(), p1.x(), p1.y())
		);
		cairo_pattern_set_matrix(cairoPatternRef.get(), &cm);
		gradient::addColorStops(cairoPatternRef, stops);
	}
public:
	//-------------------------------------------------------------------------
	virtual void getStops( ColorStops &out,
			size_t startIndex = 0,
			size_t endIndex = INT_MAX) const
	{
		gradient::getStops(cairoPatternRef, out, startIndex, endIndex);
	}
	//-------------------------------------------------------------------------
	virtual void addColorStops(const ColorStops &newStops) {
		gradient::addColorStops(cairoPatternRef, newStops);
	}
	//-------------------------------------------------------------------------
	virtual void addColorStop ( const ColorStop &col ) {
		gradient::addColorStop(cairoPatternRef, col);
	}
	//-------------------------------------------------------------------------
	virtual void addColorStop ( const ColorRGBA &col, const Number &offset ) {
		gradient::addColorStop(cairoPatternRef, col, offset);
	}
	//-------------------------------------------------------------------------
	virtual size_t getNumStop() const {
		return gradient::getNumStop(cairoPatternRef);
	}
	//-------------------------------------------------------------------------
	virtual ColorStop getColorStop ( size_t index ) const {
		return gradient::getColorStop(cairoPatternRef, index);
	}
	//-------------------------------------------------------------------------
	virtual void setOpacity(const Number &v) {
		if (v==getOpacityValue())
			return;
		setOpacityValue(v);
		_setOpacity();
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const {
		return CairoPatternBase::getOpacityValue();
	}
	//-------------------------------------------------------------------------
	virtual void setMatrix (const sambag::math::Matrix &m) {
		CairoPatternBase::setMatrixValue(m);
	}
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrix() const {
		return CairoPatternBase::getMatrixValue();
	}
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
	//-------------------------------------------------------------------------
	virtual void setExtendType(Extend type) {
		CairoPatternBase::setExtendType(type);	
	}
	//-------------------------------------------------------------------------
	virtual Extend getExtendType() const {
		return CairoPatternBase::getExtendType();
	}
	//-------------------------------------------------------------------------
	/**
	 * @return representing color if exists. Otherwise NULL_COLOR.
	 */
	virtual ColorRGBA getColor() const {
		// TODO: return colorstops average
		return ColorRGBA::NULL_COLOR;
	}
};
//=============================================================================
class CairoRadialPattern :
	public CairoPatternBase,
	public virtual IRadialPattern
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
		CairoPatternBase(ref) {}
protected:
	//-------------------------------------------------------------------------
	virtual void _setOpacity() {
		// there is no way to set the solid color, so we have to
		// re-create the pattern
		ColorStops stops;
		gradient::getStops(cairoPatternRef, stops);
		cairo_matrix_t cm;
		cairo_pattern_get_matrix(cairoPatternRef.get(), &cm);
		boost_for_each(ColorStop &s, stops) {
			Number a = s.get<IGradient::COLOR>().getA();
			s.get<IGradient::COLOR>().setA(a*getOpacityValue());
		}
		Point2D c0, c1;
		Number r0, r1;
		boost::tie(c0, r0, c1, r1) = getRadialCircles();
		cairoPatternRef = createPatternRef(
			cairo_pattern_create_radial(c0.x(), c0.y(), r0, c1.x(), c1.y(), r1)
		);
		cairo_pattern_set_matrix(cairoPatternRef.get(), &cm);
		gradient::addColorStops(cairoPatternRef, stops);
	}
public:
	//-------------------------------------------------------------------------
	virtual void getStops( ColorStops &out,
			size_t startIndex = 0,
			size_t endIndex = INT_MAX) const
	{
		gradient::getStops(cairoPatternRef, out, startIndex, endIndex);
	}
	//-------------------------------------------------------------------------
	virtual void addColorStops(const ColorStops &newStops) {
		gradient::addColorStops(cairoPatternRef, newStops);
	}
	//-------------------------------------------------------------------------
	virtual void addColorStop ( const ColorStop &col ) {
		gradient::addColorStop(cairoPatternRef, col);
	}
	//-------------------------------------------------------------------------
	virtual void addColorStop ( const ColorRGBA &col, const Number &offset ) {
		gradient::addColorStop(cairoPatternRef, col, offset);
	}
	//-------------------------------------------------------------------------
	virtual size_t getNumStop() const {
		return gradient::getNumStop(cairoPatternRef);
	}
	//-------------------------------------------------------------------------
	virtual ColorStop getColorStop ( size_t index ) const {
		return gradient::getColorStop(cairoPatternRef, index);
	}
	//-------------------------------------------------------------------------
	virtual void setOpacity(const Number &v) {
		if (v==getOpacityValue())
			return;
		CairoPatternBase::setOpacityValue(v);
		_setOpacity();
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const {
		return CairoPatternBase::getOpacityValue();
	}
	//-------------------------------------------------------------------------
	virtual void setMatrix (const sambag::math::Matrix &m) {
		CairoPatternBase::setMatrixValue(m);
	}
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrix() const {
		return CairoPatternBase::getMatrixValue();
	}
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
	//-------------------------------------------------------------------------
	virtual void setExtendType(Extend type) {
		CairoPatternBase::setExtendType(type);	
	}
	//-------------------------------------------------------------------------
	virtual Extend getExtendType() const {
		return CairoPatternBase::getExtendType();
	}
	//-------------------------------------------------------------------------
	/**
	 * @return representing color if exists. Otherwise NULL_COLOR.
	 */
	virtual ColorRGBA getColor() const {
		// TODO: return colorstops average
		return ColorRGBA::NULL_COLOR;
	}
};
//=============================================================================
class CairoSurfacePattern :
	public CairoPatternBase,
	public virtual ISurfacePattern
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
	virtual void setOpacity(const Number &v) {
		CairoPatternBase::setOpacityValue(v);
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const {
		return CairoPatternBase::getOpacityValue();
	}
	//-------------------------------------------------------------------------
	virtual void setMatrix (const sambag::math::Matrix &m) {
		CairoPatternBase::setMatrixValue(m);
	}
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrix() const {
		return CairoPatternBase::getMatrixValue();
	}
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	static Ptr create(ISurface::Ptr _surf);
	//-------------------------------------------------------------------------
	virtual void setExtendType(Extend type) {
		CairoPatternBase::setExtendType(type);	
	}
	//-------------------------------------------------------------------------
	virtual Extend getExtendType() const {
		return CairoPatternBase::getExtendType();
	}
	//-------------------------------------------------------------------------
	/**
	 * @return representing color if exists. Otherwise NULL_COLOR.
	 */
	virtual ColorRGBA getColor() const {
		return ColorRGBA::NULL_COLOR;
	}
};
}}
#endif /* CAIROPATTERN_HPP_ */
