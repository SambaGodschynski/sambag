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
#include "sambag/math/Matrix.hpp"
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
protected:
	//-------------------------------------------------------------------------
	sambag::math::Matrix matrix;
	//-------------------------------------------------------------------------
	Type type;
	//-------------------------------------------------------------------------
	Pattern() : matrix(sambag::math::IDENTITY_MATRIX) {}
public:
	//-------------------------------------------------------------------------
	virtual ~Pattern(){}
	//-------------------------------------------------------------------------
	/**
	 * Note that all values have to be inverted.
	 * see:
	 * http://cairographics.org/manual/cairo-cairo-pattern-t.html#cairo-pattern-set-matrix
	 * @param m
	 */
	void setMatrix (const sambag::math::Matrix &m) {
		if (m.size1() != 3 || m.size2() !=3)
			return;
		matrix = m;
	}
	//-------------------------------------------------------------------------
	const sambag::math::Matrix & getMatrix() const {
		return matrix;
	}
	//-------------------------------------------------------------------------
	Type getType() const {
		return type;
	}
};
//=============================================================================
class SolidPattern : public Pattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SolidPattern> Ptr;
private:
	//-------------------------------------------------------------------------
	ColorRGBA solidColor;
	//-------------------------------------------------------------------------
	SolidPattern() {}
public:
	//-------------------------------------------------------------------------
	static Ptr create (const ColorRGBA &col) {
		Ptr neu(new SolidPattern());
		neu->type = SOLID;
		neu->solidColor = col;
		return neu;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & getSolidColor() const {
		return solidColor;
	}
};
//=============================================================================
class Gradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::tuple<ColorRGBA, Number> ColorStop;
	//-------------------------------------------------------------------------
	typedef std::vector<ColorStop> ColorStops;
	//-------------------------------------------------------------------------
	enum {
		COLOR,
		OFFSET
	};
private:
	//-------------------------------------------------------------------------
	ColorStops stops;
public:
	//-------------------------------------------------------------------------
	const ColorStops & getStops() const {
		return stops;
	}
	//-------------------------------------------------------------------------
	void addColorStops(const ColorStops &newStops) {
		stops = newStops;
	}
	//-------------------------------------------------------------------------
	void addColorStop ( const ColorStop &col ) {
		stops.push_back(col);
	}
	//-------------------------------------------------------------------------
	void addColorStop ( const ColorRGBA &col, const Number &offset ) {
		stops.push_back(ColorStop(col, offset));
	}
	//-------------------------------------------------------------------------
	size_t getNumStop() const {
		return stops.size();
	}
	//-------------------------------------------------------------------------
	const ColorStop & getColorStop ( size_t index ) const {
		return stops.at(index);
	}
};
//=============================================================================
class LinearPattern : public Pattern, public Gradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<LinearPattern> Ptr;
private:
	//-------------------------------------------------------------------------
	LinearPoints linearPoints;
	//-------------------------------------------------------------------------
	LinearPattern() {}
public:
	//-------------------------------------------------------------------------
	static Ptr create (const Point2D &p0, const Point2D &p1 ) {
		Ptr neu(new LinearPattern());
		neu->type = LINEAR;
		neu->linearPoints = LinearPoints(p0, p1);
		return neu;
	}
	//-------------------------------------------------------------------------
	const LinearPoints & getLinearPoints() const {
		return linearPoints;
	}
};
//=============================================================================
class RadialPattern : public Pattern, public Gradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Number, Point2D, Number> RadialCircles;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RadialPattern> Ptr;
private:
	//-------------------------------------------------------------------------
	RadialCircles radialCirlces;
	//-------------------------------------------------------------------------
	RadialPattern() {}
public:
	//-------------------------------------------------------------------------
	static Ptr create (const Point2D &c0, Number radius,
					   const Point2D &c1, Number radius2)
	{
		Ptr neu(new RadialPattern());
		neu->type = RADIAL;
		neu->radialCirlces = RadialCircles(c0, radius, c1, radius2);
		return neu;
	}
	//-------------------------------------------------------------------------
	const RadialCircles & getRadialCircles() const {
		return radialCirlces;
	}
};
//=============================================================================
class SurfacePattern : public Pattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SurfacePattern> Ptr;
protected:
	//-------------------------------------------------------------------------
	SurfacePattern() {}
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
public:
	//-------------------------------------------------------------------------
	static Ptr create (ISurface::Ptr _surface) {
		Ptr neu(new SurfacePattern());
		neu->type = SURFACE;
		neu->surface = _surface;
		return neu;
	}
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}
};
}} // namespace
#endif /* PATTERN_HPP_ */
