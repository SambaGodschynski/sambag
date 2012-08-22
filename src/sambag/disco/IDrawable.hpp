/*
 * IGraphicObject.hpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#ifndef DISCO_I_DRAWABLE_HPP
#define DISCO_I_DRAWABLE_HPP

#include "IDrawContext.hpp"
#include "sambag/com/Common.hpp"
#include "string"

namespace sambag { namespace disco {
//=============================================================================
/**
 * Abstract base class for all graphic elements.
 */
class IDrawable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IDrawable> Ptr;
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr context) = 0;
	//-------------------------------------------------------------------------
	/**
	 * @note: can't be const because the default impl. of 
	 * GraphicElement::getBoundingBox uses the unconst draw method().
	 * And making draw() is partially harder to realize than
	 * having getBoundingBox() unconst.
	 */
	virtual Rectangle getBoundingBox(IDrawContext::Ptr context) = 0;
	//-------------------------------------------------------------------------
	/**
	 * for testing and debugging
	 */
	virtual std::string toString() const = 0;
};

}}

#endif /* IGRAPHICOBJECT_HPP_ */
