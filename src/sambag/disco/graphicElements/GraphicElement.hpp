/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef GRAPHICELEMENT_HPP_
#define GRAPHICELEMENT_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "Style.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// Abstract base class for all graphic elements.
class GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<GraphicElement> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	Style style;
	//-------------------------------------------------------------------------
	GraphicElement();
	//-------------------------------------------------------------------------
	boost::weak_ptr<GraphicElement> __self;
	//-------------------------------------------------------------------------
	void __setSelf( Ptr self ) { __self = self; }
public:
	//-------------------------------------------------------------------------
	const Style & getStyle() const { return style; }
	//-------------------------------------------------------------------------
	void setStyle( const Style &_style ) { style = _style; }
	//-------------------------------------------------------------------------
	virtual ~GraphicElement();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const = 0;
};

}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
