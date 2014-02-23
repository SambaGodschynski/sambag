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

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
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
	//-------------------------------------------------------------------------
	/**
	 * @brief all patterns with bounds (gradients) extents 0,0,100,100
	 *        this function transforms the context to the given rect
	 *        according to fit the pattern.
     */	
	void __fitPatternForFill(IDrawContext::Ptr cn, const Rectangle &r);
public:
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
	virtual Rectangle getBoundingBox(IDrawContext::Ptr context);
};

}}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
