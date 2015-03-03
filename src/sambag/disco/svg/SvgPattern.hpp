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
#include "SvgRoot.hpp"
#include "units/Units.hpp"
#include "SvgPatternMatrix.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
/**
 * @class SvgPatternBase
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
//=============================================================================
/**
 * @class SvgPattern
 */
class SvgPattern : public SvgPatternBase {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef SvgPatternBase Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<SvgPattern> Ptr;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
    struct X_tag { typedef units::Unit Type; };
    struct Y_tag { typedef units::Unit Type; };
    struct Width_tag { typedef units::Unit Type; };
    struct Height_tag { typedef units::Unit Type; };
    struct Transform_tag { typedef SvgPatternMatrix Type; };
private:
    //-------------------------------------------------------------------------
    units::Point p;
    units::Dimension size;
    math::Matrix transform;
protected:
    //-------------------------------------------------------------------------
    SvgPattern();
    //-------------------------------------------------------------------------
    sambag::disco::svg::graphicElements::Compound::Ptr combo;
public:
    //-------------------------------------------------------------------------
    virtual void add(SvgObject::Ptr obj);
    //-------------------------------------------------------------------------
    virtual ~SvgPattern(){}
    //-------------------------------------------------------------------------
    static void registerAttributes(SvgObject::BuilderType &binder);
    //-------------------------------------------------------------------------
    /**
     * creates pattern with rect's bounds.
     * @param rect
     * @return
     */
    IPattern::Ptr createPattern() const;
    //-------------------------------------------------------------------------
    static Ptr create( SvgRoot *root = NULL );
    //-------------------------------------------------------------------------
    GraphicElement::Ptr getGraphicElement() const {
        return combo;
    }
    //-------------------------------------------------------------------------
    virtual void set( const X_tag::Type &coord, X_tag ) {
        p.x(coord);
    }
    //-------------------------------------------------------------------------
    virtual void set( const Y_tag::Type &coord, Y_tag ) {
        p.y(coord);
    }
    //-------------------------------------------------------------------------
    virtual void set( const Width_tag::Type &coord, Width_tag ) {
        size.width(coord);
    }
    //-------------------------------------------------------------------------
    virtual void set( const Height_tag::Type &coord, Height_tag ) {
        size.height(coord);
    }
    //-------------------------------------------------------------------------
    virtual void set( const Transform_tag::Type &m, Transform_tag ) {
        transform = m;
    }
 };
}}}

#endif /* SVGPATTERN_HPP_ */
