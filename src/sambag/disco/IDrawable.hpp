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
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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
        typedef boost::weak_ptr<IDrawable> WPtr;
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr context) = 0;
	//-------------------------------------------------------------------------
	/**
	 * @note: not const  
	 */
	virtual Rectangle getBoundingBox(IDrawContext::Ptr context) = 0;
	//-------------------------------------------------------------------------
	/**
	 * @return the drawables name
	 */
	virtual std::string toString() const = 0;
};

}}

#endif /* IGRAPHICOBJECT_HPP_ */
