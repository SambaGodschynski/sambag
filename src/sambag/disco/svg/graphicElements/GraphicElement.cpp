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
