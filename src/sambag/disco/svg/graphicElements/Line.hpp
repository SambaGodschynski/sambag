/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef DISCO_LINE_HPP_
#define DISCO_LINE_HPP_

#include "GraphicElement.hpp"
#include <sambag/disco/svg/units/Units.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
class Line : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Line> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	units::Point p0, p1;
	//-------------------------------------------------------------------------
	Line();
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
			return "Line";
	}
	//-------------------------------------------------------------------------
	virtual GraphicElement::Ptr clone() const {
		Ptr neu = create();
		*neu = *this;
		return neu;
	}
	//-------------------------------------------------------------------------
	static Ptr create()
	{
		Ptr neu(new Line());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	const units::Point & getP0() const { return p0; }
	//-------------------------------------------------------------------------
	const units::Point & getP1() const { return p1; }
	//-------------------------------------------------------------------------
	units::Point & getP0() { return p0; }
	//-------------------------------------------------------------------------
	units::Point & getP1() { return p1; }
	//-------------------------------------------------------------------------
	void setP0(const units::Point &p)  { p0 = p; }
	//-------------------------------------------------------------------------
	void setP1(const units::Point &p)  { p1 = p; }
	//-------------------------------------------------------------------------
	virtual ~Line();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
};

}}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
