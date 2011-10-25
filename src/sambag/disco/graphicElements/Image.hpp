/*
 * Image.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include "GraphicElement.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
class Image : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Image> Ptr;
private:
	//-------------------------------------------------------------------------
	void loadImage(IDrawContext::Ptr cn);
	//-------------------------------------------------------------------------
	void drawPlaceholder(IDrawContext::Ptr cn);
protected:
	//-------------------------------------------------------------------------
	ISurface::Ptr image;
	//-------------------------------------------------------------------------
	Rectangle outline;
	//-------------------------------------------------------------------------
	std::string uri;
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
		return "Image[" + uri + "]";
	}
	//-------------------------------------------------------------------------
	void setUri(const std::string &_uri) { uri = _uri; }
	//-------------------------------------------------------------------------
	const std::string & getUri() const { return uri; }
	//-------------------------------------------------------------------------
	virtual GraphicElement::Ptr clone() const {
		Ptr neu = create();
		*neu = *this;
		return neu;
	}
	//-------------------------------------------------------------------------
	static Ptr create()
	{
		Ptr neu(new Image());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 * @return representing rectangle of image.
	 * Its the same like @see getBoundingBox
	 */
	const Rectangle & getOutline() const { return outline; }
	//-------------------------------------------------------------------------
	void setOutline(const Rectangle &out)  { outline = out; }
	//-------------------------------------------------------------------------
	virtual ~Image(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		return outline;
	}
};

}}} // namespace

#endif /* IMAGE_HPP_ */
