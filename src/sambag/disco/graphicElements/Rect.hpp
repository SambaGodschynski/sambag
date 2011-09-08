/*
 * Rect.hpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#ifndef RECT_HPP_
#define RECT_HPP_

#include "GraphicElement.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
class Rect : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Rect> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	Rectangle rect;
	//-------------------------------------------------------------------------
	Rect( const Rectangle &rect=NULL_RECTANGLE );
public:
	//-------------------------------------------------------------------------
	static Ptr create( const Rectangle &rect=NULL_RECTANGLE )
	{
		Ptr neu(new Rect(rect));
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	const Rectangle & getRectangle() const { return rect; }
	//-------------------------------------------------------------------------
	void setRectangle(const Rectangle &r)  { rect = r; }
	//-------------------------------------------------------------------------
	virtual ~Rect();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		return rect;
	}
};

}}} // namespace


#endif /* RECT_HPP_ */
