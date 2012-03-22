/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef DISCO_LINE_HPP_
#define DISCO_LINE_HPP_

#include "GraphicElement.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
class Line : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Line> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	Point2D p0, p1;
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
	const Point2D & getP0() const { return p0; }
	//-------------------------------------------------------------------------
	const Point2D & getP1() const { return p1; }
	//-------------------------------------------------------------------------
	void setP0(const Point2D &p)  { p0 = p; }
	//-------------------------------------------------------------------------
	void setP1(const Point2D &p)  { p1 = p; }
	//-------------------------------------------------------------------------
	virtual ~Line();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		return Rectangle(p0, p1);
	}
};

}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
