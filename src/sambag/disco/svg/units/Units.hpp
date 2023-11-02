/*
 * Units.hpp
 *
 *  Created on: Thu Jul 26 12:18:40 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SVGUNITS_H
#define SAMBAG_SVGUNITS_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/IDrawContext.hpp>

namespace sambag { namespace disco { namespace svg { namespace units {
//=============================================================================
/**
 * Handles various svg unit types.
 * (http://www.w3.org/TR/SVG/coords.html#Units)
 */
class Unit {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	enum Type {
				NONE,
				UnitPX,
				UnitIN,
				UnitCM,
				UnitMM,
				UnitPT,
				UnitPC,
				UnitEM,
				UnitEX,
				UnitPERCENT
			};
private:
	//-------------------------------------------------------------------------
	Coordinate value;
	//-------------------------------------------------------------------------
	Type type;
public:
	//-------------------------------------------------------------------------
	explicit Unit(const Coordinate &v = 0) : value(v), type(NONE) {}
	//-------------------------------------------------------------------------
	/**
	 * @return coordinates pixelvalue. To solve relative unit types
	 * an IDrawContext object or an absolute value is needed. If both NULL
     * NULL_NUMBER will be returned. 
	 * @param cn solves accordig to http://www.w3.org/TR/SVG/coords.html#Units
	 * @param absolute value to solve percent value type 
	 * @note if an absolute value and the context given 
	 * the absolute value it has the higher priority
     *
	 */
	Coordinate solve(IDrawContext::Ptr cn, Coordinate absolute = NULL_NUMBER) const;
	Coordinate solve(Coordinate absolute) const {
		return solve(IDrawContext::Ptr(), absolute);
	}
	//-------------------------------------------------------------------------
	void setValue(const Coordinate &v) { value = v; }
	//-------------------------------------------------------------------------
	/**
     * @return the value
     */	
	Coordinate getValue() const { return value; }
	//-------------------------------------------------------------------------
	/**
     * @return the type
     */		
	Type getType() const { return type; }
	//-------------------------------------------------------------------------
	void setType(Type t) { type = t; }

};
//=============================================================================
class Point {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	Unit _x;
	Unit _y;
public:
	//-------------------------------------------------------------------------
	const Unit & x() const { return _x; }
	//-------------------------------------------------------------------------
	Unit & x() { return _x; }
	//-------------------------------------------------------------------------
	void x(const Unit &v) { _x = v; }
	//-------------------------------------------------------------------------
	const Unit & y() const { return _y; }
	//-------------------------------------------------------------------------
	Unit & y() { return _y; }
	//-------------------------------------------------------------------------
	void y(const Unit &v) { _y = v; }
	//-------------------------------------------------------------------------
	sambag::disco::Point2D solve(IDrawContext::Ptr cn) const;
};
//=============================================================================
class Dimension {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	Point dimension;
public:
	//-------------------------------------------------------------------------
	const Unit & width() const { return dimension.x(); }
	//-------------------------------------------------------------------------
	Unit & width() { return dimension.x(); }
	//-------------------------------------------------------------------------
	void width(const Unit &v) { dimension.x(v); }
	//-------------------------------------------------------------------------
	const Unit & height() const { return dimension.y(); }
	//-------------------------------------------------------------------------
	Unit & height() { return dimension.y(); }
	//-------------------------------------------------------------------------
	void height(const Unit &v) { dimension.y(v); }
	//-------------------------------------------------------------------------
	sambag::disco::Dimension solve(IDrawContext::Ptr cn) const;
};
//=============================================================================
class Rectangle {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	Point _x0;
	Dimension _size;
public:
	//-------------------------------------------------------------------------
	const Point & x0() const { return _x0; }
	//-------------------------------------------------------------------------
	Point & x0() { return _x0; }
	//-------------------------------------------------------------------------
	void x0(const Point &v) { _x0 = v; }
	//-------------------------------------------------------------------------
	const Dimension & size() const { return _size; }
	//-------------------------------------------------------------------------
	Dimension & size() { return _size; }
	//-------------------------------------------------------------------------
	void size(const Dimension &v) { _size = v; }
	//-------------------------------------------------------------------------
	sambag::disco::Rectangle solve(IDrawContext::Ptr cn) const;

};
}}}}
#endif /* SAMBAG_SVGUNITS_H */
