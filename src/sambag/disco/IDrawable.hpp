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
	// TODO: make const
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const = 0;
};

}}

#endif /* IGRAPHICOBJECT_HPP_ */
