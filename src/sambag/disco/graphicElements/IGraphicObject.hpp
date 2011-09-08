/*
 * IGraphicObject.hpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#ifndef IGRAPHICOBJECT_HPP_
#define IGRAPHICOBJECT_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// Abstract base class for all graphic elements.
class IGraphicObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IGraphicObject> Ptr;
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const = 0;
};

}}}

#endif /* IGRAPHICOBJECT_HPP_ */
