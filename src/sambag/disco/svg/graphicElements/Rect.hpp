/*
 * Rect.hpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#ifndef RECT_HPP_
#define RECT_HPP_

#include "GraphicElement.hpp"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
class Rect : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Rect> Ptr;
private:
	//-------------------------------------------------------------------------
	void _rect(IDrawContext::Ptr context);
protected:
	//-------------------------------------------------------------------------
	Rectangle rect;
	//-------------------------------------------------------------------------
	Point2D r; // roudned rect radius
	//-------------------------------------------------------------------------
	Rect();
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
		return "Rect";
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
		Ptr neu(new Rect());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	const Rectangle & getRectangle() const { return rect; }
	//-------------------------------------------------------------------------
	void setRectangle(const Rectangle &r)  { rect = r; }
	//-------------------------------------------------------------------------
	const Point2D & getRadius() const { return r; }
	//-------------------------------------------------------------------------
	void setRadius(const Point2D &_r)  { r = _r; }
	//-------------------------------------------------------------------------
	virtual ~Rect();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		return rect;
	}
};

}}}} // namespace


#endif /* RECT_HPP_ */
