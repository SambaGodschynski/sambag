/*
 * GraphicElement.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "GraphicElement.hpp"

namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
// class GraphicElement
//=============================================================================
//-----------------------------------------------------------------------------
GraphicElement::GraphicElement() {
	using namespace boost::numeric::ublas;
	tMatrix = IDENTITY_MATRIX;
}
//-----------------------------------------------------------------------------
GraphicElement::~GraphicElement() {
}
}}} // namespaces
