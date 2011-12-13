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
class APattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<APattern> Ptr;
	//-------------------------------------------------------------------------
	enum Type { SOLID, LINEAR, RADIAL, SURFACE, PROXY };
	//-------------------------------------------------------------------------
	typedef std::pair<ColorRGBA, double> ColorStop;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Point2D> LinearPoints;
	//-------------------------------------------------------------------------
	typedef boost::tuple<Point2D, Number, Point2D, Number> RadialCircles;
	//-------------------------------------------------------------------------
	typedef std::vector<ColorStop> ColorStops;
	//-------------------------------------------------------------------------
	Number opacity;
	//-------------------------------------------------------------------------
protected:
	//-------------------------------------------------------------------------
	// will be called if a pattern value changed.
	// used by CairoProxyPattern to re-build cairo pattern.
	virtual void resetPattern() {}
	//-------------------------------------------------------------------------
	sambag::math::Matrix matrix;
	//-------------------------------------------------------------------------
	APattern() :
		opacity(1.),
		matrix(sambag::math::IDENTITY_MATRIX) {}
public:
	//-------------------------------------------------------------------------
	/**
	 * copies all common pattern values from a to b.
	 * @param a
	 * @param b
	 */
	static void copyValues(const APattern &a, APattern &b) {
		b.setOpacity(a.getOpacity());
		b.setMatrix(a.getMatrix());
	}
	//-------------------------------------------------------------------------
	// TODO: pattern comparison
	//-------------------------------------------------------------------------
	/**
	 * @param v
	 */
	virtual void setOpacity(const Number &v) {
		opacity = v;
		resetPattern();
	}
	//-------------------------------------------------------------------------
	virtual Number getOpacity() const {
		return opacity;
	}
	//-------------------------------------------------------------------------
	virtual ~APattern(){}
	//-------------------------------------------------------------------------
	/**
	 * Note that all values have to be inverted.
	 * see:
	 * http://cairographics.org/manual/cairo-cairo-pattern-t.html#cairo-pattern-set-matrix
	 * @param m
	 */
	virtual void setMatrix (const sambag::math::Matrix &m) {
		if (m.size1() != 3 || m.size2() !=3)
			return;
		matrix = m;
		resetPattern();
	}
	//-------------------------------------------------------------------------
	virtual const sambag::math::Matrix & getMatrix() const {
		return matrix;
	}
	//-------------------------------------------------------------------------
	virtual Type getType() const = 0;
};
//=============================================================================
class SolidPattern : public APattern {
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
		neu->solidColor = col;
		return neu;
	}
	//-------------------------------------------------------------------------
	const ColorRGBA & getSolidColor() const {
		return solidColor;
	}
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return SOLID;
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
protected:
	virtual void resetGradient() {};
public:
	//-------------------------------------------------------------------------
	const ColorStops & getStops() const {
		return stops;
	}
	//-------------------------------------------------------------------------
	void addColorStops(const ColorStops &newStops) {
		stops = newStops;
		resetGradient();
	}
	//-------------------------------------------------------------------------
	void addColorStop ( const ColorStop &col ) {
		stops.push_back(col);
		resetGradient();
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
class LinearPattern : public APattern, public Gradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<LinearPattern> Ptr;
private:
	//-------------------------------------------------------------------------
	LinearPoints linearPoints;
	//-------------------------------------------------------------------------
	LinearPattern() {}
	//-------------------------------------------------------------------------
	virtual void resetGradient() {
		resetPattern();
	};
public:
	//-------------------------------------------------------------------------
	static Ptr create (const Point2D &p0, const Point2D &p1 ) {
		Ptr neu(new LinearPattern());
		neu->linearPoints = LinearPoints(p0, p1);
		return neu;
	}
	//-------------------------------------------------------------------------
	const LinearPoints & getLinearPoints() const {
		return linearPoints;
	}
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return LINEAR;
	}
};
//=============================================================================
class RadialPattern : public APattern, public Gradient {
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
	//-------------------------------------------------------------------------
	virtual void resetGradient() {
		resetPattern();
	};
public:
	//-------------------------------------------------------------------------
	static Ptr create (const Point2D &c0, Number radius,
					   const Point2D &c1, Number radius2)
	{
		Ptr neu(new RadialPattern());
		neu->radialCirlces = RadialCircles(c0, radius, c1, radius2);
		return neu;
	}
	//-------------------------------------------------------------------------
	const RadialCircles & getRadialCircles() const {
		return radialCirlces;
	}
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return RADIAL;
	}
};
//=============================================================================
class SurfacePattern : public APattern {
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
		neu->surface = _surface;
		return neu;
	}
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return SURFACE;
	}
};
//=============================================================================
/**
 * @ProxyPattern
 * to have a pattern object before a concrete pattern object was created.
 */
class ProxyPattern : public APattern {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ProxyPattern> Ptr;
protected:
	//-------------------------------------------------------------------------
	ProxyPattern() {}
	//-------------------------------------------------------------------------
	APattern::Ptr target;
public:
	//-------------------------------------------------------------------------
	enum AdaptionPolicy {
		PROXY_TO_TARGET,
		TARGET_TO_PROXY
	};
	//-------------------------------------------------------------------------
	static Ptr create () {
		Ptr neu(new ProxyPattern());
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 * @param _target
	 * @param pol decides whether all proxy values are copied to target or
	 * 		  all target values to proxy.
	 */
	virtual void setTarget(APattern::Ptr _target,
			AdaptionPolicy pol = TARGET_TO_PROXY)
	{
		target = _target;
		if (!target)
			return;
		switch (pol) {
		case PROXY_TO_TARGET:
			copyValues(*this, *(target.get()));
			break;
		case TARGET_TO_PROXY:
			copyValues(*(target.get()), *this);
			break;
		}
		resetPattern();
	}
	//-------------------------------------------------------------------------
	virtual APattern::Ptr getTarget() const {
		return target;
	}
	//-------------------------------------------------------------------------
	virtual Type getType() const {
		return PROXY;
	}
	//-------------------------------------------------------------------------
	virtual void setOpacity(const Number &v) {
		APattern::setOpacity(v);
		if (target)
			target->setOpacity(v);
	}
	//-------------------------------------------------------------------------
	virtual void setMatrix (const sambag::math::Matrix &m) {
		APattern::setMatrix(m);
		if (target)
			target->setMatrix(m);
	}
};
}} // namespace
#endif /* PATTERN_HPP_ */
