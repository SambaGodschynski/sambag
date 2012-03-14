/*
 * Text.hpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "GraphicElement.hpp"
#include <string>

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
class Text : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Text> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	Point2D pos;
	//-------------------------------------------------------------------------
	std::string text;
	//-------------------------------------------------------------------------
	Text();
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
		return "Text[" + text + "]";
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
		Ptr neu(new Text());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	void setPos( const Point2D &p ) {
		pos = p;
	}
	const Point2D & getPos() const {
		return pos;
	}
	//-------------------------------------------------------------------------
	const std::string & getText() const { return text; }
	//-------------------------------------------------------------------------
	void setText(const std::string &t)  { text = t; }
	//-------------------------------------------------------------------------
	virtual ~Text();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		return Rectangle();
	}
};

}}} // namespace

#endif /* TEXT_HPP_ */
