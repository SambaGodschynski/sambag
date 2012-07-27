/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef DISCO_ARC_HPP_
#define DISCO_ARC_HPP_

#include "GraphicElement.hpp"
#include <sambag/disco/svg/units/Units.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
class Arc : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Arc> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	units::Point c;
	//-------------------------------------------------------------------------
	units::Point r;
	//-------------------------------------------------------------------------
	Arc(){}
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
			return "Arc";
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
		Ptr neu(new Arc());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	const units::Point & getCenter() const { return c; }
	//-------------------------------------------------------------------------
	const units::Point & getRadius() const { return r; }
	//-------------------------------------------------------------------------
	units::Point & getCenter() { return c; }
	//-------------------------------------------------------------------------
	units::Point & getRadius() { return r; }
	//-------------------------------------------------------------------------
	void setCenter(const units::Point &_c)  { c = _c; }
	//-------------------------------------------------------------------------
	void setRadius(const units::Point &_r)  { r = _r; }
	//-------------------------------------------------------------------------
	virtual ~Arc(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox(IDrawContext::Ptr context) const ;
};

}}}} // namespace

#endif /* DISCO_CIRCLE_HPP_ */
