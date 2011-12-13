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
#include <limits.h>


namespace sambag { namespace disco {
//=============================================================================
/**
 * A pattern is the basic fill structure for every drawing operation.
 * Its structure is similar to Cairos cairo_pattern_t
 */
class IPattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IPattern> Ptr;
	//-------------------------------------------------------------------------
	enum Type { SOLID, LINEAR, RADIAL, SURFACE };
	//-------------------------------------------------------------------------
	// TODO: pattern comparison
	//-------------------------------------------------------------------------
	/**
	 * @param v
	 */
	virtual void setOpacity(const Number &v) = 0;
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const = 0;
	//-------------------------------------------------------------------------
	virtual ~IPattern(){}
	//-------------------------------------------------------------------------
	/**
	 * Note that all values have to be inverted.
	 * see:
	 * http://cairographics.org/manual/cairo-cairo-pattern-t.html#cairo-pattern-set-matrix
	 * @param m
	 */
	virtual void setMatrix (const sambag::math::Matrix &m) = 0;
	//-------------------------------------------------------------------------
	virtual sambag::math::Matrix getMatrix() const = 0;
	//-------------------------------------------------------------------------
	virtual Type getType() const = 0;
};
//=============================================================================
class ASolidPattern : public virtual IPattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ASolidPattern> Ptr;
	//-------------------------------------------------------------------------
	virtual ColorRGBA getSolidColor() const = 0;
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return SOLID;
	}
};
//=============================================================================
class IGradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::tuple<ColorRGBA, Number> ColorStop;
	//-------------------------------------------------------------------------
	enum { COLOR, OFFSET };
	//-------------------------------------------------------------------------
	typedef std::vector<ColorStop> ColorStops;
	//-------------------------------------------------------------------------
	virtual void getStops( ColorStops &out,
			size_t startIndex = 0,
			size_t endIndex = INT_MAX) const = 0;
	//-------------------------------------------------------------------------
	virtual void addColorStops(const ColorStops &newStops) = 0;
	//-------------------------------------------------------------------------
	virtual void addColorStop ( const ColorStop &col ) = 0;
	//-------------------------------------------------------------------------
	virtual void addColorStop ( const ColorRGBA &col, const Number &offset ) = 0;
	//-------------------------------------------------------------------------
	virtual size_t getNumStop() const = 0;
	//-------------------------------------------------------------------------
	virtual ColorStop getColorStop ( size_t index ) const = 0;
};
//=============================================================================
class ALinearPattern : public virtual IPattern, public virtual IGradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ALinearPattern> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Point2D> LinearPoints;
	//-------------------------------------------------------------------------
	virtual LinearPoints getLinearPoints() const = 0;
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return LINEAR;
	}
};
//=============================================================================
class ARadialPattern : public virtual IPattern, public virtual IGradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Number, Point2D, Number> RadialCircles;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ARadialPattern> Ptr;
	//-------------------------------------------------------------------------
	virtual RadialCircles getRadialCircles() const = 0;
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return RADIAL;
	}
};
//=============================================================================
class ASurfacePattern : public virtual IPattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ASurfacePattern> Ptr;
	//-------------------------------------------------------------------------
	virtual ISurface::Ptr getSurface() const = 0;
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return SURFACE;
	}
};
}} // namespace
#endif /* PATTERN_HPP_ */