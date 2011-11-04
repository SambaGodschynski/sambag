/*
 * Pattern.hpp
 *
 *  Created on: Nov 30, 2011
 *      Author: samba
 */

#ifndef PATTERN_HPP_
#define PATTERN_HPP_

#include "ColorRGBA.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "sambag/com/Common.hpp"
#include "Geometry.hpp"
#include "sambag/com/Exception.hpp"
#include "ISurface.hpp"
#include <boost/any.hpp>


namespace sambag { namespace disco {
//=============================================================================
/**
 * A pattern is the basic fill structure for every drawing operation.
 * Its structure is similar to Cairos cairo_pattern_t
 */
class Pattern {
//=============================================================================
public:
	EXCEPTION_CLASS(TypeMismatch);
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Pattern> Ptr;
	//-------------------------------------------------------------------------
	enum Type { SOLID, LINEAR, RADIAL, SURFACE };
	//-------------------------------------------------------------------------
	typedef std::pair<ColorRGBA, double> ColorStop;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Point2D> LinearPoints;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Number, Point2D, Number> RadialCircles;
	//-------------------------------------------------------------------------
	typedef std::vector<ColorStop> ColorStops;
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	Matrix matrix;
	//-------------------------------------------------------------------------
	Type type;
	//-------------------------------------------------------------------------
	ColorStops stops;
	/*//-------------------------------------------------------------------------
	LinearPoints linearPoints;
	//-------------------------------------------------------------------------
	RadialCircles radialCirlces;
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
	//-------------------------------------------------------------------------
	ColorRGBA solidColor;*/
	boost::any content;
	//-------------------------------------------------------------------------
	Pattern() : matrix(IDENTITY_MATRIX) {}
public:
	//-------------------------------------------------------------------------
	void addColorStop ( const ColorStop &col ) {
		stops.push_back(col);
	}
	//-------------------------------------------------------------------------
	size_t getNumStop() const {
		return stops.size();
	}
	//-------------------------------------------------------------------------
	const ColorStop & getColorStop ( size_t index ) const {
		return stops.at(index);
	}
	//-------------------------------------------------------------------------
	static Ptr createSolid (const ColorRGBA &col) {
		Ptr neu(new Pattern);
		neu->type = SOLID;
		neu->content = col;
		return neu;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & getSolidColor() const {
		const ColorRGBA *ptr = boost::any_cast<ColorRGBA>(&content);
		if (!ptr)
			RAISE_WARNING(TypeMismatch,"type mismatch");
		return *ptr;
	}
	//-------------------------------------------------------------------------
	static Ptr createLinear (const Point2D &p0, const Point2D &p1 ) {
		Ptr neu(new Pattern);
		neu->type = LINEAR;
		neu->content = LinearPoints(p0, p1);
		return neu;
	}
	//-------------------------------------------------------------------------
	const LinearPoints & getLinearPoints() const {
		const LinearPoints *ptr = boost::any_cast<LinearPoints>(&content);
		if (!ptr)
			RAISE_WARNING(TypeMismatch,"type mismatch");
		return *ptr;
	}
	//-------------------------------------------------------------------------
	static Ptr createSurface (ISurface::Ptr surface) {
		Ptr neu(new Pattern);
		neu->type = SURFACE;
		neu->content = surface;
		return neu;
	}
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface () {
		ISurface::Ptr *ptr = boost::any_cast<ISurface::Ptr>(&content);
		if (!ptr)
			RAISE_WARNING(TypeMismatch,"type mismatch");
		return *ptr;
	}
	//-------------------------------------------------------------------------
	static Ptr createRadial (const Point2D &c0, Number radius,
							 const Point2D &c1, Number radius2)
	{
		Ptr neu(new Pattern);
		neu->type = RADIAL;
		neu->content = RadialCircles(c0, radius, c1, radius2);
		return neu;
	}
	//-------------------------------------------------------------------------
	const RadialCircles & getRadialCircles() const {
		const RadialCircles *ptr = boost::any_cast<RadialCircles>(&content);
		if (!ptr)
			RAISE_WARNING(TypeMismatch,"type mismatch");
		return *ptr;
	}
	//-------------------------------------------------------------------------
	void setMatrix (const Matrix &m) {
		if (m.size1() != 3 || m.size2() !=3)
			return;
	}
	//-------------------------------------------------------------------------
	const Matrix & getMatrix() const {
		return matrix;
	}
	//-------------------------------------------------------------------------
	Type getType() const {
		return type;
	}
};
}} // namespace
#endif /* PATTERN_HPP_ */
