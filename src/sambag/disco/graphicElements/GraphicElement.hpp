/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef GRAPHICELEMENT_HPP_
#define GRAPHICELEMENT_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/com/Common.hpp"
#include "sambag/disco/IDrawable.hpp"
#include "sambag/disco/Coordinate.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// Abstract base class for all graphic elements.
class GraphicElement : public IDrawable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<GraphicElement> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	GraphicElement();
	//-------------------------------------------------------------------------
	boost::weak_ptr<GraphicElement> __self;
	//-------------------------------------------------------------------------
	void __setSelf( Ptr self ) { __self = self; }
public:
	//-------------------------------------------------------------------------
	// TODO: draw path method. which saves the path data on first call
	// and restores saved data on the others.
	//-------------------------------------------------------------------------
	std::string toString() const = 0;
	//-------------------------------------------------------------------------
	virtual Ptr clone() const = 0;
	//-------------------------------------------------------------------------
	virtual ~GraphicElement();
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return __self.lock();
	}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const = 0;
};

}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
