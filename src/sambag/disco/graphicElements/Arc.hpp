/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef DISCO_ARC_HPP_
#define DISCO_ARC_HPP_

#include "GraphicElement.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
class Arc : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Arc> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	Point2D c;
	//-------------------------------------------------------------------------
	Point2D r;
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
	const Point2D & getCenter() const { return c; }
	//-------------------------------------------------------------------------
	const Point2D & getRadius() const { return r; }
	//-------------------------------------------------------------------------
	void setCenter(const Point2D &_c)  { c = _c; }
	//-------------------------------------------------------------------------
	void setRadius(const Point2D &_r)  { r = _r; }
	//-------------------------------------------------------------------------
	virtual ~Arc(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const ;
};

}}} // namespace

#endif /* DISCO_CIRCLE_HPP_ */
