/*
 * GraphicElement.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "GraphicElement.hpp"
#include <sambag/disco/IDiscoFactory.hpp>
namespace sambag { namespace disco { namespace svg { namespace graphicElements {

//=============================================================================
// class GraphicElement
//=============================================================================
//-----------------------------------------------------------------------------
GraphicElement::GraphicElement() {
}
//-----------------------------------------------------------------------------
GraphicElement::~GraphicElement() {
}
//-----------------------------------------------------------------------------
void GraphicElement::__fitPatternForFill(IDrawContext::Ptr cn, const Rectangle &b)
{
	//std::cout<<toString()<<b<<std::endl;	
	cn->translate(b.x0());
	cn->scale(Point2D(b.width()/100., 1.));
}
//-----------------------------------------------------------------------------
Rectangle GraphicElement::getBoundingBox(IDrawContext::Ptr context) {
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	Coordinate w = context->getStrokeWidth();
	cn->setStrokeWidth(w);
	cn->setStrokeColor( context->getStrokeColor() );
	cn->setFillColor( context->getFillColor() );
	Matrix m;
	context->getMatrix(m);
	cn->transform(m);
	draw(cn);
	return cn->getSurface()->getSize();
}
}}}} // namespaces
