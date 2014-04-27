/*
 * SvgPattern.hpp
 *
 *  Created on: Dec 5, 2011
 *      Author: samba
 */

#ifndef SVGPATTERN_HPP_
#define SVGPATTERN_HPP_

#include "Svg.hpp"
#include "sambag/disco/IPattern.hpp"
#include "SvgColorStop.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
/**
 * @class SvgPattern
 * @brief base class for svg pattern objects.
 */
class SvgPatternBase : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgPatternBase> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
private:
protected:
	//-------------------------------------------------------------------------
	SvgPatternBase(){}
public:
	/**
	 * creates pattern with rect's bounds.
	 * @param rect
	 * @return
	 */
	virtual IPattern::Ptr createPattern() const = 0;
	//-------------------------------------------------------------------------
	virtual ~SvgPatternBase(){}
};
//=============================================================================
/**
 * @class SvgGradient
 * base class for svg pattern objects with gradients.
 */
class SvgGradient : public SvgPatternBase {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgGradient> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
private:
	//-------------------------------------------------------------------------
	IGradient::ColorStops stops;
protected:
	//-------------------------------------------------------------------------
	SvgGradient(){}
public:
	//-------------------------------------------------------------------------
	const IGradient::ColorStops & getColorStops() const {
		return stops;
	}
	//-------------------------------------------------------------------------
	void addColorStop(SvgColorStop::Ptr stop) {
		stops.push_back(stop->getStop());
	}
	//-------------------------------------------------------------------------
	void addColorStop(const ColorRGBA &col, const Number &offset) {
		stops.push_back(IGradient::ColorStop(col, offset));
	}
	//-------------------------------------------------------------------------
	virtual ~SvgGradient(){}
	//-------------------------------------------------------------------------
};
}}}

#endif /* SVGPATTERN_HPP_ */
