/*
 * Text.hpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "GraphicElement.hpp"
#include <sambag/disco/svg/units/Units.hpp>
#include <string>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
class Text : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Text> Ptr;
private:
	//-------------------------------------------------------------------------
	void drawPath(IDrawContext::Ptr cn);
protected:
	//-------------------------------------------------------------------------
	units::Point pos;
	//-------------------------------------------------------------------------
	sambag::disco::Path::Ptr storedPath;
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
	void setPos( const units::Point &p ) {
		pos = p;
	}
	//-------------------------------------------------------------------------
	const units::Point & getPos() const {
		return pos;
	}
	//-------------------------------------------------------------------------
	units::Point & getPos() {
		return pos;
	}
	//-------------------------------------------------------------------------
	const std::string & getText() const { return text; }
	//-------------------------------------------------------------------------
	void setText(const std::string &t)  {
		text = t;
		storedPath.reset();
	}
	//-------------------------------------------------------------------------
	virtual ~Text();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox(IDrawContext::Ptr cn) const {
		return Rectangle();
	}
};

}}}} // namespace

#endif /* TEXT_HPP_ */
